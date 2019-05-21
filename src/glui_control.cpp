/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_control.cpp - top-level GLUI_Control class


          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    http://sourceforge.net/projects/ui/
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

/**************************************** GLUI_Node::_add_control() *********/
	
static void glui_control_scroll_ui_callback(UI::ScrollBar *sb)
{
	sb->redraw(); //EXPERIMENTAL: This should do it, in theory.
}

void UI::Control::set_parent(Node *p)
{
	if(!dynamic_cast<UI::Control*>(p))
	{
		if(UI*ui=dynamic_cast<UI*>(p))
		{
			//EXPERIMENTAL: Attach scrollbar to UI window??
			if(ScrollBar*sb=dynamic_cast<ScrollBar*>(this))
			{
				//NOTE: Unlike _main_panel, this scrollbar has ui as parent.
				sb->_link(sb->ui=ui,!sb->horizontal);
				sb->set_object_callback(glui_control_scroll_ui_callback);
				return;
			}

			p = ui->_main_panel;
		}
	}
	
	if(_parent_node) _unlink();

	if(!p){ ui = NULL; return; }

	UI::Control &parent = *(UI::Control*)p;

	//ui = parent.ui;
	set(&Control::ui,parent.ui);

	//EXPERIMENTAL
	if(!px) px = (PX*)"space"; //default GLUI layout

	/*** Collapsible nodes have to be handled differently, b/c the first and
	last children are swapped in and out  ***/
				   
	if(!parent.is_open&&parent.collapsible) 
	{
		//WARNING: _parent_node remains linked to this???
		_link_this_to_parent_last(parent.collapsible);
		
		//2019 breaking-change: Want normative p tree
		//_parent_node = p;

		/** Now set the 'hidden' var based on the parent **/
		//hidden = true;
	}
	else /** Now set the 'hidden' var based on the parent **/
	{
		//hidden = parent.hidden;

		_link_this_to_parent_last(p);
	}

	enabled = parent.enabled;

	//This pattern permits users to set the font prior
	//to inserting controls. Otherwise the default one
	//would have to be initialized whether used or not.
	if(!font.font)
	{	
		//Lock in _main_panel font.
		if(!parent.font.font) parent.set_font();
		//Inherit parent's font.
		set_font(parent.font,true);
	}

	//This would be pretty harmless if users want it?
	//NOTE: An agument for delaying this is the width
	//of a scrollbar can be added to the control area
	//after user code has had an opportunity to set_w.
	//(E.g. scrollbar_init.)
	//update_size();	

	//NOTICE: This change defers pack until displayed.
	_post_repack(); /* ui->refresh(); */
}
extern void glui_control_Nvidia_timer(int id)
{
	//NOTE: I'm doing this test just to err on caution.
	//(Nvidia GLUT seems to crash by default.)
	if(GLUI::find_glui_by_window_id(id,false))
	glutPostWindowRedisplay(id);
}
void UI::Control::_post_repack(bool repack)
{
	if(!ui) return; //if(!can_draw()) return;	   	

	if(repack) ui->_pending_repacking = true;

	if(!ui->_pending_redisplay)
	{
		ui->_pending_redisplay = true;
					
		if(ui->_pending_idleclose) return; //YUCK

		//This lets activate_control and deactivate_control continue
		//to be used by Nvidia's GLUT:
		//
		// It kills subwindows when glutPostRedisplay is used 
		// inside of mouse events, on Windows.
		//
		if(0) glutPostWindowRedisplay(ui->_glut_window_id);
		else glutTimerFunc(0,glui_control_Nvidia_timer,ui->_glut_window_id);
	}
}

/**************************** Layout and Packing *********************/

/****** GLUI_Control::align() **************/

