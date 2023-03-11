#include <Arduino.h>
#include <Wire.h>
#include "scd4x_driver.h"

#define DEBUG_LOG(...) \
do \
{ \
	if ( m_ShouldLog ) \
	{ \
		Serial.printf(__VA_ARGS__); \
	} \
} \
while ( false )

bool SCD4X::DebugLoggingEnabled() const
{
	return m_ShouldLog;
}

void SCD4X::SetDebugLoggingEnabled(bool enabled)
{
	m_ShouldLog = enabled;
}

void SCD4X::FirstTimeInit()
{
	DEBUG_LOG("SCD4X: FirstTimeInit()\n");
	StopPeriodicMeasurement();
}

bool SCD4X::GetSerialNumber(SerialNumber& outSerial)
{
	static constexpr uint32_t RESPONSE_DELAY_MS = 1;
	static constexpr size_t RESPONSE_SIZE_WORDS = 3;

	SendCommand(Command::SERIALNUMBER, RESPONSE_DELAY_MS);

	WordCRC response[RESPONSE_SIZE_WORDS];

	if ( !ReadAndCheckReply(response) )
	{
		return false;
	}

	outSerial.component[0] = (response[0].Byte1() << 8) | response[0].Byte0();
	outSerial.component[1] = (response[1].Byte1() << 8) | response[1].Byte0();
	outSerial.component[2] = (response[2].Byte1() << 8) | response[2].Byte0();

	return true;
}

void SCD4X::StopPeriodicMeasurement()
{
	static constexpr uint32_t RESPONSE_DELAY_MS = 500;

	DEBUG_LOG("SCD4X: StopPeriodicMeasurement()\n");
	SendCommand(Command::STOPPERIODICMEASUREMENT, RESPONSE_DELAY_MS);
}

void SCD4X::SendCommand(Command cmd, uint32_t delayAfterCmdMS)
{
	const uint16_t toWrite = static_cast<uint16_t>(cmd);

	DEBUG_LOG("SCD4X: Sending command 0x%04x to 0x%02x\n", toWrite, DEFAULT_I2C_ADDRESS);

	Wire.beginTransmission(DEFAULT_I2C_ADDRESS);
	Wire.write(reinterpret_cast<const uint8_t*>(&toWrite), sizeof(toWrite));
	Wire.endTransmission();

	if ( delayAfterCmdMS > 0 )
	{
		DEBUG_LOG("SCD4X: Waiting for %ums\n", delayAfterCmdMS);
		delay(delayAfterCmdMS);
	}
}

bool SCD4X::ReadAndCheckReply(WordCRC* words, size_t numWords)
{
	if ( !ReadReply(words, numWords) )
	{
		DEBUG_LOG("SCD4X: ReadAndCheckReply() was unable to read correct number of bytes\n");
		return false;
	}

	for ( size_t index = 0; index < numWords; ++index )
	{
		if ( ComputeCRCByte(words[index].Byte0(), words[index].Byte1()) != words[index].CRC() )
		{
			DEBUG_LOG("SCD4X: ReadAndCheckReply() CRC failed for byte %lu\n", index);
			return false;
		}
	}

	return true;
}

bool SCD4X::ReadReply(WordCRC* words, size_t numWords)
{
	return ReadReply(reinterpret_cast<uint8_t*>(words), numWords * sizeof(WordCRC)) == numWords;
}

size_t SCD4X::ReadReply(uint8_t* buffer, size_t bufferSize)
{
	DEBUG_LOG("SCD4X: Requesting %lu bytes from 0x%02x\n", bufferSize, DEFAULT_I2C_ADDRESS);

	Wire.requestFrom(DEFAULT_I2C_ADDRESS, bufferSize);

	size_t bytesRead = 0;

	while ( Wire.available() > 0 && bytesRead < bufferSize )
	{
		buffer[bytesRead++] = static_cast<uint8_t>(Wire.read());
	}

	DEBUG_LOG("SCD4X: Read a total of %lu bytes from 0x%02x\n", bytesRead, DEFAULT_I2C_ADDRESS);
	return bytesRead;
}

uint8_t SCD4X::ComputeCRCByte(uint8_t b0, uint8_t b1)
{
	uint8_t crcByte = 0xFF;

	for ( size_t index = 0; index < 2; ++index )
	{
		crcByte ^= index == 0 ? b0 : b1;

		for ( size_t bitIndex = 0; bitIndex < 8; ++bitIndex )
		{
			crcByte <<= 1;

			if ( crcByte & 0x80 )
			{
				crcByte ^= 0x31;
			}
		}
	}

	return crcByte;
}
