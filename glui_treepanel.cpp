#include "glui.h"

/* Adds branch to curr_root */
GLUI_Tree *     GLUI_TreePanel::ab(char *name, GLUI_Tree *root) {
  GLUI_Tree *temp;

  
  if (root != NULL) {
    resetToRoot(root);
  }

  temp = glui->add_tree_to_panel((GLUI_Panel *)curr_root,name);
  initNode(temp);
  formatNode(temp);

  curr_root = temp;
  curr_branch = NULL; /* Currently at leaf */

  if (temp->type == GLUI_CONTROL_TREE)
    ((GLUI_Tree *)temp)->set_current(true);
  //refresh();
  //  glui->disactivate_current_control();
  //glui->activate_control( temp, GLUI_ACTIVATE_TAB );
  return temp;

}

/* Goes up one level, resets curr_root and curr_branch to parents*/
void            GLUI_TreePanel::fb(GLUI_Tree *branch) {
  if (((GLUI_Panel *)branch) == ((GLUI_Panel *)this))
    return;

  if (((GLUI_Panel *)curr_branch) == ((GLUI_Panel *)this)) {
    resetToRoot();
    return;
  }
  if (((GLUI_Panel *)curr_root) == ((GLUI_Panel *)this)) {
    resetToRoot();
    return;
  }

  if (branch != NULL) {

    if (branch->type == GLUI_CONTROL_TREE)
      ((GLUI_Tree *)branch)->set_current(false);

    curr_branch = (GLUI_Tree *)branch->next();
    curr_root = (GLUI_Panel *)branch->parent();

    if (curr_branch == NULL && (curr_root->collapsed_node).first_child() != NULL)
      curr_branch = (GLUI_Tree *)(curr_root->collapsed_node).first_child();

    if (curr_root->type == GLUI_CONTROL_TREE)
      ((GLUI_Tree *)curr_root)->set_current(true);

  } else {
    if (curr_root != NULL) { /* up one parent */

      if (curr_root->type == GLUI_CONTROL_TREE)
	((GLUI_Tree *)curr_root)->set_current(false);

      curr_branch = (GLUI_Tree *) curr_root->next();
      curr_root = (GLUI_Panel *) curr_root->parent();

      if (curr_branch == NULL && (curr_root->collapsed_node).first_child() != NULL)
	curr_branch = (GLUI_Tree *)(curr_root->collapsed_node).first_child();

      if (curr_root->type == GLUI_CONTROL_TREE)
	((GLUI_Tree *)curr_root)->set_current(true);

    }

  }
  //refresh();
}


void            GLUI_TreePanel::refresh() {
  glui->disactivate_current_control();
  glui->activate_control( curr_root, GLUI_ACTIVATE_TAB );

  if (can_draw()) {
    translate_and_draw_front();
  }
}

void            GLUI_TreePanel::initNode(GLUI_Tree *temp) {
  if (temp == NULL)
    return;
  int level = temp->get_level();
  int child_number = 1;

  if (((GLUI_Tree *)temp->parent())->type == GLUI_CONTROL_TREE) {
    level = ((GLUI_Tree *)temp->parent())->get_level() + 1;
    if ((temp->prev()) != NULL && ((GLUI_Control *)temp->prev())->type == GLUI_CONTROL_TREE) {
      child_number = ((GLUI_Tree *)(temp->prev()))->get_child_number() + 1;
    }
  } else if (temp->type == GLUI_CONTROL_TREE && 
             ((GLUI_Tree *) temp->parent())->type == GLUI_CONTROL_TREEPANEL) {
    child_number = ++root_children;
  }
  temp->set_id(uniqueID());     // -1 if unset
  temp->set_level(level);
  temp->set_child_number(child_number);
}

