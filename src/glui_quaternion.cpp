/***********************************************************************

  quaternion.cpp - A quaternion class

  -------------------------------------------------------------------

  GLUI User Interface Toolkit
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

 ************************************************************************

  Feb 1998, Paul Rademacher (rademach@cs.unc.edu)
  Oct 2003, Nigel Stewart - GLUI Code Cleaning

 ************************************************************************/

#include "../include/glui_internal.h" //PCH
#include "../include/glui_quaternion.h" //"quaternion.h"

namespace GLUI_Library
{//-.
//<-'

//#include "glui_internal.h"
#ifndef EPS //FUDGE
#define EPS 0.00001 //Used by quaternion.cpp only
#endif
#ifndef DEG2RAD
#define DEG2RAD(x) ((x)/180*M_PI) //Used by quaternion.cpp only
#define RAD2DEG(x) ((x)/M_PI*180) //Used by quaternion.cpp only
#endif

//2019: This was missing. Probably there is more undefined ones.
quat &quat::normalize() 
{
		// it is up to caller to avoid divide-by-zero

	double l2 = v.length2()+s*s;
	double l = sqrt(l2);
	v/=l; s/=l; return *this;
}
vec3 quat::xform(const vec3 &vv) // q*v*q-1
{
	quat q(vv,0),conj(-v,s); //or (v,-s) ?

	q = (*this)*q*conj; return q.v;
}
void quat::set(double x, double y, double z, double w)
{
	v.set(x,y,z); s = w;
}

quat::quat(mat4 &m)
{
	set(m);
}
quat &quat::operator=(const mat4 &m)
{
	set(m); return *this;
}
void quat::set(const mat4 &m) //2019
{
	double ss,t = m[0][0]+m[1][1]+m[2][2]; if(t>0) 
	{
		ss = std::sqrt(1+t)*2;
		//May want to add a transposed layout here.
		//set((m[2][1]-m[1][2])/ss,(m[0][2]-m[2][0])/ss,(m[1][0]-m[0][1])/ss,ss/4); 
		set((m[1][2]-m[2][1])/ss,(m[2][0]-m[0][2])/ss,(m[0][1]-m[1][0])/ss,ss/4); 
	} 
	else if(m[0][0]>m[1][1]&&m[0][0]>m[2][2]) 
	{ 
		ss = std::sqrt(1+m[0][0]-m[1][1]-m[2][2])*2;
		//set(ss/4,(m[1][0]+m[0][1])/ss,(m[0][2]+m[2][0])/ss,(m[2][1]-m[1][2])/ss); 
		set(ss/4,(m[0][1]+m[1][0])/ss,(m[2][0]+m[0][2])/ss,(m[1][2]-m[2][1])/ss); 
	}
	else if(m[1][1]>m[2][2]) 
	{
		ss = std::sqrt(1+m[1][1]-m[0][0]-m[2][2])*2;
		//set((m[1][0]+m[0][1])/ss,ss/4,(m[2][1]+m[1][2])/ss,(m[0][2]-m[2][0])/ss); 
		set((m[0][1]+m[1][0])/ss,ss/4,(m[1][2]+m[2][1])/ss,(m[2][0]-m[0][2])/ss); 
	} 
	else
	{
		ss = std::sqrt(1+m[2][2]-m[0][0]-m[1][1])*2;
		//set((m[0][2]+m[2][0])/ss,(m[2][1]+m[1][2])/ss,ss/4,(m[1][0]-m[0][1])/ss); 
		set((m[2][0]+m[0][2])/ss,(m[1][2]+m[2][1])/ss,ss/4,(m[0][1]-m[1][0])/ss); 
	}
}

/******************************************* constructors **************/

quat::quat()
{
	*this = quat_identity();
}

quat::quat(const double x, const double y, const double z, const double w)
{
	v.set(x,y,z); s = w;
}

quat::quat(const float *f)
{
	v[0] = f[0];
	v[1] = f[1];
	v[2] = f[2]; s = f[3];
}

quat::quat(const double *d)
{
	v[0] = (double)d[0];
	v[1] = (double)d[1];
	v[2] = (double)d[2]; s = (double)d[3];
}

quat::quat(const quat &q)
{
	v = q.v; s = q.s;
}

void quat::set(const vec3 &vv, const double ss)
{
	v = vv; s = ss;
}

quat &quat::operator=(const quat &q)
{
	v = q.v;
	s = q.s; return *this;
}

/******** quat friends ************/

quat operator+(const quat &a, const quat &b)
{
	return quat(a.s+b.s,a.v+b.v);
}

quat operator-(const quat &a, const quat &b)
{
	return quat(a.s-b.s,a.v-b.v);
}

quat operator-(const quat &a)
{
	return quat(-a.s,-a.v);
}

quat operator*(const quat &a, const quat &b)
{
	//NOTE: % is cross-product. This was broken because of
	//operator precedence.
	return quat(a.s*b.s-a.v*b.v,a.s*b.v+b.s*a.v+(a.v%b.v));
}

quat operator*(const quat &a, const double t)
{
	return quat(a.v*t,a.s*t);
}

quat operator*(const double t, const quat &a)
{
	return quat(a.v*t,a.s*t);
}

mat4 quat::to_mat4()const
{
	double xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;

	double t = 2/(v*v+s*s);

	xs = v[VX]*t;  ys = v[VY]*t;  zs = v[VZ]*t;
	wx = s*xs;     wy = s*ys;     wz = s*zs;
	xx = v[VX]*xs; xy = v[VX]*ys; xz = v[VX]*zs;
	yy = v[VY]*ys; yz = v[VY]*zs; zz = v[VZ]*zs;

	mat4 matrix(
	1-(yy+zz),xy+wz,xz-wy,0,
	xy-wz,1-(xx+zz),yz+wx,0,
	xz+wy,yz-wx,1-(xx+yy),0,
	0,0,0,1);

	return matrix;
}

/************************************************* quat_identity() *****/

/* Returns quaternion identity element                                 */

quat quat_identity()
{
	return quat(vec3(0,0,0),1);
}

/************************************************ quat_slerp() ********/

/* Quaternion spherical interpolation                                 */

quat quat_slerp(const quat &from, const quat &to, double t)
{
	quat to1; double omega,cosom,sinom,scale0,scale1;

	/* calculate cosine */
	cosom = from.v*to.v+from.s+to.s;

	/* Adjust signs (if necessary) */
	if(cosom<0)
	{
		cosom = -cosom;
		to1 = -to;
	}
	else to1 = to;

	/* Calculate coefficients */
	if((1-cosom)>EPS)
	{
		/* standard case (slerp) */
		omega = (double)acos(cosom);
		sinom = (double)sin(omega);
		scale0 = (double)sin((1-t)*omega)/sinom;
		scale1 = (double)sin(t*omega)/sinom;
	}
	else
	{
		/* 'from' and 'to' are very close - just do linear interpolation */
		scale0 = 1-t;
		scale1 = t;
	}

	return scale0*from+scale1*to1;
}

/********************************************** set_angle() ************/

/* set rot angle (degrees)                                             */

void quat::set_angle(double f)
{
	vec3 axis = get_axis();

	s = (double)cos(DEG2RAD(f)/2);

	v = axis*(double)sin(DEG2RAD(f)/2);
}

/********************************************** scale_angle() ************/

/* scale rot angle (degrees)                                             */

void quat::scale_angle(double f)
{
	set_angle(f*get_angle());
}

/********************************************** get_angle() ************/

/* get rot angle (degrees). Assumes s is between -1 and 1             */

double quat::get_angle()const
{
	return (double)RAD2DEG(2*acos(s));
}

/********************************************* get_axis() **************/

vec3 quat::get_axis()const
{
	double scale = (double)sin(acos(s));

	if(scale<EPS&&scale>-EPS) //FUDGE
	{
		return vec3(0,0,0);
	}
	else return v/scale;
}

/******************************************* quat::print() ************/

void quat::print(FILE *dest, const char *name)const
{
	fprintf(dest,"%s:   v:<%3.2f %3.2f %3.2f>  s:%3.2f\n",name,v[0],v[1],v[2],s);
}

//---.
}//<-'
