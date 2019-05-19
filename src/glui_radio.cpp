/******************************************************************
**********

  GLUI User Interface Toolkit
  ---------------------------

	 glui_radio.cpp - GLUI_RadioGroup and GLUI_RadioButton control classes


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

static int glui_radio_old_val;

/********************************* GLUI_RadioGroup::set_selected() **********/

void UI::RadioGroup::_update_live()
{						
	int i = int_val;
	Control *ch = first_child();
	for(;ch;ch=ch->next())
	{
		//Allow Column or Separator? What about Panel?
		if(-1==ch->id) continue;

		if(i==-1) /*** All buttons in group are deselected ***/
		{ 
			ch->set_int_val(0);
		}
		else if(i==ch->id) /*** This is selected ch ***/
		{		
			ch->set_int_val(1);
		}
		else /*** This is NOT selected ch ***/
		{ 
			ch->set_int_val(0);
		}
	}
}

/**************************** Group_Interface::special_handler() **********/

bool Group_Interface::_special_handler(int key, int modifiers)
{
	//NOTE: This handler is for wheel only.

	int adv; 
			  
	if(key==GLUT_KEY_UP||key==GLUT_KEY_LEFT)
	{
		adv = -1;
	}
	else if(key==GLUT_KEY_DOWN||key==GLUT_KEY_RIGHT)
	{
		adv = +1;
	}
	else return true;

	if(Control *p=find_child(int_val))
	while(p=adv<0?p->prev():p->next())
	{
		if(-1==p->id) continue;
		
		set_int_val(p->id); break;
	}
	return false; 
}

/************************ GLUI_RadioButton::mouse_down_handler() **********/

bool UI::RadioButton::_mouse_down_handler(int local_x, int local_y)
{
	glui_radio_old_val = parent()->int_val;							
	parent()->set_int_val(id); return false;
}

/********************** GLUI_RadioButton::mouse_held_down_handler() ******/

bool UI::RadioButton::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	if(inside!=GLUI.prev_inside)
	{
		parent()->set_int_val(inside?id:glui_radio_old_val);
	}
	return false;
}

/*************************** GLUI_RadioButton::mouse_up_handler() **********/

bool UI::RadioButton::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(!inside)
	{
		parent()->set_int_val(glui_radio_old_val);

		redraw();
	}
	else
	{
		/** Now we update the radio button group.  We tell the group
		handler to set the currently-selected item to this button, which
		is reference by its user_id/ordinal number within group **/

		parent()->set_int_val(id);

		/*** Now update the linked variable, and call the callback,
		  but ONLY if the value of the radio group actually changed ***/
		if(parent()->int_val!=glui_radio_old_val)
		{
			parent()->execute_callback();
		}
		else assert(glui_radio_old_val==id); //NEW
	}

	return false;
}

/****************************** GLUI_RadioButton::draw() **********/

void UI::RadioButton::_draw()
{
	//Why not just int_val?
	/*** See if we're the currently-selected button. If so, draw ***/
	int i = parent()->int_val==id; 
	assert(i==int_val);

	if(i)
	{
		i = enabled?Bitmap::RADIOBUTTON_ON:Bitmap::RADIOBUTTON_ON_DIS;
	}
	else i = enabled?Bitmap::RADIOBUTTON_OFF:Bitmap::RADIOBUTTON_OFF_DIS;

	std_bitmaps[i]->draw(); 
		
	draw_name(x_lr,0,this==GLUI::get_active());
}

/******************************** GLUI_RadioButton::update_size() **********/

void UI::RadioButton::_update_size()
{	
	//HACK: Initialize RadioGroup. This way the
	//constructor doesn't have to fill this out.
	if(id==parent()->int_val)
	{
		if(!int_val) parent()->set_int_val(id);
	}

	int text_size = string_width(name);

	/* if(w<x_lr+text_size+6) */
	{
		w = x_lr+text_size+6;
	}
}
 		  
//---.
}//<-'
