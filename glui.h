/*

  glui.h - Main header for GLUI User Interface Toolkit

  GLUI User Interface Toolkit (LGPL)
  Copyright (c) 1998 Paul Rademacher

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef GLUI_GLUI_H
#define GLUI_GLUI_H

#if 1

  #ifdef __APPLE__
  #include <GLUT/glut.h>
  #else
  #include <GL/glut.h>
  #endif

#else

  /* GLUI does not yet work perfectly      */
  /* with FreeGLUT - use at your own risk. */
  
  #include <GL/freeglut.h>
  #include <GL/freeglut_ext.h>

#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>

#define GLUI_VERSION 2.2f    /********** Current version **********/

#if defined(MSC_VER)
#pragma comment(lib, "glui32.lib")  // Link against GLUI library  
#endif

/********** Do some basic defines *******/

#ifndef Byte
#define Byte unsigned char
#endif

#ifndef _RGBC_
class RGBc {
public:
  Byte r, g, b;
    
  void set(Byte r,Byte g,Byte b) {this->r=r;this->g=g;this->b=b;}
    
  RGBc( void ) {}
  RGBc( Byte r, Byte g, Byte b ) { set( r, g, b ); }
};
#define _RGBC_
#endif

/********** List of GLUT callbacks ********/

enum GLUI_Glut_CB_Types 
{ 
    GLUI_GLUT_RESHAPE,
    GLUI_GLUT_KEYBOARD,
    GLUI_GLUT_DISPLAY,
    GLUI_GLUT_MOUSE,
    GLUI_GLUT_MOTION,
    GLUI_GLUT_SPECIAL,
    GLUI_GLUT_PASSIVE_MOTION,  
    GLUI_GLUT_ENTRY,
    GLUI_GLUT_VISIBILITY  
};

/********** List of control types **********/

enum GLUI_Control_Types 
{
    GLUI_CONTROL_CHECKBOX = 1,
    GLUI_CONTROL_BUTTON,
    GLUI_CONTROL_RADIOBUTTON,
    GLUI_CONTROL_RADIOGROUP,
    GLUI_CONTROL_SLIDER,
    GLUI_CONTROL_STATICTEXT,
    GLUI_CONTROL_EDITTEXT,
    GLUI_CONTROL_BITMAP,
    GLUI_CONTROL_PANEL,
    GLUI_CONTROL_SPINNER,
    GLUI_CONTROL_SEPARATOR,
    GLUI_CONTROL_COLUMN,
    GLUI_CONTROL_LISTBOX,
    GLUI_CONTROL_MOUSE_INTERACTION,
    GLUI_CONTROL_ROTATION,
    GLUI_CONTROL_TRANSLATION,
    GLUI_CONTROL_ROLLOUT,
    GLUI_CONTROL_TEXTBOX,      /* New GLUI Widgets - JVK */
    GLUI_CONTROL_LIST,
    GLUI_CONTROL_FILEBROWSER,
    GLUI_CONTROL_TREE,
    GLUI_CONTROL_TREEPANEL,
    GLUI_CONTROL_SCROLL
};


/********* Constants for window placement **********/

#define GLUI_XOFF                       6
#define GLUI_YOFF                       6
#define GLUI_ITEMSPACING                3
#define GLUI_CHECKBOX_SIZE             13
#define GLUI_RADIOBUTTON_SIZE          13
#define GLUI_BUTTON_SIZE               20
#define GLUI_STATICTEXT_SIZE           13
#define GLUI_SEPARATOR_HEIGHT           8
#define GLUI_DEFAULT_CONTROL_WIDTH    100
#define GLUI_DEFAULT_CONTROL_HEIGHT    13 
#define GLUI_EDITTEXT_BOXINNERMARGINX   3
#define GLUI_EDITTEXT_HEIGHT           20
#define GLUI_EDITTEXT_WIDTH           130
#define GLUI_EDITTEXT_MIN_INT_WIDTH    35
#define GLUI_EDITTEXT_MIN_TEXT_WIDTH   50
#define GLUI_PANEL_NAME_DROP            8
#define GLUI_PANEL_EMBOSS_TOP           4
/* #define GLUI_ROTATION_WIDTH         60 */
/* #define GLUI_ROTATION_HEIGHT        78 */
#define GLUI_ROTATION_WIDTH            50
#define GLUI_ROTATION_HEIGHT           (GLUI_ROTATION_WIDTH+18)
#define GLUI_MOUSE_INTERACTION_WIDTH   50
#define GLUI_MOUSE_INTERACTION_HEIGHT  (GLUI_MOUSE_INTERACTION_WIDTH)+18

/** Different panel control types **/
#define GLUI_PANEL_NONE      0
#define GLUI_PANEL_EMBOSSED  1
#define GLUI_PANEL_RAISED    2

/**  Max # of els in control's float_array  **/
#define GLUI_DEF_MAX_ARRAY  30

/********* The control's 'active' behavior *********/
#define GLUI_CONTROL_ACTIVE_MOUSEDOWN       1
#define GLUI_CONTROL_ACTIVE_PERMANENT       2

/********* Control alignment types **********/
#define GLUI_ALIGN_CENTER   1
#define GLUI_ALIGN_RIGHT    2
#define GLUI_ALIGN_LEFT     3

/********** Limit types - how to limit spinner values *********/
#define GLUI_LIMIT_NONE    0
#define GLUI_LIMIT_CLAMP   1
#define GLUI_LIMIT_WRAP    2

/********** Translation control types ********************/
#define GLUI_TRANSLATION_XY 0
#define GLUI_TRANSLATION_Z  1
#define GLUI_TRANSLATION_X  2
#define GLUI_TRANSLATION_Y  3

#define GLUI_TRANSLATION_LOCK_NONE 0
#define GLUI_TRANSLATION_LOCK_X    1
#define GLUI_TRANSLATION_LOCK_Y    2

/********** How was a control activated? *****************/
#define GLUI_ACTIVATE_MOUSE 1
#define GLUI_ACTIVATE_TAB   2

/********** What type of live variable does a control have? **********/
#define GLUI_LIVE_NONE          0
#define GLUI_LIVE_INT           1
#define GLUI_LIVE_FLOAT         2
#define GLUI_LIVE_TEXT          3
#define GLUI_LIVE_DOUBLE        4
#define GLUI_LIVE_FLOAT_ARRAY   5

/************* Textbox and List Defaults - JVK ******************/
#define GLUI_TEXTBOX_HEIGHT           130
#define GLUI_TEXTBOX_WIDTH             130
#define GLUI_LIST_HEIGHT           130
#define GLUI_LIST_WIDTH             130
#define GLUI_DOUBLE_CLICK             1
#define GLUI_SINGLE_CLICK             0
#define GLUI_TAB_WIDTH                50 /* In pixels */
#define GLUI_TEXTBOX_BOXINNERMARGINX   3
#define GLUI_TEXTBOX_MIN_TEXT_WIDTH   50
#define GLUI_LIST_BOXINNERMARGINX   3
#define GLUI_LIST_MIN_TEXT_WIDTH   50

/*********************** TreePanel Defaults - JVK *****************************/
#define GLUI_TREEPANEL_DEFAULTS 0                /* bar, standard bar color */        
#define GLUI_TREEPANEL_ALTERNATE_COLOR 1         /* Alternate between 8 different bar colors */
#define GLUI_TREEPANEL_ENABLE_BAR  2             /* enable the bar */
#define GLUI_TREEPANEL_DISABLE_BAR  4            /* disable the bar */
#define GLUI_TREEPANEL_DISABLE_DEEPEST_BAR 8     /* disable only the deepest bar */
#define GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY  16 /* disable only the bar of the last child of each root */
#define GLUI_TREEPANEL_DISPLAY_HIERARCHY  32     /* display some sort of hierachy in the tree node title */
#define GLUI_TREEPANEL_HIERARCHY_NUMERICDOT  64  /* display hierarchy in 1.3.2 (etc... ) format */
#define GLUI_TREEPANEL_HIERARCHY_LEVEL_ONLY  128 /* display hierarchy as only the level depth */
 
/******************* GLUI Scrollbar Defaults - JVK ***************************/
#define  GLUI_SCROLL_ARROW_WIDTH  16
#define  GLUI_SCROLL_ARROW_HEIGHT 16
#define  GLUI_SCROLL_BOX_MIN_HEIGHT 5
#define  GLUI_SCROLL_BOX_STD_HEIGHT 16
#define  GLUI_SCROLL_STATE_NONE   0
#define  GLUI_SCROLL_STATE_UP     1
#define  GLUI_SCROLL_STATE_DOWN   2
#define  GLUI_SCROLL_STATE_BOTH   3
#define  GLUI_SCROLL_STATE_SCROLL 4
#define  GLUI_SCROLL_DEFAULT_GROWTH_EXP   1.05f
#define  GLUI_SCROLL_VERTICAL           0
#define  GLUI_SCROLL_HORIZONTAL         1


/* Default String Size for Dynamic String Class - JVK */
#define GLUI_STRING_SIZE            300

