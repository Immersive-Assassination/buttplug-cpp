#include <Buttplug/Client.hpp>
#include <unistd.h>


int main() {
    Buttplug::Client client("cpp-test");
    client.Connect(new Buttplug::WebsocketConnector("ws://127.0.0.1:12345"));

    client.StartScanning();

    float val = 0;
    while(true) {
        val += 0.1;
        if(val > 1) val = 0;

        for(auto&& it : client.Devices) {
            auto dev = it.second;
            dev->ScalarActuators[0].Command(val);
        }
        usleep(1000*500);
    }
}
