/*
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
  3. This notice may not be removed or altered from any source
*/
#ifndef GLUI_INTERNAL_H
#define GLUI_INTERNAL_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath> 

#include <algorithm>
#include <limits>
#include <vector>

#ifndef M_PI
//#define M_PI 3.141592654
#define M_PI 3.14159265358979323846 //_USE_MATH_DEFINES?
#endif

/**** Return the ASCII control code given the non-control ASCII character */
#define CTRL(c) ((c>=('a'-1))?(c-'a'+1):(c-'A'+1))

//LITTLE USED (2019)
//Can use std::abs for float, but not int (until C++11)
#ifndef ABS
#define ABS(a) ((a)>=0?(a):(-(a)))
#endif

/***************** clamp a value to some fixed interval *************/
#ifndef CLAMP
#define CLAMP(x,lo,hi) { if((x)<(lo)){ (x)=(lo); }else if((x)>(hi)){ (x)=(hi); } }
#endif

/************ check if a value lies within a closed interval *********/
#ifndef IN_BOUNDS
#define IN_BOUNDS(x,lo,hi) ((x)>=(lo)&&(x)<=(hi))
#endif

/************ check if a 2D point lies within a 2D box ***************/
#ifndef PT_IN_BOX
#define PT_IN_BOX(x,y,lo_x,hi_x,lo_y,hi_y) \
( IN_BOUNDS(x,lo_x,hi_x) && IN_BOUNDS(y,lo_y,hi_y) )
#endif

	/** BEGIN old glui_internal_control.h contents */

//#ifndef GLUI_INTERNAL_CONTROL_H
//#define GLUI_INTERNAL_CONTROL_H

/**2019

	Merging these headers so precompiled header can 
	be used to speed up builds for development work.
*/
//FIRST: Include shorter GLUI_INTERNAL_H string
/* Here's some utility routines */
//#include "glui_internal.h" //"glui_internal.h"
/* This is the main GLUI external header */
#include "GL/glui.hpp" //"GL/glui.hpp"
	
//REMOVING GLU DEPENDENCY
//(Demonstrate GLUI doesn't touch GLU internally.)
#define __glu_h__
#define __GLU_H__

#if defined(GLUI_NOGLUT)

	//EXPERIMENTAL
	//GLUI needs to figure out what are the fewest
	//GLUT procedures it needs and allow user code
	//to provide alternative procedures it can use.
	#include <GL/gl.h>

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

#ifdef _MSC_VER
//"forcing value to bool"
//Using == to produce a bool is good, but only if done with consistency.
#pragma warning(disable:4800)

#define NOMINMAX
#include <windows.h> //glui_internal.h is the PCH file.
#else
#ifndef GLUI_NOGLX
#define GLUI_GLX
#include <GL/glx.h>
//Xlib/X crazily pollute the global namespace.
//#include <X11/Xlib.h>
//#include <X11/Xatom.h>
#endif
#endif

//REMINDER: MSVC vsnprintf requires this inclusion order. Don't know why.
#include <stdarg.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

//#endif /* GLUI_INTERNAL_CONTROL_H  */

#endif /* GLUI_INTERNAL_H */

