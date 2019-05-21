/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_spinner.cpp - GLUI_Spinner class


  notes:
	 spinner does not explicitly keep track of the current value - this is all
		handled by the underlying edittext control
		-> thus, spinner->sync_live() has no meaning, nor spinner->output_live
	-> BUT, edittext will alter this spinner's float_val and int_val,
	   so that spinner->get/set will work


FIXME: there's a heck of a lot of duplication between this and glui_scrollbar.cpp.
  (OSL, 2006/06)


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

static int glui_spinner_cogwheel = 0; //2019

/*********************************** GLUI_Spinner:_reset_growth() *************/

double &Spin_Interface::_growth(int inc)
{
	//CAUTION: This code just preserves the old code's behavior.
	//My sense is it can use a major review.

	double &g = GLUI._spin_growth; if(!inc) return g;

	double lim = fabs(val_end-val_start)/UI_SPIN_MIN_GROWTH_STEPS;
	
	if(inc<=-1000) //_reset_growth?
	{
		//2019: I'm combining these so Spin_Interface::_activate 
		//can call _reset_growth. (Actually that wasn't possible
		//but I've still combined the functionality.)

		if(UI::EditText *et=edittext) //Spinner like?
		if(1==et->get_line_limit()&&data_type==et->data_type)
		{
			if(!et->has_limits)
			{
				if(data_type==SPIN_FLOAT)
				{
					g = sqrt(fabs(et->float_val))*0.05;
				}
				else if(data_type==SPIN_INT)
				{
					g = 0.4;
				}
				else goto zero; //2019
			}
			else if(data_type==SPIN_FLOAT||data_type==SPIN_INT)
			{
				g = lim;
			}
			else zero: //2019
			{
				g = 0; assert(data_type==SPIN_FLOAT);
			}

			goto g; //if(!g) g = 0.001;
		}
		//ScrollBar::_reset_growth() //ScrollBar like?
		{
			//FIXED? BASICALLY THIS SEEMS TO DISABLE increase_growth.
			//g = fabs(val_end-val_start)/double(UI_SPIN_GROWTH_STEPS);
			g = lim;
			if(data_type==SPIN_INT&&g<1) 
			g = 1;
		}
	}
	else //_increase_growth?
	{
		/*REFERENCE
		//if(growth<(val_end-val_start/UI_SPIN_MIN_GROWTH_STEPS))
		if(growth<lim)
		{
			growth*=growth_exp;
		}
		return growth; 
		*/

	//TODO: The "growth" should deplete with elapsed time.
	//And somehow wheel events should manage reset_growth.

		double exp = GLUI._spin_growth_exp;

		if(inc<0){ exp = 1/exp; inc = -inc; } //NEW

		if(inc>0&&g>=lim) goto g; //HACK: This is how the old code worked.

		while(inc-->0) g*=exp;
	}

g:	if(g<0.001) g = 0.001; 
	
	//if(g>lim) g = lim; //This is not how the old code actually worked.
	
	return g; 
}

/****************************** GLUI_Spinner::mouse_down_handler() **********/

bool UI::Spinner::_mouse_down_handler(int local_x, int local_y)
{
	state = find_arrow(local_x,local_y);
	if(!state) return true; //2019
	
	if(needs_idle())
	GLUI::glui_setIdleFuncIfNecessary(true);

	/* printf( "spinner: mouse down  : %d/%d   arrow:%d\n",local_x,local_y,find_arrow(local_x,local_y)); */

	if(state!=STATE_UP&&state!=STATE_DOWN)
	{
		assert(0); //SCROLL?

		return false; //return true; //???
	}
	
	_reset_growth();

	//NOTE: I'm going to try to add this logic to do_click.
	//2019: Either do_click should set the value or it shouldn't be also
	//taken. The demos have no problem incrementing... in fact they have
	//the opposite problem.
	/*** ints and floats behave a bit differently.  When you click on
	an int spinner, you expect the value to immediately go up by 1, whereas
	for a float it'll go up only by a fractional amount.  Therefore, we
	go ahead and increment by one for int spinners ***/
	//if(data_type==SPIN_INT)	
	//if(double step=state==STATE_UP?1:STATE_DOWN?-0.9:0) //0.9???
	//{
	//	set_float_val(float_val+step); 
	//}
	
	glui_spinner_cogwheel = 0;

	do_click();

	//2019: This is new... it should probably be GLUT_ELAPSED_TIME based.
	glui_spinner_cogwheel = 45;
	
	return false;
}

/******************************** GLUI_Spinner::mouse_up_handler() **********/

