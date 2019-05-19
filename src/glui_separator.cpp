/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_separator.cpp - GLUI_Separator control class


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

/****************************** GLUI_Separator::draw() **********/

void UI::Separator::_draw()
{
	if(!get_type()) return;

	int width,_; get_column_dims(&_,&width);	

	//Making this interchangeable with Column (a horizontal column.)
	//int indent = width*0.05f;
	int indent = w?std::min(w,width):width;
	//int height = UI_SEPARATOR_HEIGHT/2;
	int height = h/2; 

	draw_etched_bars(indent-1,height-1,width-indent-1,height-1,-2);
}
		  
//---.
}//<-'
