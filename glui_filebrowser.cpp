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
#include <fnmatch.h>
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
  int this_item, l;
  this_item = list->get_current_item();
//  printf(" dir_list_callback: this item = %i\n",this_item);
  if (this_item > 0) { /* file or directory selected */
    const std::string &selected = list->get_item_ptr( this_item )->text;
//  printf(" dir_list_callback: selected  = %s\n",selected.c_str());
    l = selected.size();
    if (selected[l-1] == '/' || selected[0] == '/' || selected[0] == '\\') {
      if (me->allow_change_dir) {
#ifdef __GNUC__
        int result = chdir(selected.c_str());
        assert(result==0);
#endif
#ifdef _WIN32
        SetCurrentDirectory(selected.c_str()+1);  // REMOVE +1 ????
#endif
        me->fbreaddir(".");
      }
    } else {
      me->file = selected;
      me->execute_callback();
    }
  }
}

int fb_list_compare(const void *p1, const void *p2)
{
  GLUI_List_Item *lp1 = (GLUI_List_Item *) *(void **)p1,
                 *lp2 = (GLUI_List_Item *) *(void **)p2 ;
  int  l1 = lp1->text.length()+1;
  int  l2 = lp2->text.length()+1;
  int  iret;
  char *str1, *str2 ;

  str1 = (char *)malloc(l1*sizeof(char));
  str2 = (char *)malloc(l2*sizeof(char));

  strncpy(str1, lp1->text.c_str(),l1);
  strncpy(str2, lp2->text.c_str(),l2);
/*
  printf(" fb_list_compare : *lp1, *lp2 = %p %p\n",(void *)lp1,(void *)lp2);
  printf(" fb_list_compare : l1, str1 = %i :%s:\n", l1, str1);
  printf(" fb_list_compare : l2, str2 = %i :%s:\n", l2, str2);
  printf(" fb_list_compare : str1[l1-2] = %c\n", str1[l1-2]);
  printf(" fb_list_compare : str2[l2-2] = %c\n", str2[l2-2]);
*/
  if(strcmp(str1,"./")==0) iret = true  ;
  else if(strcmp(str2,"./")==0) iret = false ;

  else if(strcmp(str1,"../")==0) iret = true;
  else if(strcmp(str2,"../")==0) iret = false;

  else if((str1[l1-2]=='/') && (str2[l2-2]!='/')) iret = true;
  else if((str1[l1-2]!='/') && (str2[l2-2]=='/')) iret = false;

  else if(strcmp(str1,str2)<0) iret = true;
  else iret = false ;

  free(str1);
  free(str2);
  return !iret ;
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
  int iret;

  if (list) {
    list->delete_all();
    if ((dir = opendir(d.c_str())) == NULL)
      perror("fbreaddir:");
    else {
      while ((dirp = readdir(dir)) != NULL)   /* open directory     */
      {
        iret = true ;
        if (!lstat(dirp->d_name,&dr) && S_ISDIR(dr.st_mode)){
          item = dirp->d_name + GLUI_String("/");  /* Directory */
        }else{
          item = dirp->d_name;                     /* File */
          if(list_filter.length()>0){
            if(fnmatch(list_filter.c_str(),item.c_str(),0)!=0) iret = false ;
          }
        }
        if(iret){
          list->add_item(i,item.c_str());
          i++;
        }
      }
      closedir(dir);

      int iprint = 0;
      if(iprint)printf(" List created.\n");
      GLUI_List_Item  *p_item;
      p_item = (GLUI_List_Item *)list->items_list.first_child();
      while(p_item){
        if(iprint)printf(" AA item : %p %p %p %p :%i :%s:  \n",p_item,
                    p_item->parent(), p_item->next(), p_item->prev(),
                    p_item->id, p_item->text.c_str() );
        p_item = (GLUI_List_Item *)p_item->next();
      }
      if(iprint)printf("\n");

/*  Order List */
      {
        int n_list = i;
        int n ;
        GLUI_List_Item **p_list, *p_item;

/* Setup pointers to text and sort */
        if(iprint)printf(" Allocate pointer array\n");
        p_list = (GLUI_List_Item **)malloc(n_list*sizeof(*p_list));

        if(iprint)printf(" Initialise pointer array\n");
        for(i=0;i<n_list;i++){ p_list[i] = list->get_item_ptr(i); }
        for(i=0;i<n_list;i++){
           p_item = p_list[i];
           if(iprint)printf(" BB item : %p %p %p %p :%i :%s:  \n",p_item,
                    p_item->parent(), p_item->next(), p_item->prev(),
                    p_item->id, p_item->text.c_str() );
        }
        if(iprint)printf("\n");

        if(iprint)printf(" Sort\n");
        qsort(p_list,n_list,sizeof(p_list[0]),fb_list_compare);
        for(i=0;i<n_list;i++){
            p_item = p_list[i];
            if(iprint)printf(" CC item : %p %p %p %p :%i :%s:  \n",p_item,
                    p_item->parent(), p_item->next(), p_item->prev(),
                    p_item->id, p_item->text.c_str() );
        }
        if(iprint)printf("\n");

        if(iprint)printf(" Unlist\n");
/* Unlink list */
        for(i=0;i<n_list;i++){ p_list[i]->unlink(); }
        for(i=0;i<n_list;i++){
            p_item = p_list[i];
            if(iprint)printf(" DD item : %p %p %p %p :%i :%s:  \n",p_item,
                    p_item->parent(), p_item->next(), p_item->prev(),
                    p_item->id, p_item->text.c_str() );
        }
        if(iprint)printf("\n");

        n = n_list;
        if(n>5) n=5;

        if(iprint)printf(" Link\n");
/* Relink in new order to parent and siblings */
        p_list[0]->link_this_to_parent_first(&list->items_list);
//        p_list[0]->id = 0;
        for(i=1;i<n_list;i++){
          p_list[i]->link_this_to_sibling_next(p_list[i-1]);
//          p_list[i]->id = i;
        }
        for(i=0;i<n_list;i++){
            p_item = p_list[i];
            if(iprint)printf(" EE item : %p %p %p %p :%i :%s:  \n",p_item,
                    p_item->parent(), p_item->next(), p_item->prev(),
                    p_item->id, p_item->text.c_str() );
        }
        if(iprint)printf("\n");

        p_item = (GLUI_List_Item *)list->items_list.first_child();
        while(p_item){
            if(iprint)printf(" FF item : %p %p %p %p :%i :%s:  \n",p_item,
                        p_item->parent(), p_item->next(), p_item->prev(),
                        p_item->id, p_item->text.c_str() );
            p_item = (GLUI_List_Item *)p_item->next();
        }
        if(iprint)printf("\n");

        p_item = (GLUI_List_Item *)list->items_list.first_child();
        i = 0;
        while(p_item){
            p_item->id = i;
            if(iprint)printf(" GG item : %p %p %p %p :%i :%s:  \n",p_item,
                        p_item->parent(), p_item->next(), p_item->prev(),
                        p_item->id, p_item->text.c_str() );
            p_item = (GLUI_List_Item *)p_item->next();
            i++;
        }
        if(iprint)printf("\n");

        if(iprint)printf(" Free\n");
        free(p_list);
        if(iprint)printf(" Exit\n");
      }
/* End Order List */


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

void GLUI_FileBrowser::set_filter(const char *string)
{
    list_filter = string ;
    while(list_filter.length()>0 &&
         (list_filter.at(0)==' ' || list_filter.at(0)=='\t'))list_filter.erase(0,1);
}

const char * GLUI_FileBrowser::filter_cstr()
{
    return list_filter.c_str();
}