/* Size of the character width hash table for faster lookups */
#define CHAR_WIDTH_HASH_SIZE 126

/**********  Translation codes  **********/

enum TranslationCodes  
{
    GLUI_TRANSLATION_MOUSE_NONE = 0,
    GLUI_TRANSLATION_MOUSE_UP,
    GLUI_TRANSLATION_MOUSE_DOWN,
    GLUI_TRANSLATION_MOUSE_LEFT,
    GLUI_TRANSLATION_MOUSE_RIGHT,
    GLUI_TRANSLATION_MOUSE_UP_LEFT,
    GLUI_TRANSLATION_MOUSE_UP_RIGHT,
    GLUI_TRANSLATION_MOUSE_DOWN_LEFT,
    GLUI_TRANSLATION_MOUSE_DOWN_RIGHT
};

/************ A string type for us to use **********
   This is a dynamically sized string class. It's
   necessary for the TextBox which can have very
   large strings.
*/

class GLUI_String
{
private:
  char *string;

public:
  unsigned int size;
  char &operator[]( int i ) {
    char *temp;
    if (i >= int(size)) {
      temp = string;
      if (i < int(size)*2) { // double the size of the string
        string = (char *)malloc(sizeof(char)*size*2); 
        size = size*2;
        string[size-1] = '\0'; /* safety */
      } else { // If i is really big compared to size, allocate up to i
        string = (char *)malloc(sizeof(char)*(i+1)); 
        size = i + 1;
        string[size-1] = '\0'; /* safety */
      }
      strcpy(string, temp);
      free(temp);
    }
    return string[i];
  }

  operator char*() { return (char*) string; };
  /*    operator void*() { return (void*) &string[0]; }; */

  GLUI_String( void ) {
    string = (char *)malloc(sizeof(char)*GLUI_STRING_SIZE);
    size = GLUI_STRING_SIZE;
    string[0] = '\0'; /* Safety */
    string[size-1] = '\0';
  }

  GLUI_String( char *text ) {
    string = (char *)malloc(sizeof(char)*GLUI_STRING_SIZE);
    size = GLUI_STRING_SIZE;
    string[0] = '\0'; /* Safety */
    string[size-1] = '\0';
    if (text[0] == '\0' || strlen(text) < size) {
      strcpy( string, text );
    }
  }
  ~GLUI_String() {if (string != NULL) free(string);}
};

/********* Pre-declare the various classes *********/

class GLUI;
class GLUI_Control;
class GLUI_Listbox;
class GLUI_Rotation;
class GLUI_Translation;
class GLUI_Mouse_Interaction;
class GLUI_Checkbox;
class GLUI_Slider;
class GLUI_Button;
class GLUI_StaticText;
class GLUI_Bitmap;
class GLUI_EditText;
class GLUI_Node;
class GLUI_Main;
class GLUI_Panel;
class GLUI_Spinner;
class GLUI_RadioButton;
class GLUI_RadioGroup;
class GLUI_Separator;
class GLUI_Column;
class GLUI_Master;
class GLUI_Glut_Window;
class GLUI_Rollout;
class GLUI_FileBrowser;  /* New Widgets - JVK */
class GLUI_Tree;
class GLUI_TreeNode;
class GLUI_TreePanel;
class GLUI_Scrollbar;
class GLUI_TextBox;
class GLUI_List;

class Arcball;

/*** Flags for GLUI class constructor ***/
#define  GLUI_SUBWINDOW          ((long)(1<<1))
#define  GLUI_SUBWINDOW_TOP      ((long)(1<<2))
#define  GLUI_SUBWINDOW_BOTTOM   ((long)(1<<3))
#define  GLUI_SUBWINDOW_LEFT     ((long)(1<<4))
#define  GLUI_SUBWINDOW_RIGHT    ((long)(1<<5))

/*** Codes for different type of edittext boxes and spinners ***/
#define GLUI_EDITTEXT_TEXT             1
#define GLUI_EDITTEXT_INT              2
#define GLUI_EDITTEXT_FLOAT            3
#define GLUI_SPINNER_INT               GLUI_EDITTEXT_INT
#define GLUI_SPINNER_FLOAT             GLUI_EDITTEXT_FLOAT
#define GLUI_SCROLL_INT                GLUI_EDITTEXT_INT
#define GLUI_SCROLL_FLOAT              GLUI_EDITTEXT_FLOAT

/*** Definition of callbacks ***/
typedef void (*GLUI_Update_CB) (int id);
typedef void (*Int1_CB)        (int);
typedef void (*Int2_CB)        (int, int);
typedef void (*Int3_CB)        (int, int, int);
typedef void (*Int4_CB)        (int, int, int, int);
typedef void (*GLUI_InterObject_CB)(void *,int); /* Used for callbacks
                                                    to/from scrollbar */

/************************************************************/
/*                                                          */
/*          Base class, for hierarchical relationships      */
/*                                                          */
/************************************************************/

class GLUI_Node 
{
    friend class GLUI_Tree;     /* JVK */
    friend class GLUI_Rollout;
    friend class GLUI_Main;

public:
    GLUI_Node();
    ~GLUI_Node();

    GLUI_Node *first_sibling();
    GLUI_Node *last_sibling();
    GLUI_Node *prev();
    GLUI_Node *next();

    GLUI_Node *first_child()   { return child_head; }
    GLUI_Node *last_child()    { return child_tail; }
    GLUI_Node *parent()        { return parent_node; }

    void link_this_to_parent_last (GLUI_Node *parent  );
    void link_this_to_parent_first(GLUI_Node *parent  );
    void link_this_to_sibling_next(GLUI_Node *sibling );
    void link_this_to_sibling_prev(GLUI_Node *sibling );
    void unlink();

    void dump( FILE *out, char *name );

protected:

    GLUI_Node *parent_node;
    GLUI_Node *child_head;
    GLUI_Node *child_tail;
    GLUI_Node *next_sibling;
    GLUI_Node *prev_sibling;
};


/************************************************************/
/*                                                          */
/*                  Standard Bitmap stuff                   */
/*                                                          */
/************************************************************/

enum GLUI_StdBitmaps_Codes 
{
    GLUI_STDBITMAP_CHECKBOX_OFF = 0,
    GLUI_STDBITMAP_CHECKBOX_ON,
    GLUI_STDBITMAP_RADIOBUTTON_OFF,
    GLUI_STDBITMAP_RADIOBUTTON_ON,
    GLUI_STDBITMAP_UP_ARROW,
    GLUI_STDBITMAP_DOWN_ARROW,
    GLUI_STDBITMAP_LEFT_ARROW,
    GLUI_STDBITMAP_RIGHT_ARROW,
    GLUI_STDBITMAP_SPINNER_UP_OFF,
    GLUI_STDBITMAP_SPINNER_UP_ON,
    GLUI_STDBITMAP_SPINNER_DOWN_OFF,
    GLUI_STDBITMAP_SPINNER_DOWN_ON,
    GLUI_STDBITMAP_SCROLLBAR_UP_OFF, 
    GLUI_STDBITMAP_SCROLLBAR_UP_ON, 
    GLUI_STDBITMAP_SCROLLBAR_DOWN_OFF, 
    GLUI_STDBITMAP_SCROLLBAR_DOWN_ON, 

    GLUI_STDBITMAP_CHECKBOX_OFF_DIS,    /*** Disactivated control bitmaps ***/
    GLUI_STDBITMAP_CHECKBOX_ON_DIS,
    GLUI_STDBITMAP_RADIOBUTTON_OFF_DIS,
    GLUI_STDBITMAP_RADIOBUTTON_ON_DIS,
    GLUI_STDBITMAP_SPINNER_UP_DIS,
    GLUI_STDBITMAP_SPINNER_DOWN_DIS,
    GLUI_STDBITMAP_SCROLLBAR_UP_DIS,
    GLUI_STDBITMAP_SCROLLBAR_DOWN_DIS,
    GLUI_STDBITMAP_LISTBOX_UP,
    GLUI_STDBITMAP_LISTBOX_DOWN,
    GLUI_STDBITMAP_LISTBOX_UP_DIS,
    GLUI_STDBITMAP_NUM_ITEMS
};

/************************************************************/
/*                                                          */
/*                  Class GLUI_Bitmap                       */
/*                                                          */
/************************************************************/

class GLUI_Bitmap 
{
	friend class GLUI_StdBitmaps;

public:
    GLUI_Bitmap();
    ~GLUI_Bitmap();

    void init(int *array);

private:
    unsigned char *pixels;
    int            w, h;
};


/************************************************************/
/*                                                          */
/*                  Class GLUI_StdBitmap                    */
/*                                                          */
/************************************************************/

class GLUI_StdBitmaps
{
public:
    GLUI_StdBitmaps(); 
	~GLUI_StdBitmaps();

	int  width (int n) const;
	int  height(int n) const;

    void draw(int n, int x, int y) const;

private:
    GLUI_Bitmap bitmaps[GLUI_STDBITMAP_NUM_ITEMS];
};

