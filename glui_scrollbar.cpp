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


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  This program is freely distributable without licensing fees and is
  provided without guarantee or warrantee expressed or implied. This
  program is -not- in the public domain.

*****************************************************************************/

  
#include "glui.h"
#include "stdinc.h"
#include <math.h>

/*static int __debug=0;              */

#define  GLUI_SCROLL_GROWTH_STEPS         800
#define  GLUI_SCROLL_MIN_GROWTH_STEPS     100
#define  GLUI_SCROLL_CALLBACK_INTERVAL    1
#define  GLUI_SCROLL_BARLENGTH_PADDING    4
 
/****************************** GLUI_Scrollbar::mouse_down_handler() **********/

int    GLUI_Scrollbar::mouse_down_handler( int local_x, int local_y )
{
  this->state = find_arrow( local_x, local_y );

  /*  printf( "spinner: mouse down  : %d/%d   arrow:%d\n", local_x, local_y,
      find_arrow( local_x, local_y ));
      */

  if ( state != GLUI_SCROLL_STATE_UP AND state != GLUI_SCROLL_STATE_DOWN)
    return true;

  reset_growth();
  if ( can_draw() )
    draw_arrows();  

  /*** ints and floats behave a bit differently.  When you click on
    an int spinner, you expect the value to immediately go up by 1, whereas
    for a float it'll go up only by a fractional amount.  Therefore, we
    go ahead and increment by one for int spinners ***/
  if ( data_type == GLUI_SCROLL_INT ) {
    if ( state == GLUI_SCROLL_STATE_UP ) {
      if (int_val > int_min)
	set_int_val(int_val - 1);

    } else if ( state == GLUI_SCROLL_STATE_DOWN )
      if (int_val <= int_max)
	set_int_val(int_val + 1);
  }
  
  do_click();  
  
  return false;
}


/******************************** GLUI_Scrollbar::mouse_up_handler() **********/

int    GLUI_Scrollbar::mouse_up_handler( int local_x, int local_y, int inside )
{
  state = GLUI_SCROLL_STATE_NONE;

  /*  printf("spinner: mouse up  : %d/%d    inside: %d\n",local_x,local_y,inside);              */

  /*glutSetCursor( GLUT_CURSOR_INHERIT );              */
  glutSetCursor( GLUT_CURSOR_LEFT_ARROW );

  draw_arrows();

  /*  do_callbacks(); --- stub               */
  /*  if ( callback )               */
  /*  callback( this->user_id );              */
  
  return false;
}


/***************************** GLUI_Scrollbar::mouse_held_down_handler() ******/

int    GLUI_Scrollbar::mouse_held_down_handler( int local_x, int local_y,
                          int new_inside)
{
  int new_state;
  if ( state == GLUI_SCROLL_STATE_NONE )
    return false;

  /*  printf("spinner: mouse held: %d/%d    inside: %d\n",local_x,local_y,
      new_inside);
      */

  if ( state == GLUI_SCROLL_STATE_BOTH || state == GLUI_SCROLL_STATE_SCROLL) {   /* dragging? */
    do_drag( local_x-x_abs, local_y-y_abs );
  }
  else {                                      /* not dragging */
    new_state = find_arrow( local_x, local_y );

    
    if ( new_state == state ) {
      /** Still in same arrow **/
      do_click();
    }
    else {
      if ( new_inside OR 1) {
    /** The state changed, but we're still inside - that
      means we moved off the arrow: begin dragging **/
    state = GLUI_SCROLL_STATE_BOTH;
      }
      else {
    /*** Here check y of mouse position to determine whether to 
      drag ***/

    /* ... */
      }
    }

    /*** We switched to up/down dragging ***/
    if ( state == GLUI_SCROLL_STATE_BOTH ) {
      glutSetCursor( GLUT_CURSOR_UP_DOWN );
      last_x = local_x;
      last_y = local_y;

    }

    if ( can_draw() )
      draw_arrows();
  }

  return false;
}


/****************************** GLUI_Scrollbar::key_handler() **********/

int    GLUI_Scrollbar::key_handler( unsigned char key,int modifiers )
{
  

  return true;
}


