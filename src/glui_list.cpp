/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_list.cpp - GLUI_List control class


		  --------------------------------------------------

  Copyright (c) 2004 John Kew

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

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

//#include <sys/timeb.h> //2019: See mouse_down_handler notes

namespace GLUI_Library
{//-.
//<-'

extern void glui_list_scrollbar_callback(UI::ScrollBar *sb)
{
	UI::List *my = (UI::List*)sb->associated_object;
	//if(!my->scrollbar) return;
	if(sb!=my->scrollbar){ assert(0); return; }

	my->start_line = sb->get_int_val(); my->redraw();
}

/****************************** GLUI_List::mouse_down_handler() **********/

bool UI::List::_mouse_down_handler(int local_x, int local_y)
{		
	int line;
	if(local_y<0) //2019: spacebar_mouse_click? 
	{
		line = curr_line;

		if(_clicked=!_clicked) //start over counting?
		{
			curr_line = ~curr_line; //dbl
		}
	}
	else
	{		
		_clicked = true; //needed for double spacebar click 

		line = find_line(local_x-x_abs,local_y-y_abs);
		if(line<=-1||line>=num_lines) //if(-1==line)
		{
			//if(ui) ui->deactivate_current_control(); //???
			return true; //return false; //2019
		}
	}
		
	bool dbl = curr_line==line;
	curr_line = line;

	if(dbl&&GLUI.double_click_millis)
	{
		if(!GLUI::get_clicked()) dbl = false;
	}
		
	//2019 addition
	//The constructor took a live-variable. This was
	//copied from the EditText or EditBox class, and
	//not implemented.
	if(ptr_val) 
	{
		Item *item = get_line_ptr(curr_line);
		set_text(item?item->text:NULL);
	}

	//2019 thoughts
	//It seems the callback model of List fires only
	//when clicked, and not if keyboard navigated is
	//used. spacebar_mouse_click was made false, but
	//I think this is more crappy coding copied from
	//the text input control.

	execute_callback();

	//Note this code can be simplified, but there is
	//an issue with relying on timing a double-click.
	//Probably GLUI_Time is a better way to time but
	//it may be best to not time at all. sys/timeb.h
	//doesn't work for X servers running on the same
	//machine.
	if(associated_object)
	if(cb_click_type==SINGLE_CLICKED)
	{
		if(object_cb)
		{
			//object_cb(associated_object,user_id);
			object_cb(this);
		}
	}
	else if(dbl/*line==curr_line*/)
	{
		//if(last_line==curr_line&&_last_click_time&&(ms-_last_click_time)<300) 
		{
			//object_cb(associated_object,user_id);
			object_cb(this);
		}
		//else 
		//{
		//  _last_click_time = ms;
		//  last_line = curr_line;
		//}
	}

	redraw();	

	return false; //return true; //2019
}

/****************************** GLUI_List::draw() **********/

void UI::List::_draw()
{	
	Box_Interface::_draw();
			
	/* Figure out which lines are visible*/
		
	int lh = font.height; //15

	Item *item = first_item();
	for(int line=0;item;line++,item=item->next())	
	if(line>=start_line&&line<start_line+visible_lines)	
	draw_text(item->text,curr_line==line,(line-start_line)*lh);
}

/********************************* GLUI_List::draw_text() ****************/

void UI::List::draw_text(C_String t, bool selected, int y)
{	
	/** Find where to draw the text **/

	y+=y_off_top+UI_BOXINNERMARGINX-2;
	int text_x = x_lr+UI_BOXINNERMARGINX; 

	int box_width = get_box_width();
	
	/** Draw selection area dark **/
	if(enabled&&selected)
	{	
		int lh = font.height+2; //15
		//if(lh%2) lh++; //Odd squares focus rect.
		assert(0==lh%2);

		int text_xx = text_x+box_width-1;

		glColor3f(0,0,0.6f);
		glRecti(text_x,y+3,text_xx,y+lh);

		if(this==GLUI::get_active()
		||scrollbar==GLUI::get_active())
		{
			draw_active_rect(text_x-2,y+1,text_xx+2,y+lh+2);
		}
	}

	int x = text_x; 
	int l = selected&&enabled?255:enabled?0:64;		
	glColor3ub(l,l,l);
	begin_to_draw_string(x+1,y+4);
	for(int i=0;t.str[i];i++)
	{
		x+=char_width(t.str[i]); 

		if(x>=box_width) break;

		glutBitmapCharacter(get_font(),t.str[i]);
	}
}

/********************************* GLUI_List::find_line() **********/

int UI::List::find_line(int x, int y)
{
	y-=y_off_top+UI_BOXINNERMARGINX;

	return start_line+y/font.height; //15
}
 		  
/********************************* GLUI_List::special_handler() **********/

bool UI::List::_special_handler(int key, int modifiers)
{
	if(key==GLUT_KEY_DOWN)
	{
		if(curr_line<num_lines-1)
		{
			curr_line++;
			if(curr_line>=start_line+visible_lines)
			start_line++;			
		}

		_clicked = false;
	}
	else if(key==GLUT_KEY_UP)
	{
		if(curr_line>0)
		{
			curr_line--;
			if(curr_line<start_line)
			start_line--;
		}

		_clicked = false;
	}
	else return true; //signal keycode unprocessed

	//I tried to fix this, but there is a start_line?
	//if(scrollbar) scrollbar->set_int_val(curr_line);
	//if(scrollbar) scrollbar->show_item(curr_line,visible_lines);
	if(scrollbar) scrollbar->set_int_val(start_line);

	redraw(); return false; //return true; //2019
}

/************************************ GLUI_List::update_size() **********/

void UI::List::_update_size()
{
	ScrollBar *sb = scrollbar_init(glui_list_scrollbar_callback);

	Box_Interface::_update_size();

	int mw = get_box_width();
	if(mw<UI_LIST_MIN_TEXT_WIDTH)
	{
		offset_dims(&mw,NULL);		
		if(sb&&!sb->hidden) 
		mw+=UI_SCROLL_ARROW_SIZE;
		w = mw;
		mw = get_box_width();
	}
	if(mw%2) w++; //Odd squares focus rect.

	//NEW: Moving this out of draw routine.
	if(int lh=font.height)
	{
		int ih = h-UI_BOXINNERMARGINX*2;
		inset_dims(NULL,&ih);

		visible_lines = ih/lh;

		//2019: Rounding up to lines count.
		h = visible_lines*lh;
		offset_dims(NULL,&h);

		h+=UI_BOXINNERMARGINX*2;
	}
	
	if(scrollbar)
	{
		scrollbar->set_range(std::max(num_lines-visible_lines,0),0);
	}
}

/************************************ GLUI_List::update_size() **********/

void UI::List::_update_live()
{
	int line = get_line_num(text); if(line==-1) return;

	while(line>start_line+visible_lines) start_line++;

	if(line<start_line) start_line--; curr_line = line;
}

/**************************************** GLUI_Listbox::add_item() **********/

bool UI::List::_add_item(Item *new_node)
{
	if(!new_node) return false;

	new_node->_link_this_to_parent_last(&items_list);

	num_lines++;

	_post_repack(); return true;
}

/************************************** GLUI_Listbox::delete_item() **********/

bool UI::List::_delete_item(Node *ptr)
{
	if(!ptr) return false;

	if(ptr->parent()==&items_list)
	{
		int ln = get_line_num((Item*)ptr); //NEW
		
		//node->_unlink(); delete node;
		ptr->_delete(); 

		num_lines--;		

		if(ln<start_line) start_line--;
		if(ln<curr_line) curr_line--;
	}
	else if(ptr==&items_list)
	{
		_delete_all();
		
		num_lines = curr_line = start_line = 0;
	}
	else return false; 
	
	if(ScrollBar*sb=scrollbar) //NEW
	{
		if(start_line!=sb->int_val)
		sb->set_int_val(start_line);
		sb->set_range(std::max(num_lines-visible_lines,0),0);		
	}

	return true;
}

//---.
}//<-'

