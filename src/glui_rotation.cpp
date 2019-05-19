/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_rotation - GLUI_Rotation control class


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

#include "../include/glui_internal.h" //PCH
#include "../include/glui_arcball.h" //"arcball.h"

namespace GLUI_Library
{//-.
//<-'

enum{ glui_rotation_stacks = 32 };

static Texture glui_rotation_texture;

/************** GLUI_Rotation::GLUI_Rotation() ********************/

void UI::Rotation::_Rotation()
{
	delete ball; //gluDeleteQuadric(quadObj);
}

/********************** GLUI_Rotation::_copy_float_array_to_ball() *********/

void UI::Rotation::_update_live()
{
	/*
	inline void UI::Rotation::_copy_float_array_to_ball()
	{ 	
		double *fp_src = &float_array_val[0];
		double *fp_dst = &(*ball->rot_ptr)[0][0];
		for(int i=0;i<16;i++) *fp_dst++ = *fp_src++;
	}*/

	ball->q_now.set(*(mat4*)float_array_val); redraw();
}

/********************** GLUI_Rotation::_copy_ball_to_float_array() *********/

inline void UI::Rotation::_copy_ball_to_float_array()
{
	//Mouse_Interface is responsible for all but setting the value.
	//set_float_array_val(&(*ball->rot_ptr)[0][0]); //ball->q_now.set
	GLUI::set_edited(this),stage_live(&(*ball->rot_ptr)[0][0],16); 
}

/*************************** GLUI_Rotation::iaction_mouse() ***********/

void UI::Rotation::_iaction_mouse(int stage, int x, int y)
{
	if(1==stage) 
	{
		ball->mouse_down(x,y);

		/* printf("%d %d - %f %f\n",x,y,ball->center[0],ball->center[1]); */		
	}
	else if(2==stage)
	{
		//_copy_float_array_to_ball(); //2019

		/* printf("%d %d\n",x,y); */

		int cm = GLUI.curr_modifiers;

		double shift = 0; if(~cm&GLUT_ACTIVE_SHIFT) 
		{
			//4/M_PI*10 just increases 10x from the original formulation.
			//It was quadrupling the radius. Pi instead of 4 is best fit.
			shift = 4/M_PI*10; 
		}

		ball->mouse_motion(x,y,shift,cm&GLUT_ACTIVE_ALT,cm&GLUT_ACTIVE_CTRL);

		_copy_ball_to_float_array();

		//if(can_spin) spinning = true;
	}
	else if(3==stage)
	{
		ball->mouse_up(); 
	}
}
	
/******************** GLUI_Rotation::iaction_draw(1) **************/

void UI::Rotation::_iaction_draw(int persp)
{
	if(1==persp)
	{			
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		//glMultMatrixd(&(*ball->rot_ptr)[0][0]); //2019
		glMultMatrixd(float_array_val);

		//draw_ball(1.35f); //1.96f
		//Want 2px separation so odd pixels don't touch.
		draw_ball(1.25f,true,true);

		glPopMatrix();		
	}
	else if(0==persp) //ortho
	{
		//NOTE: This is basically one big RadioButton frame.

		//float radius = (h-22)/2.0f; /*std::min(w/2.0f,h/2.0f); */
		float radius = (h-UI_MOUSE_IACTION_HEADER-4)/2.0f;

		/********* Draw emboss circles around arcball control *********/
						 
		radius+=1; //pass #1		
		//NOTE: 193 is background color.
		int c1 = enabled?0:180, c2 = 193; //192;		
		for(int pass=1;pass<=3;pass++)
		{
			glBegin(GL_LINE_LOOP);
			enum{ kN=2*glui_rotation_stacks }; //60
			for(int k=0;k<kN;k++)
			{
				float phi = 2*M_PI*k/kN;
				float px = cos(phi)*radius;			
				float py = sin(phi)*radius;

				int l = py<-px?c1:c2;
				glColor3ub(l,l,l);
				glVertex2f(px,py);
			}
			glEnd();

			if(pass==2)
			{
				c1 = 128; c2 = 255; //outer ring
				
				radius+=0.75;
			}
			else radius+=0.25; //fill odd pixels
		}
	}
}

/********************************** GLUI_Rotation::init_ball() **********/

void UI::Rotation::_update_size()
{
	Mouse_Interface::_update_size();

	//float radius = 2*(h-18); //4 times radius???
	float radius = (h-18)/2.0f; 
	//ball->set_params(vec2(w/2.0f,(h-UI_MOUSE_IACTION_HEADER)/2.0f),radius);
	ball->radius = radius;
}

/****************************** GLUI_Rotation::draw_ball() **************/

static void glui_rotation_gluSphere(float);
bool UI::Rotation::draw_ball(float radius, bool t, bool l)
{
	if(t) //setup_texture
	{
		enum{ t2d=GL_TEXTURE_2D };
		glEnable(t2d);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glColor3f(1,1,1);
		//2019: Well this is embarrassing!
		//static GLuint tex = 0u;
		GLuint tex;
		if(glui_rotation_texture.get(ui,tex))
		{
			/* (OSL 2006/06) Just use glBindTexture to avoid having to re-upload the whole checkerboard every frame. */
			glBindTexture(t2d,tex);
			//return true
			goto l;
		} 
		else /* need to make a new checkerboard texture */
		{
			glGenTextures(1,&tex);
			if(!glui_rotation_texture.set(ui,tex))
			{
				//Overflow? I don't know if GLUI shares contexts with the user
				//drawing areas.
				assert(tex<256); 

				//TODO: This could use an internal system for loading textures
				//and their error output.
				glDeleteTextures(1,&tex);
				return false;
			}
		}	
		int err = glGetError(); //flushing...

		glBindTexture(t2d,tex);
		glEnable(t2d);

		enum
		{
			dark = 110, /*** Dark and light colors for ball checkerboard  ***/
			light = 220, 

			/* Note: you can change the number of checkers across there sphere in draw_ball */

			checkboard_size = 64, /* pixels across whole texture */
			checkboard_repeat = 32, /* pixels across one black/white sector */
		};

		unsigned char c,
		texture_image[checkboard_size][checkboard_size];
		for(int i=0;i<checkboard_size;i++) 
		for(int j=0;j<checkboard_size;j++)
		{	
			c = (i/checkboard_repeat)&1^(j/checkboard_repeat)&1?light:dark;

			texture_image[i][j] = c;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glTexParameteri(t2d,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(t2d,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(t2d,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

		glTexParameteri(t2d,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //GL_NEAREST
		glTexImage2D(t2d,0,1,checkboard_size,checkboard_size,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,texture_image);	
	}

	l:	if(l) //setup_lights
	{
		/* (enabled?glEnable:glDisable)(GL_LIGHTING ); */
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat light0_position[] = {-1,1,1,0};
		glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
		if(enabled) /* enabled colors */
		{
			GLfloat light0_ambient[] = {0.2f,0.2f,0.2f,1};
			GLfloat light0_diffuse[] = {1,1,1,1};
			glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		}
		else /* disabled colors */
		{ 
			GLfloat light0_ambient[] = {0.6f,0.6f,0.6f,1};
			GLfloat light0_diffuse[] = {0.2f,0.2f,0.2f,1};
			glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
		}
	}

	if(radius) //iaction_draw_active_area_persp
	{
		glEnable(GL_CULL_FACE);

	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();

		//glMultMatrixd(&(*ball->rot_ptr)[0][0]); //2019
	//	glMultMatrixd(float_array_val);

		//draw_ball
		{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			/* black-white checker tiles across whole sphere */
			glScalef(2,2,1);
			//gluSphere(quadObj,radius,32,16);
			//gluSphere(quadObj,radius,glui_rotation_stacks,glui_rotation_stacks);		
			glui_rotation_gluSphere(radius);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		}
		
	//	glPopMatrix();

		glDisable(GL_CULL_FACE);
				
		if(t) /* unhook checkerboard texture */
		{
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
		}
		if(l)
		{
			glDisable(GL_LIGHTING);
		}
	}	

	return true;
}

/****************************** GLUI_Rotation::reset() **********/

void UI::Rotation::reset()
{
	//2019: GLUI_LIVE sets ptr_val before calling reset.
	//Arcball is an incomplete type in the local header.
	bool new_Arcball = !ball;
	if(new_Arcball) ball = new Arcball;

	ball->init(); /** reset quaternion, etc. **/
	//Does same?
	//ball->set_params(vec2(w/2.0f,(h-UI_MOUSE_IACTION_HEADER)/2.0f),2.0f*(h-18));
	_update_size();	

	set_spin(spin);

	_copy_ball_to_float_array();

	if(!new_Arcball)
	{
		output_live(); /*** Output live and draw main gfx window ***/

		execute_callback(); //2019
	}
}

/****************************** GLUI_Rotation::needs_idle() *********/

bool UI::Rotation::_needs_idle()
{
	//I think this puts the UI in constant idle?
	//return can_spin;

	//I think mouse_down is gumming the ball up.
	//return ball?ball->is_mouse_down||ball->is_spinning:false;
	return ball&&ball->is_spinning;
}

/****************************** GLUI_Rotation::idle() ***************/

void UI::Rotation::_idle()
{
	//spinning = ball->is_spinning?true:false;
	//if(can_spin&&spinning)
	{
		//_copy_float_array_to_ball(); //2019
		ball->idle();

		//2019: This way is unstable... setting by idle().
		//*ball->rot_ptr = *ball->rot_ptr*ball->rot_increment;

		_copy_ball_to_float_array();

		//NOTE: Mouse_Interface::_mouse_held_down_handler does
		//this logic for mouse events, but not for idle spinning
		{
			redraw();

			output_live(); /** output live and update gfx **/

			execute_callback(); //2019
		}
	}
}

/************************ GLUI_Rotation::set_spin() **********************/

void UI::Rotation::set_spin(double damp_factor)
{	
	ball->set_damping(/*1-*/damp_factor);

	spin = damp_factor;
}

//Assuming this will emulate gluSphere.
//https://cgit.freedesktop.org/mesa/glu/tree/src/libutil/quad.c#n150
static void glui_rotation_gluSphere
(/*GLUquadric *qobj,GLdouble*/float radius/*,GLint slices,GLint stacks*/)
{
	enum
	{
		//THIS SHOULD BE A FACTOR OF RADIUS.
		//THIS SHOULD BE A FACTOR OF RADIUS.
		//THIS SHOULD BE A FACTOR OF RADIUS.
		slices=glui_rotation_stacks, stacks=glui_rotation_stacks,

		/* Make it not a power of two to avoid cache thrashing on the chip */

		CACHE_SIZE=glui_rotation_stacks+1, //240
	};

	//NOTE: Mesa code did not declare these static. But GLUI is normally
	//not expected to change these values. Note that, althout the radius
	//can be factored out, ideally tessellation increase with the radius.
	static GLfloat prevrad = 0,
	sinCache1a[CACHE_SIZE],
	cosCache1a[CACHE_SIZE],
	sinCache2a[CACHE_SIZE],
	cosCache2a[CACHE_SIZE],
	//sinCache3a[CACHE_SIZE],
	//cosCache3a[CACHE_SIZE],
	sinCache1b[CACHE_SIZE], //These include radius.
	cosCache1b[CACHE_SIZE], //These include radius.
	sinCache2b[CACHE_SIZE],
	cosCache2b[CACHE_SIZE];
	//sinCache3b[CACHE_SIZE],
	//cosCache3b[CACHE_SIZE];

	GLint i,j;
	GLfloat angle;
	GLfloat zLow,zHigh;
	GLfloat sintemp1 = 0,sintemp2 = 0,sintemp3 = 0,sintemp4 = 0;
	GLfloat costemp1 = 0,costemp2 = 0,costemp3 = 0,costemp4 = 0;
	GLboolean needCache2,needCache3;
	GLint start,finish;

	if(prevrad!=radius)
	{
		prevrad = radius;
	}
	else goto hit;	

	/*if(slices>=CACHE_SIZE) slices = CACHE_SIZE-1;
	if(stacks>=CACHE_SIZE) stacks = CACHE_SIZE-1;
	if(slices<2||stacks<1||radius<0)
	{
		gluQuadricError(qobj,GLU_INVALID_VALUE);
		return;
	}*/
	assert(slices<CACHE_SIZE&&stacks<CACHE_SIZE);
	assert(!(slices<2||stacks<1||radius<0));

	/* Cache is the vertex locations cache */
	/* Cache2 is the various normals at the vertices themselves */
	/* Cache3 is the various normals for the faces */
	needCache2 = needCache3 = GL_FALSE;

	//if(qobj->normals==GLU_SMOOTH)
	{
		needCache2 = GL_TRUE;
	}

	/*if(qobj->normals==GLU_FLAT)
	{
		if(qobj->drawStyle!=GLU_POINT)
		{
			needCache3 = GL_TRUE;
		}
		if(qobj->drawStyle==GLU_LINE)
		{
			needCache2 = GL_TRUE;
		}
	}*/

	for(i=0;i<slices;i++)
	{
		angle = 2*M_PI*i/slices;
		sinCache1a[i] = std::sin(angle);
		cosCache1a[i] = std::cos(angle);
		if(needCache2)
		{
			sinCache2a[i] = sinCache1a[i];
			cosCache2a[i] = cosCache1a[i];
		}
	}

	for(j=0;j<=stacks;j++)
	{
		angle = M_PI*j/stacks;
		if(needCache2)
		{
			//if(qobj->orientation==GLU_OUTSIDE)
			{
				sinCache2b[j] = std::sin(angle);
				cosCache2b[j] = std::cos(angle);
			}
			/*else
			{
				sinCache2b[j] = -std::sin(angle);
				cosCache2b[j] = -std::cos(angle);
			}*/
		}
		sinCache1b[j] = radius*std::sin(angle);
		cosCache1b[j] = radius*std::cos(angle);
	}
	/* Make sure it comes to a point */
	sinCache1b[0] = 0;
	sinCache1b[stacks] = 0;

	/*if(needCache3)
	{
		for(i=0;i<slices;i++)
		{
			angle = 2*M_PI*(i-0.5)/slices;
			sinCache3a[i] = std::sin(angle);
			cosCache3a[i] = std::cos(angle);
		}
		for(j=0;j<=stacks;j++)
		{
			angle = M_PI*(j-0.5)/stacks;
			//if(qobj->orientation==GLU_OUTSIDE)
			{
				sinCache3b[j] = std::sin(angle);
				cosCache3b[j] = std::cos(angle);
			}
			//else
			//{
			//	sinCache3b[j] = -std::sin(angle);
			//	cosCache3b[j] = -std::cos(angle);
			//}
		}
	}*/

	sinCache1a[slices] = sinCache1a[0];
	cosCache1a[slices] = cosCache1a[0];
	if(needCache2)
	{
		sinCache2a[slices] = sinCache2a[0];
		cosCache2a[slices] = cosCache2a[0];
	}
	/*if(needCache3)
	{
		sinCache3a[slices] = sinCache3a[0];
		cosCache3a[slices] = cosCache3a[0];
	}*/

	hit:

	//switch(qobj->drawStyle)
	//{
	//case GLU_POINT: //REMOVED
	//case GLU_SILHOUETTE: //(Mesa code implemented as GLU_LINE_
	//case GLU_LINE: //REMOVED	
	//case GLU_FILL:
		/* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
		 ** We don't do it when texturing because we need to respecify the
		 ** texture coordinates of the apex for every adjacent vertex (because
		 ** it isn't a constant for that point)
		 */
		//if(!(qobj->textureCoords))
		{
			//REMOVED
		}
		//else
		{
			start = 0;
			finish = stacks;
		}
		for(j=start;j<finish;j++)
		{
			zLow = cosCache1b[j];
			zHigh = cosCache1b[j+1];
			sintemp1 = sinCache1b[j];
			sintemp2 = sinCache1b[j+1];
			//switch(qobj->normals)
			{
			//case GLU_FLAT:
			//	sintemp4 = sinCache3b[j+1];
			//	costemp4 = cosCache3b[j+1];
			//	break;
			//case GLU_SMOOTH:
				//if(qobj->orientation==GLU_OUTSIDE)
				{
					sintemp3 = sinCache2b[j+1];
					costemp3 = cosCache2b[j+1];
					sintemp4 = sinCache2b[j];
					costemp4 = cosCache2b[j];
				}
				/*else
				{
					sintemp3 = sinCache2b[j];
					costemp3 = cosCache2b[j];
					sintemp4 = sinCache2b[j+1];
					costemp4 = cosCache2b[j+1];
				}*/
			//	break;
			//default:
			//	break;
			}

			glBegin(GL_QUAD_STRIP);
			for(i=0;i<=slices;i++)
			{
				//switch(qobj->normals)
				{
				//case GLU_SMOOTH:
					glNormal3f(sinCache2a[i]*sintemp3,cosCache2a[i]*sintemp3,costemp3);
				//	break;
				//case GLU_FLAT:
				//case GLU_NONE:
				//default:
				//	break;
				}
				//if(qobj->orientation==GLU_OUTSIDE)
				{
					//if(qobj->textureCoords)
					{
						glTexCoord2f(1-(float)i/slices,1-(float)(j+1)/stacks);
					}
					glVertex3f(sintemp2*sinCache1a[i],sintemp2*cosCache1a[i],zHigh);
				}
				/*else
				{
					if(qobj->textureCoords)
					{
						glTexCoord2f(1-(float)i/slices,1-(float)j/stacks);
					}
					glVertex3f(sintemp1*sinCache1a[i],
					sintemp1*cosCache1a[i],zLow);
				}*/
				//switch(qobj->normals)
				{
				//case GLU_SMOOTH:
					glNormal3f(sinCache2a[i]*sintemp4,cosCache2a[i]*sintemp4,costemp4);
				//	break;
				//case GLU_FLAT:
				//	glNormal3f(sinCache3a[i]*sintemp4,cosCache3a[i]*sintemp4,costemp4);
				//	break;
				//case GLU_NONE:
				//default:
				//	break;
				}
				//if(qobj->orientation==GLU_OUTSIDE)
				{
					//if(qobj->textureCoords)
					{
						glTexCoord2f(1-(float)i/slices,1-(float)j/stacks);
					}
					glVertex3f(sintemp1*sinCache1a[i],sintemp1*cosCache1a[i],zLow);
				}
				/*else
				{
					if(qobj->textureCoords)
					{
						glTexCoord2f(1-(float)i/slices,1-(float)(j+1)/stacks);
					}
					glVertex3f(sintemp2*sinCache1a[i],sintemp2*cosCache1a[i],zHigh);
				}*/
			}
			glEnd();
		}
	//	break;	
	//}
}


//---.
}//<-'
