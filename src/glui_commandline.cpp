/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_commandline.cpp - GLUI_CommandLine control class


      --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher, 2005 William Baxter

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  This program is -not- in the public domain.

*****************************************************************************/

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

namespace GLUI_Library
{//-.
//<-'

bool UI::CommandLine::_activate(int how)
{
	if(how&&how!=ACTIVATE_MOUSE)
	{
		if(sel_end==sel_start)
		{
			//HACK? Not (quite) yet activated.
			//if(GLUI::set_caret(this,sel_end))
			{
				GLUI.curr_caret_pt = sel_end;			
				find_insertion_xy(&GLUI.caret_x,&GLUI.caret_y);
			}
		}
	}

	return true;
}

/****************************** GLUI_CommandLine::key_handler() **********/

bool UI::CommandLine::_key_handler(int key, int modifiers)
{		
	_dump("-> CMD_TEXT KEY HANDLER");

	if(key=='\r'||key==-'\r'
	&&return_input_model==UI::ACTIVATE_OTHER)
	{
		add_to_history(text);
		execute_callback();
		GLUI::set_edited(); //Reproduce EditText semantics?
		set_text();
		
		return false;
	}

	bool ret = Super::_key_handler(key,modifiers);

	_dump("<- CMD_TEXT KEY HANDLER");

	return ret;
}

/**************************** GLUI_CommandLine::special_handler() **********/

bool UI::CommandLine::_special_handler(int key, int modifiers)
{ 
	_dump("-> CMD_TEXT KEY SPECIAL");

	bool ret = false;

	if(key==GLUT_KEY_UP||key==GLUT_KEY_DOWN) // PREVIOUS HISTORY
	{
		scroll_history(key==GLUT_KEY_UP?-1:1); // NEXT HISTORY
	}
	else ret = Super::_special_handler(key,modifiers); //EditText
	
	_dump("<- CMD_TEXT KEY SPECIAL");

	return ret;
}

/**************************** GLUI_CommandLine::recall_history() ********/

bool UI::CommandLine::recall_history(int hist_num)
{
	if(hist_num<oldest_hist||hist_num>newest_hist||hist_num==curr_hist)
	{
		return false;
	}

	// Commit the current text first before we blow it away!
	if(curr_hist==newest_hist)
	{
		get_history_str(newest_hist) = text;
	}

	curr_hist = hist_num;
	set_text(get_history_str(curr_hist));
	special_handler(GLUT_KEY_END);

	update_and_draw_text(); return true;
}

/**************************** GLUI_CommandLine::add_to_history() ********/

bool UI::CommandLine::add_to_history(C_String cmp)
{
	if(!cmp.str||!cmp.str[0]) return false; // don't add if it's empty
	
	String &top = get_history_str(newest_hist);	
	if(top==cmp.str) return false; 	

	curr_hist = newest_hist; top = text; // save previous command text

	newest_hist = ++curr_hist;
	if(newest_hist>=_add_to_history_HIST_SIZE)
	{
		// bump oldest off the list
		//hist_list.erase(hist_list.begin());
		//hist_list.push_back("");
		hist_list[oldest_hist_wrap].clear();
		++oldest_hist_wrap%=_add_to_history_HIST_SIZE;

		oldest_hist++;
	}

	return true;
}
	
/*************************************** GLUI_CommandLine::_dump() **************/

#ifdef GLUI_DEBUG_STDOUT
void UI::CommandLine::_dump(const char *name)
{
	fprintf(_dump_FILE,
	"%s (commandline@%p):  ins_pt:%d  subs:%d/%d  sel:%d/%d   len:%d\n",name,this,
	GLUI.curr_caret_pt,substring_start,substring_end,sel_start,sel_end,(int)text.size());
}
#endif

//---.
}//<-'

