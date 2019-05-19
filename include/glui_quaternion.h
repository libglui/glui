/*****************************************************************************

  quaternion.h - A quaternion class

  GLUI User Interface Toolkit
  Copyright (c) 1998 Paul Rademacher

  ---------------------------------------------------------------------

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

  This software is provided 'as-is',  without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose, 
  including commercial applications,  and to alter it and redistribute it
  freely,  subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product,  an acknowledgment in the product documentation would be
  appreciated but is not required.
  2. Altered source versions must be plainly marked as such,  and must not be
  misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

 *****************************************************************************/

#ifndef GLUI_QUATERNION_H
#define GLUI_QUATERNION_H

#include "glui_algebra3.h" //"algebra3.h"
	
namespace GLUI_Library
{//-.
//<-'

// this line defines a new type: pointer to a function which returns a
// double and takes as argument a double
typedef double (*V_FCT_PTR)(double);

/****************************************************************
 *                    Quaternion                                *
 ****************************************************************/

class quat
{
public:

	vec3 v; // vector component

	double s; // scalar component

	// Constructors

	quat();
	quat(mat4 &m); //2019
	quat(double x, double y, double z, double w);
	quat(const vec3 &v, double s):v(v),s(s){}
	quat(double s, const vec3 &v):v(v),s(s){}
	quat(const float *f); // copy from four-element float array
	quat(const double *d); // copy from four-element double array
	quat(const quat &q); // copy from other quat

	// Assignment operators

	quat &operator=(const mat4 &v); // assignment of a quat
	quat &operator=(const quat &v); // assignment of a quat
	quat &operator+=(const quat &v); // incrementation by a quat
	quat &operator-=(const quat &v); // decrementation by a quat
	quat &operator*=(double d); // multiplication by a constant
	quat &operator/=(double d); // division by a constant

	// special functions

	double length()const; // length of a quat
	double length2()const; // squared length of a quat
	quat &normalize(); // normalize a quat
	quat &apply(V_FCT_PTR fct); // apply a func. to each component
	vec3 xform(const vec3 &v); // q*v*q-1
	mat4 to_mat4()const;
	void set_angle(double f); // set rot angle (degrees)
	void scale_angle(double f); // scale rot angle (degrees)
	double get_angle()const; // set rot angle (degrees)
	vec3 get_axis()const; // get axis

	void print(FILE *file, const char *name)const; // print to a file

	double &operator[](int i); // indexing
	const double &operator[](int i)const; // indexing

	void set(const mat4 &m); //2019
	void set(double x, double y, double z, double w); // set quat
	void set(const vec3 &v, double s); // set quat

	// friends

	friend quat operator-(const quat &v); // -q1
	friend quat operator+(const quat &a, const quat &b); // q1 + q2
	friend quat operator-(const quat &a, const quat &b); // q1 - q2
	friend quat operator*(const quat &a, double d); // q1 * 3.0
	friend quat operator*(double d, const quat &a); // 3.0 * q1
	friend quat operator*(const quat &a, const quat &b); // q1 * q2
	friend quat operator/(const quat &a, double d); // q1 / 3.0
	friend int operator==(const quat &a, const quat &b); // q1 == q2 ?
	friend int operator!=(const quat &a, const quat &b); // q1 != q2 ?
	friend void swap(quat &a, quat &b); // swap q1  &q2
	// friend quat min(const quat &a, const quat &b); -- min(q1,  q2)
	// friend quat max(const quat &a, const quat &b); -- max(q1,  q2)
	friend quat prod(const quat &a, const quat &b); // term by term mult
};

// Utility functions

quat quat_identity(); // Returns quaternion identity element
quat quat_slerp(const quat &from, const quat &to, double t);

//---.
}//<-'

#endif //GLUI_QUATERNION_H
/*C1071*/