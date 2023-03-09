#pragma once

#include <cstdint>
#include "gemma_pins.h"

namespace Notes
{
	class Note;
}

class Beeper
{
public:
	explicit Beeper(uint32_t pinID = GemmaPin::A0);

	void PlayToneBlocking(float freqHz, uint32_t durationMS);
	void PlayNoteBlocking(const Notes::Note& note, uint32_t durationMS);

private:
	uint32_t m_PinID = GemmaPin::A0;
};
