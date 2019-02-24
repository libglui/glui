/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui_add_controls.cpp - Routines for adding controls to a GLUI_Context *window

Note: these routines are all deprecated.  Keeping them all here
prevents the linker from dragging in all the .o files, even for controls
that aren't used.

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
#include "glui_internal.h"

namespace glui {
  
  /*********************************** GLUI_Context::add_checkbox() ************/

  GLUI_Checkbox   *GLUI_Context::add_checkbox( const char *name, int *value_ptr,
                                        int id, CallBack callback )
  {
    return add_checkbox_to_panel( main_panel,
                                  name, value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_checkbox_to_panel() **********/

  GLUI_Checkbox   *GLUI_Context::add_checkbox_to_panel( GLUI_Panel *panel,
                                                const char *name, int *value_ptr,
                                                int id,
                                                CallBack callback )
  {
    return new GLUI_Checkbox( panel, name, value_ptr, id, callback );
  }

  /********************************************* GLUI_Context::add_panel() *************/

  GLUI_Panel     *GLUI_Context::add_panel( const char *name, int type )
  {
    return add_panel_to_panel( main_panel, name, type );
  }


  /**************************************** GLUI_Context::add_panel_to_panel() *********/

  GLUI_Panel *GLUI_Context::add_panel_to_panel( GLUI_Panel *parent_panel,
                                        const char *name, int type )
  {
    return new GLUI_Panel( parent_panel, name, type );
  }


  /***************************** GLUI_Context::add_radiogroup() ***************/

  GLUI_RadioGroup *GLUI_Context::add_radiogroup( int *value_ptr,
                                         int user_id, CallBack callback)
  {
    return add_radiogroup_to_panel( main_panel, value_ptr,
                                    user_id, callback );
  }


  /***************************** GLUI_Context::add_radiogroup_to_panel() ***************/

  GLUI_RadioGroup *GLUI_Context::add_radiogroup_to_panel(
                                                 GLUI_Panel *panel, int *value_ptr,
                                                 int user_id, CallBack callback
                                                 )
  {
    return new GLUI_RadioGroup( panel, value_ptr, user_id, callback );
  }


  /***************************** GLUI_Context::add_radiobutton_to_group() *************/

  GLUI_RadioButton *GLUI_Context::add_radiobutton_to_group(  GLUI_RadioGroup *group,
                                                     const char *name )
  {
    return new GLUI_RadioButton( group, name );
  }


  /********************************** GLUI_Context::add_statictext() ************/

  GLUI_StaticText  *GLUI_Context::add_statictext( const char *name )
  {
    return add_statictext_to_panel( main_panel, name );
  }


  /******************************* GLUI_Context::add_statictext_to_panel() **********/

  GLUI_StaticText *GLUI_Context::add_statictext_to_panel( GLUI_Panel *panel,
                                                  const char *name )
  {
    return new GLUI_StaticText( panel, name );
  }


  /***************************************** GLUI_Context::add_button() ************/

  GLUI_Button   *GLUI_Context::add_button( const char *name,
                                    int id, CallBack callback )
  {
    return add_button_to_panel( main_panel,
                                name, id, callback );
  }

  /*********************************** GLUI_Context::add_button_to_panel() **********/

  GLUI_Button   *GLUI_Context::add_button_to_panel( GLUI_Panel *panel,
                                            const char *name,
                                            int id,
                                            CallBack callback )
  {
    return new GLUI_Button( panel, name, id, callback );
  }

  /********************************** GLUI_Context::add_separator() ************/

  void  GLUI_Context::add_separator( void )
  {
    add_separator_to_panel( main_panel );
  }


  /******************************* GLUI_Context::add_separator_to_panel() **********/

  void      GLUI_Context::add_separator_to_panel( GLUI_Panel *panel )
  {
    new GLUI_Separator( panel );
  }


  /********************************** GLUI_Context::add_edittext() ************/

  GLUI_EditText  *GLUI_Context::add_edittext( const char *name,
                                      int data_type, void *data,
                                      int id, CallBack callback)
  {
    return add_edittext_to_panel( main_panel, name, data_type, data,
                                  id, callback );
  }


  /******************************* GLUI_Context::add_edittext_to_panel() **********/

  GLUI_EditText  *GLUI_Context::add_edittext_to_panel( GLUI_Panel *panel,
                                               const char *name,
                                               int data_type, void *data,
                                               int id, CallBack callback)
  {
    return new GLUI_EditText( panel, name, data_type, data, id, callback );
  }

  /********************************** GLUI_Context::add_edittext() ************/

  GLUI_EditText  *GLUI_Context::add_edittext( const char *name,
                                      GLUI_String & data,
                                      int id, CallBack callback)
  {
    return add_edittext_to_panel( main_panel, name, data, id, callback );
  }


  /******************************* GLUI_Context::add_edittext_to_panel() **********/

  GLUI_EditText*
  GLUI_Context::add_edittext_to_panel( GLUI_Panel *panel, const char *name,
                               GLUI_String& data,
                               int id, CallBack callback)
  {
    return new GLUI_EditText( panel, name, GLUI_EDITTEXT_STRING, &data, id, callback );
  }

  /********************************** GLUI_Context::add_spinner() ************/

  GLUI_Spinner  *GLUI_Context::add_spinner( const char *name,
                                    int data_type, void *data,
                                    int id, CallBack callback)
  {
    return add_spinner_to_panel( main_panel, name, data_type, data,
                                 id, callback );
  }


  /******************************* GLUI_Context::add_spinner_to_panel() **********/

  GLUI_Spinner  *GLUI_Context::add_spinner_to_panel(
                                            GLUI_Panel *panel, const char *name,
                                            int data_type, void *data,
                                            int id, CallBack callback
                                            )
  {
    return new GLUI_Spinner( panel, name, data_type, data, id, callback );
  }


  /********************************** GLUI_Context::add_column() ************/

  void   GLUI_Context::add_column( int draw_bar )
  {
    add_column_to_panel( main_panel, draw_bar );
  }


  /******************************* GLUI_Context::add_column_to_panel() **********/

  void   GLUI_Context::add_column_to_panel( GLUI_Panel *panel, int draw_bar )
  {
    new GLUI_Column( panel, draw_bar );
  }


  /*********************************** GLUI_Context::add_listbox() ************/

  GLUI_Listbox   *GLUI_Context::add_listbox( const char *name, int *value_ptr,
                                      int id, CallBack callback )
  {
    return add_listbox_to_panel( main_panel,
                                 name, value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_listbox_to_panel() **********/

  GLUI_Listbox   *GLUI_Context::add_listbox_to_panel( GLUI_Panel *panel,
                                              const char *name, int *value_ptr,
                                              int id,
                                              CallBack callback )
  {
    return new GLUI_Listbox( panel, name, value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_rotation() ************/

  GLUI_Rotation   *GLUI_Context::add_rotation( const char *name, float *value_ptr,
                                        int id, CallBack callback )
  {
    return add_rotation_to_panel( main_panel, name, value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_rotation_to_panel() **********/

  GLUI_Rotation *GLUI_Context::add_rotation_to_panel( GLUI_Panel *panel,
                                              const char *name, float *value_ptr,
                                              int id,
                                              CallBack callback )
  {
    return new GLUI_Rotation( panel, name, value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_translation() ************/

  GLUI_Translation *GLUI_Context::add_translation( const char *name, int trans_type,
                                            float *value_ptr, int id,
                                            CallBack callback )
  {
    return add_translation_to_panel( main_panel,name,trans_type,
                                     value_ptr, id, callback );
  }


  /*********************************** GLUI_Context::add_translation_to_panel() **********/

  GLUI_Translation *GLUI_Context::add_translation_to_panel(
                                                   GLUI_Panel *panel, const char *name,
                                                   int trans_type, float *value_ptr,
                                                   int id, CallBack callback
                                                   )
  {
    return new GLUI_Translation(panel, name, trans_type, value_ptr, id, callback);
  }


  /********************************** GLUI_Context::add_rollout() **************/

  GLUI_Rollout   *GLUI_Context::add_rollout( const char *name, int open, int type)
  {
    return add_rollout_to_panel( main_panel, name, open, type);
  }


  /****************************** GLUI_Context::add_rollout_to_panel() *********/

  GLUI_Rollout *GLUI_Context::add_rollout_to_panel(GLUI_Panel *panel, const char *name,
                                           int open, int type)
  {
    return new GLUI_Rollout( panel, name, open, type );
  }


}


