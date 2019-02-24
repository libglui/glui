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

  class GLUI_API GLUI_RadioButton : public GLUI_Control
  {
  public:
    int orig_value;
    bool currently_inside;
    int text_x_offset;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );

    void draw( int x, int y );
    void update_size( void );

    void draw_active_area( void );
    void draw_checked( void );
    void draw_unchecked( void );
    void draw_O( void );

    GLUI_RadioButton( GLUI_RadioGroup *group, const char *name );
    GLUI_RadioGroup *group;

  protected:
    void common_init()
    {
      glui_format_str( name, "RadioButton: %p", (void *) this );
      h              = GLUI_RADIOBUTTON_SIZE;
      group          = NULL;
      orig_value     = -1;
      text_x_offset  = 18;
      can_activate   = true;
    }
  };

}
