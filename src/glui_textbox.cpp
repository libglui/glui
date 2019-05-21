/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_textbox.cpp - GLUI_TextBox control class


		  --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher, 2004 John Kew

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

extern void glui_list_scrollbar_callback(UI::ScrollBar *sb);
static void glui_textbox_scrollbar_callback(UI::ScrollBar *sb)
{
	UI::TextBox *my = dynamic_cast<UI::TextBox*>(sb->associated_object);
	if(!my){ assert(0); return; }

	//This used to have some code to adjust curr_line by dragging it
	//along with the scrollbar attached to the first or last visible
	//line. curr_line was not used or maintained.

	my->update_and_draw_text();
}

/****************************** GLUI_TextBox::common_construct() **********/

UI::ScrollBar *UI::Control::scrollbar_init(CB cb)
{
	ScrollBar *sb = scrollbar;
	
	if(sb&&!sb->data_type)
	{
		sb->data_type = sb->SPIN_INT;
		sb->alignment = sb->horizontal?LEFT:RIGHT;

		(sb->horizontal?h:w)+=UI_SCROLL_ARROW_SIZE;

		sb->set_object_callback(cb,this);
	}

	return sb;	
}

/******************************** GLUI_TextBox::draw_insertion_pt() *********/

namespace glui_textbox_gl_caret
{	
	//WARNING: I threw this together pretty quickly one
	//day when I realized this is one way to do a caret. 

	enum{ ms=500 };
	
	static int x,y,h;
	
	static int sync = 0;

	static int xor_bit = 0;

	static void xor_draw(int i=1)
	{
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);
		{
			glBegin(GL_LINES);
			glColor3ub(255,255,255);
			while(i-->0)
			{
				glVertex2i(x,y);
				glVertex2i(x,y+h);			
			}
			glEnd();
		}
		glDisable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY);	
	}

	static void timer(int cmp)
	{
		if(!h||cmp!=sync||!GLUI.active_control_ui) 
		{
			return;
		}

		xor_bit = !xor_bit;

		int old = glutGetWindow();
		glutSetWindow(GLUI.active_control_ui->get_glut_window_id());
		{
			int swap;		
			glGetIntegerv(GL_DRAW_BUFFER,&swap);
			glDrawBuffer(GL_FRONT);
			{
				xor_draw();			
			}
			glFlush();
			glDrawBuffer(swap); 
		}
		glutSetWindow(old);

		glutTimerFunc(ms,timer,cmp);
	}

	extern bool callback(UI::Control *cc, int ch, int cx, int cy, bool)
	{	
		//NOTE: I think deactivated controls will send 0,0,0.

		if(x!=cx||y!=cy)
		{
			x = cx; y = cy; h = ch; xor_bit = 0;
			sync++;
			if(ch) glutTimerFunc(ms,timer,sync);
		}
		if(ch)
		{
			x-=cc->x_abs; y-=cc->y_abs; xor_draw(xor_bit+1);
			x+=cc->x_abs; y+=cc->y_abs;
		}	
		else sync++; return false;
	}
}

void UI::Control::pos_caret(int h, int x, int y)
{	
	int m[4]; get_margin_dims(m);

	if(!ui) draw:
	{
		x+=m[0]; y+=m[1];
		glColor3ub(0,0,0);
		glBegin(GL_LINES);	
		glVertex2i(x,y); glVertex2i(x,y+h); 
		glEnd();
		return;
	}
	else if(this==GLUI::get_caret()) 
	{
		x+=GLUI.caret_x; y+=GLUI.caret_y;

		assert(this==ui->_active_control);
	}
	else if(this==GLUI::get_active())
	{
		if(-1!=GLUI.prev_caret_pt)
		{
			GLUI.prev_caret_pt = -1;
			h = 0; 		
		}
		else return;
	}
	else return;
	
	int cx = 0, cy = 0;

	if(x>=0&&y>=0&&x<m[2]&&y+h<=m[3]) 
	{
		int cw = cx+ui->_w;
		int ch = cy+ui->_h;

		cx+=x+m[0]+x_abs;
		cy+=y+m[1]+y_abs;
		if(cx<0||cy<0||cx>=cw||cy>=ch) 
		{
			h = 0;
		}
	}
	else h = 0;
				   
	if(!GLUI.caret_callback||GLUI.caret_callback(this,h,cx,cy,!enabled)) 
	{
		if(h>0) goto draw;
	}
}
 
