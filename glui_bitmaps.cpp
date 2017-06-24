/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_bitmaps.cpp

Draws the hardcoded images listed in glui_bitmap_img_data with OpenGL.

FIXME: upload the images to a texture.  This will allow them to be:
  - Drawn with alpha blending
  - Drawn at random sizes and angles onscreen
  - Drawn much faster than with glDrawPixels

 --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    https://github.com/libglui/glui
  Issues: https://github.com/libglui/glui/issues

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include "GL/glui.h"
#include "glui_internal.h"
#include "glui_bitmap_img_data.h"

#include <cassert>

// These must be in the same order as the GLUI_STDBITMAP enums from glui.h!
const unsigned char * const bitmap_arrays[] = {
  glui_img_checkbox_0,
  glui_img_checkbox_1,
  glui_img_radiobutton_0,
  glui_img_radiobutton_1,
  glui_img_uparrow,
  glui_img_downarrow,
  glui_img_leftarrow,
  glui_img_rightarrow,
  glui_img_spinup_0,
  glui_img_spinup_1,
  glui_img_spindown_0,
  glui_img_spindown_1,
  glui_img_checkbox_0_dis,
  glui_img_checkbox_1_dis,
  glui_img_radiobutton_0_dis,
  glui_img_radiobutton_1_dis,
  glui_img_spinup_dis,
  glui_img_spindown_dis,
  glui_img_listbox_up,
  glui_img_listbox_down,
  glui_img_listbox_up_dis,
};


/************************************ GLUI_Bitmap::GLUI_Bitmap() ********/

GLUI_Bitmap::GLUI_Bitmap()
:   w(0),
    h(0)
{
}

/* Create bitmap from greyscale byte array */
void GLUI_Bitmap::init_grey(const unsigned char *array)
{
  w = size_t(array[0]); 
  h = size_t(array[1]);
  pixels.resize(w*h*3);
  array += 2;
  for(size_t i = 0; i<w*h; i++)
    for (size_t j = 0; j<3; j++) /* copy grey to r,g,b channels */
      pixels[i*3+j] = (unsigned char) array[i];
}

/* Create bitmap from color int array.
  (OSL) This used to be how all GLUI bitmaps were stored, which was horribly
  inefficient--three ints per pixel, or 12 bytes per pixel!
*/
void GLUI_Bitmap::init(const int *array)
{
  w = size_t(array[0]); 
  h = size_t(array[1]);
  pixels.resize(w*h*3);
  array += 2;
  pixels.assign(array, array+w*h*3);
}

/*********************************** GLUI_StdBitmaps::draw() *****************/

GLUI_StdBitmaps::GLUI_StdBitmaps()
{
  for (size_t i=0; i<bitmaps.size(); i++)
    bitmaps[i].init_grey(bitmap_arrays[i]);
}

int GLUI_StdBitmaps::width(size_t i) const
{
  assert(i<bitmaps.size());
  return i<bitmaps.size() ? bitmaps[i].w : 0;
}

int GLUI_StdBitmaps::height(size_t i) const
{
  assert(i<bitmaps.size());
  return i<bitmaps.size() ? bitmaps[i].h : 0;
}

void GLUI_StdBitmaps::draw(size_t i, int x, int y) const
{
  assert(i<bitmaps.size());
  if (i<bitmaps.size() && !bitmaps[i].pixels.empty())
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glRasterPos2f(0.5f+x, 0.5f+y+bitmaps[i].h);
    glDrawPixels(
      bitmaps[i].w, bitmaps[i].h,
      GL_RGB, GL_UNSIGNED_BYTE, bitmaps[i].pixels.data());
  }
}