/****************************** GLUI_Scrollbar::draw() **********/

void    GLUI_Scrollbar::draw( int x, int y )
{
  int orig;

  if ( NOT can_draw() )
    return;

  orig = set_to_glut_window();

  if ( enabled ) {
    /*** Draw the up arrow either pressed or unrpessed ***/
    if ( state == GLUI_SCROLL_STATE_UP OR state == GLUI_SCROLL_STATE_BOTH )
      glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_UP_ON, 
                  w-GLUI_SCROLL_ARROW_WIDTH-1, 
                  GLUI_SCROLL_ARROW_Y);
    else
      glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_UP_OFF, 
                  w-GLUI_SCROLL_ARROW_WIDTH-1,
                  GLUI_SCROLL_ARROW_Y);

    /*** Draw the down arrow either pressed or unrpessed ***/
    if (state == GLUI_SCROLL_STATE_DOWN OR state == GLUI_SCROLL_STATE_BOTH)
      glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_DOWN_ON, 
                  w-GLUI_SCROLL_ARROW_WIDTH-1, 
                  h-GLUI_SCROLL_ARROW_HEIGHT);
    else
      glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_DOWN_OFF, 
                  w-GLUI_SCROLL_ARROW_WIDTH-1,
                  h-GLUI_SCROLL_ARROW_HEIGHT);
  }
  else {  /**** The spinner is disabled ****/
    glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_UP_DIS, 
                w-GLUI_SCROLL_ARROW_WIDTH-1, 
                GLUI_SCROLL_ARROW_Y);
    glui->std_bitmaps.draw( GLUI_STDBITMAP_SPINNER_DOWN_DIS, 
                w-GLUI_SCROLL_ARROW_WIDTH-1, 
                h-GLUI_SCROLL_ARROW_HEIGHT);
  }

  draw_scroll();
  restore_window( orig );
}


void GLUI_Scrollbar::update_scroll_parameters() {
  if (int_val < 0)
    int_val = 0;
  if (int_max < 0)
    int_max = 0;
  bar_length = this->h-GLUI_SCROLL_ARROW_HEIGHT*2+GLUI_SCROLL_ARROW_Y-GLUI_SCROLL_BARLENGTH_PADDING;
  tab_length = (20 < ((double)bar_length/((double)2*int_max))) ? 
    ((int)((double)bar_length/((double)2*int_max))) : 20;
  tab_start_position = (int)((int)((double)int_val)/((double)int_max)*(bar_length-tab_length));
  tab_end_position = (int)((int)((double)int_val)/((double)int_max)*(bar_length-tab_length)+tab_length);
}


void GLUI_Scrollbar::draw_scroll() {
  int orig;

  if ( NOT can_draw() )
    return;

  orig = set_to_glut_window();

  update_scroll_parameters();

  /*
  printf("IV: %d IM: %d BL: %d TL: %d TS: %d TE: %d\n", int_val,
	 int_max,
	 bar_length,
	 tab_length,
	 tab_start_position,
	 tab_end_position);
  */

  draw_bkgd_box( w, 0, 
		 GLUI_SCROLL_ARROW_HEIGHT+GLUI_SCROLL_ARROW_Y, 
		 h-GLUI_SCROLL_ARROW_HEIGHT);
  draw_box_inwards_outline( w-5, 3, 
			    GLUI_SCROLL_ARROW_HEIGHT+GLUI_SCROLL_ARROW_Y, 
			    h-GLUI_SCROLL_ARROW_HEIGHT);

  draw_box( w-2, 0, 
	    GLUI_SCROLL_ARROW_HEIGHT + 
	    GLUI_SCROLL_ARROW_Y + 
	    tab_start_position, 
	    GLUI_SCROLL_ARROW_HEIGHT + 
	    GLUI_SCROLL_ARROW_Y + 
	    tab_end_position, .7, .7, .7);
  draw_box_inwards_outline( w-2, 0, 
			    GLUI_SCROLL_ARROW_HEIGHT + 
			    GLUI_SCROLL_ARROW_Y + 
			    tab_start_position, 
			    GLUI_SCROLL_ARROW_HEIGHT + 
			    GLUI_SCROLL_ARROW_Y + 
			    tab_end_position);

  restore_window( orig );
}


