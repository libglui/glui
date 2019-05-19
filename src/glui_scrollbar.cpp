/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_scrollbar.cpp - GLUI_Scrollbar class

          --------------------------------------------------

  Copyright (c) 2004 John Kew, 1998 Paul Rademacher

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

/****************************** GLUI_Scrollbar::common_construct() **********/

void UI::ScrollBar::_members_init()
{
	//horizontal = true;
	//h = UI_SCROLL_ARROW_SIZE;
	//w = UI_TEXTBOX_WIDTH;	
	if(horizontal)
	{
		h = UI_SCROLL_ARROW_SIZE;
		w = UI_TEXTBOX_WIDTH;
	}
	else
	{
		h = UI_TEXTBOX_HEIGHT;
		w = UI_SCROLL_ARROW_SIZE;
	}
	alignment = CENTER;
	placement = BOTTOM;
	//x_off = 0;
	//y_off_top = 0;
	//y_off_bot = 0;
	can_activate = true;
	spacebar_mouse_click = false; //2019
	
	val_end = val_start = 0;
	associated_object = NULL;
	//last_update_time = 0;
	//velocity_limit = 50; /* Change value by at most 50 per second */
	//box_length = 0;
	//box_start_position = 0;
	//box_end_position = 0;
	//track_length = 0;
	
	switch(live_type) 
	{
	case UI_LIVE_FLOAT: case UI_LIVE_DOUBLE:
		
		data_type = SPIN_FLOAT;
	}

	// make sure limits are wide enough to hold live value

	if(ptr_val) live_type->init_live();

	double lo = 0;
	double hi = data_type==SPIN_FLOAT?1:100;
	if(ptr_val) lo = std::min(lo,float_val);
	if(ptr_val) hi = std::max(hi,float_val);
	set_range(lo,hi);
}
inline int UI::ScrollBar::_box_len()
{
	if(val_end!=val_start)
	{
		const int min_tab = UI_SCROLL_BOX_STD_HEIGHT;
		////box_length = int(track_length/double(visible_range));
		////if(box_length<min_tab)
		//box_length = min_tab;
		return min_tab; 
	}
	else 
	{
		//Not sure if the idea here was to make the button long.
		//But the result was to not display the button... until
		//I broke draw_scroll into 2 parts. Then it was visible.
		////box_length = track_length;
		//box_length = box_start_position = 0; goto zero_divide;
		return 0;
	}
}
inline int UI::ScrollBar::_box_pos()
{
	if(val_end==val_start) return 0;

	double pixels_per_unit = //division-by-zero
	(_track_len()-_box_len())/(val_end-val_start);

	if(horizontal) //WHY DO THESE DIFFER IN THIS RESPECT???
	{
		//box_start_position =
		return int((float_val-val_start)*pixels_per_unit);
	}
	else 
	{
		//box_start_position = 
		return int((val_end-float_val)*pixels_per_unit);
	}
}
inline int UI::ScrollBar::_track_len()
{
	return (horizontal?w:h)-UI_SCROLL_ARROW_SIZE*2;
}
  
/****************************** GLUI_Scrollbar::mouse_down_handler() **********/

bool UI::ScrollBar::_mouse_up_handler(int local_x, int local_y, bool inside)
{	
	if(associated_object) //2019
	{
		associated_object->activate(ACTIVATE_MOUSE);
	}
	state = STATE_NONE; redraw(); return false; //2019
}

/****************************** GLUI_Scrollbar::mouse_down_handler() **********/

bool UI::ScrollBar::_mouse_down_handler(int local_x, int local_y)
{
	if(val_end==val_start) //2019 
	{
		 return true; //Disable. E.g. the list is too short.
	}

	//last_update_time = GLUI_Time()-1;
	state = find_arrow(local_x,local_y);
	if(needs_idle())
	GLUI::glui_setIdleFuncIfNecessary();

	/* printf("spinner: mouse down  : %d/%d   arrow:%d\n",local_x,local_y,
	find_arrow(local_x,local_y)); */

	if(state!=STATE_UP&&state!=STATE_DOWN)
	{
		return false; //return true; //???
	}

	_reset_growth();

	/*** ints and floats behave a bit differently.  When you click on
	an int spinner, you expect the value to immediately go up by 1, whereas
	for a float it'll go up only by a fractional amount.  Therefore, we
	go ahead and increment by one for int spinners ***/
	//#if 1
	/*2019: I think do_click implements this logic?
	if(data_type==SPIN_INT)
	{
		// Allow for possibility of reversed limits
		int lo = val_min();
		int hi = val_max();
		int increase = int_min<int_max?1:-1;
		int new_val = int_val;
		if(state==STATE_UP)
		{
			new_val+=increase;
		}
		else if(state==STATE_DOWN)
		{
			new_val-=increase;
		}
		if(new_val>=lo&&new_val<=hi&&new_val!=int_val)
		{
			set_int_val(new_val); do_callbacks(); //???
		}
	}*/
	//#endif

	do_click(); return false;
}
 
