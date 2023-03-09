#pragma once

#include <cstdint>
#include "gemma_pins.h"

class Beeper
{
public:
	explicit Beeper(uint32_t pinID = GemmaPin::A0);

	void PlayNoteBlocking(float freqHz, uint32_t durationMS);

private:
	uint32_t m_PinID = GemmaPin::A0;
};
