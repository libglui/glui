/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui.h - Main (and only) external header for 
        GLUI User Interface Toolkit (1998)

     glui.hpp - Main (and only) external header for 
        GLUI Library User Interface (2019)

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


  **** APPENDIX (2019) ****
  
  ---- C preprocessing ----
    
  GLUI_BUILD_EXAMPLES
  Build glui_example.cpp. Use glui_example to launch demo. F1~F6 to change.

  GLUI_READLINE_MODIFIERS 
  Restore Unix's Readline key bindings. If you don't know what that is you 
  don't want it.

  GLUI_DEBUG_STDOUT
  Restore (dump) debug output feature. Node::_dump_FILE is its FILE output.

*****************************************************************************/

#ifndef GLUI_GLUI_H
#define GLUI_GLUI_H

//https://github.com/libglui/glui/pull/76 REMOVES THESE BUT THIS WILL BREAK
//CODE THAT DEPENDED ON THEM
#if defined(GLUT_API_VERSION) 

	// Don't reinclude GLUT.

#elif defined(GLUI_FREEGLUT)

	// FreeGLUT does not yet work perfectly with UI
	//  - use at your own risk.   
	#include <GL/freeglut.h>

#elif defined(GLUI_OPENGLUT)

	// OpenGLUT does not yet work properly with UI
	//  - use at your own risk.
	#include <GL/openglut.h>

#else 

	// Having stdlib here first fixes some 'exit() redefined' errors on MSVC.NET
	// that come from old GLUT headers.
	#include <cstdlib>
	#ifdef __APPLE__
	#include <GLUT/glut.h>
	#else
	#include <GL/glut.h>
	#endif

#endif

#include "glui.hpp" //"glui.hpp"

