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
     A top-level window.  The GLUI_Master GLUT callback can route events
     to the callbacks in this class, for arbitrary use by external users.
     (see GLUI_Master_Object::set_glutKeyboardFunc).
 
     This entire approach seems to be superceded by the "subwindow" flavor
     of GLUI_Context *.
  */
  class GLUI_API GLUI_Glut_Window : public GLUI_Node 
  {
  public:
    GLUI_Glut_Window();

    int    glut_window_id;

    /*********** Pointers to GLUT callthrough functions *****/
    void (*glut_keyboard_CB)(unsigned char, int, int);
    void (*glut_special_CB)(int, int, int);
    void (*glut_reshape_CB)(int, int);
    void (*glut_passive_motion_CB)(int,int);
    void (*glut_mouse_CB)(int,int,int,int);
    void (*glut_visibility_CB)(int);
    void (*glut_motion_CB)(int,int);
    void (*glut_display_CB)(void);
    void (*glut_entry_CB)(int);
  };
  
}