/************************************************************/
/*                                                          */
/*                     Master GLUI Class                    */
/*                                                          */
/************************************************************/

class GLUI_Master_Object 
{

    friend void glui_idle_func();
  
public:

    GLUI_Master_Object();
    ~GLUI_Master_Object();

    GLUI_Node     gluis;
    GLUI_Control *active_control, *curr_left_button_glut_menu;
    GLUI         *active_control_glui;
    int           glui_id_counter;

    GLUI_Glut_Window   *find_glut_window( int window_id );

    void           set_glutIdleFunc(void (*f)(void));

    /**************
    void (*glut_keyboard_CB)(unsigned char, int, int);
    void (*glut_reshape_CB)(int, int);
    void (*glut_special_CB)(int, int, int);
    void (*glut_mouse_CB)(int,int,int,int);

    void (*glut_passive_motion_CB)(int,int);
    void (*glut_visibility_CB)(int);
    void (*glut_motion_CB)(int,int);
    void (*glut_display_CB)(void);
    void (*glut_entry_CB)(int);
    **********/

    void  set_left_button_glut_menu_control( GLUI_Control *control );

    /********** GLUT callthroughs **********/
    /* These are the glut callbacks that we do not handle */

    void set_glutReshapeFunc (void (*f)(int width, int height));
    void set_glutKeyboardFunc(void (*f)(unsigned char key, int x, int y));
    void set_glutSpecialFunc (void (*f)(int key, int x, int y));
    void set_glutMouseFunc   (void (*f)(int, int, int, int ));

    void set_glutDisplayFunc(void (*f)(void)) {glutDisplayFunc(f);}
    void set_glutTimerFunc(unsigned int millis, void (*f)(int value), int value)
    { ::glutTimerFunc(millis,f,value);}
    void set_glutOverlayDisplayFunc(void(*f)(void)){glutOverlayDisplayFunc(f);}
    void set_glutSpaceballMotionFunc(Int3_CB f)  {glutSpaceballMotionFunc(f);}
    void set_glutSpaceballRotateFunc(Int3_CB f)  {glutSpaceballRotateFunc(f);}
    void set_glutSpaceballButtonFunc(Int2_CB f)  {glutSpaceballButtonFunc(f);}
    void set_glutTabletMotionFunc(Int2_CB f)        {glutTabletMotionFunc(f);}
    void set_glutTabletButtonFunc(Int4_CB f)        {glutTabletButtonFunc(f);}
    /*    void set_glutWindowStatusFunc(Int1_CB f)        {glutWindowStatusFunc(f);} */
    void set_glutMenuStatusFunc(Int3_CB f)            {glutMenuStatusFunc(f);}
    void set_glutMenuStateFunc(Int1_CB f)              {glutMenuStateFunc(f);}
    void set_glutButtonBoxFunc(Int2_CB f)              {glutButtonBoxFunc(f);}
    void set_glutDialsFunc(Int2_CB f)                      {glutDialsFunc(f);}  
  

    GLUI          *create_glui( char *name, long flags=0, int x=-1, int y=-1 ); 
    GLUI          *create_glui_subwindow( int parent_window, long flags=0 );
    GLUI          *find_glui_by_window_id( int window_id );
    void           get_viewport_area( int *x, int *y, int *w, int *h );
    void           auto_set_viewport();
    void           close_all();
    void           sync_live_all();

    void           reshape();

    float          get_version() { return GLUI_VERSION; }

private:
    GLUI_Node     glut_windows;
    void (*glut_idle_CB)(void);

    void          add_cb_to_glut_window(int window,int cb_type,void *cb);
};

extern GLUI_Master_Object GLUI_Master;

/************************************************************/
/*                                                          */
/*              Class for managing a GLUT window            */
/*                                                          */
/************************************************************/

class GLUI_Glut_Window : public GLUI_Node 
{
public:
    GLUI_Glut_Window(); 
	~GLUI_Glut_Window();

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

/************************************************************/
/*                                                          */
/*              Main GLUI class (not user-level)            */
/*                                                          */
/************************************************************/

class GLUI_Main : public GLUI_Node 
{
    /********** Friend classes *************/

    friend class GLUI_Control;
    friend class GLUI_Rotation;
    friend class GLUI_Translation;
    friend class GLUI;
    friend class GLUI_Master_Object;

    /*********** Friend functions **********/

    friend void glui_mouse_func(int button, int state, int x, int y);
    friend void glui_keyboard_func(unsigned char key, int x, int y);
    friend void glui_special_func(int key, int x, int y);
    friend void glui_passive_motion_func(int x, int y);
    friend void glui_reshape_func( int w, int h );
    friend void glui_visibility_func(int state);
    friend void glui_motion_func(int x, int y);
    friend void glui_entry_func(int state);
    friend void glui_display_func( void );
    friend void glui_idle_func(void);

    friend void glui_parent_window_reshape_func( int w, int h );
    friend void glui_parent_window_keyboard_func( unsigned char, int, int );
    friend void glui_parent_window_special_func( int, int, int );
    friend void glui_parent_window_mouse_func( int, int, int, int );

protected:
    /*** Variables ***/
    int           main_gfx_window_id;
    int           mouse_button_down;
    int           glut_window_id;
    int           top_level_glut_window_id;
    GLUI_Control *active_control;
    GLUI_Control *mouse_over_control;
    GLUI_Panel   *main_panel;
    int           curr_cursor;
    int           w, h;
    long          flags; 
    int           closing;
    int           parent_window;
    int           glui_id;

    /********** Misc functions *************/

    GLUI_Control  *find_control( int x, int y );
    GLUI_Control  *find_next_control( GLUI_Control *control );
    GLUI_Control  *find_next_control_rec( GLUI_Control *control );
    GLUI_Control  *find_next_control_( GLUI_Control *control );
    GLUI_Control  *find_prev_control( GLUI_Control *control );
    void           create_standalone_window( char *name, int x=-1, int y=-1 );
    void           create_subwindow( int parent,int window_alignment );
    void           setup_default_glut_callbacks( void );

    void           mouse(int button, int state, int x, int y);
    void           keyboard(unsigned char key, int x, int y);
    void           special(int key, int x, int y);
    void           passive_motion(int x, int y);
    void           reshape( int w, int h );
    void           visibility(int state);
    void           motion(int x, int y);
    void           entry(int state);
    void           display( void );
    void           idle(void);

    void (*glut_mouse_CB)(int, int, int, int);
    void (*glut_keyboard_CB)(unsigned char, int, int);
    void (*glut_special_CB)(int, int, int);
    void (*glut_reshape_CB)(int, int);


    /*********** Controls ************/

/* These are no longer used, it seems */

//    GLUI_Control *controls;
//    int           num_controls;

    int           add_control( GLUI_Node *parent, GLUI_Control *control );


    /********** Constructors and Destructors ***********/

    GLUI_Main( void );

public:
    GLUI_StdBitmaps  std_bitmaps;
    GLUI_String      window_name;
    RGBc             bkgd_color;
    float            bkgd_color_f[3];

    void            *font;
    int              curr_modifiers;

    void         adjust_glut_xy( int &x, int &y ) { y = h-y; }
    void         activate_control( GLUI_Control *control, int how );
    void         align_controls( GLUI_Control *control );
    void         restore_draw_buffer( int buffer_state );
    void         disactivate_current_control( void );
    void         draw_raised_box( int x, int y, int w, int h );
    void         draw_lowered_box( int x, int y, int w, int h );
    int          set_front_draw_buffer();
    void         post_update_main_gfx();
    void         pack_controls();
    void         close_internal();
    void         check_subwindow_position();
    void         set_ortho_projection();
    void         set_viewport();
    void         refresh();
    int          get_glut_window_id( void ) { return glut_window_id; } /* JVK */
};

/************************************************************/
/*                                                          */
/*       GLUI_Control: base class for all controls          */
/*                                                          */
/************************************************************/

class GLUI_Control : public GLUI_Node 
{
public:

    int             w, h;                        /* dimensions of control */
    int             x_abs, y_abs;
    int             x_off, y_off_top, y_off_bot; /* INNER margins, by which
                          child controls are indented */
    int             contain_x, contain_y; 
    int             contain_w, contain_h;
    /* if this is a container control (e.g., 
     radiogroup or panel) this indicated dimensions
     of inner area in which controls reside */
    int             active, active_type, can_activate;
    int             spacebar_mouse_click;
    long            user_id, type;
    int             is_container;  /* Is this a container class (e.g., panel) */
    int             alignment;
    GLUI_Update_CB  callback;
    void            *ptr_val;                              /* A pointer value */
    float           float_val;                               /* A float value */
    int             enabled;                   /* Is this control grayed out? */
    int             int_val;                              /* An integer value */
    float           float_array_val[GLUI_DEF_MAX_ARRAY];
    int             state;
    GLUI_String     name;                         /* The name of this control */
    GLUI_String     text;              
    GLUI           *glui;
    void           *font;
    int             live_type, live_inited;
    int             last_live_int;   /* last value that live var known to have */
    float           last_live_float;
    GLUI_String     last_live_text;
    float           last_live_float_array[GLUI_DEF_MAX_ARRAY];
    int             float_array_size;

