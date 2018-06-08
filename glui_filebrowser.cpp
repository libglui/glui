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

#include "GL/glui.h"
#include "glui_internal.h"
#include <sys/types.h>

#ifdef __GNUC__
#include <dirent.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <sys/stat.h>

GLUI_FileBrowser::GLUI_FileBrowser( GLUI_Node *parent,
                                    const GLUI_String &name,
                                    int type,
                                    int id,
                                    GLUI_CB cb)
{
  common_init();

  set_name( name );
  user_id    = id;
  int_val    = type;
  callback   = cb;

  parent->add_control( this );
  list = new GLUI_List(this, true, 1);
  list->set_object_callback( GLUI_FileBrowser::dir_list_callback, this );
  list->set_click_type(GLUI_DOUBLE_CLICK);
  this->fbreaddir(this->current_dir.c_str());
}

/****************************** GLUI_FileBrowser::draw() **********/

void GLUI_FileBrowser::dir_list_callback(GLUI_Control *glui_object) {
  GLUI_List *list = dynamic_cast<GLUI_List*>(glui_object);
  if (!list)
    return;
  GLUI_FileBrowser* me = dynamic_cast<GLUI_FileBrowser*>(list->associated_object);
  if (!me)
    return;
  int this_item;
  this_item = list->get_current_item();
  if (this_item > 0) { /* file or directory selected */
    const std::string &selected = list->get_item_ptr( this_item )->text;
    if (selected[0] == '/' || selected[0] == '\\') {
      if (me->allow_change_dir) {
#ifdef __GNUC__
        int result = chdir(selected.c_str()+1);
        assert(result==0);
#endif
#ifdef _WIN32
        SetCurrentDirectory(selected.c_str()+1);
#endif
        me->fbreaddir(".");
      }
    } else {
      me->file = selected;
      me->execute_callback();
    }
  }
}



void GLUI_FileBrowser::fbreaddir(const GLUI_String &d) {
  GLUI_String item;
  int i = 0;

	if (d.empty())
    return;

#ifdef _WIN32

  WIN32_FIND_DATA FN;
  HANDLE hFind;
  //char search_arg[MAX_PATH], new_file_path[MAX_PATH];
  //sprintf(search_arg, "%s\\*.*", path_name);

  hFind = FindFirstFile("*.*", &FN);
  if (list) {
    list->delete_all();
    if (hFind != INVALID_HANDLE_VALUE) {
      do {
        int len = strlen(FN.cFileName);
        if (FN.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          item = '\\';
          item += FN.cFileName;
        } else {
          item = FN.cFileName;
        }
        list->add_item(i,item.c_str());
        i++;
      } while (FindNextFile(hFind, &FN) != 0);

      if (GetLastError() == ERROR_NO_MORE_FILES)
        FindClose(hFind);
      else
        perror("fbreaddir");
    }
  }

#elif defined(__GNUC__)

  DIR *dir;
  struct dirent *dirp;
  struct stat dr;

  if (list) {
    list->delete_all();
    if ((dir = opendir(d.c_str())) == NULL)
      perror("fbreaddir:");
    else {
      while ((dirp = readdir(dir)) != NULL)   /* open directory     */
      {
        if (!lstat(dirp->d_name,&dr) && S_ISDIR(dr.st_mode)) /* dir is directory   */
          item = dirp->d_name + GLUI_String("/");
        else
          item = dirp->d_name;

        list->add_item(i,item.c_str());
        i++;
      }
      closedir(dir);
    }
  }
#endif
}

/* Private functions to add and delete child items of local_list */

int GLUI_FileBrowser::add_list_item(GLUI_List_Item *local_list, int id, const char *text)
{
  GLUI_List_Item *new_node = new GLUI_List_Item;

//  printf(" Enter GLUI_FileBrowser::add_item\n");
//  printf("  id = %i,  text = :%s:\n",id,text);
//  printf(" new new_node\n");
//  print_list_all(new_node);

  new_node->text = text;
  new_node->id   = id;
//  printf(" new_node before addition\n");
//  print_list_all(new_node);
//  printf(" local_list before addition\n");
//  print_list_all(local_list);

  new_node->link_this_to_parent_last( local_list );

//  printf(" new_node after addition\n");
//  print_list_all(new_node);
//  printf(" local_list after addition\n");
//  print_list_all(local_list);
//  abort();

  return true;
}

int delete_list_item(GLUI_List_Item *list_item)
{
  printf(" Enter GLUI_File_Browser::delete_list_item\n");
  printf(" ERROR: Function not implemented\n");
  return false;
}

int GLUI_FileBrowser::delete_list_all(GLUI_List_Item *local_list)
{
  GLUI_List_Item *item;
  printf(" Enter GLUI_File_Browser::delete_all\n");

  item = (GLUI_List_Item *)local_list->first_child();
  while( item ) {
    item->unlink();
    delete item;
    item = (GLUI_List_Item *) local_list->first_child();
  }
  return true;
}

int GLUI_FileBrowser::copy_list(GLUI_List_Item p1, GLUI_List_Item p2)
{
  GLUI_List_Item *item;
  printf(" Enter GLUI_File_Browser::delete_all\n");

  item = (GLUI_List_Item *)local_list->first_child();
  while( item ) {
    item->unlink();
    delete item;
    item = (GLUI_List_Item *) local_list->first_child();
  }
  return true;
}

int GLUI_FileBrowser::print_list_all(GLUI_List_Item *local_list)
{
  GLUI_List_Item *item;
  printf(" Enter GLUI_FileBrowser::print_list_all.  local_list= = %p\n",
                                        local_list);

  if(!local_list){
    printf(" local_list not valid\n");
    return false;
  }
  printf(" local_list = %p\n",local_list);
  printf(" local_list = %p %p :  %p %p : %p %p : %i\n", local_list,
         local_list->parent(),  local_list->prev(),local_list->next(),
         local_list-> first_child(), local_list->last_child(),
         local_list->id) ;

  item = (GLUI_List_Item *) local_list-> first_child();
  while(item){
    printf("   item = %p %p : %p %p : %p %p : %2i :%s:\n", item,
         item->parent(),item->prev(),item->next(),
         item-> first_child(), item->last_child(),
         item->id, item->text.c_str()) ;
    item = (GLUI_List_Item *) item->next();
  }
  printf("\n");
  return true;
}

void ProcessFiles(const GLUI_String &path_name)
{
}

void GLUI_FileBrowser::set_w(int w)
{
  if (list) list->set_w(w);
}

void GLUI_FileBrowser::set_h(int h)
{
  if (list) list->set_h(h);
}