void UI::Control::_align_control()
{		
	if(dynamic_cast<Column*>(this)) return;

	int orig_x_abs = x_abs;

	int cx,cw; get_column_dims(&cx,&cw);

	Control *ch = first_child();

	int a = alignment; a:

	if(a==ALIGN_LEFT)
	{
		x_abs = cx;
	}	
	else if(a==ALIGN_CENTER)
	{
		x_abs = cx+(cw-w)/2;
	}
	else if(a==ALIGN_RIGHT)
	{
		x_abs = cx+cw-w;
	}
	else if(a==ALIGN_EXPAND)
	{
		//NOTE: _align_children elaborates on ALIGN_EXPAND.

		if(w!=cw) 
		{	
			w = cw; x_abs = cx; update_size(); 

			/***   Shift (proportionally) all child columns   ***/
			for(;ch;ch=ch->next()) if(dynamic_cast<Column*>(ch)) 
			{
				float p = ch->x_abs-x_abs; p = p/w*cw;

				//min is worried about columns on the outer edge.
				//There aren't good reasons to put tack a Column
				//on as the last child.
				//ch->x_abs = std::min(int(cx+p+0.5f),cw-ch->x_rl);
				ch->x_abs = int(cx+p+0.5f);;
			}		
		}
	}
	else if(ch) if(a==ALIGN_BUDDY)
	{
		a = ch->alignment; goto a;
	}
	/***   Shift all child columns   ***/
	for(;ch;ch=ch->next()) if(dynamic_cast<Column*>(ch)) 
	{
		ch->x_abs+=x_abs-orig_x_abs;
	}
}

/********** GLUI_Main::align_controls() **********/

void UI::Control::_align_children()
{
	//This subroutine used to be UI::_align_controls(). They're
	//separate now.
	/* _align_control(); */

	//REMINDER: This looks like _align in a recursive form, however it
	//must be more complicated than that, because it doesn't work that
	//way.

	//ALIGN_BUDDY is a back-compat workaround. It would be
	//better to have it removed.
	if(Control*ch=first_child()) if(ALIGN_BUDDY==alignment)
	{	
		//This will work if alignment is ALIGN_RIGHT. Because
		//GLUI made Spinner have an Edit control instead of the
		//other way around ALIGN_BUDDY is a backward relationship.
		
		//ch->_align_control(); //NEW
		//Here x_rl is to make spinners easier to click at.
		int hotzone = ch->w+ch->x_rl;
		ch->x_abs = x_abs+w-x_rl-hotzone;
		ch->y_abs = y_abs+y_off_top+ch->y_off_top;

		//"this" should should be EditText.
		assert(!ch->next()&&!is_container);
	}
	else //Align children recursively.
	{
		for(;ch;ch=ch->next())
		{
			ch->_align_control(); ch->_align_children();
		}
		  //ALGORITHM
		  //If rollouts are extended to fill the width of their
		  //column/container then if a rollout is for example a
		  //left-aligned list of controls (e.g. checkboxes) the 
		  //layout looks better if the controls are centered as 
		  //a group.

		if(ALIGN_EXPAND==alignment) 
		{
			Column *c = NULL;

			Control *fc = first_child();

			fc: ch = fc;
			
			int cmp = ch->x_abs; 
			int max_w = 0;
			for(;ch;ch=ch->next())
			{
				if(cmp!=ch->x_abs)
				{
					if(c=dynamic_cast<Column*>(ch))
					{
						break;
					}
					cmp = 0; break;
				}
				max_w = std::max(max_w,ch->w);
			}
			if(cmp!=0&&fc!=c) 
			{		
				int cx;
				fc->get_column_dims(&cx,&cmp);
				cmp-=max_w; if(cmp>20*2) //10
				{
					ch = first_child();
					cmp = cx+cmp/2;
					for(ch=fc;ch&&ch!=c;ch=ch->next())					
					if(cmp!=ch->x_abs)
					{
						ch->x_abs = cmp;
						ch->_align_children();
					}
				}
			}
			if(c)
			{
				fc = c->next(); c = NULL; goto fc; 
			}
		}
	}
}

/*** GLUI_Control::get_this_column_dims() **********/
/* Gets the x,y,w,h,and x/y offsets of the column to which a control belongs */

