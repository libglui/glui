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
     This is the superclass of translation and rotation widgets.
  */
  class GLUI_API GLUI_Mouse_Interaction : public GLUI_Control
  {
  public:
    /*int  get_main_area_size( void ) { return MIN( h-18,  */
    int            draw_active_area_only;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  special_handler( int key, int modifiers );
    void update_size( void );
    void draw( int x, int y );
    void draw_active_area( void );

    /***  The following methods (starting with "iaction_") need to
          be overloaded  ***/
    virtual int  iaction_mouse_down_handler( int local_x, int local_y ) = 0;
    virtual int  iaction_mouse_up_handler( int local_x, int local_y, bool inside )=0;
    virtual int  iaction_mouse_held_down_handler( int local_x, int local_y, bool inside )=0;
    virtual int  iaction_special_handler( int key, int modifiers )=0;
    virtual void iaction_draw_active_area_persp( void )=0;
    virtual void iaction_draw_active_area_ortho( void )=0;
    virtual void iaction_dump( FILE *output )=0;
    virtual void iaction_init( void ) = 0;
  
    GLUI_Mouse_Interaction( void ) {
      glui_format_str( name, "Mouse_Interaction: %p", this );
      w              = GLUI_MOUSE_INTERACTION_WIDTH;
      h              = GLUI_MOUSE_INTERACTION_HEIGHT;
      can_activate   = true;
      live_type      = GLUI_LIVE_NONE;
      alignment      = GLUI_ALIGN_CENTER;
      draw_active_area_only = false;
    }
  };

  
}
