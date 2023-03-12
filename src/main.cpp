#include <Arduino.h>
#include <Wire.h>
#include "notes.h"
#include "utils.h"
#include "gemma_pins.h"
#include "devices.h"
#include "note_sequences.h"

void setup()
{
	pinMode(GemmaPin::SINGLE_LED, OUTPUT);
	pinMode(GemmaPin::A0, OUTPUT);

	Wire.begin();
	Serial.begin(9600);
	delay(500);

	Devices::Dev_Beeper.PlayNoteSequenceBlocking(NoteSequences::BootupSequence());

	Devices::Dev_CO2Sensor.FirstTimeInit();

	SCD4X::SerialNumber serial {};

	if ( Devices::Dev_CO2Sensor.GetSerialNumber(serial) )
	{
		Serial.printf("SCD4X serial number: %u:%u:%u\n", serial.component[0], serial.component[1], serial.component[2]);
	}
	else
	{
		Serial.printf("Failed to get SCD4X serial number!\n");
	}
}

void loop()
{
}
