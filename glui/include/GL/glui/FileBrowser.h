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
     A list of files the user can select from.
  */
  class GLUI_API GLUI_FileBrowser : public GLUI_Panel
  {
  public:
    /**
       Create a new list of files the user can select from.
 
       @param parent The panel our object is inside; or the main GLUI_Context *object.
       @param name Prompt to give to the user at the top of the file browser.
       @param frame_type Optional style to display the panel with--GLUI_PANEL_EMBOSSED by default.
       GLUI_PANEL_RAISED causes the panel to appear higher than the surroundings.
       GLUI_PANEL_NONE causes the panel's outline to be invisible.
       @param id Optional ID number, to pass to the optional callback function.
       @param callback Optional callback function, taking either the int ID or control.
    */
    GLUI_FileBrowser( GLUI_Node *parent, 
                      const char *name,
                      int frame_type = GLUI_PANEL_EMBOSSED,
                      int user_id = -1,
                      CallBack callback = CallBack());

    GLUI_List *list;
    GLUI_String current_dir;

    void fbreaddir(const char *);
    static void dir_list_callback(GLUI_Control*);

    void set_w(int w);
    void set_h(int h);
    const char* get_file() { return file.c_str(); }
    void set_allow_change_dir(int c) { allow_change_dir = c; }

  protected:
    void common_init() 
    {
      w            = GLUI_DEFAULT_CONTROL_WIDTH;
      h            = GLUI_DEFAULT_CONTROL_HEIGHT;
      int_val      = GLUI_PANEL_EMBOSSED;
      alignment    = GLUI_ALIGN_CENTER;
      is_container = true; 
      can_activate = false;
      allow_change_dir = true;
      last_item    = -1;
      user_id      = -1;
      name         = "";
      current_dir  = ".";
      file         = "";
    };

  private:
    int last_item;
    GLUI_String file;
    int allow_change_dir;

  };
  
}