void            GLUI_TreePanel::formatNode(GLUI_Tree *temp) {
  if (temp == NULL)
    return;
  int level = temp->get_level();
  int child_number = temp->get_child_number();
  char level_name[100];
  char full_name[100];

  memset(level_name, '\0', 100);
  memset(full_name, '\0', 100);
  temp->level_name[0] = '\0';

  if (format & GLUI_TREEPANEL_DISPLAY_HIERARCHY) {
    if (format & GLUI_TREEPANEL_HIERARCHY_LEVEL_ONLY) {
      sprintf(level_name, "%d", level);
    }
    if (format & GLUI_TREEPANEL_HIERARCHY_NUMERICDOT) {
      if (((GLUI_Tree *)temp->parent())->type == GLUI_CONTROL_TREE)
	sprintf(level_name, "%s.%d", ((GLUI_Tree *)(temp->parent()))->level_name, child_number);
      else
	sprintf(level_name, "%d", child_number);
    }
  }

  temp->set_level_color(lred, lgreen, lblue);
  temp->set_format(format);
  strncpy((char *)(temp->level_name),level_name,100);

  if (format & GLUI_TREEPANEL_ALTERNATE_COLOR) {
    switch (level%8) {
    case (7): temp->set_color(.5,.5,.5); break;
    case (6): temp->set_color(.3,.5,.5); break;
    case (5): temp->set_color(.5,.3,.5); break;
    case (4): temp->set_color(.3,.3,.5); break;
    case (3): temp->set_color(.5,.5,.3); break;
    case (2): temp->set_color(.3,.5,.3); break;
    case (1): temp->set_color(.5,.3,.3); break;
    default: temp->set_color(.3,.3,.3);
    }
  } else {
    temp->set_color(red,green,blue);
  }

  if (format & GLUI_TREEPANEL_DISABLE_BAR) {
    temp->disable_bar();
  } else {
    if (format & GLUI_TREEPANEL_DISABLE_DEEPEST_BAR) {
      temp->disable_bar();
      if (curr_root->type == GLUI_CONTROL_TREE)
	((GLUI_Tree *)curr_root)->enable_bar();
    } else
      if (format & GLUI_TREEPANEL_CONNECT_CHILDREN_ONLY) {
	temp->disable_bar();
	if ((temp->prev()) != NULL && ((GLUI_Tree *)temp->prev())->type == GLUI_CONTROL_TREE) {
	  ((GLUI_Tree *)temp->prev())->enable_bar();
        }
    }
  }
}

void            GLUI_TreePanel::update_all() {
  printf("GLUI_TreePanel::update_all() doesn't work yet. - JVK\n");
  return;
  GLUI_Panel *saved_root = curr_root;
  GLUI_Tree *saved_branch = curr_branch;
  root_children = 0;
  resetToRoot(this);
  if (curr_branch != NULL && curr_branch->type == GLUI_CONTROL_TREE)
    formatNode((GLUI_Tree *)curr_branch);
  next();
  while (curr_root != NULL && curr_branch != this->first_child()) {
    if (curr_branch != NULL && curr_branch->type == GLUI_CONTROL_TREE) {
      formatNode((GLUI_Tree *)curr_branch);
    }
    next();
  } 
  curr_root = saved_root;
  curr_branch = saved_branch;
}

void            GLUI_TreePanel::expand_all() {
  GLUI_Panel *saved_root = curr_root;
  GLUI_Tree *saved_branch = curr_branch;

  resetToRoot(this);
  if (curr_root->type == GLUI_CONTROL_TREE)
    ((GLUI_Tree*)curr_root)->open();
  next();
  while (curr_root != NULL && curr_branch != this->first_child()) {
    if (curr_root->type == GLUI_CONTROL_TREE)
      ((GLUI_Tree*)curr_root)->open();
    next();
  }

  curr_root = saved_root;
  curr_branch = saved_branch;
}

