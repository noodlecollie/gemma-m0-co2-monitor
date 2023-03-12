#include "note_sequences.h"
#include "utils.h"

namespace NoteSequences
{
	Notes::NoteSequence Bootup()
	{
		using namespace Notes;

		static const SerialisedNote SEQUENCE[] =
		{
			{ 50, NoteID::C, 4 },
			{ 50, NoteID::E, 4 },
			{ 50, NoteID::G, 4 },
			{ 100, NoteID::C, 5 },
			{ 600, NoteID::NoNote, 5 },
		};

		return NoteSequence(SEQUENCE);
	}

	Notes::NoteSequence Success()
	{
		using namespace Notes;

		static const SerialisedNote SEQUENCE[] =
		{
			{ 100, NoteID::G, 4 },
			{ 250, NoteID::C, 5 }
		};

		return NoteSequence(SEQUENCE);
	}

	Notes::NoteSequence Failure()
	{
		using namespace Notes;

		static const SerialisedNote SEQUENCE[] =
		{
			{ 100, NoteID::C, 4 },
			{ 250, NoteID::B, 3 },
		};

		return NoteSequence(SEQUENCE);
	}
}
