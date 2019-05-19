/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_filebrowser.cpp - GLUI_FileBrowser control class


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

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

#include <sys/types.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__GNUC__)
#include <dirent.h>
#include <unistd.h>
#endif

#include <sys/stat.h>

namespace GLUI_Library
{//-.
//<-'

void UI::FileBrowser::_update_size()
{
	Panel::_update_size();	
	assert(!is_container); if(list)
	{
		if(this!=list->associated_object)
		{
			list = NULL; return;
		}
		int dh = h;
		assert(this==list->parent());
		int dw = w-list->has_parent()->w;
		inset_dims(&dw,&dh);		
		list->w+=dw; list->h = dh;
	}
}

static void glui_filebrower_dir_list_callback(UI::List *list)
{					  
	UI::FileBrowser *my = 
	dynamic_cast<UI::FileBrowser*>(list->associated_object);
	if(!my){ assert(0); return; }

	int this_item = list->get_current_item();
	if(this_item>0)
	{ 
		/* file or directory selected */
		//const char *selected = list->get_item_ptr(this_item)->text.c_str();
		const char *selected = list->get_line_ptr(this_item)->text.c_str();
		if(selected[0]=='/'/*||selected[0]=='\\'*/)
		{
			//2019: I'm changing this so that something happens when clicking
			//on directories, since there is no recourse.
			/* if(my->click_chdir) my->chdir(selected+1); */
			my->lsdir(selected+1,my->click_chdir);
		}
		else 
		{
			my->file = selected;
			my->execute_callback();
		}
	}
} 

/****************************** GLUI_FileBrowser::GLUI_FileBrowser() **********/

UI::List *UI::FileBrowser::_list_init(List *l)
{
	if(list&&l!=list) list->_delete();
	if(l) //l = new List(this,true,1);
	{	
		//_scrollbar_init creates a panel to contain the list and its
		//scrollbar... in this case it could probably use this as its
		//panel?
		//assert(this==l->parent()); 

		l->id = 1;
		l->set_object_callback(glui_filebrower_dir_list_callback,this);
		l->set_click_type(DOUBLE_CLICKED);

		//Getting margins may be unnecessary.
		Panel::_update_size();
		//scrollbar_int may be necessary.
		l->update_size();		
		offset_dims(&(w=l->w),&(h=l->h));
	}
	return list = l;
}

/****************************** GLUI_FileBrowser::fbreaddir() **********/

bool UI::FileBrowser::lsdir(C_String d, bool chdir)
{
	if(chdir)
	{
		if(d.str)
		{
			#ifdef _WIN32		
			if(!SetCurrentDirectoryA(d.str))
			#elif defined(__GNUC__)		
			if(::chdir(d.str))
			#endif
			return false;
		}
		d = "";
	}

	//2019: changing behavior to load current directory.
	if(!d.str)
	{
		d = current_dir; /*return;*/ 

		if(!*d.str) d = ".";
	}

	List::Item *i,*di = NULL;

#if defined(_WIN32)
   
	WIN32_FIND_DATAA FN;
	HANDLE hFind;
	//char search_arg[MAX_PATH], new_file_path[MAX_PATH];
	//sprintf(search_arg, "%s\\*.*", path_name);

	hFind = FindFirstFileA("*.*",&FN);
	
	if(hFind==INVALID_HANDLE_VALUE) 
	goto error;
	
	if(list) 
	{
		list->delete_all(); do 
		{
			i = new List::Item;

			int len = strlen(FN.cFileName);
			if(FN.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) 
			{
				i->text = '/'; //i->text = '\\';

				list->insert_item(i,di); di = i;
			}
			else list->add_item(i);
			
			i->text.append(FN.cFileName);

		}while(FindNextFileA(hFind,&FN));
	}

	if(GetLastError()==ERROR_NO_MORE_FILES)
	{
		FindClose(hFind);
	}
	else error:
	{
		perror("fbreaddir"); return false;
	}

#elif defined(__GNUC__)
	
	DIR *dir = opendir(d.str); if(!dir)
	{
		perror("fbreaddir"); return false;
	}
	else if(list)
	{
		list->delete_all(); 

		struct dirent *dirp; 
		while(dirp=readdir(dir)) /* open directory */
		{
			i = new List::Item;

			struct stat dr; /* dir is directory   */
			if(!lstat(dirp->d_name,&dr)&&S_ISDIR(dr.st_mode)) 
			{
				i->text = '/';

				list->insert_item(i,di); di = i;
			}
			else list->add_item(i);
			
			i->text.append(dirp->d_name);
		}
	}	
	closedir(dir);

#endif	 

	if(list)
	{
		//Historically FileBrowser assumed id matched get_current_item.
		i = list->first_item();
		for(int id=0;i;i=i->next()) i->id = id++; //back-compat
	}

	//2019: Why not update this member? And clear file, as in directory.
	current_dir = d.str; file.clear();
	current_chdir = chdir;
	if(chdir?chdir_cb:lsdir_cb) execute_callback();

	return true;
}
 
//---.
}//<-'
