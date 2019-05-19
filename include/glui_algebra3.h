/*

  algebra3.cpp,  algebra3.h -  C++ Vector and Matrix Algebra routines

  GLUI User Interface Toolkit
  Copyright (c) 1998 Paul Rademacher

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

 */

/**************************************************************************

  There are three vector classes and two matrix classes: vec2,  vec3, 
  vec4,  mat3,  and mat4.

  All the standard arithmetic operations are defined,  with '*'
  for dot product of two vectors and multiplication of two matrices, 
  and '^' for cross product of two vectors.

  Additional functions include length(),  normalize(),  homogenize for
  vectors,  and print(),  set(),  apply() for all classes.

  There is a function transpose() for matrices,  but note that it
  does not actually change the matrix, 

  When multiplied with a matrix,  a vector is treated as a row vector
  if it precedes the matrix (v*M),  and as a column vector if it
  follows the matrix (M*v).

  Matrices are stored in row-major form.

  A vector of one dimension (2d,  3d,  or 4d) can be cast to a vector
  of a higher or lower dimension.  If casting to a higher dimension, 
  the new component is set by default to 1.0,  unless a value is
  specified:
	 vec3 a(1.0,  2.0,  3.0 );
	 vec4 b( a,  4.0 );       // now b == {1.0,  2.0,  3.0,  4.0};
  When casting to a lower dimension,  the vector is homogenized in
  the lower dimension.  E.g.,  if a 4d {X, Y, Z, W} is cast to 3d,  the
  resulting vector is {X/W,  Y/W,  Z/W}.  It is up to the user to
  insure the fourth component is not zero before casting.

  There are also the following function for building matrices:
	 identity2D(),  translation2D(),  rotation2D(), 
	 scaling2D(),   identity3D(),     translation3D(), 
	 rotation3D(),  rotation3Drad(),   scaling3D(), 
	 perspective3D()

  NOTE: When compiling for Windows,  include this file first,  to avoid
		certain name conflicts

  ---------------------------------------------------------------------

  Author: Jean-Francois DOUEg
  Revised: Paul Rademacher
  Version 3.2 - Feb 1998
  Revised: Nigel Stewart (GLUI Code Cleaning)

 **************************************************************************/

#ifndef GLUI_ALGEBRA3_H
#define GLUI_ALGEBRA3_H
	
#include "glui_internal.h" //glui_internal.h

namespace GLUI_Library
{//-.
//<-'

// this line defines a new type: pointer to a function which returns a
// double and takes as argument a double
typedef double(*V_FCT_PTR)(double);

class vec2;
class vec3;
class vec4;
class mat3;
class mat4;

enum{ VX, VY, VZ, VW }; // axes

enum{ PA, PB, PC, PD }; // planes

enum{ RED, GREEN, BLUE, ALPHA }; // colors

enum{ KA, KD, KS, ES }; // phong coefficients

/****************************************************************
 *                                                              *
 *              2D Vector                                       *
 *                                                              *
 ****************************************************************/

class vec2
{
	friend class vec3;

protected:

	double n[2];

public:

	// Constructors

	vec2();
	vec2(double x, double y);
	vec2(const vec2 &v); // copy constructor
	vec2(const vec3 &v); // cast v3 to v2
	vec2(const vec3 &v, int dropAxis); // cast v3 to v2

	// Assignment operators

	vec2 &operator=(const vec2 &v); // assignment of a vec2
	vec2 &operator+=(const vec2 &v); // incrementation by a vec2
	vec2 &operator-=(const vec2 &v); // decrementation by a vec2
	vec2 &operator*=(double d); // multiplication by a constant
	vec2 &operator/=(double d); // division by a constant

	// special functions

	double length()const; // length of a vec2
	double length2()const; // squared length of a vec2
	vec2 &normalize(); // normalize a vec2
	vec2 &apply(V_FCT_PTR fct); // apply a func. to each component
	void set(double x, double y); // set vector

	double &operator[](int i); // indexing
	const double &operator[](int i)const; // indexing

	// friends

