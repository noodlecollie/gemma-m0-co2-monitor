#include <Arduino.h>
#include "beeper.h"
#include "notes.h"

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
	static Notes::NoteID note = Notes::NoteID::C;

	g_Beeper.PlayNoteBlocking(note, 500);

	note = Notes::Note::NextNote(note);
	delay(500);
}