const int
/********* Constants for window placement **********/
GLUI_XOFF                      = GLUI_Library::UI_XOFF,
GLUI_YOFF                      = GLUI_Library::UI_YOFF,
GLUI_ITEMSPACING               = GLUI_Library::UI_ITEMSPACING,
GLUI_CHECKBOX_SIZE             = GLUI_Library::UI_DEFAULT_HEIGHT,
GLUI_RADIOBUTTON_SIZE          = GLUI_Library::UI_DEFAULT_HEIGHT,
GLUI_BUTTON_SIZE               = GLUI_Library::UI_BUTTON_HEIGHT,
GLUI_STATICTEXT_SIZE           = GLUI_Library::UI_DEFAULT_HEIGHT,
GLUI_SEPARATOR_HEIGHT          = GLUI_Library::UI_SEPARATOR_HEIGHT,
GLUI_DEFAULT_CONTROL_WIDTH     = GLUI_Library::UI_DEFAULT_WIDTH,
GLUI_DEFAULT_CONTROL_HEIGHT    = GLUI_Library::UI_DEFAULT_HEIGHT,
GLUI_EDITTEXT_BOXINNERMARGINX  = GLUI_Library::UI_BOXINNERMARGINX,
GLUI_EDITTEXT_HEIGHT           = GLUI_Library::UI_EDITTEXT_HEIGHT,
GLUI_EDITTEXT_WIDTH            = GLUI_Library::UI_EDITTEXT_WIDTH,
GLUI_EDITTEXT_MIN_INT_WIDTH    = GLUI_Library::UI_EDITTEXT_MIN_INT_WIDTH,
GLUI_EDITTEXT_MIN_TEXT_WIDTH   = GLUI_Library::UI_EDITTEXT_MIN_TEXT_WIDTH,
GLUI_PANEL_NAME_DROP           = GLUI_Library::UI_BOX_NAME_DROP,
GLUI_PANEL_EMBOSS_TOP          = GLUI_Library::UI_PANEL_EMBOSS_TOP,
GLUI_ROTATION_WIDTH            = GLUI_Library::UI_MOUSE_IACTION_WIDTH,
GLUI_ROTATION_HEIGHT           = (GLUI_ROTATION_WIDTH+18),
GLUI_MOUSE_INTERACTION_WIDTH   = GLUI_Library::UI_MOUSE_IACTION_WIDTH,
GLUI_MOUSE_INTERACTION_HEIGHT  = (GLUI_MOUSE_INTERACTION_WIDTH+18),
/** Different panel control types **/
GLUI_PANEL_NONE      = GLUI_Library::UI::Panel::NONE,
GLUI_PANEL_EMBOSSED  = GLUI_Library::UI::Panel::ETCHED,
GLUI_PANEL_RAISED    = GLUI_Library::UI::Panel::RAISED,
/********* The control's 'active' behavior *********/
//GLUI_CONTROL_ACTIVE_MOUSEDOWN  = GLUI_Library::UI::Control::ACTIVE_MOUSEDOWN,
//GLUI_CONTROL_ACTIVE_PERMANENT  = GLUI_Library::UI::Control::ACTIVE_PERMANENT,
/********* Control alignment types **********/
GLUI_ALIGN_CENTER   = GLUI_Library::UI::Control::ALIGN_CENTER,
GLUI_ALIGN_RIGHT    = GLUI_Library::UI::Control::ALIGN_RIGHT,
GLUI_ALIGN_LEFT     = GLUI_Library::UI::Control::ALIGN_LEFT,
/********** Limit types - how to limit spinner values *********/
GLUI_LIMIT_NONE    = GLUI_Library::UI::EditText::LIMIT_NONE,
GLUI_LIMIT_CLAMP   = GLUI_Library::UI::EditText::LIMIT_CLAMP,
GLUI_LIMIT_WRAP    = GLUI_Library::UI::EditText::LIMIT_WRAP,
/********** Translation control types ********************/
GLUI_TRANSLATION_XY = GLUI_Library::UI::Translation::XY,
GLUI_TRANSLATION_Z  = GLUI_Library::UI::Translation::Z,
GLUI_TRANSLATION_X  = GLUI_Library::UI::Translation::X,
GLUI_TRANSLATION_Y  = GLUI_Library::UI::Translation::Y,
//GLUI_TRANSLATION_LOCK_NONE = GLUI_Library::UI::Translation::LOCK_NONE,
//GLUI_TRANSLATION_LOCK_X    = GLUI_Library::UI::Translation::LOCK_X,
//GLUI_TRANSLATION_LOCK_Y    = GLUI_Library::UI::Translation::LOCK_Y,
/********** How was a control activated? *****************/
GLUI_ACTIVATE_MOUSE = GLUI_Library::UI::ACTIVATE_MOUSE,
GLUI_ACTIVATE_TAB   = GLUI_Library::UI::ACTIVATE_TAB,
/********** What type of live variable does a control have? **********/
GLUI_LIVE_NONE          = GLUI_Library::UI_LIVE_NONE,
GLUI_LIVE_INT           = GLUI_Library::UI_LIVE_INT,
GLUI_LIVE_FLOAT         = GLUI_Library::UI_LIVE_FLOAT,
GLUI_LIVE_TEXT          = GLUI_Library::UI_LIVE_TEXT,
GLUI_LIVE_STRING        = GLUI_Library::UI_LIVE_STRING,
GLUI_LIVE_DOUBLE        = GLUI_Library::UI_LIVE_DOUBLE,
GLUI_LIVE_FLOAT_ARRAY   = GLUI_Library::UI_LIVE_FLOAT_ARRAY,
GLUI_LIVE_DOUBLE_ARRAY  = GLUI_Library::UI_LIVE_DOUBLE_ARRAY,
/************* Textbox and List Defaults - JVK ******************/
GLUI_TEXTBOX_HEIGHT          = GLUI_Library::UI_TEXTBOX_HEIGHT,
GLUI_TEXTBOX_WIDTH           = GLUI_Library::UI_TEXTBOX_WIDTH,
GLUI_LIST_HEIGHT             = GLUI_Library::UI_LIST_HEIGHT,
GLUI_LIST_WIDTH              = GLUI_Library::UI_LIST_HEIGHT,
GLUI_DOUBLE_CLICK            = GLUI_Library::UI::DOUBLE_CLICKED,
GLUI_SINGLE_CLICK            = GLUI_Library::UI::SINGLE_CLICKED,
//GLUI_TAB_WIDTH               = GLUI_Library::UI_TAB_WIDTH, /* In pixels */
GLUI_TEXTBOX_BOXINNERMARGINX = GLUI_Library::UI_BOXINNERMARGINX,
GLUI_TEXTBOX_MIN_TEXT_WIDTH  = GLUI_Library::UI_TEXTBOX_MIN_TEXT_WIDTH,
GLUI_LIST_BOXINNERMARGINX    = GLUI_Library::UI_BOXINNERMARGINX,
GLUI_LIST_MIN_TEXT_WIDTH     = GLUI_Library::UI_LIST_MIN_TEXT_WIDTH,
/*********************** TreePanel Defaults - JVK *****************************/
GLUI_TREEPANEL_DEFAULTS             = 0, // bar, standard bar color
GLUI_TREEPANEL_ALTERNATE_COLOR      = 1, // Alternate between 8 different bar colors
GLUI_TREEPANEL_ENABLE_BAR           = 2, // enable the bar
GLUI_TREEPANEL_DISABLE_BAR          = 4, // disable the bar
GLUI_TREEPANEL_DISABLE_DEEPEST_BAR  = 8, // disable only the deepest bar
GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY= 16, // disable only the bar of the last child of each root
GLUI_TREEPANEL_DISPLAY_HIERARCHY    = 32, // display some sort of hierachy in the tree node title
GLUI_TREEPANEL_HIERARCHY_NUMERICDOT = 64, // display hierarchy in 1.3.2 (etc... ) format
GLUI_TREEPANEL_HIERARCHY_LEVEL_ONLY = 128, // display hierarchy as only the level depth
 
