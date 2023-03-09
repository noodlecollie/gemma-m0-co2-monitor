#include <Arduino.h>
#include <limits>
#include "beeper.h"
#include "notes.h"

static constexpr uint32_t ANALOGUE_WAVE_HIGH = 255;
static constexpr uint32_t ANALOGUE_WAVE_LOW = 0;

// Max duration we support is the max number of ms that will not overflow
// when promoted up to microseconds.
static constexpr uint32_t MAX_DURATION_MS = std::numeric_limits<uint32_t>::max() / 1000;

// Min frequency we support is C0.
static constexpr float MIN_FREQ_HZ = 16.35f;

// Max frequency we support is C7.
static constexpr float MAX_FREQ_HZ = 2093.0f;

static constexpr uint32_t HzToMicrosecPeriod(float freqHz)
{
	// (1/freq) * 1000000 == 1000000/freq
	// We then do a +0.5f and floor, to round in a constexpr way.
	return static_cast<uint32_t>((1000000.0f / freqHz) + 0.5f);
}

Beeper::Beeper(uint32_t pinID)
	: m_PinID(pinID)
{
}

void Beeper::PlayToneBlocking(float freqHz, uint32_t durationMS) const
{
	if ( durationMS < 1 )
	{
		return;
	}

	if ( freqHz < MIN_FREQ_HZ )
	{
		freqHz = MIN_FREQ_HZ;
	}
	else if ( freqHz> MAX_FREQ_HZ )
	{
		freqHz = MAX_FREQ_HZ;
	}

	if ( durationMS > MAX_DURATION_MS )
	{
		durationMS = MAX_DURATION_MS;
	}

	const uint32_t microsecsPerHalfOscillation = HzToMicrosecPeriod(freqHz) / 2;
	const uint32_t totalHalfOscillations = (durationMS * 1000) / microsecsPerHalfOscillation;

	bool isHigh = true;

	for ( uint64_t iteration = 0; iteration < totalHalfOscillations; isHigh = !isHigh, ++iteration )
	{
		analogWrite(m_PinID, isHigh ? ANALOGUE_WAVE_HIGH : ANALOGUE_WAVE_LOW);
		delayMicroseconds(microsecsPerHalfOscillation);
	}

	// Always end low.
	analogWrite(m_PinID, 0);
}

void Beeper::PlayNoteBlocking(const Notes::Note& note, uint32_t durationMS) const
{
	if ( !note.HasFrequency() )
	{
		// Specified duration of silence
		delay(durationMS);
		return;
	}

	PlayToneBlocking(note.GetFrequency(), durationMS);
}

void Beeper::PlayNoteSequenceBlocking(const Notes::SerialisedNote* sequence, size_t sequenceLength) const
{
	if ( !sequence || sequenceLength < 1 )
	{
		return;
	}

	while ( sequenceLength > 0 )
	{
		PlayNoteBlocking(Notes::Note(*sequence), sequence->durationMS);
		++sequence;
		--sequenceLength;
	}
}
