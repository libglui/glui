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
     A GLUI_Column object separates all previous controls
     from subsequent controls with a vertical bar.
  */
  class GLUI_API GLUI_Column : public GLUI_Control
  {
  public:
    void draw( int x, int y );

    /**
       Create a new column, which separates the previous controls
       from subsequent controls.
 
       @param parent The panel our object is inside; or the main GLUI_Context *object.
       @param draw_bar If true, draw a visible bar between new and old controls.
    */
    GLUI_Column( GLUI_Node *parent, int draw_bar = true );
    GLUI_Column( void ) { common_init(); }

  protected:
    void common_init() {
      w            = 0;
      h            = 0;
      int_val      = 0;
      can_activate = false;
    }
  };
  
}