	friend vec2 operator-(const vec2 &v); // -v1
	friend vec2 operator+(const vec2 &a, const vec2 &b); // v1 + v2
	friend vec2 operator-(const vec2 &a, const vec2 &b); // v1 - v2
	friend vec2 operator*(const vec2 &a, double d); // v1 * 3.0
	friend vec2 operator*(double d, const vec2 &a); // 3.0 * v1
	friend vec2 operator*(const mat3 &a, const vec2 &v); // M . v
	friend vec2 operator*(const vec2 &v, const mat3 &a); // v . M
	friend double operator*(const vec2 &a, const vec2 &b); // dot product
	friend vec2 operator/(const vec2 &a, double d); // v1 / 3.0
	friend void operator ^ (const vec2 &a, const vec2 &b); // cross product
	friend vec3 operator % (const vec2 &a, const vec2 &b); // cross product
	friend int operator==(const vec2 &a, const vec2 &b); // v1 == v2 ?
	friend int operator!=(const vec2 &a, const vec2 &b); // v1 != v2 ?
	//friend ostream &operator<<(ostream& s, vec2 &v); // output to stream
	//friend istream &operator>>(istream& s, vec2 &v); // input from strm.
	friend void swap(vec2 &a, vec2 &b); // swap v1 & v2
	friend vec2 min_vec(const vec2 &a, const vec2 &b); // min(v1,v2)
	friend vec2 max_vec(const vec2 &a, const vec2 &b); // max(v1,v2)
	friend vec2 prod(const vec2 &a, const vec2 &b); // term by term *
};

/****************************************************************
 *                                                              *
 *               3D Vector                                      *
 *                                                              *
 ****************************************************************/

class vec3
{
	friend class vec2;
	friend class vec4;
	friend class mat3;

protected:

	double n[3];

public:

	// Constructors

	vec3();
	vec3(double x, double y, double z);
	vec3(const vec3 &v); // copy constructor
	vec3(const vec2 &v); // cast v2 to v3
	vec3(const vec2 &v, double d); // cast v2 to v3
	vec3(const vec4 &v); // cast v4 to v3
	vec3(const vec4 &v, int dropAxis); // cast v4 to v3

	// Assignment operators

	vec3 &operator=(const vec3 &v); // assignment of a vec3
	vec3 &operator+=(const vec3 &v); // incrementation by a vec3
	vec3 &operator-=(const vec3 &v); // decrementation by a vec3
	vec3 &operator*=(double d); // multiplication by a constant
	vec3 &operator/=(double d); // division by a constant

	// special functions

	double length()const; // length of a vec3
	double length2()const; // squared length of a vec3
	vec3& normalize(); // normalize a vec3
	vec3& homogenize(); // homogenize (div by Z)
	vec3& apply(V_FCT_PTR fct); // apply a func. to each component
	void set(double x, double y, double z); // set vector

	void print(FILE *file, const char *name)const; // print vector to a file


	double &operator[](int i); // indexing
	const double &operator[](int i)const; // indexing

	// friends

	friend vec3 operator-(const vec3 &v); // -v1
	friend vec3 operator+(const vec3 &a, const vec3 &b); // v1 + v2
	friend vec3 operator-(const vec3 &a, const vec3 &b); // v1 - v2
	friend vec3 operator*(const vec3 &a, double d); // v1 * 3.0
	friend vec3 operator*(double d, const vec3 &a); // 3.0 * v1
	friend vec3 operator*(const mat4 &a, const vec3 &v); // M . v
	friend vec3 operator*(const vec3 &v, const mat4 &a); // v . M
	friend double operator*(const vec3 &a, const vec3 &b); // dot product
	friend vec3 operator/(const vec3 &a, double d); // v1 / 3.0
	//2019: % has equal precedence to * nor less than +.
	friend void operator^(const vec3 &a, const vec3 &b); // cross product
	friend vec3 operator%(const vec3 &a, const vec3 &b); // cross product
	friend int operator==(const vec3 &a, const vec3 &b); // v1 == v2 ?
	friend int operator!=(const vec3 &a, const vec3 &b); // v1 != v2 ?
	//friend ostream &operator<<(ostream &s, vec3 &v); // output to stream
	//friend istream &operator>>(istream &s, vec3 &v); // input from strm.
	friend void swap(vec3 &a, vec3 &b); // swap v1 & v2
	friend vec3 min_vec(const vec3 &a, const vec3 &b); // min(v1,v2)
	friend vec3 max_vec(const vec3 &a, const vec3 &b); // max(v1,v2)
	friend vec3 prod(const vec3 &a, const vec3 &b); // term by term *

	// necessary friend declarations

	friend vec2 operator*(const mat3 &a, const vec2 &v); // linear transform
	friend vec3 operator*(const mat3 &a, const vec3 &v); // linear transform
	friend mat3 operator*(const mat3 &a, const mat3 &b); // matrix 3 product
};

/****************************************************************
 *                                                              *
 *              4D Vector                                       *
 *                                                              *
 ****************************************************************/

class vec4
{
	friend class vec3;
	friend class mat4;

protected:

