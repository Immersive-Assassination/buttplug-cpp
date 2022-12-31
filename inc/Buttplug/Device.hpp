#pragma once

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


namespace Buttplug {
    class Client;

    class Actuator;
    class ScalarActuator;
    class LinearActuator;
    class RotatoryActuator;

    namespace Messages {
        class Incoming;
        class Outgoing;
    }
    //class Sensor;

    class Device {
    public:
        static Device* from_json(json& j);

        Messages::Incoming* Send(Messages::Outgoing& message, bool wait = true);
        void Stop();

        Client* client;
        std::string DeviceName;
        int DeviceIndex;
        int DeviceMessageGap;
        std::string DeviceDisplayName;

        std::vector<ScalarActuator*> ScalarActuators;
        std::vector<LinearActuator*> LinearActuators;
        std::vector<RotatoryActuator*> RotatryActuators;
        //std::vector<Sensor> _sensors;
    };
}
