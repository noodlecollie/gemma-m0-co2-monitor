#pragma once

#include <cstdint>

namespace Notes
{
	static constexpr uint8_t MAX_OCTAVE = 8;
	static constexpr uint8_t DEFAULT_OCTAVE = 4;

#define NOTE_DEF_LIST \
	LIST_ITEM(C = 0, 16.35f) \
	LIST_ITEM(CSharp, 17.32f) \
	LIST_ITEM(D, 18.35f) \
	LIST_ITEM(DSharp, 19.45f) \
	LIST_ITEM(E, 20.6f) \
	LIST_ITEM(F, 21.83f) \
	LIST_ITEM(FSharp, 23.12) \
	LIST_ITEM(G, 24.5f) \
	LIST_ITEM(GSharp, 25.96f) \
	LIST_ITEM(A, 27.5f) \
	LIST_ITEM(ASharp, 29.14f) \
	LIST_ITEM(B, 30.87f) \

	enum NoteID : int32_t
	{
#define LIST_ITEM(value, freq) value,
		NOTE_DEF_LIST
#undef LIST_ITEM

		NumNotes,
		NoNote = -1,

		// Useful aliases:
		DFlat = CSharp,
		EFlat = DSharp,
		GFlat = FSharp,
		AFlat = GSharp,
		BFlat = ASharp,
	};

	class Note
	{
	public:
		Note();
		Note(NoteID noteID);
		Note(NoteID noteID, uint8_t octave);

		static float NoteToFrequency(NoteID noteID, uint8_t octave);
		static NoteID PreviousNote(NoteID noteID);
		static NoteID NextNote(NoteID noteID);

		NoteID GetNoteID() const;
		void SetNoteID(NoteID noteID);

		uint8_t GetOctave() const;
		void SetOctave(uint8_t octave);

		bool HasFrequency() const;
		float GetFrequency() const;
		float GetFrequency(uint8_t octave) const;

		// Prefix
		Note& operator ++();
		Note& operator --();

		// Postfix
		Note operator ++(int);
		Note operator --(int);

	private:
		NoteID m_NoteID = NoteID::NoNote;
		uint8_t m_Octave = DEFAULT_OCTAVE;
	};
}