	double n[4];

public:

	// Constructors

	vec4();
	vec4(double x, double y, double z, double w);
	vec4(const vec4 &v); // copy constructor
	vec4(const vec3 &v); // cast vec3 to vec4
	vec4(const vec3 &v, double d); // cast vec3 to vec4

	// Assignment operators

	vec4 &operator=(const vec4 &v); // assignment of a vec4
	vec4 &operator+=(const vec4 &v); // incrementation by a vec4
	vec4 &operator-=(const vec4 &v); // decrementation by a vec4
	vec4 &operator*=(double d); // multiplication by a constant
	vec4 &operator/=(double d); // division by a constant

	// special functions

	double length()const; // length of a vec4
	double length2()const; // squared length of a vec4
	vec4 &normalize(); // normalize a vec4
	vec4 &apply(V_FCT_PTR fct); // apply a func. to each component
	vec4 &homogenize();

	void print(FILE *file, const char *name)const; // print vector to a file

	void set(double x, double y, double z, double a);

	double &operator[](int i); // indexing
	const double &operator[](int i)const; // indexing

	// friends

	friend vec4 operator-(const vec4 &v); // -v1
	friend vec4 operator+(const vec4 &a, const vec4 &b); // v1 + v2
	friend vec4 operator-(const vec4 &a, const vec4 &b); // v1 - v2
	friend vec4 operator*(const vec4 &a, double d); // v1 * 3.0
	friend vec4 operator*(double d, const vec4 &a); // 3.0 * v1
	friend vec4 operator*(const mat4 &a, const vec4 &v); // M . v
	friend vec4 operator*(const vec4 &v, const mat4 &a); // v . M
	friend double operator*(const vec4 &a, const vec4 &b); // dot product
	friend vec4 operator/(const vec4 &a, double d); // v1 / 3.0
	friend int operator==(const vec4 &a, const vec4 &b); // v1 == v2 ?
	friend int operator!=(const vec4 &a, const vec4 &b); // v1 != v2 ?
	//friend ostream &operator<<(ostream &s, vec4& v); // output to stream
	//friend istream &operator>>(istream &s, vec4& v); // input from strm.
	friend void swap(vec4 &a, vec4 &b); // swap v1 & v2
	friend vec4 min_vec(const vec4 &a, const vec4 &b); // min(v1,v2)
	friend vec4 max_vec(const vec4 &a, const vec4 &b); // max(v1,v2)
	friend vec4 prod(const vec4 &a, const vec4 &b); // term by term *

	// necessary friend declarations

	friend vec3 operator*(const mat4 &a, const vec3 &v); // linear transform
	friend mat4 operator*(const mat4 &a, const mat4 &b); // matrix 4 product
};

/****************************************************************
 *                                                              *
 *             3x3 Matrix                                       *
 *                                                              *
 ****************************************************************/

class mat3
{
protected:

	vec3 v[3];

public:

	// Constructors

	mat3();
	mat3(const vec3 &v0, const vec3 &v1, const vec3 &v2);
	mat3(const mat3 &m);

	// Assignment operators

	mat3 &operator=(const mat3 &m); // assignment of a mat3
	mat3 &operator+=(const mat3 &m); // incrementation by a mat3
	mat3 &operator-=(const mat3 &m); // decrementation by a mat3
	mat3 &operator*=(double d); // multiplication by a constant
	mat3 &operator/=(double d); // division by a constant

	// special functions

	mat3 transpose()const; // transpose
	mat3 inverse()const; // inverse
	mat3 &apply(V_FCT_PTR fct); // apply a func. to each element

	void print(FILE *file, const char *name)const; // print matrix to a file

	void set(const vec3 &v0, const vec3 &v1, const vec3 &v2);

	vec3 &operator[](int i); // indexing
	const vec3 &operator[](int i)const; // indexing

	// friends

	friend mat3 operator-(const mat3 &a); // -m1
	friend mat3 operator+(const mat3 &a, const mat3 &b); // m1 + m2
	friend mat3 operator-(const mat3 &a, const mat3 &b); // m1 - m2
	friend mat3 operator*(const mat3 &a, const mat3 &b); // m1 * m2
	friend mat3 operator*(const mat3 &a, double d); // m1 * 3.0
	friend mat3 operator*(double d, const mat3 &a); // 3.0 * m1
	friend mat3 operator/(const mat3 &a, double d); // m1 / 3.0
	friend int operator==(const mat3 &a, const mat3 &b); // m1 == m2 ?
	friend int operator!=(const mat3 &a, const mat3 &b); // m1 != m2 ?
	//friend ostream &operator<<(ostream& s, mat3 &m); // output to stream
	//friend istream &operator>>(istream& s, mat3 &m); // input from strm.
	friend void swap(mat3 &a, mat3 &b); // swap m1 & m2

