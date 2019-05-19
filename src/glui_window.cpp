/*

  glui_window.cpp - GLUI_Button control class

  GLUI User Interface Toolkit
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

*/

#include "../include/glui_internal.h" //PCH

namespace GLUI_Library
{//-.
//<-'

/********* GLUI_Control::translate_to_origin() ********/

void Window::translate_to_origin(double x, double y, double z)
{
	glTranslated(x,y,z);
}

/********* GLUI_Main::set_ortho_projection() **********/

void Window::set_ortho_projection(int win_w, int win_h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* gluOrtho2D(0,win_w,0,win_h); */
	glOrtho(-0.5,win_w-0.5,win_h-0.5,-0.5,-1,1);

	glMatrixMode(GL_MODELVIEW);
}

/*************** GLUI_Control::set_font() **********/

static char glui_window_char_widths[7][256] = {};

Window::Font Window::_glut_fonts[7] = //const
{
	//REMINDER: GLUT_BITMAP_HELVETICA_12 is actually
	//a nice default since its tallest of those that
	//are small enough to be suitable as UI elements.
	//Not everything adapts to font-height presently.

	//glutBitmapHeight 
	//These hail from fg_font_data.c
	Font(16+2,GLUT_BITMAP_HELVETICA_12), //default
	Font(14+2,GLUT_BITMAP_HELVETICA_10),
	Font(23+3,GLUT_BITMAP_HELVETICA_18), //Odd squares focus rect.
	Font(14+2,GLUT_BITMAP_TIMES_ROMAN_10),
	Font(29+3,GLUT_BITMAP_TIMES_ROMAN_24), //Odd squares focus rect.
	Font(14+2,GLUT_BITMAP_8_BY_13),
	Font(16+2,GLUT_BITMAP_9_BY_15),
};

void Window::set_font(void *new_font, bool recurse)
{	
	UI::Control *c = dynamic_cast<UI::Control*>(this);

	if(!new_font) if(c&&c->ui&&c->ui->font)
	{
		new_font = c->ui->font;
	}
	else if(GLUT.font)
	{
		new_font = GLUT.font;
	}
	else goto use_default_font;

	int i; 
	for(i=0;i<7;i++)	
	if(new_font==_glut_fonts[i].font)
	{	
		break;
	}	
	if(i==7) use_default_font:
	{
		i = 0; new_font = _glut_fonts[0].font;
	}

	if(!_glut_fonts[i].width)
	{	
		char *w = glui_window_char_widths[i];
		(void*&)_glut_fonts[i].width = w;
		//Freeglut assigns various sizes to its nonprintable characters.
		//Let those be zero so they are detectable by the returned size.
		for(int i=32;i<256;i++)		
		w[i] = 0xFF&glutBitmapWidth(new_font,i);		
	}

	const_cast<Font&>(font) = _glut_fonts[i]; 
	
	if(c)
	{
		//GCC treats this as &Window::font, which just
		//seems wrong, since Control is in the name...
		//and Window is not even a direct base class.
		//if(recurse) c->set(&UI::Control::font,font);
		if(recurse) c->set((Font UI::Control::*)&UI::Control::font,font);

		c->redraw();
	}
}

/********** GLUI_Control::draw_string() ************/

void Window::draw_string(int x, int y, C_String str)
{
	//Instead of fiddling with the height at the call site.
	//This constant is used to adjust to font requirements. 
	enum{ adjust=8 };

	//2019: Clip bitmap at edges.
	//glRasterPos2i(x,y);
	glRasterPos2i(0,0);
	//OpenGL is ridiculous. Using glBitmap gets around the
	//error condition when the raster position lies beyond
	//the viewport, which otherwise clips the entire image.
	glBitmap(0,0,0,0,x,-y-font.height+adjust,0);

	draw_string(str.str);
}
void Window::draw_string(C_String str)
{
	if(!str.str) return;
	
	/* Displays the contents of a string using GLUT's bitmap character function 
	Does not handle newlines */

	#ifdef GLUI_FREEGLUT
	glutBitmapString(font.font,(const unsigned char*)str.str);
	#else
	for(const char*p=str.str;*p;p++) glutBitmapCharacter(font.font,*p);
	#endif
}
 
/************* GLUI_Control::draw_name() ***********/
/* This draws the name of the control as either black (if enabled), or       */
/* embossed if disabled.                                                     */

void Window::draw_active_name(int x, int y, C_String name, int active)
{
	if(!font.font||!name.str) return; 

	if(active==1)
	{
		int top = y-3;
		int bottom = top+font.height+1;
		assert((bottom-top)%2); //Odd squares focus rect.

		int left = x-3;
		int right = left+7+string_width(name);
		if((right-left)%2==0) right--; //Odd squares focus rect.	

		draw_active_rect(left,top,right,bottom);
	}

	if(active==-1) /* Control is disabled - emboss the string */
	{
		glColor3ub(255,255,255);
		//I think this is better perceptually, in vertical spacing terms.
		//draw_string(x+1,y+1,name);
		draw_string(x--,y--,name);
		glColor3f(0.4f,0.4f,0.4f);
	}
	else glColor3ub(0,0,0); draw_string(x,y,name);
}

/************************************ GLUI_Main::draw_raised_box() **********/

void Window::draw_raised_rect(int x1, int y1, int x2, int y2, int shadow)
{
	glColor3ub(255,255,255);
	draw_border_rect(x1,y1,x2,y2,1|8);

	glColor3ub(0,0,0);
	draw_border_rect(x1,y1,x2,y2,2|4);	
	
	glColor3ub(128,128,128);	
	draw_border_rect(x1+1,y1+1,x2-1,y2-1,2|4);

	glColor3ub(175,175,175);
	draw_border_rect(x1+1,y1+1,x2-1,y2-1,1|8);
	if(shadow)
	{
		//glColor3ub(175,175,175);
		draw_border_rect(x1,y1+1,x2+1,y2+1,shadow);
	}
}

/************************************ GLUI_Main::draw_lowered_box() **********/

void Window::draw_sunken_rect(int x1, int y1, int x2, int y2)
{
	glColor3ub(128,128,128);
	draw_border_rect(x1,y1,x2,y2-1,1|8);

	glColor3ub(0,0,0);
	draw_border_rect(x1+1,y1+1,x2-1,y2-2,1|8);

	glColor3ub(255,255,255);
	draw_border_rect(x1-1,y1,x2,y2,2|4); 	
	
	glColor3ub(175,175,175); 
	draw_border_rect(x1+1,y1+2,x2-2,y2-2,2|4); 
}

/******* GLUI_Control::draw_box() **********/

void Window::draw_filled_rect(int x1, int y1, int x2, int y2, int box_colors)
{	
	//PORTABLE?
	//glRect (polygons?) behave differently from LINES.
	//x2--; y2--;

	switch(box_colors)
	{
	case BOX_ACTIVE:

		glColor3f(0,0,0.6f); break;

	case BOX_CLEAR:

		glColor3ub(193,193,193); break; //background fill

		//Windows uses the old background (192~193) for this.
		
	default: return; //Background seems to look alright for disabling.

	case BOX_WHITE: //case -BOX_WHITE:

		//I don't know the right color but 80% is not enough contrast.
		//bool disable = box_colors<0;
		//float l = disable?0.8f:1; glColor3f(l,l,l); break; 
		glColor3f(1,1,1); break; 
	}		
	glRecti(x1,y1,x2,y2);
}
void Window::draw_framed_rect(int x1, int y1, int x2, int y2, int box_colors)
{	
	if(ABS(box_colors)!=BOX_CLEAR)
	{	
		draw_filled_rect(x1,y1,x2,y2,box_colors);
	}

	//draw_box_inwards_outline
	{	
		draw_sunken_rect(x1-2,y1-2,x2+3,y2+3);
	}
}

/**** GLUI_Control::draw_active_area() ********/

void Window::draw_active_rect(int x1, int y1, int x2, int y2)
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x5555);
	glColor3ub(0,0,0);

	//PORTABLE?
	//draw_border_rect must use LOOP or STRIP mode to work 
	//with STIPPLE on my system.
	draw_border_rect(x1,y1,x2,y2);

	glDisable(GL_LINE_STIPPLE);
}
void Window::draw_border_rect(int x1, int y1, int x2, int y2, int mask)
{
	x2--; y2--; /*if(0) //REFERENCE
	{
		glBegin(GL_LINE_LOOP);
		glVertex2i(x1,y1); glVertex2i(x2,y1);
		glVertex2i(x2,y2); glVertex2i(x1,y2);
		glEnd();
	}	
	else*/ //REMINDER: LINES doesn't work with STIPPLE.
	{
		int t = mask&1, r = mask&2, b = mask&4, l = mask&8;

		glBegin(GL_LINE_STRIP);
		if(t) glVertex2i(x1,y1);
		if(t||r) glVertex2i(x2,y1);
		if(t&&!r){ glEnd(); glBegin(GL_LINE_STRIP); }
		if(r||b) glVertex2i(x2,y2);
		if(r&&!b){ glEnd(); glBegin(GL_LINE_STRIP); }
		if(b||l) glVertex2i(x1,y2);		
		if(l) glVertex2i(x1,y1);
		glEnd();
	}

	if(mask>>=4) draw_border_rect(x1+1,y1+1,x2,y2,mask);
}

/**** GLUI_Control::draw_emboss_box() ********/

void Window::draw_etched_rect(int x1, int y1, int x2, int y2)
{
	glColor3ub(255,255,255);
	draw_border_rect(x1,y1,x2,y2,0xFF);

	glColor3ub(128,128,128);
	draw_border_rect(x1,y1,x2-1,y2-1);
}

void Window::draw_etched_bars(int x1, int y1, int x2, int y2, int mask)
{
	int xh = (x1+x2)/2+1, yh = (y1+y2)/2+1;

	int l = mask<0?128:0; if(l) mask = -mask;
	
	glBegin(GL_LINES);

	for(int c=255;;c=l,xh--,yh--)
	{
		glColor3ub(c,c,c);

		if(mask&1){ glVertex2i(xh,y1); glVertex2i(xh,y2); }
		if(mask&2){ glVertex2i(x1,yh); glVertex2i(x2,yh); }

		if(c==l) break;
	}

	glEnd();
}

//---.
}//<-'
