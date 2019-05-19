/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_mouse_iaction - GLUI Mouse Interaction control class


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

//TODO: XGrabPointer can offer better control, and maybe not interfere.
#ifdef GLUI_GLX
#define GLUI_NOCAPTURE //freeglut's XWarpPointer isn't quite cutting it.
#endif

/********************** GLUI_Mouse_Interaction::mouse_down_handler() ******/

static int glui_mouse_iaction_warp_x;
static int glui_mouse_iaction_warp_y; 

static bool glui_mouse_iaction_special = false;

bool Mouse_Interface::_mouse_stage(int stage, _iaction_iii mf, int local_x, int local_y, bool inside)
{
	if(!GLUI.init_inside) 
	{
		GLUI.curr_inside = false; return true;
	}
	else GLUI.curr_inside = true;

	if(1==stage)
	{
		if(local_y>UI_MOUSE_IACTION_WIDTH)
		{		
			assert(TOP&placement);
			GLUI.init_inside = 
			GLUI.curr_inside = false; return false;
		}

		glui_mouse_iaction_warp_x = local_x-x_center();
		glui_mouse_iaction_warp_y = local_y-y_center();

		(this->*mf)(1,glui_mouse_iaction_warp_x,glui_mouse_iaction_warp_y);

		//NEW: I think Rotation::needs_idle was putting the UI in constant
		//idle.
		if(needs_idle())
		GLUI::glui_setIdleFuncIfNecessary();

		//redraw(); //Enable click to select, maybe double-click to reset?
	}
	else if(2==stage)
	{
		int wx = 0, wy = 0; //EXPERIMENTAL

		bool shift = true;
		#ifndef GLUI_NOCAPTURE	
		shift = GLUI.curr_modifiers&GLUT_ACTIVE_SHIFT;
		#endif
		if(shift)
		{	
			glui_mouse_iaction_warp_x = local_x-x_center();
			glui_mouse_iaction_warp_y = local_y-y_center();
		}
		else
		{
			glui_mouse_iaction_warp_x+=wx=local_x-GLUI.down_x;
			glui_mouse_iaction_warp_y+=wy=local_y-GLUI.down_y;
			if(!glui_mouse_iaction_special)
			{
				glutSetCursor(GLUT_CURSOR_NONE); //2019: No spraycan cursor
				glutWarpPointer(GLUI.down_x,GLUI.down_y);
			}
			else wx = wy = 0;
		}	

		(this->*mf)(2,glui_mouse_iaction_warp_x,glui_mouse_iaction_warp_y);	

		//freeglut uses XWarpPointer that generates mouse events, which it
		//doesn't suppress.
		#ifdef GLUI_GLX
		//NOTE: This works, but it jams up after a while... I think events
		//just stop coming.
		//Maybe XGrabPointer (https://stackoverflow.com/questions/2792954)
		//can do it?
		glui_mouse_iaction_warp_x+=wx; glui_mouse_iaction_warp_y+=wy;
		#endif

		output_live(); execute_callback();
	}
	else if(3==stage)
	{	
		(this->*mf)(3,glui_mouse_iaction_warp_x,glui_mouse_iaction_warp_y);

		#ifndef GLUI_NOCAPTURE
		if(!glui_mouse_iaction_special)
		{
			if(~GLUI.curr_modifiers&GLUT_ACTIVE_SHIFT)
			{
				glutWarpPointer(GLUI.down_x,GLUI.down_y);
			}

			redraw();
		}
		#endif 
	}

	return false;
}	

/****************************** GLUI_Mouse_Interaction::draw() **********/

void Mouse_Interface::_draw(_iaction_i mf, int vpw, int vph)
{
	bool draw_inactive_area = !vpw;

	if(vpw<=0) if(!ui)
	{
		vpw = glutGet(GLUT_WINDOW_WIDTH); 
		vph = glutGet(GLUT_WINDOW_HEIGHT);
	}
	else ui->get_dims(&vpw,&vph);

	if(draw_inactive_area)
	{
		int x1 = w/2-string_width(name)/2-1;
		draw_name(x1,UI_MOUSE_IACTION_WIDTH+5,this==GLUI::get_active());		
	}	

	//draw_active_area();
	{
		int text_height = UI_MOUSE_IACTION_HEADER; /* kludge */

		int viewport_size = h-text_height; /* std::min(w,h); */

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//-1,-1 is just to center Rotation's ball.
		glTranslatef(x_abs+w/2-1,y_abs+viewport_size/2-1,0);

		/***   Draw the interaction control's orthographic elements   ***/
		(this->*mf)(0); //iaction_draw(0);

		/***   Setup and draw the interaction control's perspective elements   ***/

		int vpx = x_abs+(w-viewport_size)/2;
		int vpy = vph-y_abs-h+text_height;
		glViewport(vpx,vpy,viewport_size,viewport_size);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	
		double xy = 1, zc = 50; /* X-Y size, and Z origin */
		glFrustum(-xy,+xy,-xy,+xy,zc*0.7,zc*1.3);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslated(0,0,-zc);

		(this->*mf)(1); //iaction_draw(1);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glViewport(0,0,vpw,vph);
		set_ortho_projection(vpw,vph);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

/************************************ GLUI_Mouse_Interaction::update_size() **********/

void Mouse_Interface::_update_size()
{	
	int name_packed = name_width(true);

	w = std::max(w,name_packed+4);
	w = std::max(w,h-UI_MOUSE_IACTION_HEADER); //i.e. UI_MOUSE_IACTION_RADIUS

	//iaction_update_size();
}

/****************************** GLUI_Mouse_Interaction::special_handler() **********/

bool Mouse_Interface::_special_handler(int key, int modifiers)
{
	int drag_x = 0, drag_y = 0;

	switch(key)
	{
	case GLUT_KEY_LEFT: drag_x = -6; break;
	
	case GLUT_KEY_RIGHT: drag_x = 6; break;

	case GLUT_KEY_UP: drag_y = -6; break;

	case GLUT_KEY_DOWN: drag_y = 6; break;

	default: return true; //signal keycode unprocessed
	}

	if(drag_x||drag_y)
	{
		//HACK: Visualize arraow key?
		GLUI.spacebar_is_down = true; 

		glui_mouse_iaction_special = true;		
		GLUI::Click
		iaction(this,x_center(),y_center());
		iaction.drag(drag_x,drag_y);
		bool ret = 
		iaction.finish();
		glui_mouse_iaction_special = false;
		if(ret) return true;
	}
	return false;
}

//---.
}//<-'
