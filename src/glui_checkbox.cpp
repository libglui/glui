
/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_checkbox - GLUI_Checkbox control class


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

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

*****************************************************************************/

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

namespace GLUI_Library
{//-.
//<-'

static int glui_checkbox_orig_val;

/****************************** GLUI_Checkbox::mouse_down_handler() **********/

bool UI::CheckBox::_mouse_down_handler(int local_x, int local_y)
{
	glui_checkbox_orig_val = int_val; set_int_val(!int_val);

	redraw(); return false;
}

/****************************** GLUI_Checkbox::mouse_up_handler() **********/

bool UI::CheckBox::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(inside) /*** Invoke the callback ***/
	{ 
		//set_int_val(int_val);
		int nv = inside?!glui_checkbox_orig_val:glui_checkbox_orig_val;
		if(nv!=int_val) set_int_val(nv); 
		
		execute_callback();
	}
	else set_int_val(glui_checkbox_orig_val); /* undo effect on value */

	redraw(); return false;
}

/****************************** GLUI_Checkbox::mouse_held_down_handler() ******/

bool UI::CheckBox::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	/********** Toggle checked and unchecked bitmap if we're entering or
	leaving the checkbox area **********/
	if(inside!=GLUI.prev_inside)
	{
		//set_int_val(!int_val); 
		set_int_val(inside?!glui_checkbox_orig_val:glui_checkbox_orig_val); 
		
		redraw();
	}
	return false;
}

/****************************** GLUI_Checkbox::draw() **********/

void UI::CheckBox::_draw()
{
	int i; if(int_val) 
	{
		i = enabled?Bitmap::CHECKBOX_ON:Bitmap::CHECKBOX_ON_DIS;
	}
	else i = enabled?Bitmap::CHECKBOX_OFF:Bitmap::CHECKBOX_OFF_DIS;

	std_bitmaps[i]->draw(); 
		
	draw_name(x_lr,0,this==GLUI::get_active());
}

/************************************ GLUI_Checkbox::update_size() **********/

void UI::CheckBox::_update_size()
{				  
	int text_size = name_width();

	/* if(w<x_lr+text_size+6) */
	{
		w = x_lr+text_size+6;
	}
}

//---.
}//<-'
