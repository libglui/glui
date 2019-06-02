
#ifdef GLUI_BUILD_EXAMPLES

//REMOVING GLU DEPENDENCY
//(Demonstrate GLUI doesn't touch GLU internally.)
#define __glu_h__
#define __GLU_H__

#define GLUI_INTERNAL_H //GLUI_DLL
#include "../../include/GL/glui.h"

	//FOREWORD
	//These are old examples (1998) demonstrating back-compatibility
	//with the glui.h header. There is also new methodology where it
	//is necessary to modify the demos.

static void glui_example_switch(int i, int pid)
{
	//These examples have a lot of different combinations.
	//They originally were standalone programs that would
	//use exit to terminate the program.
	int esc = 0;
	if(GLUI*ui=GLUI_Master.find_glui_by_window_id(pid))
	{
		esc = ui->get_glut_escape_id();
		pid = ui->get_glut_origin_id();
	}		
	else esc = pid; if(esc>0)
	{
		glutSetWindow(esc);
		glutInitWindowPosition(glutGet(GLUT_WINDOW_X),glutGet(GLUT_WINDOW_Y));			
	}
	if(!i||!GLUI_Library::glui_example(i))
	{		
		if(!GLUI_Master.close_glui_by_origin_id(pid)||esc!=pid)
		{			
			if(!GLUI_Master.destroy_glut_window(esc))
			{
				if(esc>0) glutDestroyWindow(esc);
			}			
		}
		GLUI_Master.close_glui_by_escape_id(esc);		
	}	
}
static void glui_example_special(int key, int, int)
{
	if(key>=1&&key<=12) //GLUT_KEY_F1(2)
	{
		glui_example_switch(key,glutGetWindow());
	}
}
static bool glui_example_special(GLUI *ui, int key, int)
{
	if(key>=1&&key<=12) //GLUT_KEY_F1(2)
	{
		glui_example_switch(key,ui->get_glut_window_id());	
		return false;
	}
	return true;
}			

namespace //some common variables (that don't record state)
{
	enum
	{
		//#3
		OPEN_CONSOLE_ID     = 100,
		CMD_HIST_RESET_ID   = 101,
		CMD_CLOSE_ID        = 102,
		LIGHT0_ENABLED_ID   = 200,
		LIGHT1_ENABLED_ID   = 201,
		LIGHT0_INTENSITY_ID = 250,
		LIGHT1_INTENSITY_ID = 251,
		//#4
		//LIGHT0_ENABLED_ID   = 200,
		//LIGHT1_ENABLED_ID   = 201,
		//LIGHT0_INTENSITY_ID = 250,
		//LIGHT1_INTENSITY_ID = 260,
		ENABLE_ID           = 300,
		DISABLE_ID          = 301,
		SHOW_ID             = 302,
		HIDE_ID             = 303,
		//#5
		//LIGHT0_ENABLED_ID   = 200,
		//LIGHT1_ENABLED_ID   = 201,
		//LIGHT0_INTENSITY_ID = 250,
		//LIGHT1_INTENSITY_ID = 251,
	};
	static float xy_aspect;
	static int last_x,last_y;
	static const GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
	static const GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
	static const GLfloat light0_position[] = {0.5f,0.5f,1,0};
	static const GLfloat light1_ambient[] = {0.1f,0.1f,0.3f,1};
	static const GLfloat light1_diffuse[] = {0.9f,0.6f,0,1};
	static const GLfloat light1_position[] = {-1,-1,1,0};
	static struct main_window 
	:
	GLUI_Library::Window //font
	{
		int id; //int main_window;

		operator int&(){ return id; }

	}main_window;

	static void quit_cb(int)
	{
		glui_example_switch(0,main_window);
	}

	/************************************************** draw_axes() **********/

	/* Disables lighting, then draws RGB axes                                */

	static void draw_axes(float scale)
	{
		glDisable(GL_LIGHTING);

		glPushMatrix();
		glScalef(scale,scale,scale);

		glBegin(GL_LINES);

		glColor3f(1,0,0);
		glVertex3f(0.8f,0.05f,0);
		glVertex3f(1,0.25f,0);/* Letter X*/
		glVertex3f(0.8f,0.25f,0);
		glVertex3f(1,0.05f,0);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);/* X axis */

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,1,0);/* Y axis */

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1);/* Z axis  */
		glEnd();

		glPopMatrix();

		glEnable(GL_LIGHTING);
	}
}
namespace glui_example_1 //EXAMPLE #1
{
	/****************************************************************************

	  A simple GLUT program using the GLUI User Interface Library

	  This program sets up a checkbox and a spinner, both with live variables.
	  No callbacks are used.

	  -----------------------------------------------------------------------
	   
	  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

	 ****************************************************************************/

	/** These are the live variables passed into GLUI ***/
	static int wireframe = 0;
	static int segments = 8;

	//static int main_window;
	static float rotationX = 0, rotationY = 0;

	/***************************************** myGlutIdle() ***********/

	static void myGlutIdle()
	{
		/* According to the GLUT specification, the current window is 
		   undefined during an idle callback.  So we need to explicitly change
		   it if necessary */
		glutSetWindow(main_window);

		//2019: Hold freeglut to reasonable frame rate so not to degrade demo.
		static int et = 0;
		int now = glutGet(GLUT_ELAPSED_TIME);
		if(now-et>10){ et = now; glutPostRedisplay(); }
	}

	/**************************************** myGlutReshape() *************/

	static void myGlutReshape(int x, int y)
	{
		float xy_aspect = (float)x/y;
		glViewport(0,0,x,y);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-xy_aspect*0.08,xy_aspect*0.08,-0.08,0.08,0.1,15);

		glutPostRedisplay();
	}

	/***************************************** myGlutDisplay() *****************/

	static void myGlutDisplay()
	{	
		glClearColor(0.9f,0.9f,0.9f,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		/*** Rotate the object ***/
		rotationX += 3.3f;
		rotationY += 4.7f;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-1);
		glRotatef(rotationY,0,1,0);
		glRotatef(rotationX,1,0,0);

		/*** Now we render object, using the variables 'segments' and
		  'wireframe'.  These are _live_ variables, which are transparently 
		  updated by GLUI ***/

		(wireframe?glutWireTorus:glutSolidTorus)(0.2,0.5,16,segments);

		glutSwapBuffers();
	}

	/**************************************** main() ********************/

	static void main(bool quit)  //#1
	{
		/****************************************/
		/*   Initialize GLUT and create window  */
		/****************************************/

		//glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
		//glutInitWindowPosition(50,50);
		glutInitWindowSize(300,300);

		main_window.id = glutCreateWindow("GLUI Example 1 (F1)");
		GLUI_Master.set_glutDisplayFunc(myGlutDisplay);
		GLUI_Master.set_glutReshapeFunc(myGlutReshape);		
		GLUI_Master.set_glutIdleFunc(myGlutIdle);
		GLUI_Master.set_glutSpecialFunc(glui_example_special);

		/****************************************/
		/*       Set up OpenGL lights           */
		/****************************************/

		//GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
		//GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
		const GLfloat light0_position[] = {1,1,1,0};

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		/****************************************/
		/*          Enable z-buferring          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);

		/****************************************/
		/*         Here's the GLUI code         */
		/****************************************/

		GLUI *glui = GLUI_Master.create_glui("GLUI (F1)",0,
		glutGet(GLUT_WINDOW_X)+400,glutGet(GLUT_WINDOW_Y)+50);
		glui->set_main_gfx_window(main_window);

		new GLUI_Checkbox(glui,"Wireframe",&wireframe);

		//2019: Making room for a title bar on Windows 10.
		new GLUI_Column(glui); 

		GLUI_Spinner *spinner = new GLUI_Spinner(glui,"Segments",&segments);
		spinner->set_int_limits(3,60);

		if(quit) new GLUI_Button(glui,"Quit",0,quit_cb);

		//glutMainLoop(); return EXIT_SUCCESS;
	}
}
namespace glui_example_2 //EXAMPLE #2
{
	/****************************************************************************

	  example2.cpp

	  A simple GLUT program using the GLUI User Interface Library

	  -----------------------------------------------------------------------
	   
	  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

	 ****************************************************************************/

