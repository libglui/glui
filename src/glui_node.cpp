/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

	 glui_node.cpp - linked-list tree structure


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

#include "../include/glui_internal.h" //"GL/glui.hpp" //PCH

namespace GLUI_Library
{//-.
//<-'

/*************************** glui_sort_node() *******/

void Node::_sort(bool(*pred)(Node*,Node*), int depth)
{
	if(--depth<0||!_child_head) return;

	enum{ ISIZE=32 };
	Node *stackbuf[ISIZE];
	size_t bufsz = ISIZE;
	Node **buf = stackbuf;

	size_t i = 0; 
	Node *p = _child_head;
	for(;;)
	{
		for(;i<bufsz&&p;p=p->next())
		{
			buf[i++] = p;

			if(depth) p->_sort(pred,depth);
		}		
		if(!p) break;

		// else make a bigger buf, try again
		bufsz*=2; if(buf==stackbuf) 
		{
			buf = (Node**)malloc(sizeof(Node*)*bufsz);
		}
		else buf = (Node**)realloc(buf,sizeof(Node*)*bufsz);
	}

	if(i==1)
	{
		//This is in case users don't want to implement a
		//recursive walk of the tree so to systematically
		//initialize the UI.
		pred(buf[0],buf[0]);		
	}
	else 
	{	
		//std::sort(buf,buf+i,pred);
		std::stable_sort(buf,buf+i,pred);

		_child_head = buf[0];
		_child_tail = buf[--i];
		for(;i-->0;)
		{
			buf[i]->_next_sibling = buf[i+1];
			buf[i+1]->_prev_sibling = buf[i];
		}
	}

	if(buf!=stackbuf) free(buf);
}

/*************************** GLUI_Node::link_this_to_parent_last() *******/

/* Links as last child of parent                                         */

void Node::_link(Node *p, int how)
{	
	if(!p) return; //ins_item for first item?

	if(_parent_node) 
	{
		_unlink(); //NEW
	}
	else //Siblings implies parents?
	{
		assert(!_prev_sibling&&!_next_sibling);
	}

	if(how<2)
	{
		if(!p->_child_tail) /* parent has no children */
		{
			p->_child_head = this;
			p->_child_tail = this;		
		}
		else if(0==how) /* parent has children */
		{
			//link_this_to_parent_last
			p->_child_tail->_next_sibling = this;
			_prev_sibling = p->_child_tail;
			p->_child_tail = this;		
		}
		else //UNUSED
		{
			assert(1==how);

			//link_this_to_parent_first
			p->_child_head->_prev_sibling = this;
			_next_sibling = p->_child_head;
			p->_child_head = this;
		}
		_parent_node = p;
	}
	else //UNUSED
	{
		if(how==2) //UNUSED
		{
			//link_this_to_sibling_next
			if(!p->_next_sibling) //node has no next parent
			{
				p->_next_sibling = this;
				_prev_sibling = p;

				//This was the parent's last child, so update that as well
				if(p->_parent_node)
				{
					p->_parent_node->_child_tail = this;
				}
				else assert(0);
			}
			else //node already has a next parent
			{
				p->_next_sibling->_prev_sibling = this;
				_next_sibling = p->_next_sibling;
				p->_next_sibling = this;
				_prev_sibling = p;
			}
		}
		else //UNUSED
		{
			assert(2==how);

			//_link_this_to_sibling_prev
			if(!p->_prev_sibling) //node has no prev parent
			{
				p->_prev_sibling = this;
				_next_sibling = p;

				//This was the parent's first child, so update that as well 
				if(p->_parent_node)
				{
					p->_parent_node->_child_head = this;
				}
				else assert(0);
			}
			else //node already has a prev parent
			{
				p->_prev_sibling->_next_sibling = this;
				_prev_sibling = p->_prev_sibling;
				p->_prev_sibling = this;
				_next_sibling = p;
			}
		}
		_parent_node = p->_parent_node;
	}
}

/**************************************** GLUI_Node::_unlink() **************/

void Node::_unlink()
{
	//Trying to eliminate internal dangling references.
	if(UI::Control*c=dynamic_cast<UI::Control*>(this))
	{
		if(c->ui&&c->ui==GLUI.active_control_ui)
		{
			if(c->_child_head) //Recursive deactivate?
			{
				bool nice = c->enabled;

				c->disable(); //Should deactivate children.

				if(nice) c->enable(); 
			}
			else if(c==c->ui->_active_control)
			{
				c->ui->deactivate_current_control();
			}
		}
	}

	if(_prev_sibling) /* Unlink from prev sibling */
	{
		_prev_sibling->_next_sibling = _next_sibling;
	}
	else /* No prev sibling: this was parent's first child */
	{
		_parent_node->_child_head = _next_sibling;
	}

	if(_next_sibling) /* Unlink from next sibling */
	{
		_next_sibling->_prev_sibling = _prev_sibling;
	}
	else /* No next sibling: this was parent's last child */
	{
		_parent_node->_child_tail = _prev_sibling;
	}

	_parent_node = NULL;
	_next_sibling = _prev_sibling = NULL;

	//2019: This is a memory leak. _unlink is only used on childless
	//nodes except for TreePanel's pruning method
	//_child_head = _child_tail = NULL;
}

/**************************************** GLUI_Node::_dump() **************/

#ifdef GLUI_DEBUG_STDOUT
FILE *Node::_dump_FILE = stdout;
void Node::_dump(const char *name)
{
	fprintf(_dump_FILE,"GLUI_node: %s\n",name);
	fprintf(_dump_FILE,"   parent: %p     _child_head: %p    _child_tail: %p\n",_parent_node,_child_head,_child_tail);
	fprintf(_dump_FILE,"   next: %p       prev: %p\n",_next_sibling,_prev_sibling);
}
#endif

//---.
}//<-'
