#pragma once

#include <string>
#include <functional>
#include <condition_variable>
#include <thread>

#define ASIO_STANDALONE
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;
typedef websocketpp::config::asio_client::message_type::ptr ws_message_ptr;

namespace Buttplug {
    class Connector {
    public:
        virtual ~Connector() {};
        virtual bool Connect() = 0;
        virtual bool Disconnect() = 0;
        virtual bool Send(std::string message) = 0;
        virtual bool Connected() = 0;

        std::function<void(std::string)> _callback;
    };

    class WebsocketConnector : public Connector {
    public:
        WebsocketConnector(std::string address);
        virtual ~WebsocketConnector();
        virtual bool Connect();
        virtual bool Disconnect();
        virtual bool Send(std::string message);
        virtual bool Connected();
    private:
        std::string _address;
        ws_client _client;
        websocketpp::connection_hdl _connection;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> _thread;

        void on_message(websocketpp::connection_hdl hdl, ws_message_ptr msg);
    };
}
