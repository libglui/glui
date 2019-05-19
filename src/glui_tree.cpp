/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_panel.cpp - GLUI_Panel control class


		  --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

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

#if defined(GLUI_BUILD_TREE_CONTROLS) || defined(GLUI_BUILD_EXAMPLES)

namespace GLUI_Library
{//-.
//<-'

/**************************** GLUI_Tree::mouse_down_handler() **********/

bool UI::Experiments::Tree::_mouse_down_handler(int local_x, int local_y)
{
	if(local_y-y_abs>18) //UI_TREE_HEADER?
	{
		GLUI.init_inside = GLUI.prev_inside = false;

		return false; //What about redraw??
	}

	redraw(); return false;
}

/**************************** GLUI_Tree::mouse_held_down_handler() ****/

bool UI::Experiments::Tree::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{	
	if(!GLUI.init_inside) return false;

	if(local_y-y_abs>18) inside = false; //UI_TREE_HEADER?

	if(GLUI.prev_inside!=inside) redraw(); return false;
}

/**************************** GLUI_Tree::mouse_down_handler() **********/

bool UI::Experiments::Tree::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(GLUI.init_inside&&inside)
	{
		_set_is_open(!is_open);
	}
	else redraw(); return false;
}

/********************************* GLUI_Tree::draw() ***********/

void UI::Experiments::Tree::_draw()
{
	int left = 5;
	int right = w-left;
	int top = 3;
	int bottom = 3+16;
	int middle = (top+bottom)/2;
	int delta_x = 0;

	draw_raised_rect(left,left+16,top,top+16);
	
	/*2019
	glColor3ubv(bkgd_color);
	glRecti(left+17,top+1,right-1,bottom-1);
	*/

	if(_format&DISPLAY_HIERARCHY)
	{
		/* The hierarchy is drawn in bold */
		delta_x = string_width(level_name)+char_width(' ');
		glColor3ubv((GLubyte*)_lRGB);
		draw_string(left+25,top,level_name);		
		draw_string(left+24,top,level_name);
	}

	//if(this==GLUI::get_active())
	//draw_active_rect(left+22,top,delta_x+left+string_width(name)+32,bottom-2);
	draw_name(delta_x+left+24,top,this==GLUI::get_active());	
	
	if(0) /**   Draw '+' or '-'  **/
	{
		//This should be identical except for the blue color.
		//I'm not sure this look suits a Tree control though.
		int icon = (is_open?2:1|2)*(enabled?1:-1);
		draw_etched_bars(left+4,top,left+13,bottom,icon);
	}
	else //REMOVE ME /**   Draw '+' or '-'  **/
	{
			glBegin(GL_LINES);
	
			if(is_open)
			{
				if(enabled)
				{
					glColor3f(0,0,_is_current);
				}
				else glColor3ub(128,128,128);

				glVertex2i(left+4,middle);
				glVertex2i(left+13,middle);

				glColor3f(1,1,1);
				glVertex2i(left+4,1+middle);
				glVertex2i(left+13,1+middle);
			}
			else
			{
				glColor3f(1,1,1);
				glVertex2i(left+9,top+3);
				glVertex2i(left+9,bottom-4);
				glVertex2i(left+4,middle);
				glVertex2i(left+13,middle);

				if(enabled)
				{
					glColor3f(0,0,_is_current);
				}
				else glColor3ub(128,128,128);
		
				glVertex2i(left+4,-1+middle);
				glVertex2i(left+13,-1+middle);
				glVertex2i(left+8,top+3);
				glVertex2i(left+8,bottom-4);
			}
			glEnd();
	}

	if(this==GLUI::get_pressed()) draw_pressed();
}

/***************************** GLUI_Tree::update_size() **********/

void UI::Experiments::Tree::_update_size()
{
	Panel::_update_size();

	int delta_x = 0;
	if(_format&DISPLAY_HIERARCHY)
	delta_x = string_width(level_name);
	int text_size = string_width(name);

	w = std::max(w,text_size+36+delta_x);

	if(!is_open) h = UI_DEFAULT_HEIGHT + 7; //2019
}

/**************************** GLUI_Tree::draw_pressed() ***********/

void UI::Experiments::Tree::draw_pressed()
{
	int left = 5, right = w-left;
	int top = 3, bottom = 3+16;

	glColor3ub(0,0,0);
	draw_border_rect(left,top,right,bottom,0xFF);
}

//---.
}//<-'

#endif //GLUI_BUILD_TREE_CONTROLS
