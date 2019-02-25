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

  /**
     A checkbox, which can be checked on or off.  Can be linked
     to an int value, which gets 1 for on and 0 for off.
  */
  class GLUI_API GLUI_Checkbox : public GLUI_Control
  {
  public:
    int  orig_value;
    bool currently_inside;
    int  text_x_offset;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );

    void update_size( void );

    void draw( int x, int y );

    void draw_active_area( void );
    void draw_empty_box( void );
    void set_int_val( int new_val );

    /**
       Create a new checkbox object.
 
       @param parent The panel our object is inside; or the main GLUI_Context *object.
       @param name Label next to our checkbox.
       @param value_ptr Optional integer value to attach to this checkbox.  When the 
       checkbox is checked or unchecked, *value_ptr will also be changed. ("Live Vars").
       @param id Optional ID number, to pass to the optional callback function.
       @param callback Optional callback function, taking either the int ID or control.
    */
    GLUI_Checkbox(GLUI_Node *parent, const char *name, int *value_ptr=NULL,
                  int id=-1, CallBack callback=CallBack());
    GLUI_Checkbox( void ) { common_init(); }

  protected:
    void common_init(void) {
      glui_format_str( name, "Checkbox: %p", this );
      w              = 100;
      h              = GLUI_CHECKBOX_SIZE;
      orig_value     = -1;
      text_x_offset  = 18;
      can_activate   = true;
      live_type      = GLUI_LIVE_INT;   /* This control has an 'int' live var */
    }
  };
  
}
