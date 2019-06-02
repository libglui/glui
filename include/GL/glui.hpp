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

#ifndef GLUI_GLUI_HPP
#define GLUI_GLUI_HPP

#include <cstdio>
#include <cstring>
#include <cassert>
#include <climits> //INT_MAX
#include <cfloat> //DBL_MAX
#include <string>
#include <algorithm>

/********** Current version **********/

#define GLUI_VERSION 3.00f //2.36f  

namespace GLUI_Library
{//-.
//<-'

#if defined(GLUI_GLUI_H) && !defined(GLUI_NODEPRECATED)
#define GLUI_GLUI_H___NODEPRECATED
enum{ GLUI_H = 1 };
#else
enum{ GLUI_H = 0 };
#endif
#ifdef GLUI_INTERNAL_H
enum{ INTERNAL_H = 1 };
#else
enum{ INTERNAL_H = 0 };
#endif

/* GLUI API shared library export/import declarations. */
#ifdef GLUI_DLL
#ifdef __GNUC__
#ifdef GLUI_INTERNAL_H
#define LINKAGE __attribute__((__dllexport__))
#else
#define LINKAGE __attribute__((__dllimport__))
#endif
#else
#ifdef GLUI_INTERNAL_H
#define LINKAGE __declspec(dllexport) //GLUIAPI
#else
#define LINKAGE __declspec(dllimport) //GLUIAPI
#endif
#endif
#else
#ifdef __GNUC__
#define LINKAGE __attribute__((__visibility__("default")))
#else
#define LINKAGE //GLUIAPI
#endif
#endif

#ifdef GLUI_BUILD_EXAMPLES 
/* If argc is nonzero glutInit and glutMainLoop is done. */
LINKAGE int glui_example(int n, int argc=0, char*argv[]=0);
#endif

/********* Constants for window placement **********/

const int /* enum must write std::max<int> in full */
UI_XOFF                    =   6,
UI_YOFF                    =   6,
UI_ITEMSPACING             =   3, //Rollout is expecting 3.
UI_DEFAULT_WIDTH           = 100,
UI_DEFAULT_HEIGHT          =  15,
UI_BUTTON_HEIGHT           =  25, //Odd squares focus rect.
UI_SEPARATOR_HEIGHT        =   8,
UI_ALIGN_EXPAND_WIDTH      =   8, //Can't support a column.
UI_BOXINNERMARGINX         =   3,
UI_EDITTEXT_HEIGHT         =  21, //20
UI_EDITTEXT_WIDTH          = 130,
UI_EDITTEXT_MIN_INT_WIDTH  =  35,
UI_EDITTEXT_MIN_TEXT_WIDTH =  50,
UI_BOX_NAME_DROP           =   6, //8
UI_PANEL_EMBOSS_TOP        =   2, //4
//UI_TREE_HEADER           =  21, //Proposal for Tree's 18?
UI_MOUSE_IACTION_HEADER    =  21, //Might use Font::height?
UI_MOUSE_IACTION_WIDTH     =  50,
UI_MOUSE_IACTION_HEIGHT    =  50+21, //RADIUS+HEADER
UI_SPINNER_ARROW_WIDTH     =  12,
UI_SPINNER_ARROW_HEIGHT    =   8,
UI_SPINNER_ARROW_Y         =   2, //2019: border of EditText
				   				
/************* Textbox and List Defaults - JVK ******************/
UI_TEXTBOX_HEIGHT          = 130,
UI_TEXTBOX_WIDTH           = 130,
UI_LIST_HEIGHT             = 130,
UI_LIST_WIDTH              = 130,
//UI_TAB_WIDTH               =  50, /* In pixels */
UI_TEXTBOX_MIN_TEXT_WIDTH  =  50,
UI_LIST_MIN_TEXT_WIDTH     =  50,

/******************* UI ScrollBar Defaults - JVK ***************************/
UI_SCROLL_ARROW_SIZE       =  17, //Odd centers arrow tips.
//UI_SCROLL_BOX_MIN_HEIGHT   =   5, //UNSUSED
UI_SCROLL_BOX_STD_HEIGHT   =  17; //Odd squares focus rect.

enum
{
/********** What type of live variable does a control have? **********/
UI_LIVE_NONE        = 0,
UI_LIVE_INT         = 1,
UI_LIVE_FLOAT       = 2,
UI_LIVE_TEXT        = 3,
UI_LIVE_STRING      = 6,
UI_LIVE_DOUBLE      = 4,
UI_LIVE_FLOAT_ARRAY = 3+UI_LIVE_FLOAT, //5
UI_LIVE_DOUBLE_ARRAY= 3+UI_LIVE_DOUBLE, //7

/*** Codes for different type of edittext boxes and spinners ***/
UI_EDIT_TEXT        = 1,
UI_EDIT_INT         = 2,
UI_EDIT_FLOAT       = 3,
// This is only for deprecated interface
UI_EDIT_STRING      = 4,
UI_EDIT_DOUBLE      = 5, //2019

UI_SPIN_GROWTH_STEPS     = 800,
UI_SPIN_MIN_GROWTH_STEPS = 100,
#define GLUI_SPIN_DEFAULT_GROWTH_EXP 1.05
};
	
/************ A string type for us to use **********/

//WORK POSTPONED
//#ifndef _DEBUG
//#error This is bad: https://github.com/libglui/glui/issues/80
//#endif
typedef std::string String;
LINKAGE String &glui_format_str(String &str, const char *fmt,...);

/************************************************************/
/*                                                          */
/*                 GLUI_String wrapper                      */
/*                                                          */
/************************************************************/

/* An experiment to alleviate std::string c_str requirement */

typedef int C_Bool; // Pointers convert to C++ bool too easily.

class C_String
{
public:

	const char *str;
	
	//NOTE: These are designed to not pick up on too many types
	//so the constructors are not too easily rendered ambiguous.
													 
	C_String():str(NULL)
	{}	
	C_String(const char *str):str(str)
	{}
	C_String(const C_String &cp):str(cp.str)
	{}
	template<class T, int N>
	/* NOTE: const char(&nul)[1] is ambiguous. */
	C_String(T (&str)[N]):str(N==1?NULL:(char*)str)
	{
		int compile[sizeof(T)==1]; (void)compile;
	}	
	template<class A, class B, class C>
	C_String(const std::basic_string<A,B,C> &str):str((char*)str.c_str())
	{
		int compile[sizeof(A)==1]; (void)compile;
	}
	template<class T>
	C_String(const T &a, typename T::value_type b=0, typename T::size_type c=0)
	{
		str = (char*)&a[0]; int compile[sizeof(b)==1]; (void)compile;
	}

	template<class T, int N>
	/** I don't understand why this is required, but = weakly
	  converts to a pointer more readily than the () syntax. */
	C_String &operator=(T (&cp)[N])
	{ 
		//return *new(this) C_String(cp); //Doesn't work for MSVC2010.
		str = N==1?NULL:(char*)cp; return *this;
		int compile[sizeof(T)==1]; (void)compile;
	}
};

/************************************************************/
/*                                                          */
/*                  Basic utitlity class                    */
/*                                                          */
/************************************************************/

	//2019 addition
	//This class originated to expose the Font functionality
	//but it can be helpful in some other ways too.

class Window
{	
public: //Interface methods
	
	virtual ~Window(){} /* dynamic_cast (set_font) */

	Window(){} //C2512

private:

	class Font
	{
	public:
		
		int height; 		
		void *font;	 		
		const char(*width)[256];

		Font(int h=0, void *f=0, void *w=0) //GCC
		{
			height = h; font = f; (void*&)width = w;
		}

	public: //Compatibility layer
		
		inline operator void*()const{ return font; }

		inline const Font &operator=(void *font)const
		{	
			Window *p = (Window*)this;
			(char*&)p-=(char*)&p->font-(char*)p; 

			p->set_font(font); return *this;
		}
		inline const Font &operator=(const Font &cp)const
		{
			return *(Font*)
			memcpy(const_cast<Font*>(this),&cp,sizeof(cp));
		}		
	};
	static /*const*/ Font _glut_fonts[7];
	
public:

	/** Properties of our control */    

	const Font font; /**< Our glutbitmap font */

public:

	/** Historically set_font isn't recursive. */
	LINKAGE void set_font(void *new_font=NULL, bool recurse=false);
	
	inline void *get_font(){ return font.font; }
    
	/** char_width is not currently set up to use a dummy 
	where the font is unset. set_parent or set_font do so. */
	inline int char_width(char c)
	{
		assert(font.width);
		return (*font.width)[(unsigned char)c&0xFF]; 
	}	
	inline int string_width(C_String str)
	{
		const char *p = str.str;
		int width = 0; while(*p) width+=char_width(*p++);
		return width;
	}	
	inline int string_width(C_String t, int start, int end)
	{
		int width = 0;
		for(int i=start;i</*=*/end;i++) width+=char_width(t.str[i]);
		return width;
	}

public: //User draw methods

	LINKAGE static void set_ortho_projection(int w, int h);
	LINKAGE static void translate_to_origin(double x, double y, double z=0);

	/** Enables clipping text for draw_string strings. 
	WARNING: This is a wrapper around glRasterPos/glBitmap and so it must
	be called again whenever the glColor is changed.
	OpenGL is pretty messed in that regard and that glRasterPos cannot do
	clipping without going through glBitmap as backdoor. */
	inline void begin_to_draw_string(int x=0, int y=0){ draw_string(x,y,""); }

	inline void draw_char(int glutBitmapCharacter) 
	{
		char s[2] = {(char)(unsigned char)glutBitmapCharacter}; 
		draw_string(s);
	}	
	LINKAGE void draw_string(C_String glutBitmapString);
	/** @param y inverts OpenGL's convention. */
	LINKAGE void draw_string(int x, int y, C_String begin_to_draw_string);
	
	enum /** Different panel control types **/
	{		
		NONE=0, ETCHED, RAISED, BOX, ROLLOUT,

		BOX_ACTIVE, BOX_CLEAR, BOX_WHITE=BOX, 
	};

	//In order to standardize these parameters I've removed the old APIs.
	/** ff draws a 2px border; fff 3px; etc. 1 is top, 2 right, 4 bottom, 8 left. */
	LINKAGE static void draw_border_rect(int x1, int y1, int x2, int y2, int edge_mask=0xF);
	LINKAGE static void draw_raised_rect(int x1, int y1, int x2, int y2, int shadow=2|4);
	LINKAGE static void draw_sunken_rect(int x1, int y1, int x2, int y2);
	LINKAGE static void draw_framed_rect(int x1, int y1, int x2, int y2, int neg_disables=BOX_CLEAR);	
	LINKAGE static void draw_filled_rect(int x1, int y1, int x2, int y2, int neg_disables=BOX_WHITE);	
	LINKAGE static void draw_etched_rect(int x1, int y1, int x2, int y2); /*draw_emboss_box*/
	LINKAGE static void draw_etched_bars(int x1, int y1, int x2, int y2, int bars_mask_neg_disables);
	LINKAGE static void draw_active_rect(int x1, int y1, int x2, int y2);
	LINKAGE void draw_active_name(int x1, int y1, C_String name, int active_neg_1_disabled=0);

	//SKETCHY
	static inline void _rgb255(unsigned int *c, float r, float g, float b)
	{
		_rgb255(*(unsigned char(*)[4])c,r,g,b);
	}
	static inline void _rgb255(unsigned char (&c)[4], float r, float g, float b)
	{
		c[0] = (unsigned char)(r*255); c[1] = (unsigned char)(g*255); 
		c[2] = (unsigned char)(b*255); c[3] = 255;
	}			
};

/************************************************************/
/*                                                          */
/*                  Class GLUI_Bitmap                       */
/*                                                          */
/************************************************************/

/**
 GLUI_Bitmap is a simple 2D texture map.  It's used
 to represent small textures like checkboxes, arrows, etc.
 via the GLUI_StdBitmaps class.
*/
class Bitmap
{
public:

	unsigned char width, height, lumens[1]; 

public:

	LINKAGE static int _num_items();	

	LINKAGE void draw(int x=0, int y=0)const;

	/********************************************************/
	/*                                                      */
	/*              Standard Bitmap stuff                   */
	/*                                                      */
	/********************************************************/

	enum 
	{
	CHECKBOX_OFF=0,
	CHECKBOX_ON,
	RADIOBUTTON_OFF,
	RADIOBUTTON_ON,
	UP_ARROW,
	DOWN_ARROW,
	LEFT_ARROW,
	RIGHT_ARROW,
	SPINNER_UP_OFF,
	SPINNER_UP_ON,
	SPINNER_DOWN_OFF,
	SPINNER_DOWN_ON,
	CHECKBOX_OFF_DIS,    /*** Disabled control bitmaps ***/
	CHECKBOX_ON_DIS,
	RADIOBUTTON_OFF_DIS,
	RADIOBUTTON_ON_DIS,
	SPINNER_UP_DIS,
	SPINNER_DOWN_DIS,
	LISTBOX_UP,
	LISTBOX_DOWN,
	LISTBOX_UP_DIS,
	_NUM_ITEMS
	};
};

/************************************************************/
/*                                                          */
/*                  Class GLUI_StdBitmap                    */
/*                                                          */
/************************************************************/

/**
 Keeps an array of GLUI_Bitmap objects to represent all the 
 images used in the UI: checkboxes, arrows, etc.
*/
LINKAGE const Bitmap *std_bitmaps[/*Bitmap::_NUM_ITEMS*/];

struct Silver3ubv //back-compat 
{
	operator const unsigned char*()
	{
		return std_bitmaps[Bitmap::RADIOBUTTON_OFF]->lumens;
	}
};
static float Silver3[3] = {}; /** TRANSLATION-UNIT STORAGE */
struct Silver3fv //back-compat 
{
	operator const float*()
	{
		Silver3[0] = Silver3[1] = Silver3[2] = 
		*std_bitmaps[Bitmap::RADIOBUTTON_OFF]->lumens/255.0f;
		return Silver3;
	}
};

/************************************************************/
/*                                                          */
/*            Building-Block classes (abstract)             */
/*                                                          */
/************************************************************/

typedef class Box_Interface BI;
typedef class Text_Interface TI;
typedef class Spin_Interface SI;
typedef class List_Interface LI;
typedef class Group_Interface GI;
typedef class Mouse_Interface MI;

/************************************************************/
/*                                                          */
/*          Node class, for hierarchical relationships      */
/*                                                          */
/************************************************************/

namespace Meta /*workarounds*/
{
	struct Spinner;
	struct ScrollBar;
}

template<class T> class NodePtr
{
	class Node *_union; 

public: 
	
	inline T *operator->()const
	{
		assert(*this); return (T*)_union; 
	}

	inline operator typename T*()const
	{
		if(sizeof(T)!=sizeof(typename T::Sub))
		return dynamic_cast<typename T*>(_union);
		return (T*)dynamic_cast<typename T::Sub*>(_union);
	}
};
/**
 Node is a node in a sort of tree of UI controls.
 Each Node has a list of siblings (in a circular list)
 and a linked list of children.
 
 Everything onscreen is a Node--windows, buttons, etc.
 The nodes are traversed for event processing, sizing, redraws, etc.
*/
class Node 
{	
public:
	
	friend class UI;

	union
	{
		Node *_child_head;		
		NodePtr<Meta::ScrollBar> scrollbar;
	};	
	union
	{
		Node *_child_tail;
		NodePtr<Meta::Spinner> spinner;
		NodePtr<Meta::ScrollBar> scrollbar2;
	};		
	union
	{
		Node *_parent_node;
		NodePtr<Group_Interface> group;
	};
	Node *_next_sibling,*_prev_sibling;

public:

	inline Node()
	{
		memset(&_child_head,0,5*sizeof(void*)); 
	}
	virtual ~Node()
	{
		if(_parent_node) _unlink();

		for(Node*q,*p=_child_head;p;)
		{
			q = p; p = p->next();			
			q->_parent_node = NULL; q->_delete();
		}
	}
	/** Uses delete matching new */
	virtual void _delete(){ delete this; }
	
	inline Node *parent(){ return _parent_node; }
	inline Node *first_child(){ return _child_head; }
	inline Node *last_child(){ return _child_tail; }
	inline Node *next(){ return _next_sibling; }
	inline Node *prev(){ return _prev_sibling; }	
	
	// pred is a a less-than function however it may also
	// double as a node initialization function; therefor
	// is always called. The inputs may be the same child.
	LINKAGE void _sort(bool(*pred)(Node*,Node*), int depth);

public: 

	// The Interface template uses these.
	typedef Node Sub; typedef void Super; 

	// I would prefer to not advertise these so not to
	// clutter the UI object namespace. set_parent can
	// remove controls from the parent.
	LINKAGE void _unlink(),_link(Node*, int how);

	inline void _link_this_to_parent_last(Node *parent)
	{
		_link(parent,0);
	}
	inline void _link_this_to_parent_first(Node *parent)
	{
		_link(parent,1); //UNUSED
	}
	inline void _link_this_to_sibling_next(Node *sibling)
	{
		_link(sibling,2); //UNUSED
	}
	inline void _link_this_to_sibling_prev(Node *sibling)
	{
		_link(sibling,3); //UNUSED
	}
	
	#ifdef GLUI_GLUI_H___NODEPRECATED
	template<class C>
	/** Link in a new child control */
	inline void add_control(C *c){ c->set_parent(this); }
	#endif

	/** You may find this useful??? */
	#ifdef GLUI_DEBUG_STDOUT
	LINKAGE static FILE *_dump_FILE;
	void _dump(const char *text);
	#define GLUI_DEBUG_STDOUT__void_dump(...) \
	void _dump(__VA_ARGS__)
	#else
	static void _dump(...){} 
	#define GLUI_DEBUG_STDOUT__void_dump(...)
	#endif
};
  
/************ A simple nodes sort utility **********/

template<class T>
inline void glui_sort_node(Node &node, bool(*pred)(T*,T*), int depth)
{
	Node &upcast = *(T*)NULL; (void)upcast;
	node._sort((bool(*)(Node*,Node*))pred,depth);
}

/************************************************************/
/*                                                          */
/*                     User-Level UI class                  */
/*                                                          */
/************************************************************/

/**
 The main user-visible interface object to UI. 
*/
class UI : public Node
{	
public:

	inline UI *prev(){ return (UI*)_prev_sibling; }
	inline UI *next(){ return (UI*)_next_sibling; }

    /********** friend classes *************/

	friend class GLUI;
	friend class GLUT;
	friend class Node;	

	/********* abstract classes *************/

	class Control;	

	/************ UI objects ****************/

	//Define this to gain friendship
	#ifdef GLUI_USER_CONTROLS
	GLUI_USER_CONTROLS
	#endif

	class Button;
	class CheckBox;
	class Column;
	class Panel;
	class FileBrowser;
	class Rollout;	
	class EditText;	
	class CommandLine;
	class RadioGroup;
	class RadioButton;
	class Separator;
	class Spinner;
	class StaticText;
	class TextBox;
	class ScrollBar;
	class List;
	class ListBox;
	class Rotation;
	class Translation;

