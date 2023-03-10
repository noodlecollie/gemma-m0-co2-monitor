#include <algorithm>
#include "notes.h"
#include "utils.h"

namespace Notes
{
	static const float NOTE_FREQUENCIES[NoteID::NumNotes] =
	{
#define LIST_ITEM(value, freq) freq,
		NOTE_DEF_LIST
#undef LIST_ITEM
	};

	static bool NoteHasFrequency(NoteID note)
	{
		return static_cast<NoteIDBaseType>(note) < static_cast<NoteIDBaseType>(Utils::ArraySize(NOTE_FREQUENCIES));
	}

	static float GetNoteFrequency(NoteID noteID)
	{
		return NoteHasFrequency(noteID) ? NOTE_FREQUENCIES[static_cast<NoteIDBaseType>(noteID)] : 0.0f;
	}

	static NoteID DeserialiseNoteID(NoteIDBaseType inID)
	{
		return inID <= NoteID::NoNote ? static_cast<NoteID>(inID) : NoteID::NoNote;
	}

	Note::Note()
	{
	}

	Note::Note(NoteID noteID)
		: m_NoteID(noteID)
	{
	}

	Note::Note(NoteID noteID, uint8_t octave) :
		m_NoteID(noteID),
		m_Octave(std::min<uint8_t>(octave, MAX_OCTAVE))
	{
	}

	Note::Note(const SerialisedNote& sNote) :
		m_NoteID(DeserialiseNoteID(sNote.noteID)),
		m_Octave(std::min<uint8_t>(sNote.octave, MAX_OCTAVE))
	{
	}

	float Note::NoteToFrequency(NoteID noteID, uint8_t octave)
	{
		if ( !NoteHasFrequency(noteID) )
		{
			return 0.0f;
		}

		float freq = GetNoteFrequency(noteID);

		if ( octave > MAX_OCTAVE )
		{
			octave = MAX_OCTAVE;
		}

		while ( octave > 0 )
		{
			freq *= 2.0f;
			--octave;
		}

		return freq;
	}

	NoteID Note::PreviousNote(NoteID noteID)
	{
		if ( !NoteHasFrequency(noteID) )
		{
			return noteID;
		}

		if ( noteID == NoteID::C )
		{
			return NoteID::B;
		}

		return static_cast<NoteID>(static_cast<NoteIDBaseType>(noteID) - 1);
	}

	NoteID Note::NextNote(NoteID noteID)
	{
		if ( !NoteHasFrequency(noteID) )
		{
			return noteID;
		}

		if ( noteID == NoteID::B )
		{
			return NoteID::C;
		}

		return static_cast<NoteID>(static_cast<NoteIDBaseType>(noteID) + 1);
	}

	NoteID Note::GetNoteID() const
	{
		return m_NoteID;
	}

	void Note::SetNoteID(NoteID noteID)
	{
		m_NoteID = noteID;
	}

	uint8_t Note::GetOctave() const
	{
		return m_Octave;
	}

	void Note::SetOctave(uint8_t octave)
	{
		m_Octave = std::min<uint8_t>(octave, MAX_OCTAVE);
	}

	bool Note::HasFrequency() const
	{
		return NoteHasFrequency(m_NoteID);
	}

	float Note::GetFrequency() const
	{
		return GetFrequency(m_Octave);
	}

	float Note::GetFrequency(uint8_t octave) const
	{
		return NoteToFrequency(m_NoteID, octave);
	}

	Note& Note::operator ++()
	{
		if ( m_NoteID == NoteID::B && m_Octave == MAX_OCTAVE )
		{
			return *this;
		}

		if ( m_NoteID == NoteID::B )
		{
			++m_Octave;
		}

		m_NoteID = NextNote(m_NoteID);
		return *this;
	}

	Note& Note::operator --()
	{
		if ( m_NoteID == NoteID::C && m_Octave == 0 )
		{
			return *this;
		}

		if ( m_NoteID == NoteID::C )
		{
			--m_Octave;
		}

		m_NoteID = NextNote(m_NoteID);
		return *this;
	}


	Note Note::operator ++(int)
	{
		Note out(*this);
		operator ++();
		return out;
	}

	Note Note::operator --(int)
	{
		Note out(*this);
		operator --();
		return out;
	}
}