/***************************** GLUI_Scrollbar::mouse_held_down_handler() ******/

bool UI::ScrollBar::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	//This interferes with being able to hover in and out of the buttons boxes.
	//if(state==STATE_NONE) return false;

	/* printf("spinner: mouse held: %d/%d    inside: %d\n",local_x,local_y,inside); */

	if(state==STATE_SCROLL) /* dragging? */
	{
		do_drag(local_x-x_abs,local_y-y_abs);
	}
	else /* not dragging */
	{
		//_idle just calls do_click... though I'm uncertain why it's required if the
		//mouse is stationary. (It can cleanup when GLUT fails to send the up event.)
		//int new_state = find_arrow(local_x,local_y);
		//if(new_state==state)
		{
			/** Still in same arrow **/
			do_click();
		}
	}

	return false;
}

/****************************** GLUI_Scrollbar::draw() **********/

void UI::ScrollBar::_draw()
{ 	
	int bsp = _box_pos();
	int box = _box_len();
	int bep = bsp+(enabled?box:0);
	int sas = UI_SCROLL_ARROW_SIZE;		
	int run = (horizontal?w:h);
	int opp = (horizontal?h:w);
	int bar = run-2*sas+1;

	//Darkening somewhat to make it distinguisable from
	//the white on the buttons. Maybe if the track were
	//not white it wouldn't be an issue?
	enum{ shadow=233-10, white=255-10 };

	if(horizontal)
	{
		//NOTE: These have shadows.
		draw_scroll_arrow(LEFT,0,0); //#1
		draw_scroll_box(sas+bsp,0,box,opp); //#2
		draw_scroll_arrow(RIGHT,run-sas,0); //#3
		glColor3ub(white,white,white);
		draw_scroll(sas,0,bsp,opp,STATE_UP&state?0:shadow);
		glColor3ub(white,white,white);
		draw_scroll(sas+bep,0,bar-bep-1,opp,shadow);
	}
	else
	{
		draw_scroll_arrow(TOP,0,0);
		draw_scroll_box(0,sas+bsp,opp,box);
		draw_scroll_arrow(BOTTOM,0,run-sas);				
		glColor3ub(white,white,white);
		draw_scroll(0,sas,opp,bsp,STATE_UP&state?0:shadow);
		glColor3ub(white,white,white);
		draw_scroll(0,sas+bep,opp,bar-bep-1,shadow);
	}
}

/****************************** GLUI_Scrollbar::draw_scroll_arrow() **********/

void UI::ScrollBar::draw_scroll_arrow(int arrowtype, int x, int y)
{
	//NOTE: These figures use 16 for UI_SCROLL_ARROW_SIZE.
	float offset = 0;
	const float L = 3.5f,HC = 7.f,R = 10.5f;
	const float T = 4.5f,VC = 8.f,B = 11.5;
	enum{ c=1 }; //2019 (Maybe draw_raised_box changed?)
	const float verts[][6] = 
	{
		{ L,10.5f-c,R,10.5f-c,HC,6.5f-c},// up arrow
		{ L,6.5f-c,R,6.5f-c,HC,10.5f-c},// down arrow
		{ R-2,T-c,R-2,B-c,L+1,VC-c},// left arrow
		{ L+2,T-c,L+2,B-c,R-1,VC-c} // right arrow
	};

	const float *tri = NULL;

	switch(arrowtype)
	{
	case TOP:

		tri = verts[0];
		if(state&STATE_UP) offset = 1; //STATE_BOTH???
		break;

	case BOTTOM:

		tri = verts[1];
		if(state&STATE_DOWN) offset = 1; //STATE_BOTH???
		break;

	case LEFT:

		tri = verts[2];
		if(state&STATE_DOWN) offset = 1; //STATE_BOTH???
		break;

	case RIGHT: 

		tri = verts[3];
		if(state&STATE_UP) offset = 1; //STATE_BOTH???
		break;

	default: return; /* tri is NULL */
	}
		
	if(!offset)
	{
		draw_raised_rect(x,y,x+UI_SCROLL_ARROW_SIZE,y+UI_SCROLL_ARROW_SIZE);
	}
	else
	{
		glColor3ub(128,128,128);		
		draw_border_rect(x,y,x+UI_SCROLL_ARROW_SIZE,y+UI_SCROLL_ARROW_SIZE);
	}

	//2019: Only push down in Y direction.
	int press = offset; offset = 0;

	GLubyte black[] = {0,0,0};
	GLubyte white[] = {255,255,255};
	GLubyte gray[] = {128,128,128};
	GLubyte *color = black;
	if(!enabled)
	{
		offset = 1; color = white;
	}

	//HACK? Adjust "verts" for UI_SCROLL_ARROW_SIZE.
	offset+=UI_SCROLL_ARROW_SIZE-16;

	glTranslatef(x+offset,y+offset+press,0);
	glColor3ubv(color);
	glBegin(GL_TRIANGLES);
	glVertex2fv(tri);
	glVertex2fv(tri+2); glVertex2fv(tri+4);
	glEnd();
	glTranslatef(-(x+offset),-(y+offset+press),0);

	if(!enabled) // once more!
	{ 
		glTranslatef(x+offset,y,0);
		glColor3ubv(gray);
		glBegin(GL_TRIANGLES);
		glVertex2fv(tri);
		glVertex2fv(tri+2); glVertex2fv(tri+4);
		glEnd();
		glTranslatef(-x,-y,0);
	}
}

