#include <Arduino.h>

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
	static bool isHigh = false;
	static uint32_t count = 0;

	digitalWrite(PinNumber::SINGLE_LED, isHigh ? HIGH : LOW);
	analogWrite(PinNumber::A0, (isHigh && count < 500) ? 255 : 0);

	isHigh = !isHigh;
	++count;

	if ( count >= 1000 )
	{
		count = 0;
	}

	delay(1);
}
