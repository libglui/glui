/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui.cpp

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

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

namespace GLUI_Library
{//-.
//<-'

class GLUT GLUT; /*** This object must be used to create a GLUI ***/

static bool glui_initialized = false;

inline GLUT::Window::Window():glut_window_id()
{
	void **p=&(void*&)_glut_idle_CB;
	while(p<=&(void*&)glut_mouse_CB) *p++ = NULL;
}

int UI::_textures_id(int inc)
{
	//REMINDER: This has to be initialized inside this function to
	//be independent of translation-unit's initialization ordering.
	static int static_local_variable = 0;								 

	int id = static_local_variable; if(inc)
	{
		static_local_variable+=inc; if(glui_initialized)
		{
			size_t size = (size_t)static_local_variable;
			for(UI*ui=GLUI::first_ui();ui;ui=ui->next())
			ui->_textures.resize(size);
		}
	}
	return id;
}

/**************************************** UI::disable() ********************/

void UI::enable(bool y)
{
	_main_panel->enable(y); //TODO? Set keyboard nav to first control.
}

/******************************************** UI::sync_live() **************/

void UI::sync_live()
{
	//Can't hurt to check.
	if(!_pending_idleclose) _main_panel->sync_live(); 
}

/****************************************************************************************/
		 
struct UI::_glut_friends
{
	static void display()
	{	
		/*  printf("display func\n"); */
		if(UI*ui=GLUI::find_glui_by_window_id(glutGetWindow()))
		{
			ui->_display();
			/*
				Do not do anything after the above line, b/c the UI
				window might have just closed itself
			*/
			assert(ui->_glut_window_id==glutGetWindow());
		}		
	}
	static void reshape(int w, int h)
	{
		int current_window = glutGetWindow();

		/*printf("glui_reshape_func(): %d  w/h: %d/%d\n",current_window,w,h); */

		/***  First check if this is main glut window ***/		
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			if(glut_window->glut_reshape_CB) 
			{
				glut_window->glut_reshape_CB(w,h);				
			}

			/***  Now send reshape events to all subwindows  ***/
			for(UI*ui=GLUI::first_ui();ui;ui=ui->next())		
			if(current_window==ui->_glut_parent_id) 
			{
				glutSetWindow(ui->_glut_window_id);
				ui->_reshape(w,h);
			}
		}
		else if(UI*ui=GLUI::find_glui_by_window_id(current_window))
		{
			/***  A standalone window  ***/ 		
			ui->_reshape(w,h);
		}

		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
	}
	static void reshape2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::RESHAPE,f))
		glutReshapeFunc(reshape);
	}
	static void keyboard(unsigned char key, int x, int y)
	{
		int current_window = glutGetWindow();

		/*printf("key: %d\n",current_window); */		

		GLUI.curr_x = x; GLUI.curr_y = y;

		int cm = GLUI.curr_modifiers = glutGetModifiers();

		UI *ui = GLUI.active_control_ui;

		if(key==27) //CTRL('[') Esc? Switching window.
		{
			if(cm||GLUI.curr_button_down||GLUI.spacebar_is_down)
			{
				GLUI.curr_cancel_down = true;
				GLUI.spacebar_is_down = false; 
				
				return;
			}

			//On Windows Esc belongs to the parent container.
			//I can't recall if it changes controls' state or not.
			//The parent might hide the control group or close itself.
			if(ui&&ui->_glut_escape_id>0)
			{
				ui->deactivate_current_control();
								
				if(!ui->keyboard_callback
				||ui->keyboard_callback(ui,27,0)) //Esc
				{
					glutSetWindow(ui->_glut_escape_id);					
					if(GLUT::Window*gw=GLUT::find_glut_window(ui->_glut_escape_id))
					if(gw->glut_keyboard_CB) 
					gw->glut_keyboard_CB(27,x,y); //Esc
					glutPopWindow();
				}
			}
			return;
		}

		/**  Was event in a GLUT window?  **/
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			//Why? I think GLUT doesn't direct keyboard intput to subwindows.
			if(ui&&ui->_glut_parent_id==current_window)
			{
				assert(GLUI.active_control);
				glutSetWindow(ui->get_glut_window_id());
				ui->_keyboard(key);
			}
			else if(glut_window->glut_keyboard_CB) 
			{	
				glut_window->glut_keyboard_CB(key,x,y);				
			}
		}
		else if(ui=GLUI::find_glui_by_window_id(current_window))
		{
			ui->_keyboard(key); /***  A standalone window  ***/
		}
		
		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
	}
	static void keyboard2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::KEYBOARD,f))
		glutKeyboardFunc(keyboard);
	}
	static void keyboard_up(unsigned char key, int x, int y)
	{
		int current_window = glutGetWindow();

		/*printf("key: %d\n",current_window); */
		
		GLUI.curr_x = x; GLUI.curr_y = y;

		int cm = GLUI.curr_modifiers = glutGetModifiers();
		
		/**  Was event in a GLUT window?  **/
		UI *ui = GLUI.active_control_ui;
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			//Why? I think GLUT doesn't direct keyboard intput to subwindows.
			if(ui&&ui->_glut_parent_id==current_window)
			{
				assert(GLUI.active_control);
				glutSetWindow(ui->get_glut_window_id());
				ui->_keyboard_up(key);		
			}
			else if(glut_window->glut_keyboard_up_CB) 
			{	
				glut_window->glut_keyboard_up_CB(key,x,y);
			}
		}
		else if(ui=GLUI::find_glui_by_window_id(current_window))
		{
			ui->_keyboard_up(key); /***  A standalone window  ***/ 		
			
		}

		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
	}
	static void keyboard_up2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::KEYBOARD_UP,f))
		glutKeyboardUpFunc(keyboard_up);
	}
	static void special(int key, int x, int y)
	{
		int current_window = glutGetWindow();

		GLUI.curr_x = x; GLUI.curr_y = y;

		int cm = GLUI.curr_modifiers = glutGetModifiers();

		/**  Was event in a GLUT window?  **/
		UI *ui = GLUI.active_control_ui;
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			//Why? I think GLUT doesn't direct keyboard intput to subwindows.
			if(ui&&ui->_glut_parent_id==current_window)
			{
				assert(GLUI.active_control);
				glutSetWindow(ui->get_glut_window_id());
				ui->_special(key);
			}
			else if(glut_window->glut_special_CB) 
			{	
				glut_window->glut_special_CB(key,x,y);
			}
		}
		else if(UI*ui=GLUI::find_glui_by_window_id(current_window))
		{
			ui->_special(key); /***  A standalone window  ***/ 					
		}

		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
	}
	static void special2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::SPECIAL,f))
		glutSpecialFunc(special);
	}  
	static void special_up(int key, int x, int y)
	{
		int current_window = glutGetWindow();

		/*printf("key: %d\n",current_window); */
		
		GLUI.curr_x = x; GLUI.curr_y = y;

		int cm = GLUI.curr_modifiers = glutGetModifiers();
		
		/**  Was event in a GLUT window?  **/
		UI *ui = GLUI.active_control_ui;
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			//Why? I think GLUT doesn't direct keyboard intput to subwindows.
			if(ui&&ui->_glut_parent_id==current_window)
			{
				assert(GLUI.active_control);
				glutSetWindow(ui->get_glut_window_id());
				ui->_special_up(key);		
			}
			else if(glut_window->glut_special_up_CB) 
			{	
				glut_window->glut_special_up_CB(key,x,y);
			}
		}
		else if(ui=GLUI::find_glui_by_window_id(current_window))
		{
			ui->_special_up(key); /***  A standalone window  ***/ 		
			
		}

		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
	}
	static void special_up2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::SPECIAL_UP,f))
		glutSpecialUpFunc(special_up);
	}
	static void mouse(int button, int state, int x, int y)
	{			
		bool down = state==GLUT_DOWN;
		bool left = button==GLUT_LEFT_BUTTON;

		int current_window = glutGetWindow();
		
		GLUI.curr_x = x; GLUI.curr_y = y;		
		GLUI.curr_modifiers = glutGetModifiers();
		if(left)
		{	
			//glutMouseFunc is only called if buttons move.
			assert(down!=GLUI.curr_button_down||GLUI.curr_cancel_down);

			//NOTE: prev_button_down is only for the event.
			GLUI.prev_button_down = state!=GLUT_DOWN;
			GLUI.curr_button_down = state==GLUT_DOWN;
			assert(state==GLUT_DOWN||state==GLUT_UP);
			if(down)
			{
				GLUI.down_x = x; GLUI.down_y = y;

				GLUI.curr_cancel_down = false; //NEW

				//REMINDER: _mouse zeroes prev_click_et.
				GLUI.prev_click_et = GLUI.curr_click_et;
				GLUI.curr_click_et = glutGet(GLUT_ELAPSED_TIME);
			}
		}		
		
		/**  Was event in a GLUT window?  **/
		UI *ui = GLUI.active_control_ui;
		if(GLUT::Window*glut_window=GLUT::find_glut_window(current_window))	
		{
			if(ui) ui->deactivate_current_control();
			
			if(glut_window->glut_mouse_CB)
			{
				glut_window->glut_mouse_CB(button,state,x,y);		
			}			
		}
		else //if(left) //need to passthrough mouse_callback.
		{
			if(ui=GLUI::find_glui_by_window_id(current_window))
			{
				/***  A standalone window  ***/ 		
				//ui->_passive_motion(0,0); //2019?????
				ui->_mouse(button,state,x,y);
			}
		}
		
		//Don't do this. Outputs "bogus window" warning on closing.
		//glutSetWindow(current_window); 
							 
		if(left) GLUI.prev_button_down = down;

		//HACK: Can this be cleaned up reliably?
		if(GLUI.menu_control) GLUI::set_menu(); 
	}
	static void mouse2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::MOUSE,f))
		glutMouseFunc(mouse);
	}
	static void motion(int x, int y)
	{
		int current_window = glutGetWindow();

		GLUI.curr_x = x; GLUI.curr_y = y;

		//Nvidia GLUT return 0 in this context. They are locked in.
		//GLUI.curr_modifiers = glutGetModifiers();

		if(UI*ui=GLUI::find_glui_by_window_id(current_window))
		{
			ui->_motion(x,y); 
			
			//Don't do this. Outputs "bogus window" warning on closing.
			//glutSetWindow(current_window); 
		}
	}
	static void passive_motion(int x, int y)
	{
		GLUI.curr_x = x; GLUI.curr_y = y;
		
		//Nvidia GLUT return 0 in this context. They are locked in.
		//GLUI.curr_modifiers = glutGetModifiers();

		if(UI*ui=GLUI::find_glui_by_window_id(glutGetWindow()))
		{
			ui->_passive_motion(x,y);
			
			assert(ui->_glut_window_id==glutGetWindow());
		}

		//HACK: Can this be cleaned up reliably?
		//if(GLUI.menu_control) GLUI::set_menu(); 
	}
	static void entry(int state)
	{
		if(UI*ui=GLUI::find_glui_by_window_id(glutGetWindow()))
		{
			ui->_entry(state);

			assert(ui->_glut_window_id==glutGetWindow());
		}
	}
	static void visibility(int state)
	{	
		/* printf("IN UI VISIBILITY()\n"); */
		/* fflush(stdout); */
		if(UI*ui=GLUI::find_glui_by_window_id(glutGetWindow()))
		{
			ui->_visibility(state); //2019: NOP unless not

			assert(ui->_glut_window_id==glutGetWindow());
		}
	}
	/* Send idle event to each UI, then to the main window */
	static void idle()
	{	
		//Spinner needs a speed limit... besides GLUT will
		//call this idle function at infinite rate. That's
		//unacceptable.
		static int was = 0;
		int now = glutGet(GLUT_ELAPSED_TIME);
		{
			if(now-was<2)
			{
				#ifdef _WIN32
				Sleep(1);
				#else
				usleep(1000);
				#endif
				now = glutGet(GLUT_ELAPSED_TIME);
			}
		}
		was = now;

		for(UI*ui=GLUI::first_ui();ui;/*ui=ui->next*/) 
		{
			UI *swap = ui->next(); //_pending_idleclose
			ui->_idle(); 
			ui = swap;
		}

		bool necessary = false;
		//NEW: Tying idle routines to life of windows/facilitating 
		//multiple idle routines.
		//if(GLUT._glut_idle_CB)
		for(GLUT::Window*gw=GLUT::first_glut_window();gw;gw=gw->next())
		if(gw->_glut_idle_CB)
		{
			gw->_glut_idle_CB(); //GLUT._glut_idle_CB();

			necessary = true;
		}		
		if(!necessary) //else 
		{		
			//2019: Automatically uninstall idle so controls don't have to?
			GLUI::glui_setIdleFuncIfNecessary();
		}
	}
	static void idle2(UI::Void_CB f)
	{
		if(GLUT._add_cb_to_glut_window(glutGetWindow(),GLUT::IDLE,f))
		GLUI::glui_setIdleFuncIfNecessary(f?true:false);
	}
};

