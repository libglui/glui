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

  class GLUI_API GLUI_TextBox : public GLUI_Control
  {
  public:
    /* GLUI_Context *Textbox - JVK */
    GLUI_TextBox(GLUI_Node *parent, GLUI_String &live_var,
                 bool scroll = false, int id=-1, CallBack callback=CallBack() );
    GLUI_TextBox( GLUI_Node *parent,
                  bool scroll = false, int id=-1,
                  CallBack callback=CallBack() );

    GLUI_String         orig_text;
    int                 insertion_pt;
    int                 substring_start; /*substring that gets displayed in box*/
    int                 substring_end;  
    int                 sel_start, sel_end;  /* current selection */
    int                 last_insertion_pt;
    int                 draw_text_only;
    int                 tab_width;
    int                 start_line;
    int                 num_lines;
    int                 curr_line;
    int                 visible_lines;
    int                 insert_x;        /* Similar to "insertion_pt", these variables keep */
    int                 insert_y;        /* track of where the ptr is, but in pixels */
    int                 keygoal_x;       /* where up down keys would like to put insertion pt*/
    GLUI_Scrollbar     *scrollbar;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler( int key,int modifiers );
  
    void activate( int how );
    void deactivate( void );

    void enable( void );
    void disable( void );

    void draw( int x, int y );

    int  mouse_over( int state, int x, int y );

    int get_box_width();
    int  find_word_break( int start, int direction );
    int  substring_width( int start, int end, int initial_width=0 );
    void clear_substring( int start, int end );
    int  find_insertion_pt( int x, int y );
    int  update_substring_bounds( void );
    void update_and_draw_text( void );
    void draw_text( int x, int y );
    void draw_insertion_pt( void );
    void update_x_offsets( void );
    void update_size( void );

    void set_text( const char *text );
    const char *get_text( void )         { return text.c_str(); }

    void dump( FILE *out, const char *text );
    void set_tab_w(int w) { tab_width = w; }
    void set_start_line(int l) { start_line = l; }
    static void scrollbar_callback(GLUI_Control*);

    bool wants_tabs( void ) const { return true; }

  protected:
    void common_init()
    {
      h                     = GLUI_TEXTBOX_HEIGHT;
      w                     = GLUI_TEXTBOX_WIDTH;
      tab_width             = GLUI_TAB_WIDTH;
      num_lines             = 0;
      visible_lines         = 0;
      start_line            = 0;
      curr_line             = 0;
      insert_y              = -1;
      insert_x              = -1;
      insertion_pt          = -1;
      last_insertion_pt     = -1;
      name[0]               = '\0';
      substring_start       = 0;
      substring_end         = 2;
      sel_start             = 0;
      sel_end               = 0;
      active_type           = GLUI_CONTROL_ACTIVE_PERMANENT;
      can_activate          = true;
      spacebar_mouse_click  = false;
      scrollbar             = NULL;
      draw_text_only        = false;
    }
    void common_construct(
                          GLUI_Node *parent, GLUI_String *live_var, 
                          bool scroll, int id, CallBack callback); 
  };
  
}
