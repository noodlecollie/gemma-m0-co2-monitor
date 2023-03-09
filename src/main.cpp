#include <Arduino.h>
#include "beeper.h"
#include "notes.h"
#include "utils.h"

Beeper g_Beeper;

namespace PinNumber
{
	static constexpr uint32_t A0 = 1;
	static constexpr uint32_t SINGLE_LED = 13;
}

void setup()
{
	pinMode(PinNumber::SINGLE_LED, OUTPUT);
	pinMode(PinNumber::A0, OUTPUT);
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

	digitalWrite(PinNumber::SINGLE_LED, HIGH);
	g_Beeper.PlayNoteSequenceBlocking(SEQUENCE, Utils::ArraySize(SEQUENCE));
	digitalWrite(PinNumber::SINGLE_LED, LOW);

	delay(3000);
}