/******************* GLUI ScrollBar Defaults - JVK ***************************/
 GLUI_SCROLL_ARROW_WIDTH     = GLUI_Library::UI_SCROLL_ARROW_SIZE,
 GLUI_SCROLL_ARROW_HEIGHT    = GLUI_Library::UI_SCROLL_ARROW_SIZE,
 //GLUI_SCROLL_BOX_MIN_HEIGHT  = GLUI_Library::UI_SCROLL_BOX_MIN_HEIGHT,
 GLUI_SCROLL_BOX_STD_HEIGHT  = GLUI_Library::UI_SCROLL_BOX_STD_HEIGHT,
 GLUI_SCROLL_STATE_NONE      = GLUI_Library::Spin_Interface::STATE_NONE,
 GLUI_SCROLL_STATE_UP        = GLUI_Library::Spin_Interface::STATE_UP,
 GLUI_SCROLL_STATE_DOWN      = GLUI_Library::Spin_Interface::STATE_DOWN,
 GLUI_SCROLL_STATE_BOTH      = GLUI_Library::Spin_Interface::STATE_BOTH,
 GLUI_SCROLL_STATE_SCROLL    = GLUI_Library::Spin_Interface::STATE_SCROLL,
 GLUI_SCROLL_VERTICAL        = GLUI_Library::Spin_Interface::VERTICAL,
 GLUI_SCROLL_HORIZONTAL      = GLUI_Library::Spin_Interface::HORIZONTAL,
#define GLUI_SCROLL_DEFAULT_GROWTH_EXP GLUI_SPIN_DEFAULT_GROWTH_EXP
GLUI_SPINNER_ARROW_WIDTH   = GLUI_Library::UI_SPINNER_ARROW_WIDTH,
GLUI_SPINNER_ARROW_HEIGHT  = GLUI_Library::UI_SPINNER_ARROW_HEIGHT,
GLUI_SPINNER_ARROW_Y       = GLUI_Library::UI_SPINNER_ARROW_Y,
GLUI_SPINNER_STATE_NONE    = GLUI_Library::Spin_Interface::STATE_NONE,
GLUI_SPINNER_STATE_UP      = GLUI_Library::Spin_Interface::STATE_UP,
GLUI_SPINNER_STATE_DOWN    = GLUI_Library::Spin_Interface::STATE_DOWN,
GLUI_SPINNER_STATE_BOTH    = GLUI_Library::Spin_Interface::STATE_BOTH;
#define GLUI_SPINNER_DEFAULT_GROWTH_EXP GLUI_SPIN_DEFAULT_GROWTH_EXP

/************ A string type for us to use **********/

typedef GLUI_Library::String GLUI_String;
using GLUI_Library::glui_format_str;
using GLUI_Library::glui_sort_node;

