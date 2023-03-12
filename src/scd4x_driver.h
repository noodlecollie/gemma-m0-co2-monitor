#pragma once

#include <cstdint>
#include <cstddef>

// Based on https://github.com/adafruit/Adafruit_CircuitPython_SCD4X/blob/main/adafruit_scd4x.py
// This class assumes that Wire.begin() has already been called in setup().
class SCD4X
{
public:
	struct SerialNumber
	{
		static constexpr size_t NUM_COMPONENTS = 3;
		uint16_t component[NUM_COMPONENTS];
	};

	// Messages are logged to serial - it is assumed that this has been set up.
	bool DebugLoggingEnabled() const;
	void SetDebugLoggingEnabled(bool enabled);

	void FirstTimeInit();
	bool GetSerialNumber(SerialNumber& outSerial);

private:
	static const uint8_t DEFAULT_I2C_ADDRESS = 0x62;

	enum class Command : uint16_t
	{
		REINIT = 0x3646,
		FACTORYRESET = 0x3632,
		FORCEDRECAL = 0x362F,
		SELFTEST = 0x3639,
		DATAREADY = 0xE4B8,
		STOPPERIODICMEASUREMENT = 0x3F86,
		STARTPERIODICMEASUREMENT = 0x21B1,
		STARTLOWPOWERPERIODICMEASUREMENT = 0x21AC,
		READMEASUREMENT = 0xEC05,
		SERIALNUMBER = 0x3682,
		GETTEMPOFFSET = 0x2318,
		SETTEMPOFFSET = 0x241D,
		GETALTITUDE = 0x2322,
		SETALTITUDE = 0x2427,
		SETPRESSURE = 0xE000,
		PERSISTSETTINGS = 0x3615,
		GETASCE = 0x2313,
		SETASCE = 0x2416,
	};

#pragma pack(push, 1)
	struct WordCRC
	{
		enum Component
		{
			BYTE_0 = 0,
			BYTE_1,
			BYTE_CRC,
			NUM_BYTES
		};

		uint8_t bytes[Component::NUM_BYTES];

		uint8_t Byte0() const
		{
			return bytes[Component::BYTE_0];
		}

		uint8_t Byte1() const
		{
			return bytes[Component::BYTE_1];
		}

		uint8_t CRC() const
		{
			return bytes[Component::BYTE_CRC];
		}
	};
#pragma pack(pop)

	template<size_t SIZE>
	bool ReadAndCheckReply(WordCRC (&buffer)[SIZE])
	{
		return ReadAndCheckReply(buffer, SIZE);
	}

	static uint8_t ComputeCRCByte(uint8_t b0, uint8_t b1);

	void StopPeriodicMeasurement();
	void SendCommand(Command cmd, uint32_t delayAfterCmdMS = 0);
	bool ReadAndCheckReply(WordCRC* words, size_t numWords);
	bool ReadReply(WordCRC* words, size_t numWords);
	size_t ReadReply(uint8_t* buffer, size_t bufferSize);

	bool m_ShouldLog = false;
};