/****************************** GLUI_TextBox::draw() **********/

void UI::TextBox::_draw()
{
	_dump("-> DRAW");

	Box_Interface::_draw();

	_CLAMP_substring(); 

	/* Begin Drawing Lines of Text */
	//substring_start = substring_end = 0;
	int sol,sss = substring_start;
	int eol,sse = substring_end;
	int len = get_char_count();

	int box_width = get_box_width();

	int lh = get_line_height();
	
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _find_insertion_pt.
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _find_insertion_pt.
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _find_insertion_pt.

	int y = 0; for(sol=eol=sss;eol<sse;y+=lh)
	{
		int tw,sw = 0; do
		{
			if(tw=substring_tab_width(sw,eol))
			{
				sw+=tw;

				if(substring_wrap_compare(sw,box_width)) 
				break;
			}
			else break; /* Skip newline */

		}while(++eol<len);

		if(!tw) switch(text[eol]) //UNFINISHED
		{
		//case '\n': case '\r': case '\t':
		
		default: eol++; /* Skip newline */
		}
		else if(eol<len) //NEW: wrap on word.
		{
			eol = substring_wrap(sol,eol);
		}

		substring_start = sol;
		substring_end = eol;
		draw_substring(y);

		sol = eol;
	}

	//EXPERIMENTAL
	//Record meaningful substring for TextBox when not drawing.
	//substring_start = swap_sstart; substring_end = swap_send;
	substring_start = sss; substring_end = sse;

	draw_insertion_pt();

	_dump("-> DRAW");
}

/************************************ GLUI_TextBox::update_size() **********/

void UI::TextBox::_update_size()
{			
	ScrollBar *sb = scrollbar_init(glui_textbox_scrollbar_callback);

	if(tab_width<=0) //set_tab_width?
	{
		if(!tab_width)
		{
			if(font.width) tab_width = 4*char_width(' '); 
			else assert(0);
		}
		else tab_width = -tab_width; redraw();
	}

	Box_Interface::_update_size();

	int mw = get_box_width();
	if(mw<UI_TEXTBOX_MIN_TEXT_WIDTH)
	{
		offset_dims(&mw,NULL);		
		if(sb&&!sb->hidden) 
		mw+=UI_SCROLL_ARROW_SIZE;
		w = mw;
	}

	//NEW: Moving out of draw routine.
	if(int lh=font.height)
	{
		int ih = h-UI_BOXINNERMARGINX*2;
		inset_dims(NULL,&ih);

		int visible_lines = ih/lh; //visible_lines

		//2019: Rounding up to lines count.
		h = visible_lines*lh;
		offset_dims(NULL,&h);

		h+=UI_BOXINNERMARGINX*2;
	}

	update_substring_bounds();
}

/****************************** GLUI_TextBox::set_text() **********/

void UI::TextBox::_update_live()
{
	substring_start = 0;
	substring_end = get_char_count();
	GLUI::reset_caret(this);
	sel_start = sel_end = 0;
	//start_line = curr_line = num_lines = 0;
	if(scrollbar) scrollbar->set_range();

	//HACK: UI_TEXTBOX_HEIGHT is very small. 
	//This delays update_string_bounds until
	//update_size. It will be wrong without
	//tabs anyway.
	if(tab_width) update_substring_bounds();
}
	
//---.
}//<-'
