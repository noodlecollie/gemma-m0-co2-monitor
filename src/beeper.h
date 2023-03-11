#pragma once

#include <cstdint>
#include <cstddef>
#include "gemma_pins.h"

namespace Notes
{
	struct SerialisedNote;
	class Note;
}

class Beeper
{
public:
	explicit Beeper(uint32_t pinID = GemmaPin::A0);

	void PlayToneBlocking(float freqHz, uint32_t durationMS) const;
	void PlayNoteBlocking(const Notes::Note& note, uint32_t durationMS) const;
	void PlayNoteSequenceBlocking(const Notes::SerialisedNote* sequence, size_t sequenceLength) const;

private:
	uint32_t m_PinID = GemmaPin::A0;
};
