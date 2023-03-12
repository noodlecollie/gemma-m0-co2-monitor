#pragma once

#include <cstddef>
#include "notes.h"

namespace NoteSequences
{
	Notes::NoteSequence Bootup();
	Notes::NoteSequence Success();
	Notes::NoteSequence Failure();

	static inline Notes::NoteSequence Result(bool success)
	{
		return success ? Success() : Failure();
	}
}
