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

static bool glui_panel_main(UI::Control *p)
{
	return !p->_parent_node&&p->ui&&p==p->ui->get_main_panel();
}

/****************************** GLUI_Panel::draw() **********/

void Box_Interface::_draw()
{
	int drop = -1; //UI_BOX_NAME_DROP+1; //9 

	int str_width = string_width(name);

	int top,bot,left = 0; if(box_type==RAISED)
	{
		top = 0; left = x_off; drop+=UI_YOFF;

		glColor3f(1,1,1);
		draw_border_rect(0,0,w,h-1,1|8);
		glColor3ub(128,128,128);
		draw_border_rect(-1,0,w,h,2|4);		
	}
	else if(box_type==ETCHED)
	{
		if(!name.empty())
		{
			drop-=2;
			top = UI_PANEL_EMBOSS_TOP;
		}
		else top = 0; bot = h;

		int right = w;

		if(glui_panel_main(this))
		{
			//Retain old style that chops off white edge.
			right++; bot++;
		}

		draw_etched_rect(0,top,right,bot);
		
		if(str_width) /* Only  draw non-null strings */
		{ 
			left = 8;
			draw_filled_rect(left-1,0,left+str_width+2,UI_BOX_NAME_DROP,BOX_CLEAR);
		}
	}
	else if(box_type) //BOX
	{	 
		top = y_off_top; bot = h-y_off_bot;
				
		Control *ch = first_child();		
		int colors = enabled?box_type:-box_type;
		int clear = enabled?BOX_CLEAR:-BOX_CLEAR;
		//-1 is because x_rl should be 3 but is 2.
		int x = x_lr, xx = w-x_rl-1, y = top, yy = bot;
		draw_framed_rect(x,y,xx,yy,ch?clear:colors);

		//Subtract children from fill area?
		int dx = 0, dxx = 0;
		for(;ch;ch=ch->next())
		{			
			int hzw = ch->w, hzh = ch->h; //hotzone
			ch->offset_dims(&hzw,&hzh); 
			if(alignment==ALIGN_BUDDY||ALIGN_LEFT!=ch->alignment)
			{
				dxx-=hzw;
			}
			else dx+=hzw;
		}						
		if(dx||dxx)
		{
			//The right margin omits a pixel
			//when adding scrollbars.
			if(dx) x+=dx+1; if(dxx) xx+=dxx;
			draw_filled_rect(x,y,xx,yy,colors);
		}
	} 

	if(!name.empty())
	{
		if(x_lr>x_off+2)
		{
			left = x_lr-str_width-UI_XOFF-1;
			if(left<0)
			{
				//NOTE: This may draw out-of-bounds.
				int cx,cw; get_column_dims(&cx,&cw);
				cx-=x_abs;
				left = std::max(cx,left);
			}
			drop = UI_BOXINNERMARGINX; //font.height; //13
		}
		draw_name(left,drop);
	}
}

/************************************** GLUI_Panel::update_size() **********/

void Box_Interface::_update_size()
{
	int text_size = string_width(name);

	w = std::max(w,text_size+16);

	switch(box_type)
	{
	case ETCHED: case RAISED: //Panel
	{
		y_off_top = UI_YOFF;
		if(!name.empty()) switch(box_type) 
		{
		case ETCHED: y_off_top+=8; break;
		case RAISED: y_off_top+=14; break;
		}
		y_off_top+=2;

		y_off_bot = x_lr = x_rl = x_off+2; 
				
		if(glui_panel_main(this)) //ETCHED?
		{
			y_off_bot--; x_rl--; 
		}

		break;
	}
	default: //BOX (EditText, TextBox, etc.)

		y_off_top = 0; if(!name.empty()) 
		{
			if(x_lr<x_off+2) 
			y_off_top = UI_DEFAULT_HEIGHT+UI_ITEMSPACING;
		}
		else x_lr = 2; //NEW: Eliminate name tag.

		x_lr = std::max(2,x_lr);

		//It's really 3 pixels but want to shift scrollbar/spinner.
		x_rl = 2; 
		
		y_off_top+=2; 
		
		y_off_bot = 3; //2
		
		break;

	case NONE: //Panel (NONE)

		if(!name.empty())
		y_off_top = std::max(y_off_top,UI_DEFAULT_HEIGHT+UI_ITEMSPACING);
	}	
}
		  
//---.
}//<-'
