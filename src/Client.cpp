#include "Buttplug/Client.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Buttplug {
    class _reply_wait {
    public:
        _reply_wait(int Id) : Id(Id), lk(cv_m), reply(nullptr) {}

        inline Messages::Incoming* Wait() {
            if(reply == nullptr) {
                cv.wait(lk);
            };
            return reply;
        }

        inline void Finish(Messages::Incoming* reply) {
            this->reply = reply;
            cv.notify_all();
        }

        int Id;
    private:
        std::condition_variable cv;
        std::mutex cv_m;
        std::unique_lock<std::mutex> lk;
        Messages::Incoming* reply;
    };


    Client::Client(std::string name)  {
        _name = name;
        _last_id = 0;
    }

    void Client::Connect(Connector* connector) {
        if(this->connector) {
            this->Disconnect();
        }
        this->connector = connector;
        this->connector->_callback = std::bind(&Client::_handle_message, this, std::placeholders::_1);
        this->connector->Connect();

        auto m = Messages::RequestServerInfo(this->_name, 3);
        auto server_info = static_cast<Messages::ServerInfo*>(this->Send(m));

        auto m2 = Messages::RequestDeviceList();
        auto device_list = static_cast<Messages::DeviceList*>(this->Send(m2));
        this->Devices.clear();
        for(auto&& dev : device_list->Devices) {
            dev->client = this;
            this->Devices[dev->DeviceIndex] = dev;
        }
    }

    void Client::Disconnect() {
        this->connector->Disconnect();
        delete this->connector;
        this->connector = nullptr;
    }

    void Client::StartScanning() {
        auto m = Messages::StartScanning();
        this->Send(m);
    }

    void Client::StopScanning() {
        auto m = Messages::StopScanning();
        this->Send(m);
    }

    void Client::StopAll() {
        auto m = Messages::StopAllDevices();
        this->Send(m);
    }

    void Client::_handle_message(std::string message_str) {
        auto j = json::parse(message_str);
        printf("received: %s\n", message_str.c_str());
        for (auto& element : j) {
            for(auto it = element.begin(); it != element.end(); it++) {
                auto key = it.key();
                auto _m = Messages::_message_map.at(key);
                if(_m == nullptr) {
                    printf("Message Type Unknown %s\n", key.c_str());
                    continue;
                }
                auto m = _m(it.value());

                if(m->Id == 0) {
                    // server sent event
                    if(m->Name() == "DeviceAdded") {
                        auto add = static_cast<Messages::DeviceAdded*>(m);
                        auto dev = add->Device;
                        bool exists = Devices.find(dev->DeviceIndex) != Devices.end();
                        if(exists) {
                            delete Devices.at(dev->DeviceIndex);
                        }
                        dev->client = this;
                        Devices[dev->DeviceIndex] = dev;
                    } else if(m->Name() == "DeviceRemoved") {
                        auto del = static_cast<Messages::DeviceRemoved*>(m);
                        Devices.erase(del->Id);
                    }
                } else {
                    auto w = this->_waiting[m->Id];
                    if(w == nullptr) {
                        printf("Unexpected Message %s %d\n", m->Name().c_str(), m->Id);
                        continue;
                    }
                    w->Finish(m);
                    this->_waiting.erase(m->Id);
                }
            }
        }
    }

    Messages::Incoming* Client::Send(Messages::Outgoing& message, bool wait) {
        json j = json::array();
        json j2 = json::object();
        message.Id = ++_last_id;
        message.serialize(j2);
        j.push_back(j2);

        auto s = j.dump();
        printf("sending: %s\n", s.c_str());

        auto w = new _reply_wait(message.Id);
        _waiting[message.Id] = w;
        this->connector->Send(s);
        if(wait) {
            auto reply = w->Wait();
            return reply;
        } else {
            return nullptr;
        }
    }
}