void UI::Control::get_column_dims(int *cx, int *cw)
{	
	Column *c; assert(cx&&cw);

	/*** Look for preceding column ***/    
	for(Control*ch=prev();ch;ch=ch->prev()) 
	{
		if(c=dynamic_cast<Column*>(ch)) if(!c->hidden) 
		{
			*cx = c->x_abs+c->x_rl;
			*cw = c->w;			
			return;
		}
	}

	/*** Nope, Look for next column ***/
	for(Control*ch=next();ch;ch=ch->next())
	{
		if(c=dynamic_cast<Column*>(ch)) if(!c->hidden) 
		{
			if(Control*p=dynamic_cast<Control*>(parent()))
			*cx = p->x_abs+p->x_lr;
			else
			*cx = 0; //collapsed_node (collapsible)		
			*cw = c->x_abs-c->x_lr-*cx;
			return;
		}
	}

	 /*** This is single-column panel, so return panel dims ***/
	if(Control*p=has_parent())
	{	
		*cx = p->x_abs+p->x_lr;	
		*cw = p->w-p->x_lr-p->x_rl;
		return;
	}

	*cx = x_abs; *cw = w; //2019
}

/*** GLUI_Control::pack() **********/

void UI::Control::_pack(int x, int y)
{
	int x_in = x, y_in = y;
	
	//NOTE: In theory containers shouldn't 
	//have to update_size here, but it may
	//also used to initialize their margin.
	if(ALIGN_EXPAND==alignment&&!is_container)
	{	
		//An expanded control is a one way ratchet.
		//If not shrunken down it will distort the
		//column layout. But it can't be by itself
		//in a column as a result.
		w = UI_ALIGN_EXPAND_WIDTH; //8
	}
	else update_size();
		   
	const int x_margin = x_off;
	const int y_margin_top = y_off_top;
			   
	x_abs = x_in; y_abs = y_in;
	
	Control *ch = first_child();
	if(!ch&&collapsible) //2019
	{
		ch = (Control*)collapsible->first_child();

		//NOTE: collapsed spaces are located at 0,0
		//but I don't think it affects width/height 
		//calculations; that are all that is needed.
		
		x_in = y_in = 0;		
	}

	int curr_x = x_in+x_lr;
	int curr_y = y_in+y_margin_top;

	/*** Record start of this set of columns ***/

	int y_top_column = curr_y;
	int column_x     = curr_x;

	/*** Iterate over children, packing them first ***/

	Column *curr_column = NULL;

	int panel_padding = 0; 

	int max_y = curr_y; 

	int max_w = 0; while(ch) if(!ch->hidden)
	{	
		//if(Panel*p=dynamic_cast<Panel*>(ch))
		if(Box_Interface*p=dynamic_cast<Box_Interface*>(ch))
		{				
			int prev_panel = panel_padding;

			if(p->box_type||!p->name.empty())
			{
				if(p->h>UI_EDITTEXT_HEIGHT+2
				&&Panel::ROLLOUT!=p->box_type)
				{
					panel_padding = UI_ITEMSPACING;		
				}
			}	
				
			if(!prev_panel
			&&curr_y!=y_top_column
			&&ch->y_off_top<=UI_ITEMSPACING)
			{
				/* Pad some space above fixed-size panels */
				if(!prev<Separator>())
				curr_y+=panel_padding;
			}
		}
		else panel_padding = 0;

		if(Column*c=dynamic_cast<Column*>(ch)) 
		{
			//NOTE: curr_column sets w/h below...
			curr_column = c;
			int cw = c->get_type()?2:0;
			c->w = cw; 
			c->h = curr_y - y_top_column;
			c->y_abs = y_top_column;
			curr_y = y_top_column;

			/*2019: This is pretty uneven...
			curr_x+=max_w+x_margin;
			column_x = curr_x;
			c->x_abs = curr_x;
			curr_x+=x_margin;
			*/
			curr_x+=max_w; if(cw)
			{	
				c->x_abs = curr_x+x_margin+1;
				c->x_lr = c->x_rl = 1+x_margin; 

				curr_x+=2+x_margin;
			}
			else //Invisible column?
			{
				int hm = x_margin/2; //half-margin

				c->x_lr = hm; c->x_rl = x_margin-hm;

				if(!max_w) //Prior column is hidden?
				{					
					//UNTESTED: Should appear as if 
					//this column uses the previous
					//margin.
					c->x_abs = curr_x-c->x_rl;

					goto dbl_col;
				}
				else c->x_abs = curr_x+hm;
			}
			
			max_w = 0;

			curr_x+=x_margin;

			column_x = curr_x;

		dbl_col: ch = ch->next(); continue;
		}

		ch->_pack(curr_x,curr_y);
							  
		curr_y+=ch->h;		
		
		if(ch->w>max_w) 
		{
			max_w = ch->w;

			//2019: Seems wrong to include the margin here?
			//if(curr_column) curr_column->w = max_w+x_margin;
			if(curr_column) curr_column->w = max_w;
		}

		if(curr_y>max_y) max_y = curr_y;

		if(ch=ch->next()) 
		{
			curr_y+=UI_ITEMSPACING;

			if(ch->y_off_bot<=UI_YOFF+3) //NEW
			{
				/* Pad some space below fixed-size panels */
				if(!next<Separator>())
				curr_y+=panel_padding;
			}
		}
	}
	else ch = ch->next();

	if(is_container) 
	{							  
		int new_w = column_x-x_in+max_w+x_rl;

		if(first_child()) 
		{
			/*** Add bottom border inside box */

			w = new_w; h = max_y-y_in+y_off_bot;

			if(collapsible&&!next<Rollout>())
			{
				h+=2+UI_ITEMSPACING;
			}
		}
		else /* An empty container, so just assign default w & h */
		{          
			//2019: These are both collapsible 
			//if(!dynamic_cast<Rollout*>(this)
			//&&!dynamic_cast<Experiments::Tree*>(this)) 
			if(!collapsible)
			{
				w = UI_DEFAULT_WIDTH;
				h = UI_DEFAULT_HEIGHT;
			}
			else if(max_w) w = new_w; //Rollout width
		}

		/** Expand (e.g., to include all the text in a panel label) **/

		update_size();
		
		/*** Now we step through the columns, setting the 'h'  ***/

		for(ch=first_child();ch;ch=ch->next()) 
		{
			if(dynamic_cast<Column*>(ch)) 
			{
				ch->h = h-y_off_bot-y_margin_top;
			}
		}
	}
}

