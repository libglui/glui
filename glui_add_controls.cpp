/****************************************************************************

  GLUI User Interface Toolkit (LGPL)
  ---------------------------

     glui_add_controls.cpp - Routines for adding controls to a GLUI window

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

#include "glui.h"
#include "glui_internal.h"


/*********************************** GLUI:: add_checkbox() ************/

GLUI_Checkbox   *GLUI:: add_checkbox( const char *name, int *value_ptr,
                                      int id, GLUI_CB callback )
{
  return add_checkbox_to_panel( main_panel,
				name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_checkbox_to_panel() **********/

GLUI_Checkbox   *GLUI::add_checkbox_to_panel( GLUI_Panel *panel,
					      const char *name, int *value_ptr,
					      int id, 
					      GLUI_CB callback )
{
  return new GLUI_Checkbox( panel, name, value_ptr, id, callback );
}


/************************************ GLUI_Node::add_control() **************/

int GLUI_Node::add_control( GLUI_Control *child )
{
  GLUI_Control *parent_control;

  /*** Collapsible nodes have to be handled differently, b/c the first and 
    last children are swapped in and out  ***/
  parent_control = ((GLUI_Control*)this);
  if ( parent_control->collapsible == true ) {
    if ( NOT parent_control->is_open ) {
      /** Swap in the original first and last children **/
      parent_control->child_head  = parent_control->collapsed_node.child_head;
      parent_control->child_tail  = parent_control->collapsed_node.child_tail;

      /*** Link this control ***/
      child->link_this_to_parent_last( parent_control );

      /** Swap the children back out ***/
      parent_control->collapsed_node.child_head = parent_control->child_head;
      parent_control->collapsed_node.child_tail = parent_control->child_tail;
      parent_control->child_head = NULL;
      parent_control->child_tail = NULL;
    }
    else {
      child->link_this_to_parent_last( parent_control );
    }
  }
  else {
    child->link_this_to_parent_last( parent_control );
  }
  child->glui = (GLUI*) parent_control->glui;
  child->update_size();
  child->enabled = parent_control->enabled;
  child->glui->refresh();

  /** Now set the 'hidden' var based on the parent **/
  if ( parent_control->hidden OR 
       (parent_control->collapsible AND NOT parent_control->is_open ) )
  {
    child->hidden = true;
  }

  return true;
}

/************************************ GLUI_Main::add_control() **************/
 
int    GLUI_Main::add_control( GLUI_Node *parent, GLUI_Control *control )
{
  GLUI_Control *parent_control;

  /*** Collapsible nodes have to be handled differently, b/c the first and 
    last children are swapped in and out  ***/
  parent_control = ((GLUI_Control*)parent);
  if ( parent_control->collapsible == true ) {
    if ( NOT parent_control->is_open ) {
      /** Swap in the original first and last children **/
      parent_control->child_head  = parent_control->collapsed_node.child_head;
      parent_control->child_tail  = parent_control->collapsed_node.child_tail;

      /*** Link this control ***/
      control->link_this_to_parent_last( parent );

      /** Swap the children back out ***/
      parent_control->collapsed_node.child_head = parent_control->child_head;
      parent_control->collapsed_node.child_tail = parent_control->child_tail;
      parent_control->child_head = NULL;
      parent_control->child_tail = NULL;
    }
    else {
      control->link_this_to_parent_last( parent );
    }
  }
  else {
    control->link_this_to_parent_last( parent );
  }
  control->glui = (GLUI*) this;
  control->update_size();
  control->enabled = ((GLUI_Control*) parent)->enabled;
  control->glui->refresh();

  /** Now set the 'hidden' var based on the parent **/
  if ( parent_control->hidden OR 
       (parent_control->collapsible AND NOT parent_control->is_open ) )
  {
    control->hidden = true;
  }

  return true;
}


/********************************************* GLUI::add_panel() *************/

GLUI_Panel     *GLUI::add_panel( const char *name, int type )
{
  return add_panel_to_panel( main_panel, name, type );
}


/**************************************** GLUI::add_panel_to_panel() *********/

GLUI_Panel *GLUI::add_panel_to_panel( GLUI_Panel *parent_panel,
                                          const char *name, int type )
{
  return new GLUI_Panel( parent_panel, name, type );
}


/***************************** GLUI::add_radiogroup() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup( int *value_ptr,
				       int user_id, GLUI_CB callback)
{
  return add_radiogroup_to_panel( main_panel, value_ptr,
				  user_id, callback );
}


/***************************** GLUI::add_radiogroup_to_panel() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup_to_panel(  
  GLUI_Panel *panel, int *value_ptr,
  int user_id, GLUI_CB callback
  )
{
  return new GLUI_RadioGroup( panel, value_ptr, user_id, callback );
}


/***************************** GLUI::add_radiobutton_to_group() *************/

GLUI_RadioButton *GLUI::add_radiobutton_to_group(  GLUI_RadioGroup *group,
                                                   const char *name )
{
  return new GLUI_RadioButton( group, name );
}


/********************************** GLUI::add_statictext() ************/

GLUI_StaticText  *GLUI::add_statictext( const char *name )
{
  return add_statictext_to_panel( main_panel, name );
}


/******************************* GLUI::add_statictext_to_panel() **********/

GLUI_StaticText *GLUI::add_statictext_to_panel( GLUI_Panel *panel, 
                                                const char *name )
{
  return new GLUI_StaticText( panel, name );
}


/***************************************** GLUI:: add_button() ************/

GLUI_Button   *GLUI:: add_button( const char *name, 
				  int id, GLUI_CB callback )
{
  return add_button_to_panel( main_panel,
                              name, id, callback );
}

/*********************************** GLUI:: add_button_to_panel() **********/

GLUI_Button   *GLUI::add_button_to_panel( GLUI_Panel *panel,
					  const char *name, 
					  int id, 
					  GLUI_CB callback )
{
  return new GLUI_Button( panel, name, id, callback );
}

/********************************** GLUI::add_separator() ************/

void  GLUI::add_separator( void )
{
  add_separator_to_panel( main_panel );
}


/******************************* GLUI::add_separator_to_panel() **********/

void      GLUI::add_separator_to_panel( GLUI_Panel *panel )
{
  new GLUI_Separator( panel );
}


/********************************** GLUI::add_edittext() ************/

GLUI_EditText  *GLUI::add_edittext( const char *name, 
				    int data_type, void *data,
				    int id, GLUI_CB callback)
{
  return add_edittext_to_panel( main_panel, name, data_type, data,
                                id, callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_EditText  *GLUI::add_edittext_to_panel( GLUI_Panel *panel, 
                                             const char *name, 
                                             int data_type, void *data,
                                             int id, GLUI_CB callback)
{
  return new GLUI_EditText( panel, name, data_type, data, id, callback );
}

/********************************** GLUI::add_edittext() ************/

GLUI_EditText  *GLUI::add_edittext( const char *name, 
                                    GLUI_String & data,
                                    int id, GLUI_CB callback)
{
  return add_edittext_to_panel( main_panel, name, data, id, callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_EditText*
GLUI::add_edittext_to_panel( GLUI_Panel *panel, const char *name, 
                             GLUI_String& data,
                             int id, GLUI_CB callback)
{
  return new GLUI_EditText( panel, name, GLUI_EDITTEXT_STRING, &data, id, callback );
}

/********************************** GLUI::add_spinner() ************/

GLUI_Spinner  *GLUI::add_spinner( const char *name, 
				  int data_type, void *data,
				  int id, GLUI_CB callback)
{
  return add_spinner_to_panel( main_panel, name, data_type, data,
			       id, callback );
}


/******************************* GLUI::add_spinner_to_panel() **********/

GLUI_Spinner  *GLUI::add_spinner_to_panel( 
  GLUI_Panel *panel, const char *name, 
  int data_type, void *data,
  int id, GLUI_CB callback
)
{
  return new GLUI_Spinner( panel, name, data_type, data, id, callback );
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

GLUI_Listbox   *GLUI:: add_listbox( const char *name, int *value_ptr,
				    int id, GLUI_CB callback )
{
  return add_listbox_to_panel( main_panel,
                               name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_listbox_to_panel() **********/

GLUI_Listbox   *GLUI::add_listbox_to_panel( GLUI_Panel *panel,
                                            const char *name, int *value_ptr,
                                            int id, 
                                            GLUI_CB callback )
{
  return new GLUI_Listbox( panel, name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_rotation() ************/

GLUI_Rotation   *GLUI:: add_rotation( const char *name, float *value_ptr,
                                      int id, GLUI_CB callback )
{
  return add_rotation_to_panel( main_panel, name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_rotation_to_panel() **********/

GLUI_Rotation *GLUI::add_rotation_to_panel( GLUI_Panel *panel,
                                            const char *name, float *value_ptr,
                                            int id, 
                                            GLUI_CB callback )
{
  return new GLUI_Rotation( panel, name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_translation() ************/

GLUI_Translation *GLUI:: add_translation( const char *name, int trans_type,
                                          float *value_ptr, int id, 
                                          GLUI_CB callback )
{
  return add_translation_to_panel( main_panel,name,trans_type, 
                                   value_ptr, id, callback );
}


/*********************************** GLUI:: add_translation_to_panel() **********/

GLUI_Translation *GLUI::add_translation_to_panel( 
  GLUI_Panel *panel, const char *name, 
  int trans_type, float *value_ptr,
  int id, GLUI_CB callback 
  )
{
  return new GLUI_Translation(panel, name, trans_type, value_ptr, id, callback);
}


/********************************** GLUI::add_rollout() **************/

GLUI_Rollout   *GLUI::add_rollout( const char *name, int open, int type)
{
  return add_rollout_to_panel( main_panel, name, open, type);
}


/****************************** GLUI::add_rollout_to_panel() *********/

GLUI_Rollout *GLUI::add_rollout_to_panel(GLUI_Panel *panel, const char *name,
                                         int open, int type)
{
  return new GLUI_Rollout( panel, name, open, type );
}