	struct Experiments //DEPRECATED
	{
		//These require extensive work 
		//to be usable. They should be
		//rethought from the ground up.

		class Tree; class TreePanel;
	};
	
	//UNIMPLEMENTED
	/** PX is four margins: left,right,top,bottom, or a string. 
	 * 
	 * "fixed" disables the built-in layout model.
	 * "space" is the built-in model.
	 * "block" prevents item spacing.
	 *
	 * Margins are limited (-31,+31) to the nonprintable range.
	 * Unrecognized strings may be processed by user callbacks.
	 */	
	typedef signed char PX;
	
	/***************************************/

	/*** Definition of callbacks ***/
	//Note: Void_CB is used to cast pointers also, see:
	//https://stackoverflow.com/questions/36645660/why-cant-i-cast-a-function-pointer-to-void
	//https://github.com/libglui/glui/pull/71 
	typedef void(*Void_CB)();
	typedef void(*Update_CB)(int id);
	typedef void(*Control_CB)(Control*);
	
	/**
	 Callback Adapter Class                            
	 Allows us to support different types of callbacks;
	 like a UI_Update_CB function pointer--which takes an int;
	 and a UI_Control_CB function pointer--which takes a UI::Control object.
	*/
	class CB
	{
		//2019: Making this more extensible/manageable.
		//Update_CB _idCB; Control_CB _objCB;
		int _sig; Update_CB _cb;

	public:
	
		CB():_sig(),_cb(){}
		CB(Update_CB cb):_sig(1),_cb((Update_CB)cb)
		{}
		template<class T> //NEW
		CB(void(*cb)(T*)):_cb((Update_CB)cb)
		{
			//Type 3 is not used, but may be helpful to figure
			//out if the callback is specialized.

			//Note: Clients may define a version of Control
			//that has the same layout with more functionality.
			_sig = sizeof(T)>sizeof(typename T::Control)?3:2;
			return;
			typename T::Control *upcast = (T*)NULL; (void)upcast;
		}
		//Needed because MSVC2010 thinks "typename Control" 
		//is the constructor. It doesn't require & decoration.
		CB(Control_CB cb):_sig(2),_cb((Update_CB)cb)
		{}

		/** This control just activated. Fire our callback.*/
		inline void operator()(Control*)const;
		inline bool operator!()const{ return !_sig; }
		inline operator bool()const{ return _sig!=0; }
	};

	/*** Flags for UI class constructor ***/
	enum
	{
	SUBWINDOW         = 2,
	SUBWINDOW_TOP     = 4,
	SUBWINDOW_BOTTOM  = 8,
	SUBWINDOW_LEFT    = 16,
	SUBWINDOW_RIGHT   = 32,

	_HIDDEN           = 64, //2019
	};

	inline UI(C_String name, int x=-1, int y=-1, int flags=0, Panel *mp=NULL)
	{
		_main_panel = mp; _init(name.str,flags,x,y,-1);
	}
	inline UI(int parent__glut_window_id, int subpos=SUBWINDOW, Panel *mp=NULL)
	{
		_main_panel = mp; _init(NULL,subpos,0,0,parent__glut_window_id);
	}
	//EXPERIMENTAL
	//This may not make any sense as GLUI works right now.
	inline UI(UI *parent, int subpos=SUBWINDOW, Panel *mp=NULL)
	{
		_main_panel = mp; _init(NULL,subpos,0,0,parent->_glut_window_id);
	}
	
	//DEPRECATED
	#ifdef GLUI_GLUI_H___NODEPRECATED
	/** Set the window where our widgets should be displayed. */
	inline void set_main_gfx_window(int window_id);
	/** Redraw the main graphics window. */
	inline void post_update_main_gfx();
	/** DEPRECATED interface for creating new UI objects */
	inline bool add_control(Control *control);
	inline void add_column(bool draw_bar=true);
	inline void add_column_to_panel(Panel *panel, bool draw_bar=true);
	inline void add_separator();
	inline void add_separator_to_panel(Panel *panel);
	inline RadioGroup *add_radiogroup(int *live_var=NULL, int id=-1, CB=CB());
	inline RadioGroup *add_radiogroup_to_panel(Panel *panel, int *live_var=NULL, int id=-1, CB=CB());
	inline RadioButton *add_radiobutton_to_group(RadioGroup *group, const char *name);
	inline ListBox *add_listbox(const char *name, int *live_var=NULL, int id=-1, CB=CB());
	inline ListBox *add_listbox_to_panel(Panel *panel, const char *name, int *live_var=NULL, int id=-1, CB=CB());
	inline Rotation *add_rotation(const char *name, float *live_var=NULL, int id=-1, CB=CB());
	inline Rotation *add_rotation_to_panel(Panel *panel, const char *name, float *live_var=NULL, int id=-1, CB=CB());
	inline Translation *add_translation(const char *name, int trans_type, float *live_var=NULL, int id=-1, CB=CB());
	inline Translation *add_translation_to_panel(Panel *panel, const char *name, int trans_type, float *live_var=NULL, int id=-1, CB=CB());
	inline CheckBox *add_checkbox(const char *name, int *live_var=NULL, int id=-1, CB=CB());
	inline CheckBox *add_checkbox_to_panel(Panel *panel, const char *name, int *live_var=NULL, int id=-1, CB=CB());
	inline Button *add_button(const char *name, int id=-1, CB=CB());
	inline Button *add_button_to_panel(Panel *panel, const char *name, int id=-1, CB=CB());
	inline StaticText *add_statictext(const char *name);
	inline StaticText *add_statictext_to_panel(Panel *panel, const char *name);
	inline EditText *add_edittext(const char *name, int data_type=UI_EDIT_TEXT, void *live_var=NULL, int id=-1, CB=CB());
	inline EditText *add_edittext_to_panel(Panel *panel, const char *name, int data_type=UI_EDIT_TEXT, void *live_var=NULL, int id=-1, CB=CB());
	inline EditText *add_edittext(const char *name, String &live_var, int id=-1, CB=CB());
	inline EditText *add_edittext_to_panel(Panel *panel, const char *name, String &live_var, int id=-1, CB=CB());
	inline Spinner *add_spinner(const char *name, int data_type=UI_EDIT_INT, void *live_var=NULL, int id=-1, CB=CB());
	inline Spinner *add_spinner_to_panel(Panel *panel, const char *name, int data_type=UI_EDIT_INT, void *live_var=NULL, int id=-1, CB=CB());
	inline Panel *add_panel(const char *name, int type=/*ETCHED*/1);
	inline Panel *add_panel_to_panel(Panel *panel, const char *name, int type=/*ETCHED*/1);
	inline Rollout *add_rollout(const char *name, bool open=true, int type=/*ETCHED*/1);
	inline Rollout *add_rollout_to_panel(Panel *panel, const char *name, bool open=true, int type=/*ETCHED*/1);
	#endif		
	
	/** Gets the subwindow position. It doesn't have to
	match GLUT_WINDOW_X & GLUT_WINDOW_Y, since that seems
	to not get flushed until a window frame is manipulated. */
	inline void get_subpos(int *x, int *y)
	{
		if(x) *x = _subpos_x; if(y) *y = _subpos_y;
	}
			   
	/** Packs controls if packing operation is pending. */
	LINKAGE void get_dims(int *w, int *h);
	int get_w(){ int w; get_dims(&w,NULL); return w; }
	int get_h(){ int h; get_dims(NULL,&h); return h; }

	/** Can't think of a good reason to hide this. */
	inline Panel *get_main_panel(){ return _main_panel; }
				
	inline int get_glut_window_id(){ return _glut_window_id; }
	inline int get_glut_parent_id(){ return _glut_parent_id; }
	inline int get_glut_escape_id(){ return _glut_escape_id; }
	inline int get_glut_origin_id(){ return _glut_origin_id; }
		
	/** 
	  Set the window that gains focus when Esc key is pressed.
	  @param execute_callback is only for set_main_gfx_window.
	  It causes execute_callback to @c glutSetWindow(glut_id).
	 */
	inline void set_main_esc_window(int glut_id=-1, bool execute_callback=false)
	{
		_glut_escape_id = glut_id;
		if(execute_callback) _main_gfx_callback = true; //back-compat
	}

	//WARNING: Adding controls no longer refreshes gratuitously.
	/** Pack, resize the window, and redraw all the controls. */
	LINKAGE void refresh();
     					 	  
	LINKAGE void enable(bool=true);
	
	inline void disable(){ enable(false); }

	LINKAGE void sync_live();
	
	LINKAGE void show(bool show=true);
	
	inline void hide(){ show(false); }

	/**WARNING
	 * Destroying GLUT windows is very sensitive to crashing
	 * GLUT. Originally closing was done in the next display
	 * event. It's now done in an idle phase. Nvidia GLUT is
	 * known to crash inside mouse events, but not keyboards.
	 * The final subwindow will close the parent window when
	 * the detach==false. That's the recommended way, but it
	 * is set to true by default for historical and semantic
	 * reasons. 
	*/
	LINKAGE void close(bool now=false, bool detach=true);

protected:
	
	/**WARNING
	 * Closing had been implemented by close_internal inside
	 * the glutDisplayFunc callback. Closing subwindows this
	 * way would mean that the parent window would either be
	 * closed first (destroying them) or would wait for them
	 * to be closed; which is not really possible to do very
	 * conveniently. Now that glutDestroyWindow is called at
	 * the same time as deleting the UI, when and where GLUT 
	 * implementations crash is a process of trial-and-error.
	 *
	 * Signals to destroy GLUT window and do this->_delete().
	 */
	void _close_internal();

	inline UI(){ /*NOP*/ }

	LINKAGE void _init(const char *name, int subpos, int x, int y, int parent_glut_window_id);

	/************************************************************/
	/*                                                          */
	/*       Main Window UI class (not user-level)              */
	/*                                                          */
	/************************************************************/

	/**
	A GLUI_Main handles GLUT events for one window, routing them to the 
	appropriate controls.  The central user-visible "UI" class 
	inherits from this class; users should not allocate GLUT_Main objects.
  
	There's a separate GLUI_Main object for:
  	- Each top-level window with GUI stuff in it.
	- Each "subwindow" of another top-level window.

	All the GLUI_Main objects are listed in GLUI_Master.gluis.
	A better name for this class might be "GLUI_Environment";
	this class provides the window-level context for every control.
	*/

	//PACKING DATA	
	bool _pending_idleclose; //Nvidia: Close within mouse event loop is unsafe.
	bool _pending_repacking; 
	bool _pending_redisplay; //Suppresses glutPostRedisplay in close procedure.
	bool _main_gfx_callback;	
	int _glut_window_id;
	int _glut_parent_id;
	int _glut_origin_id; //_top_level_glut_window_id
	int _glut_escape_id; //_main_gfx_window_id		
	Control *_active_control;
	Control *_mouse_over_control;
	Panel *_main_panel;
	enum _buffer_mode 
	{
	_buffer_front=1, ///< Draw updated controls directly to screen.
	_buffer_back=2   ///< Double buffering: postpone updates until next redraw.
	}
	_buffer_mode; ///< Current drawing mode	
	int _w,_h;
	int _subpos_x,_subpos_y; //NEW
	int _scroll_w,_scroll_h; //UNUSED (Plan to use with glutPopWindow/scrolling)
	int _flags; //SUBWINDOW 		
	int _ui_id;

	friend class Texture;
	String _textures; 
	LINKAGE static int _textures_id(int inc);

	/********** Misc functions *************/

	Control *_find_next_control(Control *control);	
	Control *_find_prev_control(Control *control);
    
	struct _glut_friends;
	bool _mouse(int button, int state, int x, int y);
	bool _keyboard(int key);
	void _keyboard_up(int key);
	bool _special(int key);
	void _special_up(int key);
	bool _motion(int x, int y);
	void _passive_motion(int x, int y);
	void _reshape(int w, int h);
	void _visibility(int state){ /*NOP*/ }
	void _entry(int state);
	void _display();
	void _idle();
	bool _needs_idle();

public:
	
	enum /****** How was a control activated? ************/
	{
	  //NOTE: MOUSE implies there are valid coordinates.
	  //TAB is blocked by controls that use tab or that
	  //do not participate in tab navigation.
	  //OTHER (NEW) is a request to focus without mouse
	  //information. Enablement happens post activation.

		ACTIVATE_MOUSE=1, ACTIVATE_TAB, ACTIVATE_OTHER,

	 /*************** set_click_type *********************/

		/* wincon.h defines DOUBLE_CLICK as 2 */
		SINGLE_CLICKED=0, DOUBLE_CLICKED
	};	

	String window_name;

	union /* zero-sized structures */
	{
		Silver3ubv bkgd_color; //back-compat
		Silver3fv bkgd_color_f; //back-compat
				
		// Historically this is GLUT_BITMAP_HELVETICA_12
		// It can default to NULL if it would not affect
		// existing software.
		// If it is changed before controls are added it
		// can avoid needlessly initializing the default.
		void *font;	
	};
					/**** 2019 ****/

	//Supports create_callback.
	#ifndef GLUI_USER_TYPE
	#define GLUI_USER_TYPE void
	GLUI_USER_TYPE *user;
	#endif

	/** This is a default callback for controls without one. */
	CB active_callback; 
								
	/** Restoring these previously disabled callbacks. 
	 If the various handlers return true these are consulted.
	 NOTE: More can be added. */
	bool(*keyboard_callback)(UI*,int,int);
	bool(*special_callback)(UI*,int,int);
	bool(*mouse_callback)(UI*,int,int,int,int);
	bool(*motion_callback)(UI*,int,int);
	bool(*wheel_callback)(UI*,int, int x, int y, int modifiers);
	bool(*reshape_callback)(UI*,int,int);
	
	/** New replacement for old post_update_main_gfx method. 
	
	   output_live is true when called by output_live. The
	   other time it's called is after execute_callback to
	   ensure the results of the callback will be rendered. 
	   This is because Nvidia GLUT fires the display event
	   immediately when glutPostDisplay is called, instead
	   of "posting" it as its name implies.

	 */
	void(*output_callback)(Control*, bool output_live);

	/** New way to draw over UI prior to flushing the buffer. 

	 Return true to get default behavior. If post is true it
	 is to swap-the-buffers, or else it is to clear prior to
	 drawing the controls. Control is the UI's main panel or
	 else it might be a modal dialog, for instance a message
	 box or other "pop-up" element. You may also use this to
	 do anything fancy like switch the "render-target" prior
	 to drawing, and switch back for post-processing effects.

	 */	
	bool(*canvas_callback)(UI::Control*, bool post);

public:
	
	inline bool deactivate_current_control()
	{
		return activate_control(NULL,0);
	}
	LINKAGE bool activate_control(Control*, int how=ACTIVATE_OTHER);

	inline void activate(){ activate_control(NULL); }
	
						/**** 2019 ****/

	LINKAGE Control *find_control(int x, int y);

	enum /****** curr_modifiers for wheel API ****************/
	{
		WHEEL_SHIFT=1, //GLUT_ACTIVE_SHIFT
		WHEEL_CTRL=2, //GLUT_ACTIVE_CTRL
		WHEEL_ALT=4, //GLUT_ACTIVE_ALT (Mac may want Opt key?)
		WHEEL_HORZ=8, //horizontal wheel
	};
	/** GLUT doesn't have portable wheel facilities, however  *
	/** this method will find Spin_Interface controls to use. */	
	inline bool wheel(int amount, int x, int y, int modifiers=0)
	{
		if(!_wheel(amount,find_control(x,y),modifiers))
		return false;
		if(wheel_callback) 
		return wheel_callback(this,amount,x,y,modifiers);
		return true;
	}
	LINKAGE bool _wheel(int amount, Control*, int modifiers); 

public: //User draw methods

	/* Mouse_Interface calls this to restore projection */
	inline void set_ortho_projection()
	{
		Window::set_ortho_projection(_w,_h);
	}

private: //Display manager methods
	
	/** Switch to the appropriate draw buffer now.
	 Returns the old draw buffer. */
	int _set_current_draw_buffer();	
	
	/** This is a subroutine of GLUI::reshape. */
	void _subwindow_position(int,int);

	/** Recompute size/position for all controls */
	void _pack_controls();    
};

/************************************************************/
/*                                                          */
/*                  Class GLUI_Texture                      */
/*                                                          */
/************************************************************/

/**2019 addition
  Texture is used with glGetTextures on a per OpenGL context
  basis. Controls that use textures need to use it. Refer to
  Rotation::texture_and_light.
  NOTE: The problem this solves is like thread-local-storage
  but for the UI objects in place of a "thread."
 */
class Texture
{
public:

	const int id;
	Texture():id(UI::_textures_id(1))
	{}

	inline unsigned char &get(UI *ui)
	{
		assert(ui);
		return (unsigned char&)ui->_textures[id];
	}
	inline bool get(UI *ui, unsigned int &texture)
	{
		texture = ui?get(ui):0; return texture!=0;
	}
	inline bool set(UI *ui, unsigned int &texture)
	{
		if(!ui||texture-1>=255) return false;
		unsigned char &slot = get(ui);
		if(!slot) slot = (unsigned char)texture;
		else return false; return true;
	}	
};

/************************************************************/
/*                                                          */
/*            Class Live_Variable (abstract)                */
/*                                                          */
/************************************************************/

/**
  Historically Control objects contained two copies of this
  data structure. Which is why it's its own class. It turns
  out, however, that the present code did not use the other
  copy, called last_live_int etc.
 */
class Live_Storage
{ 
public:

	//NOTE: Historically this data is multi-representational.
	//That approach has its benefits, but it is not required.
	//Presently it's a back-compat matter.
	
	/** Variable value storage */
	int int_val; /**< Our integer value */		
	String text; /**< The text inside this control */    	
	double*float_array_val; // Points at float_val initially.
	double float_val; /**< Our float value(s) */
	union Extra_Data //UI::Translation
	{							  
		//EXPERIMENTAL
		//This can be a user data field, but it can depend on
		//the control. The Translation control uses it for 2D
		//translation.
		double float_union; int int_union; void *ptr_union; 

	}extra_data; /**< One more float value */
		
	Live_Storage()
	{
		int_val = 0;		
		//text = "";		
		float_val = 0; float_array_val = &float_val;	
		extra_data.ptr_union = 0;
	}

public:

	//These implement the Live template.
	//They are not extensible presently.

