#include "Buttplug/Device.hpp"
#include "Buttplug/Client.hpp"

using namespace Buttplug;

bool check_ok(Messages::Message* message) {
    if(message->Name() == "Ok") {
        return true;
    } else if(message->Name() == "Error") {
        auto err = static_cast<Messages::Error*>(message);
        printf("Error: %s\n", err->ErrorMessage.c_str());
        return false;
    } else {
        printf("Unexpected Reply: %s\n", message->Name().c_str());
        return false;
    }
}

Device* Device::from_json(json& j) {
    auto e = new Device;
    j.at("DeviceName").get_to(e->DeviceName);
    j.at("DeviceIndex").get_to(e->DeviceIndex);
    if(j.contains("DeviceMessageGap")) {
        j.at("DeviceMessageGap").get_to(e->DeviceMessageGap);
    } else {
        e->DeviceMessageGap = 0;
    }
    if(j.contains("DeviceDisplayName")) {
        j.at("DeviceDisplayName").get_to(e->DeviceDisplayName);
    } else {
        e->DeviceDisplayName = e->DeviceName;
    }

    auto m = j.at("DeviceMessages");
    for(auto it = m.begin(); it != m.end(); it++) {
        auto key = it.key();
        auto a = m.at(key);
        if(key == "ScalarCmd") {
            int i = 0;
            for (auto& element : a) {
                auto desc = element.at("FeatureDescriptor").get<std::string>();
                auto actuator_type = element.at("ActuatorType").get<std::string>();
                auto step_count =element.at("StepCount").get<int>();
                e->ScalarActuators.push_back(ScalarActuator(e, i, desc, actuator_type, step_count));
                i++;
            }
        }
    }

    return e;
}

Messages::Incoming* Device::Send(Messages::Outgoing& message, bool wait) {
    return this->client->Send(message, wait);
}

void Device::Stop() {
    auto m = Messages::StopDeviceCmd(this->DeviceIndex);
    this->client->Send(m, false);
}


void ScalarActuator::Command(float scalar) {
    auto m = Messages::ScalarCmd(
        this->device->DeviceIndex,
        { Messages::_Scalar(this->index, scalar, this->ActuatorType), }
    );
    this->device->Send(m, false);
}
