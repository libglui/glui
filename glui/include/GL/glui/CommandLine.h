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

  class GLUI_API GLUI_CommandLine : public GLUI_EditText
  {
  public:
    typedef GLUI_EditText Super;

    enum { HIST_SIZE = 100 };

    std::vector<GLUI_String> hist_list;
    int  curr_hist;
    int  oldest_hist;
    int  newest_hist;
    bool commit_flag;

  public:
    int  key_handler( unsigned char key,int modifiers );
    int  special_handler(	int key,int modifiers );
    void deactivate( void );

    virtual const char *get_history( int command_number ) const
    { return hist_list[command_number - oldest_hist].c_str(); }
    virtual GLUI_String& get_history_str( int command_number )
    { return hist_list[command_number - oldest_hist]; }
    virtual const GLUI_String& get_history_str( int command_number ) const
    { return hist_list[command_number - oldest_hist]; }
    virtual void recall_history( int history_number );
    virtual void scroll_history( int direction );
    virtual void add_to_history( const char *text );
    virtual void reset_history( void );

    void dump( FILE *out, const char *text );


    GLUI_CommandLine( GLUI_Node *parent, const char *name, void *live_var=NULL,
                      int id=-1, CallBack callback=CallBack() );
    GLUI_CommandLine( void ) { common_init(); }
  protected:
    void common_init() {
      hist_list.resize(HIST_SIZE);
      curr_hist = 0;
      oldest_hist = 0;
      newest_hist = 0;
      commit_flag = false;
    }

  };

}