/********* Pre-declare classes as needed *********/

typedef GLUI_Library::Bitmap GLUI_Bitmap;
typedef GLUI_Library::Texture GLUI_Texture;
typedef GLUI_Library::StdBitmaps GLUI_StdBitmaps;
typedef GLUI_Library::Node GLUI_Node;
typedef GLUI_Library::UI GLUI_Main, GLUI;
typedef GLUI_Library::UI::CB GLUI_CB;
typedef GLUI_Library::UI::Control GLUI_Control;
typedef GLUI_Library::UI::Button GLUI_Button;
typedef GLUI_Library::UI::CheckBox GLUI_Checkbox;
typedef GLUI_Library::UI::Column GLUI_Column;
typedef GLUI_Library::UI::Panel GLUI_Panel;
typedef GLUI_Library::UI::FileBrowser GLUI_FileBrowser;
typedef GLUI_Library::UI::Rollout GLUI_Rollout;	
typedef GLUI_Library::UI::EditText GLUI_EditText;
typedef GLUI_Library::UI::CommandLine GLUI_CommandLine;
typedef GLUI_Library::UI::RadioGroup GLUI_RadioGroup;
typedef GLUI_Library::UI::RadioButton GLUI_RadioButton;
typedef GLUI_Library::UI::Separator GLUI_Separator;
typedef GLUI_Library::UI::Spinner GLUI_Spinner;
typedef GLUI_Library::UI::StaticText GLUI_StaticText;
typedef GLUI_Library::UI::TextBox GLUI_TextBox;
typedef GLUI_Library::UI::ScrollBar GLUI_Scrollbar;
typedef GLUI_Library::List_Interface::Item GLUI_List_Item;
typedef GLUI_Library::UI::List GLUI_List;
typedef GLUI_Library::UI::ListBox GLUI_Listbox;
typedef GLUI_Library::Mouse_Interface GLUI_Mouse_Interaction;
typedef GLUI_Library::UI::Rotation GLUI_Rotation;
typedef GLUI_Library::UI::Translation GLUI_Translation;
typedef GLUI_Library::UI::Experiments::Tree GLUI_Tree;
typedef GLUI_Library::UI::Experiments::TreePanel GLUI_TreePanel;

typedef GLUI_Library::Arcball Arcball;

/*** Flags for GLUI class constructor ***/
#define GLUI_SUBWINDOW          (1<<1)
#define GLUI_SUBWINDOW_TOP      (1<<2)
#define GLUI_SUBWINDOW_BOTTOM   (1<<3)
#define GLUI_SUBWINDOW_LEFT     (1<<4)
#define GLUI_SUBWINDOW_RIGHT    (1<<5)

/*** Codes for different type of edittext boxes and spinners ***/
#define GLUI_EDITTEXT_TEXT             GLUI_Library::UI_EDIT_TEXT
#define GLUI_EDITTEXT_INT              GLUI_Library::UI_EDIT_INT
#define GLUI_EDITTEXT_FLOAT            GLUI_Library::UI_EDIT_FLOAT
#define GLUI_SPINNER_INT               GLUI_EDITTEXT_INT
#define GLUI_SPINNER_FLOAT             GLUI_EDITTEXT_FLOAT
#define GLUI_SCROLL_INT                GLUI_EDITTEXT_INT
#define GLUI_SCROLL_FLOAT              GLUI_EDITTEXT_FLOAT
// This is only for deprecated interface
#define GLUI_EDITTEXT_STRING           GLUI_Library::UI_EDIT_STRING
#define GLUI_EDITTEXT_DOUBLE           GLUI_Library::UI_EDIT_DOUBLE

/*** Definition of (some) callbacks ***/
typedef void(*GLUI_Void_CB)();
typedef void(*GLUI_Update_CB)(int id);
typedef void(*GLUI_Control_CB)(GLUI_Control*);
typedef void(*Int1_CB)(int);
typedef void(*Int2_CB)(int,int);
typedef void(*Int3_CB)(int,int,int);
typedef void(*Int4_CB)(int,int,int,int);			  

/************************************************************/
/*                                                          */
/*                     Master UI Class                      */
/*                                                          */
/************************************************************/

