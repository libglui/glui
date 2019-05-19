/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_translation - GLUI_Translation control class


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

static int glui_translation_locked = 0;

static double glui_tranlsation_orig_x, glui_tranlsation_orig_yz;

/*********************** GLUI_Translation::iaction_mouse() **********/

void UI::Translation::_iaction_mouse(int stage, int x, int y)
{
	if(1==stage)
	{
		if(trans_type==XY)
		{
			glui_tranlsation_orig_x = float_array_val[0];
			glui_tranlsation_orig_yz = float_array_val[1];

			/** Check if the Alt key is down, which means lock to an axis **/

			if(GLUI.curr_modifiers&GLUT_ACTIVE_ALT)
			{
				if(ABS(y)>ABS(x))
				{
					glui_translation_locked = 'X';
			//		glutSetCursor(GLUT_CURSOR_UP_DOWN);
				}
				else
				{
					glui_translation_locked = 'Y';
			//		glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
				}
			}
			else
			{
				glui_translation_locked = 0; //LOCK_NONE;

				//XC_spraycan (GLUT_CURSOR_SPRAY) is an airbrush can. 
			//	glutSetCursor(GLUT_CURSOR_SPRAY);
			}
		}
		else if(trans_type==X)
		{
			//glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
			glui_tranlsation_orig_x = float_array_val[0];
		}
		else //if(trans_type==Y) if(trans_type==Z)
		{
			//glutSetCursor(GLUT_CURSOR_UP_DOWN);
			glui_tranlsation_orig_yz = float_array_val[0];
		}
	}
	else if(2==stage)
	{	
		if(GLUI.spacebar_is_down) //glui_mouse_iaction_special?
		{
			//if(trans_type==XY)
			{
				if(!x) glui_translation_locked = 'X';
				if(!y) glui_translation_locked = 'Y';
			}
		}

		float xf = +speed*(x+x_center()-GLUI.down_x);
		float yf = -speed*(y+y_center()-GLUI.down_y);

		switch(GLUI.curr_modifiers)
		{
		case GLUT_ACTIVE_CTRL:		
			//TOO MUCH
			xf*=10; yf*=10; //xf*=100; yf*=100; 
			break;		
		case GLUT_ACTIVE_SHIFT:		
			//TOO LITTLE
			xf*=0.1f; yf*=0.1f; //xf*=0.01f; yf*=0.01f
			break;
		}

		float off_array[2]; switch(trans_type) 
		{		
		case XY:

			if('Y'==glui_translation_locked)
			{
				yf = 0;
			}
			else if('X'==glui_translation_locked)
			{
				xf = 0;
			}

			off_array[1] = yf+glui_tranlsation_orig_yz;
			case X:
			off_array[0] = xf+glui_tranlsation_orig_x;
			break;

		default:

			off_array[0] = yf+glui_tranlsation_orig_yz;
		}

		//Mouse_Interface is responsible for all but setting the value.
		//set_float_array_val(off_array);
		GLUI::set_edited(this),stage_live(off_array,2);
	}
	else if(3==stage)
	{	
		//glui_translation_locked = 0; //glui_mouse_iaction_special 
	}
}

/******************** GLUI_Translation::iaction_draw(0) **********/

void UI::Translation::_iaction_draw(int persp)
{
	if(persp!=0) return; //ortho only

	/********* Draw emboss circles around arcball control *********/

	//int radius = int((h-22)/2.0f); /* std::min(w/2.0f,h/2.0f); */
	int radius = (h-UI_MOUSE_IACTION_HEADER-4)/2;
	draw_etched_rect(-radius-2,-radius-2,radius+3,radius+3);
	draw_2d_arrows(radius-1);
}