	//static float xy_aspect;
	//static int last_x,last_y;
	static float rotationX = 0,rotationY = 0;
	//static int main_window;

	/** These are the live variables passed into GLUI ***/
	static int wireframe = 0;
	static int obj = 0;
	static int segments = 8;

	// Using a std::string as a live variable is safe.
	static std::string text = "Hello World!";

	// Using a char buffer as a live var is also possible, but it is dangerous 
	// because GLUI doesn't know how big your buffer is.  
	// But still, it works as long as text doesn't happen to overflow.
	//char  text[200] = {"Hello World!"};

	static GLUI_Checkbox *checkbox;
	static GLUI_Spinner *spinner;
	static GLUI_RadioGroup *radio;
	static GLUI_EditText *edittext;

	/**************************************** control_cb() *******************/

	/* GLUI control callback                                                 */

	static void control_cb(int control)
	{		
		/********************************************************************
		  Here we'll print the user id of the control that generated the
		  callback, and we'll also explicitly get the values of each control.
		  Note that we really didn't have to explicitly get the values, since
		  they are already all contained within the live variables:
		  'wireframe',  'segments',  'obj',  and 'text'  
		 ********************************************************************/

		printf("callback: %d\n",control);
		printf("             checkbox: %d\n",checkbox->get_int_val());
		printf("              spinner: %d\n",spinner->get_int_val());
		printf("          radio group: %d\n",radio->get_int_val());
		printf("                 text: %s\n",edittext->get_text());
	}

	/**************************************** myGlutKeyboard() **********/

	static void myGlutKeyboard(unsigned char Key, int x, int y)
	{
		switch(Key)
		{
			// A few keys here to test the sync_live capability.
		case 'o':
			// Cycle through object types
			++obj %= 3;
			GLUI_Master.sync_live_all();
			break;
		case 'w':
			// Toggle wireframe mode
			wireframe = !wireframe;
			GLUI_Master.sync_live_all();
			break;
		}
		glutPostRedisplay();
	}

	/***************************************** myGlutMouse() **********/

	static void myGlutMouse(int button, int button_state, int x, int y)
	{
		if(button==GLUT_LEFT_BUTTON&&button_state==GLUT_DOWN)
		{
			last_x = x; last_y = y;
		}
	}

	/***************************************** myGlutMotion() **********/

	static void myGlutMotion(int x, int y)
	{
		rotationX += (float)y-last_y;
		rotationY += (float)x-last_x;

		last_x = x; last_y = y;

		glutPostRedisplay();
	}

	/**************************************** myGlutReshape() *************/

	static void myGlutReshape(int x, int y)
	{
		xy_aspect = (float)x/y; glViewport(0,0,x,y);

		glutPostRedisplay();
	}

	/***************************************** myGlutDisplay() *****************/

	static void myGlutDisplay()
	{
		glClearColor(0.9f,0.9f,0.9f,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-xy_aspect*0.08,xy_aspect*0.08,-0.08,0.08,0.1,15);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-1.6f);
		glRotatef(rotationY,0,1,0);
		glRotatef(rotationX,1,0,0);

		/*** Now we render object, using the variables 'obj', 'segments', and
		  'wireframe'.  These are _live_ variables, which are transparently 
		  updated by GLUI ***/

		if(obj==0)
		{
			(wireframe?glutWireSphere:glutSolidSphere)(0.6,segments,segments);
		}
		else if(obj==1)
		{
			(wireframe?glutWireTorus:glutSolidTorus)(0.2,0.5,16,segments);
		}
		else if(obj==2)
		{
			(wireframe?glutWireTeapot:glutSolidTeapot)(0.5);
		}

		glDisable(GL_LIGHTING);/* Disable lighting while we render text */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,100,0,100,-1,1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3ub(0,0,0);
		
		//  printf( "text: %s\n", text );

		/*** Render the live character array 'text' ***/
		//glRasterPos2i(10,10);
		//for(size_t i=0;i<text.length();++)
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
		main_window.draw_string(10,-25,text);		

		glEnable(GL_LIGHTING);

		glutSwapBuffers();
	}

	/**************************************** main() ********************/

	static void main(bool quit) //#2
	{
		main_window.set_font(GLUT_BITMAP_HELVETICA_18);

		/****************************************/
		/*   Initialize GLUT and create window  */
		/****************************************/

		//glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
		//glutInitWindowPosition(50,50);
		glutInitWindowSize(300,300);

		main_window.id = glutCreateWindow("GLUI Example 2 (F2)");
		GLUI_Master.set_glutDisplayFunc(myGlutDisplay);
		GLUI_Master.set_glutReshapeFunc(myGlutReshape);
		GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
		GLUI_Master.set_glutMotionFunc(myGlutMotion);
		GLUI_Master.set_glutMouseFunc(myGlutMouse);
		GLUI_Master.set_glutSpecialFunc(glui_example_special);

		/****************************************/
		/*       Set up OpenGL lights           */
		/****************************************/

		//GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
		//GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
		const GLfloat light0_position[] = {1,1,1,0};

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		/****************************************/
		/*          Enable z-buferring          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);

		/****************************************/
		/*         Here's the GLUI code         */
		/****************************************/

		GLUI *glui = GLUI_Master.create_glui("GLUI (F2)",0,//400,50
		glutGet(GLUT_WINDOW_X)+400,glutGet(GLUT_WINDOW_Y)+50);
		glui->set_main_gfx_window(main_window);

		new GLUI_StaticText(glui,"GLUI Example 2");
		new GLUI_Separator(glui);
		checkbox = new GLUI_Checkbox(glui,"Wireframe",&wireframe,1,control_cb);
		spinner = new GLUI_Spinner(glui,"Segments",&segments,2,control_cb);
		spinner->set_int_limits(3,60);
		spinner->right();
		edittext = new GLUI_EditText(glui,"Text",text,3,control_cb);
		edittext->set_w(150);

		//2019: Making room for a title bar on Windows 10.
		new GLUI_Column(glui); 

		GLUI_Panel *obj_panel = new GLUI_Panel(glui,"Object Type");
		radio = new GLUI_RadioGroup(obj_panel,&obj,4,control_cb);
		new GLUI_RadioButton(radio,"Sphere");
		new GLUI_RadioButton(radio,"Torus");
		new GLUI_RadioButton(radio,"Teapot");
		
		if(quit)
		new GLUI_Button(glui,"Quit",0,quit_cb);

		//glutMainLoop(); return EXIT_SUCCESS;
	}
}
namespace glui_example_3 //EXAMPLE #3
{
	/****************************************************************************

	  example3.cpp

	  A GLUT program using all the features of the GLUI User Interface Library
	  (except columns, featured in example4.cpp)

	  -----------------------------------------------------------------------
	   
	  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

	 ****************************************************************************/

	static float rotationX = 0,rotationY = 0;

	/** These are the live variables passed into GLUI ***/
	static int wireframe = 0;
	static int obj_type = 1; //Torus
	static int segments = 8;
	static int segments2 = 8;
	static std::string text = "Hello World!";
	static int light0_enabled = 1;
	static int light1_enabled = 0;
	static float light0_intensity = 1;
	static float light1_intensity = 1;
	//static int main_window;
	static int counter = 0;
	static float scale = 1;

	/** Pointers to the windows and some of the controls we'll create **/
	static GLUI *cmd_line_glui = 0,*glui;
	static GLUI_Checkbox *checkbox;
	static GLUI_Spinner *spinner,*light0_spinner,*light1_spinner,*scale_spinner;
	static GLUI_RadioGroup *radio;
	static GLUI_EditText *edittext;
	static GLUI_CommandLine *cmd_line;
	static GLUI_Panel *obj_panel;
	static GLUI_Button *open_console_btn;

	/********** Miscellaneous global variables **********/

