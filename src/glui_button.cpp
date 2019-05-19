/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_button.cpp - GLUI_Button control class


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

namespace GLUI_Library
{//-.
//<-'

/****************************** GLUI_Button::mouse_down_handler() **********/

bool UI::Button::_mouse_down_handler(int local_x, int local_y)
{
	/** A button always in unpressed before here, so
	now we invariably set it to 'depressed' **/
	set_int_val(1); return false;
}

/****************************** GLUI_Button::mouse_up_handler() **********/

bool UI::Button::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	set_int_val(0); /** A button always turns off after you press it **/

	if(inside) execute_callback(); /*** Invoke the user's callback ***/
	
	return false;
}

/****************************** GLUI_Button::mouse_held_down_handler() ******/

bool UI::Button::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	if(inside!=GLUI.prev_inside) redraw(); return false;
}

/********************************************** GLUI_Button::draw() **********/

void UI::Button::_draw()
{
	int press = this==GLUI::get_pressed(); 
	
	if(press) //draw_pressed()
	{			
		//NOTE: This NEW look matches the 
		//scrollbar buttons.
		//glColor3ub(0,0,0);
		//draw_border_rect(0,0,w,h,0xFF); //double black border
		glColor3ub(128,128,128);		
		draw_border_rect(0,0,w,h,0xF);
	}
	else draw_raised_rect(0,0,w,h,2|4);
	
	//draw_text(int press)
	{
		glColor3ub(0,0,0);

		//I have no clue how to calculate this.
		int y = (h-font.height)/2+2;
		draw_name(std::max(w-string_width(name),0)/2,y+press);

		if(this==GLUI::get_active())
		{
			//REMINDER: This tweak is concerned with label spacing
			//and collision with the stippled border.

			if(!press)				
			draw_active_rect(3,3,w-3,h-3);
			else
			draw_active_rect(3-press,3-press,w-3+press,h-3+press);
		}
	}
}

/************************************** GLUI_Button::update_size() **********/

void UI::Button::_update_size()
{
	int lineup = 16;
	int text_size = string_width(name)+lineup;
	text_size = lineup*int(text_size/(float)lineup+0.5f);

	w = std::max(w,text_size+16);

	if(w%2==0) w++; //Odds squares focus rect.
}

//---.
}//<-'
