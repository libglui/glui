/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui_translation - GLUI_TransferFunction control class


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

#include "GL/glui.h"
#include "GL/glui/TransferFunction.h"
#include "glui_internal.h"
#include "algebra3.h"
#include "glui_internal_control.h"
#include <iostream>
#include <stdio.h>

#ifndef PRINT
# define PRINT(var) std::cout << #var << "=" << var << std::endl;
# define PING std::cout << __FILE__ << "::" << __LINE__ << ": " << __PRETTY_FUNCTION__ << std::endl;
#endif

namespace glui {
  inline float clampf(const float f, const float f0, const float f1)
  {
    return std::max(f0,std::min(f,f1));
  }
  
  /********************** GLUI_TransferFunction::GLUI_TransferFunction() ***/

  GLUI_TransferFunction::GLUI_TransferFunction(GLUI_Node *parent,
                                               const char *name,
                                               float *live_array,
                                               int live_array_count,
                                               int id,
                                               CallBack cb)
  {
    common_init();

    xf_alpha_count = live_array_count;
    if (live_array)
      xf_alpha = live_array;
    else
      xf_alpha = new float[live_array_count];
    
    user_id    = id;
    set_name( name );
    callback    = cb;
    parent->add_control( this );

    for (int i=0;i<live_array_count;i++)
      live_array[i] = i/float(live_array_count-1);

    init_live();
  }
  
  int  GLUI_TransferFunction::iaction_mouse_down_handler( int local_x, int local_y )
  {
    lastMouseX = local_x;
    lastMouseY = local_y;

    // iw - "hack": force even the first click to appear as if it was
    // a drag: this allows a single click to modify the transfer
    // function, and to do so in a way that will properly trigger the
    // callback, redraw, etc.
    mouse_held_down_handler(local_x+x_abs,local_y+y_abs,true);
    
    return 0;
  }
  
  int  GLUI_TransferFunction::iaction_mouse_up_handler( int local_x, int local_y, bool inside )
  {
    lastMouseX = lastMouseY = -1; 
    return false;
  }
  
  int  GLUI_TransferFunction::iaction_mouse_held_down_handler( int local_x, int local_y, bool inside )
  {
    float thisRelX
      = (local_x //- x_abs -1
         - total_border_width)
      / float(w - 2*total_border_width-2);
    float thisRelY = // 1.f - 
      (local_y //- y_abs -1
       - total_border_width)
      / float(h - 2*total_border_width-2-GLUI_STATICTEXT_SIZE-1);

    float lastRelX
      = (lastMouseX// - x_abs -1
         - total_border_width)
      / float(w - 2*total_border_width-2);
    float lastRelY = // 1.f -
      (lastMouseY// - y_abs -1
       - total_border_width)
      / float(h - 2*total_border_width-2-GLUI_STATICTEXT_SIZE-1);
    
    // clamp
    thisRelX = clampf(thisRelX,0.f,1.f);
    thisRelY = clampf(thisRelY,0.f,1.f);
    lastRelX = clampf(lastRelX,0.f,1.f);
    lastRelY = clampf(lastRelY,0.f,1.f);

    thisRelY = 1.f - thisRelY;
    lastRelY = 1.f - lastRelY;

    // printf("LOCAL %f %f --> REL %f %f\n",(float)local_x,(float)local_y,thisRelX,thisRelY);
    if (lastMouseX >= 0) {
      int   loBinX = std::min(int(thisRelX * xf_alpha_count),xf_alpha_count-1);
      float loBinY = thisRelY;
      int   hiBinX = std::min(int(lastRelX * xf_alpha_count),xf_alpha_count-1);
      float hiBinY = lastRelY;
      if (loBinX > hiBinX) {
        std::swap(loBinX,hiBinX);
        std::swap(loBinY,hiBinY);
      }
      // std::cout << "(" << loBinX << "," << loBinY <<") - (" << hiBinX << "," << hiBinY << ")" << std::endl;
      for (int x=loBinX;x<=hiBinX;x++) {
        float y
          = (loBinX==hiBinX)
          ? loBinY
          : (loBinY+(hiBinY-loBinY)*(x-loBinX)/float(hiBinX-loBinX));
        xf_alpha[x] = clampf(y,0.f,1.f);
      }
      output_live(true);
      /** Update the main gfx window? **/
      if (// update_main_gfx && 
          context ) 
        context->post_update_main_gfx();
      // else printf("no context\n");
    }
    
    lastMouseX = local_x;
    lastMouseY = local_y;
    
    return false;
  }

  int  GLUI_TransferFunction::iaction_special_handler( int key, int modifiers )
  {
    return 0;
  }
  
  void GLUI_TransferFunction::iaction_init( void )// { init_ball(); }
  {
  }
  
  void GLUI_TransferFunction::iaction_dump( FILE *output )
  {
  }

  void    GLUI_TransferFunction::iaction_draw_active_area_persp( void )
  {
  }

  /******************** GLUI_TransferFunction::iaction_draw_active_area_ortho() **********/
  void    GLUI_TransferFunction::iaction_draw_active_area_ortho( void )
  {
  }
  
  void    GLUI_TransferFunction::draw( int,int )
  {
    GLUI_DRAWINGSENTINAL_IDIOM
      glLineWidth( 1.0 );
    
    draw_name(w/2-string_width(name)/2,h);
    draw_emboss_box(0,0,1,1);
    draw_emboss_box(outer_border_width,w-1-outer_border_width,
                    outer_border_width,h-1-outer_border_width-1*GLUI_STATICTEXT_SIZE);

    glTranslatef(outer_border_width+inner_border_width,
                 outer_border_width+inner_border_width,
                 0);
    glScalef((w-2*inner_border_width-2*outer_border_width-2),
             (h-2*inner_border_width-2*outer_border_width-2-GLUI_STATICTEXT_SIZE-1),
             1);
    glTranslatef(0.f,1.f,0.f);
    glScalef(1.f,-1.f,1.f);
    
    for (int i=0;i<xf_alpha_count;i++) {
      if (xf_color_array)
        glColor3f(xf_color_array[i*xf_color_channel_count+0],
                  xf_color_array[i*xf_color_channel_count+1],
                  xf_color_array[i*xf_color_channel_count+2]);
      else
        glColor3f(1,i/float(xf_alpha_count),0);
      float x0 = (i+0.f) / xf_alpha_count;
      float x1 = (i+1.f) / xf_alpha_count;
      float y0 = 0.f;
      float y1 = xf_alpha[i];
      glBegin( GL_QUADS );
      glVertex2f( x0,y0 );       glVertex2f( x0,y1 );
      glVertex2f( x1,y1 );       glVertex2f( x1,y0 );
      glEnd();
    }
  }

  /******************************* GLUI_TransferFunction::set_one_val() ****/

  void  GLUI_TransferFunction::set_one_val( float val, int index )
  {
    
    // float_array_val[index] = val;    /* set value in array              */

    // /*** The code below is like output_live, except it only operates on
    //      a single member of the float array (given by 'index') instead of
    //      outputting the entire array   ****/

    // if ( ptr_val == NULL OR NOT live_inited )
    //   return;

    // float *fp = (float*) ptr_val;
    // fp[index]                    = float_array_val[index];
    // last_live_float_array[index] = float_array_val[index];

    // /** Update the main gfx window? **/
    // if (context) {
    //   context->post_update_main_gfx();
    // }
  }
}