/**
 The master manages our interaction with GLUI.
 There's only one GLUI.
*/
typedef class GLUI_Library::GLUT GLUI_Master_Object;
/**
 This is the only GLUI_Master in existence
*/
static GLUI_Master_Object &GLUI_Master = GLUI_Library::GLUT;

/************************************************************/
/*                                                          */
/*              Class for managing a GLUT window            */
/*                                                          */
/************************************************************/

/**
 A top-level window.  The GLUI_Master GLUT callback can route events
 to the callbacks in this class, for arbitrary use by external users.
 (see GLUI_Master_Object::set_glutKeyboardFunc).
 
 This entire approach seems to be superceded by the "subwindow" flavor
 of GLUI.
*/
typedef GLUI_Library::GLUT::Window GLUI_Glut_Window;
   
/************************************************************/
/*                                                          */
/*                   Copy/Paste class                       */
/*                                                          */
/************************************************************/

typedef class GLUI_Library::Clipboard GLUI_Clipboard_Object;

/**
 * These are built-in clipboards. On non X11-like systems
 * @c UI_Selection is a "no-op" object.
 */
static GLUI_Clipboard_Object &GLUI_Clipboard = GLUI_Library::Clipboard, &GLUI_Selection = GLUI_Library::Selection;

//DEPRECATED API
#ifndef GLUI_NODEPRECATED 
namespace GLUI_Library 
{//-.
//<-'

inline void UI::post_update_main_gfx()
{
	//NOTE: Nvidia GLUT fires this off before returning
	//unless it's set to a 100ms timer. output_callback
	//is echoed by execute_callback to flush the render.
	GLUT::cdecl_glutPostWindowRedisplay(_glut_escape_id);
}
static void glui_update_main_gfx(UI::Control *c, bool)
{
	c->ui->post_update_main_gfx();
}	
inline void UI::set_main_gfx_window(int window_id)
{
	//Note: "true" informs execute_callback to wrap the 
	//call in glutGetWindow/glutSetWindow/glutSetWindow  
	//This behavior is not recommended; for back-compat.

	set_main_esc_window(window_id,true);

	output_callback = glui_update_main_gfx;
}

/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_add_controls.cpp - Routines for adding controls to a GLUI window

Note: these routines are all deprecated.  Keeping them all here
prevents the linker from dragging in all the .o files, even for controls
that aren't used.

*****************************************************************************/
 	
inline UI *GLUI::create_glui(C_String name, int flags, int x, int y)
{	 
	//_flags is SUBWINDOW only. This API cannot be used with a
	//client derived class, and so cannot possible require the
	//protected members. (Underscores on the member names will
	//not work if these members are accessed to be repurposed)
	assert(flags==0); 

	//"new" should use the shared library's C++ Runtime module
	//in order to be strictly compatible with existing clients
	//If it were otherwise the header could implement this API
	return new UI(name,x,y,flags);
}
inline UI *GLUI::create_glui_subwindow(int parent_window, int flags)
{
	//"new" should use the shared library's C++ Runtime module
	//in order to be strictly compatible with existing clients
	//If it were otherwise the header could implement this API
	return new UI(parent_window,flags);
}
inline bool UI::add_control(Control *control)
{
	control->set_parent(this); return true; 
}
inline UI::CheckBox *UI::add_checkbox(const char *name, int *value_ptr, int id, CB callback)
{
	return add_checkbox_to_panel(_main_panel,name,value_ptr,id,callback);
}
inline UI::CheckBox *UI::add_checkbox_to_panel(Panel *panel, const char *name, int *value_ptr, int id, CB callback)
{
	return new CheckBox(panel,name,value_ptr,id,callback);
}
inline UI::Panel *UI::add_panel(const char *name, int type)
{
	return add_panel_to_panel(_main_panel,name,type);
}
inline UI::Panel *UI::add_panel_to_panel(Panel *parent_panel, const char *name, int type)
{
	return new Panel(parent_panel,name,type);
}
inline UI::RadioGroup *UI::add_radiogroup(int *value_ptr, int user_id, CB callback)
{
	return add_radiogroup_to_panel(_main_panel,value_ptr, user_id,callback);
}
inline UI::RadioGroup *UI::add_radiogroup_to_panel(Panel *panel, int *value_ptr, int user_id, CB callback)
{
	return new RadioGroup(panel,value_ptr,user_id,callback);
}
inline UI::RadioButton *UI::add_radiobutton_to_group(RadioGroup *group, const char *name)
{
	return new RadioButton(group,name);
}
inline UI::StaticText *UI::add_statictext(const char *name)
{
	return add_statictext_to_panel(_main_panel,name);
}
inline UI::StaticText *UI::add_statictext_to_panel(Panel *panel, const char *name)
{
	return new StaticText(panel,name);
}
inline UI::Button *UI::add_button(const char *name, int id, UI::CB callback)
{
	return add_button_to_panel(_main_panel,name,id,callback);
}
inline UI::Button *UI::add_button_to_panel(Panel *panel, const char *name, int id, CB callback)
{
	return new Button(panel,name,id,callback);
}
inline void UI::add_separator()
{
	add_separator_to_panel(_main_panel); 
}
inline void UI::add_separator_to_panel(Panel *panel)
{
	new Separator(panel); 
}
inline UI::EditText *UI::add_edittext(const char *name, int data_type, void *data, int id, CB callback)
{
	return add_edittext_to_panel(_main_panel,name,data_type,data,id,callback);
}
inline UI::EditText *UI::add_edittext_to_panel(Panel *panel, const char *name, int data_type, void *data, int id, CB callback)
{
	return new EditText(panel,name,data_type,data,id,callback);
}
inline UI::EditText *UI::add_edittext(const char *name, String &data, int id, CB callback)
{
	return add_edittext_to_panel(_main_panel,name,UI_EDIT_STRING,&data,id,callback);
}
inline UI::EditText *UI::add_edittext_to_panel(Panel *panel, const char *name, String &data, int id, CB callback)
{
	return new EditText(panel,name,&data,id,callback);
}
inline UI::Spinner *UI::add_spinner(const char *name, int data_type, void *data, int id, CB callback)
{
	return add_spinner_to_panel(_main_panel,name,data_type,data,id,callback);
}
inline UI::Spinner *UI::add_spinner_to_panel(Panel *panel, const char *name, int data_type, void *data, int id, CB callback)
{
	return new Spinner(panel,name,data_type,data,id,callback);
}
inline void UI::add_column(bool draw_bar)
{
	add_column_to_panel(_main_panel,draw_bar);
}
inline void UI::add_column_to_panel(Panel *panel, bool draw_bar)
{
	new Column(panel,draw_bar);
}
inline UI::ListBox *UI::add_listbox(const char *name, int *value_ptr, int id, CB callback)
{
	return add_listbox_to_panel(_main_panel,name,value_ptr,id,callback);
}
inline UI::ListBox *UI::add_listbox_to_panel(Panel *panel, const char *name, int *value_ptr, int id, CB callback)
{
	return new ListBox(panel,name,value_ptr,id,callback);
}
inline UI::Rotation *UI::add_rotation(const char *name, float *value_ptr, int id, CB callback)
{
	return add_rotation_to_panel(_main_panel,name,value_ptr,id,callback);
}
inline UI::Rotation *UI::add_rotation_to_panel(Panel *panel, const char *name, float *value_ptr, int id, CB callback)
{
	return new Rotation(panel,name,value_ptr,id,callback);
}
inline UI::Translation *UI::add_translation(const char *name, int trans_type, float *value_ptr, int id, CB callback)
{
	return add_translation_to_panel(_main_panel,name,trans_type,value_ptr,id,callback);
}
inline UI::Translation *UI::add_translation_to_panel(Panel *panel, const char *name, int trans_type, float *value_ptr, int id, CB callback)
{
	return new Translation(panel,name,trans_type,value_ptr,id,callback);
}
inline UI::Rollout *UI::add_rollout(const char *name, bool open, int type)
{
	return add_rollout_to_panel(_main_panel,name,open,type);
}
inline UI::Rollout *UI::add_rollout_to_panel(Panel *panel, const char *name, bool open, int type)
{
	return new Rollout(panel,name,open,type);
}

//---.
}//<-'
#endif //GLUI_NODEPRECATED

#endif //GLUI_GLUI_H
/*C1071*/
