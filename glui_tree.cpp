/****************************************************************************
  
  GLUI User Interface Toolkit
  ---------------------------

     glui_panel.cpp - GLUI_Panel control class


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  This program is freely distributable without licensing fees and is
  provided without guarantee or warrantee expressed or implied. This
  program is -not- in the public domain.

*****************************************************************************/

#include "glui.h"
#include "stdinc.h"


/****************************** GLUI_Tree::GLUI_Tree() **********/
GLUI_Tree::GLUI_Tree(GLUI_Node *parent, const char *name, 
                     int open, int inset)
{
  common_init();
  GLUI_StaticText *inset_label;
  GLUI_Column     *col;

  this->set_name( name );
  this->user_id    = -1;
        
  if ( NOT open ) {
    this->is_open = false;
    this->h = GLUI_DEFAULT_CONTROL_HEIGHT + 7;
  }

  parent->add_control( this );
  inset_label = new GLUI_StaticText(this,"");
  inset_label->set_w(inset);
  col = new GLUI_Column(this,true);
  this->set_column(col);
  this->set_alignment(GLUI_ALIGN_LEFT);
}


/****************************** GLUI_Tree::open() **********/

void    GLUI_Tree::open( void )
{
  int orig;

  if ( NOT glui )
    return;

  if ( is_open )
    return;

  is_open = true;

  orig = set_to_glut_window();

  child_head = collapsed_node.child_head;
  child_tail = collapsed_node.child_tail;

  collapsed_node.child_head = NULL;
  collapsed_node.child_tail = NULL;

  if ( child_head != NULL ) {
    ((GLUI_Control*) child_head)->unhide_internal( true );
  }

  glui->refresh();

  restore_window(orig);
}


/****************************** GLUI_Tree::close() **********/

void    GLUI_Tree::close( void )
{
  int orig;

  if ( NOT glui )
    return;

  if ( NOT is_open )
    return;

  orig = set_to_glut_window();

  if ( child_head != NULL ) {
    ((GLUI_Control*) child_head)->hide_internal( true );
  }

  collapsed_node.child_head = first_child();
  collapsed_node.child_tail = last_child();

  child_head = NULL;
  child_tail = NULL;

  restore_window(orig);

  this->h = GLUI_DEFAULT_CONTROL_HEIGHT + 7;

  is_open = false;

  glui->refresh();
}


/**************************** GLUI_Tree::mouse_down_handler() **********/


int   GLUI_Tree::mouse_down_handler( int local_x, int local_y )
{
  if ( local_y - y_abs > 18 ) {
    initially_inside = currently_inside = false;
    return false;
  }

  currently_inside = true;
  initially_inside = true;

  draw_pressed();

  return false;
}


/**************************** GLUI_Tree::mouse_down_handler() **********/

int   GLUI_Tree::mouse_up_handler( int local_x, int local_y, bool inside )
{
  draw_unpressed();

  if ( currently_inside ) {    
    if ( is_open )
      close();
    else
      open();
  }

  initially_inside = false;

  return false;
}


/********************************* GLUI_Tree::draw() ***********/

