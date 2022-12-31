#include "Buttplug/Messages.hpp"
#include "Buttplug/Device.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace Buttplug::Messages;


Incoming* Incoming::from_json(json& j) {
    auto e = new Incoming;
    j.at("Id").get_to(e->Id);
    return e;
}

void Outgoing::serialize(json& j) {
    j[this->Name()] = json::object();
    this->inner_json(j[this->Name()]);
}

Error* Error::from_json(json& j) {
    auto e = new Error;
    j.at("Id").get_to(e->Id);
    j.at("ErrorMessage").get_to(e->ErrorMessage);
    j.at("ErrorCode").get_to(e->ErrorCode);
    return e;
}

ServerInfo* ServerInfo::from_json(json& j) {
    auto e = new ServerInfo;
    j.at("Id").get_to(e->Id);
    j.at("ServerName").get_to(e->ServerName);
    j.at("MessageVersion").get_to(e->MessageVersion);
    j.at("MaxPingTime").get_to(e->MaxPingTime);
    return e;
}

DeviceList* DeviceList::from_json(json& j) {
    auto e = new DeviceList;
    j.at("Id").get_to(e->Id);
    for(auto&& dev : j.at("Devices")) {
        e->Devices.push_back(Buttplug::Device::from_json(dev));
    }
    return e;
}

DeviceAdded* DeviceAdded::from_json(json& j) {
    auto e = new DeviceAdded;
    e->Device = Buttplug::Device::from_json(j);
    return e;
}

DeviceRemoved* DeviceRemoved::from_json(json& j) {
    auto e = new DeviceRemoved;
    j.at("Id").get_to(e->Id);
    j.at("DeviceIndex").get_to(e->DeviceIndex);
    return e;
}


#define MM(x) {#x, x::from_json}

const std::map<std::string, std::function<Incoming*(json&)>> Buttplug::Messages::_message_map {
    MM(Ok),
    MM(Error),
    MM(ServerInfo),
    MM(DeviceList),
    MM(DeviceAdded),
    MM(DeviceRemoved),
};
