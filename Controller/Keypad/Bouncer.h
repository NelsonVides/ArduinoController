/*
  The MIT License (MIT)

  Copyright (c) 2013 thomasfredericks

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement ANALOG_PINS
 * and modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Bounce2_h
#define Bounce2_h

// Uncomment the following line for analog pins in use
#define ANALOG_PINS

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT

// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <inttypes.h>

namespace buttonsMgmt {
    namespace BouncerConsts {
        constexpr int16_t NOISE_TOLL = 20;
    }

    class Bouncer {
    public:
        // Create an instance of the bounce library
        explicit Bouncer(uint8_t pin);

        // Updates the pin
        // Returns whether the state changed or not
        bool update();

        // Returns the updated pin state
        bool read() const;

        // Returns the falling pin state
        bool fell() const;

        // Returns the rising pin state
        bool rose() const;

    #ifdef ANALOG_PINS
        // Returns the updated pin value
        int getValue() const;
    #endif

    protected:
        unsigned long previous_millis = 0;
        uint16_t interval_millis = 10;
    #ifdef ANALOG_PINS
        int value = 0;
    #endif
        uint8_t state = 0;
        uint8_t pin;
    };
}

#endif
