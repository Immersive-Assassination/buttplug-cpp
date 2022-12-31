#include <Buttplug/Client.hpp>


int main() {
    auto connector = new Buttplug::WebsocketConnector("ws://127.0.0.1:12345");
    
    Buttplug::Client client("cpp-test");
    client.Connect(connector);

    client.StartScanning();

    float val = 0;
    while(true) {
        val += 0.2;
        if(val > 1) val = 0;

        for(auto&& it : client.Devices) {
            auto dev = it.second;
            dev->ScalarActuators[0].Command(val);
        }
        sleep(1);
    }
}