	//static GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
	//static GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
	//static GLfloat light0_position[] = {0.5f,0.5f,1,0};

	//static GLfloat light1_ambient[] = {0.1f,0.1f,0.3f,1};
	//static GLfloat light1_diffuse[] = {0.9f,0.6f,0,1};
	//static GLfloat light1_position[] = {-1,-1,1,0};

	/**************************************** control_cb() *******************/

	/* GLUI control callback                                                 */

	static void control_cb(int control)
	{
		if(control==LIGHT0_ENABLED_ID)
		{
			if(light0_enabled)
			{
				glEnable(GL_LIGHT0);
				light0_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT0);
				light0_spinner->disable();
			}
		}
		else if(control==LIGHT1_ENABLED_ID)
		{
			if(light1_enabled)
			{
				glEnable(GL_LIGHT1);
				light1_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT1);
				light1_spinner->disable();
			}
		}
		else if(control==LIGHT0_INTENSITY_ID)
		{
			float v[] = 
			{
				light0_diffuse[0],light0_diffuse[1],
				light0_diffuse[2],light0_diffuse[3]
			};

			v[0] *= light0_intensity;
			v[1] *= light0_intensity;
			v[2] *= light0_intensity;

			glLightfv(GL_LIGHT0,GL_DIFFUSE,v);
		}
		else if(control==LIGHT1_INTENSITY_ID)
		{
			float v[] = 
			{
				light1_diffuse[0],light1_diffuse[1],
				light1_diffuse[2],light1_diffuse[3]
			};

			v[0] *= light1_intensity;
			v[1] *= light1_intensity;
			v[2] *= light1_intensity;

			glLightfv(GL_LIGHT1,GL_DIFFUSE,v);
		}
	}

	/**************************************** pointer_cb() *******************/
	/* GLUI control pointer callback                                         */
	/* You can also use a function that takes a GLUI_Control pointer  as its */
	/* argument.  This can simplify things sometimes, and reduce the clutter */
	/* of global variables by giving you the control pointer directly.       */
	/* For instance here we didn't need an additional global ID for the      */

	/* cmd_line because we can just compare pointers directly.               */
		
	static void pointer_cb(GLUI_Control *control)
	{
		int id = control->get_id();

		if(id==OPEN_CONSOLE_ID)
		{
			/****** Make command line window ******/
			cmd_line_glui = GLUI_Master.create_glui("Enter command",0, //50,500
			glutGet(GLUT_WINDOW_X)+100,glutGet(GLUT_WINDOW_Y)+400);
			cmd_line = new GLUI_CommandLine(cmd_line_glui,"Command (try 'exit')",NULL,-1,pointer_cb);
			cmd_line->set_w(400);/** Widen 'command line' control **/

			GLUI_Panel *panel = new GLUI_Panel(cmd_line_glui,"",GLUI_PANEL_NONE);
			new GLUI_Button(panel,"Clear History",CMD_HIST_RESET_ID,pointer_cb);
			new GLUI_Column(panel,false);
			new GLUI_Button(panel,"Close",CMD_CLOSE_ID,pointer_cb);

			cmd_line_glui->set_main_gfx_window(main_window);
			cmd_line_glui->special_callback = glui_example_special;

			control->disable();
		}
		else if(id==CMD_CLOSE_ID) exit:
		{
			open_console_btn->enable();
			control->glui->close();
		}
		else if(control==cmd_line)
		{
			/*** User typed text into the 'command line' window ***/
			printf("Command (%d): %s\n",counter,cmd_line->get_text());
			std::string text = cmd_line->get_text();
			if(text=="exit"||text=="quit")
			{
				goto exit; //exit(0); //Close window instead of Quit.
			}
		}
		else if(id==CMD_HIST_RESET_ID)
		{
			cmd_line->reset_history();
		}
	}

	/**************************************** myGlutKeyboard() **********/

	static void myGlutKeyboard(unsigned char Key, int x, int y)
	{
		switch(Key)
		{
			// A few keys here to test the sync_live capability.
		case 'o':
			// Cycle through object types
			++obj_type %= 3;
			GLUI_Master.sync_live_all();
			break;
		case 'w':
			// Toggle wireframe mode
			wireframe = !wireframe;
			GLUI_Master.sync_live_all();
			break;
		//case 27:
		//case 'q': exit(0); break;
		}

		glutPostRedisplay();
	}

	/***************************************** myGlutIdle() ***********/

	static void myGlutIdle()
	{
		//These aren't used.
		/* According to the GLUT specification, the current window is 
		   undefined during an idle callback.  So we need to explicitly change
		   it if necessary */
		//glutSetWindow(main_window);
		//glutPostRedisplay(); 

		/****************************************************************/
		/*            This demonstrates GLUI::sync_live()               */
		/*   We change the value of a variable that is 'live' to some   */
		/*   control.  We then call sync_live, and the control          */
		/*   associated with that variable is automatically updated     */
		/*   with the new value.  This frees the programmer from having */
		/*   to always remember which variables are used by controls -  */
		/*   simply change whatever variables are necessary, then sync  */
		/*   the live ones all at once with a single call to sync_live  */
		/****************************************************************/

		counter++; glui->sync_live();
	}

	/***************************************** myGlutMouse() **********/

	static void myGlutMouse(int button, int button_state, int x, int y)
	{
		if(button==GLUT_LEFT_BUTTON&&button_state==GLUT_DOWN)
		{
			last_x = x; last_y = y;
		}
	}

	/***************************************** myGlutMotion() **********/

	static void myGlutMotion(int x, int y)
	{
		rotationX += (float)y-last_y;
		rotationY += (float)x-last_x;

		last_x = x; last_y = y;

		glutPostRedisplay();
	}

	/**************************************** myGlutReshape() *************/

	static void myGlutReshape(int x, int y)
	{
		xy_aspect = (float)x/y; glViewport(0,0,x,y);

		glutPostRedisplay();
	}

	/***************************************** myGlutDisplay() *****************/

	static void myGlutDisplay()
	{
		glClearColor(0.9f,0.9f,0.9f,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-xy_aspect*0.08,xy_aspect*0.08,-0.08,0.08,0.1,15);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-1.2f);
		glRotatef(rotationY,0,1,0);
		glRotatef(rotationX,1,0,0);

		glScalef(scale,scale,scale);

		/*** Now we render object, using the variables 'obj_type', 'segments', and
		  'wireframe'.  These are _live_ variables, which are transparently 
		  updated by GLUI ***/

		if(obj_type==0)
		{
			(wireframe?glutWireSphere:glutSolidSphere)(0.6,segments,segments);
		}
		else if(obj_type==1)
		{
			(wireframe?glutWireTorus:glutSolidTorus)(0.2,0.5,16,segments);
		}
		else if(obj_type==2)
		{
			(wireframe?glutWireTeapot:glutSolidTeapot)(0.5);
		}

		/* Disable lighting and set up ortho projection to render text */
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,100,0,100,-1,1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3ub(0,0,0);

		/*** Render the live character array 'text' ***/
		//glRasterPos2i(10,10);
		//for(size_t i=0;i<text.length();++)
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
		main_window.draw_string(10,-25,text);		

		glEnable(GL_LIGHTING);

		glutSwapBuffers();
	}

	/**************************************** main() ********************/

	static void main(bool quit) //#3
	{
		main_window.set_font(GLUT_BITMAP_HELVETICA_18);

		/****************************************/
		/*   Initialize GLUT and create window  */
		/****************************************/

		//glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
		//glutInitWindowPosition(50,50);
		glutInitWindowSize(300,300);

		main_window.id = glutCreateWindow("GLUI Example 3 (F3)");
		GLUI_Master.set_glutDisplayFunc(myGlutDisplay);
		GLUI_Master.set_glutReshapeFunc(myGlutReshape);
		GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);		
		GLUI_Master.set_glutMotionFunc(myGlutMotion);
		GLUI_Master.set_glutMouseFunc(myGlutMouse);		
		GLUI_Master.set_glutIdleFunc(myGlutIdle); 
		GLUI_Master.set_glutSpecialFunc(glui_example_special);

		/****************************************/
		/*       Set up OpenGL lights           */
		/****************************************/

		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
		glLightfv(GL_LIGHT1,GL_POSITION,light1_position);

		/****************************************/
		/*          Enable z-buferring          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);

		/****************************************/
		/*         Here's the GLUI code         */
		/****************************************/

		//printf("GLUI version: %3.2f\n",GLUI_Master.get_version());
				
		glui = GLUI_Master.create_glui("GLUI (F3)",0,//400,50
		glutGet(GLUT_WINDOW_X)+400,glutGet(GLUT_WINDOW_Y)+50);
		glui->set_main_gfx_window(main_window);

		//new GLUI_StaticText(glui,"GLUI Example 3 (F3)");
		obj_panel = new GLUI_Panel(glui,"Object");

		/***** Control for the object type *****/

		GLUI_Panel *type_panel = new GLUI_Panel(obj_panel,"Type");
		radio = new GLUI_RadioGroup(type_panel,&obj_type,4,control_cb);
		new GLUI_RadioButton(radio,"Sphere");
		new GLUI_RadioButton(radio,"Torus");
		new GLUI_RadioButton(radio,"Teapot");

		checkbox = new GLUI_Checkbox(obj_panel,"Wireframe",&wireframe,1,control_cb);
		spinner = new GLUI_Spinner(obj_panel,"Segments",&segments);
		spinner->set_int_limits(3,60);
		spinner->set_alignment(GLUI_ALIGN_RIGHT);

		scale_spinner = new GLUI_Spinner(obj_panel,"Scale",&scale);
		scale_spinner->set_float_limits(0.2f,4);
		scale_spinner->set_alignment(GLUI_ALIGN_RIGHT);

		new GLUI_Separator(obj_panel);
		edittext = new GLUI_EditText(obj_panel,"Text",text);
		edittext->set_w(150);

		//2019: Making room for a title bar on Windows 10.
		new GLUI_Column(glui,0); 

		/******** Add some controls for lights ********/

		GLUI_Panel *light0 = new GLUI_Panel(glui,"Light 1");
		GLUI_Panel *light1 = new GLUI_Panel(glui,"Light 2");

		new GLUI_Checkbox(light0,"Enabled",&light0_enabled,LIGHT0_ENABLED_ID,control_cb);
		light0_spinner = new GLUI_Spinner(light0,"Intensity",&light0_intensity,LIGHT0_INTENSITY_ID,control_cb);
		light0_spinner->right();
		light0_spinner->set_float_limits(0,1);

		new GLUI_Checkbox(light1,"Enabled",&light1_enabled,
		LIGHT1_ENABLED_ID,control_cb);
		light1_spinner = new GLUI_Spinner(light1,"Intensity",&light1_intensity,LIGHT1_INTENSITY_ID,control_cb);
		light1_spinner->right();
		light1_spinner->set_float_limits(0,1);
		light1_spinner->disable();/* Disable this light initially */		

		/****** Add a grayed-out counter *****/

		GLUI_EditText *counter_edittext = new GLUI_EditText(glui,"Count",&counter);
		counter_edittext->disable();

		/****** Button to Open Command Line Window ******/
		open_console_btn =
		new GLUI_Button(glui,"Open Console",OPEN_CONSOLE_ID,pointer_cb);

		if(quit) /****** A 'quit' button *****/
		{		
			//new GLUI_Button(glui,"Quit",0,(GLUI_Update_CB)exit);
			new GLUI_Button(glui,"Quit",0,quit_cb);
		}
	}
}
namespace glui_example_4 //EXAMPLE #4
{
	/****************************************************************************

	example4.cpp

	A GLUI program demonstrating nested columns

	-----------------------------------------------------------------------
	   
	10/2/98 Paul Rademacher (rademach@cs.unc.edu)

	****************************************************************************/
 