/************************************************ UI::UI() **********/

extern void glui_control_Nvidia_timer(int);
void UI::_init(const char *text, int subpos, int x, int y, int parent_window)
{	
	assert(subpos||-1==parent_window);

	if(subpos||-1!=parent_window) subpos|=SUBWINDOW;

	//REMOVE ME
	//Disabling this because the control constructors no longer take part in this.
	//if(!text) glui_format_str(window_name,subpos?"subwin_%p":"window_%p",&GLUT);	

	/*********************************** GLUI_Main::GLUI_Main() ********/

	//GLUI_Main::GLUI_Main()
	{
		_pending_idleclose       = false;
		_pending_repacking       = true;
		_pending_redisplay       = false;
		_main_gfx_callback       = false;		
		_glut_window_id          = -1;
		//_glut_origin_id        = -1;
		//_glut_escape_id        = -1; //_main_gfx_window_id
		_active_control          = NULL;
		_mouse_over_control      = NULL;				
		_buffer_mode = _buffer_back;  ///< New smooth way
		//_buffer_mode = _buffer_front; ///< Old flickery way (a bit faster).
		_w                       = 0;
		_h                       = 0;			
		_flags                   = subpos;		
		_subpos_x                = 0;
		_subpos_y                = 0;
		//_closing                 = false;
		_glut_parent_id          = -1;
		_ui_id                  = GLUI._ui_id_counter++;

		_textures.resize(_textures_id(+0));
		
		if(text)
		window_name              = text;

		//2019: Can set to NULL except old software may
		//expect to be non-null. Setting GLUT.font opts
		//into this new behavior.
		font                    = GLUT.font?NULL:GLUT_BITMAP_HELVETICA_12;

		user                    = NULL;
		output_callback         = GLUI.output_callback;
		canvas_callback         = GLUI.canvas_callback;
		keyboard_callback       = GLUI.keyboard_callback;		
		special_callback        = GLUI.special_callback;
		mouse_callback          = GLUI.mouse_callback;
		motion_callback         = GLUI.motion_callback;
		wheel_callback          = GLUI.wheel_callback;
		reshape_callback        = GLUI.reshape_callback;

		/*** Create the main panel ***/

		//Note: This is the only Control instantiated from
		//inside the library module. There's no big reason 
		//for instantiating in the header (inline) besides
		//if there is a bug it might be confusing if every
		//control has an unknown origin.
		if(!_main_panel)
		_main_panel              = new Panel;
		if(subpos)
		{
			//NOTE: Outlines are needed for glutPopWindow
			//to clearly illustrate the top window.
			//NOTE: ETCHED looks more like RAISED on
			//subwindows. The drawing routines would need
			//to be rewritten to draw boxes on w-1.
			//_main_panel->box_type  = Panel::NONE;
			_main_panel->box_type  = Panel::ETCHED; //RAISED;
		}
		_main_panel->ui          = this;
		//_main_panel->name        = "";
	}

	int old_glut_window;
												
	//WARNING: order depends on GLUT state calls
	old_glut_window = glutGetWindow();
	if(~subpos&SUBWINDOW) 
	{
		/* not a subwindow, creating a new top-level window */
		
		//create_standalone_window(window_name.c_str(),x,y);
		{		
			glutInitWindowSize(100,100);
			int ix,iy; if(x>=0||y>=0)
			{
				ix = glutGet(GLUT_INIT_WINDOW_X);
				iy = glutGet(GLUT_INIT_WINDOW_Y);
				glutInitWindowPosition(x,y);
			}
			glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
			_glut_window_id = glutCreateWindow(window_name.c_str());
			if(x>=0||y>=0)
			{
				glutInitWindowPosition(ix,iy);
			}
		}

		_glut_escape_id = -1;
		_glut_origin_id = _glut_window_id;

		//HACK: _display selects the first control. Right now there
		//is no control to focus.
		if(GLUI.active_control_ui) 
		GLUI.active_control_ui->deactivate_current_control();
		GLUI.active_control_ui = this;
		GLUI.active_control = _main_panel;
	}
	else /* *is* a subwindow */
	{
		_glut_escape_id = 
		_glut_origin_id = parent_window;
		
		//_create_subwindow(parent_window);
		{
			_glut_parent_id = parent_window;
			_glut_window_id = glutCreateSubWindow(parent_window,0,0,100,100);			
		}

		//2019: This way users don't have to do set_glutKeyboarFunc(NULL).
		glutSetWindow(parent_window);
		if(!GLUT._add_cb_to_glut_window(parent_window))
		assert(0);
		glutSetWindow(_glut_window_id);
	}		
	//setup_default_glut_callbacks();	
	{
		//glutSetWindow(_glut_window_id);
		glutDisplayFunc(_glut_friends::display);
		glutReshapeFunc(_glut_friends::reshape);
		glutKeyboardFunc(_glut_friends::keyboard);
		glutKeyboardUpFunc(_glut_friends::keyboard_up);
		glutSpecialFunc(_glut_friends::special);
		glutSpecialUpFunc(_glut_friends::special_up);
		glutMouseFunc(_glut_friends::mouse);
		glutMotionFunc(_glut_friends::motion);
		glutPassiveMotionFunc(_glut_friends::passive_motion);
		glutEntryFunc(_glut_friends::entry);
		glutVisibilityFunc(_glut_friends::visibility);	
		/*  glutIdleFunc(_glut_friends::idle);    // FIXME!  100% CPU usage!      */
	}	
	if(old_glut_window>0) glutSetWindow(old_glut_window);
	
	//if(subpos&SUBWINDOW) /* *is* a subwindow */
	{
		//This looks like disabled functionality//

		/*
		glutReshapeFunc(glui_glut_parent_id_reshape_func);
		glutSpecialFunc(glui_glut_parent_id_special_func);
		glutKeyboardFunc(glui_glut_parent_id_keyboard_func);
		glutMouseFunc(glui_glut_parent_id_mouse_func);
		*/
	}	
	
	_link_this_to_parent_last(&GLUI._glui_windows);

	if(GLUI.create_callback) GLUI.create_callback(this);

	//HACK: Give freeglut a kick in the pants. X servers
	//on Windows initialize to white screens every other
	//time.	
	glutTimerFunc(100,glui_control_Nvidia_timer,_glut_window_id);
	glutTimerFunc(500,glui_control_Nvidia_timer,_glut_window_id);
}

