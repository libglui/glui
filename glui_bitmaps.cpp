/****************************************************************************
  
  GLUI User Interface Toolkit
  ---------------------------

     glui_bitmaps.cpp


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  This program is freely distributable without licensing fees and is
  provided without guarantee or warrantee expressed or implied. This
  program is -not- in the public domain.

*****************************************************************************/

#include "glui.h"
#include "stdinc.h"

#include <cassert>

/************ Image Bitmap arrays **********/

extern int glui_img_checkbox_0[];
extern int glui_img_checkbox_1[];
extern int glui_img_radiobutton_0[];
extern int glui_img_radiobutton_1[];
extern int glui_img_uparrow[];
extern int glui_img_downarrow[];
extern int glui_img_leftarrow[];
extern int glui_img_rightarrow[];
extern int glui_img_spinup_0[];
extern int glui_img_spinup_1[];
extern int glui_img_spindown_0[];
extern int glui_img_spindown_1[];
extern int glui_img_checkbox_0_dis[];
extern int glui_img_checkbox_1_dis[];
extern int glui_img_radiobutton_0_dis[];
extern int glui_img_radiobutton_1_dis[];
extern int glui_img_spinup_dis[];
extern int glui_img_spindown_dis[];
extern int glui_img_listbox_up[];
extern int glui_img_listbox_down[];
extern int glui_img_listbox_up_dis[];

int *bitmap_arrays[] = {
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


/************************************ GLUI_Bitmap::load_from_array() ********/

GLUI_Bitmap::GLUI_Bitmap() 
:   pixels(NULL),
    w(0), 
    h(0)
{
}

GLUI_Bitmap::~GLUI_Bitmap()
{
	if (pixels)
	{
		free(pixels);
		pixels = NULL;
	}
}

void 
GLUI_Bitmap::init(int *array)
{
	w = 0;
	h = 0;
	pixels = NULL;

	assert(array);
	if (!array)
		return;

	w = array[0];
	h = array[1];

	pixels = (unsigned char *) malloc(w*h*3);

	assert(pixels);

	for (int i = 0; i<w*h*3; i++)
		pixels[i] = (unsigned char) array[i+2];
}


/*********************************** GLUI_StdBitmaps::draw() *****************/

GLUI_StdBitmaps::GLUI_StdBitmaps() 
{
    for (int i=0; i<GLUI_STDBITMAP_NUM_ITEMS; i++) 
        bitmaps[i].init(bitmap_arrays[i]);
}

GLUI_StdBitmaps::~GLUI_StdBitmaps()
{
}

int 
GLUI_StdBitmaps::width(int i) const
{
	assert(i>=0 && i<GLUI_STDBITMAP_NUM_ITEMS);
	return bitmaps[i].w;
}

int
GLUI_StdBitmaps::height(int i) const
{
	assert(i>=0 && i<GLUI_STDBITMAP_NUM_ITEMS);
	return bitmaps[i].h;
}

void 
GLUI_StdBitmaps::draw(int i, int x, int y) const
{
	assert(i>=0 && i<GLUI_STDBITMAP_NUM_ITEMS);

	if (bitmaps[i].pixels != NULL ) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glRasterPos2f(0.5f+x, 0.5f+y+bitmaps[i].h);
		glDrawPixels( 
			bitmaps[i].w, bitmaps[i].h,
			GL_RGB, GL_UNSIGNED_BYTE, bitmaps[i].pixels); 
	}
}