/******************************** Live Variables **************************/
/*********** GLUI_Control::sync_live() ************/
/* Reads live variable and sets control to its current value                */
/* This function is recursive, and operates on control's children           */

void UI::Control::sync_live(bool recurse, bool draw_it)
{
	/*REFERENCE

		THIS LOGIC IS REPRODUCED BELOW, FOR BETTER OR WORSE. I FEEL 
		LIKE IT'S RUDE TO OVERWRITE THE ACTIVE CONTROL'S INPUT. BUT
		IT'S ON USER CODE TO MAKE THAT CALL.

	bool sync_it = true, changed = false;	
	/*** If this is currently active control, and mouse button is down,
	don't sync **?/
	if(ui)
	{
		if(this==ui->_active_control&&GLUI.curr_button_down)
		{
			sync_it = false;
		}

		//REMOVE ME
		/*** Actually, just disable syncing if button is down **?/
		/*** Nope, go ahead and sync if mouse is down - this allows syncing in
		callbacks **?/
		if(0) /* THIS CODE BELOW SHOULD NOT BE EXECUTED ?/
		{
			if(GLUI.curr_button_down)
			{
				/* printf("Can't sync\n");              ?/
				return;
			}
		}
	}*/

	/***  If this control has a live variable, we check its current value
	against the stored value in the control  ***/

	if(ptr_val&&live_type/*&&sync_it*/) 
	{			
		/*ORIGINAL CODE
		if(live_type==GLUI_LIVE_INT)
		{
			if(*(int*)ptr_val!=last_live_int) 
			{
				set_int_val(*(int*)ptr_val);
				last_live_int = *(int*)ptr_val;
				changed = true;
			}
		}
		EQUIVALENT CODE?*/
		Live_Pointer *cmp = this;
		if(this==GLUI.active_control)
		{				  
			//NOTE: No idea what this filter is intended to accomplish??
			//It's reproduced from the old code up above. I can imagine
			//it's for dragging a spinner maybe? I don't know. It seems
			//like text boxes should enjoy the same protection. In which
			//case the active control may as well be flatly exempt from
			//sync_live.
			if(!GLUI.curr_button_down)
			{				
				//NOTE: See concerns expressed above about how this steps on
				//the end-user's work. I still think get_edited is useful to
				//keep around, even if it's ultimately dropped for sync_live
				//purposes.
				if(GLUI::get_edited()) cmp = GLUI::get_edited();

				goto sync_it;
			}
		}
		else sync_it:
		{	
			if(cmp->live_type->compare())
			{
				live_type->init_live();

				if(cmp!=this) GLUI::set_edited();

				update_live(); //set_int_val

				//NOTE: update_live may redraw... although it shouldn't?
				if(draw_it) redraw();
			}
		}
	}	

	/*** Now recursively output live vars for all children ***/

	if(recurse) for(int i=1;i<=2;i++,draw_it=false)	
	{
		if(Node*ch=(i==1?this:collapsible))		
		for(ch=ch->first_child();ch;ch=ch->next()) 
		{
			((Control*)ch)->sync_live(true,draw_it);
		}
	}
}

