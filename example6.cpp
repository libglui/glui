/****************************************************************************

  A simple GLUT program using the GLUI User Interface Library

  This program sets up a checkbox and a spinner, both with live variables.
  No callbacks are used.

  -----------------------------------------------------------------------
  9/17/04 John Kew - Natural Selection, Inc. (jkew@natural-selection.com)   
  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

****************************************************************************/

#include <string.h>
#include <GL/glut.h>
#include <new.h>
//#include <unistd.h>
#include "glui.h"

/** These are the live variables passed into GLUI ***/
int   main_window;
int   dirty;
int num_display  = 0;
int num_format  = 0;
int disable_textbox = 0;
GLUI_StaticText *text;
GLUI_List *hah;
GLUI_TextBox *moo;
GLUI *tree;
GLUI_TreePanel *tp;
GLUI_FileBrowser *fb;
GLUI_EditText * bedit;

char scroll[] = "Scrollbar Widget";

char general[] = "GLUI 2.3 Widgets";

char tree_txt[] = "Tree Widget";

char rollout[] = "Rollout Widget";

char string[] = "GLUI_String";

char list[] = "List Widget";

char text_box[] = "TextBox w/ optional Scrollbar\n\
\n\
A TextBox is a multiline editing box that supports basic navigation, formatting, and editing capabilities. These are text block selection, tabs, new lines, arrow key navigation, as well as an optional scrollbar. The scrollbar is actually a distinct widget and can exist by itself, or be associated with objects that have a specified callback routine.\n\
\n\
add_textbox, add_textbox_to_panel\n\
Adds a new textbox to a GLUI window, optionally nested within another rollout or panel.\n\
\n\
Usage:\n\
GLUI_TextBox  *add_textbox(char *live_var=NULL,\n\
\tint scroll = false,\n\
\tint id=-1,\n\
\tGLUI_Update_CB callback=NULL );\n\
GLUI_TextBox  *add_textbox_to_panel( GLUI_Panel *panel, \n\
\tchar *live_var=NULL,\n\
\tint scroll = false,\n\
\tint id=-1,\n\
\tGLUI_Update_CB callback=NULL );\n\
\n\
\tpanel\t- Name of panel to place the TextBox in.\n\
\tlive_var\t - Currently unimplemented, this might be used\n\t \tin future iterations.\n\
\tscroll\t - set to \"true\" if you want a scrollbar on the left side\n\
\tid\t - If callback is defined, it will be passed this integer\n\t \twhen the value is modified.\n\
\tcallback\t - Pointer to callback function of type (*)(int)\n\
\n\
Notes:\n\
The scrollbar widget uses a different callback scheme to communicate with the textbox. Any object wishing to utilize a scrollbar in some way should implement a static function in the object with the signature:\n\
\n\
(*)(GLUI_Control *, int)\n\
\n\
Where the first parameter is the object the scrollbar is associated with and the second is the int_val passed into the object by the scrollbar. Refer to glui_textbox.cpp and glui_scrollbar.cpp for implementation details.\n\
\n\
Wishlist:\n\
* Word Wrap\n\
* Optimized Drawing (Update only lines that change, etc.)\n\
* Multicolour text\n\
* Current word selection\n\
* copy/paste\n\
\n\
";
/*Bugs:\n\
* fewer Mouse Drag calls  with large texts\n\
* Insertion Pt sometimes drawn on wrong line.\n\
* The scrollbar tab does not exactly match mouse position.\n\
* Two tabs cannot be placed next to each other in a string without a space between them.\n\
* Occasional horizontal drawing overruns.\n\
* set_text seems to like being called after all the windows are open for large amounts of text. If you try it with a string longer than 195 characters it seems to trucate it to 195.\n\
\n\
";*/

/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if (!dirty)
    ;//;usleep(1000);
  else if ( glutGetWindow() != main_window ) {
    glutSetWindow(main_window); 
    glutPostRedisplay();
    dirty = 0;
  }
}


/***************************************** myGlutDisplay() *****************/

