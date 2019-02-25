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
     One collapsible entry in a GLUI_TreePanel.
  */
  class GLUI_API GLUI_Tree : public GLUI_Panel
  {
  public:
    GLUI_Tree(GLUI_Node *parent, const char *name, 
              int open=false, int inset=0);

  private:
    int level;   // how deep is this node
    float red;   //Color coding of column line
    float green;
    float blue;
    float lred;   //Color coding of level name
    float lgreen;
    float lblue;
    int id;
    GLUI_Column *column;
    int is_current;          // Whether this tree is the
    // current root in a treePanel
    int child_number;
    int format;

  public:
    bool        currently_inside, initially_inside;
    GLUI_Button  button;
    GLUI_String  level_name; // level name, eg: 1.1.2, III, or 3
    GLUI_TreePanel *panel; 

    void draw( int x, int y );
    void draw_pressed( void );
    int mouse_down_handler( int local_x, int local_y );
    int mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    void set_column(GLUI_Column *c) { column = c; }
    void  open( void ); 
    void  close( void );

    /*   void set_name( const char *text )   { panel.set_name( text ); }; */
    void update_size( void );
    void set_id(int i) { id = i; }
    void set_level(int l) { level = l; }
    void set_format(int f) { format = f; }
    void set_current(int c) { is_current = c; }
    int get_id() { return id; }
    int get_level() { return level; }
    int get_child_number() { return child_number; }
    void enable_bar() { if (column) { column->int_val = 1;  set_color(red, green, blue); } }
    void disable_bar() { if (column) { column->int_val = 0;  } } 
    void set_child_number(int c) { child_number = c; } 
    void set_level_color(float r, float g, float b) { 
      lred = r;
      lgreen = g;
      lblue  = b;
    }
    void set_color(float r, float g, float b) { 
      red = r;
      green = g;
      blue  = b;
    }
  protected:
    void common_init()
    {
      currently_inside = false;
      initially_inside = false;
      can_activate     = true;
      is_container     = true;
      h                = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
      w                = GLUI_DEFAULT_CONTROL_WIDTH;
      y_off_top        = 21;
      collapsible      = true;
      red              = .5;
      green            = .5;
      blue             = .5;
      lred             = 0;
      lgreen           = 0;
      lblue            = 0;
      column           = NULL;
      is_current       = 0;
      child_number     = 0;
      format           = 0;
      panel            = NULL;
      name             = "";
      level_name       = "";
      level            = 0;
    
    };
  };
  
}