bool UI::Spinner::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(edittext) //2019
	{
		edittext->activate(ACTIVATE_MOUSE); 
	}
	state = STATE_NONE;

	//2019: The idle function should now do this automatically.
	//GLUI::glui_setIdleFuncIfNecessary();

	/* printf("spinner: mouse up  : %d/%d    inside: %d\n",local_x,local_y,inside); */

	//This is done automatically.
	//glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	redraw();

	//Make of this what you will???
	/* do_callbacks(); --- stub */
	/* if(callback) callback(user_id); */ return false;
}

/***************************** GLUI_Spinner::mouse_held_down_handler() ******/

static int glui_spinner_last_y = 0;
bool UI::Spinner::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	if(state==STATE_NONE) return false;
	
	/* printf("spinner: mouse held: %d/%d    inside: %d\n",local_x,local_y,inside); */

	if(state==STATE_BOTH) /* dragging? */
	{
		do_drag(local_x,local_y);
	}
	else /* not dragging */
	{
		int new_state = find_arrow(local_x,local_y);

		if(new_state==state)
		{
			/** Still in same arrow **/
			do_click();
		}
		else
		{
			//if(inside||1) //2019: ???
			{
				/** The state changed, but we're still inside - that
				means we moved off the arrow: begin dragging **/
				state = STATE_BOTH;
			}
			//else //???
			{
				/*** Here check y of mouse position to determine whether to
				  drag ***/

				/* ... */
			}
		}

		/*** We switched to up/down dragging ***/
		if(state==STATE_BOTH)
		{
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
			
			//_last_x = local_x; 
			glui_spinner_last_y = local_y;

			/** If the spinner has limits, we reset the _growth value, since
			_reset_growth() will compute a new _growth value for dragging
			vs. clicking.  If the spinner has no limits, then we just let the
			_growth remain at whatever the user has incremented it up to **/
			if(LIMIT_NONE!=edittext->has_limits)
			{
				_reset_growth();
			}
		}
	}

	return false;
}
	
/****************************** GLUI_Spinner::draw() **********/

void UI::Spinner::_draw()
{
	int x1 = UI_SPINNER_ARROW_Y;
	int x2 = w-x1;
	int y1 = x1;
	int y2 = y1+UI_SPINNER_ARROW_HEIGHT;

	int i,j; if(!enabled)
	{
		i = Bitmap::SPINNER_UP_DIS;
		j = Bitmap::SPINNER_DOWN_DIS;
	}
	else
	{
		i = state==STATE_UP||state==STATE_BOTH?
		Bitmap::SPINNER_UP_ON:Bitmap::SPINNER_UP_OFF;
		j = state==STATE_DOWN||state==STATE_BOTH?
		Bitmap::SPINNER_DOWN_ON:Bitmap::SPINNER_DOWN_OFF;
	}
	std_bitmaps[i]->draw(x1,y1);
	std_bitmaps[j]->draw(x1,y2);
	
	/*2019: Spin_Interface::_activate is preventing tab
	//navigation. Page, Arrow, Home, End should be used.
	//if(active)
	if(this==GLUI::get_active())
	draw_active_box(w-UI_SPINNER_ARROW_WIDTH-2,w,0,h);
	*/

	//Shadow effect for bitmaps?
	/*Note: The difference is neglible for buddy spinners.
	//(And that currently all spinners are buddy spinners.)
	if(i==Bitmap::SPINNER_UP_OFF||i==Bitmap::SPINNER_UP_DIS)
	{
		glColor3ub(175,175,175);
		draw_border_rect(x1,y1+1,x2+1,y2+2,2);	
	}
	if(j==Bitmap::SPINNER_DOWN_OFF||j==Bitmap::SPINNER_DOWN_DIS)
	{
		y1+=UI_SPINNER_ARROW_HEIGHT;
		y2+=UI_SPINNER_ARROW_HEIGHT;
		glColor3ub(175,175,175);
		draw_border_rect(x1,y1+1,x2+1,y2+1,2|4);	
	}*/
}

/********************************* GLUI_Spinner::special_handler() **********/

bool Spin_Interface::_special_handler(int key, int modifiers)
{
	//NOTE: It's currently not really possible to do key input
	//into a Spinner becaue the focus goes to the text control.

	if(key>=GLUT_KEY_LEFT&&key<=GLUT_KEY_DOWN)
	{
		int x,y;
		if(horizontal!=(key==GLUT_KEY_UP||key==GLUT_KEY_RIGHT))
		{
			x = y = 3; goto xy;
		}
		else if(horizontal!=(key==GLUT_KEY_DOWN||key==GLUT_KEY_LEFT))
		{
			x = horizontal?w-3:3;
			y = !horizontal?h-3:3;	
			xy:
			x+=x_abs; y+=y_abs;

			glui_spinner_cogwheel = 0; //NEW

			if(GLUI::Click(this,x,y))
			return true;
		}
	}
	else if(key==GLUT_KEY_HOME)
	{ 
		/** Set value to limit top - or increment by 10 **/

		//???
	}
	else if(key==GLUT_KEY_END)
	{
		//???
	}
	else return true; //signal keycode unprocessed

	return false; //return true; //2019
}
 