/*********************************** GLUT::GLUT() ******/

//GLUT calling-conventions vary across implementations :(
typedef void(*Int1_CB)(int);
typedef void(*Int2_CB)(int,int);
typedef void(*Int3_CB)(int,int,int);
typedef void(*Int4_CB)(int,int,int,int);
static void glui_glutDisplayFunc(void (*f)())
{
	glutDisplayFunc(f);
}
static void glui_glutMotionFunc(Int2_CB f)
{
	glutMotionFunc(f);
}
static void glui_glutPassiveMotionFunc(Int2_CB f)
{
	glutPassiveMotionFunc(f);
}
static void glui_glutEntryFunc(Int1_CB f)
{
	glutEntryFunc(f);
}
static void glui_glutVisibilityFunc(Int1_CB f)
{
	glutVisibilityFunc(f);
}
static void glui_glutTimerFunc(unsigned int millis, void (*f)(int value), int value)
{
	glutTimerFunc(millis,f,value);
}
static void glui_glutOverlayDisplayFunc(void(*f)())
{
	glutOverlayDisplayFunc(f);
}
static void glui_glutSpaceballMotionFunc(Int3_CB f)
{
	glutSpaceballMotionFunc(f);
}
static void glui_glutSpaceballRotateFunc(Int3_CB f)
{
	glutSpaceballRotateFunc(f);
}
static void glui_glutSpaceballButtonFunc(Int2_CB f)
{
	glutSpaceballButtonFunc(f);
}
static void glui_glutTabletMotionFunc(Int2_CB f)
{
	glutTabletMotionFunc(f);
}
static void glui_glutTabletButtonFunc(Int4_CB f)
{
	glutTabletButtonFunc(f);
}
static void glui_glutWindowStatusFunc(Int1_CB f)
{
	glutWindowStatusFunc(f);
} 
static void glui_glutMenuStatusFunc(Int3_CB f)
{
	glutMenuStatusFunc(f);
}
static void glui_glutMenuStateFunc(Int1_CB f)
{
	glutMenuStateFunc(f);
}
static void glui_glutButtonBoxFunc(Int2_CB f)  
{
	glutButtonBoxFunc(f);
}
static void glui_glutDialsFunc(Int2_CB f) 
{
	glutDialsFunc(f);
}    
/*static void glui_glutKeyboardUpFunc(void(*f)(unsigned char key, int x, int y))
{
	glutKeyboardUpFunc(f);
}
static void glui_glutSpecialUpFunc(void(*f)(int key, int x, int y))
{
	glutSpecialUpFunc(f);
}*/
static void glui_glutJoystickFunc(void(*f)(unsigned int,int,int,int),int pollInterval)
{
	glutJoystickFunc(f,pollInterval);
}

static void glui_glutDestroyWindow(int wid)
{
	glutDestroyWindow(wid);
}
static void glui_glutPostWindowRedisplay(int wid)
{
	glutPostWindowRedisplay(wid);
}
static int glui_glutGet(unsigned int e)
{
	return glutGet(e);
}

