// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Natural Selection Module
//

/* This file is a replacement for the engine call of ALLOC_STRING
 * The main difference is that a string will not be allocated twice
 * as to try to avoid wasting the HL zone space.
 *
 * NOTE: The lookup uses strcmp() in a linked list! It is not fast
 *       Its implementation in the NS module is on rarely used
 *       natives.
 */

#ifndef ALLOCSTRING_H
#define ALLOCSTRING_H

#include "CString.h"
#include "sh_list.h"

class StringManager
{
private:
	List<String *>		m_StringList;

public:
	/**
	 * sh_list.h does not delete objects put into
	 * the list, so I need to delete those and clear()
	 */
	inline void Clear(void)
	{
		List<String *>::iterator		 end;
		List<String *>::iterator		 iter;

		iter=m_StringList.begin();
		end=m_StringList.end();

		while (iter!=end)
		{
			delete (*iter++);
		}

		m_StringList.clear();
	}

	/**
	 * Iterate the list to see if the string exists
	 * This is slow and not very ideal, however
	 * this is *very* rarely used so it won't matter
	 */
	inline int Allocate(const char *str)
	{
		List<String *>::iterator		 end;
		List<String *>::iterator		 iter;

		iter=m_StringList.begin();
		end=m_StringList.end();

		while (iter!=end)
		{
			if (strcmp(str, (*iter)->c_str()) == 0)
			{
				// String is already in the list, do not allocate it again
				return MAKE_STRING((*iter)->c_str());
			}
			++iter;
		}

		// Was not found in the linked list, allocate it and add it to the list
		String *AllocStr = new String;

		AllocStr->assign(str);

		m_StringList.push_back(AllocStr);

		return MAKE_STRING(AllocStr->c_str());

	};

};

extern StringManager AllocStringList;

/**
 * Simple wrapper to make conversion easier
 */
inline int ALLOC_STRING2(const char *InputString)
{
	return AllocStringList.Allocate(InputString);
}


#endif // ALLOCSTRING_H
