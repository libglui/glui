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
     An onscreen, clickable button--an outlined label that 
     can be clicked.  When clicked, a button
     calls its CallBack callback with its ID.
  */
  class GLUI_API GLUI_Button : public GLUI_Control
  {
  public:
    bool currently_inside;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );

    void draw( int x, int y );
    void draw_pressed( void );
    void draw_text( int sunken );

    void update_size( void );

    /**
       Create a new button.
 
       @param parent The panel our object is inside; or the main GLUI_Context *object.
       @param name The text inside the button.
       @param id Optional ID number, to pass to the optional callback function.
       @param callback Optional callback function, taking either the int ID or control.
    */
    GLUI_Button( GLUI_Node *parent, const char *name, 
                 int id=-1, CallBack cb=CallBack() );
    GLUI_Button( void ) { common_init(); };

  protected:
    void common_init(void) {
      glui_format_str(name, "Button: %p", this );
      h            = GLUI_BUTTON_SIZE;
      w            = 100;
      alignment    = GLUI_ALIGN_CENTER;
      can_activate = true;
    }
  };
  
}