namespace glui_textbox_gl_caret
{
	extern bool callback(UI::Control*,int,int,int,bool);
}
GLUI::GLUI():_ui_id_counter(1),_edited()
{
	_glui_version = GLUI_VERSION;

	prev_caret_pt = curr_caret_pt = -1; //Causing problems.

	keyboard_callback = NULL; //glut_keyboard_CB
	special_callback = NULL; //glut_special_CB
	mouse_callback = NULL; //glut_mouse_CB
	motion_callback = NULL; //glut_motion_CB
	wheel_callback = NULL;
	reshape_callback = NULL; //glut_reshape_CB

	output_callback = NULL; 
	canvas_callback = NULL; 
	create_callback = NULL;	
	caret_callback = glui_textbox_gl_caret::callback;

	_spin_growth = 0; //1;
	_spin_growth_exp = GLUI_SPIN_DEFAULT_GROWTH_EXP;

	double_click_millis = 500;
}
GLUT::GLUT()
{
	//Reminder: (void*&) isn't valid on some architectures.
	typedef void(*func)(UI::Void_CB);

	_glut_funcs1[RESHAPE] = UI::_glut_friends::reshape2;
	_glut_funcs1[KEYBOARD] = UI::_glut_friends::keyboard2;
	_glut_funcs1[KEYBOARD_UP] = UI::_glut_friends::keyboard_up2;
	_glut_funcs1[SPECIAL] = UI::_glut_friends::special2;
	_glut_funcs1[SPECIAL_UP] = UI::_glut_friends::special_up2;
	_glut_funcs1[MOUSE] = UI::_glut_friends::mouse2;
	_glut_funcs1[IDLE] = UI::_glut_friends::idle2;
	//PASS-THROUGHS
	//So glui.hpp does not have to #include glut.h.
	_glut_funcs1[DISPLAY] = (func)glui_glutDisplayFunc;
	_glut_funcs1[MOTION] = (func)glui_glutMotionFunc;
	_glut_funcs1[PASSIVE_MOTION] = (func)glui_glutPassiveMotionFunc;
	_glut_funcs1[ENTRY] = (func)glui_glutEntryFunc;
	_glut_funcs1[VISIBILITY] = (func)glui_glutVisibilityFunc;
	_glut_funcs1[TIMER] = (func)glui_glutTimerFunc;
	_glut_funcs1[OVERLAY_DISPLAY] = (func)glui_glutOverlayDisplayFunc;
	_glut_funcs1[SPACEBALL_MOTION] = (func)glui_glutSpaceballMotionFunc;
	_glut_funcs1[SPACEBALL_ROTATE] = (func)glui_glutSpaceballRotateFunc;
	_glut_funcs1[SPACEBALL_BUTTON] = (func)glui_glutSpaceballButtonFunc;
	_glut_funcs1[TABLET_MOTION] = (func)glui_glutTabletMotionFunc;
	_glut_funcs1[TABLET_BUTTON] = (func)glui_glutTabletButtonFunc;
	_glut_funcs1[WINDOW_STATUS] = (func)glui_glutWindowStatusFunc;
	_glut_funcs1[MENU_STATUS] = (func)glui_glutMenuStatusFunc;
	_glut_funcs1[MENU_STATE] = (func)glui_glutMenuStateFunc;
	_glut_funcs1[BUTTON_BOX] = (func)glui_glutButtonBoxFunc;	
	_glut_funcs1[DIALS] = (func)glui_glutDialsFunc;
	_glut_funcs1[JOYSTICK] = (func)glui_glutJoystickFunc;

	//EXPERIMENTAL
	_glut_funcs1[_glutDestroyWindow] = (func)glui_glutDestroyWindow;
	_glut_funcs1[_glutPostWindowRedisplay] = (func)glui_glutPostWindowRedisplay;
	_glut_funcs1[_glutGet] = (func)glui_glutGet;

	glui_initialized = true;
}
					  
/***************** UI::get_dims() ***********/

void UI::get_dims(int *w, int *h)
{
	if(_pending_repacking) _pack_controls();

	if(w) *w = _w; if(h) *h = _h;
}

/******************************************** GLUI_Main::display() **********/

void UI::_display()
{
	if(!_pending_idleclose) //freeglut
	{
		_pending_redisplay = false;
	}
	else return;
			
	//HACK: _init sets _main_panel to be its active control in order
	//defer activation until the controls are added.
	if(_main_panel==GLUI.active_control) activate();

	//2019: HOW CAN THIS BE RIGHT? CALLBACKS ARE SHARED!
	/* SUBTLE: on freeGLUT, the correct window is always already set.
	But older versions of GLUT need this call, or else subwindows
	don't update properly when resizing or damage-painting.
	*/
	//glutSetWindow(_glut_window_id);
	assert(_glut_window_id==glutGetWindow());

	//NOTE: This is out of sync with _w/_h.
	int win_w = glutGet(GLUT_WINDOW_WIDTH);
	int win_h = glutGet(GLUT_WINDOW_HEIGHT);
	if(_pending_repacking) _pack_controls();

	//2019: Seems wrong this way.
	/*** Check here if the window needs resizing ***/
	//if(win_w!=_main_panel->w||win_h!=_main_panel->h) 
	if(win_w!=_w||win_h!=_h) 
	{
		//TRIAL AND ERROR
		//Had a hunch this might help with freeglut but it
		//makes things worse.
		//_main_panel->redraw();

		//glutReshapeWindow(_main_panel->w,_main_panel->h);		
		glutReshapeWindow(_w,_h);
		return;
	}

	//2019: Supplementing GLUI_DRAWINGSENTINAL_IDIOM	
	_buffer_mode = glutGet(GLUT_WINDOW_DOUBLEBUFFER)?_buffer_back:_buffer_front;
	//2019: Leaving this code in place. GLUT may not
	//set glDrawBuffer itself.
	int pro_forma = _set_current_draw_buffer();
	
	//REMOVE ME
	//These are OpenGL defaults.
	/* Set up OpenGL state for widget drawing */
	//glDisable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_LIGHTING);

	/*******    Draw GLUI window     ******/
	if(!canvas_callback||canvas_callback(_main_panel,false))
	{
		float lum = bkgd_color_f[0];
		glClearColor(lum,lum,lum,1);
		glClear(GL_COLOR_BUFFER_BIT/*|GL_DEPTH_BUFFER_BIT*/); 
	}

	//REMOVE ME
	//set_ortho_projection accomplishes this.
	/*** Rotate image so y increases downward.
	In normal OpenGL, y increases upward. ***/
	//glTranslatef(win_w/2.0f,win_h/2.0f,0);
	//glRotatef(180,0,1,0);
	//glRotatef(180,0,0,1);
	//glTranslatef(-win_w/2.0f,-win_h/2.0f,0);
	set_ortho_projection();
	//Moving here from _reshape seems best.
	glViewport(0,0,_w,_h); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	bool bt = GLUI.curr_button_down;
	if(GLUI.spacebar_is_down)
	GLUI.curr_button_down = true;
	{
		//Recursively draw the main panel
		//NOTE: Weirdly Panel::_draw doesn't fill itself in, and
		//neither does _draw_recursive. (glClear() does earlier.)
		/* _main_panel->draw_bkgd_box(0,0,win_w,win_h); */
		_main_panel->draw_recursive();
	
		//HACK: Draw frame on top of children.
		if(_main_panel->box_type)
		{
			//_main_panel->draw_at_origin(); //Maybe scrollbar?
			_main_panel->draw();
		}
	}
	GLUI.curr_button_down = bt;

	//2019: In case user code want to draw before the flush */
	if(!canvas_callback||canvas_callback(_main_panel,true))
	{
		switch(_buffer_mode) 
		{
		case _buffer_front: /* Make sure drawing gets to screen */
	
			glFlush(); break;

		case _buffer_back: /* Bring back buffer to front */
	
			glutSwapBuffers(); break;
		}
	}

	//_restore_draw_buffer(pro_forma);
	glDrawBuffer(pro_forma);
}

/****************************** GLUI_Main::reshape() **************/

static void glui_reshape_timer(int wid) 
{
	int gw = glutGetWindow(); glutSetWindow(wid);

	GLUI::reshape(); //HACK: Has functionality required/access.

	glutSetWindow(gw);
}
void UI::_reshape(int reshape_w, int reshape_h)
{			
	int gw  = glutGetWindow();

	glutSetWindow(_glut_window_id);

	//2019: I don't think worked this way originally, but I can't
	//see another way for a standalone window to respond to frame
	//dragging based resize.
	if(reshape_callback) reshape_callback(this,reshape_w,reshape_h);

	_pack_controls();
				 
	if(_w!=reshape_w||_h!=reshape_h) 
	{
		glutReshapeWindow(_w,_h);
	}
	else if(_flags&SUBWINDOW)
	{		
		//2019: Nvidia GLUT isn't able to work like this.
		//if(_flags&SUBWINDOW) _check_subwindow_position();
		glutTimerFunc(0,glui_reshape_timer,_glut_parent_id);
	}

	//2019: Moving this to _display.
	//glViewport(0,0,_w,_h);

	/* printf("%d: %d\n",glutGetWindow(),_flags); */

	//Hack: use glui_control_Nvidia_timer
	//_pending_redisplay = true; glutPostRedisplay();
	_main_panel->redraw();

	glutSetWindow(gw);
}

/****************************** GLUI_Main::keyboard() **************/

