#pragma once

#include <vector>
#include <string>

#include "Buttplug/Messages.hpp"



namespace Buttplug {
    class Client;

    class Actuator;
    class LinearActuator;
    class RotatoryActuator;
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

        std::vector<ScalarActuator> ScalarActuators;
        std::vector<LinearActuator> LinearActuators;
        std::vector<RotatoryActuator> RotatryActuators;
        //std::vector<Sensor> _sensors;
    };
}
