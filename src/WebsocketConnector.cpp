#include "Buttplug/Connector.hpp"

using namespace Buttplug;


void Buttplug::WebsocketConnector::on_message(websocketpp::connection_hdl hdl, ws_message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    if(this->_callback != nullptr) this->_callback(msg->get_payload());
}

WebsocketConnector::WebsocketConnector(std::string address) {
    _address = address;
    try {
        _client.set_access_channels(websocketpp::log::alevel::all);
        _client.clear_access_channels(websocketpp::log::alevel::frame_payload);
        _client.set_error_channels(websocketpp::log::elevel::all);
        _client.init_asio();
        _client.set_message_handler(std::bind(&WebsocketConnector::on_message, this, std::placeholders::_1, std::placeholders::_2));
        _client.start_perpetual();

        _thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&ws_client::run, &_client);
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

WebsocketConnector::~WebsocketConnector() {
    _client.stop_perpetual();
    this->Disconnect();
    _thread->join();
}

bool WebsocketConnector::Connect() {
    std::cout << "Connecting to: " << _address << std::endl;
    try {
        websocketpp::lib::error_code ec;
        ws_client::connection_ptr con = _client.get_connection(_address, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return false;
        }
        _connection = con->get_handle();
        
        std::condition_variable cv;
        std::mutex cv_m;
        std::unique_lock<std::mutex> lk(cv_m);

        bool err = false;
        
        con->set_open_handler([&cv](websocketpp::connection_hdl hdl) -> void {
            cv.notify_all();
        });

        con->set_fail_handler([&cv, &err](websocketpp::connection_hdl hdl) -> void {
            err = true;
            cv.notify_all();
        });

        _client.connect(con);
        cv.wait(lk);

        return !err;
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

bool WebsocketConnector::Disconnect() {
    websocketpp::lib::error_code ec;
    auto con = _client.get_con_from_hdl(_connection, ec);
    if(ec) {
        std::cout << "could not disconnect because: " << ec.message() << std::endl;
        return false;
    }

    _client.close(_connection, websocketpp::close::status::going_away, "", ec);
    if (ec) {
        std::cout << "> Error closing connection: " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool WebsocketConnector::Send(std::string message) {
    websocketpp::lib::error_code ec;
    _client.send(_connection, message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool WebsocketConnector::Connected() {
    websocketpp::lib::error_code ec;
    auto con = _client.get_con_from_hdl(_connection, ec);
    if(ec) {
        return false;
    }
    return true;
}