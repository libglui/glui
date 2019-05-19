/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_panel.cpp - GLUI_Panel control class


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

/**************************** GLUI_Rollout::mouse_down_handler() **********/

bool UI::Rollout::_mouse_down_handler(int local_x, int local_y)
{
	if(local_y-y_abs>UI_BUTTON_HEIGHT)
	{
		GLUI.init_inside = GLUI.prev_inside = false;

		return false;
	}
	
	redraw(); return false;
}

/**************************** GLUI_Rollout::mouse_held_down_handler() ****/

bool UI::Rollout::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	if(local_y-y_abs>UI_BUTTON_HEIGHT)
	{
		GLUI.curr_inside = false;
	}
	
	if(GLUI.init_inside)
	{
		if(inside!=GLUI.prev_inside) redraw();
	}

	return false;
}

/**************************** GLUI_Rollout::mouse_down_handler() **********/

bool UI::Rollout::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(GLUI.init_inside&&inside)
	{
		_set_is_open(!is_open);
	}
	else redraw(); return false;
}

/********************************* GLUI_Rollout::draw() ***********/

void UI::Rollout::_draw()
{
	int x1 = 1, x2 = w-1;
	int y1 = 1, y2 = 1+UI_BUTTON_HEIGHT;

	int press = this==GLUI::get_pressed();

	//Eliminating SW white pixel sticking out of bottom.
	//draw_etched_rect(-1,-1,w+1,is_open?h+3:UI_BUTTON_HEIGHT+1);
	if(!is_open)
	{
		glColor3ub(128,128,128); 
		draw_border_rect(x1-1,y1-1,x2+1,y2+1,1|8);
		
		if(!press) //Shadow?
		{
			glColor3ub(255,255,255);
			draw_border_rect(x1-1,y1-1,x2+1,y2+1,2|4);
			glColor3ub(233,233,233);
		}
		else glColor3ub(255,255,255);

		draw_border_rect(x1-!press,y1-1,x2+1,y2+1,press?2|4|8:2|4);
	}
	else 
	{
		int bot = h;
		if(next<Rollout>()) 
		bot+=2+UI_ITEMSPACING;
		draw_etched_rect(x1-1,y1-1,x2+1,bot);

		if(!press) //Shadow?
		{
			glColor3ub(175,175,175);
			draw_border_rect(x1,y1+1,x2-1,y2+1,4);
			glColor3ub(233,233,233);
			draw_border_rect(x1,y1-1,x2+1,y2+2,2);
		}
	}
	
	//The following should match Button's appearance.
	//int save = h;
	//h = UI_BUTTON_HEIGHT; ((Button*)this)->_draw(); 
	//h = save;
	if(press)
	{
		draw_filled_rect(x1+1,y1,x2,y2,BOX_CLEAR);
		glColor3ub(128,128,128); 
		draw_border_rect(x1+1,y1,x2,y2);
	}
	else draw_raised_rect(x1,y1,x2,y2,0);	
	glColor3ub(0,0,0);
	//I have no clue how to calculate this.
	int y = y1+(UI_BUTTON_HEIGHT-font.height)/2+2;
	draw_name(x1+std::max(w-string_width(name),0)/2,y+press);
	if(this==GLUI::get_active())
	{
		//REMINDER: This tweak is concerned with label spacing
		//and collision with the stippled border.
	
		if(!press)
		draw_active_rect(4,4,w-5,UI_BUTTON_HEIGHT-2);
		else
		draw_active_rect(4,3,w-3,UI_BUTTON_HEIGHT-1);
	}

	/** Draw '+' or '-' **/
	int icon = (is_open?2:1|2)*(enabled?1:-1);
	draw_etched_bars(w-21,8+press,w-12,17+press,icon);
}

/***************************** GLUI_Rollout::update_size() **********/

void UI::Rollout::_update_size()
{
	//May adjust or make room for a scrollbar.
	x_lr = x_rl = x_off+2;
	  
	int text_size = string_width(name);

	w = std::max(w,text_size+36);
 
	if(w%2!=0) w++; //Odds squares focus rect.

	//if(!is_open) h = UI_DEFAULT_HEIGHT + 7; //2019
	if(!is_open)
	{
		h = UI_BUTTON_HEIGHT+(next<Rollout>()?-2:2);	
	}
}
	  
//---.
}//<-'