	inline void _var_set(int *p, int)
	{
		int_val = *p; 
		glui_format_str(text,"%d",int_val);
		float_val = (double)int_val;
	}
	inline void _var_set(const char *p, int)
	{
		int_val = atoi(p); 
		if(p!=text.c_str()) text = p; //HACK
		float_val = strtod(p,NULL);
	}	
	inline void _var_set(String *p, int=0)
	{
		const char *cs = p->c_str();
		int_val = atoi(cs); text = *p; 
		float_val = strtod(cs,NULL);
	}	
	inline void _var_set(float *p, int float_count)
	{
		double d = *p; _var_set(&d,1);
		for(int i=1;i<float_count;i++)
		{
			float_array_val[i] = p[i];
		}
	}
	inline void _var_set(double *p, int float_count)
	{
		int_val = (int)*p;
		//EditText::set_numeric_text()
		{
			//Note, the text contains only one value.

			glui_format_str(text,"%#g",*p);
			//Printing all digits looks better when using the
			//spinner control. update_live can format numbers.
			if(1) //"0.000000" -> "0.0"
			{
				size_t size = text.size()-1;		
				while('0'==text[size]) size--;
				//NEW: Keep 0's when not whole number.
				//if('.'==text[size]) size++; text.erase(size+1);
				if('.'==text[size]){ size++; text.erase(size+1); }
			}
		}
		memcpy(float_array_val,p,sizeof(*p)*float_count);
	}

	inline void _var_get(int *p){ *p = int_val; }	
	inline void _var_get(String *p){ *p = text; }
	inline void _var_get(double *p){ *p = *float_array_val; }
	inline void _var_get(float *p){ *p = (float)*float_array_val; }
	
	inline bool _var_cmp(int *cmp, int){ return *cmp!=int_val; }	
	inline bool _var_cmp(String *p, int){ return *p!=text; }
	inline bool _var_cmp(float *cmp, int float_count)
	{
		for(int i=float_count;i-->0;) 
		if(float_array_val[i]!=cmp[i]) return true; return false;
	}	
	inline bool _var_cmp(double *cmp, int float_count)
	{
		for(int i=float_count;i-->0;) //memcmp?
		if(float_array_val[i]!=cmp[i]) return true; return false;
	}	

	/*UNUSED because last_live is not in use
	inline void _live_dup(const Live_Storage &cp, int float_count)
	{
		int_val = cp.int_val; 
		text = cp.text;		
		float_val = cp.float_val;
		if(float_count>1) memcpy(float_array_val,
		cp.float_array_val,float_count*sizeof(double));
	}*/

	/* Control::live_type is a back-compat matter. */
	inline int _live_type(int*){ return UI_LIVE_INT; }
	inline int _live_type(String*){ return UI_LIVE_STRING; }
	inline int _live_type(float*){ return UI_LIVE_FLOAT; }
	inline int _live_type(double*){ return UI_LIVE_DOUBLE; }
};
class Live_None
{ 
public: //Interface methods

	typedef class Live_Variable LV;

	~Live_None(){} /* Live::~Live is non-virtual*/

	virtual void init_live(){ /*0-INITIALIZE ME*/ }

	virtual void output_live(){}

	virtual bool compare(){ return false; }

	virtual int live_type(){ return UI_LIVE_NONE; }
};
class Live_Pointer //* FIRST Live_Variable MEMBER */
{
public:

	struct Live_Type //A name silences GDB output.
	:
	private Live_None //FIRST MEMBER
	{
		/* Use -> to access virtual methods. */
		inline Live_None *operator->()
		{
			return this;
		}
		/* Replaces old live_type enum field. */
		inline operator int()
		{
			return operator->()->live_type(); 
		}
		/* Raw copy virtual method pointers. */
		inline void operator=(const Live_None &cp)
		{
			memcpy((void*)this,&cp,sizeof(cp));
		}

	}live_type;

	/** A pointer to the user's live variable value */
	void *ptr_val; 
		  
	//2019: Taking pains these may be merged into one member
	//however, on 64bit builds there won't be memory savings.	
	int live_var_width, float_array_size;

	Live_Pointer()
	{
		ptr_val = NULL;
		live_var_width = 0;
		float_array_size = 1;
	}
};
class Live_Variable : public Live_Pointer, public Live_Storage
{	
public:

	/** "Live variable" updating */

	//2019: last_live_int and friends are not actually playing
	//a part in updates. The live_sync methods compare with it.
	//But they could just as well compare to the current value.
	//It could be valuable, but it would have to be used first.

	//Live_Storage last_live; //UNUSED

public: //Live template support

	inline void _copy_live_string()
	{		
		int w = live_var_width;
		if(!w) w = text.size()+1;
		if(ptr_val) memcpy(ptr_val,text.c_str(),w);
	}
	inline bool _compare_live_string()
	{
		if(!ptr_val
		||live_var_width&&live_var_width<(int)text.size()+1) 
		return false;		
		return 0!=strcmp((char*)ptr_val,text.c_str());
	}
};

template<class S, class T>
/**2019 addition
 *
 * This template is primarily because GCC cannot export virtual 
 * method definitions. It makes it possible to overload methods 
 * like set_text. Exporting virtual methods is needed so client
 * code can instantiate/extend the classes without linking with
 * the same C++ Runtime library as the GLUI module is linked to.
 */
class Interface : public T
{
public:
	
	/* CommandLine had used Super */
	typedef typename T::Sub Super;

	/* This way Super refers to a base class
	that is itself an Interface */
	typedef S Sub;

	/** Note: DRAW SHOULD BE protected BUT IT'S EXPOSED
	SO USERS CAN (IF THEY CHOOSE) BYPASS GLUT ALTOGETHER
	IN ORDER TO DRAW CONTROLS MANUALLY. */
	virtual void draw(){ ((S*)this)->_draw(); }

	/** This replaces various set_int/float_val methods. */
	virtual void update_live(){ ((S*)this)->_update_live(); }

	virtual void update_size(){ ((S*)this)->_update_size(); }	

	virtual bool mouse_down_handler(int local_x, int local_y)
	{
		return ((S*)this)->_mouse_down_handler(local_x,local_y);
	}
	virtual bool mouse_up_handler(int local_x, int local_y, bool inside=true)
	{
		return ((S*)this)->_mouse_up_handler(local_x,local_y,inside);
	}
	virtual bool mouse_held_down_handler(int local_x, int local_y, bool inside=true)
	{
		return ((S*)this)->_mouse_held_down_handler(local_x,local_y,inside);		
	}
	virtual bool mouse_over(bool state, int local_x, int local_y)
	{
		return ((S*)this)->_mouse_over(state,local_x,local_y);
	}

	virtual bool key_handler(int key, int modifiers=0)
	{
		return ((S*)this)->_key_handler(key,modifiers);
	}
	virtual bool special_handler(int key, int modifiers=0)
	{
		return ((S*)this)->_special_handler(key,modifiers);
	}

	virtual void idle(){ ((S*)this)->_idle(); }

	//Dual use: 1) implements control's activation logic.
	//2) Dispenses with this->ui->activate_control(this).
	virtual bool activate(int how=UI::ACTIVATE_OTHER)
	{
		if(!((S*)this)->_activate(how))
		return false;
		//I wanted to constrain this to Control but MSVC and GCC
		//both wanted different formulations, so.
		return ((S*)this)->_activate_control(how);
	}
	//WARNING: Super::deactiveate (or ::) doesn't invoke the base
	//class implementation. Use _activate or activate(0) to do so.
	//This can be solved by making this a virtual method, however
	//this use case should be limited to developing a new control.
	inline bool deactivate(){ return activate(0); }
	
	virtual bool needs_idle(){ return ((S*)this)->_needs_idle(); }	

	virtual UI::Control *wheel(){ return ((S*)this)->_wheel(); }	
};

/************************************************************/
/*                                                          */
/*    GLUI_Control: abstract base class for all controls    */
/*                                                          */
/************************************************************/

/**
 All the GUI objects inherit from UI_Control: buttons,
 checkboxes, labels, edit boxes, scrollbars, etc.
 Most of the work of this class is in routing events,
 like keystrokes, mouseclicks, redraws, and sizing events.
 
 Yes, this is a huge and hideous class.  It needs to be 
 split up into simpler subobjects.  None of the data members
 should be directly accessed by users (they should be protected,
 not public); only subclasses.
*/
class UI::Control 
:
public Interface<Control,Node>
,
public Live_Variable, public Window
{
public: //Retired interfaces
			
	LINKAGE void set_name(C_String="");
				 
	LINKAGE void set_int_val(int new_int=0);

	LINKAGE void set_text(C_String="");

	LINKAGE void set_float_val(double new_float=0);

	LINKAGE void set_float_array_val(float *array_ptr=0);

	LINKAGE void set_float_array_val(double *array_ptr=0);
	
	inline void post_live(){ output_live(); update_live(); }
		  	
	//REMOVE ME
	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline void set_ptr_val(void *new_ptr=NULL)
	{
		//The semantics here are very unclear.
		ptr_val = new_ptr; /* output_live(); */
		post_live(); //output_live or init_live?
	}
	#endif

	inline int get_int_val(){ return int_val; }

	inline double get_float_val(){ return float_val; }

	inline void get_float_array_val(float *array_ptr)
	{	
		if(!GLUI_H||array_ptr) //legacy
		for(int i=float_array_size;i-->0;)
		array_ptr[i] = (float)float_array_val[i];
	}
	inline void get_float_array_val(double *array_ptr)
	{				 
		if(!GLUI_H||array_ptr) //legacy
		memcpy(array_ptr,float_array_val,float_array_size);
	}
	inline double *get_float_array_val(){ return float_array_val; }	
	
	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline int get_id(){ return user_id; }	
	inline void set_id(int id){ user_id = id; }
	#endif

	LINKAGE void enable(bool enable=true);
	inline void disable(){ enable(false); }
	inline void disable(Control *toggle){ disable(); toggle->activate(); }

private: //Interface methods

	template<class,class>
	//These abstract implementations cannot be called 
	//directly, to minimize programming typo mistakes.
	//template<class,class> friend class Interface;
	//Let's uglify for a 10yo GCC Bug (I'm speechless.)
	//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52625
	friend class GLUI_Library::Interface;

	inline bool _mouse_down_handler(int local_x, int local_y)
	{
		return true; //return false; //2019
	}
	inline bool _mouse_up_handler(int local_x, int local_y, bool inside)
	{
		return true; //return false; //2019
	}
	inline bool _mouse_held_down_handler(int local_x, int local_y, bool inside)
	{
		return true; //return false; //2019
	}
	inline bool _mouse_over(bool state, int local_x, int local_y)
	{
		return true; //return false; //2019
	}

	inline bool _key_handler(int key, int modifiers)
	{
		//Note: If arrow keys are made to navigate groups then
		//GLUI's 3D widgets should use WASD so to be navigable.
		switch(key) 
		{
		case 'w': key = 101; break; //GLUT_KEY_UP
		case 's': key = 103; break; //GLUT_KEY_DOWN
		case 'a': key = 100; break; //GLUT_KEY_LEFT
		case 'd': key = 102; break; //GLUT_KEY_RIGHT
		default: return true;
		}
		return special_handler(key,modifiers);
	}
	inline bool _special_handler(int key, int modifiers)
	{
		return true; //return false; //2019
	}

	inline void _update_size(){ /*NOP*/ }

	inline void _update_live(){ /*NOP*/ }

	inline void _idle(){ /*NOP*/ }    

	inline bool _activate(int how){ return true; }	

	//Note: This isn't an interface. It's defined here for proximity to _activate.
	inline bool _activate_control(int how)
	{
		if(!ui) return false;
		Node *cmp = ui->_active_control;
		if(how&&this==cmp||!how&&this!=cmp)
		return true;
		return ui->activate_control(this,how);
	}

	//Note: This WAS a pure-virtual method.
	inline void _draw(){ assert(0); }
	
	/***** GLUI_Control::needs_idle() *********/
	/* This method gets overloaded by specific classes, e.g. Spinner.            */
	/* It returns whether or not a control needs to receive an idle event or not */
	/* For example, a spinner only needs idle events when the user is holding    */
	/* the mouse down in one of the arrows.  Otherwise, don't waste cycles       */
	/* and OpenGL context switching by calling its idle.                         */

	inline bool _needs_idle(){ return false; }

	inline Control *_wheel(){ return NULL; }
	  
public:
		
	/** Link in a new child control.
	WARNING: Replaces Node::add_control. 
	*/
	LINKAGE void set_parent(Node *parent=NULL);
	
	//REMINDER: Parent may be collapsible Node.
	inline Control *has_parent()
	{
		return dynamic_cast<Control*>(_parent_node);
	}
	inline Control *prev(){ return (Control*)_prev_sibling; }
	inline Control *next(){ return (Control*)_next_sibling; }
	inline Control *first_child(){ return (Control*)_child_head; }
	inline Control *last_child(){ return (Control*)_child_tail; }

	template<class T> T *prev()
	{
		while(Control*p=prev()) if(!p->hidden)
		return dynamic_cast<T*>(p); return NULL;
	}
	template<class T> T *next()
	{
		while(Control*p=next()) if(!p->hidden)
		return dynamic_cast<T*>(p); return NULL;
	}

	inline Control *find(int id)
	{
		return first_child()->find_fn(&Control::id,id,0);
	}
	inline Control *find_child(int id)
	{
		return first_child()->find_fn(&Control::id,id,1);
	}
	template<class S, class T>
	/** find_fn can resume search for a non-unique value. */
	inline Control *find_fn(const S Control::*PtoM, const T &cmp, 
	int depth=1, Control*(Control::*fn)()=&Control::next)
	{
		typedef Control*(Control::*fn_t)();
		if(cmp==(this->*PtoM)) return this; 
		Control *c; if(depth-1) 
		if(c=fn==(fn_t)&Control::prev?last_child():first_child())
		if(c=c->find_fn(PtoM,cmp,depth-1,fn)) 
		return c; if(c=(this->*fn)()) 
		c = c->find_fn(PtoM,cmp,depth,fn); return c;
	}
	
	enum /********* Control placement types **********/
	{		
		CENTER=1, RIGHT, LEFT, //NEW

		/***** Control placement modifiers ******/
			
		TOP=4, BOTTOM=8, BEHIND=16, FRONT=32,
	
	/********* Control alignment types **********/
	
		ALIGN_CENTER=CENTER, ALIGN_RIGHT=RIGHT, ALIGN_LEFT=LEFT, 

		ALIGN_EXPAND=0, //NEW
		ALIGN_BUDDY=-1, //back-compat (Spinner->EditText)

	/*************** set_click_type *********************/

		/* wincon.h defines DOUBLE_CLICK as 2 */
		SINGLE_CLICKED=0, DOUBLE_CLICKED
	};

	int w, h; /* dimensions of control */
	
	int x_abs, y_abs; /** Onscreen coordinates */
	
	/* INNER margins, by which child controls are indented 
	 Column STORES ITS LEFT AND RIGHT MARGINS RESPECTIVELY. */
	int x_off, y_off_top, y_off_bot;
	
	//These are new. I don't know what to call them, but I 
	//know I don't want to read "x_off_left"/"x_off_right".
	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ int x_lr, text_x_offset; }; int x_rl;
	#else
	int x_lr, x_rl; /* Read: left-to-right/right-to-left. */
	#endif
					
	/** "activation" for tabbing between controls. */
	
	//bool active;       ///< If true, we've got the focus
	bool spacebar_mouse_click; ///< Spacebar simulates click.	
	bool can_activate; ///< If false, remove from tab order.
	bool tab_navigate; //NEW: Replacing wants_tabs
        
	/** Our containing event handler (NEVER NULL during event processing!) */
	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ UI *ui, *glui; };
	#else
	UI *ui;
	#endif

	/** Properties of our control */    

	String name; /**< The name of this control */

	Node *collapsible; //Node collapsed_node;

	/** Alignment changes the layout; placement is relative to the titles. */
	int alignment,placement;

	bool is_container;  /**< Is this a container class (e.g., panel) */
	bool is_open;	
	bool enabled;    /**< Is this control grayed out? */
	bool hidden; //2019: hidden no longer means collapsed. */
	
	/** Callbacks */
	CB callback; ///< User callback function, or NULL.
	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ int id,user_id; }; ///< ID to pass to callback.
	#else
	int id; ///< Integer to pass to the callback function.
	#endif
	
	//UNIMPLEMENTED
	/** User layout control. Margins or string. */
	union{ PX *px, (*_px_visual)[32]; };

	template<class T>
	/** Sets any of the above public data members recursively. */
	inline void set(const T Control::*PtoM, const T &cp, bool recurse=true)
	{
		//REMINDER: Casting can construct objects.
		*const_cast<T*>(&(this->*PtoM)) = cp; 
		if(recurse) for(int pass=1;pass<=2;pass++)
		{
			if(Node*node=pass==1?this:collapsible)
			for(node=node->first_child();node;node=node->next()) 
			{
				((Control*)node)->set(PtoM,cp);
			}
		}
	}

public:
	
	//TODO? I think this should be characterized more generically as tabs.
	LINKAGE void _set_is_open(bool is_open=true);

	/**2019 addition 
	 "hidden" status no longer refers to collapsed controls. The elements
	 don't take up space in the layout when hidden. I think ultimately it
	 will be desirable for end-users to optionally hide the elements they
	 don't won't to see or interact with. A right-click pop-up menu would
	 be a good way to summon previously hidden elements.
	*/
	LINKAGE void set_hidden(bool hidden=true, bool recurse=true);

	//This operates on siblings. It's badly designed, worded, unintuitive.
	#ifdef GLUI_GLUI_H___NODEPRECATED
	/** Hide (shrink into a rollout) and unhide (expose from a rollout) */
	inline void hide_internal(bool recurse=true)
	{
		for(Control*p=this;p;p=p->next()) p->set_hidden(true);
	}
	/** Hide (shrink into a rollout) and unhide (expose from a rollout) */
	inline void unhide_internal(bool recurse=true)
	{
		for(Control*p=this;p;p=p->next()) p->set_hidden(false);
	}
	#endif

	#ifdef GLUI_GLUI_H___NODEPRECATED
	/** Return true if it currently makes sense to draw this class. */
	inline int can_draw(){ return ui&&!hidden; }
	#endif
	 
	/* Resize, flagging UI for repack & redraw. */
	inline void resize(bool now=true) 
	{
		if(now) update_size(); _post_repack(); 
	}
	inline void redraw() /* Redraw the control. */
	{
		_post_repack(false); 
	}	
	LINKAGE void _post_repack(bool repack=true);			       

	/** Implements old set_w/set_h APIs. */
	LINKAGE void set_dims(int w, int h);
	inline void set_w(int new_w){ set_dims(new_w,h); }
	inline void set_h(int new_h){ set_dims(w,new_h); }
	
	/**2019 addition
	 * Use with LEFT, RIGHT, CENTER and these modifiers:
	 *
	 * TOP: Positions control above its name.
	 * BOTTOM: Positions below.
	 * BEHIND: Control draws its own name. Depends on the control.
	 * FRONT: Hides name. Controls may add this or do custom draw.
	 *
	 * This is a new feature and not every control will work with
	 * it, though this is something that's ideally done without a
	 * control's involvement.
	 */
	LINKAGE void set_placement(int new_place);

	/** 
	 * Use with ALIGN_LEFT, etc. LEFT is equivalent to ALIGN_LEFT.
	 * Not every alignment type is so. 
	 *
	 * Alignment describes how controls are positioned in columns.
	 * One so positioned, "placement" describes where they appear
	 * relative to their name.
	 */
	LINKAGE void set_alignment(int new_align);

	inline void center(){ alignment = CENTER; }

	inline void align(int a=LEFT){ alignment = a; }

	inline void place(int p=RIGHT){ placement = p; }
	
	/* Called when ctrl is created */
	LINKAGE void init_live();	

	/* Reads live variable */	
	LINKAGE void sync_live(bool recurse=true, bool draw=true); 

	/** Writes live variable **/
	LINKAGE void output_live(); 	
	#ifdef GLUI_GLUI_H___NODEPRECATED
	/** Writes live variable **/
	inline void output_live(bool update_main_gfx)
	{
		output_live(); (void)update_main_gfx; //unused/legacy
	}
	#endif
		
	template<class T>
	/**2019 addition
	 * stage_live is equivalent to assigning int_val, etc. It
	 * keeps the other values in their multi-representational 
	 * state.
	 * Controls may need to use this in some contexts where a
	 * call to update_live would precipitate a stack-overflow.
	 */
	inline void stage_live(T *p, int float_count=1)
	{
		_var_set(p,float_count); 
	}
	inline void stage_live()
	{
		int_val = 0; text = '0'; float_array_val[0] = 0;
	}
	inline void stage_live(int float_count)
	{
		int_val = 0; text = "0.0"; assert(float_count>0);
		memset(float_array_val,0x00,sizeof(double)*float_count);
	}

	LINKAGE void execute_callback();