/*************************** GLUI_Translation::draw_2d_arrow() **************/
/* orientation: 0=up, 1=left, 2=down, 3=right (note: not diagram notation)  */
/*                                                                          */
/*                                                                          */
/*                           0, y2                                          */
/*                      /            \                                      */
/*                     /              \                                     */
/*                    /                \                                    */
/*                   /                  \                                   */
/*                  /                    \                                  */
/*                 /                      \                                 */
/*                /                        \                                */
/*               /                          \                               */
/*            -x2,y1   -x1b,y1   x1b,y1     x2,y1                           */
/*                        |        |                                        */
/*                        |        |                                        */
/*                        |        |                                        */
/*                        |        |                                        */
/*                        |        |                                        */
/*                    -x1a,y0    x1a,y0                                     */
/*                                                                          */

inline void UI::Translation::_draw_2d_arrow(int radius, bool filled, int orientation)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(-1,1,1);

	float white[3] = {1,1,1};
	float black[3] = {0,0,0};
	float gray[3] = {0.45f,0.45f,0.45f};
	float bkgd[3] = {0.7f,0.7f,0.7f};	
	if(ui) memcpy(bkgd,ui->bkgd_color_f,sizeof(bkgd));
	float bkgd2[3] = {bkgd[0]+0.07f,bkgd[1]+0.07f,bkgd[2]+0.07f};
	float *bkgd3 = enabled?bkgd:bkgd2;

	/** The following 8 colors define the shading of an octagon,
	in clockwise order, starting from the upstroke on the left  **/
	/** This is for an outside and inside octagons **/
	float *colors_in[8] = { bkgd3,white,bkgd3,gray,gray,gray,gray,gray };
	float *colors_out[8] = { white,white,white,gray,black,black,black,gray };
	
	int c_rot = 0; /* color index offset */

	if(orientation==2)
	{
		c_rot = 4;		
	}
	else if(orientation==0)
	{
		c_rot = 0; glRotatef(180,0,0,1);
	}
	else if(orientation==1)
	{
		c_rot = 2; glRotatef(90,0,0,1);
	}
	else if(orientation==3)
	{
		c_rot = 6; glRotatef(-90,0,0,1);
	}

	float x1_= radius*0.2f;
	float x2 = x1_*2;
	float y0 = trans_type==XY?x1_:0;
	float y1 = radius*0.54f;
	float y2 = y1+x2;
	float x1a = x1_, x1b = x1_;		
	if(trans_type==Z)
	{
		float x = 0.75f; 

		if(orientation==0)
		{	
			//x1b-=2;  x1a+=2;			
			x1b*=x*0.8f; x1a/=x;

			//y1+=2; //y2+=0;
			x = y1; y1*=1.2f; //45 degrees

			//x2-=2;
			x2-=int(y1+0.5f)-x; //45
		}
		else if(orientation==2)
		{
			//x1a+=2; 
			x1a/=x;

			//y1-=6; 
			x = y1; y1/=2; //45 degrees 

			//x2+=6;
			x2-=int(y1+0.5f)-x; //45
			
			//x1b+=4;
			x1b*=1.7f;
		}
	}
	bool z = false; if(1) //EXPERIMENTAL
	{
		//Pretty good foreshortening. This is pretty cool
		//but I think I did it because the thin neck part
		//looked either too thin or too fat with the hard
		//pixel offests.
		if(z=(0&&Z==trans_type)) //DISABLED (0) 
		{
			y2*=orientation?1.2f:1;

			float x = 0.5f;
			y1*=x; y2*=x;

			x = 1.8f;
			x1a*=x;			
			if(orientation)
			{
				x2*=x; x1b*=x;
			}
		}

		//Fixing to hard pixel boundaries before applying
		//1px offsets below.
		//x1_ = int(x1+0.5f);
		x2 = int(x2+0.5f);
		y0 = int(y0+0.5f);
		y1 = int(y1+0.5f);
		y2 = int(y2+0.5f);
		x1a = int(x1a+0.5f);
		x1b = int(x1b+0.5f);
	}

	if(!enabled) /*** Check if control is enabled or not ***/
	{
		c_rot+=4; /* Indents the shadows - goes from a raised look to embossed */
	
		/*This drew an outline/ridge/etch effect, however the lines 
		//aren't really offset +1,+1 and if they were the 45 degree
		//diagonals would not look so hot.
		colors_out[0] = colors_out[1] = colors_out[2] = colors_out[7] = white; //gray
		colors_out[3] = colors_out[4] = colors_out[5] = colors_out[6] = gray; //white
		colors_in[0] = colors_in[1] = colors_in[2] = colors_in[7] = gray; //white
		colors_in[3] = colors_in[4] = colors_in[5] = colors_in[6] = white; //gray
		*/

		//2019: ??? (I guess this was for front-buffer drawing)
		goto disabled; //set_to_bkgd_color();
	}
	else if(!filled) /*** Fill in inside of arrow  ***/
	{
		/*** Means button is up - control is not clicked ***/
		/*glColor3f(0.8f,0.8f,0.8f); */

		glColor3fv(bkgd2); //set_to_bkgd_color();
	}
	else /*** Button is down on control ***/
	{
		glColor3f(0.6f,0.6f,0.6f);

		c_rot+=4; /* Indents the shadows - goes from a raised look to embossed */
	}	

	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(-x1a,0);
	glVertex2f(-x1a,0);
	glVertex2f(-x1b,y1);
	glVertex2f(x1b,y1);
	glVertex2f(x1a,0);
	glVertex2f(x1a,0);
	glVertex2f(0,0);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(-x2,y1);
	glVertex2f(0,y2);
	glVertex2f(x2,y1);
	glEnd();

	disabled: //2019

	bool z2 = z; (orientation?z:z2) = false;

	/*** Draw arrow outline ***/
	glBegin(GL_LINES); 
	//for(int pass=1;pass<=(enabled?1:2);pass++) //2019
	{		
	//The 2 passes puts the white lines underneath the gray lines.
	float *col,*sel = 0;//enabled?0:pass==1?white:gray; 
	#define SET_COL(io,i) \
	if(sel!=(col=colors_##io[(i+c_rot)%8])&&sel) col = 0;
	#define DRAW_SEG(xa,ya,xb,yb) \
	if(col){ glColor3fv(col); glVertex2f(xa,ya); glVertex2f(xb,yb); }\
	if(col){ glVertex2f(xb,yb); glVertex2f(xa,ya); } //double ends?

	/*if(0) //REFERENCE
	{
	//Some of these offsets look sloppy, but may be intentional?
	SET_COL(in,1) DRAW_SEG(0,y2-1,-x2,y1-1)
	SET_COL(in,6) DRAW_SEG(-x2+2.0f,y1+1,-x1b+1,y1+1)
	SET_COL(in,0) DRAW_SEG(-x1b+1,y1+1,-x1a+1,y0)
	SET_COL(in,3) DRAW_SEG(0,y2-1,x2,y1-1)
	SET_COL(in,6) DRAW_SEG(x2-1,y1+1,x1b-1,y1+1)
	SET_COL(in,4) DRAW_SEG(x1b-1,y1+1,x1a-1,y0)
	//
	SET_COL(out,0) DRAW_SEG(-x1a,y0,-x1b,y1);
	SET_COL(out,6) DRAW_SEG(-x1b,y1,-x2,y1);
	SET_COL(out,1) DRAW_SEG(-x2,y1,0,y2);
	SET_COL(out,3) DRAW_SEG(0.0,y2,x2,y1);
	SET_COL(out,6) DRAW_SEG(x2,y1,x1b,y1);
	SET_COL(out,4) DRAW_SEG(x1b,y1,x1a,y0);
	}
	else //EXPERIMENTAL*/ //Seems better quality.
	{
		if(z) goto z;
	SET_COL(in,1) DRAW_SEG(0+0,y2-1,-x2+2,y1+1) //flipping		
	SET_COL(in,3) DRAW_SEG(0+0,y2-1,x2-2,y1+1)
		z:
		if(z) goto z3;
	SET_COL(out,1) DRAW_SEG(0,y2,-x2,y1) //flipping		
	SET_COL(out,3) DRAW_SEG(0,y2,x2,y1)
		z3:

	int l = 1;
	if(bkgd3==colors_in[(6+c_rot)%8]) l = 3;

	int ll = XY==trans_type?1:0;

	SET_COL(in,0) DRAW_SEG(-x1b+1,y1+1,-x1a+1,y0+ll) //flipping	
	SET_COL(in,6) DRAW_SEG(-x2+l,y1+1,-x1b+1,y1+1) //flipping	
	SET_COL(in,6) DRAW_SEG(x2-l,y1+1,x1b-1,y1+1)
	SET_COL(in,4) DRAW_SEG(x1b-1,y1+1,x1a-1,y0+ll)		
	
		if(Z==trans_type) //Fill in gaps.
		{
			float l = 0.5f;
	SET_COL(in,0) DRAW_SEG(-x1b+l,y1,-x1a+1,y0) //flipping	
	SET_COL(in,4) DRAW_SEG(x1b-l,y1,x1a-1,y0)	
		}

		if(!z2) //Saving for end?
		{	
	SET_COL(out,0) DRAW_SEG(-x1b,y1,-x1a,y0) //flipping	
	SET_COL(out,6) DRAW_SEG(-x2,y1,-x1b,y1) //flipping	
		}
		else //Make point 2 pixels tall.
		{			
	SET_COL(out,1) DRAW_SEG(0,y2+1,-x2,y1) //flipping	
	SET_COL(out,3) DRAW_SEG(0,y2+1,x2,y1)
		}
	//-1 is not connecting in this order??? But flipping 
	//its line makes the other end break.
	SET_COL(out,6) DRAW_SEG(x2,y1,x1b,y1) //doubling
	//SET_COL(out,6) DRAW_SEG(x1b,y1,x2,y1) //doubling flipped	
	SET_COL(out,4) DRAW_SEG(x1b,y1,x1a,y0) //doubling
	//SET_COL(out,4) DRAW_SEG(x1a,y0,x1b,y1) //doubling flipped

		if(z2) //front of forshortened arrow
		{
	//Dawing these last for forshortened Z to be drawn
	//in front of the frame.
	SET_COL(out,0) DRAW_SEG(-x1b,y1,-x1a,y0) //flipping	
	SET_COL(out,6) DRAW_SEG(-x2,y1,-x1b,y1) //flipping	
		}
	}

	#undef SET_COL
	#undef DRAW_SEG
	}
	glEnd();

	glPopMatrix();
}
 
/*************************** GLUI_Translation::draw_2d_arrows() **************/

void UI::Translation::draw_2d_arrows(int radius)
{	
	bool press = this==GLUI::get_pressed();
	
	int o1, o2; if(trans_type==Z)
	{
		o1 = 2; o2 = 0; //draw_2d_z_arrows(radius);
	}
	else if(trans_type==XY) //draw_2d_xy_arrows(radius);
	{	
		int lock = press?glui_translation_locked:0;

		bool filled = false; if(!lock)
		{
			//filled = trans_mouse_code!=MOUSE_NONE;
			filled = press;

			o1 = 0; o2 = 1;
		}
		else if(lock=='Y') //LOCK_X
		{
			o1 = 0; o2 = 2; //Y
		}
		else if(lock=='X') //LOCK_Y
		{
			o1 = 1; o2 = 3; //X
		}
		_draw_2d_arrow(radius,filled,o1);
		_draw_2d_arrow(radius,filled,o2);

		if(lock=='Y') goto X; //LOCK_X
		if(lock=='X') goto Y; //LOCK_Y
		
		o1 = 2; o2 = 3;		
	}
	else if(trans_type==X) X:
	{
		o1 = 1; o2 = 3; //draw_2d_x_arrows((int)radius-1);
	}
	else if(trans_type==Y) Y:
	{
		o1 = 0; o2 = 2; //draw_2d_y_arrows((int)radius-1);
	}

	_draw_2d_arrow(radius,press,o1);
	_draw_2d_arrow(radius,press,o2);
}
 
//---.
}//<-'