void control_cb(int control) {
  int item;
  GLUI_String text;
  char file_name[100];
  FILE  *file;
  char c;
  int i;
  int format;
  dirty = 1;
  item = hah->get_current_item();
  
  if (control == 7) {
    i = 0;
    memset(file_name, '\0', 100);
    strncpy(file_name, fb->get_file(), 100);
    file = fopen(file_name,"r"); 
    if (file == NULL) 
      printf("Error opening file\n");
    else {  
      do {
	c = getc(file);
	text[i] = c;
	i++;
      } while (c != EOF);
    fclose(file);
    } 

    moo->set_text(text);
  }


  if ( control == 1 ) {
    tree->hide();
    if (item==0) {
      moo->set_text(general);
    }
    if (item==1) {
      moo->set_text(text_box);
    }
    if (item==2) {
      moo->set_text(scroll);
    }
    if (item==3) {
      moo->set_text(string);
    }
    if (item==4) {
      moo->set_text(rollout);
    }
    if (item==5) {
      tree->show();
      moo->set_text(tree_txt);
    }
    if (item==6) {
      moo->set_text(list);
    }
  }
  if ( control == 2) {
    tp->ab("foo");
  }
  if ( control == 3) {
      tp->db();
  }
  if ( control == 4) {
    tp->fb();
  }
  if ( control == 5) {
      tp->resetToRoot();
  }
  if ( control == 6) {
      tp->descendBranch();
  }
  if ( control == 8) {
      tp->next();
  }
  if ( control == 9) {
      tp->expand_all();
  }
  if ( control == 10) {
      tp->collapse_all();
  }
  if ( control == 11) {
    format = 0; 
    format = GLUI_TREEPANEL_ALTERNATE_COLOR | 
      GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY;
    
    if (num_display)
      format = format | GLUI_TREEPANEL_DISPLAY_HIERARCHY;
    if (num_format)
      format = format | GLUI_TREEPANEL_HIERARCHY_NUMERICDOT;
    else
      format = format | GLUI_TREEPANEL_HIERARCHY_LEVEL_ONLY;
    tp->set_format(format);
    tp->update_all();
  }
  if (control == 12) {
    if (disable_textbox) {
      moo->disable();
    } else {
      moo->enable();
    }
  }
}

//void out_of_memory() {
//  printf("Out of memory!\n\n");
//} 

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
  dirty = 1;
//  set_new_handler(out_of_memory);
  /**************************  cmd_line = cmd_line_glui->add_edittext( "Command:",
					  GLUI_EDITTEXT_TEXT, NULL,
					  CMD_LINE_ID, control_cb )**************/
  /*         Here's the GLUI code         */
  /****************************************/
  GLUI *edit = GLUI_Master.create_glui("Help on GLUI Widgets", 0);
  main_window = edit->get_glut_window_id();
  GLUI_Panel *ep = edit->add_panel("",true);
  edit->add_statictext_to_panel(ep,"Widget Information:");
  hah = edit->add_list_to_panel(ep,NULL,true,1,control_cb);
  hah->add_item(0,"GLUI 2.3");
  hah->add_item(1,"TextBox");
  hah->add_item(2,"Scrollbar");
  hah->add_item(3,"GLUI_String");
  hah->add_item(4,"Rollout");
  hah->add_item(5,"Tree");
  hah->add_item(6,"List");
  edit->add_statictext_to_panel(ep,"Open Text File:");
  fb = edit->add_filebrowser_to_panel(ep, "", false, 7, control_cb);
  fb->set_h(180);
  hah->set_h(180);
  edit->add_column_to_panel(ep,false); 


  moo = edit->add_textbox_to_panel(ep,NULL,true);
  moo->set_text(general);
  moo->set_h(400);
  moo->set_w(410);
  moo->disable();
  disable_textbox=1;
  edit->add_checkbox_to_panel(ep, "Disable text box:",&disable_textbox,12,control_cb);




  tree = GLUI_Master.create_glui("Tree Test", 0);
  ep = tree->add_panel("Tree Controls");
  bedit = tree->add_edittext_to_panel(ep, "New Branch Name:");
  tree->add_checkbox_to_panel(ep, "Display Numbers", &num_display);
  tree->add_statictext_to_panel(ep, "Number format:");
  GLUI_RadioGroup *rg = tree->add_radiogroup_to_panel(ep, &num_format);
  tree->add_radiobutton_to_group(rg, "Level Only");
  tree->add_radiobutton_to_group(rg, "Hierarchal");
  tree->add_button_to_panel(ep, "Update Format", 11, control_cb); 
  tree->add_column_to_panel(ep);
  tree->add_button_to_panel(ep, "Add Branch", 2, control_cb); 
  tree->add_button_to_panel(ep, "Del Branch", 3, control_cb);
  tree->add_button_to_panel(ep, "Up Branch", 4, control_cb); 
  tree->add_button_to_panel(ep, "Goto Root", 5, control_cb);
  tree->add_column_to_panel(ep);
  tree->add_button_to_panel(ep, "Descend to Leaf", 6, control_cb); 
  tree->add_button_to_panel(ep, "Next Branch", 8, control_cb); 
  tree->add_button_to_panel(ep, "Expand All", 9, control_cb); 
  tree->add_button_to_panel(ep, "Collapse All", 10, control_cb); 
  tp = tree->add_treepanel("Tree Test");
  tp->set_format(GLUI_TREEPANEL_ALTERNATE_COLOR | 
                 GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY |
                 GLUI_TREEPANEL_DISPLAY_HIERARCHY | 
                 GLUI_TREEPANEL_HIERARCHY_NUMERICDOT);
  tp->set_level_color(1,1,1);
  tp->ab("foo you");
  tree->hide();
 
  edit->set_main_gfx_window(main_window); 
  tree->set_main_gfx_window(main_window); 
  GLUI_Master.set_glutIdleFunc( myGlutIdle );

  glutMainLoop();
  return 0;
}