void UI::ScrollBar::draw_scroll(int xx, int yy, int ww, int hh, int shadow)
{
	//2019: Adding parameter list. 
	float x0 = xx, y0 = yy, x1 = xx+ww, y1 = yy+hh;

	const struct stipple
	{
		unsigned int a:32,b:32;
		stipple():a(0x55555555),b(~0x55555555){}		
	}pattern[16+1]; 
	//I developed this to make the white pixel not to
	//extend the white highlight on the button. But a
	//bigger defect is the gray pixel on the opposite
	//side seems to carve out a notch. A shadow helps
	//for the white highlight problem.
	int odd = y_abs%2?32/CHAR_BIT:0;
	//glColor3ub(255,255,255);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple((GLubyte*)pattern+odd);	
	glRecti(x0,y0,x1,y1);
	if(shadow&&ww&&hh)
	{
		glColor3ub(shadow,shadow,shadow);
		if(horizontal)
		glRecti(x0,y0,x0+1,y1);
		else
		glRecti(x0,y0,x1,y0+1);
	}
	glDisable(GL_POLYGON_STIPPLE);
}

/****************************** GLUI_Scrollbar::draw_scroll_box() **********/

void UI::ScrollBar::draw_scroll_box(int x, int y, int w, int h)
{
	//NOTE: Both cases are functionally disabled. 
	//draw_raised_box can't draw a zero-size box.
	if(!enabled||val_start==val_end) return;

	draw_raised_rect(x,y,x+w,y+h); //horizontal?4:2

	if(this==GLUI::get_active())
	{
		//2019: Spin_Interface::_activate is preventing tab
		//navigation. Page, Arrow, Home, End should be used.
		if(!associated_object||state==STATE_SCROLL)
		{
			if(state==STATE_SCROLL)
			{
				//NOTE: I developed this alternative because
				//draw_active_rect wouldn't work. But I made
				//it work by increasing it to 17. Still this
				//is useful I think.
				glColor3ub(96,96,96);
				draw_scroll(x+3,y+3,w-6,h-6,0);
			}
			else
			{
				//REMINDER: My Intel chipset glitches out if
				//UI_SCROLL_ARROW_SIZE is not odd. Button is
				//fine with even sizes, though it looks best
				//if they can be odd. I guess it's too small.
				draw_active_rect(x+3,y+3,x+w-3,y+h-3);
			}
		}
	}
}
  
/************************************ GLUI_Scrollbar::find_arrow() ************/

int UI::ScrollBar::find_arrow(int local_x, int local_y)
{
	int x = local_x-=x_abs, y = local_y-=y_abs;

	int hh = h, ww = w; if(horizontal) 
	{
		std::swap(x,y); std::swap(hh,ww);
	}
	
	if(x>=ww-UI_SCROLL_ARROW_SIZE-3&&x<=ww)
	{	
		int pos = UI_SCROLL_ARROW_SIZE+_box_pos();
						
		if(y>=0&&y<=pos)
		{
			//FIX ME
			//return STATE_UP;
			return horizontal?STATE_DOWN:STATE_UP; //WHY DIFFER?????
		}
		if(y>=pos+_box_len()&&y<=(hh+UI_SCROLL_ARROW_SIZE))
		{
			//FIX ME
			//return STATE_DOWN;
			return horizontal?STATE_UP:STATE_DOWN; //WHY DIFFER?????
		}
		return STATE_SCROLL;
	}	

	return STATE_NONE;
}

/***************************************** GLUI_Scrollbar::do_click() **********/

