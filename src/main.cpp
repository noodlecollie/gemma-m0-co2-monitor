#include <Arduino.h>
#include "beeper.h"

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
	static constexpr float NOTE_C0 = 16.35f;

	static float pitch = NOTE_C0;

	g_Beeper.PlayNoteBlocking(pitch, 500);

	pitch *= 2;

	if ( pitch > 1400.0f )
	{
		pitch = NOTE_C0;
	}

	delay(500);
}