	//static float xy_aspect;
	//static int last_x,last_y;
	static float rotationX = 0,rotationY = 0;

	/** These are the live variables passed into GLUI ***/
	static int wireframe = 0;
	static int obj_type = 1;
	static int segments = 8;
	static int segments2 = 8;
	static int light0_enabled = 1;
	static int light1_enabled = 0;
	static float light0_intensity = 1;
	static float light1_intensity = 1;
	//static int main_window;
	static float scale = 1;
	static int show_sphere = 1;
	static int show_torus = 1;

	/** Pointers to the windows and some of the controls we'll create **/
	static GLUI *glui;
	static GLUI_Checkbox *checkbox;
	static GLUI_Spinner *spinner,*light0_spinner,*light1_spinner;
	static GLUI_RadioGroup *radio;
	static GLUI_Panel *obj_panel;

	/********** User IDs for callbacks ********/
	/*
	#define LIGHT0_ENABLED_ID    200
	#define LIGHT1_ENABLED_ID    201
	#define LIGHT0_INTENSITY_ID  250
	#define LIGHT1_INTENSITY_ID  251
	*/

	/********** Miscellaneous global variables **********/

	//static GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
	//static GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
	//static GLfloat light0_position[] = {0.5f,0.5f,1,0};

	//static GLfloat light1_ambient[] = {0.1f,0.1f,0.3f,1};
	//static GLfloat light1_diffuse[] = {0.9f,0.6f,0,1};
	//static GLfloat light1_position[] = {-1,-1,1,0};

	/**************************************** control_cb() *******************/

	/* GLUI control callback                                                 */