public: //User draw methods

	LINKAGE void get_column_dims(int *x, int *w);

	inline void get_margin_dims(int (&m)[4], int add=0)
	{
		m[0] = x_lr+add; m[1] = y_off_top+add;
		m[2] = w-m[0]-x_rl-add; m[3] = h-m[1]-y_off_bot-add;
	}

	inline void offset_dims(int *w, int *h)
	{
		if(w) *w+=x_lr+x_rl; if(h) *h+=y_off_top+y_off_bot;
	}
	inline void inset_dims(int *w, int *h)
	{
		if(w) *w-=x_lr+x_rl; if(h) *h-=y_off_top+y_off_bot;
	}	
	
	inline int name_width(bool pack=false)
	{
		int width = font?string_width(name):0;
		for(Control*p=has_parent();p&&pack;)
		return width-p->x_off; return width;
	}
	
	inline void draw_name(int x=0, int y=0, bool active=false)
	{
		draw_active_name(x,y,name,enabled?active:-1);
	}

	/* Calls "draw" interface recursively */
	inline void draw_recursive(bool if_hidden=true)
	{
		if(if_hidden&&hidden) return;

		draw_at_origin();
		
		for(Control*ch=first_child();ch;ch=ch->next()) 
		{
			ch->draw_recursive(if_hidden);
		}
	}	
	inline void draw_at_origin()
	{
		translate_to_origin(+1); draw();
		translate_to_origin(-1);
	}
	inline void translate_to_origin(int inv=1)
	{
		translate_to_origin(x_abs*inv,y_abs*inv); 
	}
	using Window::translate_to_origin;	

	/** Can technically be used without a UI pointer. */
	LINKAGE void pos_caret(int h=0, int x=0, int y=0);
		
private: //Display manager methods
	
	//NOTE: There's no reason to fully encapsulate this
	//methodology. It's just not exposed/exported right
	//this moment.

	friend class UI;
	
	/* Recalculate positions and offsets */
	void _pack(int=0,int=0),_align_control(),_align_children();
	
protected:

	inline Control()
	{
		//-1 indicates generic STATIC elements. 
		//execute_callback and Group_Interface treat
		//negative IDs differently from positive IDs. 
		id = -1; 
		px = NULL; //set_parent sets to "space".

		x_off          = UI_XOFF;	
		y_off_top      = 0; //UI_YOFF;
		y_off_bot      = 0; //UI_YOFF;
		x_lr           = 0;
		x_rl           = 0;
		x_abs          = 0; //UI_XOFF;
		y_abs          = 0; //UI_YOFF;
		
		ui             = NULL;
		w              = UI_DEFAULT_WIDTH;
		h              = UI_DEFAULT_HEIGHT;	
		alignment      = LEFT;
		placement      = RIGHT;
		is_container   = false;
		can_activate   = true;         /* By default, you can activate a control */
		spacebar_mouse_click = true;    /* Does spacebar simulate a mouse click? */	
		tab_navigate   = true;
		collapsible    = NULL;
		is_open        = true;
		hidden         = false;
		enabled        = true;
	}
	inline ~Control()
	{
		//_unlink does work to nullify dangling pointers like _active_control.
		if(!_parent_node) return;
		//~Node can't do this because _unlink needs to dynamic_cast<Control*>.
		_unlink();
		//This helps compilers better inline ~Node.
		assert(!_parent_node); _parent_node = NULL; //__assume(!_parent_node);
	}	
	template<class T>	
	#define GLUI_ARGS(...) \
	Node *parent, C_String name __VA_ARGS__ int id=-1, UI::CB cb=UI::CB()	
	#define GLUI_LIVE Live<LV> __(this,live_var); GLUI_INIT
	#define GLUI_INIT _control_init(this,parent,name.str,id,cb);
	#define GLUI_SBAR if(scroll) new ScrollBar(this,"",0,0); 
	static void _control_init(T *my, Node *parent, const char *name, int id, CB &cb)
	{	
		if(name) my->name = name; my->id = id; my->callback = cb;
		my->_members_init();
		my->_dynamic_init(parent); if(parent) my->set_parent(parent);
	}
	inline void _members_init(){}
	inline void _dynamic_init(...){}
	LINKAGE ScrollBar *scrollbar_init(CB);
	LINKAGE int _live_init_text_type(int et, const void *lv);		
};
/*** Requires definition of Control ***/
inline void UI::CB::operator()(Control *ctrl)const
{
	switch(_sig)
	{
	case 1: ((Update_CB)_cb)(ctrl->id); break;
	case 2:
	case 3: ((Control_CB)_cb)(ctrl); break;
	}
}

template<class T> 
/**
 * This class has dual-use, it implements Live_None but
 * it also implements the controls' inline constructors.
 */
class Live : public Live_None
{
public:

	//TODO: If _init were Live_Variable type it
	//would be possible to have more than 1 per
	//control, or use outside of controls. Then
	//~Live would call a user provided function.

	UI::Control *_init;
	Live(UI::Control *control, T *ptr)
	{
		//These must be identical to Live<char>.
		_init = control; _init->ptr_val = ptr;
		_init->live_type = *this;
	}
	~Live(){ _init->init_live(); } //non-virtual
	 	
	virtual int live_type()
	{
		return ((LV*)this)->_live_type((T*)NULL); 
	}
	virtual void init_live()
	{
		LV &lv = *(LV*)this; if(lv.ptr_val)
		lv._var_set((T*)lv.ptr_val,lv.float_array_size);
	}
	virtual bool compare()
	{
		LV &lv = *(LV*)this; if(lv.ptr_val)
		return lv._var_cmp((T*)lv.ptr_val,lv.float_array_size);
		return false;
	}	
	virtual void output_live()
	{
		LV &lv = *(LV*)this; 
		if(lv.ptr_val) lv._var_get((T*)lv.ptr_val);
	}
};
template<>
/**Adds live_var_width into the mix
 */
class Live<char> : public Live_None
{
public:

	UI::Control *_init;
	Live(UI::Control *control, char *ptr, int n=0)
	{
		//These must be identical to Live<T>.
		_init = control; _init->ptr_val = ptr;
		_init->live_type = *this;

		_init->live_var_width = n;
	}	
	~Live(){ _init->init_live(); } //non-virtual

	virtual void init_live()
	{
		LV &lv = *(LV*)this;
		if(lv.ptr_val) lv.text = (char*)lv.ptr_val;
	}
	virtual bool compare()
	{
		return ((LV*)this)->_compare_live_string();
	}	
	virtual void output_live()
	{
		((LV*)this)->_copy_live_string();
	}
	virtual int live_type(){ return UI_LIVE_TEXT; }
};
template<class T, int N>
/**
 * Float (and double) array forms
 */
class Live<T[N]> : public Live<T>
{
public:

	Live(UI::Control *control, T *ptr, int n=N)
	:
	Live<T>(control,ptr)
	{
		//Requiring control to coordinate itself.
		//_init->float_array_size = n;
		//_init->float_array_val = new double[n];
		(void)n;

		//Live<T> sets this, but it must be done
		//again to copy the virtual-method-table.
		Live<T>::_init->live_type = *this;
	}

	virtual void output_live()
	{
		typedef Live_Variable LV;
		LV &lv = *(LV*)this; if(lv.ptr_val)
		for(int i=lv.float_array_size;i-->0;)		
		((T*)lv.ptr_val)[i] = (T)lv.float_array_val[i];
	}
	virtual int live_type()
	{
		return 3+Live<T>::live_type(); //UI_LIVE_FLOAT_ARRAY; 
	}
};

/************************************************************/
/*                                                          */
/*                      Button class                        */
/*                                                          */
/************************************************************/
/**
  An onscreen, clickable button--an outlined label that 
  can be clicked.  When clicked, a button
  calls its CB callback with its ID.
*/
class UI::Button : public Interface<Button,Control>
{
public: //Interface methods

	LINKAGE void _draw();
	LINKAGE void _update_size();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);
	
public:
	
	/**
	Create a new button.
 
	@param parent The panel our object is inside; or the main UI object.
	@param name The text inside the button.
	@param id Optional ID number, to pass to the optional callback function.
	@param callback Optional callback function, taking either the int ID or control.
	*/
	explicit Button(GLUI_ARGS(="",)){ GLUI_INIT }
	template<class LV>
	inline Button(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }
	inline Button(){ _members_init(); }
	inline void _members_init()
	{
		h            = UI_BUTTON_HEIGHT;
		w            = 90; //100;
		alignment    = CENTER;
		placement    = CENTER|BEHIND;
	}
};

/************************************************************/
/*                                                          */
/*                     CheckBox class                       */
/*                                                          */
/************************************************************/

/**
 A checkbox, which can be checked on or off.  Can be linked
 to an int value, which gets 1 for on and 0 for off.
*/
class UI::CheckBox : public Interface<CheckBox,Control>
{
public: //Interface methods

	LINKAGE void _draw();		
	LINKAGE void _update_size();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);

public:	
	
	template<class LV>
	/**
	Create a new checkbox object.
 
	@param parent The panel our object is inside; or the main UI object.
	@param name Label next to our checkbox.
	@param value_ptr Optional integer value to attach to this checkbox.  When the 
	checkbox is checked or unchecked, *value_ptr will also be changed. ("Live Vars").
	@param id Optional ID number, to pass to the optional callback function.
	@param callback Optional callback function, taking either the int ID or control.
	*/
	inline CheckBox(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }
	explicit CheckBox(GLUI_ARGS(="",)){ GLUI_INIT }
	inline CheckBox(){ _members_init(); }
	inline void _members_init()
	{
		w              = 100;
		//h              = UI_DEFAULT_HEIGHT;		
		x_lr           = 18; //text_x_offset
		placement      = LEFT;	
	}
};

/************************************************************/
/*                                                          */
/*                     Column class                         */
/*                                                          */
/************************************************************/

/**
 A UI::Column object separates all previous controls
 from subsequent controls with a vertical bar.
*/
class UI::Column : public Interface<Column,Control>
{
public: //Interface methods

	inline void _draw()
	{
		if(get_type()) draw_etched_bars(-1,0,-1,h,-1);
	}

public:

	inline void set_type(int new_type=NONE)
	{
		//NOTE: set_int_val(new_type) is computational overkill.
		//Setting int_val by itself violates normative practice.
		//int_val = new_type;
		extra_data.int_union = new_type;
	}
	inline int get_type(){ return extra_data.int_union; }

public:
	
	/**
	 Create a new column, which separates the previous controls
	 from subsequent controls. 
	 @param parent The panel our object is inside; or the main UI object.
	 @param draw_bar If true, draw a visible bar between new and old controls.
	*/
	explicit Column(Node *parent=NULL, int type=ETCHED)
	{
		set_type(type); //int_val = draw_bar;
		
		w            = 0;
		h            = 0; 
		can_activate = false;
		placement    = CENTER;

		set_parent(parent);
	}
};

/************************************************************/
/*                                                          */
/*             Box_Inteface class (abstract)                */
/*                                                          */
/************************************************************/

	//Box_Interface standardizes labels and scrollbars.
	//It may be merged back into Control.

class Box_Interface : public UI::Control
{
public: //Interface methods

	LINKAGE void _draw();	
	LINKAGE void _update_size();
	
public:

	int box_type; /*NONE, ETCHED, RAISED, BOX, ROLLOUT*/

public:

	inline void set_type(int new_type=NONE)
	{
		//set_int_val(new_type);
		box_type = new_type; resize();
	}

	inline void expand(){ alignment = ALIGN_EXPAND; }

	inline bool box_test(int x, int y)
	{
		x-=x_abs; y-=y_abs; 
		return x>x_lr&&x<w-x_rl&&y>y_off_top&&y<h-y_off_bot;		
	}
	
	inline int get_box_width()
	{
		int bud = 0; //RIGHT_ALIGN
		if(Control*ch=first_child()) if(!ch->hidden)
		bud = ch->w+ch->x_lr+ch->x_rl; //Hotzone?
		return std::max(w-x_lr-x_rl-2*UI_BOXINNERMARGINX-bud,0);
	}

public:

	inline Box_Interface():box_type(BOX){}
};

/************************************************************/
/*                                                          */
/*                 Panel class (container)                  */
/*                                                          */
/************************************************************/

/**
 A UI::Panel contains a group of related controls.
*/
class UI::Panel : public Interface<Panel,Box_Interface>
{
public:

	/**
	 Create a new panel.  A panel groups together a set of related controls.
 
	  @param parent The outer panel our panel is inside; or the main UI object.
	  @param name The string name at the top of our panel.
	  @param type Optional style to display the panel with--ETCHED by default.
	  RAISED causes the panel to appear higher than the surroundings.
	  NONE causes the panel's outline to be invisible.
	*/
	explicit Panel(GLUI_ARGS(="", int type=ETCHED,)){ box_type = type; GLUI_INIT; }
	template<class LV>
	inline Panel(GLUI_ARGS(,int type, LV *live_var,)){ box_type = type; GLUI_LIVE; }	
	inline Panel(){ _members_init(false); }
	inline void _members_init(bool bt=true)
	{	
		w             = 300;
		h             = UI_DEFAULT_HEIGHT + 7;				
		is_container  = true; 
		can_activate  = false;						
		placement     = TOP|LEFT|BEHIND;
		if(!bt) //Old, safer, super-class's default.
		{
			alignment = CENTER;
			box_type  = ETCHED;
		}
		else if(box_type) //NEW Panel class default.
		{
			alignment = ALIGN_EXPAND; //ALIGN_CENTER
		}
		else //NONE
		{
			//The rationale for changing how NONE works is to let it be used to
			//insert groups of columns into columns, i.e. for one row only in a
			//larger column.
			alignment = LEFT;
		}	   		
	}	
};

/************************************************************/
/*                                                          */
/*               Rollout class (container)                  */
/*                                                          */
/************************************************************/
/**
 A rollout contains a set of controls,
 like a panel, but can be collapsed to just the name.
*/
class UI::Rollout : public Interface<Rollout,Panel>
{
public: //Interface methods
		
	LINKAGE void _draw();
	LINKAGE void _update_size();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);

public:

	Node collapsed_node;
	
public:

	inline void open(){ _set_is_open(true); }
	inline void close(){ _set_is_open(false); }

public:

	//REMOVE ME
	#ifdef GLUI_GLUI_H___NODEPRECATED
	/**
	 Create a new rollout.  A rollout contains a set of controls,
	 like a panel, but can be collapsed to just the name.
 
	  @param parent The panel our object is inside; or the main UI object.
	  @param name String to show at the top of the rollout.
	  @param open Optional boolean.  If true (the default), the rollout's controls are displayed.
	  If false, the rollout is closed to display only the name.
	  @param type Optional style to display the panel with--ETCHED by default.
	  RAISED causes the panel to appear higher than the surroundings.
	  NONE causes the panel's outline to be invisible.
	*/	
	inline Rollout(Node *parent, C_String name, C_Bool open=true, int type=ETCHED)
	{
		//NOTE: Historically Rollout worked with add_panel, etc.
		//Other than that, they've nothing in common.
		Panel *p = this; (void)p;

		int id = -1; CB cb = CB();
		GLUI_INIT; is_open = open!=0; (void)type; //UNUSED
	}
	#else	
	explicit Rollout(GLUI_ARGS(="",C_Bool open=true,))
	{
		GLUI_INIT; is_open = open!=0;
	}
	#endif
	inline Rollout(){ _members_init(); }
	inline void _members_init()
	{	
		//h                = UI_DEFAULT_HEIGHT + 7;
		h                = UI_BUTTON_HEIGHT-2; 
		w                = UI_DEFAULT_WIDTH;
		y_off_top        = UI_BUTTON_HEIGHT+UI_YOFF+1;
		y_off_bot        = UI_YOFF-3;
		x_lr             = x_off;
		x_rl             = x_off;
		collapsible      = &collapsed_node;
		can_activate     = true; 
		alignment        = ALIGN_EXPAND; 
		placement        = CENTER|BEHIND;

		//_pack uses this to not double UI_ITEMSPACING.
		box_type         = ROLLOUT;
	}
};


/************************************************************/
/*                                                          */
/*               Spin_Interface class (abstract)            */
/*                                                          */
/************************************************************/

	//2019: This (new) class attempts to unify the Spinner
	//and ScrollBar implementations. So far it constitutes
	//the code that is identical. It's unclear why the two
	//classes differ at all. See glui_spinner.cpp comments.

class Spin_Interface : public UI::Control
{
public: //Interface methods
	
	LINKAGE bool _special_handler(int key, int modifiers);	
	inline bool _needs_idle()
	{
		//return state==STATE_UP||state==STATE_DOWN;
		return state!=STATE_NONE;
	}
	inline Control *_wheel(){ return this; }

public:

	//PACKING DATA
	int data_type;
	int state;	
	bool horizontal;	

	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ double speed,user_speed; };
	#else
	double speed;
	#endif
	
	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ double float_min, int_min, val_start; };
	union{ double float_max, int_max, val_end; };
	#else
	double val_start, val_end;
	#endif

	UI::CB object_cb; union
	{
		Control *associated_object;	
		NodePtr<Text_Interface> ti;
		NodePtr<List_Interface> li;
		NodePtr<UI::TextBox> textbox;
		NodePtr<UI::EditText> edittext;
	};
	inline void set_object_callback(UI::CB cb=UI::CB(), UI::Control*obj=NULL)
	{
		set_object(); object_cb = cb; associated_object = obj; 
	}
	inline void set_object() /* A simple notification system. */
	{
		Control *swap = associated_object; associated_object = NULL;		
		if(dynamic_cast<Control*>((Node*)swap)) swap->update_size();
	}