bool UI::_keyboard(int key)
{
	int cm = GLUI.curr_modifiers;

	Control *new_control,*old_control = _active_control;

	//I'm all for remembering a dialog's active control, but this should
	//correspond to GLUI.active_control. I think they're one-in-the-same
	//right now, but ideally it can/will change.
	assert(old_control==GLUI.active_control);

	/*** If it's a tab or shift tab, we don't pass it on to the controls.
	Instead, we use it to cycle through active controls ***/	
	if(key=='\t' 
	//&&!_mouse_button_down //2019: Why on earth???
	/*&&(!old_control||!old_control->wants_tabs())*/)
	{
	  //TODO: Nvidia GLUT DOESN'T SEND Ctrl+Tab COMBINATIONS. USERS WILL
	  //JUST HAVE TO SEND THEM DIRECTLY. IF SO GLUI SHOULD IGNORE GLUT'S.
	  //(SOMEHOW!)

		bool c4805 = cm&GLUT_ACTIVE_CTRL;

		//2019: Without Undo feature consuming tabs is pretty unfriendly.
		if(old_control&&c4805==old_control->tab_navigate) 
		{
			goto wants_tabs;
		}
	
		//2019: This loop is for TextBox but users should want to configure
		//this behavior. TextBox::_activate returns false.
		wants_tabs2_or_loop: 

		if(cm&GLUT_ACTIVE_SHIFT) 
		{
			new_control = _find_prev_control(old_control);
		}
		else new_control = _find_next_control(old_control);

		/* if(new_control) printf("new_control: %s\n",new_control->name); */

		//2019: loop around
		if(!new_control&&_active_control&&old_control)
		{
			old_control = NULL; goto wants_tabs2_or_loop;
		}

		//deactivate_current_control();
		if(!activate_control(new_control,ACTIVATE_TAB))
		{
			old_control = new_control;
			
			if(new_control!=_active_control) goto wants_tabs2_or_loop;
		}
	}
	else if(key==' '&&!cm&&old_control
	&&old_control->spacebar_mouse_click&&!GLUI.curr_button_down)
	{
		/*** If the user presses the spacebar, and a non-edittext control
		is active, we send it a mouse down event followed by a mouse up
		event (simulated mouse-click) ***/

		//glutKeyboardUpFunc
		//GLUI::spacebar_mouse_click();		
		GLUI.init_inside =
		GLUI.curr_inside = 
		GLUI.spacebar_is_down = true; 
		GLUI.curr_cancel_down = false; 
		old_control->redraw();
	}
	else if(key==27) //ESC (CTRL('['))
	{
		assert(0); //Needs to be handled even higher up the chain.
	}
	else wants_tabs:
	{
		bool callthrough = true; //2019

		if(old_control)
		{
			/*** Pass the keystroke onto the active control, if any ***/		
			callthrough = old_control->key_handler(key,cm);
		}

		if(callthrough&&keyboard_callback)
		{
			keyboard_callback(this,key,cm);
			return true;
		}
	}

	return false;
}
void UI::_keyboard_up(int key)
{
	//Letting Mouse_Interface hijack spacebar_is_down
	//for arrow key presses. This also makes it easier
	//to unstick the spacebar if GLUT fails to send the
	//up event.
	if(GLUI.spacebar_is_down) //key==' '
	{
		GLUI.spacebar_is_down = false;

        if(Control*c=GLUI.active_control)
        {
            c->redraw();                
			if(key==' ')
			if(c->spacebar_mouse_click)
			{
				GLUI::spacebar_mouse_click(); 
			}
		} 
	}
}

/****************************** GLUI_Main::special() **************/

bool UI::_special(int key)
{	
	bool callthrough = true; //2019

	/*** Pass the keystroke onto the active control, if any ***/
	if(_active_control) callthrough = 
	_active_control->special_handler(key,GLUI.curr_modifiers);

	if(callthrough&&special_callback)
	{
		special_callback(this,key,GLUI.curr_modifiers);
		return true;
	}
	return false;
}
void UI::_special_up(int key)
{
	//Letting Mouse_Interface hijack spacebar_is_down
	//for arrow key presses. This also makes it easier
	//to unstick the spacebar if GLUT fails to send the
	//up event.
	if(GLUI.spacebar_is_down) //key==' '
	{
		GLUI.spacebar_is_down = false;

		if(GLUI.active_control) GLUI.active_control->redraw();
	}
}

/****************************** GLUI_Main::mouse() **************/

bool UI::_mouse(int button, int state, int x, int y)
{				
	/* printf("MOUSE: %d %d\n", button, state); */

	bool callthrough = true;

	if(button==GLUT_MIDDLE_BUTTON)
	{
		if(TI*ti=dynamic_cast<TI*>(GLUI.active_control))
		{
			if(TI*tj=dynamic_cast<TI*>(find_control(x,y)))
			{
				if(state==GLUT_DOWN)
				{
					ti->key_in('c',GLUT_ACTIVE_CTRL);
					tj->mouse_down_handler(x,y);
					tj->mouse_up_handler(x,y);
					tj->key_in('v',GLUT_ACTIVE_CTRL);
				}
				return false;
			}
		}		
	}
	else if(button==GLUT_LEFT_BUTTON)
	{
		Control *control = find_control(x,y);
		
		/* printf("control: %s\n",control->name.c_str()); */

		if(state==GLUT_DOWN&&control) 
		{	 				
			//CHICKEN & EGG: The control may reject the click, but it
			//causes problems if the down event doesn't have controls
			//activated. Specifically TextBox's "follow_cursor" logic
			//must not activate the box when the scrollbar is dragged.
			UI::Control *swap = GLUI.active_control;

			//Prevent double clicks.
			if(control!=swap) GLUI.prev_click_et = 0;

			//This check doesn't always work lately.
			//if(control->ui!=GLUI.active_control_ui) //NEW
			{
				glutPopWindow(); //Reveal hidden part of subwindow.
			}		

			GLUI.curr_inside =
			GLUI.prev_inside =
			GLUI.init_inside = true; //NEW
			if(activate_control(control,ACTIVATE_MOUSE))
			{
				GLUI.curr_cancel_down =
				callthrough = control->mouse_down_handler(x,y);
				if(callthrough)
				if(!swap||!swap->ui->activate_control(swap,ACTIVATE_MOUSE))
				{
					deactivate_current_control();
				}
			}	
			else 
			{
				GLUI.curr_cancel_down = true;
				GLUI.curr_button_down = false; //Prevent press appearance.					

				if(!swap||this!=GLUI.active_control_ui)
				{
					activate();
				}
			}
		}
		else if(state==GLUT_UP&&_active_control
		&&(!GLUI.curr_cancel_down||GLUI.menu_control)) 
		{
			assert(state==GLUT_UP);

			/** We just released the mouse, which was depressed at some control **/

			bool inside = control==_active_control;

			control = _active_control;
				
			GLUI.prev_inside = GLUI.curr_inside; GLUI.curr_inside = inside; //NEW

			//REMINDER: Do first so scrollbar can transfer activation to container.
			//2019: Ensure not undoing mouse_over setting.
			if(control->mouse_over(true,x,y))
			{
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			}								
			callthrough = control->mouse_up_handler(x,y,inside);				
		}  
	}	 

	//2019: This was disabled??? Users may want to implement other
	//buttons or mouse wheel.
	if(callthrough&&mouse_callback) 
	{
		mouse_callback(this,button,state,x,y);
		return true;
	}
	return false;
}

/****************************** GLUI_Main::motion() **************/

bool UI::_motion(int x, int y)
{
	/* printf("MOTION: %d %d\n",x,y); */

	bool callthrough = true;	

	if(GLUI.curr_button_down&&_active_control&&!GLUI.curr_cancel_down)
	{
		bool inside = _active_control==find_control(x,y);

		GLUI.prev_inside = GLUI.curr_inside; GLUI.curr_inside = inside; //NEW

		callthrough = _active_control->mouse_held_down_handler(x,y,inside);
	}

	//2019: This was disabled??? Users may want to implement other
	//buttons or mouse wheel.
	if(callthrough&&motion_callback)
	{
		motion_callback(this,x,y); return true;
	}
	return false;
}

