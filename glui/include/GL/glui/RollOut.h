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
     A rollout contains a set of controls,
     like a panel, but can be collapsed to just the name.
  */
  class GLUI_API GLUI_Rollout : public GLUI_Panel
  {
  public:

    /**
       Create a new rollout.  A rollout contains a set of controls,
       like a panel, but can be collapsed to just the name.
 
       @param parent The panel our object is inside; or the main GLUI_Context *object.
       @param name String to show at the top of the rollout.
       @param open Optional boolean.  If true (the default), the rollout's controls are displayed.
       If false, the rollout is closed to display only the name.
       @param type Optional style to display the panel with--GLUI_PANEL_EMBOSSED by default.
       GLUI_PANEL_RAISED causes the panel to appear higher than the surroundings.
       GLUI_PANEL_NONE causes the panel's outline to be invisible.
    */
    GLUI_Rollout( GLUI_Node *parent, const char *name, int open=true, 
                  int type=GLUI_PANEL_EMBOSSED );
    GLUI_Rollout( void ) { common_init(); }
    
    
    bool        currently_inside, initially_inside;
    GLUI_Button  button;

    void draw( int x, int y );
    void draw_pressed( void );
    int mouse_down_handler( int local_x, int local_y );
    int mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
        
    void  open( void ); 
    void  close( void );

    void update_size( void );

  protected:
    void common_init() {
      currently_inside = false;
      initially_inside = false;
      can_activate     = true;
      is_container     = true;
      h                = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
      w                = GLUI_DEFAULT_CONTROL_WIDTH;
      y_off_top        = 21;
      collapsible      = true;
      name = "";
    }
  };
  
}
