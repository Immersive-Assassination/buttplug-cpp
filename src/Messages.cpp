#include "Buttplug/Device.hpp"
#include "Buttplug/Messages.hpp"

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

void Outgoing::inner_json(json& j) {
	j = json{
		{"Id", this->Id},
	};
}

Error* Error::from_json(json& j) {
    auto e = new Error;
    j.at("Id").get_to(e->Id);
    j.at("ErrorMessage").get_to(e->ErrorMessage);
    j.at("ErrorCode").get_to(e->ErrorCode);
    return e;
}

void RequestServerInfo::inner_json(json& j) {
	j = json{
		{"Id", this->Id},
		{"ClientName", this->ClientName},
		{"MessageVersion", this->MessageVersion},
	};
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
    e->_Device = Buttplug::Device::from_json(j);
    return e;
}

DeviceRemoved* DeviceRemoved::from_json(json& j) {
    auto e = new DeviceRemoved;
    j.at("Id").get_to(e->Id);
    j.at("DeviceIndex").get_to(e->DeviceIndex);
    return e;
}

void StopDeviceCmd::inner_json(json& j) {
	j = json{
		{"Id", this->Id},
		{"DeviceIndex", this->DeviceIndex},
	};
}

void ScalarCmd::inner_json(json& j) {
	j = json{
		{"Id", this->Id},
		{"DeviceIndex", this->DeviceIndex},
		{"Scalars", this->Scalars},
	};
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