void   GLUI_Tree::draw( int x, int y )
{
  int orig, left, right, top, bottom, delta_x;

  if ( NOT can_draw() )
    return;

  orig = set_to_glut_window();
    
  left   = 5;
  right  = w-left;
  top    = 3;
  bottom = 3+16;
  delta_x = 0;

  glui->draw_raised_box( left, top, 16, 16 );

  if ( glui )
    glColor3ub(glui->bkgd_color.r,glui->bkgd_color.g,glui->bkgd_color.b);
  glDisable( GL_CULL_FACE );
  glBegin( GL_QUADS );
  glVertex2i( left+17, top+1 );      glVertex2i( right-1, top+1 );
  glVertex2i( right-1, bottom-1 );  glVertex2i( left+17, bottom-1 );
  glEnd();

  if (format & GLUI_TREEPANEL_DISPLAY_HIERARCHY) {
    delta_x = string_width( level_name ) + char_width(' ');
    glColor3f( lred, lgreen, lblue);    /* The hierarchy is drawn in bold */
    glRasterPos2i(left + 25, top + 11);
    draw_string(level_name);
    glRasterPos2i(left + 24, top + 11);
    draw_string(level_name);
  }

  draw_name( delta_x+left+24, top+11 );

  if ( active )
    draw_active_box( left+22, delta_x+left+string_width( name )+32,
		     top, bottom-2 );


  /**   Draw '+' or '-'  **/

  glBegin( GL_LINES );
  if ( is_open ) {
    if ( enabled )      
      if (is_current) 
	glColor3f( 0, 0, 1 ); 
      else 
	glColor3f( 0.0, 0.0, 0.0 );
    else 
	glColor3f( 0.5, 0.5, 0.5 );
    glVertex2i(left+4,(top+bottom)/2);  glVertex2i(left+13,(top+bottom)/2);

    glColor3f( 1.0, 1.0, 1.0 );
    glVertex2i(left+4,1+(top+bottom)/2);glVertex2i(left+13,1+(top+bottom)/2);
  }
  else
  {
    glColor3f( 1.0, 1.0, 1.0 );
    glVertex2i(left+9,top+3);                          glVertex2i(left+9,bottom-4);
    glVertex2i(left+4,(top+bottom)/2);        glVertex2i(left+13,(top+bottom)/2);

    if ( enabled )
      if (is_current) 
	glColor3f( 0, 0, 1 ); 
      else       
	glColor3f( 0.0, 0.0, 0.0 );
    else 
      glColor3f( 0.5, 0.5, 0.5 );
    glVertex2i(left+4,-1+(top+bottom)/2);
    glVertex2i(left+13,-1+(top+bottom)/2);
    glVertex2i(left+8,top+3);
    glVertex2i(left+8,bottom-4);
  }
  glEnd();

  glLineWidth( 1.0 );

  restore_window(orig);
}


/***************************** GLUI_Tree::update_size() **********/

void   GLUI_Tree::update_size( void )
{
  int text_size = 0, delta_x = 0;

  if ( NOT glui )
    return;

  text_size = string_width(name);

  if (format & GLUI_TREEPANEL_DISPLAY_HIERARCHY) {
    delta_x = string_width( level_name );
  }

  if ( w < text_size + 36 + delta_x)
    w = text_size + 36 + delta_x;


}


/**************************** GLUI_Tree::draw_pressed() ***********/

void   GLUI_Tree::draw_pressed( void )
{
  int state, orig;
  int left, right, top, bottom;

  left   = 5;
  right  = w-left;
  top    = 3;
  bottom = 3+16;

  if ( NOT can_draw() )
    return;

  orig  = set_to_glut_window();
  state = glui->set_front_draw_buffer();
  
  glColor3f( 0.0, 0.0, 0.0 );
  glPushMatrix();
  translate_to_origin();

  glBegin( GL_LINE_LOOP );
  glVertex2i( left, top );         glVertex2i( right, top );
  glVertex2i( right, bottom );     glVertex2i( left,bottom );
  glEnd();

  glBegin( GL_LINE_LOOP );
  glVertex2i( left+1, top+1 );         glVertex2i( right-1, top+1 );
  glVertex2i( right-1, bottom-1 );     glVertex2i( left+1,bottom-1 );
  glEnd();

  glPopMatrix();

  glui->restore_draw_buffer(state);
  restore_window(orig);

}


/**************************** GLUI_Tree::draw_unpressed() ***********/

void   GLUI_Tree::draw_unpressed( void )
{
  if ( NOT can_draw() )
    return;

  translate_and_draw_front();
}


/**************************** GLUI_Tree::mouse_held_down_handler() ****/

int  GLUI_Tree::mouse_held_down_handler( 
                       int local_x, int local_y, 
                       bool new_inside )
{
  if ( NOT initially_inside )
    return false;

  if ( local_y - y_abs> 18 )
    new_inside = false;

  if ( NOT new_inside AND currently_inside ) {
    draw_unpressed();
  } 
  else if ( new_inside AND !currently_inside ) {
    draw_pressed();
  }

  currently_inside = new_inside;
  
  return false;
}
