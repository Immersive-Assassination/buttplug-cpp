#include "Buttplug/DeviceParts.hpp"
#include "Buttplug/Messages.hpp"
#include "Buttplug/Device.hpp"

using namespace Buttplug;

void ScalarActuator::Command(float scalar) {
    auto m = Messages::ScalarCmd(
        this->device->DeviceIndex,
        { Messages::_Scalar(this->index, scalar, this->ActuatorType), }
    );
    this->device->Send(m, false);
}
