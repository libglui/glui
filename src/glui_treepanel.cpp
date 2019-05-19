/*
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
 */

#include "../include/glui_internal.h" //PCH

#if defined(GLUI_BUILD_TREE_CONTROLS) || defined(GLUI_BUILD_EXAMPLES)

namespace GLUI_Library
{//-.
//<-'

/****************************** GLUI_TreePanel::ab() *********/

/* Adds branch to curr_root */
UI::Experiments::Tree*
UI::Experiments::TreePanel::add_tree(Tree *o)
{
	//if(root) go_to_root();
	//Tree *o = new Tree(curr_root,name);
	if(!o->parent()) o->set_parent(curr_root);

	//init_node(o); /**** GLUI_TreePanel::initNode() ****/
	{
		int level = o->get_level();
		int child_number = 1;

		//Tree *ptree = dynamic_cast<Tree*>(o->parent());
		Tree *ptree = dynamic_cast<Tree*>(curr_root);
		if(ptree)
		{
			level = 1+ptree->get_level();
			Tree *prevTree = dynamic_cast<Tree*>(o->prev());
			if(prevTree)
			{
				child_number = 1+prevTree->get_child_number();
			}
			else assert(!o->prev());
		}
		else //if(dynamic_cast<Tree*>(o))
		{
			//if(dynamic_cast<TreePanel*>(o->parent())) 
			if(dynamic_cast<TreePanel*>(curr_root))
			{
				child_number = ++root_children;
			}
		}
		//UNUSED (why not userid?)
		//o->set_id(_unique_id());// -1 if unset
		o->set_level(level);
		o->set_child_number(child_number);
	}

	update_tree(o);

	curr_root = o;
	curr_branch = NULL; /* Currently at leaf */

	o->set_current(true);

	//refresh();
	//o->activate();

	return o;
}

/****************************** GLUI_TreePanel::format_node() *********/

void UI::Experiments::TreePanel::update_tree(Tree *tree)
{
	if(!tree) return;
	
	tree->set_level_color(lred,lgreen,lblue);

	tree->set_format(format);
	tree->level_name.clear();	
	if(format&DISPLAY_HIERARCHY)
	{
		if(format&HIERARCHY_LEVEL_ONLY)
		glui_format_str(tree->level_name,"%d",tree->get_level());
		if(format&HIERARCHY_NUMERICDOT)		
		if(dynamic_cast<Tree*>(tree->parent()))		
		glui_format_str(tree->level_name,"%s.%d",
		((Tree*)tree->parent())->level_name.c_str(),tree->get_child_number());
		else glui_format_str(tree->level_name,"%d",tree->get_child_number());
	}	

	/*UNUSED (Tree doesn't render set_color)
	if(format&ALTERNATE_COLOR) 
	switch(8%tree->get_level())
	{
	case 7: tree->set_color(0.5f,0.5f,0.5f); break;
	case 6: tree->set_color(0.3f,0.5f,0.5f); break;
	case 5: tree->set_color(0.5f,0.3f,0.5f); break;
	case 4: tree->set_color(0.3f,0.3f,0.5f); break;
	case 3: tree->set_color(0.5f,0.5f,0.3f); break;
	case 2: tree->set_color(0.3f,0.5f,0.3f); break;
	case 1: tree->set_color(0.5f,0.3f,0.3f); break;
	default:tree->set_color(0.3f,0.3f,0.3f); break;
	}
	else tree->set_color(red,green,blue);
	*/

	if(~format&DISABLE_BAR)
	{
		if(format&DISABLE_DEEPEST_BAR)
		{
			tree->disable_bar();
			if(dynamic_cast<Tree*>(curr_root))
			{
				((Tree*)curr_root)->enable_bar();
			}
		}
		else if(format&CONNECT_CHILDREN_ONLY)
		{
			tree->disable_bar();
			if(tree->prev()&&dynamic_cast<Tree*>(tree->prev()))
			{
				((Tree*)tree->prev())->enable_bar();
			}
		}
	}
	else tree->disable_bar();

	redraw(); //2019
}

/****************************** GLUI_TreePanel::update_all() *********/

void UI::Experiments::TreePanel::update_all()
{
	/*2019: Looks fine to me, except for root_children = 0;
	printf("UI::Experiments::TreePanel::update_all() doesn't work yet. - JVK\n");
	return;
	root_children = 0; */

	Panel *saved_root = curr_root;
	Tree *saved_branch = curr_branch;

	go_to_root();

	if(curr_branch&&dynamic_cast<Tree*>(curr_branch))
	{
		update_tree(curr_branch);
	}

	go_to_next();

	while(curr_root&&curr_branch!=first_child())
	{
		if(curr_branch&&dynamic_cast<Tree*>(curr_branch))
		{
			update_tree(curr_branch);
		}

		go_to_next();
	}

	curr_root = saved_root;
	curr_branch = saved_branch;
}

/****************************** GLUI_TreePanel::expand_all() *********/

void UI::Experiments::TreePanel::expand_all()
{
	Panel *saved_root = curr_root;
	Tree *saved_branch = curr_branch;

	go_to_root();

	if(dynamic_cast<Tree*>(curr_root))
	{
		((Tree*)curr_root)->open();
	}

	go_to_next();

	while(curr_root&&curr_branch!=first_child())
	{
		if(dynamic_cast<Tree*>(curr_root))
		{
			((Tree*)curr_root)->open();
		}

		go_to_next();
	}

	curr_root = saved_root;
	curr_branch = saved_branch;
}

/****************************** GLUI_TreePanel::collapse_all() *********/

void UI::Experiments::TreePanel::collapse_all()
{
	Panel *saved_root = curr_root;
	Tree *saved_branch = curr_branch;

	go_to_root(); go_to_next();

	while(curr_root!=NULL&&curr_branch!=first_child())
	{
		if(!curr_branch&&dynamic_cast<Tree*>(curr_root))
		{
			/* we want to close everything leaf-first */

			((Tree*)curr_root)->close();

			/* Rather than simply go_to_next(), we need to manually move the
			curr_root because this node has been moved to the
			collapsed_node list */

			curr_branch = (Tree*)curr_root->next();
			curr_root = (Panel*)curr_root->parent();
		}
		else go_to_next();
	}

	curr_root = saved_root;
	curr_branch = saved_branch;
}

/****************************** GLUI_TreePanel::db() *********/

/* Deletes the curr_root */
void UI::Experiments::TreePanel::delete_tree(Panel *root)
{
	if(root==this) return;

	if(root)
	{
		curr_root = root;
		curr_branch = NULL;
	}

	if(!curr_root||curr_root==this)
	{
		go_to_root(); return;
	}

	Tree *temp_branch = (Tree*)curr_root->next(); /* Next branch, if any */
	Panel *temp_root = (Panel*)curr_root->parent(); /* new root */

	//2019: This was a memory-leak because unlink was zeroing its child pointers
	//curr_root->_unlink(); delete curr_root;
	curr_root->_delete();

	curr_branch = (Tree*)temp_branch;
	curr_root = (Panel*)temp_root;

	if(dynamic_cast<Tree*>(curr_root))
	{
		((Tree*)curr_root)->open();
	
		if(format&DISABLE_DEEPEST_BAR&&!curr_root->next())
		{
			((Tree*)curr_root)->disable_bar();
		}
	}

	//refresh();
}

/****************************** GLUI_TreePanel::fb() *********/

/* Goes up one level, resets curr_root and curr_branch to parents*/
void UI::Experiments::TreePanel::go_up_tree(Panel *branch)
{
	if(branch==this) return;

	if(_curr_branch==this||curr_root==this)
	{
		go_to_root(); return;
	}

	if(!branch)
	{
		branch = curr_root;/* up one parent */

		if(!branch) return;
	}

	if(dynamic_cast<Tree*>(branch))
	{
		((Tree*)branch)->set_current(false);
	}

	curr_branch = (Tree*)branch->next();
	curr_root = (Panel*)branch->parent();

	if(!curr_branch&&curr_root->collapsible->first_child())
	{
		curr_branch = (Tree*)curr_root->collapsible->first_child();
	}

	if(dynamic_cast<Tree*>(curr_root))
	{
		((Tree*)curr_root)->set_current(true);
	}
}

/****************************** GLUI_TreePanel::descendBranch() *********/

/* Finds the very last branch of curr_root, resets vars */
void UI::Experiments::TreePanel::go_to_leaf(Panel *root)
{
	go_to_root(root?root:curr_root);

	if(curr_branch&&_curr_branch!=this)
	{
		if(dynamic_cast<Tree*>(curr_root))
		{
			((Tree*)curr_root)->set_current(false);
		}

		go_to_leaf(curr_branch);
	}
}

/****************************** GLUI_TreePanel::next() *********/

void UI::Experiments::TreePanel::go_to_next()
{
	if(!curr_root) go_to_root();

	if(!curr_branch&&curr_root->collapsible->first_child())
	{
		curr_branch = (Tree*)curr_root->collapsible->first_child();
	}

	if(curr_branch&&_curr_branch!=this)
	{
		/* Descend into branch */
		if(dynamic_cast<Tree*>(curr_root))
		{
			((Tree*)curr_root)->set_current(false);
		}
		go_to_root(curr_branch);
	}
	else if(!curr_branch)
	{
		go_up_tree();/* Backup and move on */
	}
}

/****************************** GLUI_TreePanel::resetToRoot() *********/

/* Resets curr_root and curr branch to TreePanel and last_child */
void UI::Experiments::TreePanel::go_to_root(Panel *new_root)
{
	Panel *root = new_root?new_root:this;

	curr_root = root;

	if(dynamic_cast<Tree*>(curr_root))
	{
		((Tree*)curr_root)->set_current(true);
	}

	/* since Trees are collapsable, we need to check the collapsed nodes
		in case the curr_root is collapsed */
	Node *c = !root->first_child()?root->collapsible:root;

	//2019: Why not just use last_child?
	curr_branch = dynamic_cast<Tree*>(c->last_child());
	if(!curr_branch&&c->last_child())
	{
		//OLD CODE: Falling back to old linked-list traversal
		assert(0);

		curr_branch = (Tree*)c->first_child();

		while(curr_branch&&dynamic_cast<Tree*>(curr_branch))
		{
			curr_branch = (Tree*)curr_branch->next();
		}
	}
	assert(!curr_branch||dynamic_cast<Tree*>(curr_branch));
}

//---.
}//<-'

#endif //GLUI_BUILD_TREE_CONTROLS

