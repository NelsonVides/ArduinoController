/*
 * sim.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Nelson Vides
 */

#ifndef SIM_H_
#define SIM_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800
SoftwareSerial sim(11, 10);

void smsTest() {
    //Set SMS format to ASCII
    sim.write("AT+CMGF=1\r\n");
    delay(1000);

    //Send new SMS command and message number
    sim.write("AT+CMGS=\"733791307\"\r\n");
    delay(1000);

    //Send SMS content
    sim.write("TEST");
    delay(1000);

    //Send Ctrl+Z / ESC to denote SMS message is complete
    sim.write((char)26);
    delay(1000);

    Serial.println("SMS Sent!");
}



#endif /* SIM_H_ */