/*********************** GLUI_Main::passive_motion() **************/

void UI::_passive_motion(int x, int y)
{
	Control *control = find_control(x,y);

	/* printf("%p %p\n",control,_mouse_over_control); */

	if(control!=_mouse_over_control)
	{
		if(_mouse_over_control)
		{
			_mouse_over_control->mouse_over(false,x,y);
		}
	}

	_mouse_over_control = control;

	if(!control||control->mouse_over(true,x,y))
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
}

/****************************** GLUI_Main::entry() **************/

void UI::_entry(int state)
{
	/*if(!_active_control||(_active_control
	&&(_active_control->type==UI_CONTROL_EDITTEXT
	 ||_active_control->type==UI_CONTROL_SPINNER)))*/
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
}

/****************************** GLUI_Main::idle() **************/

bool UI::_needs_idle()
{
	return _pending_idleclose
	||_active_control&&_active_control->needs_idle();
}
void UI::_idle()
{
	if(_pending_idleclose){ _close_internal(); return; }

	/*** Pass the idle event onto the active control, if any ***/

	/* printf("IDLE \t"); */

	/* First we check if the control actually needs the idle right now.
	Otherwise, let's avoid wasting cycles and OpenGL context switching */

	if(_active_control&&_active_control->needs_idle()) 
	{
		/*** Set the current glut window to the UI window */
		/*** But don't change the window if we're already at that window ***/

		if(_glut_window_id>0&&_glut_window_id!=glutGetWindow()) 
		{
			glutSetWindow(_glut_window_id);
		}

		_active_control->idle();
	}
}

/******************************************* GLUI_Main::find_control() ******/

UI::Control *UI::find_control(int x, int y)
{
	Control *last_container = NULL;

	for(Control*node=_main_panel;node;)
	{
		if(PT_IN_BOX(x,y,
		node->x_abs,node->x_abs+node->w,
		node->y_abs,node->y_abs+node->h)&&!dynamic_cast<Column*>(node))
		{
			/*** Point is inside current node ***/

			if(!node->first_child())
			{
				/* point is inside this node, and node has no children,
				so return this node as the selected node */

				return node;   
			}
			
			/*** This is a container class ***/
			last_container = node;
			
			node = (Control*)node->first_child();  /* Descend into child */			
		}
		else node = node->next();
	}

	/** No leaf-level nodes found to accept the mouse click, so
	return the last container control found which DOES accept the click **/

	if(last_container)
	{
		/* printf("ctrl: '%s'\n",last_container->name); */

		return last_container;
	}
	else return NULL;
}

/************************************* GLUI_Main::pack_controls() ***********/

void UI::_pack_controls()
{
	_main_panel->_pack(0,0); 

	_pending_repacking = false; //Also reset below...
							    
	//Need to do after to center controls in subwindow.
	/**** Now align controls within their bounds ****/
	//_main_panel->_align_children();

	int orig_w = _scroll_w = _main_panel->w;
	int orig_h = _scroll_h = _main_panel->h;

	/***  If this is a subwindow, expand panel to fit parent window  ***/
	if(_flags&SUBWINDOW) 
	{
		int orig_window = glutGetWindow();
		glutSetWindow(_glut_origin_id);
		int parent_h = glutGet(GLUT_WINDOW_HEIGHT);
		int parent_w = glutGet(GLUT_WINDOW_WIDTH); 
		glutSetWindow(orig_window);

		/* printf("%d %d\n",parent_h,parent_w); */

		int w = _main_panel->w, h = _main_panel->h;

		if(_flags&SUBWINDOW_TOP) 
		{
			w = std::max(w,parent_w);
		}
		else if(_flags&SUBWINDOW_LEFT)
		{
			h = std::max(h,parent_h);
		}
		else if(_flags&SUBWINDOW_BOTTOM)
		{
			w = std::max(w,parent_w);
		}
		else if(_flags&SUBWINDOW_RIGHT) 
		{
			h = std::max(h,parent_h);
		}		

		if(!w) return; //Indicates iconified.

		//NEW: Might cause problems?
		//This helps the frames/glutPopWindow.
		_main_panel->w = std::min(w,parent_w);
		_main_panel->h = std::min(h,parent_h);
	}

	_w = _main_panel->w; _h = _main_panel->h;

	//Do ALIGN_CENTER with full min/max size.
	/**** Now align controls within their bounds ****/
	_main_panel->_align_children();
	
	//Alignment can change the scrollable are of subwindows.
	for(Control*ch=_main_panel->last_child();ch;ch=ch->prev())
	{
		if(ch->x_abs+ch->w>orig_w) _scroll_w = _w;
		if(ch->y_abs+ch->h>orig_h) _scroll_h = _h;
	}

  //This is not required, but I want to assume repack
  //requests should be correct after one packing turn.

	_pending_repacking = false; //Also reset above.
}
 
/********************************* GLUI_Main::_set_current_draw_buffer() ********/

int UI::_set_current_draw_buffer()
{
	/* Save old buffer */
	GLint state;
	glGetIntegerv(GL_DRAW_BUFFER,&state);
	/* Switch to new buffer */
	switch(_buffer_mode)
	{
	case _buffer_front: glDrawBuffer(GL_FRONT); break;
	case _buffer_back: glDrawBuffer(GL_BACK); break; /* might not be needed... */
	}
	return (int)state;
}

/************************************* GLUI_Main::activate_control() *********/

bool UI::activate_control(Control *control, int how)
{
	if(!how) //deactivate_current_control
	{
		if(!control) control = _active_control;
		if(!control) return false;

		if(this!=control->ui){ assert(0); return false; }

		//NOTE: deactivate calls activate_control if it is activated.
		_active_control = NULL; 
		if(!control->deactivate()) //I suppose this is permissible?
		{
			_active_control = control; return false; 
		}

		/** If this isn't a container control, then redraw it in its
		deactivated state.  Container controls, such as panels, look
		the same activated or not **/

		/***
		if(!control->is_container
		||control->type==UI_CONTROL_ROLLOUT) 
		{***/
			control->redraw();
		/***
		}**/
				
		if(this==GLUI.active_control_ui)
		{
			//NEW: Manage caret to simplify text objects.
			GLUI.prev_caret_pt = GLUI.curr_caret_pt = -1;
			if(GLUI.caret_callback)
			GLUI.caret_callback(control,0,0,0,!control->enabled);	

			/*  printf("deactivate: %d\n",glutGetWindow()); */
			GLUI.active_control = NULL;
			GLUI.active_control_ui = NULL;

			GLUI::set_edited(NULL);
		}

		return true;	
	}
	 
	if(control) /*******  Now activate it   *****/
	{
		if(!control->can_activate||control->hidden) //NEW
		{
			return false; //NEW
		}
		if(!control->enabled) //NEW: OTHER implies enablement.
		{	
			if(how!=ACTIVATE_OTHER)
			{
				//Text should be highlightable/copyable. 
				//TODO: Titles/StaticText should be too.
				if(!dynamic_cast<Text_Interface*>(control))
				{
					return false;
				}
			}
			else control->enable();
		}
		if(this!=control->ui){ assert(0); return false; }

		//2019: TextBox prevents ACTIVATE_TAB activation.
		//NOTE: deactivate calls activate_control if it is deactivated.
		Control *swap = _active_control; _active_control = control;
		bool ret = control->activate(how); _active_control = swap; 
		if(!ret) return false;
			 		
		goto deactivate;
	}	
	else 
	{
		control = _find_next_control(NULL); 
		
		deactivate:
		
		/** Are we not activating a control in the same window as the
		previous active control? */
		if(GLUI.active_control_ui/*&&this!=GLUI.active_control_ui*/)
		{
			if(control!=GLUI.active_control)
			GLUI.active_control_ui->deactivate_current_control();
		}

		if(_active_control=control) control->redraw();
	}

	/* printf("activate: %d\n",glutGetWindow()); */
	GLUI.active_control = _active_control;
	GLUI.active_control_ui = _active_control?this:NULL; 
	
	return true;
}

