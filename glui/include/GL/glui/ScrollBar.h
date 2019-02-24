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

  class GLUI_API GLUI_Scrollbar : public GLUI_Control
  {
  public:
    // Constructor, no live var
    GLUI_Scrollbar( GLUI_Node *parent,
                    const char *name, 
                    int horz_vert=GLUI_SCROLL_HORIZONTAL,
                    int data_type=GLUI_SCROLL_INT,
                    int id=-1, CallBack callback=CallBack() 
                    /*,GLUI_Control *object = NULL*/
                    /*,GLUI_InterObject_CB obj_cb = NULL*/
                    );

    // Constructor, int live var
    GLUI_Scrollbar( GLUI_Node *parent, const char *name, int horz_vert,
                    int *live_var,
                    int id=-1, CallBack callback=CallBack() 
                    /*,GLUI_Control *object = NULL*/
                    /*,GLUI_InterObject_CB obj_cb = NULL*/
                    );

    // Constructor, float live var
    GLUI_Scrollbar( GLUI_Node *parent, const char *name, int horz_vert,
                    float *live_var,
                    int id=-1, CallBack callback=CallBack()
                    /*,GLUI_Control *object = NULL*/
                    /*,GLUI_InterObject_CB obj_cb = NULL*/
                    );

    bool          currently_inside;
    int           state;
    float         growth, growth_exp;
    int           last_x, last_y;
    int           data_type;
    int           callback_count;
    int           last_int_val;  ///< Used to prevent repeated callbacks.
    float         last_float_val;
    int           first_callback;
    float         user_speed;
    float         float_min, float_max;
    int           int_min, int_max;
    int           horizontal;
    double     last_update_time; ///< GLUI_Time() we last advanced scrollbar.
    double     velocity_limit; ///< Maximum distance to advance per second.
    int box_length;
    int box_start_position;
    int box_end_position;
    int track_length;


    /* Rather than directly access an Editbox or Textbox for 
       changing variables, a pointer to some object is defined
       along with a static callback in the form func(void *, int) -
       the int is the new value, the void * must be cast to that
       particular object type before use.
    */
    void *        associated_object; /* Lets the Spinner manage it's own callbacks */
    CallBack       object_cb; /* function pointer to object call_back */

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler( int key,int modifiers );
  
    void draw( int x, int y );
    void draw_pressed( void );
    void draw_unpressed( void );
    void draw_text( int sunken );

    void update_size( void );

    void set_int_limits( int low, int high,int limit_type=GLUI_LIMIT_CLAMP);
    void set_float_limits( float low,float high,int limit_type=GLUI_LIMIT_CLAMP);
    int  find_arrow( int local_x, int local_y );
    void do_drag( int x, int y );
    void do_callbacks( void );
    void draw_scroll( void );
    void do_click( void );
    void idle( void );
    bool needs_idle( void ) const;
    void set_int_val( int new_val );
    void set_float_val( float new_val );
    void increase_growth( void );
    void reset_growth( void );

    void set_speed( float speed ) { user_speed = speed; };
    void update_scroll_parameters();
    void set_object_callback(CallBack cb=CallBack(), GLUI_Control*obj=NULL)
    { object_cb=cb; associated_object=obj; }

  protected:
    void common_init ( void );
    void common_construct(
                          GLUI_Node *parent,
                          const char *name, 
                          int horz_vert,
                          int data_type, void* live_var,
                          int id, CallBack callback
                          /*,GLUI_Control *object
                            ,GLUI_InterObject_CB obj_cb*/
                          );

    virtual void draw_scroll_arrow(int arrowtype, int x, int y);
    virtual void draw_scroll_box(int x, int y, int w, int h);
  };
  
}