/********************************* GLUI_Scrollbar::special_handler() **********/

int    GLUI_Scrollbar::special_handler( int key,int modifiers )
{
  if ( key == GLUT_KEY_UP ) {    /** Simulate a click in the up arrow **/
    mouse_down_handler( x_abs + w - GLUI_SCROLL_ARROW_WIDTH + 1,
            y_abs + GLUI_SCROLL_ARROW_Y+1 );
    mouse_up_handler( x_abs + w - GLUI_SCROLL_ARROW_WIDTH + 1,
              y_abs + GLUI_SCROLL_ARROW_Y+1, true );
  }
  else if ( key == GLUT_KEY_DOWN ) {  /** Simulate a click in the up arrow **/
    mouse_down_handler(x_abs + w - GLUI_SCROLL_ARROW_WIDTH + 1,
               y_abs+GLUI_SCROLL_ARROW_Y+1+GLUI_SCROLL_ARROW_HEIGHT);
    mouse_up_handler( x_abs + w - GLUI_SCROLL_ARROW_WIDTH + 1,
              y_abs+GLUI_SCROLL_ARROW_Y+1 +GLUI_SCROLL_ARROW_HEIGHT,
              true );
  }
  else if ( key == GLUT_KEY_HOME ) {  /** Set value to limit top - 
                    or increment by 10 **/
  }
  else if ( key == GLUT_KEY_END ) {  
  }

  return true;
}



/********************************** GLUI_Scrollbar::set_int_val() ************/

void   GLUI_Scrollbar::set_int_val( int new_val )
{
  if (new_val >= int_max)
    new_val = int_max -1;
  last_int_val = int_val;
  int_val = new_val;

}


/************************************ GLUI_Scrollbar::update_size() **********/

void   GLUI_Scrollbar::update_size( void )
{
  int bar_width;
  bar_width = GLUI_SCROLL_ARROW_WIDTH;
  if (associated_object) {
    this->w = bar_width;
    this->h = ((GLUI_Control *)associated_object)->h;
  }
}
 

/************************************ GLUI_Scrollbar::find_arrow() ************/

int    GLUI_Scrollbar::find_arrow( int local_x, int local_y )
{

  local_x = local_x-x_abs; 
  local_y = local_y-y_abs;

  if ( local_x >=  w-GLUI_SCROLL_ARROW_WIDTH-3 &&
       local_x <= w) {
    update_scroll_parameters();
    if ( local_y >= GLUI_SCROLL_ARROW_Y AND 
     local_y <= (GLUI_SCROLL_ARROW_Y+GLUI_SCROLL_ARROW_HEIGHT+tab_start_position) )
      return GLUI_SCROLL_STATE_UP;

    if ( local_y >= (GLUI_SCROLL_ARROW_Y+GLUI_SCROLL_ARROW_HEIGHT+tab_end_position) AND 
     local_y <= (h+GLUI_SCROLL_ARROW_HEIGHT) )
      return GLUI_SCROLL_STATE_DOWN;
    return GLUI_SCROLL_STATE_SCROLL;
  }


  return GLUI_SCROLL_STATE_NONE;
}


/****************************** GLUI_Scrollbar::draw_arrows() **********/

void    GLUI_Scrollbar::draw_arrows( void )
{
  if ( NOT can_draw() )
    return;

  translate_and_draw_front();
}


/***************************************** GLUI_Scrollbar::do_click() **********/

void    GLUI_Scrollbar::do_click( void )
{
  int    direction = 0;
  float  incr;
  float  modifier_factor;

  /* not used */
  if ( state == GLUI_SCROLL_STATE_UP )
    direction = -1;
  else if ( state == GLUI_SCROLL_STATE_DOWN )
    direction = +1;

  increase_growth();

  modifier_factor = 1.0;

  incr = direction * growth * modifier_factor * user_speed;
  int_val += (int)incr;

  /*** Now update live variable and do callback.  We don't want
    to do the callback on each iteration of this function, just on every 
    i^th iteration, where i is given by GLUI_SCROLL_CALLBACK_INTERVAL ****/
  callback_count++;
  if ( (callback_count % GLUI_SCROLL_CALLBACK_INTERVAL ) == 0 )
    do_callbacks();

}


