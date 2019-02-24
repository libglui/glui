/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui.h - Main (and only) external header for 
        GLUI_Context *User Interface Toolkit

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

#pragma once

namespace glui {

#define  GLUI_SPINNER_ARROW_WIDTH   12
#define  GLUI_SPINNER_ARROW_HEIGHT   8
#define  GLUI_SPINNER_ARROW_Y        2

#define  GLUI_SPINNER_STATE_NONE     0
#define  GLUI_SPINNER_STATE_UP       1
#define  GLUI_SPINNER_STATE_DOWN     2
#define  GLUI_SPINNER_STATE_BOTH     3

#define  GLUI_SPINNER_DEFAULT_GROWTH_EXP   1.05f

  class GLUI_API GLUI_Spinner : public GLUI_Control
  {
  public:
    // Constructor, no live var
    GLUI_Spinner( GLUI_Node* parent, const char *name, 
                  int data_type=GLUI_SPINNER_INT, int id=-1, CallBack callback=CallBack() );
    // Constructor, int live var
    GLUI_Spinner( GLUI_Node* parent, const char *name, 
                  int *live_var, int id=-1, CallBack callback=CallBack() );
    // Constructor, float live var
    GLUI_Spinner( GLUI_Node* parent, const char *name, 
                  float *live_var, int id=-1, CallBack callback=CallBack() );
    // Deprecated constructor
    GLUI_Spinner( GLUI_Node* parent, const char *name, 
                  int data_type,
                  void *live_var,
                  int id=-1, CallBack callback=CallBack() );
    // Deprecated constructor
    GLUI_Spinner( void ) { common_init(); }

    bool          currently_inside;
    int           state;
    float         growth, growth_exp;
    int           last_x, last_y;
    int           data_type;
    int           callback_count;
    int           last_int_val;
    float         last_float_val;
    int           first_callback;
    float         user_speed;

    GLUI_EditText *edittext;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler(   int key,int modifiers );

    void draw( int x, int y );
    void draw_pressed( void );
    void draw_unpressed( void );
    void draw_text( int sunken );

    void update_size( void );

    void set_float_limits( float low,float high,int limit_type=GLUI_LIMIT_CLAMP);
    void set_int_limits( int low, int high,int limit_type=GLUI_LIMIT_CLAMP);
    int  find_arrow( int local_x, int local_y );
    void do_drag( int x, int y );
    void do_callbacks( void );
    void do_click( void );
    void idle( void );
    bool needs_idle( void ) const;

    const char *get_text( void );

    void set_float_val( float new_val );
    void set_int_val( int new_val );
    float  get_float_val( void );
    int    get_int_val( void );
    void increase_growth( void );
    void reset_growth( void );

    void set_speed( float speed ) { user_speed = speed; }

  protected:
    void common_init() {
      glui_format_str( name, "Spinner: %p", this );
      h            = GLUI_EDITTEXT_HEIGHT;
      w            = GLUI_EDITTEXT_WIDTH;
      x_off        = 0;
      y_off_top    = 0;
      y_off_bot    = 0;
      can_activate = true;
      state        = GLUI_SPINNER_STATE_NONE;
      edittext     = NULL;
      growth_exp   = GLUI_SPINNER_DEFAULT_GROWTH_EXP;
      callback_count = 0;
      first_callback = true;
      user_speed   = 1.0;
    }
    void common_construct( GLUI_Node* parent, const char *name, 
                           int data_type, void *live_var,
                           int id, CallBack callback );
  };
  
}
