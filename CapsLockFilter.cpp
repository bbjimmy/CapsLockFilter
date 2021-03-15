/*
 * Copyright 2012, Haiku, Inc.  All Rights Reserved
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Philippe Saint-Pierre <stpere@gmail.com>
 *		Pete Goodeve <pete.goodeve
 */


#include <OS.h>
#include <InputServerFilter.h>
#include <InterfaceDefs.h>

#include <new>


class CapsLockFilter : public BInputServerFilter {
	public:
		CapsLockFilter();

		virtual filter_result Filter(BMessage* message, BList*);

	private:
		bool fCapsLocked;
};


CapsLockFilter::CapsLockFilter(): fCapsLocked(false)
{
}


filter_result
CapsLockFilter::Filter(BMessage* message, BList*)
{
	if (message->what == B_MODIFIERS_CHANGED) {
		int32 modifiers;
		message->FindInt32("modifiers", &modifiers);
		if (modifiers & B_CAPS_LOCK) {
			if (!fCapsLocked && !(modifiers & B_CONTROL_KEY)) {
				modifiers &= B_SCROLL_LOCK | B_NUM_LOCK;
					// clear CAPS -- ignore SHIFT etc.
				set_keyboard_locks(modifiers);
				return B_SKIP_MESSAGE;
			} else
				fCapsLocked = true;
		} else
			fCapsLocked = false;
	}

	return B_DISPATCH_MESSAGE;
}


extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();

BInputServerFilter*
instantiate_input_filter()
{
	return new (std::nothrow) CapsLockFilter();
}
