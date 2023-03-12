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

void Beeper::PlayNoteSequenceBlocking(const Notes::NoteSequence& sequence) const
{
	if ( !sequence.IsValid() )
	{
		return;
	}

	for ( size_t index = 0; index < sequence.length; ++index )
	{
		const Notes::SerialisedNote* note = &sequence.notes[index];
		PlayNoteBlocking(Notes::Note(*note), note->durationMS);
	}
}

void Beeper::PlayValueEncoding(uint16_t value) const
{
	if ( value == 0 )
	{
		PlayDigitEncoding(0);
		return;
	}

	bool havePlayed = false;

	havePlayed = TryPlayValueEncoding(value, 4, havePlayed);
	havePlayed = TryPlayValueEncoding(value, 3, havePlayed);
	havePlayed = TryPlayValueEncoding(value, 2, havePlayed);
	havePlayed = TryPlayValueEncoding(value, 1, havePlayed);
	havePlayed = TryPlayValueEncoding(value, 0, havePlayed);
}

bool Beeper::TryPlayValueEncoding(uint16_t& value, uint8_t power, bool havePlayedBefore) const
{
	uint16_t divisor = 1;

	for ( uint8_t iteration = 0; iteration < power; ++iteration )
	{
		divisor *= 10;
	}

	uint16_t multiple = value / divisor;

	if ( multiple > 0 || havePlayedBefore )
	{
		PlayDigitPositionEncoding(power);
		PlayDigitEncoding(static_cast<uint8_t>(multiple));

		value -= multiple * divisor;
		return true;
	}

	return false;
}

void Beeper::PlayDigitPositionEncoding(uint8_t position) const
{
	for ( uint8_t iteration = 0; iteration <= position; ++iteration )
	{
		PlayNoteBlocking(Notes::Note(Notes::NoteID::C, 4), 100);
		PlayNoteBlocking(Notes::Note(Notes::NoteID::NoNote, 4), 100);
	}

	PlayNoteBlocking(Notes::Note(Notes::NoteID::NoNote, 4), 1000);
}

void Beeper::PlayDigitEncoding(uint8_t digit) const
{
	if ( digit == 0 )
	{
		PlayDigit0();
	}
	else
	{
		if ( digit >= 5 )
		{
			PlayDigit5();

			if ( digit > 5 )
			{
				Play5Punctuation();
			}

			digit -= 5;
		}

		for ( uint8_t iteration = 0; iteration < digit; ++iteration )
		{
			if ( iteration > 0 )
			{
				Play1Punctuation();
			}

			PlayDigit1();
		}
	}

	PlayNoteBlocking(Notes::Note(Notes::NoteID::NoNote, 4), 1500);
}

void Beeper::PlayDigit0() const
{
	PlayNoteBlocking(Notes::Note(Notes::NoteID::G, 4), 400);
}

void Beeper::PlayDigit5() const
{
	PlayNoteBlocking(Notes::Note(Notes::NoteID::C, 5), 300);
}

void Beeper::PlayDigit1() const
{
	PlayNoteBlocking(Notes::Note(Notes::NoteID::B, 4), 200);
}

void Beeper::Play5Punctuation() const
{
	PlayNoteBlocking(Notes::Note(Notes::NoteID::NoNote, 4), 300);
}

void Beeper::Play1Punctuation() const
{
	PlayNoteBlocking(Notes::Note(Notes::NoteID::NoNote, 4), 100);
}
