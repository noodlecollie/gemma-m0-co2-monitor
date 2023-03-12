#include "note_sequences.h"
#include "utils.h"

namespace NoteSequences
{
	Notes::NoteSequence BootupSequence()
	{
		using namespace Notes;

		static const SerialisedNote SEQUENCE[] =
		{
			{ 50, NoteID::C, 4 },
			{ 50, NoteID::E, 4 },
			{ 50, NoteID::G, 4 },
			{ 100, NoteID::C, 5 }
		};

		return NoteSequence(SEQUENCE);
	}
}
