#include "Buttplug/DeviceParts.hpp"

void ScalarActuator::Command(float scalar) {
    auto m = Messages::ScalarCmd(
        this->device->DeviceIndex,
        { Messages::_Scalar(this->index, scalar, this->ActuatorType), }
    );
    this->device->Send(m, false);
}
