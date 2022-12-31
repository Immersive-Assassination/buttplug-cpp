#include <string>


namespace Buttplug {
    class Device;

    class DevicePart {
    public:
        DevicePart(Device* device, int index, std::string description) :
            device(device), index(index), description(description) {};
        Device* device;
        int index;
        std::string description;

        friend void from_json(const json& j, DevicePart*) {

        }

    };

    class Actuator : public DevicePart {
    public:
        Actuator(Device* device, int index, std::string description, int step_count = -1) :
            DevicePart(device, index, description), _step_count(step_count) {};
    private:
        int _step_count;
    };

    class SingleMotorVibrateActuator : public Actuator {
    public:
        SingleMotorVibrateActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(float speed);
    };

    class KiirooActuator : public Actuator {
    public:
        KiirooActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(std::string command);
    };

    class FleshlightLaunchFW12Actuator : public Actuator {
    public:
        FleshlightLaunchFW12Actuator(Device* device, int index);
        void Command(int position, int speed);
    };

    class LovenseActuator : public Actuator {
    public:
        LovenseActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(std::string command);
    };

    class VorzeA10CycloneActuator : public Actuator {
    public:
        VorzeA10CycloneActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(int speed, bool clockwise);
    };

    class VibrateActuator : public Actuator {
    public:
        VibrateActuator(Device* device, int index, int step_count) :
            Actuator(device, index, "", step_count) {};
        void Command(float speed);
    };

    class LinearActuator : public Actuator {
    public:
        LinearActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(int duration, float position);
    };

    class RotatoryActuator : public Actuator {
    public:
        RotatoryActuator(Device* device, int index) :
            Actuator(device, index, "") {};
        void Command(float speed, bool clockwise);
    };

    class ScalarActuator : public Actuator {
    public:
        ScalarActuator(Device* device, int index, std::string description, std::string ActuatorType, int step_count) :
            Actuator(device, index, description, step_count), ActuatorType(ActuatorType) {};
        void Command(float scalar);
        
        std::string ActuatorType;
    };


    const extern std::map<std::string, std::function<DevicePart*(json&)>> _device_messages;
}
