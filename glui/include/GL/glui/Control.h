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
     All the GUI objects inherit from GLUI_Control: buttons,
     checkboxes, labels, edit boxes, scrollbars, etc.
     Most of the work of this class is in routing events,
     like keystrokes, mouseclicks, redraws, and sizing events.
 
     Yes, this is a huge and hideous class.  It needs to be 
     split up into simpler subobjects.  None of the data members
     should be directly accessed by users (they should be protected,
     not public); only subclasses.
  */
  class GLUI_API GLUI_Control : public GLUI_Node 
  {
  public:

    /** Onscreen coordinates */
    int             w, h;                        /* dimensions of control */
    int             x_abs, y_abs;
    int             x_off, y_off_top, y_off_bot; /* INNER margins, by which child
                                                    controls are indented */
    int             contain_x, contain_y; 
    int             contain_w, contain_h;
    /* if this is a container control (e.g., 
       radiogroup or panel) this indicated dimensions
       of inner area in which controls reside */

    /** "activation" for tabbing between controls. */
    int             active_type; ///< "GLUI_CONTROL_ACTIVE_..."
    bool            active;       ///< If true, we've got the focus
    bool            can_activate; ///< If false, remove from tab order.
    bool            spacebar_mouse_click; ///< Spacebar simulates click.
    
    /** Callbacks */
    long            user_id;  ///< Integer to pass to callback function.
    CallBack         callback; ///< User callback function, or NULL.

    /** Variable value storage */
    float           float_val;        /**< Our float value */
    int             int_val;          /**< Our integer value */
    float           float_array_val[GLUI_DEF_MAX_ARRAY];
    int             float_array_size;
    GLUI_String     text;       /**< The text inside this control */
    
    /** "Live variable" updating */
    void           *ptr_val { nullptr };          /**< A pointer to the user's live variable value */
    int             live_type;
    bool            live_inited;
    /* These variables store the last value that live variable was known to have. */
    int             last_live_int;  
    float           last_live_float;
    GLUI_String     last_live_text;
    float           last_live_float_array[GLUI_DEF_MAX_ARRAY];
    
    /** Properties of our control */    
    GLUI_Context           *context { nullptr };       /**< Our containing event handler (NEVER NULL during event processing!) */
    bool            is_container;  /**< Is this a container class (e.g., panel) */
    int             alignment;
    bool            enabled;    /**< Is this control grayed out? */
    GLUI_String     name;       /**< The name of this control */
    void           *font;       /**< Our glutbitmap font */
    bool            collapsible, is_open;
    GLUI_Node       collapsed_node;
    bool            hidden; /* Collapsed controls (and children) are hidden */
    int             char_widths[CHAR_WIDTH_HASH_SIZE][2]; /* Character width hash table */

  public:
    /*** Get/Set values ***/
    virtual void   set_name( const char *string );
    virtual void   set_int_val( int new_int )         { int_val = new_int; output_live(true); }
    virtual void   set_float_val( float new_float )   { float_val = new_float; output_live(true); }
    virtual void   set_ptr_val( void *new_ptr )       { ptr_val = new_ptr; output_live(true); }
    virtual void   set_float_array_val( float *array_ptr );

    virtual float  get_float_val( void )              { return float_val; }
    virtual int    get_int_val( void )                { return int_val; }
    virtual void   get_float_array_val( float *array_ptr );
    virtual int    get_id( void ) const { return user_id; }
    virtual void   set_id( int id ) { user_id=id; }

    virtual int mouse_down_handler( int local_x, int local_y )                 { return false; }
    virtual int mouse_up_handler( int local_x, int local_y, bool inside )       { return false; }
    virtual int mouse_held_down_handler( int local_x, int local_y, bool inside) { return false; }
    virtual int key_handler( unsigned char key, int modifiers )                { return false; }
    virtual int special_handler( int key,int modifiers )                       { return false; }

    virtual void update_size( void )     { }
    virtual void idle( void )            { }
    virtual int  mouse_over( int state, int x, int y ) { return false; }

    virtual void enable( void ); 
    virtual void disable( void );
    virtual void activate( int how )     { active = true; }
    virtual void deactivate( void )     { active = false; }

    /** Hide (shrink into a rollout) and unhide (expose from a rollout) */
    void         hide_internal( int recurse );
    void         unhide_internal( int recurse );

    /** Return true if it currently makes sense to draw this class. */
    int          can_draw( void ) { return (context != nullptr && hidden == false); }

    /** Redraw this control.
        In single-buffering mode (drawing to GL_FRONT), this is just 
        a call to translate_and_draw_front (after a can_draw() check).
        In double-buffering mode (drawing to GL_BACK), this queues up 
        a redraw and returns false, since you shouldn't draw yet.
    */
    void          redraw(void);
    
    /** Redraw everybody in our window. */
    void         redraw_window(void);

    virtual void align( void );
    void         pack( int x, int y );    /* Recalculate positions and offsets */
    void         pack_old( int x, int y );    
    void         draw_recursive( int x, int y );
    int          set_to_glut_window( void );
    void         restore_window( int orig );
    void         translate_and_draw_front( void );
    void         translate_to_origin( void ) 
    {glTranslatef((float)x_abs+.5,(float)y_abs+.5,0.0);}
    virtual void draw( int x, int y )=0;
    void         set_font( void *new_font );
    void        *get_font( void );
    int          string_width( const char *text );
    int          string_width( const GLUI_String &str ) 
    { return string_width(str.c_str()); }
    int          char_width( char c );

    void         draw_name( int x, int y );
    void         draw_box_inwards_outline( int x_min, int x_max, 
                                           int y_min, int y_max );
    void         draw_box( int x_min, int x_max, int y_min, int y_max,
                           float r, float g, float b );
    void         draw_bkgd_box( int x_min, int x_max, int y_min, int y_max );
    void         draw_emboss_box( int x_min, int x_max,int y_min,int y_max);
    void         draw_string( const char *text );
    void         draw_string( const GLUI_String &s ) 
    { draw_string(s.c_str()); }
    void         draw_char( char c );
    void         draw_active_box( int x_min, int x_max, int y_min, int y_max );
    void         set_to_bkgd_color( void );

    void         set_w( int new_w );
    void         set_h( int new_w );
    void         set_alignment( int new_align );
    void         sync_live( int recurse, int draw );  /* Reads live variable */
    void         init_live( void );
    void         output_live( int update_main_gfx );        /** Writes live variable **/
    virtual void set_text( const char *t )   {}
    void         execute_callback( void );
    void         get_this_column_dims( int *col_x, int *col_y, 
                                       int *col_w, int *col_h, 
                                       int *col_x_off, int *col_y_off );
    virtual bool needs_idle( void ) const;
    virtual bool wants_tabs() const      { return false; }

    GLUI_Control(void) 
      {
        x_off          = GLUI_XOFF;
        y_off_top      = GLUI_YOFF;
        y_off_bot      = GLUI_YOFF;
        x_abs          = GLUI_XOFF;
        y_abs          = GLUI_YOFF;
        active         = false;
        enabled        = true;
        int_val        = 0;
        last_live_int  = 0;
        float_array_size = 0;
        glui_format_str(name, "Control: %p", this);
        float_val      = 0.0;
        last_live_float = 0.0;
        w              = GLUI_DEFAULT_CONTROL_WIDTH;
        h              = GLUI_DEFAULT_CONTROL_HEIGHT;
        font           = NULL;
        active_type    = GLUI_CONTROL_ACTIVE_MOUSEDOWN;
        alignment      = GLUI_ALIGN_LEFT;
        is_container   = false;
        can_activate   = true;         /* By default, you can activate a control */
        spacebar_mouse_click = true;    /* Does spacebar simulate a mouse click? */
        live_type      = GLUI_LIVE_NONE;
        text = "";
        last_live_text = "";
        live_inited    = false;
        collapsible    = false;
        is_open        = true;
        hidden         = false;
        memset(char_widths, -1, sizeof(char_widths)); /* JVK */
        int i;
        for( i=0; i<GLUI_DEF_MAX_ARRAY; i++ )
          float_array_val[i] = last_live_float_array[i] = 0.0;
      }

    virtual ~GLUI_Control();
  };
  
}
