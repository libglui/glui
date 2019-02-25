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
     A GLUI_Panel contains a group of related controls.
  */
  class GLUI_API GLUI_Panel : public GLUI_Control
  {
  public:

    /**
       Create a new panel.  A panel groups together a set of related controls.
 
       @param parent The outer panel our panel is inside; or the main GLUI_Context *object.
       @param name The string name at the top of our panel.
       @param type Optional style to display the panel with--GLUI_PANEL_EMBOSSED by default.
       GLUI_PANEL_RAISED causes the panel to appear higher than the surroundings.
       GLUI_PANEL_NONE causes the panel's outline to be invisible.
    */
    GLUI_Panel( GLUI_Node *parent, const char *name, 
                int type=GLUI_PANEL_EMBOSSED );
    GLUI_Panel() { common_init(); }

    void draw( int x, int y );
    void set_name( const char *text );
    void set_type( int new_type );

    void update_size( void );

  protected:
    void common_init( void ) {
      w            = 300;
      h            = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
      int_val      = GLUI_PANEL_EMBOSSED;
      alignment    = GLUI_ALIGN_CENTER;
      is_container = true; 
      can_activate = false;
      name="";
    };
  };
  
}