	static void control_cb(int control)
	{
		if(control==LIGHT0_ENABLED_ID)
		{
			if(light0_enabled)
			{
				glEnable(GL_LIGHT0);
				light0_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT0);
				light0_spinner->disable();
			}
		}
		else if(control==LIGHT1_ENABLED_ID)
		{
			if(light1_enabled)
			{
				glEnable(GL_LIGHT1);
				light1_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT1);
				light1_spinner->disable();
			}
		}
		else if(control==LIGHT0_INTENSITY_ID)
		{
			float v[] = 
			{
				light0_diffuse[0],light0_diffuse[1],
				light0_diffuse[2],light0_diffuse[3]
			};

			v[0] *= light0_intensity;
			v[1] *= light0_intensity;
			v[2] *= light0_intensity;

			glLightfv(GL_LIGHT0,GL_DIFFUSE,v);
		}
		else if(control==LIGHT1_INTENSITY_ID)
		{
			float v[] = 
			{
				light1_diffuse[0],light1_diffuse[1],			
				light1_diffuse[2],light1_diffuse[3]
			};

			v[0] *= light1_intensity;
			v[1] *= light1_intensity;
			v[2] *= light1_intensity;

			glLightfv(GL_LIGHT1,GL_DIFFUSE,v);
		}
	}

	/***************************************** myGlutMouse() **********/

	static void myGlutMouse(int button, int button_state, int x, int y)
	{
		if(button==GLUT_LEFT_BUTTON&&button_state==GLUT_DOWN)
		{
			last_x = x; last_y = y;
		}
	}

	/***************************************** myGlutMotion() **********/

	static void myGlutMotion(int x, int y)
	{
		rotationX+=float(y-last_y);
		rotationY+=float(x-last_x);

		last_x = x; last_y = y;

		glutPostRedisplay();
	}

	/**************************************** myGlutReshape() *************/

	static void myGlutReshape(int x, int y)
	{
		xy_aspect = (float)x/(float)y;

		glViewport(0,0,x,y);

		glutPostRedisplay();
	}
 
	/***************************************** myGlutDisplay() *****************/

	static void myGlutDisplay()
	{
		glClearColor(0.9f,0.9f,0.9f,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-xy_aspect*0.04,xy_aspect*0.04,-0.04,0.04,0.1,15);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-2.6f);

		glScalef(scale,scale,scale);

		/*** Now we render object, using the variables 'obj_type', 'segments', and
		  'wireframe'.  These are _live_ variables, which are transparently 
		  updated by GLUI ***/

		glPushMatrix();
		glTranslatef(-0.5,0,0);
		glRotatef(rotationY,0,1,0);
		glRotatef(rotationX,1,0,0);
		if(wireframe&&show_sphere)
		{
			glutWireSphere(0.4,segments,segments);
		}
		else if(show_sphere)
		{
			glutSolidSphere(0.4,segments,segments);
		}
		draw_axes(0.52f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5,0,0);
		glRotatef(rotationY,0,1,0);
		glRotatef(rotationX,1,0,0);
		if(wireframe&&show_torus)
		{
			glutWireTorus(0.15,0.3,16,segments);
		}
		else if(show_torus)
		{
			glutSolidTorus(0.15,0.3,16,segments);
		}
		draw_axes(0.52f);
		glPopMatrix();

		glutSwapBuffers();
	}

	/**************************************** main() ********************/

	static void main(bool quit) //#4
	{
		/****************************************/
		/*   Initialize GLUT and create window  */
		/****************************************/

		//glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
		//glutInitWindowPosition(50,50);
		glutInitWindowSize(300,300);

		main_window.id = glutCreateWindow("GLUI Example 4 (F4)");
		GLUI_Master.set_glutDisplayFunc(myGlutDisplay);
		GLUI_Master.set_glutReshapeFunc(myGlutReshape);		
		GLUI_Master.set_glutMotionFunc(myGlutMotion);
		GLUI_Master.set_glutMouseFunc(myGlutMouse);
		GLUI_Master.set_glutSpecialFunc(glui_example_special);

		/****************************************/
		/*       Set up OpenGL lights           */
		/****************************************/

		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
		glLightfv(GL_LIGHT1,GL_POSITION,light1_position);

		/****************************************/
		/*          Enable z-buffering          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);

		/****************************************/
		/*         Here's the GLUI code         */
		/****************************************/

		//printf("GLUI version: %3.2f\n",GLUI_Master.get_version());

		glui = GLUI_Master.create_glui("GLUI (F4)",0,//400,50
		glutGet(GLUT_WINDOW_X)+400,glutGet(GLUT_WINDOW_Y)+50);
		glui->set_main_gfx_window(main_window);

		//2019: Removing this (padding?) panel.
		/*** Add invisible panel to hold rest of controls ***/
		//GLUI_Panel *panel1 = new GLUI_Panel(glui,"",GLUI_PANEL_NONE);		
		GLUI_Panel *panel1 = glui->get_main_panel();

		obj_panel = new GLUI_Panel(panel1,"Objects");

		/***** Control for object params *****/

		checkbox =
		new GLUI_Checkbox(obj_panel,"Wireframe",&wireframe,1,control_cb);
		spinner = new GLUI_Spinner(obj_panel,"Segments",&segments);
		spinner->set_int_limits(3,60);
		spinner->set_alignment(GLUI_ALIGN_RIGHT);

		GLUI_Spinner *scale_spinner =
		new GLUI_Spinner(obj_panel,"Scale",&scale);
		scale_spinner->set_float_limits(0.2f,4);
		scale_spinner->set_alignment(GLUI_ALIGN_RIGHT);

		GLUI_Panel *panel2 = new GLUI_Panel(obj_panel,"",GLUI_PANEL_NONE);
		new GLUI_Checkbox(panel2,"Sphere",&show_sphere);
		new GLUI_Column(panel2);
		new GLUI_Checkbox(panel2,"Torus",&show_torus);
		
		/****** A 'quit' button *****/

		if(quit)
		{
			//new GLUI_Button(glui,"Quit",0,(GLUI_Update_CB)exit);
			new GLUI_Button(glui,"Quit",0,quit_cb);
		}

		/*** Start a new column in this panel ***/
		new GLUI_Column(panel1,false); /* 'false' means don't draw bar */

		/******** Add some controls for lights ********/

		GLUI_Panel *light0 = new GLUI_Panel(panel1,"Light 1");
		GLUI_Panel *light1 = new GLUI_Panel(panel1,"Light 2");

		new GLUI_Checkbox(light0,"Enabled",&light0_enabled,
		LIGHT0_ENABLED_ID,control_cb);
		light0_spinner =
		new GLUI_Spinner(light0,"Intensity",
		&light0_intensity,LIGHT0_INTENSITY_ID,
		control_cb);
		light0_spinner->set_float_limits(0,1);

		new GLUI_Checkbox(light1,"Enabled",&light1_enabled,
		LIGHT1_ENABLED_ID,control_cb);
		light1_spinner =
		new GLUI_Spinner(light1,"Intensity",
		&light1_intensity,LIGHT1_INTENSITY_ID,
		control_cb);
		light1_spinner->set_float_limits(0,1);
		light1_spinner->disable(); /* Disable this light initially */

		//glutMainLoop(); return EXIT_SUCCESS;
	}
}
namespace glui_example_5 //EXAMPLE #5
{
	/****************************************************************************

	  example5.cpp

	  A GLUI program demonstrating subwindows, rotation controls,
		translation controls, and listboxes

	  -----------------------------------------------------------------------
	   
	  7/10/98 Paul Rademacher (rademach@cs.unc.edu)

	 ****************************************************************************/

	//static float xy_aspect;
	//static int last_x,last_y;
	static float rotationX = 0,rotationY = 0;