public:
	  
	enum
	{
	VERTICAL=0, HORIZONTAL, 
	
	STATE_NONE=0, STATE_UP,STATE_DOWN,STATE_BOTH, STATE_SCROLL,	

	/********** Data types - for constraining text input *********/
		
	SPIN_INT=UI_EDIT_INT, SPIN_FLOAT=UI_EDIT_FLOAT,	
	};

	LINKAGE void do_callbacks();
	
	LINKAGE void set_range(double start=0, double end=0);

	inline double val_min(){ return std::min(val_start,val_end); }
	inline double val_max(){ return std::max(val_start,val_end); }

	template<class T> inline void set_val(T val)
	{
		if(float_val!=val) if(data_type==SPIN_INT)
		set_int_val((int)val); else set_float_val((double)val);
	}
	
	inline void set_speed(double scale_factor){ speed = scale_factor; };

	//TODO: The "growth" should deplete with elapsed time.
	//And somehow wheel events should manage reset_growth.

	/** These return GLUI::_spin_growth */
	inline double &_reset_growth(){ return _growth(-1000); }
	inline double &_increase_growth(){ return _growth(+1); }
	LINKAGE double &_growth(int inc=0);

protected:

	inline Spin_Interface()
	{
		state = STATE_NONE;
		data_type = SPIN_INT;
		speed = 1;
		horizontal = false;

		associated_object = NULL;
	}
	inline ~Spin_Interface(){ set_object(); }
};

/************************************************************/
/*                                                          */
/*               ScrollBar class - JVK                      */
/*                                                          */
/************************************************************/
 
class UI::ScrollBar : public Interface<ScrollBar,Spin_Interface>
{
public: //User draw methods

	//Opening up arrow/box methods and making scroll not draw
	//the box.
	LINKAGE void draw_scroll_box(int x, int y, int w, int h);
	LINKAGE void draw_scroll_arrow(int placement, int x, int y);
	LINKAGE void draw_scroll(int x, int y, int w, int h, int shadow);
	
public: //Interface methods
	
	LINKAGE void _draw();	
	LINKAGE void _update_size();
	LINKAGE void _update_live();	
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);	
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	inline bool _activate(int how)
	{
		switch(how)
		{
		case ACTIVATE_TAB:
		if(associated_object) return false;		
		case ACTIVATE_OTHER: _reset_growth();
		default: return true;
		}
	}
	inline void _idle(){ do_click(); }

public:
	
	/*See glui_scrollbar.cpp
	//https://github.com/libglui/glui/commit/de688fd
	double last_update_time; ///< GLUI_Time we last advanced scrollbar.
	double velocity_limit; ///< Maximum distance to advance per second.
	int box_length, box_start_position, box_end_position, track_length;
	*/

public:

	LINKAGE void do_click();
	LINKAGE void do_drag(int x, int y);	
	LINKAGE int find_arrow(int local_x, int local_y); 	

	#ifdef GLUI_GLUI_H___NODEPRECATED 
	inline void set_int_limits(int l, int h, int=0){ set_range(l,h); }	
	inline void set_float_limits(double l, double h, int=0){ set_range(l,h); }
	#endif

	/** NEW: List control had implemented this logic incorrectly. */
	inline void show_item(int i, int iviz)
	{
		if(i>=int_val+iviz) i-=iviz-1;
		else if(i>=int_val) return; assert(i>=0); set_int_val(i);
	}

public:

	// Constructor, no live variable
	explicit ScrollBar(GLUI_ARGS(="", int horz_vert=HORIZONTAL, int spin_type=SPIN_INT,))
	{	
		data_type = spin_type;
		horizontal = horz_vert==HORIZONTAL; GLUI_INIT
	}
	template<class LV>
	inline ScrollBar(GLUI_ARGS(,int horz_vert, LV *live_var,))
	{
		horizontal = horz_vert==HORIZONTAL; GLUI_LIVE 
	}	
	inline ScrollBar(){ _members_init(); }
	LINKAGE void _members_init();

private:

	/** update_size (update_scroll_parameters)  */
	inline int _box_len(),_box_pos(),_track_len();
};
struct Meta::ScrollBar : UI::ScrollBar{ /*workaround*/ };

/************************************************************/
/*                                                          */
/*            Text_Inteface class (abstract)                */
/*                                                          */
/************************************************************/

class Text_Interface : public Box_Interface
{		
public: //User draw methods
	
	inline void update_and_draw_text()
	{
		update_substring_bounds(); redraw();
	}	

	LINKAGE void draw_substring(int y=-1); //draw_text
		
public: //Interface methods
	
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);	
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_over(bool state, int local_x, int local_y);
	LINKAGE bool _key_handler(int key, int modifiers);	
	LINKAGE bool _special_handler(int key, int modifiers);	
	LINKAGE bool _activate(int how);
	inline Control *_wheel(){ return first_child(); }

public:

	/*substring that gets displayed in box*/
	int substring_start,substring_end;  

	#ifdef GLUI_GLUI_H___NODEPRECATED
	int sel_start; union{ int sel_end,insertion_pt; };
	#else
	int sel_start,sel_end;  /* current selection */    
	#endif

	int tab_width;

	int return_input_model;
	int data_type;

	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ double float_low,int_low, limits1; };
	union{ double float_high,int_high, limits2; };
	#else
	double limits1,limits2;
	#endif
	int has_limits;

public:
	
	inline int get_line_limit()
	{
		if(data_type!=UI_EDIT_TEXT)
		return 1;
		return has_limits?(int)limits1:INT_MAX;
	}
	inline int get_char_limit()
	{
		if(data_type==UI_EDIT_TEXT&&has_limits)
		return (int)limits2; return INT_MAX;
	}

	inline int scrollbar_val() //start_line
	{
		return scrollbar?scrollbar->int_val:0;
	}
	inline int scrollbar_bot()
	{
		return scrollbar?(int)scrollbar->val_start:0;
	}
	/** May returns 0 if single-line; or may return 1 also. */
	inline int get_page_size()
	{
		assert(font.height); //zero divide
		return (h-y_off_top-y_off_bot)/font.height; 
	}
	/** May returns 0 if single-line; includes blank lines. */
	inline int approx_line_count()
	{
		return scrollbar_bot()+get_page_size();
	}	
	inline int fast_line_count() //More code, faster count.
	{
		int c = approx_line_count();
		return c<=1?1:scrollbar_bot()?c:find_line_count();
	}
	inline int find_line_count() //Less code, slower count.
	{
		assert(font.height); //zero divide
		int _,y;_find_insertion_pt(&_,&y,(int)text.size());
		return y/font.height+1;
	}

	inline int get_line_height(){ return font.height; }
	
	inline const char *get_text(){ return text.c_str(); }

	inline int get_char_count(){ return (int)text.size(); }
	 
	inline int sel_min(){ return std::min(sel_start,sel_end); }
	inline int sel_max(){ return std::max(sel_start,sel_end); }

	//EXPERIMENTAL
	/*enum{ SHIFT=1, CTRL=2, ALT=4 };*/
	inline void key_in(int a, int mod=0)
	{
		#ifdef GLUI_READLINE_MODIFIERS
		key_handler(-a,mod);
		#else
		key_handler(+a,mod);
		#endif
	}
	//There's actually two ways to select-all. 
	//This might not be what the user desires.
	//inline void select_all(){ key_in('a',2); }

	/**WORK IN PROGRESS (\r is unimplemented) 
	 Sets return_input_model to a "new-line" character sequence.
	 NULL disables multi-line input and uses UI::ACTIVATE_OTHER.
	 NOTE: Internally \n, \r and \r\n are handled automatically.
	 Other values are can be interpreted with keyboard_callback.
	*/
	inline void set_newline(const char *newline=NULL)
	{
		int im = UI::ACTIVATE_OTHER; if(newline) 
		{
			im = *newline; if(*++newline) im|=*newline<<8;

			assert((im&8)!='\r'); //UNIMPLEMENTED
		}
		return_input_model = im;
	}

	LINKAGE bool update_substring_bounds();

	/** find_insertion_xy is draw_insertion_pt */		
	inline int find_insertion_xy(int *x, int *y)
	{
		return sel_end<0?-1:_find_insertion_pt(x,y,sel_end);
	}
	int find_insertion_pt(int x, int y, bool held_down=false)
	{
		return _find_insertion_pt(&x,&y,held_down?-3:-2);
	}	
	LINKAGE int _find_insertion_pt(int *x, int *y, int mode);	

	//2019 changes:
	//*If going backward start must be manually advanced (previously
	//2 was subtracted from it.
	//*The ":.," separators are removed. They normally are joined by
	//spaces in English. Slashes are added to the remaining - hyphen.
	LINKAGE int find_word_break(int start, int dir);

	inline int substring_wrap(int start, int end)
	{
		int brk = find_word_break(--end,-1); return start>=brk?end:brk;
	}	
	LINKAGE bool substring_wrap_compare(int string_width, int box_width);
	/*{
		//This common comparison must be pixel perfect across algorithms.

		return string_width>box_width-4; //-4: Matches right/left margin.
	}*/
	
	inline int substring_width(int i=0)
	{
		//return substring_width(substring_start,substring_end);
		return substring_width(substring_start,substring_end,i);
	}
	inline int substring_width(int start, int end)
	{
		int i,o = 0; for(i=start;i<end;i++)
		o+=char_width(text[i]); return o;
	}
	inline int substring_width(int start, int end, int initial_width)
	{
		for(int i=start;i<end;i++)
		initial_width+=substring_tab_width(initial_width,i); 
		return initial_width;
	}
	inline int substring_tab_width(int initial_width, int i)
	{
		i = text[i]; if(i=='\t'&&tab_width) //zero divide
		{
			return tab_width-initial_width%tab_width;
		}
		return char_width(i);
	}
	
	inline int substring_pop_back(){ return char_width(text[--substring_end]); }	
	inline int substring_push_back(){ return char_width(text[substring_end++]); }
	inline int substring_pop_front(){ return char_width(text[substring_start++]); }
	inline int substring_push_front(){ return char_width(text[--substring_start]); }

	enum
	{
	/***** Limit types - how to limit spinner values ********/
	
	LIMIT_NONE=0, LIMIT_CLAMP=1, LIMIT_WRAP=2, 

	/********** Data types - for constraining text input *********/

	EDIT_INT=UI_EDIT_INT, EDIT_FLOAT=UI_EDIT_FLOAT,
	};
	#ifdef GLUI_GLUI_H___NODEPRECATED 
	inline void set_int_limits(int l, int h, int type=1){ set_limits(l,h,type); }
	inline void set_float_limits(double l, double h, int type=1){ set_limits(l,h,type); }
	#endif
	LINKAGE void set_limits(double limits1, double limits2, int limit_type=LIMIT_CLAMP);
	inline void set_limits(){ return set_limits(0,0,LIMIT_NONE); }

	GLUI_DEBUG_STDOUT__void_dump(const char *text);

public:

	inline Text_Interface()
	{
		tab_width             = 0;
		return_input_model    = UI::ACTIVATE_OTHER;

		substring_start       = 0;
		substring_end         = 0; //2; //???
		sel_start             = 0;
		sel_end               = 0;

		data_type             = UI_EDIT_TEXT;
		has_limits            = LIMIT_NONE;	
		limits1               = 0;
		limits2               = 0;
	}
	
protected:

	void _CLAMP_substring();
	void _kh_erase(int,int);
	void _evaluate_limits();
};

/************************************************************/
/*                                                          */
/*                   EditText class                         */
/*                                                          */
/************************************************************/

class UI::EditText : public Interface<EditText,Text_Interface>
{
public: //User draw methods

	inline void draw_insertion_pt()
	{
		pos_caret(h-y_off_bot-y_off_top,UI_BOXINNERMARGINX);
	}

public: //Interface methods

	LINKAGE void _draw();
	LINKAGE void _update_size();
	LINKAGE void _update_live();
	LINKAGE bool _key_handler(int key, int modifiers);	
	LINKAGE bool _special_handler(int key, int modifiers); 

public:

	inline void right(){ alignment = ALIGN_RIGHT; }

public:

	//REMOVE ME
	#ifdef GLUI_GLUI_H___NODEPRECATED	
	inline EditText(GLUI_ARGS(,String &legacy,))
	{
		typedef String LV; LV *live_var = &legacy; GLUI_LIVE 
	}			
	// Deprecated constructor, only called internally
	// NOTICE: UI_EDIT_STRING and UI_EDIT_DOUBLE won't
	// be assigned to the data_type field because it's
	// really an input-model.
	inline EditText(GLUI_ARGS(,int edit_type, void *live_var,))
	{
		GLUI_INIT; data_type = _live_init_text_type(edit_type,live_var);
	}
	#endif
	// Constructor, no live variable
	explicit EditText(GLUI_ARGS(="", int edit_type=UI_EDIT_TEXT,))
	{	
		GLUI_INIT; data_type = edit_type;
	}
	template<class LV>
	inline EditText(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }	
	inline EditText(){ _members_init(); }
	inline void _members_init()
	{	
		h = UI_EDITTEXT_HEIGHT;
		w = UI_EDITTEXT_WIDTH;
		spacebar_mouse_click  = false;
				
		x_lr                  = 55; //text_x_offset	
		int dt; switch(live_type) 
		{
		case UI_LIVE_INT: dt = EDIT_INT; break;
		case UI_LIVE_FLOAT:
		case UI_LIVE_DOUBLE: dt = EDIT_FLOAT; break;
		default: dt = UI_EDIT_TEXT;
		has_limits            = LIMIT_CLAMP;	
		limits1               = 1;
		limits2               = DBL_MAX;
		}
		data_type             = dt;							
	}	
};

/************************************************************/
/*                                                          */
/*                     Spinner class                        */
/*                                                          */
/************************************************************/
 
class UI::Spinner : public Interface<Spinner,Spin_Interface>
{
public: //Interface methods

	LINKAGE void _draw();
	LINKAGE void _update_live();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);	
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);	
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	inline bool _activate(int how)
	{
		switch(how)
		{
		case ACTIVATE_TAB:
		if(associated_object==_parent_node) return false;		
		case ACTIVATE_OTHER: _reset_growth();
		default: return true;
		}
	}
	inline void _idle(){ do_click(); }
		
public:

	inline void right(){ alignment = ALIGN_RIGHT; }

	LINKAGE void do_click();
	LINKAGE void do_drag(int x, int y);	
	LINKAGE int find_arrow(int local_x, int local_y);

	inline const char *get_text(){ return edittext->text.c_str(); }

	enum /***** Limit types - how to limit spinner values ********/
	{
		//Note: glui_edittext.cpp implements these

		LIMIT_NONE=0, LIMIT_CLAMP=1, LIMIT_WRAP=2, 
	};
	#ifdef GLUI_GLUI_H___NODEPRECATED 
	inline void set_int_limits(int l, int h, int type=1){ set_limits(l,h,type); }	
	inline void set_float_limits(double l, double h, int type=1){ set_limits(l,h,type); }
	#endif
	inline void set_limits(double start, double end, int limit_type=LIMIT_CLAMP)
	{
		set_range(start,end);
		edittext->set_limits(val_min(),val_max(),limit_type);
	}
		
public:

	// Constructor, no live variable
	explicit Spinner(GLUI_ARGS(="", int spin_type=SPIN_INT,))
	{	
		GLUI_INIT; data_type = spin_type; 
	}	
	// Deprecated constructor, only called internally
	// See EditText's comment.
	inline Spinner(GLUI_ARGS(,int edit_type, void *live_var,))
	{
		GLUI_INIT; data_type = _live_init_text_type(edit_type,live_var); 		
	}
	template<class LV>
	inline Spinner(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }	
	inline Spinner(){ _members_init(); }
	inline void _members_init()
	{
		h            = UI_EDITTEXT_HEIGHT;
		w            = UI_SPINNER_ARROW_WIDTH+4;
		y_off_top    = -2;
		x_lr         = -2;
		x_rl         = -2;
		y_off_bot    = -2;

		//REMINDER: Don't do this. This is the alignment of
		//the text that is created by the constructor. It's
		//sourced via ALIGN_BUDDY. It seems backward for it
		//to work this way. But it's a simple system, owing
		//to historical happenstance.
		//alignment    = ALIGN_RIGHT;

		data_type    = SPIN_INT; 
		switch(live_type) 
		{
		case UI_LIVE_FLOAT: 
		case UI_LIVE_DOUBLE:
		data_type   = SPIN_FLOAT;		
		}
	}
	inline void _dynamic_init(Node* &parent)
	{
		//set_parent(parent);

		//REMINDER: sync_live will break if the live-pointer
		//is assigned to the text control. It can't consider
		//both.
		UI::EditText *et = new 
		UI::EditText(/*this*/parent,name,data_type,id,callback);		
		et->alignment = ALIGN_BUDDY; //NEW		
		set_parent(associated_object=et); parent = NULL;
	}
};
struct Meta::Spinner : UI::Spinner{ /*workaround*/ };

/************************************************************/
/*                                                          */
/*               TextBox class - JVK                        */
/*                                                          */
/************************************************************/

class UI::TextBox : public Interface<TextBox,Text_Interface>
{
public: //User draw methods

	inline void draw_insertion_pt()
	{
		int lh = get_line_height();
		int x = UI_BOXINNERMARGINX;
		pos_caret(lh,x,x-lh*scrollbar_val());
	}

public: //Interface methods

	LINKAGE void _draw();	
	LINKAGE void _update_size();
	LINKAGE void _update_live();					
	
public:
	
	inline void set_tab_w(int w=0) /** 0 uses internal default. */
	{
		assert(w>=0); tab_width = -w; update_size(); //Redraw, etc.
	}

	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline void set_start_line(int l){ scrollbar->set_int_val(l); }
	#endif
	
public:

	#ifdef GLUI_GLUI_H___NODEPRECATED	
	//REMOVE ME (PARTICULARLY BAD)
	inline TextBox(Node *parent, String &legacy, C_Bool scroll=false, int id=-1, CB cb=CB())
	{
		typedef String LV; LV *live_var = &legacy;

		C_String name/* = "textbox"*/; GLUI_LIVE; GLUI_SBAR;
	}
	inline TextBox(Node *parent, C_Bool scroll=false, int id=-1, CB cb=CB())
	{
		C_String name/* = "textbox"*/; GLUI_INIT; GLUI_SBAR;
	}	
	#endif //#else (Breaking change: String& is ambiguous.)
	explicit TextBox(GLUI_ARGS(="", C_Bool scroll=false,)){ GLUI_INIT; GLUI_SBAR; }	
	//#endif
	template<class LV>
	inline TextBox(GLUI_ARGS(,C_Bool scroll, LV *live_var,)){ GLUI_LIVE; GLUI_SBAR; }
	inline TextBox(){ _members_init(); }		
	inline void _members_init() //GLUI_SBAR
	{
		h                     = UI_TEXTBOX_HEIGHT;
		w                     = UI_TEXTBOX_WIDTH;
		spacebar_mouse_click  = false;
		tab_navigate          = false;		
		placement             = BOTTOM|LEFT;

		//tab_width             = UI_TAB_WIDTH; //4*char_width(' ');
		return_input_model    = '\n';
	}
};