/***************************************** GLUI_Scrollbar::do_drag() **********/

void    GLUI_Scrollbar::do_drag( int x, int y )
{
  int   direction = 0;
  float incr, modifier_factor;
  /* int delta_x;              */

  modifier_factor = 1.0;
  if ( state == GLUI_SCROLL_STATE_SCROLL) {
    update_scroll_parameters();
    
    if (y < (GLUI_SCROLL_ARROW_Y+GLUI_SCROLL_ARROW_HEIGHT+tab_start_position)){
      int_val = (int)((int)((double)y)/((double)(bar_length-tab_length))*int_max);
    } else if (y > (GLUI_SCROLL_ARROW_Y+GLUI_SCROLL_ARROW_HEIGHT+tab_end_position)) {
      int_val = (int)((int)((double)y-((double)tab_length))/((double)(bar_length-tab_length))*int_max);
    }
    last_y = y;
  } else {
    if ( state == GLUI_SCROLL_STATE_UP )
      direction = -1;
    else if ( state == GLUI_SCROLL_STATE_DOWN )
      direction = +1;
    /** Remember, edittext mirrors the float and int values ***/
    last_y = y;
  }
  last_x = x;
  incr = growth * direction * modifier_factor * user_speed;
  int_val += direction;
  if (int_val < int_min)
    int_val = int_min;
  if (int_val > int_max)
    int_val = int_max;
  /*** Now update live variable and do callback.  We don't want
    to do the callback on each iteration of this function, just on every 
    i^th iteration, where i is given by GLUI_SCROLL_CALLBACK_INTERVAL ****/
  callback_count++;
  if ( (callback_count % GLUI_SCROLL_CALLBACK_INTERVAL ) == 0 )
    do_callbacks();
}


/***************************************** GLUI_Scrollbar::needs_idle() ******/

int     GLUI_Scrollbar::needs_idle( void )
{
  if  (state == GLUI_SCROLL_STATE_UP OR state == GLUI_SCROLL_STATE_DOWN ) {
    return true;
  }
  else {
    return false;
  }
}

/***************************************** GLUI_Scrollbar::idle() **********/

void    GLUI_Scrollbar::idle( void )
{
  if ( NOT needs_idle() )
    return;
  else
    do_click();
}


/************************************ GLUI_Scrollbar::do_callbacks() **********/

void    GLUI_Scrollbar::do_callbacks( void )
{

  /*    *******************************************/

  if ( NOT first_callback ) {
    if ( data_type == GLUI_SCROLL_INT AND int_val == last_int_val ) {
      return;
    }
  }

  if (associated_object == NULL)
    this->execute_callback();
  else                         /* Use internal Callbacks */
    if (object_cb != NULL) {
      object_cb(associated_object, int_val);
    }
  last_int_val   = int_val;
  last_float_val = float_val;
  first_callback = false;
}


/*********************************** GLUI_Scrollbar::set_int_limits() *********/

void   GLUI_Scrollbar::set_int_limits( int low, int high, int limit_type )
{

  int_min = low;
  int_max = high;    
}
 

/*********************************** GLUI_Scrollbar::reset_growth() *************/

void    GLUI_Scrollbar::reset_growth( void )
{
    growth = (int_max - int_min) / GLUI_SCROLL_GROWTH_STEPS;
}


/******************************* GLUI_Scrollbar::increase_growth() *************/

void    GLUI_Scrollbar::increase_growth( void )
{
  if ( growth < (int_max-int_min) / GLUI_SCROLL_MIN_GROWTH_STEPS )
    growth *= growth_exp;
  return;
  /*  printf( "growth: %f\n", growth );              */
}


/********************************** GLUI_Scrollbar::get_int_val() *************/

int    GLUI_Scrollbar::get_int_val( void )
{
  return int_val;
}


