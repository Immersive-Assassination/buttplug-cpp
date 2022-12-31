#pragma once

#include <string>
#include <functional>
#include <condition_variable>
#include <thread>

#ifndef WSCONN
class ws_client;
struct ws_message_ptr;
#endif
typedef std::weak_ptr<void> ws_connection_hdl;

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
        ws_client* _client;
		ws_connection_hdl _connection;
        std::shared_ptr<std::thread> _thread;

        void on_message(ws_connection_hdl hdl, ws_message_ptr msg);
    };
}
