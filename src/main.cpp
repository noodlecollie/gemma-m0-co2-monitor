#include <Arduino.h>

namespace PinNumber
{
	static constexpr uint32_t A0 = 1;
	static constexpr uint32_t SINGLE_LED = 13;
}

void setup()
{
	pinMode(PinNumber::SINGLE_LED, OUTPUT);
	pinMode(1, OUTPUT);
}

void loop()
{
	static bool isHigh = false;

	digitalWrite(PinNumber::SINGLE_LED, isHigh ? HIGH : LOW);
	analogWrite(PinNumber::A0, isHigh ? 999 : 0);

	isHigh = !isHigh;
	delay(1);
}