    int             collapsible, is_open;
    GLUI_Node       collapsed_node;
    int             hidden; /* Collapsed controls (and children) are hidden */
    int             char_widths[CHAR_WIDTH_HASH_SIZE][2]; /* Character width hash table */
    /*** Get/Set values ***/

    virtual void   set_name( char *string );
    virtual void   set_int_val( int new_int )         { int_val = new_int; output_live(true); }
    virtual void   set_float_val( float new_float )   { float_val = new_float; output_live(true); }
    virtual void   set_ptr_val( void *new_ptr )       { ptr_val = new_ptr; output_live(true); }
    virtual void   set_float_array_val( float *array_ptr );

    virtual float  get_float_val( void )              { return float_val; }
    virtual int    get_int_val( void )                { return int_val; }
    virtual void   get_float_array_val( float *array_ptr );

    virtual int mouse_down_handler( int local_x, int local_y )                 { return false; }
    virtual int mouse_up_handler( int local_x, int local_y, int inside )       { return false; }
    virtual int mouse_held_down_handler( int local_x, int local_y, int inside) { return false; }
    virtual int key_handler( unsigned char key, int modifiers )                { return false; }
    virtual int special_handler( int key,int modifiers )                       { return false; }

    virtual void update_size( void )     { }
    virtual void idle( void )            { }
    virtual int  mouse_over( int state, int x, int y ) { return false; }

    virtual void enable( void ); 
    virtual void disable( void );
    virtual void activate( int how )     { active = true; }
    virtual void disactivate( void )     { active = false; }

    void         hide_internal( int recurse );
    void         unhide_internal( int recurse );

    int          can_draw( void ) { return (glui != NULL && hidden == false); }

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
    int          string_width( char *text );
    int          char_width( char c );

    void         draw_name( int x, int y );
    void         draw_box_inwards_outline( int x_min, int x_max, 
                     int y_min, int y_max );
    void         draw_box( int x_min, int x_max, int y_min, int y_max,
             float r, float g, float b );
    void         draw_bkgd_box( int x_min, int x_max, int y_min, int y_max );
    void         draw_emboss_box( int x_min, int x_max,int y_min,int y_max);
    void         draw_string( char *text );
    void         draw_char( char c );
    void         draw_active_box( int x_min, int x_max, int y_min, int y_max );
    void         set_to_bkgd_color( void );

    void         set_w( int new_w );
    void         set_h( int new_w );
    void         set_alignment( int new_align );
    void         sync_live( int recurse, int draw );  /* Reads live variable */
    void         init_live( void );
    void         output_live( int update_main_gfx );        /** Writes live variable **/
    virtual void set_text( char *t )   {}
    void         execute_callback( void );
    void         get_this_column_dims( int *col_x, int *col_y, 
                     int *col_w, int *col_h, 
                     int *col_x_off, int *col_y_off );
    virtual int  needs_idle( void );

    GLUI_Control(void) 
    {
        x_off          = GLUI_XOFF;
        y_off_top      = GLUI_YOFF;
        y_off_bot      = GLUI_YOFF;
        x_abs          = GLUI_XOFF;
        y_abs          = GLUI_YOFF;
        state          = 0;
        active         = false;
        enabled        = true;
        int_val        = 0;
        last_live_int  = 0;
        float_array_size = 0;
        sprintf( (char*)name, "Control: %p", (void *) this );
        float_val      = 0.0;
        last_live_float = 0.0;
        ptr_val        = NULL;
        glui           = NULL;
        w              = GLUI_DEFAULT_CONTROL_WIDTH;
        h              = GLUI_DEFAULT_CONTROL_HEIGHT;
        font           = NULL;
        active_type    = GLUI_CONTROL_ACTIVE_MOUSEDOWN;
        alignment      = GLUI_ALIGN_LEFT;
        is_container   = false;
        can_activate   = true;         /* By default, you can activate a control */
        spacebar_mouse_click = true;    /* Does spacebar simulate a mouse click? */
        live_type      = GLUI_LIVE_NONE;
        strcpy( (char*)text, "" );
        strcpy( (char*)last_live_text, "" );
        live_inited    = false;
        collapsible    = false;
        is_open        = true;
        hidden         = false;
        memset(char_widths, -1, CHAR_WIDTH_HASH_SIZE*2); /* JVK */
        int i;
        for( i=0; i<GLUI_DEF_MAX_ARRAY; i++ )
            float_array_val[i] = last_live_float_array[i] = 0.0;
    }

    virtual ~GLUI_Control();
};

/************************************************************/
/*                                                          */
/*               Button class (container)                   */
/*                                                          */
/************************************************************/

class GLUI_Button : public GLUI_Control
{
public:
    int currently_inside;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int same );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  key_handler( unsigned char key,int modifiers );

    void draw( int x, int y );
    void draw_pressed( void );
    void draw_unpressed( void );
    void draw_text( int sunken );

    void update_size( void );

    GLUI_Button( void ) 
    {
        sprintf( name, "Button: %p", (void *) this );
        type         = GLUI_CONTROL_BUTTON;
        h            = GLUI_BUTTON_SIZE;
        w            = 100;
        alignment    = GLUI_ALIGN_CENTER;
        can_activate = true;
    }

    virtual ~GLUI_Button() {}
};

/************************************************************/
/*                                                          */
/*               Checkbox class (container)                 */
/*                                                          */
/************************************************************/

class GLUI_Checkbox : public GLUI_Control
{
public:
    int  orig_value, currently_inside;
    int  text_x_offset;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int inside );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  key_handler( unsigned char key,int modifiers );

    void update_size( void );

    void draw( int x, int y );

    void draw_active_area( void );
    void draw_checked( void );
    void draw_unchecked( void );
    void draw_X( void );
    void draw_empty_box( void );
    void set_int_val( int new_val );

    GLUI_Checkbox() 
    {
        sprintf( name, "Checkbox: %p", (void *) this );
        type           = GLUI_CONTROL_CHECKBOX;
        w              = 100;
        h              = GLUI_CHECKBOX_SIZE;
        orig_value     = -1;
        text_x_offset  = 18;
        can_activate   = true;
        live_type      = GLUI_LIVE_INT;   /* This control has an 'int' live var */
    }
  
    virtual ~GLUI_Checkbox() {}
};

/************************************************************/
/*                                                          */
/*               Column class                               */
/*                                                          */
/************************************************************/

class GLUI_Column : public GLUI_Control
{
public:
    float red;    /* Color of the column line - JVK */
    float green;  /* Column colors are used bby the tree panel to */
    float blue;   /* clearly denote the hierarchy */ 
    void draw( int x, int y );

    GLUI_Column() 
    {
        red          = .5; /* Column Colors default to grey - JVK */
        green        = .5;
        blue         = .5;
        type         = GLUI_CONTROL_COLUMN;
        w            = 0;
        h            = 0;
        int_val      = 0;
        can_activate = false;
    }

    virtual ~GLUI_Column() {}
};

/************************************************************/
/*                                                          */
/*               Panel class (container)                    */
/*                                                          */
/************************************************************/

class GLUI_Panel : public GLUI_Control
{
public:
    GLUI_Panel() 
    {
        type         = GLUI_CONTROL_PANEL;
        w            = 300;
        h            = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
        int_val      = GLUI_PANEL_EMBOSSED;
        alignment    = GLUI_ALIGN_CENTER;
        is_container = true; 
        can_activate = false;
        strcpy( name, "" );
    }

    virtual ~GLUI_Panel() {}

    void draw( int x, int y );
    void set_name( char *text );
    void set_type( int new_type );

    void update_size( void );
};

/************************************************************/
/*                                                          */
/*               File Browser class (container)             */
/*                         JVK                              */
/************************************************************/

class GLUI_FileBrowser : public GLUI_Panel
{
 private:
  int last_item;
  char *file;
  int allow_change_dir;
 public:
  GLUI_List *list;
  GLUI_String current_dir;

  void fbreaddir(char *);
  static void dir_list_callback(void *, int);

  void set_w(int w);// { if (list) list->set_w(w) }
  void set_h(int h);// { if (list) list->set_h(h) }
  char* get_file() { return file; }
  void set_allow_change_dir(int c) { allow_change_dir = c; }

  GLUI_FileBrowser( void ) {
    type         = GLUI_CONTROL_FILEBROWSER;
    w            = GLUI_DEFAULT_CONTROL_WIDTH;
    h            = GLUI_DEFAULT_CONTROL_HEIGHT;
    int_val      = GLUI_PANEL_EMBOSSED;
    alignment    = GLUI_ALIGN_CENTER;
    is_container = true; 
    can_activate = false;
    allow_change_dir = true;
    last_item    = -1;
    user_id      = -1;
    strcpy( name, "" );
    strcpy( current_dir, "." );
    file = NULL;
  };

  virtual ~GLUI_FileBrowser() {};
};

/************************************************************/
/*                                                          */
/*               Rollout class (container)                  */
/*                                                          */
/************************************************************/