/********** GLUI_Control::init_live() ************/
/* Reads in  value of a live variable.  Called once, when ctrl is created   */

void UI::Control::init_live()
{
	assert(ptr_val||live_type==UI_LIVE_NONE);

	live_type->init_live(); 

	update_live(); 

	//2019: Why after and not before? I suppose
	//it's to nullify change-notices, but if so
	//this leaves the last values uninitialized.
	//Maybe they were just for users to consult.
	//last_live._live_dup(*this,float_array_size); //UNSUED

	//Rather do this than have a bool arg overload/signature.
	redraw(); 
}

/********** GLUI_Control::output_live() ************/
/* Writes current value of control to live variable. */

void UI::Control::output_live()
{
	assert(ptr_val||live_type==UI_LIVE_NONE);

	live_type->output_live(); 

	//2019: This makes no sense... why would 
	//setting the user copy update the last?
	//last_live._live_dup(*this,float_array_size); //UNSUED

	/*2019
	All calls to post_update_main_gfx were tied to 
	output_live. It calls glutPostRedisplay on the
	GLUT window set by the old set_main_gfx_window
	This way users can respond to this anyway they
	like. 
	*/
	if(ui&&ui->output_callback&&!ui->_pending_idleclose)
	{
		ui->output_callback(this,true);
	}

	redraw(); //2019: Removing rarely used bool parameter.
}

/****** GLUI_Control::execute_callback() **********/

void UI::Control::execute_callback()
{
	CB *cb; if(!*(cb=&callback)) //2019
	{
		//Mainly want to filter out -1 however as a feature
		//other negative values can be used to disable this.
		if(id<0) return;

		if(!ui||!*(cb=&ui->active_callback))
		{
			if(!*(cb=&GLUI.active_callback))
			{
				return; //2019
			}
		}
	}

	//back-compat (set_main_gfx_window feature)
	int old_window = ui&&ui->_main_gfx_callback?1:0;
	if(old_window>0)
	{
		old_window = glutGetWindow();
		glutSetWindow(ui->_glut_escape_id);
	}
		//TODO: Might want to guard this too.
		//if(!ui||!ui->_pending_idleclose)
		{
			//if(callback) callback(user_id);
			(*cb)(this); 
		}

	if(old_window>0) glutSetWindow(old_window);

	//HACK: output_live is responsible for output_callback.
	//However, when there is a user callback GLUT may have
	//drawn the picture too soon. So it must refresh again.

	//When controls do set_int_val before execute_callback
	//output_callback is called before the callback. If it
	//causes the display to refresh before the callback is
	//processed it may be wrong. Unfortunately GLUT's post
	//APIs don't seem to really "post" like a message that
	//shouldn't happen until the poster subroutine returns.
	if(ui&&ui->output_callback&&!ui->_pending_idleclose)
	{
		ui->output_callback(this,false);
	}
}

/***** GLUI_Control::set_float_array_val() ********/

