#include <Arduino.h>
#include <ble/ble.hpp>

ble::device device{Serial};

void setup() {
    Serial.begin(9600);
    Serial.println("Serial begin.");

    device.init();
    Serial.println("Device init.");

    {
        pinMode(3, OUTPUT);
        Serial.println("Pin test.");
        analogWrite(3, 255);
        delay(1000);
        Serial.println("Pin test over.");
        analogWrite(3, 0);
    }
}

void loop() {
    auto *pkt = device.network.recvLoop();
    if (pkt != nullptr) {
        int16_t brightness = max(min(pkt->buffer[0] - 0x61, 25), 0);
        analogWrite(3, map(brightness, 0, 25, 0, 255));
    }

    device.network.sendLoop();
}