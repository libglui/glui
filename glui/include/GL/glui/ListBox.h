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

  class GLUI_API GLUI_Listbox_Item : public GLUI_Node 
  {
  public:
    GLUI_String text;
    int         id;
  };

  class GLUI_API GLUI_Listbox : public GLUI_Control
  {
  public:
    GLUI_String       curr_text;
    GLUI_Listbox_Item items_list;
    int               depressed;

    int  orig_value;
    bool currently_inside;
    int  text_x_offset, title_x_offset;
    int  glut_menu_id;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler( int key,int modifiers );

    void update_size( void );
    void draw( int x, int y );
    int  mouse_over( int state, int x, int y );

    void set_int_val( int new_val );
    void dump( FILE *output );

    int  add_item( int id, const char *text );
    int  delete_item( const char *text );
    int  delete_item( int id );
    int  sort_items( void );

    int  do_selection( int item );

    GLUI_Listbox_Item *get_item_ptr( const char *text );
    GLUI_Listbox_Item *get_item_ptr( int id );
  

    GLUI_Listbox( GLUI_Node *parent,
                  const char *name, int *live_var=NULL,
                  int id=-1, CallBack callback=CallBack() );
    GLUI_Listbox( void ) { common_init(); }

  protected:
    /** Change w and return true if we need to be widened to fit the current item. */
    bool recalculate_item_width( void );
    void common_init() {
      glui_format_str( name, "Listbox: %p", this );
      w              = GLUI_EDITTEXT_WIDTH;
      h              = GLUI_EDITTEXT_HEIGHT;
      orig_value     = -1;
      title_x_offset = 0;
      text_x_offset  = 55;
      can_activate   = true;
      curr_text      = "";
      live_type      = GLUI_LIVE_INT;  /* This has an integer live var */
      depressed      = false;
      glut_menu_id   = -1;
    }

    ~GLUI_Listbox();
  };
}