/****************************** GLUI_Main::_find_next_control() **************/

UI::Control *UI::_find_next_control(Control *control)
{									
	if(!control) control = _main_panel;

	while(control)
	{
		/** see if this control has a child **/
		Control *tmp_control = (Control*)control->first_child();

		if(tmp_control) 
		{
			if(tmp_control->can_activate&&tmp_control->enabled)
			{
				return tmp_control;
			}

			control = tmp_control;  /* Descend into child */

			continue;
		}

		/*** At this point, control has no children ***/

		/** see if this control has a next sibling **/
		if(tmp_control=control->next())
		{
			if(tmp_control->can_activate&&tmp_control->enabled)
			{
				return tmp_control;
			}

			control = tmp_control;

			continue;
		}

		/** back up until we find a sibling of an ancestor **/

		bool back_up = true;
		while(back_up&&dynamic_cast<Control*>(control->parent())) 
		{
			control = (Control*)control->parent();

			if(control->next()) 
			{
				control = control->next();
				if(control->can_activate&&control->enabled)
				{
					return control;
				}
				
				back_up = false;

				/***	
				if(control->is_container) 
				{
					tmp_control = control;
					control = NULL;
					break;
				}
				else back_up = false;
				***/
			}
		}

		/** Check if we've cycled back to the top... if so, return NULL **/
		if(control==_main_panel) return NULL;
	}
	/*
	if(tmp_control&&tmp_control->can_activate&&tmp_control->enabled)
	{
		return tmp_control;
	}*/

	return NULL;
}

/****************************** GLUI_Main::_find_prev_control() **************/

UI::Control *UI::_find_prev_control(Control *control)
{
	Control *tmp_control, *next_control;

	if(!control) /* here we find the last valid control */
	{
		next_control = _main_panel; do 
		{
			tmp_control  = next_control;
			next_control = _find_next_control(tmp_control);

		}while(next_control);
	}
	else /* here we find the actual previous control */
	{
		next_control = _main_panel; do
		{
			tmp_control  = next_control;
			next_control = _find_next_control(tmp_control);

		}while(next_control&&next_control!=control);

		if(!next_control||tmp_control==_main_panel)
		{
			return NULL;
		}
	}

	return tmp_control;
}

/************************************************** UI::close() **********/

void UI::close(bool now, bool detach)
{
	if(_pending_idleclose) return;

	//freeglut or X servers leave dirty traces of new windows
	//if other windows are on top of them at time of creation.
	hide();

	if(detach) _glut_parent_id = -1; 
	_pending_idleclose = _pending_redisplay = true; 	
	if(!now) GLUI::glui_setIdleFuncIfNecessary(true);
	else _close_internal();
}
void UI::_close_internal()
{	
	//IMPORTANT
	//Nvidia GLUT crashes when redraw() creates timers during
	//this closing process. This suppresses glutPostRedisplay.
	assert(_pending_redisplay&&_pending_idleclose);

	deactivate_current_control();
		
	glutDestroyWindow(_glut_window_id); /** Close this window **/		
		
	assert(!_active_control);

	_unlink();

	//NEW: The last subwindow closes the non-GLUI parent window.
	//This absolves users of coordinating around GLUT's finicky
	//event manager.
	if(_glut_parent_id>0)
	if(GLUT::Window*gw=GLUT::find_glut_window(_glut_parent_id))
	{
		UI *fellow = NULL;
		for(UI*ui=GLUI::first_ui();ui;ui=ui->next())
		if(_glut_origin_id==ui->_glut_origin_id)
		{
			fellow = ui; break;
		}
		if(!fellow)
		{			
			gw->_delete();

			glutDestroyWindow(_glut_parent_id);

			//Assuming parent is non-GLUI parent of GLUI subwindow.
			assert(!GLUI::find_glui_by_window_id(_glut_parent_id));
		}
	}

	if(_main_panel) //Users may detach the main panel.
	{			
		//Reminder: _main_panel doesn't have a parent
		//(it's not parented to the UI)
		assert(!_main_panel->parent());

		_main_panel->_delete(); //delete _main_panel;

		//Users may redefine _delete in order to make
		//their UI a static variable.
		_main_panel = NULL;
	}	

	//TODO: Maybe permit a _flags value to not delete?
	//Otherwise users must implement/override _delete.

	_delete(); //delete this;
}

/********************************* GLUT::reshape() **********/
/* This gets called by the user from a GLUT reshape callback.  So we look */
/* for subwindows that belong to the current window                   */

void GLUI::reshape()
{
	int current_window = glutGetWindow();
	
	int parent_w = glutGet(GLUT_WINDOW_WIDTH);
	int parent_h = glutGet(GLUT_WINDOW_HEIGHT);

	for(UI*ui=first_ui();ui;ui=ui->next())	
	if(current_window==ui->_glut_parent_id) 
	{
		//Only GLUI::reshape should call this now.
		//glutSetWindow(ui->get_glut_window_id());
		ui->_subwindow_position(parent_w,parent_h);
	}

	glutSetWindow(current_window);
}
void UI::_subwindow_position(int parent_w, int parent_h)
{
	int new_x, new_y;
	if(_flags&SUBWINDOW_RIGHT)
	{
		new_x = parent_w-_w;
		new_y = 0;
	}
	else if(_flags&SUBWINDOW_LEFT)
	{
		new_x = new_y = 0;
	}
	else if(_flags&SUBWINDOW_BOTTOM) 
	{
		new_x = 0;
		new_y = parent_h-_h;
	}
	else if(_flags&SUBWINDOW_TOP)
	{
		new_x = new_y = 0;
	}
	else return; /* SUBWINDOW */

	/** Now make adjustments based on presence of other subwindows **/
	for(UI*ui=GLUI::first_ui();ui;ui=ui->next())		
	if(_glut_parent_id==ui->_glut_parent_id)
	{
		if(ui->_flags&SUBWINDOW_TOP
		&&_flags&(SUBWINDOW_LEFT|SUBWINDOW_RIGHT)) 
		{
			/** If we are a RIGHT or LEFT subwindow, and there exists some
			TOP subwindow, bump our position down  **/

			new_y += ui->_h;
		}

		/** Check multiple subwins at same position  **/
		/** We check the glui_id's:  only the UI with the higher
			ID number (meaning it was created later) gets bumped over **/
		if(ui!=this&&_ui_id>ui->_ui_id) 
		{
			switch(_flags&ui->_flags)
			{
			case SUBWINDOW|SUBWINDOW_LEFT: 
			
				new_x += ui->_w; break;

			case SUBWINDOW|SUBWINDOW_TOP: 
			
				new_y += ui->_h; break;

			case SUBWINDOW|SUBWINDOW_BOTTOM: 
			
				new_y -= ui->_h; break;
			
			case SUBWINDOW|SUBWINDOW_RIGHT: 
			
				new_x -= ui->_w; break;

			case SUBWINDOW: case 0: break;

			//Assuming SUBWINDOW flags don't require a mask
			default: assert(0); 
			}
		}
	}

	if(_subpos_x!=new_x||_subpos_y!=new_y)
	{	
		_subpos_x = new_x; _subpos_y = new_y;

		glutSetWindow(_glut_window_id);
		glutPositionWindow(new_x,new_y);
	}
}

/******************** GLUT::add_cb_to_glut_window() **********/

