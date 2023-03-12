#include <Arduino.h>
#include <Wire.h>
#include "notes.h"
#include "utils.h"
#include "gemma_pins.h"
#include "devices.h"

void setup()
{
	pinMode(GemmaPin::SINGLE_LED, OUTPUT);
	pinMode(GemmaPin::A0, OUTPUT);

	Wire.begin();
	Serial.begin(9600);
	delay(500);

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
	using namespace Notes;

	static const SerialisedNote SEQUENCE[] =
	{
		{ 200, NoteID::C, 4 },
		{ 200, NoteID::D, 4 },
		{ 200, NoteID::E, 4 },
		{ 200, NoteID::F, 4 },
		{ 200, NoteID::G, 4 },
		{ 200, NoteID::A, 4 },
		{ 200, NoteID::B, 4 },
		{ 200, NoteID::C, 5 },
		{ 1000, NoteID::NoNote, 0 },
		{ 200, NoteID::C, 5 },
		{ 200, NoteID::B, 4 },
		{ 200, NoteID::A, 4 },
		{ 200, NoteID::G, 4 },
		{ 200, NoteID::F, 4 },
		{ 200, NoteID::E, 4 },
		{ 200, NoteID::D, 4 },
		{ 200, NoteID::C, 4 },
	};

	digitalWrite(GemmaPin::SINGLE_LED, HIGH);
	Devices::Dev_Beeper.PlayNoteSequenceBlocking(SEQUENCE, Utils::ArraySize(SEQUENCE));
	digitalWrite(GemmaPin::SINGLE_LED, LOW);

	delay(3000);
}
