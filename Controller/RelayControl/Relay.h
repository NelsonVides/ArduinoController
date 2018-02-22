/*
 * Relay.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Nelson Vides
 */

#ifndef RELAYCONTROL_RELAY_H_
#define RELAYCONTROL_RELAY_H_

#include <Arduino.h>

namespace Heater {

    class Relay {
    public:
        explicit Relay(uint8_t pin);
        void begin() const;
        void connect() const;
        void disconnect() const;
    private:
        const uint8_t _pin;
    };
}

#endif /* RELAYCONTROL_RELAY_H_ */
