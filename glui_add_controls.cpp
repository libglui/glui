/*

  glui_add_controls.cpp - Routines for adding controls to a GLUI window

  GLUI User Interface Toolkit (LGPL)
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
*/

#include "glui.h"
#include "stdinc.h"


/*********************************** GLUI:: add_checkbox() ************/

GLUI_Checkbox   *GLUI:: add_checkbox( char *name, int *value_ptr,
				      int id, GLUI_Update_CB callback )
{
  return add_checkbox_to_panel( main_panel,
				name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_checkbox_to_panel() **********/

GLUI_Checkbox   *GLUI::add_checkbox_to_panel( GLUI_Panel *panel,
					      char *name, int *value_ptr,
					      int id, 
					      GLUI_Update_CB callback )
{
  GLUI_Checkbox *control;

  control = new GLUI_Checkbox;

  if ( control ) {
    control->set_ptr_val( value_ptr );
    control->user_id    = id;
    control->set_name( name );
    control->callback    = callback;

    add_control( panel, control );

    control->init_live();

    return control;
  }
  else {
    return NULL;
  }
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

GLUI_Panel     *GLUI::add_panel( char *name, int type )
{
  return add_panel_to_panel( main_panel, name, type );
}


/**************************************** GLUI::add_panel_to_panel() *********/

GLUI_Panel     *GLUI::add_panel_to_panel( GLUI_Panel *parent_panel,
					  char *name, int type )
{
  GLUI_Panel *panel;
  
  panel = new GLUI_Panel;

  if ( panel ) {
    panel->set_name( name );
    panel->user_id    = -1;
    panel->int_val    = type;

    add_control( parent_panel, panel );

    return panel;
  }
  else {
    return NULL;
  }  
}


/********************************************* GLUI::add_panel() *************/

GLUI_FileBrowser     *GLUI::add_filebrowser( char *name, int type, int user_id, 
						      GLUI_Update_CB callback)
{
  return add_filebrowser_to_panel( main_panel, name, type, user_id, callback);
}


/**************************************** GLUI::add_panel_to_panel() *********/

GLUI_FileBrowser     *GLUI::add_filebrowser_to_panel( GLUI_Panel *parent_panel,
						      char *name, int type, int user_id, 
						      GLUI_Update_CB callback)
{
  GLUI_FileBrowser *panel;
  
  panel = new GLUI_FileBrowser;

  if ( panel ) {
    panel->set_name( name );
    panel->user_id    = user_id;
    panel->int_val    = type;
    panel->callback   = callback;

    add_control( parent_panel, panel );
    panel->list = add_list_to_panel(panel,
				    NULL,
				    true,
				    1,
				    NULL,
				    panel,
				    (GLUI_InterObject_CB)(GLUI_FileBrowser::dir_list_callback));
    (panel->list)->set_click_type(GLUI_DOUBLE_CLICK);
    panel->fbreaddir(panel->current_dir);
    return panel;
  }
  else {
    return NULL;
  }  
}

/***************************** GLUI::add_radiogroup() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup( int *value_ptr,
				       int user_id, GLUI_Update_CB callback)
{
  return add_radiogroup_to_panel( main_panel, value_ptr,
				  user_id, callback );
}


/***************************** GLUI::add_radiogroup_to_panel() ***************/

GLUI_RadioGroup *GLUI::add_radiogroup_to_panel(  GLUI_Panel *panel,
						 int *value_ptr,
						 int user_id, GLUI_Update_CB callback)
{
  GLUI_RadioGroup *control;
  GLUI_String      buf;

  control = new GLUI_RadioGroup;

  if ( control ) {
    control->set_ptr_val( value_ptr );
    if ( value_ptr ) {
      control->int_val = *value_ptr;  /** Can't call set_int_val(), b/c that 
					function will try to call the 
					callback, etc */
      /** Actually, maybe not **/
      control->last_live_int = *value_ptr;
    }

    control->user_id    = user_id;
    sprintf( buf, "RadioGroup: %p", (void *) control );
    control->set_name( buf );
    control->callback   = callback;

    add_control( panel, control );

    control->init_live();

    return control;
  }
  else {
    return NULL;
  }

}


/***************************** GLUI::add_radiobutton_to_group() *************/

GLUI_RadioButton *GLUI::add_radiobutton_to_group(  GLUI_RadioGroup *group,
						   char *name )
{
  GLUI_RadioButton *control;

  if ( group->type != GLUI_CONTROL_RADIOGROUP ) {
    /*fprintf( stderr, "ERROR: Trying to add radiobutton to non-radiogroup\n" );              */
    /*fflush( stderr );              */
    return NULL;
  }

  control = new GLUI_RadioButton;

  if ( control ) {
    control->set_int_val( 0 );
    /*int_val = 0;              */

    /** A radio button's user id is always its ordinal number (zero-indexed)
      within the group */
    control->user_id    = group->num_buttons;
    group->num_buttons++;   /* Increments radiogroup's button count */
    control->set_name( name );
    control->group = group;

    add_control( group, control );

    /*** Now update button states ***/
    group->set_int_val( group->int_val ); /* This tells the group to
					     reset itself to its
					     current value, thereby
					     updating all its buttons */

    return control;
  }
  else {
    return NULL;
  }

}


/********************************** GLUI::add_statictext() ************/

GLUI_StaticText  *GLUI::add_statictext( char *name )
{
  return add_statictext_to_panel( main_panel, name );
}


/******************************* GLUI::add_statictext_to_panel() **********/

GLUI_StaticText  *GLUI::add_statictext_to_panel( GLUI_Panel *panel, char *name )
{
  GLUI_StaticText *control;

  control = new GLUI_StaticText;

  if ( control ) {
    control->set_name( name );
    add_control( panel, control );

    return control;
  }
  else {
    return NULL;
  }

}


/***************************************** GLUI:: add_button() ************/

GLUI_Button   *GLUI:: add_button( char *name, 
				  int id, GLUI_Update_CB callback )
{
  return add_button_to_panel( main_panel,
			      name, id, callback );
}


/*********************************** GLUI:: add_button_to_panel() **********/

GLUI_Button   *GLUI::add_button_to_panel( GLUI_Panel *panel,
					  char *name, 
					  int id, 
					  GLUI_Update_CB callback )
{
  GLUI_Button *control;
  
  control = new GLUI_Button;
  
  if ( control ) {
    /*    control->set_ptr_val( value_ptr );
	  if ( value_ptr ) {
	  control->set_int_val( *value_ptr );
	  }*/

    control->user_id     = id;
    control->callback    = callback;
    control->set_name( name );
    
    add_control( panel, control );
    
    return control;
  }
  else {
    return NULL;
  }
}



/********************************** GLUI::add_separator() ************/

void  GLUI::add_separator( void )
{
  add_separator_to_panel( main_panel );
}


/******************************* GLUI::add_separator_to_panel() **********/

void      GLUI::add_separator_to_panel( GLUI_Panel *panel )
{
  GLUI_Separator *control = new GLUI_Separator;

  if ( control ) {
    add_control( panel, control );
  }
}


/********************************** GLUI::add_edittext() ************/

GLUI_EditText  *GLUI::add_edittext( char *name, 
				    int data_type, void *data,
				    int id, GLUI_Update_CB callback)
{
  return add_edittext_to_panel( main_panel, name, data_type, data,
				id, callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_EditText  *GLUI::add_edittext_to_panel( GLUI_Panel *panel, char *name, 
		       int data_type, void *data,
		       int id, GLUI_Update_CB callback)
{
  GLUI_EditText *control;

  control = new GLUI_EditText;

  if ( control ) {
    control->set_name( name );
    
    control->data_type   = data_type;
    control->ptr_val     = data;
    control->user_id     = id;
    control->callback    = callback;
    
    if ( data_type == GLUI_EDITTEXT_TEXT ) {
      control->live_type = GLUI_LIVE_TEXT;
    }
    else if ( data_type == GLUI_EDITTEXT_INT ) {
      control->live_type = GLUI_LIVE_INT;
      if ( data == NULL )
	control->set_int_val(control->int_val);   /** Set to some default, in case of no live var **/
    }
    else if ( data_type == GLUI_EDITTEXT_FLOAT ) {
      control->live_type = GLUI_LIVE_FLOAT;
      control->num_periods = 1;
      if ( data == NULL )
	control->set_float_val(control->float_val);   /** Set to some default, in case of no live var **/
    }
    else {
      return NULL;   /* Did not pass in a valid data type */
    }

    add_control( panel, control );

    control->init_live();

    return control;
  }
  return 0;
}

/********************************** GLUI::add_edittext() ************/

GLUI_TextBox  *GLUI::add_textbox(char *data, int scroll,
				 int id, GLUI_Update_CB callback)
{
  return add_textbox_to_panel( main_panel, data, scroll,
			       id, callback );
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_TextBox  *GLUI::add_textbox_to_panel( GLUI_Panel *panel, char *data,
					   int scroll, int id, GLUI_Update_CB callback)
{
  GLUI_TextBox *control;
  GLUI_Panel *tb_panel = panel;

  control = new GLUI_TextBox;


  if ( control ) {
    if (scroll) {
      tb_panel = add_panel_to_panel(panel,"",GLUI_PANEL_NONE);
      tb_panel->x_off = 1;
    }
    control->ptr_val     = data;
    control->user_id     = id;
    control->callback    = callback;
    control->name        = "textbox";
    add_control( tb_panel, control );
    if (scroll) {
      add_column_to_panel(tb_panel, false);
      control->scrollbar = add_scrollbar_to_panel(tb_panel,
						  "scrollbar",
						  GLUI_SPINNER_INT, 
						  NULL, 
						  -1,
						  NULL,
						  (GLUI_Control*) control,
						  (GLUI_InterObject_CB )GLUI_TextBox::scrollbar_callback);
    }
    return control;
  }
  else {
    return NULL;
  }

}


/********************************** GLUI::add_edittext() ************/

GLUI_List  *GLUI::add_list(char *data, int scroll,
			   int id, 
			   GLUI_Update_CB callback, 
			   GLUI_Control *object, 
			   GLUI_InterObject_CB obj_cb)
{
  return add_list_to_panel( main_panel, data, scroll,
			    id, callback , object, obj_cb);
}


/******************************* GLUI::add_edittext_to_panel() **********/

GLUI_List  *GLUI::add_list_to_panel( GLUI_Panel *panel, char *data,
				     int scroll, int id, 
				     GLUI_Update_CB callback,  
				     GLUI_Control *object, 
				     GLUI_InterObject_CB obj_cb)
{
  GLUI_List *control;
  GLUI_Panel *list_panel = panel;

  control = new GLUI_List;

  if ( control ) {
    if (scroll) {
      list_panel = add_panel_to_panel(panel,"",GLUI_PANEL_NONE);
      list_panel->x_off = 1;
    }
    control->ptr_val     = data;
    control->user_id     = id;
    control->callback    = callback;
    control->name        = "list";
    add_control( list_panel, control );
    if (scroll) {
      add_column_to_panel(list_panel, false);
      control->scrollbar = add_scrollbar_to_panel(list_panel,
						  "scrollbar",
						  GLUI_SPINNER_INT, 
						  NULL, 
						  -1,
						  NULL,
						  (GLUI_Control*) control,
						  (GLUI_InterObject_CB )GLUI_List::scrollbar_callback);      

    }
    if (object) {
      control->obj_cb    = obj_cb;
      control->associated_object = object;
    }
    return control;
  }
  else {
    return NULL;
  }

}

/********************************** GLUI::add_spinner() ************/

GLUI_Spinner  *GLUI::add_spinner( char *name, 
				  int data_type, void *data,
				  int id, GLUI_Update_CB callback)
{
  return add_spinner_to_panel( main_panel, name, data_type, data,
			       id, callback );
}


/******************************* GLUI::add_spinner_to_panel() **********/

GLUI_Spinner  *GLUI::
add_spinner_to_panel( GLUI_Panel *panel, char *name, 
		      int data_type, void *data,
		      int id, GLUI_Update_CB callback)
{
  GLUI_Spinner *control;
  int           text_type;

  control = new GLUI_Spinner;
 
  if ( NOT strcmp( name, "Spinner Test" ))
    id=id;


  if ( control ) {
    if ( data_type == GLUI_SPINNER_INT ) {
      text_type = GLUI_EDITTEXT_INT;
      /*      control->live_type = GLUI_LIVE_INT;              */
    }
    else if ( data_type == GLUI_SPINNER_FLOAT ) {
      text_type = GLUI_EDITTEXT_FLOAT;
      /*      control->live_type = GLUI_LIVE_FLOAT;              */
    }
    else {
      return NULL;   /* Did not pass in a valid data type */
    }

    GLUI_EditText *edittext = 
      add_edittext_to_panel( (GLUI_Panel*) control, name, text_type, data,
			     id, callback );

    if ( edittext ) {
      control->set_name( name );
      control->edittext    = edittext;  /* Link the edittext to the spinner */
      /*      control->ptr_val     = data;               */
      control->user_id     = id;
      control->data_type   = data_type;
      control->callback    = callback;
    
      edittext->spinner    = control; /* Link the spinner to the edittext */
            
      add_control( panel, control );
      
      return control;
    }
    else {
      return NULL;
    }
  }
  else {
    return NULL;
  }

}

/********************************** GLUI::add_scrollbar() ************/

GLUI_Scrollbar  *GLUI::add_scrollbar( char *name, 
                  int data_type, void *data,
				      int id, GLUI_Update_CB callback, 
				      GLUI_Control *object,
				  GLUI_InterObject_CB obj_cb)
{
  return add_scrollbar_to_panel( main_panel, name, data_type, data,
				 id, callback, object, obj_cb);
}


/******************************* GLUI::add_scrollbar_to_panel() **********/

GLUI_Scrollbar  *GLUI::add_scrollbar_to_panel( GLUI_Panel *panel, char *name, 
			int data_type, void *data,
			int id, GLUI_Update_CB callback,
			GLUI_Control *object,
			GLUI_InterObject_CB obj_cb)
{
  GLUI_Scrollbar *control;
  int           text_type;

  control = new GLUI_Scrollbar;
 
  if ( control ) {
      control->set_int_limits(0,0,GLUI_LIMIT_CLAMP);
      control->set_int_val(0);
      control->data_type = GLUI_SPINNER_INT;
      control->set_name(name);
      control->user_id = id;
      control->callback    = callback;
      control->associated_object = object;
      control->object_cb = obj_cb;
      add_control( panel, control );
      return control;
  }
  else {
    return NULL;
  }

}

/********************************** GLUI::add_column() ************/

GLUI_Column *   GLUI::add_column( int draw_bar )
{
  return add_column_to_panel( main_panel, draw_bar );
}


/******************************* GLUI::add_column_to_panel() **********/

GLUI_Column * GLUI::add_column_to_panel( GLUI_Panel *panel, int draw_bar )
{
  GLUI_Column *control = new GLUI_Column;

  if ( control ) {
    control->int_val = draw_bar; /* Whether to draw vertical bar or not */

    add_control( panel, control );
  }
  else {
  }
  return control;
}


/*********************************** GLUI:: add_listbox() ************/

GLUI_Listbox   *GLUI:: add_listbox( char *name, int *value_ptr,
				    int id, GLUI_Update_CB callback )
{
  return add_listbox_to_panel( main_panel,
			       name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_listbox_to_panel() **********/

GLUI_Listbox   *GLUI::add_listbox_to_panel( GLUI_Panel *panel,
					    char *name, int *value_ptr,
					    int id, 
					    GLUI_Update_CB callback )
{
  GLUI_Listbox *control;

  control = new GLUI_Listbox;

  if ( control ) {
    control->set_ptr_val( value_ptr );
    control->user_id    = id;
    control->set_name( name );
    control->callback    = callback;

    add_control( panel, control );

    control->init_live();

    return control;
  }
  else {
    return NULL;
  }
}


/*********************************** GLUI:: add_rotation() ************/

GLUI_Rotation   *GLUI:: add_rotation( char *name, float *value_ptr,
				      int id, GLUI_Update_CB callback )
{
  return add_rotation_to_panel( main_panel,
				name, value_ptr, id, callback );
}


/*********************************** GLUI:: add_rotation_to_panel() **********/

GLUI_Rotation   *GLUI::add_rotation_to_panel( GLUI_Panel *panel,
					      char *name, float *value_ptr,
					      int id, 
					      GLUI_Update_CB callback )
{
  GLUI_Rotation *control;

  control = new GLUI_Rotation;

  if ( control ) {
    control->set_ptr_val( value_ptr );
    control->user_id    = id;
    control->set_name( name );
    control->callback    = callback;
    add_control( panel, control );
    control->init_live();

    /*** Init the live 4x4 matrix.  This is different than the standard
      live variable behavior, since the original value of the 4x4 matrix
      is ignored and reset to Identity  ***/
    if ( value_ptr != NULL ) {
      int i, j, index;
      for( i=0; i<4; i++ ) {
	for( j=0; j<4; j++ ) {
	  index = i*4+j;
	  if ( i==j )
	    value_ptr[index] = 1.0;
	  else
	    value_ptr[index] = 0.0;
	}
      }
    }

    /*init_ball();              */
		
    return control;
  }
  else {
    return NULL;
  }
}


/*********************************** GLUI:: add_translation() ************/

GLUI_Translation *GLUI:: add_translation( char *name, int trans_type,
			   float *value_ptr, int id, GLUI_Update_CB callback )
{
  return add_translation_to_panel( main_panel,name,trans_type, 
				   value_ptr, id, callback );
}


/*********************************** GLUI:: add_translation_to_panel() **********/

GLUI_Translation *GLUI::add_translation_to_panel( 
				   GLUI_Panel *panel, char *name, 
				   int trans_type, float *value_ptr,
				   int id, GLUI_Update_CB callback )
{
  GLUI_Translation *control;

  control = new GLUI_Translation;

  if ( control ) {
    control->set_ptr_val( value_ptr );
    control->user_id    = id;
    control->set_name( name );
    control->callback    = callback;
    add_control( panel, control );
    control->init_live();

    control->trans_type = trans_type;

    if ( trans_type == GLUI_TRANSLATION_XY ) {
      control->float_array_size = 2;
    }
    else if ( trans_type == GLUI_TRANSLATION_X ) {
      control->float_array_size = 1;
    }
    else if ( trans_type == GLUI_TRANSLATION_Y ) {
      control->float_array_size = 1;
    }
    else if ( trans_type == GLUI_TRANSLATION_Z ) {
      control->float_array_size = 1;
    }

    return control;
  }
  else {
    return NULL;
  }
}


/********************************** GLUI::add_rollout() **************/

GLUI_Rollout   *GLUI::add_rollout( char *name, int open, int type)
{
  return add_rollout_to_panel( main_panel, name, open, type);
}


/****************************** GLUI::add_rollout_to_panel() *********/

GLUI_Rollout *GLUI::add_rollout_to_panel(GLUI_Panel *panel,char *name,int open, int type)
{
  GLUI_Rollout     *rollout;
  
  rollout = new GLUI_Rollout;

  if ( rollout ) {
    rollout->set_name( name );
    rollout->user_id    = -1;
    rollout->int_val    = type;
		
    if ( NOT open ) {
      rollout->is_open = false;
      rollout->h = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
    }

    add_control( panel, rollout );

    return rollout;
  }
  else {
    return NULL;
  }  
}


/********************************** GLUI::add_rollout() **************/

GLUI_Tree   *GLUI::add_tree(char *name, int open, int inset)
{
  return add_tree_to_panel( main_panel, name, open, inset);
}


/****************************** GLUI::add_rollout_to_panel() *********/

GLUI_Tree *GLUI::add_tree_to_panel(GLUI_Panel *panel,char *name,int open, int inset)
{
  GLUI_Tree       *tree;
  GLUI_StaticText *inset_label;
  GLUI_Column     *col;
  tree = new GLUI_Tree;

  if ( tree ) {
    tree->set_name( name );
    tree->user_id    = -1;
        
    if ( NOT open ) {
      tree->is_open = false;
      tree->h = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
    }

    add_control( panel, tree );
    inset_label = add_statictext_to_panel(tree,"");
    inset_label->set_w(inset);
    col = add_column_to_panel(tree,true);
    tree->set_column(col);
    tree->set_alignment(GLUI_ALIGN_LEFT);
    return tree;
  }
  else {
    return NULL;
  }  
}

/********************************** GLUI::add_rollout() **************/

GLUI_TreePanel   *GLUI::add_treepanel(char *name, int open, int inset)
{
  return add_treepanel_to_panel( main_panel, name, open, inset);
}


/****************************** GLUI::add_rollout_to_panel() *********/

GLUI_TreePanel *GLUI::add_treepanel_to_panel(GLUI_Panel *panel,char *name,int open, int inset)
{
  GLUI_TreePanel       *tree;
  GLUI_StaticText *inset_label;
  tree = new GLUI_TreePanel;

  if ( tree ) {
    tree->set_name( name );
    tree->user_id    = -1;
        
    if ( NOT open ) {
      tree->is_open = false;
      tree->h = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
    }

    add_control( panel, tree );
    return tree;
  }
  else {
    return NULL;
  }  
}