void            GLUI_TreePanel::collapse_all() {
  GLUI_Panel *saved_root = curr_root;
  GLUI_Tree *saved_branch = curr_branch;

  resetToRoot(this);
  next();
  while (curr_root != NULL && curr_branch != this->first_child()) {
    if (curr_root->type == GLUI_CONTROL_TREE && 
	curr_branch == NULL) { /* we want to close everything leaf-first */
      ((GLUI_Tree*)curr_root)->close();
      /* Rather than simply next(), we need to manually move the
         curr_root because this node has been moved to the 
         collapsed_node list */
      curr_branch = (GLUI_Tree *)curr_root->next();
      curr_root = (GLUI_Panel *)curr_root->parent();
    } else
      next();
  }

  curr_root = saved_root;
  curr_branch = saved_branch;

}

/* Deletes the curr_root */
void            GLUI_TreePanel::db(GLUI_Tree *root) {
  GLUI_Tree  *temp_branch;
  GLUI_Panel *temp_root;

  if (((GLUI_Control *)root) == ((GLUI_Control *)this))
    return;

  if (root != NULL) {
    curr_root = (GLUI_Tree *)root;
    curr_branch = NULL;
  }

  if (curr_root == NULL || ((GLUI_Panel *)curr_root) == ((GLUI_Panel *)this)) {
    resetToRoot();
    return;
  }


  temp_branch = (GLUI_Tree *)curr_root->next();              /* Next branch, if any */
  temp_root   = (GLUI_Panel *)curr_root->parent();      /* new root */
  curr_root->unlink();
  delete curr_root;
  curr_branch = (GLUI_Tree *) temp_branch;
  curr_root   = (GLUI_Panel *) temp_root;
  if (curr_root->type == GLUI_CONTROL_TREE)
    ((GLUI_Tree *)curr_root)->open();

  if ((format & GLUI_TREEPANEL_DISABLE_DEEPEST_BAR) == GLUI_TREEPANEL_DISABLE_DEEPEST_BAR) {
    if (curr_root->type == GLUI_CONTROL_TREE && ((GLUI_Tree *)curr_root->next()) == NULL)
      ((GLUI_Tree *)curr_root)->disable_bar();
  }
  //refresh();
}

/* Finds the very last branch of curr_root, resets vars */
void            GLUI_TreePanel::descendBranch(GLUI_Panel *root) {
  if (root)
    resetToRoot(root);
  else
    resetToRoot(curr_root);
  if (curr_branch != NULL && curr_branch != ((GLUI_Panel *)this)) {
    if (curr_root->type == GLUI_CONTROL_TREE)
      ((GLUI_Tree *)curr_root)->set_current(false);
    descendBranch(curr_branch); 
  } 
}

void            GLUI_TreePanel::next() {
  if (curr_root == NULL)
    resetToRoot(this);

  if (curr_branch == NULL && (curr_root->collapsed_node).first_child() != NULL)
    curr_branch = (GLUI_Tree *)(curr_root->collapsed_node).first_child();


  if (curr_branch != NULL && curr_branch != ((GLUI_Panel *)this)) {     /* Descend into branch */
    if (curr_root->type == GLUI_CONTROL_TREE)
      ((GLUI_Tree *)curr_root)->set_current(false);
    resetToRoot(curr_branch);
  } else if (curr_branch == NULL) {
    fb(NULL);  /* Backup and move on */
  }
}

/* Resets curr_root and curr branch to TreePanel and lastChild */
void            GLUI_TreePanel::resetToRoot(GLUI_Panel *new_root) {
  GLUI_Panel *root = this;
  if (new_root != NULL)
    root = new_root;
  curr_root = root;
  if (curr_root->type == GLUI_CONTROL_TREE)
    ((GLUI_Tree *)curr_root)->set_current(true);
  curr_branch = (GLUI_Tree *)root->first_child();

  /* since Trees are collapsable, we need to check the collapsed nodes
     in case the curr_root is collapsed */
  if (curr_branch == NULL && (root->collapsed_node).first_child() != NULL) {
    curr_branch = (GLUI_Tree *)(root->collapsed_node).first_child();
  }
  while (curr_branch != NULL && curr_branch->type != GLUI_CONTROL_TREE) {
    curr_branch=(GLUI_Tree *)curr_branch->next();
  }
}