	/** These are the live variables passed into GLUI ***/
	static int wireframe = 0;
	static int obj_type = 1;
	static int segments = 8;
	static int segments2 = 8;
	static int light0_enabled = 1;
	static int light1_enabled = 1;
	static float light0_intensity = 1;
	//2019: I think light1_diffuse is not initialized to 0.4?
	//static float light1_intensity = 0.4;
	static float light1_intensity = 1;
	//static int main_window;
	static float scale = 1;
	static int show_sphere = 1;
	static int show_torus = 1;
	static int show_axes = 1;
	static int show_text = 1;
	static float sphere_rotate[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	static float torus_rotate[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	static float view_rotate[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	static float obj_pos[] = {0,0,0};
	static const char *string_list[] = {"Hello World!","Foo","Testing...","Bounding box: on"};
	static int curr_string = 0;

	/** Pointers to the windows and some of the controls we'll create **/
	static GLUI *glui,*glui2;
	static GLUI_Spinner *light0_spinner,*light1_spinner;
	static GLUI_RadioGroup *radio;
	static GLUI_Panel *obj_panel;

	/********** User IDs for callbacks ********/
	/*
	#define LIGHT0_ENABLED_ID    200
	#define LIGHT1_ENABLED_ID    201
	#define LIGHT0_INTENSITY_ID  250
	#define LIGHT1_INTENSITY_ID  260
	#define ENABLE_ID            300
	#define DISABLE_ID           301
	#define SHOW_ID              302
	#define HIDE_ID              303
	*/

	/********** Miscellaneous global variables **********/

	//static GLfloat light0_ambient[] = {0.1f,0.1f,0.3f,1};
	static GLfloat light0_diffuse[] = {0.6f,0.6f,1,1};
	//static GLfloat light0_position[] = {0.5f,0.5f,1,0};

	//static GLfloat light1_ambient[] = {0.1f,0.1f,0.3f,1};
	static GLfloat light1_diffuse[] = {0.9f,0.6f,0,1};
	//static GLfloat light1_position[] = {-1,-1,1,0};

	static GLfloat lights_rotation[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

	/**************************************** myGlutReshape() *************/

	static void myGlutReshape(int=0, int=0)
	{
		int tx,ty,tw,th;
		GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
		glViewport(tx,ty,tw,th);

		//freeglut fix, but technically this is necessary.
		glScissor(tx,ty,tw,th); glEnable(GL_SCISSOR_TEST);

		xy_aspect = (float)tw/th;

		GLUI_Master.reshape(); //2019 (testing)

		glutPostRedisplay();
	}
	
	/***************************************** myGlutDisplay() *****************/

	static void myGlutDisplay()
	{	
		glClearColor(0.9f,0.9f,0.9f,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-xy_aspect*0.04,xy_aspect*0.04,-0.04,0.04,0.1,15);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glMultMatrixf(lights_rotation);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		glLoadIdentity();
		glTranslatef(0,0,-2.6f);
		glTranslatef(obj_pos[0],obj_pos[1],-obj_pos[2]);
		glMultMatrixf(view_rotate);

		glScalef(scale,scale,scale);

		/*** Now we render object, using the variables 'obj_type', 'segments', and
		  'wireframe'.  These are _live_ variables, which are transparently 
		  updated by GLUI ***/

		glPushMatrix();
		glTranslatef(-0.5f,0,0);
		glMultMatrixf(sphere_rotate);
		if(wireframe&&show_sphere)
		{
			glutWireSphere(0.4,segments,segments);
		}
		else if(show_sphere)
		{
			glutSolidSphere(0.4,segments,segments);
		}
		
		if(show_axes) draw_axes(0.52f);

		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5f,0,0);
		glMultMatrixf(torus_rotate);
		if(wireframe&&show_torus)
		{
			glutWireTorus(0.15,0.3,16,segments);
		}
		else if(show_torus)
		{
			glutSolidTorus(0.15,0.3,16,segments);
		}
		if(show_axes) draw_axes(0.52f);
		glPopMatrix();

		if(show_text)
		{
			glDisable(GL_LIGHTING);/* Disable lighting while we render text */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0,100,0,100,-1,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glColor3ub(0,0,0);			

			/*  printf( "text: %s\n", text );              */

			//glRasterPos2i(10,10);
			main_window.draw_string(10,-25,string_list[curr_string]);
		}

		glEnable(GL_LIGHTING);

		glutSwapBuffers();
	}
	
	/**************************************** control_cb() *******************/

	/* GLUI control callback                                                 */

	static void control_cb(GLUI_Control *c)
	{
		int control = c->user_id;

		if(control==LIGHT0_ENABLED_ID)
		{
			if(light0_enabled)
			{
				glEnable(GL_LIGHT0);
				light0_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT0);
				light0_spinner->disable();
			}
		}
		else if(control==LIGHT1_ENABLED_ID)
		{
			if(light1_enabled)
			{
				glEnable(GL_LIGHT1);
				light1_spinner->enable();
			}
			else
			{
				glDisable(GL_LIGHT1);
				light1_spinner->disable();
			}
		}
		else if(control==LIGHT0_INTENSITY_ID)
		{
			float v[] = 
			{
				light0_diffuse[0],light0_diffuse[1],
				light0_diffuse[2],light0_diffuse[3]
			};

			v[0] *= light0_intensity;
			v[1] *= light0_intensity;
			v[2] *= light0_intensity;

			glLightfv(GL_LIGHT0,GL_DIFFUSE,v);
		}
		else if(control==LIGHT1_INTENSITY_ID)
		{
			float v[] = 
			{
				light1_diffuse[0],light1_diffuse[1],
				light1_diffuse[2],light1_diffuse[3]
			};

			v[0] *= light1_intensity;
			v[1] *= light1_intensity;
			v[2] *= light1_intensity;

			glLightfv(GL_LIGHT1,GL_DIFFUSE,v);
		}
		else if(control==DISABLE_ID)
		{
			glui2->disable(); 
					
			tog1: c->disable(c->next());
		}
		else if(control==ENABLE_ID)
		{
			glui2->enable();

			tog2: c->disable(c->prev());
		}
		else if(control==HIDE_ID)
		{
			glui2->hide();

			myGlutReshape(); goto tog1; //2019
		}
		else if(control==SHOW_ID)
		{
			glui2->show();

			myGlutReshape(); goto tog2; //2019
		}		
	}

	/**************************************** main() ********************/

	static void main(bool quit) //#5
	{
		main_window.set_font(GLUT_BITMAP_HELVETICA_18);

		/****************************************/
		/*   Initialize GLUT and create window  */
		/****************************************/

		//glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
		//glutInitWindowPosition(50,50);
		glutInitWindowSize(800,600);

		main_window.id = glutCreateWindow("GLUI Example 5 (F5)");
		GLUI_Master.set_glutDisplayFunc(myGlutDisplay);
		GLUI_Master.set_glutReshapeFunc(myGlutReshape);
		GLUI_Master.set_glutSpecialFunc(glui_example_special);

		/****************************************/
		/*       Set up OpenGL lights           */
		/****************************************/

		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
		glLightfv(GL_LIGHT1,GL_POSITION,light1_position);

		/****************************************/
		/*          Enable z-buffering          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);

		/****************************************/
		/*         Here's the GLUI code         */
		/****************************************/

		//printf("GLUI version: %3.2f\n",GLUI_Master.get_version());

		/*** Create the side subwindow ***/
		glui = GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);
		glui->set_main_gfx_window(main_window);

		obj_panel = new GLUI_Rollout(glui,"Properties",false);

		/***** Control for object params *****/

		new GLUI_Checkbox(obj_panel,"Wireframe",&wireframe,1,control_cb);
		GLUI_Spinner *spinner =
		new GLUI_Spinner(obj_panel,"Segments",&segments);
		spinner->set_int_limits(3,60);
		spinner->set_alignment(GLUI_ALIGN_RIGHT);

		GLUI_Spinner *scale_spinner =
		new GLUI_Spinner(obj_panel,"Scale",&scale);
		scale_spinner->set_float_limits(0.2f,4);
		scale_spinner->set_alignment(GLUI_ALIGN_RIGHT);

		/******** Add some controls for lights ********/

		GLUI_Rollout *roll_lights = new GLUI_Rollout(glui,"Lights",false);

		GLUI_Panel *light0 = new GLUI_Panel(roll_lights,"Light 1");
		GLUI_Panel *light1 = new GLUI_Panel(roll_lights,"Light 2");

		new GLUI_Checkbox(light0,"Enabled",&light0_enabled,LIGHT0_ENABLED_ID,control_cb);
		light0_spinner = new GLUI_Spinner(light0,"Intensity",&light0_intensity,LIGHT0_INTENSITY_ID,control_cb);
		spinner->set_alignment(GLUI_ALIGN_RIGHT);
		light0_spinner->set_float_limits(0,1);
		GLUI_Scrollbar *sb;
		sb = new GLUI_Scrollbar(light0,"Red",GLUI_SCROLL_HORIZONTAL,&light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);
		sb = new GLUI_Scrollbar(light0,"Green",GLUI_SCROLL_HORIZONTAL,&light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);
		sb = new GLUI_Scrollbar(light0,"Blue",GLUI_SCROLL_HORIZONTAL,&light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);
		new GLUI_Checkbox(light1,"Enabled",&light1_enabled,LIGHT1_ENABLED_ID,control_cb);
		light1_spinner =
		new GLUI_Spinner(light1,"Intensity",&light1_intensity,LIGHT1_INTENSITY_ID,control_cb);
		light1_spinner->set_float_limits(0,1);
		sb = new GLUI_Scrollbar(light1,"Red",GLUI_SCROLL_HORIZONTAL,&light1_diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);
		sb = new GLUI_Scrollbar(light1,"Green",GLUI_SCROLL_HORIZONTAL,&light1_diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);
		sb = new GLUI_Scrollbar(light1,"Blue",GLUI_SCROLL_HORIZONTAL,&light1_diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
		sb->set_float_limits(0,1);


		/*** Add another rollout ***/
		GLUI_Rollout *options = new GLUI_Rollout(glui,"Options",true);
		new GLUI_Checkbox(options,"Draw sphere",&show_sphere);
		new GLUI_Checkbox(options,"Draw torus",&show_torus);
		new GLUI_Checkbox(options,"Draw axes",&show_axes);
		new GLUI_Checkbox(options,"Draw text",&show_text);

		/**** Add listbox ****/
		new GLUI_Separator(glui,false);

		GLUI_Listbox *list = new GLUI_Listbox(glui,"Text",&curr_string);		
		for(int i=0;i<4;i++)
		list->add_item(i,string_list[i]);

		new GLUI_Separator(glui,false);

		GLUI_Button *tog = 
		new GLUI_Button(glui,"Hide",HIDE_ID,control_cb);
		tog = 
		new GLUI_Button(glui,"Show",SHOW_ID,control_cb);
		tog->disable();

		new GLUI_Separator(glui,false);

		/*** Disable/Enable buttons ***/		
		new GLUI_Button(glui,"Disable movement",DISABLE_ID,control_cb);
		tog = 
		new GLUI_Button(glui,"Enable movement",ENABLE_ID,control_cb);
		tog->disable();

		new GLUI_Separator(glui,false);

		if(quit)
		{
			/****** A 'quit' button *****/
			//new GLUI_Button(glui,"Quit",0,(GLUI_Update_CB)exit);
			new GLUI_Button(glui,"Quit",0,quit_cb);
		}

		glui->activate(); //NEW (main_window is noninteractive.)
		
		/*** Create the bottom subwindow ***/
		glui2 = GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_BOTTOM);
		glui2->set_main_gfx_window(main_window);

		GLUI_Rotation *view_rot = new GLUI_Rotation(glui2,"Objects",view_rotate);
		view_rot->set_spin(1); //INFINITE SPIN
		new GLUI_Column(glui2,false);
		GLUI_Rotation *sph_rot = new GLUI_Rotation(glui2,"Sphere",sphere_rotate);
		sph_rot->set_spin(0.98);
		new GLUI_Column(glui2,false);
		GLUI_Rotation *tor_rot = new GLUI_Rotation(glui2,"Torus",torus_rotate);
		tor_rot->set_spin(0.98);
		new GLUI_Column(glui2,false);
		GLUI_Rotation *lights_rot = new GLUI_Rotation(glui2,"Light 1",lights_rotation);
		lights_rot->set_spin(0.82);
		new GLUI_Column(glui2,false);
		GLUI_Translation *trans_xy =
		new GLUI_Translation(glui2,"Objects XY",GLUI_TRANSLATION_XY,obj_pos);
		trans_xy->set_speed(0.005);
		new GLUI_Column(glui2,false);
		GLUI_Translation *trans_x =
		new GLUI_Translation(glui2,"X",GLUI_TRANSLATION_X,obj_pos);
		trans_x->set_speed(0.005);
		new GLUI_Column(glui2,false);
		GLUI_Translation *trans_y =
		new GLUI_Translation(glui2,"Y",GLUI_TRANSLATION_Y,&obj_pos[1]);
		trans_y->set_speed(0.005);
		new GLUI_Column(glui2,false);
		GLUI_Translation *trans_z =
		new GLUI_Translation(glui2,"Z",GLUI_TRANSLATION_Z,&obj_pos[2]);
		trans_z->set_speed(0.005);

		/**** Regular GLUT main loop ****/

		//glutMainLoop(); return EXIT_SUCCESS;
	}
}
namespace glui_example_6 //EXAMPLE #6
{
	/****************************************************************************

	  A simple GLUT program using the GLUI User Interface Library

	  This program sets up a checkbox and a spinner, both with live variables.
	  No callbacks are used.

	  -----------------------------------------------------------------------
	  9/17/04 John Kew - Natural Selection, Inc. (jkew@natural-selection.com)   
	  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

	****************************************************************************/