/************************************************************/
/*                                                          */
/*               CommandLine class                          */
/*                                                          */
/************************************************************/

class UI::CommandLine : public Interface<CommandLine,EditText>
{	
public: //Interface methods

	LINKAGE bool _key_handler(int key, int modifiers);
	LINKAGE bool _special_handler(int key, int modifiers);
	LINKAGE bool _activate(int);

public:

	#ifdef GLUI_COMMANDLINE_HIST_SIZE
	enum{ HIST_SIZE = GLUI_COMMANDLINE_HIST_SIZE }; 
	#else
	enum{ HIST_SIZE = 64 }; //100
	#endif

	//2019: The way this was implemented is pretty unintuitive
	//and dangerous. oldest_hist is basically the 0th index in
	//the vector, but it continues to go upward. So you cannot
	//access an index less than it. After it exceeds HIST_SIZE
	//add_to_history does erase(begin()); push_back(""); Since
	//move-semantics were added to C++ containers this is only
	//shifting the entire vector. Prior it would've been a bad
	//memory stategy to say the least. I've tried to implement
	//it as a circular buffer to not have to shift. I think if
	//it had a ListBox feature it could work as a location-bar.

	String hist_list[HIST_SIZE];
	int curr_hist;
	int oldest_hist; 
	int newest_hist;
	//PACKING DATA	
	//2019: add_to_history uses this size.
	short _add_to_history_HIST_SIZE; 
	int oldest_hist_wrap; 

public:

	inline const char *get_history(int command_number)
	{
		//2019: This code demonstrates the earlier methodology.
		//return hist_list[command_number-oldest_hist].c_str();
		return get_history_str(command_number).c_str(); 
	}
	inline String &get_history_str(int command_number)
	{			
		//Note: This makes this a little saner for users
		//to interact with since it can't go out of bounds.
		int i = command_number-oldest_hist+oldest_hist_wrap;
		unsigned int hist_size = //micro-optimization
		INTERNAL_H?_add_to_history_HIST_SIZE:HIST_SIZE;
		return hist_list[(unsigned int)i%hist_size];		
	}

	/**WARNING: displacing_command will not be added to the
	history... its only purpose is to be tested for being an
	empty string, or (since 2019) matching the newest item in
	the history. In both cases the returned value is false and
	the history is unamended. */
	LINKAGE bool add_to_history(C_String displacing_command);
	inline void reset_history(){ _members_init(); }
	inline bool scroll_history(int direction)
	{
		return recall_history(curr_hist+direction);
	}
	LINKAGE bool recall_history(int history_number);	

	GLUI_DEBUG_STDOUT__void_dump(const char *text);
	
public:

	//REMOVE ME
	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline CommandLine(GLUI_ARGS(,void *lv,))
	{
		typedef char LV; LV *live_var = (LV*)lv; 
		
		if(lv){ GLUI_LIVE }else{ GLUI_INIT }
	}	
	#endif
	template<class LV>
	inline CommandLine(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }	
	explicit CommandLine(GLUI_ARGS(="",)){ GLUI_INIT }
	inline CommandLine(){ _members_init(); }
	inline void _members_init()
	{	
		//REMINDER: This implements reset_history.
		curr_hist   = 0;
		oldest_hist = 0;
		newest_hist = 0;
		
		//2019
		//hist_list.resize(HIST_SIZE); 
		oldest_hist_wrap = 0; 
		_add_to_history_HIST_SIZE = HIST_SIZE;
		assert(HIST_SIZE==_add_to_history_HIST_SIZE);
	}
};

/************************************************************/
/*                                                          */
/*                      RadioGroup class                    */
/*                                                          */
/************************************************************/

/**2019 addition
 This is for back-compat with RadioButton::group, that was a
 second pointer to its parent.
 It might be useful for RadioButton to have a mode that does
 not nest under a parent node. It would want to use group_id.
 */
class Group_Interface : public UI::Control
{
public: //Interface methods

	//TODO: Make this recursive?
	LINKAGE bool _special_handler(int key, int modifiers);
	inline Control *_wheel(){ return this; }
	inline void _draw(){ /*NOP*/ }

public:

	inline void set_selected(int radio_button_id=-1)
	{
		set_int_val(radio_button_id); 
	}

	inline int make_id(int id=-1)
	{	
		if(id==-1) if(Node*last=last_child())
		{
			id = ((Control*)last)->id+1;
		}
		else id = 0; return id;
	}

public:

	inline Group_Interface()
	{
		can_activate  = false;
		is_container  = true; 	
		placement     = BOTTOM|LEFT;
	}
};
class UI::RadioGroup : public Interface<RadioGroup,Group_Interface>
{
public: //Interface methods

	//TODO: Make this recursive?
	LINKAGE void _update_live();
	
public:

	inline RadioGroup(Node *parent, int id=-1, CB cb=CB())
	{
		C_String name; GLUI_INIT;
	}	
	inline RadioGroup(Node *parent, int *live_var, int id=-1, CB cb=CB())
	{
		C_String name; typedef int LV; GLUI_LIVE;
	}	
	template<class LV>
	inline RadioGroup(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }	
	explicit RadioGroup(GLUI_ARGS(="",)){ GLUI_INIT }
	inline RadioGroup(){ _members_init(); }
	inline void _members_init()
	{	
		w             = 300;
		h             = 300;
	}
};

/************************************************************/
/*                                                          */
/*                    RadioButton class                     */
/*                                                          */
/************************************************************/

class UI::RadioButton : public Interface<RadioButton,Control>
{	
public: //Interface methods

	LINKAGE void _draw();
	LINKAGE void _update_size();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);
	inline Control *_wheel(){ return parent(); }

public:

	inline Group_Interface *parent()
	{
		return (Group_Interface*)_parent_node; 
	}
	
public:

	explicit RadioButton(Group_Interface *group, C_String label, int id=-1)
	{
		_members_init();

		Control::id = group->make_id(id);

		name = label.str; set_parent(group);
	}		
	inline RadioButton(){ _members_init(); }
	inline void _members_init()
	{
		//h              = UI_DEFAULT_HEIGHT;				
		x_lr           = 18; //text_x_offset
		placement      = LEFT;	
	}
};

/************************************************************/
/*                                                          */
/*                     Separator class                      */
/*                                                          */
/************************************************************/

class UI::Separator : public Interface<Separator,Control>
{
public: //Interface methods

	LINKAGE void _draw();
		
public:

	inline void set_type(int new_type=NONE)
	{	
		extra_data.int_union = new_type;
	}
	inline int get_type(){ return extra_data.int_union; }

public:

	explicit Separator(Node *parent=NULL, int type=ETCHED)
	{
		set_type(type); //int_val = draw_bar;

		w            = 0; //100;
		h            = UI_SEPARATOR_HEIGHT;
		placement    = CENTER;
		can_activate = false;
		
		set_parent(parent);
	}
};

/************************************************************/
/*                                                          */
/*               StaticText class                           */
/*                                                          */
/************************************************************/

class UI::StaticText : public Interface<StaticText,Control>
{
public: //User draw methods

	inline void draw_text(){ draw_name(); }

public: //Interface methods

	inline void _draw(){ draw_text(); }
	LINKAGE void _update_size();
	inline void _update_live(){ name = text; }

public:
		
	explicit StaticText(Node *parent=NULL, C_String text="")
	{
		//h             = UI_DEFAULT_HEIGHT;		
		can_activate  = false;
		placement     = BOTTOM|LEFT;

		if(text.str) name = text.str; set_parent(parent); 
	}
};

/************************************************************/
/*                                                          */
/*                List_Inteface class (abstract)            */
/*                                                          */
/************************************************************/

class List_Interface : public Box_Interface
{	
public:	//Note: This doesn't have to use Interface.
		//Multiple inheritance would also work, if
		//list classes have nothing more in common.
		
	class Item : public Node 
	{
	public:

		int id; String text;

	public:
		
		Item(int id=-1, C_String text=""):id(id),text(text.str)
		{}

		inline Item *prev(){ return (Item*)_prev_sibling; }
		inline Item *next(){ return (Item*)_next_sibling; }

		inline bool operator==(int cmp){ return id==cmp; }				
		inline bool operator==(Item *i){ return this==i; }
		inline bool operator==(C_String cmp)
		{
			return cmp.str?text==cmp.str:text.empty(); 
		}
		template<class T>
		inline bool operator!=(const T &cmp){ return !operator==(cmp); }

		template<class T> 
		inline Item *_find_ptr(const T &cmp)
		{
			Item *i; for(i=this;i&&*i!=cmp;i=i->next()); return i; 
		}		

		template<class T> 
		inline int _count(const T &cmp)
		{
			Item *i = this; for(int j=0;i;j++) if(*i!=cmp)
			{
				i = i->next(); j++;
			}
			else return j; return -1; 
		}		
		inline Item *_line(size_t l)
		{
			Item *i; for(i=this;i&&l-->0;i=i->next()); return i; 
		}
	};

	Node items_list;

	inline void _delete_all()
	{
		items_list.~Node(); new(&items_list) Node;
	}

public:
	 
	inline Item *first_item()
	{
		return (Item*)items_list.first_child(); 
	}
	inline Item *last_item()
	{
		return (Item*)items_list.last_child(); 
	}

	template<class T>
	inline Item *get_item_ptr(const T &cmp)
	{
		Item *i = first_item(); 
		for(;i&&*i!=cmp;i=i->next()); return i; 
	}
	
	template<class I>
	inline Item *get_line_ptr(const I &line)
	{
		Item *i = first_item();
		size_t l = (size_t)line;
		for(;i&&l-->0;i=i->next()); return i; 
	}

	template<class T>
	inline int get_line_num(const T &cmp)
	{
		Item *i = first_item();
		for(int j=0;i;j++) if(*i!=cmp)
		{
			i = i->next(); j++;
		}
		else return j; return -1; 
	}

	template<class T>
	/**
		DOES NOT RESET THE STATE OF THIS CONTROL. 
	*/	
	inline void sort_items(bool(*pred)(T*,T*)=_text_sort)
	{
		Item &upcast = *(T*)NULL; (void)upcast;
		items_list._sort(pred,1);
	}
	static bool _text_sort(Item *a, Item *b){ return a->text<b->text; }
};
template<class S>
/**
 * This template extends the Interface template for the classes
 * that have some more methods in addition to the Control class.
 */
class Interface<LI,S> : public Interface<S,List_Interface>
{
public:

	inline bool add_item(LI::Item *item)
	{
		return ((S*)this)->_add_item(item);
	}
	inline bool add_item(int id, C_String text)
	{
		return add_item(new LI::Item(id,text));
	}	

	inline bool insert_item(LI::Item *item, LI::Item *sib, bool before=false)
	{
		if(add_item(item)) item->_link(sib,before?3:2);
		else return false; return true;
	}

	inline bool delete_item(LI::Item *ptr)
	{
		return ((S*)this)->_delete_item(ptr);
	}
	inline bool delete_item(C_String text)
	{
		return delete_item(((S*)this)->get_item_ptr(text.str));
	}
	inline bool delete_item(int id)
	{
		return delete_item(((S*)this)->get_item_ptr(id));
	}

	inline bool delete_all()
	{
		return ((S*)this)->_delete_item(&((S*)this)->items_list);
	}
};

/************************************************************/
/*                                                          */
/*               List class - JVK                           */
/*                                                          */
/************************************************************/

  //WARNING: List doesn't use int_val to mark its selection.
  //It doesn't use it at all. Though it sets scrollbar's to
  //its selection, and uses curr_line to mark its selection.

class UI::List : public Interface<LI,List>
{
public: //User draw methods

	LINKAGE void draw_text(C_String, bool selected, int y);	

public: //Interface methods
		
	LINKAGE void _draw();
	LINKAGE void _update_size();
	LINKAGE void _update_live();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);	
	LINKAGE bool _special_handler(int key, int modifiers);	
	inline Control *_wheel(){ return first_child(); }

public:

	int start_line;
	int num_lines;
	int curr_line;
	int visible_lines;
	bool _clicked; //spacebar_mouse_click

	//REMOVE ME?
	//https://github.com/libglui/glui/pull/84/commits
	//unsigned long long _last_click_time;

	//FileBrowser uses these to get feedback.
	int cb_click_type;
	Control *associated_object; CB object_cb;	
	inline void set_object_callback(CB cb=CB(), Control *obj=NULL)
	{
		set_object(); object_cb = cb; associated_object = obj; 
	}
	inline void set_object() /* A simple notification system. */
	{
		Control *swap = associated_object; associated_object = NULL;		
		if(dynamic_cast<Control*>((Node*)swap)) swap->update_size();
	}	
	/* Seems to pertain to set_object_callback */
	inline void set_click_type(int d=SINGLE_CLICKED){ cb_click_type = d; }	

public:
	
	LINKAGE int find_line(int x, int y);
		
	#ifdef GLUI_GLUI_H___NODEPRECATED	
	inline void set_start_line(int l)
	{
		assert(l<=num_lines-visible_lines); //NEW
		start_line = l; 
		if(scrollbar) scrollbar->set_int_val(l); //NEW
	}
	#endif
	
	inline int get_current_item(){ return curr_line; }	

	LINKAGE bool _add_item(Item*),_delete_item(Node*);
	
	GLUI_DEBUG_STDOUT__void_dump(const char *text);

public:

	//NOTE: There was a String& live-variable version here, 
	//but the feature was unimplemented. It'd be ambiguous.
	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline List(Node *parent, C_Bool scroll, int id=-1, CB cb=CB())
	{
		C_String name/* = "list"*/; GLUI_INIT; GLUI_SBAR;
	}		
	#endif
	explicit List(GLUI_ARGS(="", C_Bool scroll=false,)){ GLUI_INIT; GLUI_SBAR; }
	template<class LV>
	inline List(GLUI_ARGS(,C_Bool scroll, LV *live_var,)){ GLUI_LIVE; GLUI_SBAR; }
	inline List(){ _members_init(); }
	inline void _members_init()
	{
		h                     = UI_LIST_HEIGHT;
		w                     = UI_LIST_WIDTH;
		placement             = BOTTOM|LEFT;

		num_lines             = 0;
		start_line            = 0;
		curr_line             = 0;				
		cb_click_type         = SINGLE_CLICKED;
		associated_object     = NULL;
		_clicked              = false;
	}
	inline ~List(){ set_object(); }
};

/************************************************************/
/*                                                          */
/*                   ListBox class                          */
/*                                                          */
/************************************************************/
	
class UI::ListBox : public Interface<LI,ListBox>
{
public: //Interface methods

	LINKAGE void _draw();	
	LINKAGE void _update_size();
	inline void _update_live(){ do_selection(int_val); }
	LINKAGE bool _mouse_over(bool state, int local_x, int local_y);
	LINKAGE bool _special_handler(int key, int modifiers);	
	inline Control *_wheel(){ return this; }

public:

	String curr_text;
	int glut_menu_id;
	int _last_int_val; 

public:
	
	/** Won't invoke callback. Do so manually if desired. */
	LINKAGE bool do_selection(int item);

	LINKAGE bool _add_item(Item*),_delete_item(Node*);
	
	GLUI_DEBUG_STDOUT__void_dump();

public:

	template<class LV>
	inline ListBox(GLUI_ARGS(,LV *live_var,)){ GLUI_LIVE }
	explicit ListBox(GLUI_ARGS(="",)){ GLUI_INIT }
	inline ListBox(){ _members_init(); }
	inline void _members_init()
	{
		w              = UI_EDITTEXT_WIDTH;
		h              = UI_EDITTEXT_HEIGHT     +1;		

		x_lr           = 55; //text_x_offset
		//curr_text      = "";
		glut_menu_id   = -1;
	}

	/** This is a trick to do glutDestroyMenu cleanup without complicating things. */
	inline ~ListBox(){ _delete_item(_child_head); }

protected:
	
	inline void _common_init();

	/** Change w and return true if we need to be widened to fit the current item. */
	inline bool _recalculate_item_width();
};

/************************************************************/
/*                                                          */
/*                     File Browser class                   */
/*                                                          */
/************************************************************/

/**
 A list of files the user can select from.
 */
class UI::FileBrowser : public Interface<FileBrowser,Panel>
{
public: //Interface Methods

	LINKAGE void _update_size();
	inline Control *_wheel(){ return list?list->wheel():NULL; }

public:

	List *list;
	String current_dir;
	String file;
			
	//PACKING DATA
	#ifdef GLUI_GLUI_H___NODEPRECATED	
	union{ bool allow_change_dir, click_chdir; };	
	#else
	bool click_chdir;	
	#endif
	bool lsdir_cb, chdir_cb; //Call execute_callback?
	bool current_chdir; //current_dir is chdir result.

public:
	
	LINKAGE bool lsdir(C_String="", bool chdir=false);	

	inline void chdir(C_String str){ lsdir(str,true); }

	inline const char *get_file(){ return file.c_str(); }

	inline void set_click_type(int d=DOUBLE_CLICKED)
	{
		assert(list); list->set_click_type(d); 
	}
	inline void set_click_chdir(bool y=false)
	{
		click_chdir = y; 
	}	
	inline void set_lsdir_cb(bool y=true)
	{
		lsdir_cb = y; 
	}	
	inline void set_chdir_cb(bool y=true)
	{
		chdir_cb = y; 
	}	

	#ifdef GLUI_GLUI_H___NODEPRECATED		
	inline void set_allow_change_dir(bool y=true)
	{
		click_chdir = y; //allow_change_dir = y;
	}
	inline void fbreaddir(C_String str)
	{
		lsdir(str,click_chdir); //allow_change_dir
	}
	#endif

public:

	/**
	 Create a new list of files the user can select from.
 
	  @param parent The panel our object is inside; or the main UI object.
	  @param name Prompt to give to the user at the top of the file browser.
	  @param frame_type Optional style to display the panel with--ETCHED by default.
	  RAISED causes the panel to appear higher than the surroundings.
	  NONE causes the panel's outline to be invisible.
	  @param id Optional ID number, to pass to the optional callback function.
	  @param callback Optional callback function, taking either the int ID or control.
	*/
	explicit FileBrowser(GLUI_ARGS(="", int frame_type=ETCHED,))
	{
		GLUI_INIT; box_type = frame_type;		
		list = _list_init(new List(this,"",true)); 
		#ifdef GLUI_GLUI_H___NODEPRECATED
		fbreaddir(".");
		#endif
	}
	inline FileBrowser(){ _members_init(); }
	inline void _members_init()
	{
		w             = UI_DEFAULT_WIDTH;
		h             = UI_DEFAULT_HEIGHT;			
		alignment     = CENTER;	
		placement     = BOTTOM|LEFT;
		is_container  = false; //2019

		//allow_change_dir = true;
		#ifdef GLUI_GLUI_H___NODEPRECATED		
		click_chdir = true;
		lsdir_cb = chdir_cb = false;
		#else
		click_chdir = false;
		lsdir_cb = chdir_cb = true;
		#endif

		//current_dir  = ".";
		//file        = "";		
		list         = NULL;
	}
	LINKAGE List *_list_init(List*);
};

/************************************************************/
/*                                                          */
/*              Mouse_Interface class (abstract)            */
/*                                                          */
/************************************************************/

/**
  This is the superclass of translation and rotation widgets.
*/
class Mouse_Interface : public UI::Control
{
public: //Interface methods	

	LINKAGE void _update_size();	
	LINKAGE bool _special_handler(int key, int modifiers);
	inline Control *_wheel(){ return this; }
	
	//virtual void iaction_draw(int persp) = 0;
	//virtual void iaction_mouse(int stage, int x, int y) = 0;	
	typedef void(MI::*_iaction_i)(int);
	typedef void(MI::*_iaction_iii)(int,int,int);
	inline void _draw(){ assert(0); }	
	LINKAGE void _draw(_iaction_i mf, int vpw=0, int vph=0);
	LINKAGE bool _mouse_stage(int stage, _iaction_iii mf, int lx, int ly, bool inside);

public:

	inline int x_center(){ return x_abs+w/2; }
	inline int y_center(){ return y_abs+(h-UI_MOUSE_IACTION_HEADER)/2; }
	
protected:

	inline Mouse_Interface()
	{
		w              = UI_MOUSE_IACTION_WIDTH;
		h              = UI_MOUSE_IACTION_HEIGHT;			
		alignment      = CENTER;
		placement      = CENTER|TOP;
	}
};
template<class S>
/**
 * This template extends the Interface template for the classes
 * that have some more methods in addition to the Control class.
 */
class Interface<MI,S> : public Interface<S,Mouse_Interface>
{
public: //Interface methods	

	virtual void draw()
	{
		MI::_draw((MI::_iaction_i)&S::_iaction_draw); 
	}

	virtual bool mouse_down_handler(int local_x, int local_y)
	{
		return MI::_mouse_stage(1,(MI::_iaction_iii)&S::_iaction_mouse,local_x,local_y,true);
	}	
	virtual bool mouse_held_down_handler(int local_x, int local_y, bool inside=true)
	{
		return MI::_mouse_stage(2,(MI::_iaction_iii)&S::_iaction_mouse,local_x,local_y,inside);		
	}
	virtual bool mouse_up_handler(int local_x, int local_y, bool inside=true)
	{
		return MI::_mouse_stage(3,(MI::_iaction_iii)&S::_iaction_mouse,local_x,local_y,inside);
	}

public: //User draw methods

	inline void draw_active_area(int vpw=-1, int vph=0)
	{
		MI::_draw((MI::_iaction_i)&S::_iaction_draw,vpw,vph); 
	}
};

/************************************************************/
/*                                                          */
/*                   Rotation class                         */
/*                                                          */
/************************************************************/

/**
  An onscreen rotation controller--allows the user to interact with
  a 3D rotation via a spaceball-like interface.
*/
class UI::Rotation : public Interface<MI,Rotation>
{
public: //User draw methods
	
	/* Implements old setup_texture & setup_lights APIs. */	
	inline bool texture_and_light(bool t=true, bool l=true)
	{
		return draw_ball(0,t,l);
	}	
	inline bool setup_texture(){ return draw_ball(0,true,false); }
	inline void setup_lights(){ draw_ball(0,false,true); }
	
	/** Historically this didn't do texture_and_light by default. */	
	LINKAGE bool draw_ball(float radius, bool t=false, bool l=false);

public: //Interface methods	 
	
	LINKAGE void _update_size();
	LINKAGE void _update_live();
	LINKAGE bool _needs_idle();
	LINKAGE void _idle();
	LINKAGE void _iaction_mouse(int stage, int x, int y);
	LINKAGE void _iaction_draw(int persp);
	
public:

	//PACKING DATA
	double matrix[4][4];
	class Arcball *ball; //Hide me in Extra_Data?

	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ double spin, damping; size_t can_spin; }; //bool
	#else
	double spin;
	#endif

	//NOTE: needs_idle can tell if spinning.
	//bool spinning; //Arcball::is_spinning

public:

	//UNDOCUMENTED (Resets quaternion, draws, updates live-variable.)
	LINKAGE void reset(); 
	
	//UNDOCUMENTED: (Sets damping member. 0 disables can_spin.)
	//CAUTION: 1 spins perpetually. More than 1 is treated as 1.
	LINKAGE void set_spin(double damp_factor=0); 
	
public:

	inline Rotation(GLUI_ARGS(,float live_var[16],))
	{
		typedef float LV[16]; GLUI_LIVE; 
	}
	inline Rotation(GLUI_ARGS(,double live_var[16],))
	{
		typedef double LV[16]; GLUI_LIVE; 
	}
	explicit Rotation(GLUI_ARGS(="",)){ GLUI_INIT }
	inline Rotation(){ _members_init(); }
	inline void _members_init()
	{					
		float_array_size = 4*4;
		float_array_val = matrix[0];	
		spin = 0;
		ball = NULL; reset();
	}
	virtual ~Rotation(){ _Rotation(); }	
	LINKAGE void _Rotation();

protected:

	//I think this would be automatic via live-variable if it was
	//meant to be a user interface?
	inline void _copy_ball_to_float_array();
};

/************************************************************/
/*                                                          */
/*                   Translation class                      */
/*                                                          */
/************************************************************/

/**
  An onscreen translation controller--allows the user to interact with
  a 3D translation.

  TODO: I THINK glutSpecialUpFunc IS NEEDED TO UTILIZE BOTH ARROW KEYS.
*/
class UI::Translation : public Interface<MI,Translation>
{
public: //User draw methods

	LINKAGE void draw_2d_arrows(int radius);

public: //Interface methods

	LINKAGE void _iaction_mouse(int stage, int x, int y);	
	LINKAGE void _iaction_draw(int persp);	

public:

	enum{ XY=0, Z,X,Y }; /** Translation control types */
	
	int trans_type; /* Is this an XY or a Z controller? */
		
	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ double speed,scale_factor; };
	#else
	double speed;
	#endif

public:
		
	inline void set_axes(int axes) //2019
	{
		trans_type = axes;
		float_array_size = XY==axes?2:1; //legacy
	}

	inline void set_speed(double scale_factor){ speed = scale_factor; }
	
	/* Float array is either a single float (for single-axis controls),
	or two floats for X and Y (if an XY controller) */

	inline double get_z(){ return float_array_val[0]; }
	inline double get_x(){ return float_array_val[0]; }
	inline double get_y()
	{
		return float_array_val[trans_type==XY?1:0];
	}

	/*DEPRECATE ME? back-compat*/
	inline void set_z(double val){ set_one_val(val,0); }
	inline void set_x(double val){ set_one_val(val,0); }
	inline void set_y(double val){ set_one_val(val,trans_type==XY); }
	inline void set_one_val(double val, int index)
	{
		//This needs to do GLUI::set_edited(this). It could be defined
		//after GLUI but I think it's best to take a normative pathway.
		//float_array_val[index] = val; post_live();
		double cp[2]; cp[index] = val; cp[!index] = float_array_val[!index];
		set_float_array_val(cp);
	}

public:

	explicit Translation(GLUI_ARGS(="", int axes=XY,))
	{
		GLUI_INIT; set_axes(axes);
	}
	inline Translation(GLUI_ARGS(,int axes, float live_var[2],))
	{
		typedef float LV[2]; GLUI_LIVE; set_axes(axes);
	}
	inline Translation(GLUI_ARGS(,int axes, double live_var[2],))
	{
		typedef double LV[2]; GLUI_LIVE; set_axes(axes);
	}
	inline Translation(){ _members_init(); }
	inline void _members_init()
	{
		assert(float_array_val==&extra_data.float_union-1);

		set_axes(XY); speed = 1;
	}

protected:

	inline void _draw_2d_arrow(int radius, bool filled, int orientation); 
};

#if defined(GLUI_BUILD_TREE_CONTROLS) || defined(GLUI_BUILD_EXAMPLES)

	//EXPERIMENTAL 
	//These (Tree) classes are here out of respect for 
	//their author. They can't be used without writing
	//new code to solve a multitude of problems. Share
	//new (and improved) classes with the GLUI project.

/************************************************************/
/*                                                          */
/*                   Tree class (container)                 */
/*                                                          */
/************************************************************/

/**UNFINISHED WORK
  One collapsible entry in a UI::TreePanel.
*/
class UI::Experiments::Tree : public Interface<Tree,Panel>
{
public: //User draw methods

	LINKAGE void draw_pressed();

public: //Interface methods

	LINKAGE void _draw();
	LINKAGE bool _mouse_down_handler(int local_x, int local_y);
	LINKAGE bool _mouse_up_handler(int local_x, int local_y, bool inside);
	LINKAGE bool _mouse_held_down_handler(int local_x, int local_y, bool inside);
	LINKAGE void _update_size();		  
	
private:

	friend class UI::Experiments::TreePanel;

	int _level;   // How deep is this node
	//UNUSED
	//unsigned char _cRGB[4]; //Color coding of column line 
	unsigned char _lRGB[4]; // Color coding of level name	
	bool _is_current; // Whether this tree is the
	int _child_number; // Current root in a treePanel
	int _format;

public:

	Node collapsed_node;
	
	String level_name; // level name, eg: 1.1.2, III, or 3
	
public:
	
	/************ Tree::set_format bitwise codes **********/
	enum
	{	
	DEFAULTS              = 0, // bar, standard bar color
	//UNUSED
	//ALTERNATE_COLOR       = 1, // Alternate between 8 different bar colors
	//ENABLE_BAR            = 2, // enable the bar (column?)
	DISABLE_BAR           = 4, // disable the bar (column?)
	DISABLE_DEEPEST_BAR   = 8, // disable only the deepest bar
	CONNECT_CHILDREN_ONLY = 16, // disable only the bar of the last child of each root
	DISPLAY_HIERARCHY     = 32, // display some sort of hierachy in the tree node title
	HIERARCHY_NUMERICDOT  = 64, // display hierarchy in 1.3.2 (etc...) format
	HIERARCHY_LEVEL_ONLY  = 128, // display hierarchy as only the level depth
	};
	inline void set_format(int f){ _format = f; }
	
	inline void open(){ _set_is_open(true); }
	inline void close(){ _set_is_open(false); }

	inline void set_level(int l){ _level = l; }
	inline void set_current(bool c){ _is_current = c; }

	inline int get_level(){ return _level; }
	inline int get_child_number(){ return _child_number; }
	
	inline void set_child_number(int c){ _child_number = c; } 
	
	//REMOVE ME
	inline void set_color(float r, float g, float b)
	{
		//_rgb255(_cRGB,r,g,b); //UNUSED
	}
	inline void set_level_color(float r, float g, float b)
	{ 
		_rgb255(_lRGB,r,g,b);
	}

	//UNDOCUMENTED 
	//Why set_column? Why can't _draw draw a bar?
	//inline void set_column(Column *c){ _column = c; }
	inline void enable_bar(){ /*if(_column) _column->set_type(1);*/ }
	inline void disable_bar(){ /*if(_column) _column->set_type(0);*/ } 
	
public:

	/*2019
	 @param inset appears to make an empty StaticText element to indent 
	 @a inset pixels. It HAD done so even if 0. I don't think it's used.
	 I'm treating it like an ID field so its constructor can be regular.
	*/
	explicit Tree(Node *parent, C_String name="", bool open=false, int inset=-1, CB cb=CB())
	{
		is_open = open; int id = inset; GLUI_INIT;

		//THIS WHOLE CLASS IS WEIRD. I DON'T SEE HOW THESE
		//THINGS CAN MAKE A DIFFERENCE. I'M JUST DISABLING
		//THIS CODE SO THAT IT DOESN'T HAVE TO BE COMPILED.

		/*What's the point? Can't this be x_off?
		//2019: Adding condition for when inset it default.
		//If this makes a smaller margin, maybe col can be
		//adjusted to compensate.
		if(inset>0) //int inset=0
		{
			StaticText *inset_label = new StaticText(this);
			inset_label->set_w(inset);
		}*/
		/*Just to draw a bar?
		set_column(new Column(this,true));*/		
	}
	inline Tree(){ _members_init(); }
	inline void _members_init()
	{
		h                = UI_DEFAULT_HEIGHT + 7;
		w                = UI_DEFAULT_WIDTH;
		y_off_top        = 21;
		//y_off_bot        = UI_YOFF;
		w                = UI_DEFAULT_WIDTH;
		alignment        = ALIGN_LEFT;		
		collapsible      = &collapsed_node;
		is_open			 = false;

		//_cRGB            = 0x808080; //UNUSED
		//set_color(0.5f,0.5f,0.5f);
		//_lRGB            = 0;		
		memset(_lRGB,0x00,sizeof(_lRGB));
		_is_current      = 0;
		_child_number    = 0;
		_format          = 0;		
		//name           = "";
		//level_name     = "";
		_level           = 0; 		
	}
};

/************************************************************/
/*                                                          */
/*                 TreePanel class (container)              */
/*                                                          */
/************************************************************/

/**UNFINISHED WORK
  Manages, maintains, and formats a tree of UI::Tree objects.
  These are shown in a heirarchical, collapsible display.
  
  FIXME: There's an infinite loop in the traversal code (OSL 2006/06)
*/
class UI::Experiments::TreePanel : public Interface<TreePanel,Panel>
{
public:
	
	int max_levels;	
	int format;
	//float red,green,blue; //UNUSED
	float lred,lgreen,lblue;
	int root_children;
	/* These variables allow the tree panel to traverse the tree
	using only two function calls. (Well, four, if you count 
	going in reverse */
	union
	{
		Tree *curr_branch; /* Current Branch */
		Panel *_curr_branch; 
	};
	Panel *curr_root; /* Current Root */

public:
		
	//REMOVE ME
	#ifdef GLUI_GLUI_H
	inline void set_color(float r, float g, float b)
	{
		//red = r; green = g; blue = b;
	}
	#endif
	void set_level_color(float r, float g, float b)
	{
		lred = r; lgreen = g; lblue = b;

		//redraw(); //2019: redraw has no effect here
		update_all(); 
	}
	
	/************ Tree::set_format bitwise codes **********/
	enum
	{	
	DEFAULTS              = 0, // bar, standard bar color
	//UNUSED
	//ALTERNATE_COLOR       = 1, // Alternate between 8 different bar colors
	//ENABLE_BAR            = 2, // enable the bar (column?)
	DISABLE_BAR           = 4, // disable the bar (column?)
	DISABLE_DEEPEST_BAR   = 8, // disable only the deepest bar
	CONNECT_CHILDREN_ONLY = 16, // disable only the bar of the last child of each root
	DISPLAY_HIERARCHY     = 32, // display some sort of hierachy in the tree node title
	HIERARCHY_NUMERICDOT  = 64, // display hierarchy in 1.3.2 (etc...) format
	HIERARCHY_LEVEL_ONLY  = 128, // display hierarchy as only the level depth
	};
	inline void set_format(int f){ format = f; }

	//2019: EVERYTHING ABOUT TreePanel IS STRANGE
	//
	// Changing camelCase names based on Example6
	// Don't rely on curr_branch... it goes weird
	// places. These methods are most inscrutable
	//	
	/* Goes up one level, resets curr_root and curr_branch to parents*/
	LINKAGE void go_up_tree(Panel *branch=NULL);	
	/* Finds the very last branch of curr_root, resets vars */
	LINKAGE void go_to_leaf(Panel *root=NULL);
	/* Resets curr_root and curr_branch to TreePanel and last_child */
	LINKAGE void go_to_root(Panel *new_root=NULL);
	LINKAGE void go_to_next();
	inline void refresh(){ curr_root->activate(); redraw(); } //???
	
	LINKAGE void expand_all(),collapse_all();
	LINKAGE void update_all();
	LINKAGE void update_tree(Tree *update);

	/* Goes to root; adds Tree to root; Makes Tree curr_root */
	inline Tree *add_tree(C_String name, Tree *root=NULL)
	{
		go_to_root(root); return add_tree(new Tree(NULL,name));
	}	
	/* If orphan, adds Tree to curr_root; Makes Tree curr_root */
	LINKAGE Tree *add_tree(Tree *tree);
	
	/* 
	Goes to curr_root; goes up one level; deletes/unlinks curr_root 
	NOTE: default argument is removed so it's more like delete_item
	and because the semantics are inconsistent across these methods
	 */
	LINKAGE void delete_tree(Panel *curr_root/*=NULL*/);

public:

	explicit TreePanel(GLUI_ARGS(="", C_Bool open=false,))
	{
		GLUI_INIT; is_open = open!=0;
	}
	inline TreePanel(){ _members_init(); }
	inline void _members_init() 
	{
		y_off_top     = UI_YOFF;
		y_off_bot     = UI_YOFF;
		is_open       = false;

		//_next_id      = 0; //UNUSED
		curr_root     = this;
		curr_branch   = NULL;
		//2019: red, etc. is not used.
		//lred, etc. was not initialized.
		//Example6 uses: tp->set_level_color(1,1,1);
		//UNUSED
		//red           = 0.5f;
		//green         = 0.5f;
		//blue          = 0.5f;
		lred = lblue = lgreen = 1;
		root_children = 0;
	}

protected:

	//int _next_id; //UNUSED
	//inline int _unique_id(){ return _next_id++; }
};

#endif //defined(GLUI_BUILD_TREE_CONTROLS) || defined(GLUI_BUILD_EXAMPLES)

/************************************************************/
/*                                                          */
/*                   Old GLUI_Master data                   */
/*                                                          */
/************************************************************/

struct Event /* Zero-initialized GLUI_Master data members. */
{
	int curr_x,curr_y;
	int down_x,down_y;
	int prev_click_et;
	int curr_click_et;

	int curr_modifiers; // Shift, Ctrl, Alt

	bool prev_button_down;
	bool curr_button_down;
	bool curr_cancel_down; /** control rejected down event */
	bool spacebar_is_down;

	bool init_inside;
	bool prev_inside;
	bool curr_inside;	

	bool wheel_event; /** special_handler is wheel */
	
	int prev_caret_pt; /* code-point for text boxes */
	int curr_caret_pt; 
	int caret_x,caret_y; /* curr_caret_pt, in pixels */

