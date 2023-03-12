#pragma once

#include <cstdint>
#include <cstddef>
#include "gemma_pins.h"

namespace Notes
{
	struct SerialisedNote;
	struct NoteSequence;
	class Note;
}

class Beeper
{
public:
	explicit Beeper(uint32_t pinID = GemmaPin::A0);

	void PlayToneBlocking(float freqHz, uint32_t durationMS) const;
	void PlayNoteBlocking(const Notes::Note& note, uint32_t durationMS) const;
	void PlayNoteSequenceBlocking(const Notes::NoteSequence& sequence) const;
	void PlayValueEncoding(uint16_t value) const;

private:
	bool TryPlayValueEncoding(uint16_t& value, uint8_t power, bool havePlayedBefore) const;
	void PlayDigitPositionEncoding(uint8_t position) const;
	void PlayDigitEncoding(uint8_t digit) const;

	void PlayDigit0() const;
	void PlayDigit5() const;
	void PlayDigit1() const;
	void Play5Punctuation() const;
	void Play1Punctuation() const;

	uint32_t m_PinID = GemmaPin::A0;
};