class GLUI_Rollout : public GLUI_Panel
{
  /*  private: */
  /*    GLUI_Panel panel; */
public:
    int          currently_inside, initially_inside;
    GLUI_Button  button;

    void draw( int x, int y );
    void draw_pressed( void );
    void draw_unpressed( void );
    int mouse_down_handler( int local_x, int local_y );
    int mouse_up_handler( int local_x, int local_y, int inside );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
        
    void  open( void ); 
    void  close( void );

    /*   void set_name( char *text )   { panel.set_name( text ); } */
    void update_size( void );

    GLUI_Rollout( void ) 
    {
        currently_inside = false;
        initially_inside = false;
        can_activate     = true;
        is_container     = true;
        type             = GLUI_CONTROL_ROLLOUT;
        h                = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
        w                = GLUI_DEFAULT_CONTROL_WIDTH;
        y_off_top        = 21;
        collapsible      = true;
        strcpy( name, "" );
    }
 
    virtual ~GLUI_Rollout() {}
};

/************************************************************/
/*                                                          */
/*               Tree    Panel class (container)            */
/*                         JVK                              */
/************************************************************/

class GLUI_Tree : public GLUI_Panel
{

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
  int          currently_inside, initially_inside;
  GLUI_Button  button;
  char         level_name[100]; // level name, eg: 1.1.2, III, or 3
  GLUI_TreePanel *panel; 

  void draw( int x, int y );
  void draw_pressed( void );
  void draw_unpressed( void );
  int mouse_down_handler( int local_x, int local_y );
  int mouse_up_handler( int local_x, int local_y, int inside );
  int  mouse_held_down_handler( int local_x, int local_y, int inside );
  void set_column(GLUI_Column *c) { column = c; }
  void  open( void ); 
  void  close( void );

    /*   void set_name( char *text )   { panel.set_name( text ); }; */
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
    if (column) {
      column->red = r;
      column->green = g;
      column->blue = b;
    }
  }

  GLUI_Tree( void ) {
    currently_inside = false;
    initially_inside = false;
    can_activate     = true;
    is_container     = true;
    type             = GLUI_CONTROL_TREE;
    h                = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
    w                = GLUI_DEFAULT_CONTROL_WIDTH;
    y_off_top        = 21;
    collapsible      = true;
    red              = .5;
    green            = .5;
    blue             = .5;
    lred              = 0;
    lgreen            = 0;
    lblue             = 0;
    column           = NULL;
    is_current       = 0;
    child_number     = 0;
    panel         = NULL;
    strcpy( name, "" );
    strcpy( level_name, "" );
    level            = 0;
    
  };
 
  virtual ~GLUI_Tree() {};
};


/************************************************************/
/*                                                          */
/*               TreePanel class (container) JVK            */
/*     Manages, maintains, and formats entire trees         */
/************************************************************/
class GLUI_TreePanel : public GLUI_Panel {
   int max_levels;
   int next_id;
   int format;
   float red;
   float green;
   float blue;
   float lred;
   float lgreen;
   float lblue;
   int root_children;
   /* These variables allow the tree panel to traverse the tree
      using only two function calls. (Well, four, if you count 
      going in reverse */

   GLUI_Tree    *curr_branch; /* Current Branch */
   GLUI_Panel *curr_root;   /* Current Root */

 protected:
   int uniqueID( void ) { next_id++; return next_id - 1; }
 public:
   void            set_color(float r, float g, float b) {
                                     red = r;
                                     green = g;
                                     blue = b;
                                    }
   void            set_level_color(float r, float g, float b) {
                                     lred = r;
                                     lgreen = g;
                                     lblue = b;
                                    }
   void            set_format(int f) { format = f; }

   /* Adds branch to curr_root */
   GLUI_Tree *     ab(char *name, GLUI_Tree *root = NULL);
   /* Goes up one level, resets curr_root and curr_branch to parents*/
   void            fb(GLUI_Tree *branch= NULL);
   /* Deletes the curr_branch, goes up one level using fb */
   void            db(GLUI_Tree *branch = NULL);
   /* Finds the very last branch of curr_root, resets vars */
   void            descendBranch(GLUI_Panel *root = NULL);
   /* Resets curr_root and curr branch to TreePanel and lastChild */
   void            resetToRoot(GLUI_Panel *new_root = NULL);
   void            next( void );
   void            refresh( void );
   void            expand_all( void );
   void            collapse_all( void );
   void            update_all( void );
   void            initNode(GLUI_Tree *temp);
   void            formatNode(GLUI_Tree *temp);

   GLUI_TreePanel() {
     GLUI_Panel();
     type = GLUI_CONTROL_TREEPANEL;
     next_id = 0;
     curr_root = this;
     curr_branch = NULL;
     red = .5;
     green = .5;
     blue = .5;
     root_children = 0;
   }

   ~GLUI_TreePanel() {
   }

};

/************************************************************/
/*                                                          */
/*                     User-Level GLUI class                */
/*                                                          */
/************************************************************/

class GLUI : public GLUI_Main 
{
private:
public:
   /* Add Column returns a pointer so the tree can later change
      the column color - JVK */
    GLUI_Column *  add_column( int draw_bar = true );
    GLUI_Column *  add_column_to_panel( GLUI_Panel *panel, int draw_bar = true );

    void  add_separator( void );
    void  add_separator_to_panel( GLUI_Panel *panel );

    GLUI_RadioGroup 
    *add_radiogroup( int *live_var=NULL,
             int user_id=-1,GLUI_Update_CB callback=NULL);

    GLUI_RadioGroup 
    *add_radiogroup_to_panel(  GLUI_Panel *panel,
                   int *live_var=NULL,
                   int user_id=-1, GLUI_Update_CB callback=NULL );
    GLUI_RadioButton
    *add_radiobutton_to_group(  GLUI_RadioGroup *group,
                char *name );

    GLUI_Listbox *add_listbox( char *name, int *live_var=NULL,
                 int id=-1, GLUI_Update_CB callback=NULL    );
    GLUI_Listbox *add_listbox_to_panel( GLUI_Panel *panel,
                      char *name, int *live_var=NULL,
                      int id=-1, GLUI_Update_CB callback=NULL);

    GLUI_Rotation *add_rotation( char *name, float *live_var=NULL,
                   int id=-1, GLUI_Update_CB callback=NULL  );
    GLUI_Rotation *add_rotation_to_panel( GLUI_Panel *panel,
                    char *name, float *live_var=NULL,
                    int id=-1, GLUI_Update_CB callback=NULL);

    GLUI_Translation *add_translation( char *name,
                     int trans_type, float *live_var=NULL,
                     int id=-1, GLUI_Update_CB callback=NULL    );
    GLUI_Translation *add_translation_to_panel( 
                         GLUI_Panel *panel, char *name, 
                         int trans_type, float *live_var=NULL,
                         int id=-1, GLUI_Update_CB callback=NULL);

    GLUI_Checkbox  *add_checkbox( char *name, 
                int *live_var=NULL,
                int id=-1, GLUI_Update_CB callback=NULL);
    GLUI_Checkbox  *add_checkbox_to_panel( GLUI_Panel *panel, char *name, 
                     int *live_var=NULL, int id=-1, 
                     GLUI_Update_CB callback=NULL);

    GLUI_Button  *add_button( char *name, int id=-1, 
                GLUI_Update_CB callback=NULL);
    GLUI_Button  *add_button_to_panel( GLUI_Panel *panel, char *name, 
                     int id=-1, GLUI_Update_CB callback=NULL );

    GLUI_StaticText  *add_statictext( char *name );
    GLUI_StaticText  *add_statictext_to_panel( GLUI_Panel *panel, char *name );

    GLUI_EditText  *add_edittext( char *name, 
                int data_type=GLUI_EDITTEXT_TEXT,
                void *live_var=NULL,
                int id=-1, GLUI_Update_CB callback=NULL );
    GLUI_EditText  *add_edittext_to_panel( GLUI_Panel *panel, 
                     char *name,
                     int data_type=GLUI_EDITTEXT_TEXT,
                     void *live_var=NULL, int id=-1, 
                     GLUI_Update_CB callback=NULL );

    /* GLUI Textbox - JVK */
    GLUI_TextBox  *add_textbox(char *live_var=NULL, int scroll = false,
		     int id=-1, GLUI_Update_CB callback=NULL );
    GLUI_TextBox  *add_textbox_to_panel( GLUI_Panel *panel, 
		     char *live_var=NULL, int scroll = false, int id=-1, 
		     GLUI_Update_CB callback=NULL );

    /* GLUI List - JVK */
    GLUI_List  *add_list(char *live_var=NULL, int scroll = false,
		         int id=-1, GLUI_Update_CB callback=NULL,
		         GLUI_Control *object = NULL, 
		         GLUI_InterObject_CB obj_cb = NULL);
    GLUI_List  *add_list_to_panel( GLUI_Panel *panel, 
				   char *live_var=NULL, int scroll = false, 
				   int id=-1, 
				   GLUI_Update_CB callback=NULL, 
				   GLUI_Control *object = NULL, 
				   GLUI_InterObject_CB obj_cb = NULL);