	#ifdef GLUI_GLUI_H___NODEPRECATED
	union{ UI *active_control_ui, *active_control_glui; };
	#else
	UI *active_control_ui;
	#endif

	UI::Control *active_control, *menu_control;

	Event(){ memset(this,0x00,sizeof(*this)); }

public:

	inline UI::Control *get_pressed()
	{
		if(!curr_inside||!curr_button_down) 
		return NULL; return active_control;
	}
	inline UI::Control *get_caret()
	{
		if(curr_button_down||curr_caret_pt<0)
		return NULL; return active_control;
	}
	inline bool set_caret(UI::Control *c, int pt=-1)
	{
		if(c!=active_control||pt==curr_caret_pt) 
		return false;
		prev_caret_pt = curr_caret_pt; 
		curr_caret_pt = pt; return true;
	}	
	inline void reset_caret(UI::Control *c)
	{
		if(set_caret(c,0)) caret_x = caret_y = 0; 
	}	

	inline int get_clicked(int ms)
	{
		return curr_click_et-prev_click_et<ms?UI::DOUBLE_CLICKED:0; 
	}
};

/************************************************************/
/*                                                          */
/*                     Master UI Class                      */
/*                                                          */
/************************************************************/

/**
 The master manages our interaction with GLUI.
 There's only one GLUI.
*/
class GLUI : public Event
{	  
public:

	double _spin_growth;
	double _spin_growth_exp;

	int double_click_millis; /* 500 */

	class Click /* spacebar_mouse_click */
	{
	public:
		
		Event &e;		
		UI::Control *c;
		bool down, callthrough; 

		Click(UI::Control *c, 
		int x=-1, //spacebar_mouse_click 
		int y=-1, Event *_=&GLUI::_this())
		:e(*_),c(c),down(_->curr_button_down)
		{	
			if(c&&!down)
			{				
				e.curr_x = e.down_x = x; 
				e.curr_y = e.down_y = y;				
				e.prev_inside =				
				e.prev_button_down = false;
				e.init_inside = 
				e.curr_inside =
				e.curr_button_down = true;
				callthrough = 
				c->mouse_down_handler(x,y);				
			}
			else callthrough = true;
		}
		inline void drag(int dx, int dy)
		{
			e.curr_x+=dx; e.curr_y+=dy;
			if(!callthrough) //curr_button_down
			c->mouse_held_down_handler(e.curr_x,e.curr_y);
		}
		inline bool finish()
		{	
			bool ret = callthrough;
			callthrough = true;
			if(!ret) //curr_button_down
			c->mouse_up_handler(e.curr_x,e.curr_y);	
			return ret;
		}
		~Click() //RAII/functor destructor
		{
			bool ret = callthrough;
			finish();
			callthrough = ret;
			e.curr_button_down = down;
		}
		inline operator bool()
		{
			return callthrough; 
		}
	};
	static bool spacebar_mouse_click()
	{
		return Click(get_active()); 
	}

	static UI::Control *get_active()
	{
		return _this().active_control;
	}

	static UI::Control *get_pressed()
	{
		return _this().Event::get_pressed();
	}

	static UI::Control *get_caret()
	{
		return _this().Event::get_caret();
	}
	static bool set_caret(UI::Control *c, int pt=-1)
	{
		return _this().Event::set_caret(c,pt);
	}	
	static void reset_caret(UI::Control *c)
	{
		return _this().Event::reset_caret(c);
	}		 
	 
	static Live_Variable *get_edited()
	{
		return _this()._edited;
	}
	static void set_edited(UI::Control *c=NULL)
	{
		_this()._set_edited(c);
	}
	LINKAGE void _set_edited(UI::Control *c);

	static int get_clicked()
	{
		GLUI &e = _this();
		return e.Event::get_clicked(e.double_click_millis);
	}
	
public: //New data (2019)

	/** This is a default callback for controls without one. 
	 NOTE: This is evaluated by execute_callback even for a
	 control that is not assigned to a UI. */
	UI::CB active_callback; 

	/*2019 addition
	 *
	 * If this is set the UI/GLUI objects' fonts will be initialized
	 * to NULL.
	 */
	void *font;

	/** Restoring these previously disabled callbacks. 
	 If the various handlers return true these are consulted. 
	 NOTE: These defaults are assigned by UI's constructor. */
	bool(*keyboard_callback)(UI*,int,int);    
	bool(*special_callback)(UI*,int,int);
	bool(*mouse_callback)(UI*,int,int,int,int);
	bool(*motion_callback)(UI*,int,int);
	bool(*wheel_callback)(UI*,int,int,int,int);
	bool(*reshape_callback)(UI*,int,int);
	
	/** These defaults are assigned by UI's constructor. */
	void(*output_callback)(UI::Control*,bool);	
	bool(*canvas_callback)(UI::Control*,bool);

	/** For non-portable extensions. */
	void(*create_callback)(UI*);

	/** By default this is set to an OpenGL based caret that uses 
	 glutTimerFunc to xor the front buffer every 500 milliseconds.
	 Set it to "NULL" to disable the blinking caret, or set it to
	 implement the caret.
	 */
	bool(*caret_callback)(UI::Control*,int h, int x, int y, bool readonly);

public: //Old GLUI_Master methods
	
	//REMOVE ME
	//This is set by hovering over a ListBox.
	static void set_menu(UI::Control *c=NULL) 
	{
		GLUI &e = _this(); 
		if(e.menu_control&&c!=e.menu_control)
		{
			UI::Control *mc = e.menu_control; 
			e.menu_control = NULL;
			mc->mouse_over(false,-1,-1);
		}
		if(c) e.curr_cancel_down = true; 

		/* set_left_button_glut_menu_control */

		e.menu_control = c; //curr_left_button_glut_menu
	}
	
	/* Replaces gluis public member. */
	
	static UI *first_ui() 
	{
		return (UI*)_this()._glui_windows.first_child();
	}
	static UI *last_ui() 
	{
		return (UI*)_this()._glui_windows.last_child();
	}
	 	
	static UI *find_glui_by_window_id(int window_id)
	{
		for(UI*ui=first_ui();ui;ui=ui->next())
		if(ui->_glut_window_id==window_id)
		return ui; return NULL;
	}
	static UI *find_glui_by_window_id(int window_id, bool closing)
	{
		UI *ui = find_glui_by_window_id(window_id);
		return ui&&ui->_pending_idleclose==closing?ui:NULL;
	}

	static void sync_live_all()
	{
		for(UI*ui=first_ui();ui;ui=ui->next())
		{
			ui->sync_live(); /** sync it **/
		}
	}

	static void close_all(bool now=false)
	{
		for(UI*ui;ui=first_ui();) ui->close(now);
	}	

	/**NEW: This is the safest way to close out a GLUT/GLUI window complex.
	 * It will close all of the UI windows; the last UI will close out the
	 * parent (origin) window. set_main_gfx_window and set_main_esc_window
	 * windows are not destroyed. output_callback is killed in the hope it
	 * won't disturb them, since GLUT crashes as soon as destroyed windows
	 * are selected by ID.
	 *
	 * The bool arguments are forwarded to UI::close. See its instructions.
	 */
	static int close_glui_by_id(int (UI::*PtoM)(), int id, bool now=false, bool detach=false)
	{
		int i = 0; if(id>0) for(UI*uj,*ui=first_ui();ui;ui=uj)
		{
			uj = ui->next();
			if(id==(ui->*PtoM)()) if(!ui->_pending_idleclose||now)
			{
				ui->close(now,detach); i++;
			}
		}
		return i;
	}	
	static int close_glui_by_origin_id(int id, bool now=false, bool detach=false)
	{
		return close_glui_by_id(&UI::get_glut_origin_id,id,now,detach);
	}
	static int close_glui_by_escape_id(int id, bool now=false, bool detach=false)
	{
		return close_glui_by_id(&UI::get_glut_escape_id,id,now,detach);
	}
	static int close_glui_by_parent_id(int id, bool now=false, bool detach=false)
	{
		return close_glui_by_id(&UI::get_glut_parent_id,id,now,detach);
	}
	static bool close_glui_by_window_id(int id, bool now=false, bool detach=false)
	{
		if(UI*ui=find_glui_by_window_id(id)) ui->close(now,detach); 
		else return false; return true;
	}

	//UNDOCUMENTED USER METHODS
	LINKAGE static void get_viewport_area(int *x, int *y, int *w, int *h);
	LINKAGE static void auto_set_viewport();
	LINKAGE static void reshape();

	inline static float get_version(){ return _this()._glui_version; }

	//UNDOCUMENTED 
	LINKAGE static void glui_setIdleFuncIfNecessary(bool necessary=false);
	 	
	#ifdef GLUI_GLUI_H___NODEPRECATED
	inline UI *create_glui(C_String name, int flags=0, int x=-1, int y=-1); 
	inline UI *create_glui_subwindow(int parent_window, int flags=0);
	#endif

	GLUI(); ~GLUI(){}

protected:
	
	friend class UI;
	
	int _ui_id_counter;

	Node _glui_windows;

	float _glui_version;

	Live_Variable *_edited;
	
	// C++ forbids forward declared references
	inline static GLUI &_this();
};
  
/************************************************************/
/*                                                          */
/*                     Master UI Class                      */
/*                                                          */
/************************************************************/

/**
 The master manages our interaction with GLUT.
 There's only one GLUT.
*/
class GLUT : public GLUI
{	  
	inline static GLUT &_this()
	{
		return static_cast<GLUT&>(GLUI::_this()); 
	}

public:
		
	/************************************************************/
	/*                                                          */
	/*              Class for managing a GLUT window            */
	/*                                                          */
	/************************************************************/

	/**
	 A top-level window.  The GLUI_Master GLUT callback can route events
	 to the callbacks in this class, for arbitrary use by external users.
	 (see GLUT::set_glutKeyboardFunc).
 
	 This entire approach seems to be superceded by the "subwindow" flavor
	 of UI.
	*/
	class Window : public Node 
	{	
	public: 

		int glut_window_id;

		//Reminder: the idle function is not particluar 
		//to a window, and so should not be stored here.
		//But I'm making it so there can be many idling
		//routines running without special coordination.
		void(*_glut_idle_CB)();

		/*********** Pointers to GLUT callthrough functions *****/
		void(*glut_reshape_CB)(int, int);
		void(*glut_keyboard_CB)(unsigned char, int, int);
		void(*glut_keyboard_up_CB)(unsigned char, int, int);
		void(*glut_special_CB)(int, int, int);
		void(*glut_special_up_CB)(int, int, int);
		void(*glut_mouse_CB)(int,int,int,int);
		/*UNUSED
		void(*glut_passive_motion_CB)(int,int);		
		void(*glut_visibility_CB)(int);
		void(*glut_motion_CB)(int,int);
		void(*glut_display_CB)();
		void(*glut_entry_CB)(int);
		*/
	
	public: 
		
		inline Window *prev(){ return (Window*)_prev_sibling; }
		inline Window *next(){ return (Window*)_next_sibling; }

		inline Window();
	};

public:

	static Window *first_glut_window()
	{
		return (Window*)_this()._glut_windows.first_child();
	}
	static Window *find_glut_window(int window_id)
	{
		Window *w = first_glut_window();
		for(;w;w=w->next()) 
		if(window_id==w->glut_window_id) return w; return NULL;
	}

	/**WARNING: This now calls close_glui_by_origin_id by default.
	 * If it finds UIs attached to the window the window will not
	 * be destroyed until the next GLUT idle callback. "now" will
	 * force close to happen immediately. "close" cannot be false
	 * unless no UI objects are attached to the window, otherwise
	 * GLUT will crash when the UIs objects are delted.
	 */
	static bool destroy_glut_window(int window_id, bool now=false, bool close=true)
	{
		Window *w; if(w=find_glut_window(window_id))
		if(!close||!close_glui_by_origin_id(window_id,now)||now)
		{
			w->_delete(); cdecl_glutDestroyWindow(window_id);
		}
		return w!=NULL;
	}

	/********** GLUT callthroughs **********/
    
	enum //glut.h
	{ 
	_INIT_PARENT=-1,

	DISPLAY=0,
	RESHAPE, //managed
	KEYBOARD, //managed
	MOUSE, //managed
	MOTION,
	PASSIVE_MOTION, 
	ENTRY,
	VISIBILITY,		
	IDLE, //managed
	TIMER,
	MENU_STATE,

	SPECIAL, //managed		
	SPACEBALL_MOTION,SPACEBALL_ROTATE,SPACEBALL_BUTTON,
	BUTTON_BOX,
	DIALS,
	TABLET_MOTION,TABLET_BUTTON,

	MENU_STATUS,
	OVERLAY_DISPLAY,		
	WINDOW_STATUS,
		
	KEYBOARD_UP,
	SPECIAL_UP,
	JOYSTICK,

	//EXPERIMENTAL	
	_glutDestroyWindow,
	_glutPostWindowRedisplay,
	_glutGet,

	_NUM_FUNCS
	};
	 
	//TODO: PUT IN glut.h ORDER
	#define GLUI_FUNC(x,y,...) \
	static void set_glut##y##Func(__VA_ARGS__){ _this()._glut_funcs1[x]((UI::Void_CB)f); }
	GLUI_FUNC(RESHAPE,Reshape,void(*f)(int width, int height))
	GLUI_FUNC(KEYBOARD,Keyboard,void(*f)(unsigned char key, int x, int y)=NULL)
	GLUI_FUNC(SPECIAL,Special,void(*f)(int key, int x, int y)=NULL)
	GLUI_FUNC(MOUSE,Mouse,void(*f)(int,int,int,int)=NULL)
	//WARNING: Idle is unowned by a window. Is it unique?
	GLUI_FUNC(IDLE,Idle,void(*f)()=NULL)
	/* These are the glut callbacks that we do not handle */
	GLUI_FUNC(DISPLAY,Display,void(*f)()=NULL)
	GLUI_FUNC(MOTION,Motion,void(*f)(int x, int y)=NULL)
	GLUI_FUNC(PASSIVE_MOTION,PassiveMotion,void(*f)(int x, int y)=NULL)
	GLUI_FUNC(ENTRY,Entry,void(*f)(int state))
	GLUI_FUNC(VISIBILITY,Visibility,void(*f)(int state))	
	static void set_glutTimerFunc(unsigned int millis, void(*f)(int value), int value)
	{
		_this()._glut_funcs2[TIMER](millis,(UI::Void_CB)f,value);
	}
	GLUI_FUNC(OVERLAY_DISPLAY,OverlayDisplay,void(*f)()) //EXOTIC
	GLUI_FUNC(SPACEBALL_MOTION,SpaceballMotion,void(*f)(int x, int y, int z)) //EXOTIC
	GLUI_FUNC(SPACEBALL_ROTATE,SpaceballRotate,void(*f)(int x, int y, int z)) //EXOTIC
	GLUI_FUNC(SPACEBALL_BUTTON,SpaceballButton,void(*f)(int button, int state)) //EXOTIC
	GLUI_FUNC(TABLET_MOTION,TabletMotion,void(*f)(int x, int y)) //EXOTIC
	GLUI_FUNC(TABLET_BUTTON,TabletButton,void(*f)(int button, int state, int x, int y)) //EXOTIC
	GLUI_FUNC(WINDOW_STATUS,WindowStatus,void(*f)(int state))
	GLUI_FUNC(MENU_STATUS,MenuStatus,void(*f)(int status, int x, int y))
	GLUI_FUNC(MENU_STATE,MenuState,void(*f)(int state)) //DEPRECATED
	GLUI_FUNC(BUTTON_BOX,ButtonBox,void(*f)(int button, int state)) //EXOTIC
	GLUI_FUNC(DIALS,Dials,void(*f)(int dial, int value)) //EXOTIC
	GLUI_FUNC(KEYBOARD_UP,KeyboardUp,void(*f)(unsigned char key, int x, int y))
	GLUI_FUNC(SPECIAL_UP,SpecialUp,void(*f)(int key, int x, int y))
	static  void set_glutJoystickFunc(void(*f)(unsigned int,int,int,int),int pollInterval)
	{
		_this()._glut_funcs3[JOYSTICK]((UI::Void_CB)f,pollInterval);
	}

	//(These would better be placed in a C++ namespace.)
	//EXPERIMENTAL 
	static void cdecl_glutDestroyWindow(int glut_window)
	{
		_this()._glut_funcs4[_glutDestroyWindow](glut_window);
	}
	static void cdecl_glutPostWindowRedisplay(int glut_window)
	{
		_this()._glut_funcs4[_glutPostWindowRedisplay](glut_window);
	}
	static int cdecl_glutGet(unsigned int glut_enum)
	{
		return _this()._glut_funcs5[_glutGet](glut_enum);
	}

	GLUT(); ~GLUT(){}

private:
	
	friend class UI;
	friend class GLUI;

	Node _glut_windows;
	
	union //GLUI_FUNC
	{
	//GLUT's variants' calling conventions are all over the place.
	void(*_glut_funcs1[_NUM_FUNCS])(UI::Void_CB);
	void(*_glut_funcs2[_NUM_FUNCS])(unsigned int,UI::Void_CB,int); //TIMER
	void(*_glut_funcs3[_NUM_FUNCS])(UI::Void_CB,int); //JOYSTICK	
	void(*_glut_funcs4[_NUM_FUNCS])(int); //_glutDestroyWindow
	int (*_glut_funcs5[_NUM_FUNCS])(unsigned int); //_glutGet
	};

	bool _add_cb_to_glut_window(int,int=_INIT_PARENT,UI::Void_CB=NULL);
};

/**
 This is the only GLUI_Master in existence
*/
extern LINKAGE class GLUT GLUT;
/**
 This is the only GLUI_Master in existence
*/
static class GLUI &GLUI = GLUT;

// C++ forbids forward declared references
class GLUI &GLUI::_this(){ return GLUT; }
	  
/************************************************************/
/*                                                          */
/*                   Copy/Paste class                       */
/*                                                          */
/************************************************************/

class Clipboard
{
public:

	Clipboard(C_String name):name(name.str){}

	/**
	 * Typically @c name is CLIPBOARD or PRIMARY.
	 * PRIMARY is available only on X11 systems, and
	 * should be set whenever a selection is finalized.
	 */
	const String name;
	
	/**
	 * This is analogous to copying to the clipboard.
	 */
	LINKAGE void set_text(C_String sel, size_t len=~size_t());	

	/**
	 * This is analogous to pasting from the clipboard, or
	 * that is, getting the text on the clipboard to paste.
	 */
	LINKAGE bool get_text(String &container);
};

/**
 * These are built-in clipboards. On non X11-like systems
 * @c UI_Selection is a "no-op" object.
 */
extern LINKAGE class Clipboard Clipboard, Selection;

#undef LINKAGE
 
//---.
}//<-'

#endif //GLUI_GLUI_HPP
/*C1071*/
