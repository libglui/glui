/****************************************************************************
  
  GLUI User Interface Toolkit
  ---------------------------

     glui_window.cpp - GLUI_Button control class

          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  This program is freely distributable without licensing fees and is
  provided without guarantee or warrantee expressed or implied. This
  program is -not- in the public domain.

*****************************************************************************/

#include "glui.h"
#include "stdinc.h"

GLUI_Glut_Window::GLUI_Glut_Window()
:
	glut_display_CB(NULL),
	glut_reshape_CB(NULL),
	glut_keyboard_CB(NULL),
	glut_special_CB(NULL),
	glut_mouse_CB(NULL),
	glut_motion_CB(NULL),
	glut_passive_motion_CB(NULL),
	glut_entry_CB(NULL),
	glut_visibility_CB(NULL),
	glut_window_id(0)
{
}

GLUI_Glut_Window::~GLUI_Glut_Window()
{
}