	/** These are the live variables passed into GLUI ***/
	static GLUI *edit; //int main_window;
	static int num_display  = 0;
	static int num_format  = 0;
	static int enable_textbox_checked = 1;
	static GLUI_StaticText  *text;
	static GLUI_List        *hah;
	static GLUI_TextBox     *moo;
	static GLUI             *tree;
	static GLUI_TreePanel   *tp;
	static GLUI_FileBrowser *fb;
	static GLUI_EditText    *bedit;

	static const char general[] =
	  "GLUI 2.3 Widgets\n"
	  "\n"
	  "A number of widgets have been introduced in GLUI 2.3\n"
	  "\n"
	  "  * GLUI_Scrollbar   - A scrollbar slider widget\n"
	  "  * GLUI_TextBox     - A multi-line text widget\n"
	  "  * GLUI_List        - A static choice list\n"
	  "  * GLUI_FileBrowser - A simple filebrowser based on GLUI_List\n"
	  "  * GLUI_Tree        - Hierarchical tree widget\n"
	  "  * GLUI_TreePanel   - Manager for the tree widget\n";

	static const char scroll[] = 
	  "ScrollBar Widget\n"
	  "\n"
	  "The GLUI_Scrollbar can be used more or less like a GLUI_Spinner. "
	  "It can be horizontal or vertical, and have float or int values.\n"
	  "\n"
	  "The set_int_limits and set_float_limits functions work properly even "
	  "if the min and max values are reversed.  In this way you can choose "
	  "whether the left/right or top/bottom is the minimum value.  \n"
	  "\n"
	  "For instance, with set_int_limits(0, 100) you get a normal slider with "
	  "0 corresponding to the left (bottom) end, and 100 corresponding to the "
	  "right (top) end.\n"
	  "\n"
	  "By calling set_int_limits(100,0) instead, the right (top) end becomes 0, "
	  "and the left (bottom) end is 100.\n";

	static const char tree_txt[] = "Tree Widget";

	static const char commandline[] = 
	  "CommandLine Widget\n"
	  "\n"
	  "The GLUI_CommandLine is a GLUI_EditText with some special functionality "
	  "useful for implementing a simple command-line style input widget. "
	  "The main feature is that it maintains a command history buffer so that "
	  "users can recall previous commands.\n";

	static const char gstring[] = 
	  "GLUI_String\n"
	  "\n"
	  "The GLUI string class used to be wrapper class around "
	  "a fixed 300 char buffer. "
	  "Now GLUI_String is just a typedef for std::string.\n"
	  "\n"
	  "One new function has been introduced, which is a sprintf() equivalent "
	  "for std::string/GLUI_String.\n"
	  "\n"
	  "  glui_format_str(GLUI_String& str, const char *format_str, ...);\n"
	  "\n"
	  "Other than the fact that it takes a GLUI_String as the first argument, "
	  "it works just like sprintf().\n";

	static const char list[] =
	  "List Widget\n"
	  "\n"
	  "As seen on the left side of this window.";

	static const char filebrowser[] = 
	  "FileBrowserWidget\n"
	  "\n"
	  "As seen in the lower left corner of this window.\n"
	  "\n"
	  "The GLUI_FileBrowser is a subclass of GLUI_List and provides, "
	  "a simple file browsing and selecting interface under Windows and Unix.\n"
	  "\n"
	  "Go ahead and give it a try!  Navigate and select files by double-clicking. "
	  "If you pick a text file it will be displayed in this GLUI_TextBox.\n";

	static const char text_box[] = 
	  "TextBox w/ optional ScrollBar\n"
	  "\n"
	  "A TextBox is a multiline editing box that supports basic navigation, "
	  "formatting, and editing capabilities. These are text block selection, "
	  "tabs, new lines, arrow key navigation, as well as an optional scrollbar. "
	  "The scrollbar is actually a distinct widget and can exist by itself, or "
	  "be associated with objects that have a specified callback routine.\n"
	  "\n"
	  "new GLUI_TextBox "
	  "adds a new textbox to a GLUI window, optionally nested within another "
	  "rollout or panel.\n"
	  "\n"
	  "Usage:\n"
	  "new GLUI_TextBox(GLUI_Node *glui_or_panel, char *live_var=NULL,\n"
	  "	int scroll = false,\n"
	  "	int id=-1,\n"
	  "	GLUI_Update_CB callback=NULL );\n"
	  "\n"
	  "	panel    - Name of panel to place the TextBox in.\n"
	  "	live_var - Currently unimplemented, this might be used\n"
	  "	           in future iterations.\n"
	  "	scroll   - set to \"true\" if you want a scrollbar on the left side\n"
	  "	id       - If callback is defined, it will be passed this integer\n"
	  "	           when the value is modified.\n"
	  "	callback - Pointer to callback function of type (*)(int)\n"
	  "\n"
	  "Notes:\n"
	  "The scrollbar widget uses a different callback scheme to communicate with "
	  "the textbox. Any object wishing to utilize a scrollbar in some way should "
	  "implement a static function in the object with the signature:\n"
	  "\n"
	  "(*)(GLUI_Control *, int)\n"
	  "\n"
	  "Where the first parameter is the object the scrollbar is associated with "
	  "and the second is the int_val passed into the object by the scrollbar. "
	  "Refer to glui_textbox.cpp and glui_scrollbar.cpp for implementation details.\n"
	  "\n"/*
	  "Wishlist:\n"
	  "* Word Wrap\n"
	  "* Optimized Drawing (Update only lines that change, etc.)\n"
	  "* Multicolour text\n"
	  "* Current word selection\n"
	  "* copy/paste\n"*/;