bool GLUT::_add_cb_to_glut_window(int window_id, int cb_type, UI::Void_CB cb)
{
	Window *window = find_glut_window(window_id); assert(window_id>0);
	if(!window) 
	{
		if(window_id<=0) return false;
		/***  Allocate new window structure  ***/
		window = new Window;
		window->glut_window_id = window_id;
		window->_link_this_to_parent_last(&_glut_windows);

		//2019: This way users don't have to do set_glutKeyboarFunc(NULL).		
		glutReshapeFunc(UI::_glut_friends::reshape);
		glutKeyboardFunc(UI::_glut_friends::keyboard);
		glutKeyboardUpFunc(UI::_glut_friends::keyboard_up);
		glutSpecialFunc(UI::_glut_friends::special);
		glutSpecialUpFunc(UI::_glut_friends::special_up);
		glutMouseFunc(UI::_glut_friends::mouse);
	}

	switch(cb_type) 
	{
	case IDLE:
	//NEW: Tying this idle routine to life of this window.
	window->_glut_idle_CB = (void(*)())cb;
	break;
	case RESHAPE:
	window->glut_reshape_CB = (void(*)(int,int))cb;
	break;  
	case KEYBOARD:
	window->glut_keyboard_CB = (void(*)(unsigned char,int,int))cb;
	break;
	case KEYBOARD_UP:
	window->glut_keyboard_up_CB = (void(*)(unsigned char,int,int))cb;
	break;
	case SPECIAL:
	window->glut_special_CB = (void(*)(int,int,int))cb;
	break;
	case SPECIAL_UP:
	window->glut_special_up_CB = (void(*)(int,int,int))cb;
	break;
	case MOUSE:
	window->glut_mouse_CB = (void(*)(int,int,int,int))cb;
	break;
	/*UNUSED
	case DISPLAY:
	window->glut_display_CB = (void(*)())cb;
	break;
	case MOTION:
	window->glut_motion_CB = (void(*)(int,int))cb;
	break;
	case PASSIVE_MOTION:
	window->glut_passive_motion_CB = (void(*)(int,int))cb;
	break;
	case ENTRY:
	window->glut_entry_CB = (void(*)(int))cb;
	break;
	case VISIBILITY:
	window->glut_visibility_CB = (void(*)(int))cb;
	break;
	*/
	default: assert(0); case _INIT_PARENT: break;
	}
	return true;
}

/****************************** GLUI_Main::refresh() ****************/

void UI::refresh()
{
	//2019: There was code here, but it was identical to reshape.

	_reshape(_w,_h);
}

/***************** GLUT::get_main_gfx_viewport() ***********/

void GLUI::get_viewport_area(int *x, int *y, int *ww, int *hh)
{	
	int curr_x = 0;
	int curr_y = 0;
	int curr_w = glutGet(GLUT_WINDOW_WIDTH);
	int curr_h = glutGet(GLUT_WINDOW_HEIGHT);

	int current_window = glutGetWindow();

	for(UI*ui=first_ui();ui;ui=ui->next())	
	if(current_window==ui->_glut_parent_id) 
	{
		if(ui->_flags&UI::_HIDDEN) continue; //2019

		int w,h; ui->get_dims(&w,&h); //repack

		/* printf("%s -> %d   %d %d\n",ui->window_name.c_str(),ui->_flags,w,h); */

		if(ui->_flags&UI::SUBWINDOW_LEFT)
		{
			curr_x+=w; curr_w-=w;
		}
		else if(ui->_flags&UI::SUBWINDOW_BOTTOM)
		{
			curr_y+=h; curr_h-=h;
		}
		else if(ui->_flags&UI::SUBWINDOW_RIGHT)
		{
			curr_w-=w;
		}
		else if(ui->_flags&UI::SUBWINDOW_TOP)
		{
			curr_h-=h;
		}	
	}

	curr_x = std::max(0,curr_x);
	curr_y = std::max(0,curr_y);
	curr_w = std::max(0,curr_w);
	curr_h = std::max(0,curr_h);

	*x = curr_x; *y = curr_y; *ww = curr_w; *hh = curr_h;
}
 
/*****************GLUT::auto_set_main_gfx_viewport() **********/

void GLUI::auto_set_viewport()
{
	int x, y, w, h;
	get_viewport_area(&x,&y,&w,&h);
	glViewport(std::max(x,0),std::max(y,0),std::max(w,0),std::max(h,0));
}

/***************************************** UI::show() **********************/

void UI::show(bool sw)
{
	int orig = glutGetWindow();
	glutSetWindow(_glut_window_id); 
	{
		(sw?glutShowWindow:glutHideWindow)();

		if(sw) _flags&=~_HIDDEN;
		if(!sw) _flags|=_HIDDEN;
	}
	if(orig>0) glutSetWindow(orig);
}

/******************************** GLUI::set_edited() **********************/

static Live_Variable glui_edited;
static std::vector<double> glui_edited_float_array;
void GLUI::_set_edited(UI::Control *c)
{	
	if(!c){ _edited = NULL; return; } //Clear?

	if(_edited||c!=active_control) return; //Set/not owner?

	_edited = &glui_edited; glui_edited = *c;

	int fas = c->float_array_size; if(fas>=1)
	{
		std::vector<double> &fa = glui_edited_float_array;
		fa.resize(c->float_array_size);
		memcpy(glui_edited.float_array_val=&fa[0],c->float_array_val,fas*sizeof(double));
	}		
	else glui_edited.float_array_val = &glui_edited.float_val;
}

/************* GLUI::glui_setIdleFuncIfNecessary() ***********************/

void GLUI::glui_setIdleFuncIfNecessary(bool necessary)
{	
	if(!necessary)
	{
		//NEW: Tying idle routines to life of windows/facilitating 
		//multiple idle routines.
		//if(!GLUT._glut_idle_CB)
		for(GLUT::Window*gw=GLUT::first_glut_window();gw;gw=gw->next())
		{
			if(gw->_glut_idle_CB) necessary = true;
		}
		if(!necessary) for(UI*ui=GLUI::first_ui();ui;ui=ui->next())		
		{
			if(ui->_needs_idle()) necessary = true;
		}
	}
	glutIdleFunc(necessary?UI::_glut_friends::idle:NULL);
}

/**************************************** UI::wheel() **********************/

static bool glui_wheel(UI::Control *c, UI::Control *c2, int i, int modifiers)
{
	UI::Control *wh = NULL; if(c2)
	{
		if(Spin_Interface*si=dynamic_cast<Spin_Interface*>(c2))
		{
			if(c==si->associated_object)
			{

		//TODO: Here some logic needs to reset the "growth"
		//factors for the spinner like a regular activation. 

				wh = si;
			}
		}
	}
	else wh = c;

	if(!wh||!wh->enabled) return true;

	bool out = false;
	int orig = glutGetWindow();
	glutSetWindow(c->ui->get_glut_window_id());
	int k = i<0?GLUT_KEY_UP:GLUT_KEY_DOWN;
	if(modifiers&UI::WHEEL_HORZ)
	{
		//k--; //GLUT_KEY_LEFT:GLUT_KEY_RIGHT
		k = i<0?GLUT_KEY_RIGHT:GLUT_KEY_LEFT;
		modifiers&=~UI::WHEEL_HORZ;
	}
	int swap = GLUI.curr_modifiers;
	GLUI.curr_modifiers = modifiers;
	{	
		GLUI.wheel_event = true;
		{
			if(!wh->special_handler(k,modifiers)) 
			for(i=ABS(i)-1;i-->0;) 
			{
				wh->special_handler(k,modifiers);
			}
			else out = true;
		}
		GLUI.wheel_event = false;
	}
	GLUI.curr_modifiers = swap;
	glutSetWindow(orig); return out;
}
bool UI::_wheel(int i, Control *c, int modifiers)
{
	if(!c||!i) return true;

	if(Control*wh=c->wheel())
	{
		if(!glui_wheel(wh,0,i,modifiers))
		return false;
	}
	
	//Spin_Interface search.
	Control *cp = c->prev();
	Control *cn = c->next();
	for(int enough=4;enough-->0;)
	{
		if(enough-->0) 
		if(cn)
		{
			if(!glui_wheel(c,cn,i,modifiers))
			return false;

			cn = cn->next();
		}
		if(cp) 
		{
			if(!glui_wheel(c,cp,i,modifiers))
			return false;

			cp = cp->prev();
		}
	}

	//Try to use special_handler.
	if(!glui_wheel(c,0,i,modifiers))
	return false;

	//RECURSIVE: Go up to parent.
	return _wheel(i,c->has_parent(),modifiers);
}

//---.
}//<-'