/************************************ GLUI_Spinner::update_size() **********/

void UI::Spinner::_update_live()
{
	if(edittext)
	{
		if(data_type==SPIN_INT)
		{
			if(int_val!=edittext->int_val)
			edittext->set_int_val(int_val);
		}
		else if(float_val!=edittext->float_val) 
		{
			edittext->set_float_val(float_val);
		}
	}
}

/************************************ GLUI_Spinner::find_arrow() ************/

int UI::Spinner::find_arrow(int local_x, int local_y)
{
	local_x-=x_abs; local_y-=y_abs;

	if(horizontal)
	{
		assert(0);
	}
	else if(local_x>=0&&local_x<=w)
	{
		if(local_y>=0&&local_y<=UI_SPINNER_ARROW_Y+UI_SPINNER_ARROW_HEIGHT)
		{
			return STATE_UP;
		}
		if(local_y>=UI_SPINNER_ARROW_Y+UI_SPINNER_ARROW_HEIGHT&&local_y<=h)
		{
			return STATE_DOWN;
		}
	}

	return STATE_NONE;
}

/***************************************** GLUI_Spinner::do_click() **********/

void UI::Spinner::do_click()
{		
	//NEW: In case GLUT fails to send mouse-up event.
	if(!GLUI.curr_button_down)
	{
		state = STATE_NONE; 
		
		redraw(); return; 
	}
	
	if(glui_spinner_cogwheel--) return;

	glui_spinner_cogwheel = 10;
	
	double cm_factor = 1;
	switch(GLUI.curr_modifiers)
	{
	case GLUT_ACTIVE_SHIFT: cm_factor = 10; break; //100 //TOO MUCH
	case GLUT_ACTIVE_CTRL: cm_factor = 0.1; break; //0.01 //TOO LITTLE
	}

	double cmp = float_val; //limits

	//if(data_type==SPIN_FLOAT||1) //???
	if(int dir=state==STATE_UP?1:state==STATE_DOWN?-1:0)
	{
		double inc = _increase_growth()*dir*cm_factor*speed;

		double new_val = float_val+inc;

		if(data_type==SPIN_INT) 
		{
			while(int_val==(int)new_val) //NEW
			{
				new_val+=inc;
			}
			new_val = (int)new_val; //NEW
		}
		
		/** Remember, edittext mirrors the float and int values ***/

		set_float_val(new_val);
	}

	if(cmp!=float_val) do_callbacks();
}

/***************************************** GLUI_Spinner::do_drag() **********/

void UI::Spinner::do_drag(int x, int y)
{
	double cm_factor = 1;
	switch(GLUI.curr_modifiers)
	{
	case GLUT_ACTIVE_SHIFT: cm_factor = 100; break;
	case GLUT_ACTIVE_CTRL: cm_factor = 0.01; break;
	}

	/* delta_x = x-_last_x; */
	int delta_y = -(y-glui_spinner_last_y);

	double cmp = float_val; //limits

	//if(data_type==SPIN_FLOAT||1) //???
	{
		/** Remember, edittext mirrors the float and int values ***/

		double incr = _growth()*delta_y*cm_factor*speed;
		set_float_val(float_val+incr);
	}

	//_last_x = x; 
	glui_spinner_last_y = y; //_last_y = y; 

	if(cmp!=float_val) do_callbacks();
}

/************************************ GLUI_Spinner::do_callbacks() **********/

void Spin_Interface::do_callbacks()
{
	/*** This is not necessary, b/c edittext automatically updates us ***/	
	//if(!edittext) return;
	//float_val = edittext->float_val; int_val = edittext->int_val;

	/********************************************/
		
	//Merging ScrollBar logic 
	//Note: Historically Spinner does not set object_cb
	
	//2019 addition
	//I'm changing this so both callbacks are processed
	//in order to be consistent with List. I can't make
	//an argument for why to disable the user callbacks.
	//if(0)
	{
		execute_callback(); //old way for Spinner
	}
	//else //ScrollBar style logic
	{
		//Have to reverse the sense to make it work
		//if(!associated_object) 
		if(!object_cb) 
		{
	//		execute_callback();
		}
		else // Use internal Callbacks
		{ 
			//if(object_cb)
			{
				//object_cb(associated_object,int_val);
				object_cb(this);
			}
		}
	}
}

//---.
}//<-'