	// necessary friend declarations

	friend vec3 operator*(const mat3 &a, const vec3 &v); // linear transform
	friend vec2 operator*(const mat3 &a, const vec2 &v); // linear transform
};

/****************************************************************
 *                                                              *
 *             4x4 Matrix                                       *
 *                                                              *
 ****************************************************************/

class mat4
{
protected:

	vec4 v[4];

public:

	// Constructors

	mat4();
	mat4(const vec4 &v0, const vec4 &v1, const vec4 &v2, const vec4 &v3);
	mat4(const mat4 &m);
	mat4(double a00, double a01, double a02, double a03, 
	double a10, double a11, double a12, double a13, 
	double a20, double a21, double a22, double a23, 
	double a30, double a31, double a32, double a33);


	// Assignment operators

	mat4 &operator=(const mat4 &m); // assignment of a mat4
	mat4 &operator+=(const mat4 &m); // incrementation by a mat4
	mat4 &operator-=(const mat4 &m); // decrementation by a mat4
	mat4 &operator*=(double d); // multiplication by a constant
	mat4 &operator/=(double d); // division by a constant

	// special functions

	mat4 transpose()const; // transpose
	mat4 inverse()const; // inverse
	mat4 &apply(V_FCT_PTR fct); // apply a func. to each element

	void print(FILE *file, const char *name)const; // print matrix to a file

	vec4 &operator[](int i); // indexing
	const vec4 &operator[](int i)const; // indexing

	void swap_rows(int i, int j); // swap rows i and j
	void swap_cols(int i, int j); // swap cols i and j

	// friends

	friend mat4 operator-(const mat4 &a); // -m1
	friend mat4 operator+(const mat4 &a, const mat4 &b); // m1 + m2
	friend mat4 operator-(const mat4 &a, const mat4 &b); // m1 - m2
	friend mat4 operator*(const mat4 &a, const mat4 &b); // m1 * m2
	friend mat4 operator*(const mat4 &a, double d); // m1 * 4.0
	friend mat4 operator*(double d, const mat4 &a); // 4.0 * m1
	friend mat4 operator/(const mat4 &a, double d); // m1 / 3.0
	friend int operator==(const mat4 &a, const mat4 &b); // m1 == m2 ?
	friend int operator!=(const mat4 &a, const mat4 &b); // m1 != m2 ?
	//friend ostream& operator<<(ostream &s, mat4 &m); // output to stream
	//friend istream& operator>>(istream &s, mat4 &m); // input from strm.
	friend void swap(mat4 &a, mat4 &b); // swap m1 & m2

	// necessary friend declarations

	friend vec4 operator*(const mat4 &a, const vec4 &v); // linear transform
	//friend vec4 operator*(const vec4 &v, const mat4 &a); // linear transform
	friend vec3 operator*(const mat4 &a, const vec3 &v); // linear transform
	friend vec3 operator*(const vec3 &v, const mat4 &a); // linear transform
};

/****************************************************************
 *                                                              *
 *         2D functions and 3D functions                        *
 *                                                              *
 ****************************************************************/

mat3 identity2D(); // identity 2D
mat3 translation2D(const vec2 &v); // translation 2D
mat3 rotation2D(const vec2 &Center, double angleDeg); // rotation 2D
mat3 scaling2D(const vec2 &scaleVector); // scaling 2D
mat4 identity3D(); // identity 3D
mat4 translation3D(const vec3 &v); // translation 3D
mat4 rotation3D(const vec3 &Axis, double angleDeg); // rotation 3D
mat4 rotation3Drad(const vec3 &Axis, double angleRad); // rotation 3D
mat4 scaling3D(const vec3 &scaleVector); // scaling 3D
mat4 perspective3D(double d); // perspective 3D

vec3 operator*(const vec3 &v, const mat3 &a);
vec2 operator*(const vec2 &v, const mat3 &a);
vec3 operator*(const vec3 &v, const mat4 &a);
vec4 operator*(const vec4 &v, const mat4 &a);
		  
//---.
}//<-'

#endif //GLUI_ALGEBRA3_H
/*C1071*/