    GLUI_Spinner  *add_spinner( char *name, 
                  int data_type=GLUI_SPINNER_INT,
                  void *live_var=NULL,
                  int id=-1, GLUI_Update_CB callback=NULL );
    GLUI_Spinner  *add_spinner_to_panel( GLUI_Panel *panel, 
                       char *name,
                       int data_type=GLUI_SPINNER_INT,
                       void *live_var=NULL,
                       int id=-1,
                       GLUI_Update_CB callback=NULL );

    /* GLUI Scrollbar - JVK (Very Similar to Spinner) */
    GLUI_Scrollbar  *add_scrollbar( char *name, 
            int horz_vert=GLUI_SCROLL_HORIZONTAL,
				    int data_type=GLUI_SCROLL_INT,
				    void *live_var=NULL,
				    int id=-1, GLUI_Update_CB callback=NULL , 
				    GLUI_Control *object = NULL,
				    GLUI_InterObject_CB obj_cb = NULL);
    GLUI_Scrollbar  *add_scrollbar_to_panel( GLUI_Panel *panel, 
					     char *name,
               int horz_vert=GLUI_SCROLL_HORIZONTAL,
					     int data_type=GLUI_SCROLL_INT,
					     void *live_var=NULL,
					     int id=-1,
					     GLUI_Update_CB callback=NULL,
					     GLUI_Control *object = NULL,
					     GLUI_InterObject_CB obj_cb = NULL);

    GLUI_Panel     *add_panel( char *name, int type=GLUI_PANEL_EMBOSSED );
    GLUI_Panel     *add_panel_to_panel( GLUI_Panel *panel, char *name, 
                      int type=GLUI_PANEL_EMBOSSED );

    GLUI_FileBrowser  *add_filebrowser( char *name, 
				        int type=GLUI_PANEL_EMBOSSED, 
				        int user_id = -1, 
				        GLUI_Update_CB callback = NULL);
    GLUI_FileBrowser  *add_filebrowser_to_panel( GLUI_Panel *panel, 
					         char *name, 
					         int type=GLUI_PANEL_EMBOSSED, 
					         int user_id = -1, 
					         GLUI_Update_CB callback = NULL);

    GLUI_Tree      *add_tree( char *name, int open=false, int inset=0);
    GLUI_Tree      *add_tree_to_panel( GLUI_Panel *panel, char *name, 
                        	       int open=false, int inset=0);

    GLUI_TreePanel      *add_treepanel( char *name, int open=false, int inset=0);
    GLUI_TreePanel      *add_treepanel_to_panel( GLUI_Panel *panel, char *name, 
				                 int open=false, int inset=0);

    GLUI_Rollout   *add_rollout( char *name, int open=true, int type=GLUI_PANEL_EMBOSSED);
    GLUI_Rollout   *add_rollout_to_panel( GLUI_Panel *panel, char *name, 
					  int open=true, 
					  int type=GLUI_PANEL_EMBOSSED);

    void            set_main_gfx_window( int window_id );
    int             get_glut_window_id( void ) { return glut_window_id; }

    void            enable( void ) { main_panel->enable(); }
    void            disable( void );

    void            sync_live( void );

    void            close( void );

    void            show( void );
    void            hide( void );

    /***** GLUT callback setup functions *****/
    /*
    void set_glutDisplayFunc(void (*f)(void));
    void set_glutReshapeFunc(void (*f)(int width, int height));
    void set_glutKeyboardFunc(void (*f)(unsigned char key, int x, int y));
    void set_glutSpecialFunc(void (*f)(int key, int x, int y));
    void set_glutMouseFunc(void (*f)(int button, int state, int x, int y));
    void set_glutMotionFunc(void (*f)(int x, int y));
    void set_glutPassiveMotionFunc(void (*f)(int x, int y));
    void set_glutEntryFunc(void (*f)(int state));
    void set_glutVisibilityFunc(void (*f)(int state));
    void set_glutInit( int *argcp, char **argv );
    void set_glutInitWindowSize(int width, int height);
    void set_glutInitWindowPosition(int x, int y);
    void set_glutInitDisplayMode(unsigned int mode);
    int  set_glutCreateWindow(char *name);
    */

    /***** Constructors and desctructors *****/

    int init( char *name, long flags, int x, int y, int parent_window );
};

/************************************************************/
/*                                                          */
/*               EditText class                             */
/*                                                          */
/************************************************************/

class GLUI_EditText : public GLUI_Control
{
public:
    int                 has_limits;
    int                 data_type;
    GLUI_String         orig_text;
    int                 insertion_pt;
    int                 title_x_offset;
    int                 text_x_offset;
    int                 substring_start; /*substring that gets displayed in box*/
    int                 substring_end;  
    int                 sel_start, sel_end;  /* current selection */
    int                 num_periods;
    int                 last_insertion_pt;
    float               float_low, float_high;
    int                 int_low, int_high;
    GLUI_Spinner       *spinner;
    int                 debug;
    int                 draw_text_only;


    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int same );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler(   int key,int modifiers );

    void activate( int how );
    void disactivate( void );

    void draw( int x, int y );

    int  mouse_over( int state, int x, int y );

    int  find_word_break( int start, int direction );
    int  substring_width( int start, int end );
    void clear_substring( int start, int end );
    int  find_insertion_pt( int x, int y );
    int  update_substring_bounds( void );
    void update_and_draw_text( void );
    void draw_text( int x, int y );
    void draw_insertion_pt( void );
    void set_numeric_text( void );
    void update_x_offsets( void );
    void update_size( void );

    void set_float_limits( float low,float high,int limit_type=GLUI_LIMIT_CLAMP);
    void set_int_limits( int low, int high, int limit_type=GLUI_LIMIT_CLAMP );
    void set_float_val( float new_val );
    void set_int_val( int new_val );
    void set_text( char *text );
    char *get_text( void )         { return text; }  

    void dump( FILE *out, char *text );

    GLUI_EditText( void ) 
    {
        type                  = GLUI_CONTROL_EDITTEXT;
        h                     = GLUI_EDITTEXT_HEIGHT;
        w                     = GLUI_EDITTEXT_WIDTH;
        title_x_offset        = 0;
        text_x_offset         = 55;
        insertion_pt          = -1;
        last_insertion_pt     = -1;
        name[0]               = '\0';
        substring_start       = 0;
        data_type             = GLUI_EDITTEXT_TEXT;
        substring_end         = 2;
        num_periods           = 0;
        has_limits            = GLUI_LIMIT_NONE;
        sel_start             = 0;
        sel_end               = 0;
        active_type           = GLUI_CONTROL_ACTIVE_PERMANENT;
        can_activate          = true;
        spacebar_mouse_click  = false;
        spinner               = NULL;
        debug                 = false;
        draw_text_only        = false;
    }
};

/************************************************************/
/*                                                          */
/*              RadioGroup class (container)                */
/*                                                          */
/************************************************************/

class GLUI_RadioGroup : public GLUI_Control
{
public:
    int  num_buttons;

    void draw( int x, int y );
    void set_name( char *text );
    void set_int_val( int int_val ); 
    void set_selected( int int_val );

    void draw_group( int translate );

    GLUI_RadioGroup( void ) 
    {
        type          = GLUI_CONTROL_RADIOGROUP;
        x_off         = 0;
        y_off_top     = 0;
        y_off_bot     = 0;
        is_container  = true;
        w             = 300;
        h             = 300;
        num_buttons   = 0;
        name[0]       = '\0';
        can_activate  = false;
        live_type     = GLUI_LIVE_INT;
    }
};

/************************************************************/
/*                                                          */
/*               RadioButton class (container)              */
/*                                                          */
/************************************************************/

class GLUI_RadioButton : public GLUI_Control
{
public:
    int orig_value, currently_inside;
    int text_x_offset;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int inside );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );

    void draw( int x, int y );
    void update_size( void );

    void draw_active_area( void );
    void draw_checked( void );
    void draw_unchecked( void );
    void draw_O( void );

    GLUI_RadioGroup *group;

    GLUI_RadioButton( void ) 
    {
        sprintf( name, "RadioButton: %p", (void *) this );
        type           = GLUI_CONTROL_RADIOBUTTON;
        h              = GLUI_RADIOBUTTON_SIZE;
        group          = NULL;
        orig_value     = -1;
        text_x_offset  = 18;
        can_activate   = true;
    }
};


/************************************************************/
/*                                                          */
/*               Separator class (container)                */
/*                                                          */
/************************************************************/

class GLUI_Separator : public GLUI_Control
{
public:
    void draw( int x, int y );

    GLUI_Separator() 
    {
        type         = GLUI_CONTROL_SEPARATOR;
        w            = 100;
        h            = GLUI_SEPARATOR_HEIGHT;
        can_activate = false;
    }
};

#define  GLUI_SPINNER_ARROW_WIDTH   12
#define  GLUI_SPINNER_ARROW_HEIGHT   8
#define  GLUI_SPINNER_ARROW_Y        2

