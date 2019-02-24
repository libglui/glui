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

  class GLUI_API GLUI_RadioGroup : public GLUI_Control
  {
  public:
    int  num_buttons;

    void draw( int x, int y );
    void set_name( const char *text );
    void set_int_val( int int_val ); 
    void set_selected( int int_val );

    void draw_group( int translate );

    GLUI_RadioGroup( GLUI_Node *parent, int *live_var=NULL,
                     int user_id=-1,CallBack callback=CallBack() );
    GLUI_RadioGroup( void ) { common_init(); }

  protected:
    void common_init( void ) {
      x_off         = 0;
      y_off_top     = 0;
      y_off_bot     = 0;
      is_container  = true;
      w             = 300;
      h             = 300;
      num_buttons   = 0;
      name          = "";
      can_activate  = false;
      live_type     = GLUI_LIVE_INT;
    }
  };

}
