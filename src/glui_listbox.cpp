/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_listbox - GLUI_ListBox control class


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

#include "../include/glui_internal.h" //"glui_internal_control.h" //PCH

namespace GLUI_Library
{//-.
//<-'

/************************************ GLUI_Listbox::mouse_over() **********/

static void glui_listbox_glutCreateMenu_callback(int i)
{
	UI::ListBox *my = 
	dynamic_cast<UI::ListBox*>(GLUI.menu_control);
	if(!my){ assert(0); return; }

	GLUI.set_menu(); //NEW

	if(i!=my->int_val){ my->set_int_val(i); my->execute_callback(); }
}
bool UI::ListBox::_mouse_over(bool state, int local_x, int local_y)
{
	/* printf("x/y:   %d/%d\n",x,y); */

	if(state&&enabled&&box_test(local_x,local_y))
	{
		/****  Build a GLUT menu for this listbox   ***/

		/* printf("%d %d\n",x,y); */

		if(glut_menu_id<=0) //NEW
		{
			glut_menu_id = glutCreateMenu(glui_listbox_glutCreateMenu_callback);

			for(Item*item=first_item();item;item=item->next())
			{
				glutAddMenuEntry(item->text.c_str(),item->id);
			}
		}
		glutSetMenu(glut_menu_id); //NEW
		glutAttachMenu(GLUT_LEFT_BUTTON);

		GLUI::set_menu(this);
	}
	else if(-1!=glut_menu_id)
	{
		/* printf("OUT\n"); */ //???

		/*2019: Retaining menu resource.
		//Seems to confuse Nvidia's GLUT.
		//glutDetachMenu(GLUT_LEFT_BUTTON);
		{
			glutDestroyMenu(glut_menu_id);
			glut_menu_id = -1;			
		}
		//Doesn't work here either... it seems to make a
		//phantom menu as if glutDestroyMenu did nothing.
		//glutDetachMenu(GLUT_LEFT_BUTTON); 
		*/
		glutSetMenu(glut_menu_id);
		glutDetachMenu(GLUT_LEFT_BUTTON); 

		GLUI::set_menu(); //NEW
	}

	return false; //return true; //2019
}
 
/****************************** GLUI_Listbox::special_handler() **********/

bool UI::ListBox::_special_handler(int key, int modifiers)
{	
	Item *node = get_item_ptr(int_val);
	Item *new_node = NULL;

	switch(key)
	{
	case GLUT_KEY_DOWN: new_node = node->next(); break;
	
	case GLUT_KEY_UP: new_node = node->prev(); break;
	
	case GLUT_KEY_HOME: new_node = first_item(); break;
	
	case GLUT_KEY_END: new_node = last_item(); break;

	default: return true; //signal keycode unprocessed
	}

	if(new_node&&new_node!=node)
	{
		set_int_val(node->id); 
		
		execute_callback();
	}

	return false; //return true; //2019
}

/****************************** GLUI_Listbox::draw() **********/

void UI::ListBox::_draw()
{
	bool active = this==GLUI::get_active();

	int l = active?255:enabled?0:64;
	box_type = l==255?BOX_ACTIVE:BOX_WHITE;
	Box_Interface::_draw();
	glColor3ub(l,l,l);
	box_type = BOX;
	
	draw_string(x_lr+5,UI_BOXINNERMARGINX+1,curr_text); 

	int i = enabled?Bitmap::LISTBOX_UP:Bitmap::LISTBOX_UP_DIS;
	std_bitmaps[i]->draw(w-std_bitmaps[i]->width-2,2);
}

/************************* GLUI_Listbox::recalculate_item_width( void ) ***********/

/** Change w and return true if we need to be widened to fit the current items. */
inline bool UI::ListBox::_recalculate_item_width()
{		 
	/* Find the title size */
	x_lr = string_width(name)+UI_XOFF+1; //NEW (May want to put name on top?)

	/* Find the longest item string ***/
	int item_text_size = 0;

	for(Item*item=first_item();item;item=item->next())
	{
		item_text_size = std::max(item_text_size,string_width(item->text));		
	}

	/* Sum up our layout: name, item, and drop-down marker */
	int new_wid = x_lr+std::max(UI_EDITTEXT_MIN_TEXT_WIDTH,item_text_size)+20;
	if(w!=new_wid)
	{
		w = new_wid; return true;/* we gotta be shortened or widened */
	}
	else return false; /* our current width is OK */
}

/************************************ GLUI_Listbox::update_si() **********/

void UI::ListBox::_update_size()
{
	Box_Interface::_update_size();

	_recalculate_item_width();
}

/**************************************** GLUI_Listbox::add_item() **********/

bool UI::ListBox::_add_item(Item *new_node)
{
	if(!new_node) return false;

	Item *head = first_item();
	new_node->_link_this_to_parent_last(&items_list);

	if(!head) /*** First item added ***/
	{
		_last_int_val = ~new_node->id; /** Different than id **/

		do_selection(new_node->id); goto repack;
	}

	if(_recalculate_item_width()) repack:
	{
		_post_repack(); /* ui->refresh(); */
	}
	return true;
}

/************************************** GLUI_Listbox::delete_item() **********/

bool UI::ListBox::_delete_item(Node *ptr)
{
	if(!ptr) return false;

	if(ptr->parent()==&items_list)
	{
		ptr->_delete(); 
	}
	else if(ptr==&items_list)
	{
		_delete_all();
	}
	else return false;

	if(glut_menu_id>0)
	{
		glutDestroyMenu(glut_menu_id);
		glut_menu_id = -1;
	}
		
	if(_recalculate_item_width()) 
	{
		_post_repack();
	}
	
	return true;
}

/************************************ GLUI_Listbox::do_selection() **********/

bool UI::ListBox::do_selection(int value)
{
	/*** Is this item already selected? ***/
	if(_last_int_val==value) return false;
						 
	Item *sel_item = NULL;
	for(Item*item=first_item();item;item=item->next())
	{
		if(item->id==value)
		{
			sel_item = item;
			break;
		}
	}
	
	if(!sel_item) return false;
	
	//2019: This is needed to tell when _update_live
	//is not responding to set_int_value. The reason
	//this is not implemented is _upate_live is just
	//so the caller can process the bool return code.
	_last_int_val = value;

	//Means call did not originate from _update_live.
	if(int_val!=value) set_int_val(value);

	curr_text = sel_item->text;

	/* printf("-> %s\n",curr_text.c_str()); */

	redraw(); return true;
}

/********************************************* GLUI_Listbox::_dump() **********/

#ifdef GLUI_DEBUG_STDOUT
void UI::ListBox::_dump()
{
	/* printf("%p\n",(char*)name); */

	fprintf(_dump_FILE,"ListBox: %s\n",name.c_str());

	for(Item*item=first_item();item;item=item->next())
	{
		fprintf(_dump_FILE,"         %3d : %s\n",item->id,item->text.c_str());
	}
}
#endif

//---.
}//<-'
