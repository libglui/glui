/**********************************************************************

  arcball.cpp


          --------------------------------------------------

  GLUI User Interface Toolkit
  Copyright (c) 1998 Paul Rademacher
     Feb 1998, Paul Rademacher (rademach@cs.unc.edu)
     Oct 2003, Nigel Stewart - GLUI Code Cleaning

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

**********************************************************************/

#include "../include/glui_internal.h" //PCH
#include "../include/glui_arcball.h" //"arcball.h"

namespace GLUI_Library
{//-.
//<-'

/**************************************** Arcball::Arcball() ****/

/* Default (void) constructor for Arcball                       */

Arcball::Arcball()
{
	rot_ptr = &rot; init();
}

/**************************************** Arcball::Arcball() ****/

/* Takes as argument a mat4 to use instead of the internal rot  */

Arcball::Arcball(mat4 *mtx)
{
	rot_ptr = mtx;
}

/**************************************** Arcball::Arcball() ****/

/* A constructor that accepts the screen center and arcball radius*/

Arcball::Arcball(const vec2 &c, double r)
{
	rot_ptr = &rot; init(); set_params(c,r);
}

/************************************** Arcball::set_params() ****/

void Arcball::set_params(const vec2 &c, double r)
{
	center = c; radius = r;
}

/*************************************** Arcball::init() **********/

void Arcball::init()
{
	center.set(0,0);
	radius = 1;
	q_now = quat_identity();
	*rot_ptr = identity3D();
	q_increment = quat_identity();
//	rot_increment = identity3D();
	is_mouse_down = false;
	is_spinning = false;
	damp_factor = 0;
//	zero_increment = true;
}

/*********************************** Arcball::mouse_to_sphere() ****/

vec3 Arcball::mouse_to_sphere(const vec2 &p, bool cx, bool cy)
{
	//2019: This doesn't work right... okay that's much better.
	//vec2 v2 = (p-center)/radius;
	vec2 v2 = (p-center)/(radius*M_PI);
	v2[1] = -v2[1]; //2019: Call sites were inverting their y.
	vec3 v3(v2[0],v2[1],0);

	double mag = v2*v2;

	if(mag>1)
	{
		v3.normalize();
	}
	else v3[VZ] = sqrt(1-mag);

	assert(!cx||!cy); //Z? 

	if(cx||cy) v3 = constrain_vector(v3,vec3(cx,cy,0));

	return v3;
}

/************************************ Arcball::constrain_vector() ****/

vec3 Arcball::constrain_vector(const vec3 &vector, const vec3 &axis)
{
	return (vector-(vector*axis)*axis).normalize();
}

/************************************ Arcball::mouse_down() **********/

void Arcball::mouse_down(int x, int y)
{
	prev_pt.set((double)x,(double)y);
	is_mouse_down = true;

	q_increment = quat_identity();
//	rot_increment = identity3D();
//	zero_increment = true;
}

/************************************ Arcball::mouse_up() **********/

void Arcball::mouse_up()
{
	//q_now = q_drag*q_now; 
	
	is_mouse_down = false;
}

/********************************** Arcball::mouse_motion() **********/

#ifndef EPS //FUDGE
#define EPS 0.00001 //Used by quaternion.cpp only
#endif
static void glui_arcball_set_axis_angle(quat &q, vec3 &axis, double s)
{
	double rad = s/2; //radians

	q.s = cos(rad); q.v = axis*sin(rad); 
}
static void glui_arcball_DecomposeSwingTwist(quat &q, vec3 &twistAxis, quat &swing, quat &twist)
{	 
  //http://allenchou.net/2018/05/game-math-swing-twist-interpolation-sterp/

	// singularity: rotation by 180 degree
	vec3 &r = q.v; if(r.length2()<EPS)
	{
		if(1) //singularity:
		{
			twist = q; swing = quat_identity();
			return;
		}

		//THIS BLOCK IS UNTESTED/UNVERFIED CODE//

		//Vector3 rotatedTwistAxis = q * twistAxis;
		vec3 rotatedTwistAxis = q.xform(twistAxis);
		vec3 swingAxis = twistAxis%rotatedTwistAxis;
 
		if(swingAxis.length2()>EPS) //MathUtil.Epsilon
		{
			//vec3.Angle(twistAxis, rotatedTwistAxis);
			double swingAngle = acos(twistAxis*rotatedTwistAxis);
			glui_arcball_set_axis_angle(swing,swingAxis,swingAngle);
		}
		else
		{
			// more singularity: 
			// rotation axis parallel to twist axis
			swing = quat_identity(); // no swing
		}
 
		// always twist 180 degree on singularity
		//twist = Quaternion.AngleAxis(180.0f, twistAxis);
		glui_arcball_set_axis_angle(twist,twistAxis,M_PI);
		return;
	}
 
	// meat of swing-twist decomposition
	//vec3 p = vec3.Project(r,twistAxis);
	double r_twistAxis = r*twistAxis;	
	vec3 p = (r_twistAxis)*twistAxis;
	twist = quat(p,q.s);
	twist.normalize();

	//This is mainly because I'm only 99%
	//confident (r*twistAxis)*twistAxis is
	//equivalent to vec3.Project. I'm also
	//unsure if -1 or 1 should be used for 
	//the Z axis. Or if it matters to this.
	if(0==r_twistAxis) projection_failure:
	{	
		//If r*twistAxis is zero, p is NaN. 
		//I think this is the result anyway
		//but it fails acos(p.normalize()*r).
		twist = quat_identity(); swing = q;
		return;
	}	
	else
	{
		//Looks alright in animation.
		//testing vector projection accuracy
		double angle = acos(p.normalize()*r);
		//if(angle<EPS||angle>M_PI/2-EPS)
		{
			//Hitting a NaN. It's not visible.
			if(angle!=angle) goto failsafe; 

			assert(angle<DBL_MAX&&angle>-DBL_MAX);

			//Can't be too careful. Don't want balls disappearing, etc.
			if(angle<EPS||angle>M_PI/2+EPS) failsafe:
			{
				if(1) assert(0);
				
				goto projection_failure;
			}
		}
	}
	
	//I can't find sources but I believe there two (equivalent) ways
	//to invert a quaternion, and I don't understand why flipping the
	//sign of w is less common than flipping the vector component sign.
	//swing = q * Quaternion.Inverse(twist);
	twist.v = -twist.v; //twist.s = -twist.s;
	swing = q*twist;
	twist.v = -twist.v; //twist.s = -twist.s; 
}

void Arcball::mouse_motion(int x, int y, double shift, bool cx, bool cy)
{	
	vec2 new_pt((double)x,(double)y);	

	if(0)
	//Don't GLUT motion events only fire if the mouse cursor moves?
	//2019: I don't understand why this happens, but I believe it's
	//responsible for the ball not being fluid with micro movements.
	//Another factor is no inertia carries over from past movements.
	if(prev_pt==new_pt) return;

	/* Set the X constraint if CONTROL key is pressed, Y if ALT key */
	//set_constraints(ctrl,alt);	

	float unshift = radius; 

	if(shift) radius*=shift; 

	if(cx&&cy) //2019
	{
		//This offers some manual control over
		//roll. Below roll is removed from the
		//rotation gesture, but this isn't the
		//same as the uprightness of the model.
		double x = new_pt[0], xx = prev_pt[0];				
		double s = (x-xx)/radius*M_PI;
		if(fabs(s)>EPS)
		{
			vec3 z(0,0,-1);
			glui_arcball_set_axis_angle(q_drag,z,s);
			//q_drag.normalize();
		}
		else q_drag = quat_identity();
	}
	else 
	{	
		vec3 v0 = mouse_to_sphere(prev_pt,cx,cy);
		vec3 v1 = mouse_to_sphere(new_pt,cx,cy);
	
		vec3 cross = v0%v1; //v0^v1;

		q_drag.set(cross,v0*v1);

		if(!cx&&!cy) //2019
		{
			if(1) //Try to remove roll. (It's a nuisance.)
			{
				q_drag.normalize();

				//-1 or 1? Probably they are equivalent. 1
				//feels maybe better. Maybe my imagination.
				quat s,t; vec3 z(0,0,1); 
				glui_arcball_DecomposeSwingTwist(q_drag,z,s,t);
				q_drag = s; //t*s; 				
				/*testing
				for(int i=0;i<2;i++) if(fabs(q_drag.v[i])<=0.00001)
				{
					q_drag.v[i] = 0;
				}				
				q_drag.v[2] = 0;*/
			}

			q_drag.normalize();
					
			//Note: This Euler decomosition formula has some roll even 
			//though the Qz part of the quaternion is 0.
			#if 0 && defined(_DEBUG)	
			double Qx = q_drag.v[0], Qy = q_drag.v[1];
			double Qz = q_drag.v[2], Qw = q_drag.s;
			double Ex = -atan2(2*Qy*Qw+2*Qx*Qz,1-2*(Qz*Qz+Qw*Qw)); // Yaw 
			double Ey = -asin(2*(Qy*Qz-Qw*Qx)); // Pitch 
			double Ez = atan2(2*Qy*Qx+2*Qz*Qw,1-2*(Qx*Qx+Qz*Qz));
			if(fabs(Ex)>M_PI/2) Ex = (M_PI-fabs(Ex))*(Ex>0?1:-1);
			printf("Euler:   <%+3.4f %+3.4f         %+3.6f (%+3.6f)>\n",Ex,Ey,Ez,Qz);
			#endif
		}
	}

	/*2019: This way is unstable.
	// *rot_ptr = (q_drag * q_now).to_mat4();
	mat4 temp = q_drag.to_mat4();
	*rot_ptr = *rot_ptr*temp;
	prev_pt = new_pt;
	* We keep a copy of the current incremental rotation (= q_drag) ?/
	q_increment = q_drag;	
	rot_increment = q_increment.to_mat4();
	*/
	prev_pt = new_pt;
	q_increment = q_drag;
	q_now = q_increment*q_now; q_now.normalize();
	*rot_ptr = q_now.to_mat4();
	
	radius = unshift; //set_constraints(false,false);  //2019

	if(q_increment.s<0.999999)
	{
		is_spinning = true; //zero_increment = false;
	}
	else
	{
		is_spinning = false; //zero_increment = true;
	}
}

/********************************** Arcball::mouse_motion() **********/

void Arcball::mouse_motion(int x, int y)
{
	mouse_motion(x,y,0,0,0);
}

/***************************** Arcball::set_constraints() **********/

/*void Arcball::set_constraints(bool cx, bool cy)
{
	constraint_x = cx; constraint_y = cy;
}*/

/***************************** Arcball::idle() *********************/

void Arcball::idle()
{
	if(is_mouse_down)
	{
		is_spinning = false; //zero_increment = true;
	}

	//2019: Switching to same convention as Rotation control's.
	//(I THINK damp_factor<1 HERE DID NOT MATCH 1-damp_factor.)
	//if(damp_factor<1) q_increment.scale_angle(1-damp_factor);
	if(damp_factor<1) q_increment.scale_angle(damp_factor);

	q_increment.normalize(); //2019
	q_now = q_increment*q_now; q_now.normalize(); //2019

	//2019: This way is unstable.
	//rot_increment = q_increment.to_mat4();
	*rot_ptr = q_now.to_mat4();

	if(q_increment.s>=0.999999)
	{
		is_spinning = false; //zero_increment = true;
	}
}

/************************ Arcball::set_damping() *********************/

void Arcball::set_damping(double d)
{
	damp_factor = d;
}

//---.
}//<-'
