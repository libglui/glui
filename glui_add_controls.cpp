/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_add_controls.cpp - Routines for adding controls to a GLUI window

Note: these routines are all deprecated.  Keeping them all here
prevents the linker from dragging in all the .o files, even for controls
that aren't used.

          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    https://github.com/libglui/glui
  Issues: https://github.com/libglui/glui/issues

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
#include "glui_internal.h"


/*********************************** GLUI:: add_checkbox() ************/

GLUI_Checkbox   *GLUI:: add_checkbox( const GLUI_String &name, int *value_ptr,
                                      GLUI_CB callback )
{
  return add_checkbox_to_panel( main_panel,
				name, value_ptr, callback );
}


/*********************************** GLUI:: add_checkbox_to_panel() **********/

GLUI_Checkbox   *GLUI::add_checkbox_to_panel( GLUI_Panel *panel,
					      const GLUI_String &name, int *value_ptr,
					      GLUI_CB callback )
{
  return new GLUI_Checkbox( panel, name, value_ptr, callback );
}

/********************************************* GLUI::add_panel() *************/

GLUI_Panel     *GLUI::add_panel( const GLUI_String &name, int type )
{
  return add_panel_to_panel( main_panel, name, type );
}


/**************************************** GLUI::add_panel_to_panel() *********/

GLUI_Panel *GLUI::add_panel_to_panel( GLUI_Panel *parent_panel,
                                          const GLUI_String &name, int type )
{
  return new GLUI_Panel( parent_panel, name, type );
}


/***************************** GLUI::add_radiogroup() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup( int *value_ptr, GLUI_CB callback)
{
  return add_radiogroup_to_panel( main_panel, value_ptr, callback );
}


/***************************** GLUI::add_radiogroup_to_panel() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup_to_panel(
  GLUI_Panel *panel, int *value_ptr,
  GLUI_CB callback
  )
{
  return new GLUI_RadioGroup( panel, value_ptr, callback );
}


/***************************** GLUI::add_radiobutton_to_group() *************/

GLUI_RadioButton *GLUI::add_radiobutton_to_group(  GLUI_RadioGroup *group,
                                                   const GLUI_String &name )
{
  return new GLUI_RadioButton( group, name );
}


/********************************** GLUI::add_statictext() ************/

GLUI_StaticText  *GLUI::add_statictext( const GLUI_String &name )
{
  return add_statictext_to_panel( main_panel, name );
}


/******************************* GLUI::add_statictext_to_panel() **********/

GLUI_StaticText *GLUI::add_statictext_to_panel( GLUI_Panel *panel,
                                                const GLUI_String &name )
{
  return new GLUI_StaticText( panel, name );
}


/***************************************** GLUI:: add_button() ************/

GLUI_Button   *GLUI:: add_button( const GLUI_String &name, GLUI_CB callback )
{
  return add_button_to_panel( main_panel,
                              name, callback );
}

/*********************************** GLUI:: add_button_to_panel() **********/

GLUI_Button   *GLUI::add_button_to_panel( GLUI_Panel *panel, const GLUI_String &name, GLUI_CB callback )
{
  return new GLUI_Button( panel, name, callback );
}

/********************************** GLUI::add_separator() ************/

void  GLUI::add_separator()
{
  add_separator_to_panel( main_panel );
}


/******************************* GLUI::add_separator_to_panel() **********/

void      GLUI::add_separator_to_panel( GLUI_Panel *panel )
{
  new GLUI_Separator( panel );
}


/********************************** GLUI::add_edittext() ************/

