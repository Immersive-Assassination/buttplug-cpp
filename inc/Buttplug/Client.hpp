#pragma once

#include <string>
#include <map>
#include <condition_variable>

#include "Buttplug/Connector.hpp"
#include "Buttplug/Device.hpp"
#include "Buttplug/Messages.hpp"


namespace Buttplug {
    class _reply_wait;

    class Client {
    public:
        Client(std::string name);

        Messages::Incoming* Send(Messages::Outgoing& message, bool wait = true);
        void Connect(Connector* connector);
        inline void Connected() {
            return connector != nullptr && connector->Connected();
        }
        void Disconnect();
        void StartScanning();
        void StopScanning();
        void StopAll();

        std::map<int, Device*> Devices;
        Connector* connector;
    private:
        void _handle_message(std::string message);

        std::string _name;
        int _last_id;
        std::map<int, _reply_wait*> _waiting;
    };
}