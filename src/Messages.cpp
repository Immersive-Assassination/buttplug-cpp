#include "Buttplug/Messages.hpp"
#include "Buttplug/Device.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Buttplug::Messages::Outgoing::serialize(json& j) {
    j[this->Name()] = json::object();
    this->inner_json(j[this->Name()]);
}


Buttplug::Messages::DeviceList* Buttplug::Messages::DeviceList::from_json(json& j) {
    auto e = new Buttplug::Messages::DeviceList;
    j.at("Id").get_to(e->Id);
    for(auto&& dev : j.at("Devices")) {
        e->Devices.push_back(Buttplug::Device::from_json(dev));
    }
    return e;
}

Buttplug::Messages::DeviceAdded* Buttplug::Messages::DeviceAdded::from_json(json& j) {
    auto e = new Buttplug::Messages::DeviceAdded;
    e->Device = Buttplug::Device::from_json(j);
    return e;
}


template<typename T> Buttplug::Messages::Incoming* create_from_json(json& j) {
    return T::from_json(j);
}

#define MM(x) {#x, &create_from_json<Buttplug::Messages::x>}

const std::map<std::string, std::function<Buttplug::Messages::Incoming*(json&)>> Buttplug::Messages::_message_map {
    MM(Ok),
    MM(Error),
    MM(ServerInfo),
    MM(DeviceList),
    MM(DeviceAdded),
    MM(DeviceRemoved),
};