GLUI_EditText  *GLUI::add_edittext( const GLUI_String &name,
				    int data_type, void *data,
				    GLUI_CB callback)
{
  return add_edittext_to_panel( main_panel, name, data_type, data,
                                callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_EditText  *GLUI::add_edittext_to_panel( GLUI_Panel *panel,
                                             const GLUI_String &name,
                                             int data_type, void *data,
                                             GLUI_CB callback)
{
  return new GLUI_EditText( panel, name, data_type, data, callback );
}

/********************************** GLUI::add_edittext() ************/

GLUI_EditText  *GLUI::add_edittext( const GLUI_String &name,
                                    GLUI_String & data,
                                    GLUI_CB callback)
{
  return add_edittext_to_panel( main_panel, name, data, callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_EditText*
GLUI::add_edittext_to_panel( GLUI_Panel *panel, const GLUI_String &name,
                             GLUI_String& data,
                             GLUI_CB callback)
{
  return new GLUI_EditText( panel, name, GLUI_EDITTEXT_STRING, &data, callback );
}

/********************************** GLUI::add_spinner() ************/

GLUI_Spinner  *GLUI::add_spinner( const GLUI_String &name,
				  int data_type, void *data,
				  GLUI_CB callback)
{
  return add_spinner_to_panel( main_panel, name, data_type, data, callback );
}


/******************************* GLUI::add_spinner_to_panel() **********/

GLUI_Spinner  *GLUI::add_spinner_to_panel(
  GLUI_Panel *panel, const GLUI_String &name,
  int data_type, void *data,
  GLUI_CB callback
)
{
  return new GLUI_Spinner( panel, name, data_type, data, callback );
}


/********************************** GLUI::add_column() ************/

void   GLUI::add_column( int draw_bar )
{
  add_column_to_panel( main_panel, draw_bar );
}


/******************************* GLUI::add_column_to_panel() **********/

void   GLUI::add_column_to_panel( GLUI_Panel *panel, int draw_bar )
{
  new GLUI_Column( panel, draw_bar );
}


/*********************************** GLUI:: add_listbox() ************/

GLUI_Listbox   *GLUI:: add_listbox( const GLUI_String &name, int *value_ptr, GLUI_CB callback)
{
  return add_listbox_to_panel( main_panel,
                               name, value_ptr, callback );
}


/*********************************** GLUI:: add_listbox_to_panel() **********/

GLUI_Listbox   *GLUI::add_listbox_to_panel( GLUI_Panel *panel,
                                            const GLUI_String &name, int *value_ptr,
                                            GLUI_CB callback )
{
  return new GLUI_Listbox( panel, name, value_ptr, callback );
}


/*********************************** GLUI:: add_rotation() ************/

GLUI_Rotation   *GLUI:: add_rotation( const GLUI_String &name, float *value_ptr, GLUI_CB callback )
{
  return add_rotation_to_panel( main_panel, name, value_ptr, callback );
}


/*********************************** GLUI:: add_rotation_to_panel() **********/

GLUI_Rotation *GLUI::add_rotation_to_panel( GLUI_Panel *panel,
                                            const GLUI_String &name, float *value_ptr,
                                            GLUI_CB callback )
{
  return new GLUI_Rotation( panel, name, value_ptr, callback );
}


/*********************************** GLUI:: add_translation() ************/

GLUI_Translation *GLUI:: add_translation( const GLUI_String &name, int trans_type,
                                          float *value_ptr, 
                                          GLUI_CB callback )
{
  return add_translation_to_panel( main_panel,name,trans_type,
                                   value_ptr, callback );
}


/*********************************** GLUI:: add_translation_to_panel() **********/

GLUI_Translation *GLUI::add_translation_to_panel(
  GLUI_Panel *panel, const GLUI_String &name,
  int trans_type, float *value_ptr,
  GLUI_CB callback
  )
{
  return new GLUI_Translation(panel, name, trans_type, value_ptr, callback);
}


/********************************** GLUI::add_rollout() **************/

GLUI_Rollout   *GLUI::add_rollout( const GLUI_String &name, int open, int type)
{
  return add_rollout_to_panel( main_panel, name, open, type);
}


/****************************** GLUI::add_rollout_to_panel() *********/

GLUI_Rollout *GLUI::add_rollout_to_panel(GLUI_Panel *panel, const GLUI_String &name,
                                         int open, int type)
{
  return new GLUI_Rollout( panel, name, open, type );
}



