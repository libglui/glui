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
     An onscreen rotation controller--allows the user to interact with
     a 3D rotation via a spaceball-like interface.
  */
  class GLUI_API GLUI_Rotation : public GLUI_Mouse_Interaction
  {
  public:
    Arcball        *ball;
    GLUquadricObj *quadObj;
    bool           can_spin, spinning;
    float          damping;
  
    int  iaction_mouse_down_handler( int local_x, int local_y );
    int  iaction_mouse_up_handler( int local_x, int local_y, bool inside );
    int  iaction_mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  iaction_special_handler( int key, int modifiers );
    void iaction_init( void ) { init_ball(); }
    void iaction_draw_active_area_persp( void );
    void iaction_draw_active_area_ortho( void );
    void iaction_dump( FILE *output );

    /*  void update_size( void ); */
    /*  void draw( int x, int y ); */
    /*  int mouse_over( int state, int x, int y ); */

    void setup_texture( void );
    void setup_lights( void );
    void draw_ball( float radius );

    void init_ball( void );

    void reset( void );

    bool needs_idle( void ) const;
    void idle( void );

    void copy_float_array_to_ball( void );
    void copy_ball_to_float_array( void );

    void set_spin( float damp_factor );

    GLUI_Rotation( GLUI_Node *parent, const char *name, float *live_var=NULL,
                   int id=-1, CallBack callback=CallBack() );
    GLUI_Rotation(void) { common_init(); }

  protected:
    void common_init();
  };
  
}
