/**************************************************************************
  
  quaternion.h

  A quaternion class

  ---------------------------------------------------------------------

  Feb 1998, Paul Rademacher (rademach@cs.unc.edu)  
  Oct 2003, Nigel Stewart - GLUI Code Cleaning
                
**************************************************************************/

#ifndef GLUI_QUATERNION_H
#define GLUI_QUATERNION_H

#include "algebra3.h"

#include <cstdio>

/* this line defines a new type: pointer to a function which returns a */
/* float and takes as argument a float */
typedef float (*V_FCT_PTR)(float);

/****************************************************************
 *                    Quaternion                                *
 ****************************************************************/

class quat
{
  /*protected: */
public:

  vec3  v;  /* vector component */
  float s;  /* scalar component */

  /*public: */
  
  /* Constructors */

  quat();
  quat(const float x, const float y, const float z, const float w);
  quat(const vec3 &v, const float s); 
  quat(const float   s, const vec3 &v);
  quat(const float  *d);     /* copy from four-element float array  */
  quat(const double *f);     /* copy from four-element double array */
  quat(const quat   &q);     /* copy from other quat                */

  /* Assignment operators */

  quat  &operator  = (const quat &v);      /* assignment of a quat            */
  quat  &operator += (const quat &v);      /* incrementation by a quat        */
  quat  &operator -= (const quat &v);      /* decrementation by a quat        */
  quat  &operator *= (const float d);      /* multiplication by a constant    */
  quat  &operator /= (const float d);      /* division by a constant          */
  
  /* special functions */
  
  float  length() const;                   /* length of a quat                */
  float  length2() const;                  /* squared length of a quat        */
  quat  &normalize();                      /* normalize a quat                */
  quat  &apply(V_FCT_PTR fct);             /* apply a func. to each component */
  vec3   xform(const vec3 &v );            /* q*v*q-1                         */
  mat4   to_mat4() const;
  void   set_angle(const float f);         /* set rot angle (degrees)         */
  void   scale_angle(const float f);       /* scale rot angle (degrees)       */
  float  get_angle() const;                /* set rot angle (degrees)         */
  vec3   get_axis()  const;                /* get axis                        */

  void   print( FILE *file, const char *name ) const;  /* print quat to a file            */

        float &operator [] (const int i);       /* indexing                        */
  const float &operator [] (const int i) const; /* indexing                        */

  void   set(const float x, const float y, const float z);   /* set quat                        */
  void   set(const vec3 &v, const float s);                  /* set quat                        */

  /* friends */

  friend quat operator - (const quat &v);                   /* -q1            */
  friend quat operator + (const quat &a, const quat &b);    /* q1 + q2        */
  friend quat operator - (const quat &a, const quat &b);    /* q1 - q2        */
  friend quat operator * (const quat &a, const float d);    /* q1 * 3.0       */
  friend quat operator * (const float d, const quat &a);    /* 3.0 * q1       */
  friend quat operator * (const quat &a, const quat &b);    /* q1 * q2        */
  friend quat operator / (const quat &a, const float d);    /* q1 / 3.0       */
  friend int operator == (const quat &a, const quat &b);    /* q1 == q2 ?     */
  friend int operator != (const quat &a, const quat &b);    /* q1 != q2 ?     */
  friend void swap(quat &a, quat &b);                       /* swap q1  &q2   */
  /*friend quat min(const quat &a, const quat &b);          -- min(q1, q2)    */
  /*friend quat max(const quat &a, const quat &b);          -- max(q1, q2)    */
  friend quat prod(const quat &a, const quat &b);      /* term by term mult   */
}; 

/* Utility functions */

quat quat_identity();        /* Returns quaternion identity element */
quat quat_slerp(const quat &from, const quat &to, const float t);

#endif