#define  GLUI_SPINNER_STATE_NONE     0
#define  GLUI_SPINNER_STATE_UP       1
#define  GLUI_SPINNER_STATE_DOWN     2
#define  GLUI_SPINNER_STATE_BOTH     3

#define  GLUI_SPINNER_DEFAULT_GROWTH_EXP   1.05f

/************************************************************/
/*                                                          */
/*               Spinner class (container)                  */
/*                                                          */
/************************************************************/
 
class GLUI_Spinner : public GLUI_Control
{
public:
    GLUI_Spinner() 
    {
        sprintf( name, "Spinner: %p", (void *) this );
        type         = GLUI_CONTROL_SPINNER;
        h            = GLUI_EDITTEXT_HEIGHT;
        w            = GLUI_EDITTEXT_WIDTH;
        x_off        = 0;
        y_off_top    = 0;
        y_off_bot    = 0;
        can_activate = true;
        state        = GLUI_SPINNER_STATE_NONE;
        edittext     = NULL;
        growth_exp   = GLUI_SPINNER_DEFAULT_GROWTH_EXP;
        callback_count = 0;
        first_callback = true;
        user_speed   = 1.0;
    }

    ~GLUI_Spinner()
    {
    }

    int           currently_inside;
    int           state;
    float         growth, growth_exp;
    int           last_x, last_y;
    int           data_type;
    int           callback_count;
    int           last_int_val;
    float         last_float_val;
    int           first_callback;
    float         user_speed;

    GLUI_EditText *edittext;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int same );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler(   int key,int modifiers );

    void draw( int x, int y );
    void draw_pressed( void );
    void draw_unpressed( void );
    void draw_text( int sunken );

    void update_size( void );

    void set_float_limits( float low,float high,int limit_type=GLUI_LIMIT_CLAMP);
    void set_int_limits( int low, int high,int limit_type=GLUI_LIMIT_CLAMP);
    int  find_arrow( int local_x, int local_y );
    void do_drag( int x, int y );
    void do_callbacks( void );
    void draw_arrows( void );
    void do_click( void );
    void idle( void );
    int  needs_idle( void );

    char *get_text( void );

    void set_float_val( float new_val );
    void set_int_val( int new_val );
    float  get_float_val( void );
    int    get_int_val( void );
    void increase_growth( void );
    void reset_growth( void );

    void set_speed( float speed ) { user_speed = speed; }
};

/************************************************************/

/*                                                          */
/*               StaticText class                           */
/*                                                          */
/************************************************************/

class GLUI_StaticText : public GLUI_Control
{
public:
    void set_text( char *text );
    void draw( int x, int y );
    void draw_text( void );
    void update_size( void );
    void erase_text( void );

    GLUI_StaticText() 
    {
        type    = GLUI_CONTROL_STATICTEXT;
        h       = GLUI_STATICTEXT_SIZE;
        name[0] = '\0';
        can_activate  = false;
    }
};

/************************************************************/
/*                                                          */
/*               TextBox class - JVK                        */
/*                                                          */
/************************************************************/

class GLUI_TextBox : public GLUI_Control
{
public:
  GLUI_String         orig_text;
  int                 insertion_pt;
  int                 substring_start; /*substring that gets displayed in box*/
  int                 substring_end;  
  int                 sel_start, sel_end;  /* current selection */
  int                 last_insertion_pt;
  int                 debug;
  int                 draw_text_only;
  int                 tab_width;
  int                 start_line;
  int                 num_lines;
  int                 curr_line;
  int                 visible_lines;
  int                 insert_x;        /* Similar to "insertion_pt", these variables keep */
  int                 insert_y;        /* track of where the ptr is, but in pixels */
  GLUI_Scrollbar     *scrollbar;

  int  mouse_down_handler( int local_x, int local_y );
  int  mouse_up_handler( int local_x, int local_y, int same );
  int  mouse_held_down_handler( int local_x, int local_y, int inside );
  int  key_handler( unsigned char key,int modifiers );
  int  special_handler( int key,int modifiers );
  
  void activate( int how );
  void disactivate( void );

  void enable( void );
  void disable( void );

  void draw( int x, int y );

  int  mouse_over( int state, int x, int y );

  int get_box_width();
  int  find_word_break( int start, int direction );
  int  substring_width( int start, int end );
  void clear_substring( int start, int end );
  int  find_insertion_pt( int x, int y );
  int  update_substring_bounds( void );
  void update_and_draw_text( void );
  void draw_text( int x, int y );
  void draw_insertion_pt( void );
  void update_x_offsets( void );
  void update_size( void );

  void set_text( char *text );
  char *get_text( void )         { return text; };  

  void dump( FILE *out, char *text );
  void set_tab_w(int w) { tab_width = w; }
  void set_start_line(int l) { start_line = l; }
  static void scrollbar_callback(void *, int id);

  GLUI_TextBox( void ) {
    type                  = GLUI_CONTROL_TEXTBOX;
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
    debug                 = false;
    draw_text_only        = false;
  };
};

/************************************************************/
/*                                                          */
/*                   Listbox class - JVK                    */
/*                                                          */
/************************************************************/

class GLUI_List_Item : public GLUI_Node 
{
public:
  GLUI_String text;
  int         id;
};

/************************************************************/
/*                                                          */
/*               List class - JVK                           */
/*                                                          */
/************************************************************/

class GLUI_List : public GLUI_Control
{
public:
  GLUI_String         orig_text;
  int                 debug;
  int                 draw_text_only;
  int                 start_line;
  int                 num_lines;
  int                 curr_line;
  int                 visible_lines;
  GLUI_Scrollbar      *scrollbar;
  GLUI_List_Item      items_list;
  GLUI_Control        *associated_object;
  GLUI_InterObject_CB obj_cb;
  int                 cb_click_type;
  int                 last_line;
  int                 last_click_time;

  int  mouse_down_handler( int local_x, int local_y );
  int  mouse_up_handler( int local_x, int local_y, int same );
  int  mouse_held_down_handler( int local_x, int local_y, int inside );
  int  key_handler( unsigned char key,int modifiers );
  int  special_handler( int key,int modifiers );
  
  void activate( int how );
  void disactivate( void );

  void draw( int x, int y );

  int  mouse_over( int state, int x, int y );

  int get_box_width();
  int  find_word_break( int start, int direction );
  int  substring_width( char *t, int start, int end );
  int  find_line( int x, int y );
  void update_and_draw_text( void );
  void draw_text( char *t, int selected, int x, int y );
  void update_size( void );


  int  add_item( int id, char *text );
  int  delete_item( char *text );
  int  delete_item( int id );
  int  delete_all();

  GLUI_List_Item *get_item_ptr( char *text );
  GLUI_List_Item *get_item_ptr( int id );

  void dump( FILE *out, char *text );
  void set_start_line(int l) { start_line = l; }
  static void scrollbar_callback(void *, int id);
  int get_current_item() { return curr_line; }
  void set_click_type(int d) {
    cb_click_type = d; }
  
  GLUI_List( void ) {
    type                  = GLUI_CONTROL_LIST;
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
    debug                 = false;
    draw_text_only        = false;
    cb_click_type         = GLUI_SINGLE_CLICK;
    last_line             = -1;
    last_click_time       = 0;
	 obj_cb                = NULL;
	 associated_object     = NULL;
  };
};

/************************************************************/
/*                                                          */
/*               Scrollbar class - JVK                      */
/*                                                          */
/************************************************************/
 
class GLUI_Scrollbar : public GLUI_Control
{
public:
  int           currently_inside;
  int           state;
  float         growth, growth_exp;
  int           last_x, last_y;
  int           data_type;
  int           callback_count;
  int           last_int_val;
  float         last_float_val;
  int           first_callback;
  float         user_speed;
  float         float_min, float_max;
  int           int_min, int_max;
  int           horizontal;
  int box_length;
  int box_start_position;
  int box_end_position;
  int track_length;


  /* Rather than directly access an Editbox or Textbox for 
     changing variables, a pointer to some object is defined
     along with a static callback in the form func(void *, int) -
     the int is the new value, the void * must be cast to that
     particular object type before use.
  */
  void *        associated_object; /* Let's the Spinner manage it's own callbacks */
  void (*object_cb)(void *,int); /* function pointer to object call_back */

  int  mouse_down_handler( int local_x, int local_y );
  int  mouse_up_handler( int local_x, int local_y, int same );
  int  mouse_held_down_handler( int local_x, int local_y, int inside );
  int  key_handler( unsigned char key,int modifiers );
  int  special_handler( int key,int modifiers );
  
  void draw( int x, int y );
  void draw_pressed( void );
  void draw_unpressed( void );
  void draw_text( int sunken );

  void update_size( void );

  void set_int_limits( int low, int high,int limit_type=GLUI_LIMIT_CLAMP);
  void set_float_limits( float low,float high,int limit_type=GLUI_LIMIT_CLAMP);
  int  find_arrow( int local_x, int local_y );
  void do_drag( int x, int y );
  void do_callbacks( void );
  void draw_arrows( void );
  void draw_scroll( void );
  void do_click( void );
  void idle( void );
  int  needs_idle( void );
  void set_int_val( int new_val );
  void set_float_val( float new_val );
  void increase_growth( void );
  void reset_growth( void );