void UI::ScrollBar::do_click()
{
	//NEW: Reevaluate "state" so box interrupts holding
	//down the tracks.
	if(GLUI.curr_button_down)
	{
		if(state!=STATE_SCROLL)
		{
			int swap = find_arrow(GLUI.curr_x,GLUI.curr_y);

			/*if(0&&swap==STATE_SCROLL)
			{
				state = STATE_NONE; return;
			}*/

			if(swap!=state)
			{
				//TODO: Try to stop box on cursor's center.
			}

			state = swap;
		}
	}
	else //NEW: In case GLUT fails to send mouse-up event.
	{
		state = STATE_NONE; 
		
		redraw(); return;
	}

	int direction = 1; if(state==STATE_DOWN)
	{
		direction = -1;	
	}
	else if(state!=STATE_UP)
	{
		//direction = 0; //NONE, BOTH, SCROLL?

		//NEW: No movement? May as well return unless the
		//bar needs inertia (not implemented below.)
		return;
	}
	if(val_start>val_end) direction*=-1; 

	double incr = _increase_growth()*speed;

	//2019: I don't see a significant difference here, so I'm
	//knocking this feature out, because I think tuning it is
	//probably a souce of bad user-experience.
	//double et = GLUI_Time();
	//double frame_time = et-last_update_time;
	//last_update_time = et;
	//double frame_limit = velocity_limit*frame_time;
	//if(incr>frame_limit) 
	//incr = frame_limit; /* don't scroll faster than limit */

	double new_val = float_val, cmp = new_val;

	new_val+=direction*incr;
	
	/*2019: I believe set_int_val is knocked out because
	//set_float_val also sets int_val. Might as well make
	//it official...
	if(0&&data_type==SPIN_INT) set_int_val((int)new_val);
	if(1||data_type==SPIN_FLOAT)*/set_float_val(new_val);

	//printf("do_click: incr %f  val=%f  float_val=%f\n",incr,new_val,float_val);

	if(cmp!=float_val) do_callbacks(); 
}

/***************************************** GLUI_Scrollbar::do_drag() **********/

void UI::ScrollBar::do_drag(int x, int y)
{
	int box = _box_len();
	int free_len = _track_len()-box;
	if(free_len==0) return; //zero-divide
	
	double new_val = float_val;

	//if(state==STATE_SCROLL) //I think this dead code.
	{	
		int hbox = box/2;
		int track_v = (horizontal?x:y)-UI_SCROLL_ARROW_SIZE;

		double same = (track_v-hbox)*(val_end-val_start)/double(free_len);

		//WHY DO THESE DIFFER IN THIS RESPECT???
		new_val = horizontal?val_start+same:val_end-same;
	}
	/*else if(state==STATE_UP||state==STATE_DOWN)
	{
		//#ifndef _DEBUG
		//#error do_click flips the direction based on inverted min/max?
		//#endif
		assert(0);

		double advance = state==STATE_UP?1:-1;

		//double incr = _growth()*advance*user_speed; //UNUSED?

		if(data_type!=SPIN_INT)		
		advance*=(val_end-val_start)/free_len;

		new_val+=advance;		
	}*/	

	double cmp = float_val;

	if(data_type==SPIN_INT)
	{
		set_int_val((int)new_val);
	}
	else if(data_type==SPIN_FLOAT)
	{
		set_float_val(new_val);
	}
	else assert(data_type==SPIN_FLOAT);

	if(cmp!=float_val) do_callbacks();
}

/********************************** GLUI_Scrollbar::set_int_val() ************/

void UI::ScrollBar::_update_live()
{
	//set_float_val //set_int_val
	{
		double new_val = float_val;

		// Allow for the possibility that the limits are reversed
		double hi = val_max();
		double lo = val_min();
		if(new_val>hi) new_val = hi;
		if(new_val<lo) new_val = lo;

		if(new_val!=float_val) 
		{
			if(data_type==SPIN_INT)
			{
				set_int_val((int)new_val);
			}
			else if(data_type==SPIN_FLOAT)
			{
				set_float_val(new_val);
			}
			else assert(data_type==SPIN_FLOAT);
		}
	}

	_update_size();	
}

/************************************ GLUI_Scrollbar::update_size() **********/

void UI::ScrollBar::_update_size()
{
	if(horizontal)
	{
		h = UI_SCROLL_ARROW_SIZE;

		if(Control*c=associated_object)
		{
			w = associated_object->w;
			
			if(c==parent()) //NEW
			{
				w-=c->x_lr+c->x_rl;
			}
		}
	}
	else
	{
		w = UI_SCROLL_ARROW_SIZE;

		if(Control*c=associated_object)
		{
			h = associated_object->h;

			if(c==parent()) //NEW
			{
				h-=c->y_off_top+c->y_off_bot;
			}
		}
	}
}

/*********************************** GLUI_Scrollbar::set_float_limits() *********/

void Spin_Interface::set_range(double start, double end)
{
	val_start = start; val_end = end;

	// Allow for possiblitly of reversed limits
	double lo = val_min(); if(float_val<lo) set_float_val(lo);
	double hi = val_max(); if(float_val>hi) set_float_val(hi);

	update_size();
}

//---.
}//<-'
