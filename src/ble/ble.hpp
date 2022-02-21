//
// Created by maros on 19/02/2022.
//

#ifndef UNTITLED_BLE_HPP
#define UNTITLED_BLE_HPP

#define PACKET_STX '+'
#define PACKET_ETX '-'

#define RX_PKT_BUFFER_SIZE 128
#define TX_PKT_BUFFER_SIZE 64

#include <HardwareSerial.h>
#include <stdint.h>

namespace ble {

    /**
     * Network packet.
     */
    class packet {
    public:
        uint64_t buffer_size;
        uint8_t *buffer;

        packet(uint64_t buffer_size, uint8_t *buffer)
                : buffer_size(buffer_size), buffer(buffer) {
        }

        ~packet() {
            delete[] buffer;
        }
    };

    /**
     * Represents network.
     */
    class network {
    private:
        friend class device;

    private:
        HardwareSerial* serial;

    private:
        uint8_t *buffer        = nullptr;
        uint64_t buffer_size   = 0;
        uint64_t buffer_cursor = 0;

    private:
        bool packet_begin = false;

    public:
        network(HardwareSerial& serial)
        : serial(&serial) {

        }
    protected:
        /**
         * Clears packet buffer.
         */
        void clearBuffer();

        /**
         * Acquires packet buffer.
         */
        void acquireBuffer();

        /**
         * Releases packet buffer.
         */
        void releaseBuffer();

    public:
        /**
         * Handles packet receiving.
         */
        ble::packet *recvLoop();

        /**
         * Handles packet sending.
         */
        void sendLoop();
    };

    /**
     * Represents device.
     */
    class device {
    public:
        ble::network network;

    public:
        device(HardwareSerial& serial);

    public:
        /**
         * Initializes connection with device.
         */
        void init();

        /**
         * Terminates connection with device.
         */
        void term();
    };
};


#endif //UNTITLED_BLE_HPP
