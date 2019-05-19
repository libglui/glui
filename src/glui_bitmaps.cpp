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

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

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

#include "../include/glui_internal.h" //"GL/glui.h" //PCH

namespace GLUI_Library
{//-.
//<-'

/************ Image Bitmap arrays **********/

extern unsigned char glui_img_checkbox_0[];
extern unsigned char glui_img_checkbox_1[];
extern unsigned char glui_img_radiobutton_0[];
extern unsigned char glui_img_radiobutton_1[];
extern unsigned char glui_img_uparrow[];
extern unsigned char glui_img_downarrow[];
extern unsigned char glui_img_leftarrow[];
extern unsigned char glui_img_rightarrow[];
extern unsigned char glui_img_spinup_0[];
extern unsigned char glui_img_spinup_1[];
extern unsigned char glui_img_spindown_0[];
extern unsigned char glui_img_spindown_1[];
extern unsigned char glui_img_checkbox_0_dis[];
extern unsigned char glui_img_checkbox_1_dis[];
extern unsigned char glui_img_radiobutton_0_dis[];
extern unsigned char glui_img_radiobutton_1_dis[];
extern unsigned char glui_img_spinup_dis[];
extern unsigned char glui_img_spindown_dis[];
extern unsigned char glui_img_listbox_up[];
extern unsigned char glui_img_listbox_down[];
extern unsigned char glui_img_listbox_up_dis[];

// These must be in the same order as the UI_STDBITMAP enums from glui.h!
static unsigned char *glui_bitmaps[] = 
{
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
StdBitmaps &U1::std_bitmaps = *(StdBitmaps*)/*&*/glui_bitmaps;

int Bitmap::_num_items()
{
	return _NUM_ITEMS; 
}

void Bitmap::draw(int x, int y)const
{
	//2019: Clip bitmap at edges.
	//glRasterPos2f(0.5f+x,0.5f+y+height);	
	glRasterPos2i(0,0);
	//OpenGL is ridiculous. Using glBitmap gets around the
	//error condition when the raster position lies beyond
	//the viewport, which otherwise clips the entire image.
	//glBitmap(0,0,0,0,x+0.5f,-y-height-0.5f,0);
	//I assume this is offsetting the transform stack, but
	//I'm uncertain. If so, negating will prevent off-by-1.
	glBitmap(0,0,0,0,x-0.5f,-y-height+0.5f,0);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glDrawPixels(width,height,GL_LUMINANCE,GL_UNSIGNED_BYTE,lumens);
}

//---.
}//<-'
