//
// Created by maros on 19/02/2022.
//

#include "ble.hpp"
#include <HardwareSerial.h>

void ble::device::init() {
    this->network.acquireBuffer();
}

void ble::device::term() {
    this->network.releaseBuffer();
}

ble::device::device(HardwareSerial& serial)
: network(serial) {

}

void ble::network::clearBuffer() {
    memset(this->buffer, 0, this->buffer_size);
    this->buffer_cursor = 0;
}

void ble::network::acquireBuffer() {
    this->buffer_cursor = 0;
    this->buffer_size = RX_PKT_BUFFER_SIZE;
    this->buffer = new uint8_t[this->buffer_size];

    this->clearBuffer();
}

void ble::network::releaseBuffer() {
    delete[] this->buffer;
}

ble::packet *ble::network::recvLoop() {
    ble::packet *pkt = nullptr;
    if (this->buffer == nullptr) {
        Serial.println("Buffer unavailable.");
        return pkt;
    }

    auto bytesAvailable = Serial.available();
    if (!bytesAvailable)
        return pkt;

    for (int i = 0; i < bytesAvailable; ++i) {
        auto byte = Serial.read();
        switch (byte) {
            case PACKET_STX: {
                // packet begin
                this->packet_begin = true;
            }
                break;
            case PACKET_ETX: {
                // packet end
                this->packet_begin = false;

                // cpy buffer
                {
                    auto *pkt_buffer = new uint8_t[this->buffer_cursor+1];
                    memcpy(pkt_buffer, this->buffer, this->buffer_cursor+1);

                    pkt = new ble::packet(this->buffer_cursor+1,
                                          pkt_buffer);
                }

                // clear packet buffer
                this->clearBuffer();
            }
                break;
            default: {
                if (!this->packet_begin)
                    break;
                // packet byte
                this->buffer[this->buffer_cursor++] = byte;
            }
                break;
        }
    }
    return pkt;
}

void ble::network::sendLoop() {

}
