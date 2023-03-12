#include <Arduino.h>
#include <Wire.h>
#include "notes.h"
#include "utils.h"
#include "gemma_pins.h"
#include "devices.h"
#include "note_sequences.h"

static bool InitialiseCO2Monitor()
{
	Devices::Dev_CO2Sensor.Reinitialise();

	SCD4X::SerialNumber serial {};

	if ( !Devices::Dev_CO2Sensor.GetSerialNumber(serial) )
	{
		Serial.printf("Failed to get SCD4X serial number!\n");
		return false;
	}

	Serial.printf("SCD4X serial number: %04x:%04x:%04x\n", serial.component[0], serial.component[1], serial.component[2]);
	return true;
}

void setup()
{
	pinMode(GemmaPin::SINGLE_LED, OUTPUT);
	pinMode(GemmaPin::A0, OUTPUT);

	Wire.begin();
	Serial.begin(9600);

	Devices::Dev_Beeper.PlayNoteSequenceBlocking(NoteSequences::Bootup());

	const bool monitorInit = InitialiseCO2Monitor();
	Devices::Dev_Beeper.PlayNoteSequenceBlocking(NoteSequences::Result(monitorInit));
}

void loop()
{
}
