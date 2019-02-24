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

  class GLUI_API GLUI_List_Item : public GLUI_Node 
  {
  public:
    GLUI_String text;
    int         id;
  };

  class GLUI_API GLUI_List : public GLUI_Control
  {
  public:
    /* GLUI_Context *List - JVK */
    GLUI_List( GLUI_Node *parent, bool scroll = false,
               int id=-1, CallBack callback=CallBack() );
    /*, GLUI_Control *object = NULL 
      ,GLUI_InterObject_CB obj_cb = NULL);*/

    GLUI_List( GLUI_Node *parent,
               GLUI_String& live_var, bool scroll = false, 
               int id=-1, 
               CallBack callback=CallBack()
               /*,GLUI_Control *object = NULL */
               /*,GLUI_InterObject_CB obj_cb = NULL*/);


    GLUI_String         orig_text;
    int                 draw_text_only;
    int                 start_line;
    int                 num_lines;
    int                 curr_line;
    int                 visible_lines;
    GLUI_Scrollbar      *scrollbar;
    GLUI_List_Item      items_list;
    GLUI_Control        *associated_object;
    CallBack             obj_cb;
    int                 cb_click_type;
    int                 last_line;
    int                 last_click_time;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler( int key,int modifiers );
  
    void activate( int how );
    void deactivate( void );

    void draw( int x, int y );

    int  mouse_over( int state, int x, int y );

    int get_box_width();
    int  find_word_break( int start, int direction );
    int  substring_width( const char *t, int start, int end );
    int  find_line( int x, int y );
    void update_and_draw_text( void );
    void draw_text( const char *t, int selected, int x, int y );
    void update_size( void );


    int  add_item( int id, const char *text );
    int  delete_item( const char *text );
    int  delete_item( int id );
    int  delete_all();

    GLUI_List_Item *get_item_ptr( const char *text );
    GLUI_List_Item *get_item_ptr( int id );

    void dump( FILE *out, const char *text );
    void set_start_line(int l) { start_line = l; }
    static void scrollbar_callback(GLUI_Control*);
    int get_current_item() { return curr_line; }
    void set_click_type(int d) {
      cb_click_type = d; }
    void set_object_callback(CallBack cb=CallBack(), GLUI_Control*obj=NULL)
    { obj_cb=cb; associated_object=obj; }

  protected:
    void common_init()
    {
      h                     = GLUI_LIST_HEIGHT;
      w                     = GLUI_LIST_WIDTH;
      num_lines             = 0;
      visible_lines         = 0;
      start_line            = 0;
      curr_line             = 0;
      name[0]               = '\0';
      active_type           = GLUI_CONTROL_ACTIVE_PERMANENT;
      can_activate          = true;
      spacebar_mouse_click  = false;
      scrollbar             = NULL;
      draw_text_only        = false;
      cb_click_type         = GLUI_SINGLE_CLICK;
      last_line             = -1;
      last_click_time       = 0;
      associated_object     = NULL;
    };
    void common_construct(
                          GLUI_Node *parent,
                          GLUI_String* live_var, bool scroll,
                          int id,
                          CallBack callback
                          /*,GLUI_Control *object*/
                          /*,GLUI_InterObject_CB obj_cb*/);
  };
  
}
