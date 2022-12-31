#pragma once

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Buttplug/DeviceParts.hpp"


namespace Buttplug {
    enum class ErrorCode {
        ERROR_UNKNOWN,
        ERROR_INIT,
        ERROR_PING,
        ERROR_MSG,
        ERROR_DEVICE,
    };

    namespace Messages {
        #define M(x) virtual std::string Name() { return x; }
        class Message {
        public:
            M("Message");
            int Id;
        };

        class Incoming : public Message {
        public:
            static Incoming* from_json(json& j);
        };

        class Outgoing : public Message {
        public:
            void serialize(json& j);
        private:
            virtual void inner_json(json& j) {
                j = json{
                    {"Id", this->Id},
                };
            };
        };


        class Ok : public Incoming {
        public:
            Ok() {};
            M("Ok");
        };

        class Error : public Incoming {
        public:
            Error() {};
            M("Error");

            std::string ErrorMessage;
            ErrorCode ErrorCode;

            static Error* from_json(json& j);
        };

        class RequestServerInfo : public Outgoing {
        public:
            RequestServerInfo(std::string ClientName, int MessageVersion) :
                ClientName(ClientName), MessageVersion(MessageVersion) {};
            M("RequestServerInfo");
           
            std::string ClientName;
            int MessageVersion;

            void inner_json(json& j) {
                j = json{
                    {"Id", this->Id},
                    {"ClientName", this->ClientName},
                    {"MessageVersion", this->MessageVersion},
                };
            }
        };

        class ServerInfo : public Incoming {
        public:
            ServerInfo() {};
            M("ServerInfo");

            std::string ServerName;
            int MessageVersion;
            int MaxPingTime;

            static ServerInfo* from_json(json& j);
        };


        class StartScanning : public Outgoing {
        public:
            StartScanning() : Outgoing() {};
            M("StartScanning");
        };

        class StopScanning : public Outgoing {
        public:
            StopScanning() {};
            M("StopScanning");
        };

        class ScanningFinished : public Outgoing {
        public:
            ScanningFinished() {};
            M("ScanningFinished");
        };
        
        class RequestDeviceList : public Outgoing {
        public:
            RequestDeviceList() {};
            M("RequestDeviceList");
        };

        class DeviceList : public Incoming {
        public:
            DeviceList() {};
            M("DeviceList");
           
            std::vector<Device*> Devices;

            static DeviceList* from_json(json& j);
        };

        class DeviceAdded : public Incoming {
        public:
            DeviceAdded() {};
            M("DeviceAdded");
           
            Device* Device;
            static DeviceAdded* from_json(json& j);
        };

        class DeviceRemoved : public Incoming {
        public:
            DeviceRemoved() {};
            M("DeviceRemoved");

            int DeviceIndex;

            static DeviceRemoved* from_json(json& j);
        };


        class StopDeviceCmd : public Outgoing {
        public:
            StopDeviceCmd(int index) : DeviceIndex(index) {};
            M("StopDeviceCmd");
           
            int DeviceIndex;

            void inner_json(json& j) {
                j = json{
                    {"Id", this->Id},
                    {"DeviceIndex", this->DeviceIndex},
                };
            }
        };

        class StopAllDevices : public Outgoing {
        public:
            M("StopAllDevices");
        };

        class _Scalar {
        public:
            _Scalar(int Index, float Scalar, std::string ActuatorType) :
                Index(Index), Scalar(Scalar), ActuatorType(ActuatorType) {};
            int Index;
            float Scalar;
            std::string ActuatorType;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Scalar, Index, Scalar, ActuatorType)
        };

        class ScalarCmd : public Outgoing {
        public:
            M("ScalarCmd");
            ScalarCmd(int DeviceIndex, std::vector<_Scalar> Scalars) :
                DeviceIndex(DeviceIndex), Scalars(Scalars) {};

            int DeviceIndex;
            std::vector<_Scalar> Scalars;

            void inner_json(json& j) {
                j = json{
                    {"Id", this->Id},
                    {"DeviceIndex", this->DeviceIndex},
                    {"Scalars", this->Scalars},
                };
            }
        };

        const extern std::map<std::string, std::function<Buttplug::Messages::Incoming*(json&)>> _message_map;
    }
}



