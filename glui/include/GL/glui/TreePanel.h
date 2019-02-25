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
     Manages, maintains, and formats a tree of GLUI_Tree objects.
     These are shown in a heirarchical, collapsible display.
  
     FIXME: There's an infinite loop in the traversal code (OSL 2006/06)
  */
  class GLUI_API GLUI_TreePanel : public GLUI_Panel 
  {
  public:
    GLUI_TreePanel(GLUI_Node *parent, const char *name,
                   bool open=false, int inset=0);

    int max_levels;
    int next_id;
    int format;
    float red;
    float green;
    float blue;
    float lred;
    float lgreen;
    float lblue;
    int root_children;
    /* These variables allow the tree panel to traverse the tree
       using only two function calls. (Well, four, if you count 
       going in reverse */

    GLUI_Tree    *curr_branch; /* Current Branch */
    GLUI_Panel *curr_root;   /* Current Root */

  public:
    void            set_color(float r, float g, float b); 
    void            set_level_color(float r, float g, float b);
    void            set_format(int f) { format = f; }

    /* Adds branch to curr_root */
    GLUI_Tree *     ab(const char *name, GLUI_Tree *root = NULL);
    /* Goes up one level, resets curr_root and curr_branch to parents*/
    void            fb(GLUI_Tree *branch= NULL);
    /* Deletes the curr_branch, goes up one level using fb */
    void            db(GLUI_Tree *branch = NULL);
    /* Finds the very last branch of curr_root, resets vars */
    void            descendBranch(GLUI_Panel *root = NULL);
    /* Resets curr_root and curr branch to TreePanel and lastChild */
    void            resetToRoot(GLUI_Panel *new_root = NULL);
    void            next( void );
    void            refresh( void );
    void            expand_all( void );
    void            collapse_all( void );
    void            update_all( void );
    void            initNode(GLUI_Tree *temp);
    void            formatNode(GLUI_Tree *temp);

  protected:
    int uniqueID( void ) { next_id++; return next_id - 1; }
    void common_init() 
    {
      GLUI_Panel();
      next_id = 0;
      curr_root = this;
      curr_branch = NULL;
      red = .5;
      green = .5;
      blue = .5;
      root_children = 0;
    }
  };
  
}