  void set_speed( float speed ) { user_speed = speed; };
  void update_scroll_parameters();

  GLUI_Scrollbar ( void ) {
    type         = GLUI_CONTROL_SCROLL;
    horizontal   = true;
    h            = GLUI_SCROLL_ARROW_HEIGHT;
    w            = GLUI_TEXTBOX_WIDTH;
    x_off        = 0;
    y_off_top    = 0;
    y_off_bot    = 0;
    can_activate = true;
    state        = GLUI_SCROLL_STATE_NONE;
    growth_exp   = GLUI_SCROLL_DEFAULT_GROWTH_EXP;
    callback_count = 0;
    first_callback = true;
    user_speed   = 1.0;
    int_val      = 0;
    int_min      = 0;
    int_max      = 0;
    associated_object = NULL;
    object_cb = NULL;
    box_length         = 0;
    box_start_position = 0;
    box_end_position   = 0;
    track_length       = 0;

  };
protected:
  virtual void draw_scroll_arrow(int arrowtype, int x, int y);
  virtual void draw_scroll_box(int x, int y, int w, int h);
  
};


/************************************************************/
/*                                                          */
/*                   Listbox class                          */
/*                                                          */
/************************************************************/

class GLUI_Listbox_Item : public GLUI_Node 
{
public:
    GLUI_String text;
    int         id;
};

class GLUI_Listbox : public GLUI_Control
{
public:
    GLUI_String       curr_text;
    GLUI_Listbox_Item items_list;
    int               depressed;

    int  orig_value, currently_inside;
    int  text_x_offset, title_x_offset;
    int  glut_menu_id;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int inside );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler(   int key,int modifiers );

    void update_size( void );
    void draw( int x, int y );
    int  mouse_over( int state, int x, int y );

    void draw_active_area( void );
    void set_int_val( int new_val );
    void dump( FILE *output );

    int  add_item( int id, char *text );
    int  delete_item( char *text );
    int  delete_item( int id );
    int  sort_items( void );

    int  do_selection( int item );

    void increase_width( void );

    GLUI_Listbox_Item *get_item_ptr( char *text );
    GLUI_Listbox_Item *get_item_ptr( int id );
  
    GLUI_Listbox() 
    {
        sprintf( name, "Listbox: %p", (void *) this );
        type           = GLUI_CONTROL_LISTBOX;
        w              = GLUI_EDITTEXT_WIDTH;
        h              = GLUI_EDITTEXT_HEIGHT;
        orig_value     = -1;
        title_x_offset = 0;
        text_x_offset  = 55;
        can_activate   = true;
        curr_text[0]   = '\0';
        live_type      = GLUI_LIVE_INT;  /* This has an integer live var */
        depressed      = false;
        glut_menu_id   = -1;
    }

    ~GLUI_Listbox();
};

/************************************************************/
/*                                                          */
/*              Mouse_Interaction class                     */
/*                                                          */
/************************************************************/

class GLUI_Mouse_Interaction : public GLUI_Control
{
public:
    /*int  get_main_area_size( void ) { return MIN( h-18,  */
    int            draw_active_area_only;

    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, int inside );
    int  mouse_held_down_handler( int local_x, int local_y, int inside );
    int  special_handler( int key, int modifiers );
    void update_size( void );
    void draw( int x, int y );
    void draw_active_area( void );

    /***  The following methods (starting with "iaction_") need to
    be overloaded  ***/
    virtual int  iaction_mouse_down_handler( int local_x, int local_y ) = 0;
    virtual int  iaction_mouse_up_handler( int local_x, int local_y, int inside )=0;
    virtual int  iaction_mouse_held_down_handler( int local_x, int local_y, int inside )=0;
    virtual int  iaction_special_handler( int key, int modifiers )=0;
    virtual void iaction_draw_active_area_persp( void )=0;
    virtual void iaction_draw_active_area_ortho( void )=0;
    virtual void iaction_dump( FILE *output )=0;
    virtual void iaction_init( void ) = 0;
  
    GLUI_Mouse_Interaction() 
    {
        sprintf( name, "Mouse_Interaction: %p", (void *) this );
        type           = GLUI_CONTROL_MOUSE_INTERACTION;
        w              = GLUI_MOUSE_INTERACTION_WIDTH;
        h              = GLUI_MOUSE_INTERACTION_HEIGHT;
        can_activate   = true;
        live_type      = GLUI_LIVE_NONE;
        alignment      = GLUI_ALIGN_CENTER;
        draw_active_area_only = false;
    }
};

/************************************************************/
/*                                                          */
/*                   Rotation class                         */
/*                                                          */
/************************************************************/

class GLUI_Rotation : public GLUI_Mouse_Interaction
{
public:
    Arcball        *ball;
    GLUquadricObj *quadObj;
    int            can_spin, spinning;
    float          damping;
  
    int  iaction_mouse_down_handler( int local_x, int local_y );
    int  iaction_mouse_up_handler( int local_x, int local_y, int inside );
    int  iaction_mouse_held_down_handler( int local_x, int local_y, int inside );
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

    int  needs_idle( void );
    void idle( void );

    void copy_float_array_to_ball( void );
    void copy_ball_to_float_array( void );

    void set_spin( float damp_factor );

    GLUI_Rotation(void);
};

/************************************************************/
/*                                                          */
/*                   Translation class                      */
/*                                                          */
/************************************************************/

class GLUI_Translation : public GLUI_Mouse_Interaction
{
public:
    int trans_type;  /* Is this an XY or a Z controller? */
    int down_x, down_y;
    float scale_factor;
    GLUquadricObj *quadObj;
    int   trans_mouse_code;
    float orig_x, orig_y, orig_z;
    int   locked;

    int  iaction_mouse_down_handler( int local_x, int local_y );
    int  iaction_mouse_up_handler( int local_x, int local_y, int inside );
    int  iaction_mouse_held_down_handler( int local_x, int local_y, int inside );
    int  iaction_special_handler( int key, int modifiers );
    void iaction_init( void ) { }
    void iaction_draw_active_area_persp( void );
    void iaction_draw_active_area_ortho( void );
    void iaction_dump( FILE *output );

    void set_speed( float s ) { scale_factor = s; }

    void setup_texture( void );
    void setup_lights( void );
    void draw_2d_arrow( int radius, int filled, int orientation ); 
    void draw_2d_x_arrows( int radius );
    void draw_2d_y_arrows( int radius );
    void draw_2d_z_arrows( int radius );
    void draw_2d_xy_arrows( int radius );

    int  get_mouse_code( int x, int y );

    /* Float array is either a single float (for single-axis controls),
     or two floats for X and Y (if an XY controller) */

    float get_z( void ) {       return float_array_val[0];  }
    float get_x( void ) {       return float_array_val[0];  }
    float get_y( void ) {
    if ( trans_type == GLUI_TRANSLATION_XY )    return float_array_val[1];
    else                    return float_array_val[0];
    }

    void  set_z( float val );
    void  set_x( float val );
    void  set_y( float val );
    void  set_one_val( float val, int index );

    GLUI_Translation( void ) 
    {
        locked              = GLUI_TRANSLATION_LOCK_NONE;
        sprintf( name, "Translation: %p", (void *) this );
        type                = GLUI_CONTROL_TRANSLATION;
        w                   = GLUI_MOUSE_INTERACTION_WIDTH;
        h                   = GLUI_MOUSE_INTERACTION_HEIGHT;
        can_activate        = true;
        live_type           = GLUI_LIVE_FLOAT_ARRAY;
        float_array_size    = 0;
        alignment           = GLUI_ALIGN_CENTER;
        trans_type          = GLUI_TRANSLATION_XY;
        scale_factor        = 1.0;
        quadObj             = NULL;
        trans_mouse_code    = GLUI_TRANSLATION_MOUSE_NONE;
    }
};

/********** Misc functions *********************/
int _glutBitmapWidthString( void *font, char *s );
void _glutBitmapString( void *font, char *s );

/********** Our own callbacks for glut *********/
/* These are the callbacks that we pass to glut.  They take
   some action if necessary, then (possibly) call the user-level
   glut callbacks.  
   */

void glui_display_func( void );
void glui_reshape_func( int w, int h );
void glui_keyboard_func(unsigned char key, int x, int y);
void glui_special_func(int key, int x, int y);
void glui_mouse_func(int button, int state, int x, int y);
void glui_motion_func(int x, int y);
void glui_passive_motion_func(int x, int y);
void glui_entry_func(int state);
void glui_visibility_func(int state);
void glui_idle_func(void);

void glui_parent_window_reshape_func( int w, int h );
void glui_parent_window_keyboard_func(unsigned char key, int x, int y);
void glui_parent_window_mouse_func(int, int, int, int );
void glui_parent_window_special_func(int key, int x, int y);

#endif
