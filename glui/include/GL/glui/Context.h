/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui.h - Main (and only) external header for 
        GLUI_Context *User Interface Toolkit

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

#pragma once

namespace glui {

  /**
     The main user-visible interface object to GLUI_Context *.
 
  */
  class GLUI_API GLUI_Context : public GLUI_Main 
  {
  public:
    /** DEPRECATED interface for creating new GLUI_Context *objects */
    int   add_control( GLUI_Control *control ) { return main_panel->add_control(control); }

    void  add_column( int draw_bar = true );
    void  add_column_to_panel( GLUI_Panel *panel, int draw_bar = true );

    void  add_separator( void );
    void  add_separator_to_panel( GLUI_Panel *panel );

    GLUI_RadioGroup 
      *add_radiogroup( int *live_var=NULL,
                       int user_id=-1,CallBack callback=CallBack());

    GLUI_RadioGroup 
      *add_radiogroup_to_panel(  GLUI_Panel *panel,
                                 int *live_var=NULL,
                                 int user_id=-1, CallBack callback=CallBack() );
    GLUI_RadioButton
      *add_radiobutton_to_group(  GLUI_RadioGroup *group,
                                  const char *name );

    GLUI_Listbox *add_listbox( const char *name, int *live_var=NULL,
                               int id=-1, CallBack callback=CallBack()	);
    GLUI_Listbox *add_listbox_to_panel( GLUI_Panel *panel,
                                        const char *name, int *live_var=NULL,
                                        int id=-1, CallBack callback=CallBack());

    GLUI_Rotation *add_rotation( const char *name, float *live_var=NULL,
                                 int id=-1, CallBack callback=CallBack()	);
    GLUI_Rotation *add_rotation_to_panel( GLUI_Panel *panel,
                                          const char *name, float *live_var=NULL,
                                          int id=-1, CallBack callback=CallBack());
  
    GLUI_Translation *add_translation( const char *name,
                                       int trans_type, float *live_var=NULL,
                                       int id=-1, CallBack callback=CallBack()	);
    GLUI_Translation *add_translation_to_panel( 
                                               GLUI_Panel *panel, const char *name, 
                                               int trans_type, float *live_var=NULL,
                                               int id=-1, CallBack callback=CallBack());
  
    GLUI_Checkbox  *add_checkbox( const char *name, 
                                  int *live_var=NULL,
                                  int id=-1, CallBack callback=CallBack());
    GLUI_Checkbox  *add_checkbox_to_panel( GLUI_Panel *panel, const char *name, 
                                           int *live_var=NULL, int id=-1, 
                                           CallBack callback=CallBack());

    GLUI_Button  *add_button( const char *name, int id=-1, 
                              CallBack callback=CallBack());
    GLUI_Button  *add_button_to_panel( GLUI_Panel *panel, const char *name, 
                                       int id=-1, CallBack callback=CallBack() );

    GLUI_StaticText  *add_statictext( const char *name );
    GLUI_StaticText  *add_statictext_to_panel( GLUI_Panel *panel, const char *name );

    GLUI_EditText  *add_edittext( const char *name, 
                                  int data_type=GLUI_EDITTEXT_TEXT,
                                  void*live_var=NULL,
                                  int id=-1, CallBack callback=CallBack()	);
    GLUI_EditText  *add_edittext_to_panel( GLUI_Panel *panel, 
                                           const char *name,
                                           int data_type=GLUI_EDITTEXT_TEXT,
                                           void *live_var=NULL, int id=-1, 
                                           CallBack callback=CallBack() );
    GLUI_EditText  *add_edittext( const char *name, GLUI_String& live_var, 
                                  int id=-1, CallBack callback=CallBack()	);
    GLUI_EditText  *add_edittext_to_panel( GLUI_Panel *panel, const char *name, 
                                           GLUI_String& live_var, int id=-1,
                                           CallBack callback=CallBack() );

    GLUI_Spinner  *add_spinner( const char *name, 
                                int data_type=GLUI_SPINNER_INT,
                                void *live_var=NULL,
                                int id=-1, CallBack callback=CallBack() );
    GLUI_Spinner  *add_spinner_to_panel( GLUI_Panel *panel, 
                                         const char *name,
                                         int data_type=GLUI_SPINNER_INT,
                                         void *live_var=NULL,
                                         int id=-1,
                                         CallBack callback=CallBack() );

    GLUI_Panel     *add_panel( const char *name, int type=GLUI_PANEL_EMBOSSED );
    GLUI_Panel     *add_panel_to_panel( GLUI_Panel *panel, const char *name, 
                                        int type=GLUI_PANEL_EMBOSSED );


    GLUI_Rollout   *add_rollout( const char *name, int open=true,
                                 int type=GLUI_PANEL_EMBOSSED);
    GLUI_Rollout   *add_rollout_to_panel( GLUI_Panel *panel, const char *name, 
                                          int open=true,
                                          int type=GLUI_PANEL_EMBOSSED);


    /** Set the window where our widgets should be displayed. */
    void            set_main_gfx_window( int window_id );
    int             get_glut_window_id( void ) { return glut_window_id; }

    void            enable( void ) { main_panel->enable(); }
    void            disable( void );

    void            sync_live( void );

    void            close( void );

    void            show( void );
    void            hide( void );

    /***** Constructors and desctructors *****/

    int init( const char *name, long flags, int x, int y, int parent_window );
  protected:
    virtual int add_control( GLUI_Node *parent, GLUI_Control *control ) {
      return GLUI_Main::add_control( parent, control );
    }
  };
  
}