	/***************************************** myGlutDisplay() *****************/

	static void control_cb(int control)
	{
		if(control==7)
		{
			GLUI_String text;
			std::string file_name;
			FILE *file;
			int i = 0;
			file_name = "";
			file_name = fb->get_file();
			file = fopen(file_name.c_str(),"r");
			if(file)
			{
				char c; do
				{
					c = getc(file);
					text+=c;
					i++;
				}while(c!=EOF);
				fclose(file);
			}
			else printf("Error opening file\n");

			moo->set_text(text);
		}

		if(control==1)
		{
			int item = hah->get_current_item();
		
			if(tree) tree->hide(); //???

			if(item==0)
			{
				moo->set_text(general);
			}
			if(item==1)
			{
				moo->set_text(text_box);
			}
			if(item==2)
			{
				moo->set_text(scroll);
			}
			if(item==3)
			{
				moo->set_text(gstring);
			}
			if(item==4)
			{
				moo->set_text(commandline);
			}		
			if(item==5)
			{
				moo->set_text(list);
			}
			if(item==6)
			{
				moo->set_text(filebrowser);
			}
			if(item==7)
			{
				tree->show();
				moo->set_text(tree_txt);
			}
		}
		if(control==2)
		{
			tp->add_tree("foo");
		}
		if(control==3)
		{
			tp->delete_tree(tp->curr_root);
		}
		if(control==4)
		{
			tp->go_up_tree();
		}
		if(control==5)
		{
			tp->go_to_root();
		}
		if(control==6)
		{
			tp->go_to_leaf();
		}
		if(control==8)
		{
			tp->go_to_next();
		}
		if(control==9)
		{
			tp->expand_all();
		}
		if(control==10)
		{
			tp->collapse_all();
		}
		if(control==11)
		{
			int format = 
			GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY;
			if(num_display)
			format|=GLUI_TREEPANEL_DISPLAY_HIERARCHY;
			if(num_format)
			format|=GLUI_TREEPANEL_HIERARCHY_NUMERICDOT;
			else
			format|=GLUI_TREEPANEL_HIERARCHY_LEVEL_ONLY;
			tp->set_format(format);
			tp->update_all();
		}
		if(control==12)
		{
			moo->enable(1==enable_textbox_checked);
		}
	}

	static void textbox_cb(GLUI_Control *control)
	{
		printf("Got textbox callback\n");
	}

	/**************************************** main() ********************/
			  
	static void main(bool quit) //#6
	{
		edit = GLUI_Master.create_glui("Help on GLUI Widgets (F6)");

		GLUI_Panel *ep = new GLUI_Panel(edit,"",true);
		//new GLUI_StaticText(ep,"Widget Information");	
		hah = new GLUI_List(ep,"Widget Information",true,1,control_cb);
		hah->add_item(0,"GLUI 2.3");
		hah->add_item(1,"TextBox");
		hah->add_item(2,"ScrollBar");
		hah->add_item(3,"GLUI_String");
		hah->add_item(4,"CommandLine");
		hah->add_item(5,"List");
		hah->add_item(6,"FileBrowser");
		//hah->add_item(7,"Tree");
		hah->set_h(180);
		//new GLUI_StaticText(ep,"Open Text File");
		fb = new GLUI_FileBrowser(ep,"Open Text File",false,7,control_cb);
		fb->set_h(180);
		#ifdef GLUI_NODEPRECATED
		fb->fbreaddir();
		#endif
				
		if(quit) /****** A 'quit' button *****/
		{
			//new GLUI_Button(glui,"Quit",0,(GLUI_Update_CB)exit);
			new GLUI_Button(ep,"Quit",0,quit_cb);
		}

		new GLUI_Column(ep,false); 

		moo = new GLUI_TextBox(ep,true,1,textbox_cb);
		moo->set_text(general);
		moo->set_h(400);
		moo->set_w(410);
		if(0==enable_textbox_checked) 
		moo->disable();
		new GLUI_Checkbox(ep, "Enable text box",&enable_textbox_checked,12,control_cb);

		//These controls are in an unfinished state. The demo
		//locks up the app; is 90% unfinished and nonfunctional.
		//After review, it seems unlikely software uses these two. 
		/*
		tree = GLUI_Master.create_glui("Tree Test", 0);		
		ep = new GLUI_Panel(tree, "Tree Controls");
		bedit = new GLUI_EditText(ep, "New Branch Name");
		new GLUI_Checkbox(ep, "Display Numbers", &num_display);
		new GLUI_StaticText(ep, "Number format");
		GLUI_RadioGroup *rg = new GLUI_RadioGroup(ep, &num_format);
		new GLUI_RadioButton(rg, "Level Only");
		new GLUI_RadioButton(rg, "Hierarchal");
		new GLUI_Button(ep, "Update Format", 11, control_cb); 
		new GLUI_Column(ep);
		new GLUI_Button(ep, "Add Branch", 2, control_cb); 
		new GLUI_Button(ep, "Del Branch", 3, control_cb);
		new GLUI_Button(ep, "Up Branch", 4, control_cb); 
		new GLUI_Button(ep, "Goto Root", 5, control_cb);
		new GLUI_Column(ep);
		new GLUI_Button(ep, "Descend to Leaf", 6, control_cb); 
		new GLUI_Button(ep, "Next Branch", 8, control_cb); 
		new GLUI_Button(ep, "Expand All", 9, control_cb); 
		new GLUI_Button(ep, "Collapse All", 10, control_cb); 
		tp = new GLUI_TreePanel(tree,"Tree Test");
		tp->set_format(GLUI_TREEPANEL_ALTERNATE_COLOR | 
						GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY |
						GLUI_TREEPANEL_DISPLAY_HIERARCHY | 
						GLUI_TREEPANEL_HIERARCHY_NUMERICDOT);
		tp->set_level_color(1,1,1);
		tp->ab("foo you");
		tree->hide();
		*/

		//glutMainLoop(); return 0;
	}
}

/**DEMO
 If argc is nonzero glutInit and glutMainLoop is done.
*/
int GLUI_Library::glui_example(int n, int argc, char*argv[])
{
	static bool quit = argc==0; if(argc)
	{
		printf("GLUI version: %3.2f\n",GLUI::get_version());
		glutInit(&argc,argv); glutInitWindowPosition(50,50);
	}
		
	typedef UI::Void_CB v;
	typedef bool(*s)(UI*,int,int);
	v swap = (v)(s)glui_example_special;
	std::swap(swap,(v&)GLUI.special_callback); //push

	int ret = 0; switch(n) 
	{
	//NOTE: Changes the window/keyboard focus.
	case 1: glui_example_1::main(quit); break;
	case 2: glui_example_2::main(quit); break;
	case 3: glui_example_3::main(quit); break;
	case 4: glui_example_4::main(quit); break;
	case 5: glui_example_5::main(quit); break;
	case 6: glui_example_6::main(quit); break;
	default: ret = 1;
	}

	std::swap(swap,(v&)GLUI.special_callback); //pop

	if(argc) glutMainLoop(); return ret;
}

#endif //GLUI_BUILD_EXAMPLES