void UI::Control::set_int_val(int new_int)
{
	GLUI::set_edited(this);
	stage_live(&new_int,1); post_live();
}
void UI::Control::set_text(C_String t)
{
	GLUI::set_edited(this);
	stage_live(t.str?t.str:"",0); post_live();
}
void UI::Control::set_float_val(double new_float)
{
	GLUI::set_edited(this);
	stage_live(&new_float,1); post_live();
}
void UI::Control::set_float_array_val(double *array_ptr)
{
	GLUI::set_edited(this);

	if(!array_ptr) stage_live(float_array_size);
	else stage_live(array_ptr,float_array_size); post_live();
}
void UI::Control::set_float_array_val(float *array_ptr)
{
	GLUI::set_edited(this);

	if(!array_ptr) stage_live(float_array_size);
	else stage_live(array_ptr,float_array_size); post_live();
}

/**** GLUI_Control::set_name() ********************/

void UI::Control::set_name(C_String str)
{
	name = str.str?str.str:""; resize(); 
}

/**** GLUI_Control::enable() ****************/

void UI::Control::enable(bool enable)
{	
	if(Control*c=has_parent())
	if(c->alignment==ALIGN_BUDDY)
	{
		return c->enable(enable);
	}

	//NOTE: Would prefer to not disable scrollbars. Currently
	//they are created as sibling to their associated control.

	//NOTE: Node::_unlink expects this to recursively disable
	//controls, so that it's unnecessary to "unlink" children.
	set(&Control::enabled,enable);
	
	//NOTE: Node::_unlink expects this to deactivate controls.
	if(!enable)
	if(ui&&ui->_active_control&&!ui->_active_control->enabled)
	{
		ui->deactivate_current_control();
	}

	if(scrollbar) 
	{
		scrollbar->enabled = true;

		assert(scrollbar2==scrollbar);
	}

	redraw();
}

/********** GLUI_Control::set_dims() ****************/

void UI::Control::set_dims(int new_w, int new_h)
{
	if(w!=new_w||h!=new_h) //Important.
	{
		w = new_w; h = new_h; resize(); 
	}
}

/***** GLUI_Control::set_alignment() ******/

void UI::Control::set_alignment(int new_align)
{
	alignment = new_align;
	
	_align_control(); _align_children(); redraw();
}
void UI::Control::set_placement(int new_place)
{
	placement = new_place; 
	
	int ww = w, hh = h; 

	update_size(); _post_repack(ww!=w||hh!=h);
}

/********* GLUI_Control::set_hidden() ********/
/** Sets hidden==true for this  control and all its siblings. */
/**  If recurse is true, we go to children as well */
void UI::Control::set_hidden(bool hide, bool recurse)
{
	set(&Control::hidden,hide,recurse); _post_repack();
}

/****************************** GLUI_Rollout::open() **********/

void UI::Control::_set_is_open(bool open)
{
	if(is_open==open) return; is_open = open;

	//update_size gives controls control.
	//if(!open) h = UI_DEFAULT_HEIGHT+7;
	{
		resize(); //_post_repack();
	}

	//2019: Historically I think the parent pointer from inside
	//the collapsed ch retained the value of the Control ch.
	//I'm changing this so that the destructors don't leak them.
	//Currently Control doesn't have a destructor. If it did it
	//could do _set_is_open() but that feels like a hack and it
	//may be in the future closed controls may display children.
	if(!collapsible) return;
	
	/* Copy hidden children into our private list "collapsed_node" */
	std::swap(_child_head,collapsible->_child_head);
	std::swap(_child_tail,collapsible->_child_tail);

	Node &a = open?*collapsible:*this;
	Node &b = open?*this:*collapsible;	
	
	for(Control*p=(Control*)b.first_child();p;p=p->next()) 
	{
		//There's really no need to "hide" in this case. Not doing
		//so permits "hidden" to be used for other purposes, where
		//the hid control no longer takes up space.
		//p->set_hidden(!open);
		
		p->_parent_node = &b; //NEW
	}

	//2019: Don't memory-leak collapsed_node nodes?
	//Conceiveably a control could use this to display
	//two views of itself, but the assert must be removed.
	//assert(a._child_head==a._child_tail&&!a._child_head);
	for(Control*p=(Control*)a.first_child();p;p=p->next()) 
	{
		//See thoughts on not hiding in prior loop comment.
		//p->set_hidden(open); 
		
		p->_parent_node = &a; //NEW
	}	
}

//---.
}//<-'
