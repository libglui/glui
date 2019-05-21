/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_edittext.cpp - GLUI_EditText control class


          --------------------------------------------------

  Copyright(c) 1998 Paul Rademacher

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

int UI::Control::_live_init_text_type(int et, const void *lv)
{
	switch(et)
	{
	case UI_EDIT_INT: Live<int>(this,(int*)lv); break;
	case UI_EDIT_TEXT: Live<char>(this,(char*)lv); break;
	case UI_EDIT_STRING: Live<String>(this,(String*)lv);		
		et = UI_EDIT_TEXT; break;          
	case UI_EDIT_FLOAT: Live<float>(this,(float*)lv); break;		
	case UI_EDIT_DOUBLE: Live<double>(this,(double*)lv); 		
		et = UI_EDIT_FLOAT; break;
	}	
	return et;
}

/****************************** GLUI_EditText::draw() **********/

void UI::EditText::_draw()
{
	_dump("-> DRAW");

	Box_Interface::_draw();

	//2019: Don't do this here.
	/** Find where to draw the text **/
	//update_substring_bounds();
	_CLAMP_substring();
	
	draw_substring(); draw_insertion_pt();
		
	_dump("-> DRAW");
}
void Text_Interface::_CLAMP_substring()
{
	int len = get_char_count();
	CLAMP(substring_start,0,len);
	//CLAMP(substring_end,0,len);
	CLAMP(substring_end,substring_start,len); 
}

/********************************* GLUI_EditText::draw_substring() ****************/

void Text_Interface::draw_substring(int y)
{	
	bool et_like = y==-1;
  	
	/** Find where to draw the text **/

	const int text_x = x_lr+UI_BOXINNERMARGINX;

	if(!et_like) y+=UI_BOXINNERMARGINX; //TextBox	

	/** Find lower and upper selection bounds **/	

	const int sel_lo = std::min(sel_start,sel_end);
	const int sel_hi = std::max(sel_start,sel_end);
	const int sss = substring_start;
	const int sse = substring_end;	
	const int tw = tab_width;

	/** Draw selection area dark **/
	if(sel_start!=sel_end) 
	{
		bool sel_empty = true;
		int sel_x_start = text_x;
		int i,sel_x_end = text_x;
		for(i=sss;i<sse;i++)
		{
			int delta; 
			if('\t'==text[i]&&tw) //zero divide
			{
				delta = tw-(sel_x_end-text_x)%tw; 
			}
			else delta = char_width(text[i]);

			if(i<sel_lo) 
			{
				sel_x_start+=delta;
				sel_x_end+=delta;
			}
			else if(i<sel_hi) 
			{
				sel_x_end+=delta;

				sel_empty = false;
			}
			else break; //NEW?		
		}

		if(!sel_empty) //if(sel_x_start!=sel_x_end)
		{	
			if(sel_x_start!=text_x) sel_x_start+=1;
			
			if(sel_x_start==sel_x_end) sel_x_end--; //\n\n line?

			if(i!=sse)
			{
				sel_x_end+='\n'==text[i]?2:1;
			}
			else sel_x_end+=et_like?2:3;

			glColor3f(0,0,0.6f);
			if(et_like)
			{
				glRecti(sel_x_start,y_off_top,sel_x_end,h-y_off_bot);
			}
			else glRecti(sel_x_start,y+3,sel_x_end,y+font.height+2);
		}
	}

	int x = text_x+1;
	
	if(et_like) y = UI_BOXINNERMARGINX; else y+=4;

	for(int l,ll=-1,i=sss;i<sse;i++)
	{
		l = IN_BOUNDS(i,sel_lo,sel_hi-1)?255:enabled?0:64;
		if(l!=ll) 
		{
			glColor3ub(ll=l,l,l);
			//REMINDER: OpenGL ties glColor to glRasterPos.
			begin_to_draw_string(x,y);
		}

		if('\t'==text[i]&&tw) //zero divide
		{
			//I gotta say this is easier to wrap my head around.
			//But maybe it's best to be consistent.
			//x = (x-text_x)/tw*tw+tw+text_x;
			x+=tw-(x-text_x)%tw;

			// Reposition pen after tab
			begin_to_draw_string(x,y);

			continue;
		}
		//else
		{
			glutBitmapCharacter(font.font,text[i]);

			x+=char_width(text[i]);
		}
	}
}

/************************************ GLUI_EditText::update_size() **********/

void UI::EditText::_update_size()
{	
	Box_Interface::_update_size();

	//int dw = w; //Not working out.

	int delta,text_size = string_width(name);

	if(text_size)
	{
		if(x_lr<text_size+2)
		{
			delta = text_size+2-x_lr;
		}
		else delta = 0;

		x_lr+=delta; /* w+=delta; */
	}

	text_size = UI_EDITTEXT_MIN_TEXT_WIDTH;
	if(data_type==EDIT_INT)
	text_size = UI_EDITTEXT_MIN_INT_WIDTH;	
	w = std::max(w,x_lr+text_size);

	/*SPINNER_UP_ON poses a problem for this.
	//NOTE: get_page_length needs to be nonzero.
	h = std::max(h,font.height+y_off_top+y_off_bot);*/

	//if(dw!=w)
	update_substring_bounds();
}

/********************************** GLUI_EditText::set_int_val() ************/

void Text_Interface::_evaluate_limits()
{
	if(!has_limits||data_type==UI_EDIT_TEXT) return;
	
	double new_val = float_val;

	if(has_limits==LIMIT_CLAMP) 
	{
		/*** Clamp the new value to the existing limits ***/

		CLAMP(new_val,limits1,limits2);
	}
	else if(has_limits==LIMIT_WRAP) 
	{
		/*** Clamp the value cyclically to the limits - that is, if the
		value exceeds the max, set it the the minimum, and conversely ***/

		if(new_val<limits1) new_val = limits2;
		if(new_val>limits2) new_val = limits1;
	}

	if(new_val!=float_val)
	{
		if(data_type==EDIT_INT)
		{
			set_int_val((int)new_val);
		}
		else if(data_type==EDIT_FLOAT)
		{
			set_float_val(new_val);
		}
		else assert(data_type==EDIT_FLOAT);
	}	
}
void UI::EditText::_update_live()
{
	//set_float_val //set_int_val
	{
		_evaluate_limits();	
	}	
	//set_text
	{
		//text            = new_text;
		substring_start = 0;
		substring_end   = get_char_count();
		GLUI::reset_caret(this); 
		sel_start       = 0;
		sel_end         = 0;

		update_substring_bounds(); /* update_and_draw_text(); */		

		/** Update the spinner, if we have one **/
		if(spinner)	spinner->set_val(float_val);
	}
}

/********************************* GLUI_EditText::set_float_limits() *********/

void Text_Interface::set_limits(double l1, double l2, int limit_type)
{
	limits1 = l1; limits2 = l2; has_limits = limit_type;

	if(data_type!=UI_EDIT_TEXT)
	{
		float new_val = float_val; CLAMP(new_val,l1,l2);

		if(new_val!=float_val) set_float_val(new_val);
	}
}

/**************************************** GLUI_EditText::mouse_over() ********/

bool Text_Interface::_mouse_over(bool state, int local_x, int local_y)
{
	if(state/*&&enabled*/&&box_test(local_x,local_y)) 
	{
		glutSetCursor(GLUT_CURSOR_TEXT);
	}
	else return true; return false; 	
}

/****************************** GLUI_EditText::mouse_down_handler() **********/

namespace edittext_found_ip
{
	static UI::Control *c = NULL; static int x,y,meridian;
}
bool Text_Interface::_mouse_down_handler(int local_x, int local_y)
{	
	_dump("-> MOUSE DOWN");

	edittext_found_ip::c = this;
	edittext_found_ip::meridian = 0;

	//find_insertion_pt tests this.
	//if(!box_test(local_x,local_y)) return true;
	int tmp_insertion_pt = find_insertion_pt(local_x,local_y);
	if(-1==tmp_insertion_pt)
	{
		edittext_found_ip::c = NULL;

		//if(ui) ui->deactivate_current_control();
		return true; //return false; //2019
	}

	//Need to wait until mouse_up_handler to be activated.
	//GLUI::set_caret(this,sel_start=sel_end=tmp_insertion_pt);
	sel_start = sel_end = tmp_insertion_pt;

	//update_and_draw_text(); //Can wait until held_down/up.

	_dump("<- MOUSE DOWN"); //"<- MOUSE UP"

	return false; //return true; //2019
}

/***************************** GLUI_EditText::mouse_held_down_handler() ******/

bool Text_Interface::_mouse_held_down_handler(int local_x, int local_y, bool inside)
{
	_dump("-> HELD DOWN");

	int tmp_pt = find_insertion_pt(local_x,local_y,true);

	if(tmp_pt!=-1&&tmp_pt!=sel_end)
	{
		sel_end = tmp_pt; 
		
		GLUI::set_caret(this);

		update_and_draw_text();
	}

	_dump("<- HELD DOWN");

	return false;
}
 
/***************************** GLUI_EditText::mouse_up_handler() ******/

bool Text_Interface::_mouse_up_handler(int local_x, int local_y, bool inside)
{
	if(this==edittext_found_ip::c)
	if(GLUI::set_caret(this,sel_start==sel_end?sel_end:-1))
	{
		//TODO: Have to adjust these and call caret_callback somehow.
		//GLUI.caret_x = local_x-x_abs; GLUI.caret_y = local_y-y_abs;
		GLUI.caret_x = edittext_found_ip::x;
		GLUI.caret_y = edittext_found_ip::y;
	}
	redraw(); return false;
}

/****************************** GLUI_EditText::key_handler() **********/

inline void Text_Interface::_kh_erase(int start, int end)
{
	if(!enabled) return;	

	CLAMP(start,0,get_char_count()); assert(start<=end);  
	CLAMP(end,0,get_char_count());  

	if(start==end) return;	

	GLUI::set_edited(this);

	text.erase(start,end-start);
	//GLUI::set_caret(this,sel_start=sel_end=start);			
	sel_start = sel_end = start;
}
bool Text_Interface::_key_handler(int key, int modifiers)
{	
	_dump("-> KEY HANDLER");

	edittext_found_ip::meridian = 0;

	int len = get_char_count();
	int leftmost = sel_min(), rightmost = sel_max();
		  						
	ctrl:

	//I've turned key into int so user/control code
	//can negate its keys to act like Unix Readline.
	//or vice versa when GLUI_READLINE_MODIFIERS is
	//defined at build time. CommandLine might be a
	//way to demonstrate this option.
	#ifdef GLUI_READLINE_MODIFIERS	
	int rl_key = -key;
	#else
	int rl_key = +key;
	#endif
	int abs_key = ABS(key);
	if(abs_key<=127) key = abs_key;

	if(iscntrl(abs_key)) switch(rl_key)
	{				
	case -'\r': //-CTRL('m')
	case +'\r': /* RETURN */
 
		switch(return_input_model)
		{
		case UI::ACTIVATE_OTHER: //EditText

			if(!enabled) return false;

			deactivate(); /** Force callbacks, etc **/
			activate(UI::ACTIVATE_OTHER); /** Reselect all text **/			
			return false;

		case '\r': case '\n': case 0x0D0A:
		
			//if(multiline) //TextBox
			{
				/* RETURNS are written as newlines*/
				key = return_input_model;
			}
			goto whitespace;
		}
	default: return true; //keyboard_callback or wrapper?
  
	case -27: //CTRL('[')
	case +27: /* ESCAPE */
	   
		//GLUI_Main::keyboard returns control to the container.
		assert(0); break;
	
	case -'\t': //CTRL('i'):	
	case +'\t': /* TAB */
	{	
		if(!enabled) return false;

		GLUI::set_edited(this);

		if(!tab_width) key = ' ';

		//Ctrl+Tab is used to override tab_naviate.
		if(modifiers&&modifiers!=GLUT_ACTIVE_SHIFT)
		{
			modifiers = 0; //return true;
		}

		//Visual Studio does this tab on multi-line selections.
		for(int i=leftmost;i<rightmost;i++) 
		if('\n'==text[i]&&i!=rightmost)
		{
			for(i=leftmost;i&&text[i]!='\n';)
			i--;
			if(i) i++;

			for(;i<rightmost;i++)
			{
				if(modifiers==GLUT_ACTIVE_SHIFT)
				{
					if(i&&isspace(text[i]))
					{
						text.erase(i,1);

						(sel_end<sel_start?sel_start:sel_end)--;
					}
				}
				else
				{
					text.insert(i,1,key);

					(sel_end<sel_start?sel_start:sel_end)++;
				}
				while(i<rightmost&&text[i]!='\n') i++;
			}			
			goto caret;
		}

		if(modifiers==GLUT_ACTIVE_SHIFT)
		{	
			if(leftmost&&isspace(text[leftmost-1]))
			{			
				text.erase(leftmost-1,1);

				sel_end--; sel_start--;
			}
			else return false;
			
			goto caret;
		}

		goto whitespace;
	}
	case +127: //Delete 

		if(modifiers&GLUT_ACTIVE_CTRL) //Control+Delete
		{
			delete_word_forward: //Alt+D or Ctrl+Delete

			//leftmost = sel_start = GLUI.curr_caret_pt;
			//rightmost = sel_end = find_word_break(GLUI.curr_caret_pt,1);		
			rightmost = sel_end = find_word_break(rightmost,1);		
		}
		goto Del;

	case -127: case -CTRL('d'): /* FORWARD DELETE */
  
		if(modifiers&&(key==127||modifiers!=GLUT_ACTIVE_CTRL))
		{
			return true; //Verbatim: Delete or Ctrl+D only?
		}

	Del: //delete_forward:
		
		if(leftmost==rightmost) /* no selection */
		{   			
			if(rightmost/*GLUI.curr_caret_pt*/<len) 
			{							
				/*** Shift over string first ***/
				//rightmost = GLUI.curr_caret_pt+1;
				rightmost++;
			}
		}

	del: _kh_erase(leftmost,rightmost);

		break;

	case -'\b': //-CTRL('h')
	case +'\b': /* BACKSPACE */
  
		if(leftmost==rightmost) /* no selection */
		{
			/*** Shift over string first ***/
			//if(GLUI.curr_caret_pt>0) 
			//leftmost = GLUI.curr_caret_pt-1;
			if(leftmost>0) leftmost--;
		}
		goto del;
  
	//-CTRL('a') Appears below.
	case +CTRL('a'): //SELECT-ALL
	
		leftmost = sel_start = 0; 			
		rightmost = sel_end = len; //GLUI.curr_caret_pt
		break;
	
	//NOTE: Ctrl+Insert CAN ALSE COPY. HOW TO DETECT IT?

	case -CTRL('x'): case +CTRL('x'): 
	case -CTRL('c'): case +CTRL('c'): //COPY
	
		if(leftmost==rightmost) return false;
				
		Clipboard.set_text(text.c_str()+leftmost,rightmost-leftmost);
			
		if(key==CTRL('x')) goto del; //CUT?

		break;
   
	case -CTRL('v'): case +CTRL('v'): //"PASTE"	
	{
		String v; if(enabled&&Clipboard.get_text(v))
		{
			_kh_erase(leftmost,rightmost);

			GLUI::set_edited(this);

			text.insert(leftmost,v.c_str());

			sel_start = sel_end = leftmost+(int)v.size();

			//substring_end+=(int)v.size(); //Hint to update_and_draw_text?
		}
		break;
	}
	//GLUI_READLINE_MODIFIERS
	case -CTRL('a'): return special_handler(GLUT_KEY_HOME,0);
	case -CTRL('e'): return special_handler(GLUT_KEY_END,0);
	case -CTRL('b'): return special_handler(GLUT_KEY_LEFT,0);
	case -CTRL('f'): return special_handler(GLUT_KEY_RIGHT,0);
	case -CTRL('p'): return special_handler(GLUT_KEY_UP,0);
	case -CTRL('n'): return special_handler(GLUT_KEY_DOWN,0);		
	case -CTRL('u'): /* ERASE LINE */				  
		leftmost = 0; //_kh_erase(0,len); break;
	case -CTRL('k'): /* KILL TO END OF LINE */		 
		_kh_erase(leftmost/*GLUI.curr_caret_pt*/,len);
		break;	
	}
	else if(isprint(key)||abs_key>127) whitespace:
	{
		//GLUI_READLINE_MODIFIERS
		if(modifiers==GLUT_ACTIVE_ALT) /* ALT ONLY */
		{
			if(-'b'==rl_key) // Backward word
			{
				return special_handler(GLUT_KEY_LEFT,GLUT_ACTIVE_CTRL);
			}
			if(-'f'==rl_key) // Forward word
			{
				return special_handler(GLUT_KEY_RIGHT,GLUT_ACTIVE_CTRL);
			}
			if(-'d'==rl_key) goto delete_word_forward;
		}	
		if(modifiers&(GLUT_ACTIVE_CTRL|GLUT_ACTIVE_ALT))
		{
			if(modifiers==GLUT_ACTIVE_CTRL)
			{
				if(key>='a'&&key<'z'||key>='A'&&key<'Z')
				{
					key = CTRL(key); goto ctrl;
				}
			}

			/** ignore other keys with modifiers */
			return true;
		}
	
		/**** If there's a current selection, erase it ******/
		_kh_erase(leftmost,rightmost);

		/******** We insert the character into the string ***/

		//This is for return_input_model (0x0D0A) but can be used for
		//UTF-8 if a control manages its own input and renders itself.
		char big_endian_or_4B_sequence[8] = 
		{ 
			char(0xFF&key),char(0xFF&key>>8),
			char(0xFF&key>>16),char(0xFF&key>>24) 
		};
		int adv = strlen(big_endian_or_4B_sequence);
		
		if(enabled)
		{
			GLUI::set_edited(this);

			text.insert(sel_start,big_endian_or_4B_sequence);
		
			//substring_end+=adv; //Hint to update_and_draw_text?

			/******** Move the insertion point and substring_end one over ******/
			//GLUI.curr_caret_pt++;
			//sel_start = sel_end = ++GLUI.curr_caret_pt;
			sel_start = sel_end+=adv;
		}
	}
	else return true; caret:

	if(GLUI::set_caret(this,sel_start==sel_end?sel_end:-1))
	{
		find_insertion_xy(&GLUI.caret_x,&GLUI.caret_y);
	}

	/******** Now redraw text ***********/
      
	update_and_draw_text();
  
	_dump("<- KEY HANDLER");

	return false;
}
bool UI::EditText::_key_handler(int key, int modifiers)
{
	//FILTERS
	/** Check if we only accept numbers **/
	if(data_type==EDIT_FLOAT)
	{
		if(key=='.'||key==-'.') 
		{
			/** We're trying to type a period, but the text already contains
			a period.  Check whether the period is contained within
			is current selection(thus it will be safely replaced) */
			int dp =(int)text.find_first_of('.');
			if(dp!=text.npos)
			if(dp<sel_min()||dp>=sel_max())
			return true;
			goto decimal_point;
		}
		goto floating_point;	 
	}
	if(data_type==EDIT_INT) 
	{
		floating_point:
		{
			int ak = ABS(key);
			if((key<'0'||key>'9')&&key!='-'/*&&key!='.'*/)
			{
				if(!iscntrl(ak)) return true;
			}
		}
		decimal_point:

		if(key=='-'||key==-'-')  /* User typed a '-' */
		{
			/* If user has first character selected, then '-' is allowed */
			if(sel_min()||text[0]=='-'&&!sel_max()) 
			{
				/* Can only place negative at beginning of text,
				and only one of them */
				return true; 
			}
		}
	}    

	return Text_Interface::_key_handler(key,modifiers);
}
 
/********************************* GLUI_EditText::special_handler() **********/

bool UI::EditText::_special_handler(int key, int modifiers)
{
	switch(key)
	{
	case GLUT_KEY_PAGE_UP: 
	case GLUT_KEY_PAGE_DOWN: return true;		
	case GLUT_KEY_UP: case GLUT_KEY_DOWN:

		key = key==GLUT_KEY_UP?GLUT_KEY_LEFT:GLUT_KEY_RIGHT;
		
		//Normally I'd choose GLUT_ACTIVE_CTRL for this, but
		//GLUI is using that for reducing the increment.
		if(modifiers&~GLUT_ACTIVE_SHIFT) if(Spinner*s=spinner)
		{
			if(data_type==EDIT_INT)
			{
				modifiers&=~GLUT_ACTIVE_CTRL;
			}
			return s->_special_handler(key,modifiers);
		}		
		break;
	}
	return Text_Interface::_special_handler(key,modifiers);
}
bool Text_Interface::_special_handler(int key, int modifiers)
{
	_dump("-> SPECIAL HANDLER");
	
	int selector = 0;  int ss = sel_start;

	int se = sel_end, len = get_char_count();

	if(key==GLUT_KEY_UP||key==GLUT_KEY_DOWN)
	{
		selector = key; goto selector;
	}
	else edittext_found_ip::meridian = 0;

	switch(key)
	{
	case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
	{
		int dx = key==GLUT_KEY_LEFT?-1:1;

		if(modifiers&GLUT_ACTIVE_CTRL) 
		{
			se = find_word_break(sel_end,dx);
		}
		else if(ss==se||modifiers&GLUT_ACTIVE_SHIFT)
		{
			se+=dx;
		}
		else if(dx<0) //GLUT_KEY_LEFT
		{
			se = std::min(ss,se);
		}
		else //GLUT_KEY_RIGHT
		{
			se = std::max(ss,se);
		}
		break;
	}
	case GLUT_KEY_PAGE_UP: case GLUT_KEY_PAGE_DOWN:

		if(!scrollbar) return true; //...

	case GLUT_KEY_HOME: case GLUT_KEY_END:
	
		if(1!=get_line_limit()&&~modifiers&GLUT_ACTIVE_CTRL)
		{
			selector = key;
		}
		else se = key==GLUT_KEY_HOME?0:len; break;

	default:
		//Don't react to GLUT_KEY_CTRL_L(freeglut)
		//by clearing the selection.
		//return false;
		return true; //signal keycode unprocessed
	}

	if(selector) selector: //EXPEDIENT
	{		
		//TODO: Could scan for lines above/below substring_start/end
		//if there was some convenient subroutines already available.
		int x,y; if(-1!=find_insertion_xy(&x,&y))
		{
			int m[4]; get_margin_dims(m,UI_BOXINNERMARGINX);
			switch(selector)
			{
			case GLUT_KEY_UP: case GLUT_KEY_DOWN: 
				
				if(this!=edittext_found_ip::c
				||!edittext_found_ip::meridian)
				{
					if(se<len&&se>0)
					{
						x+=char_width(text[se])/2;
					}
					edittext_found_ip::meridian = x;
				}
				else x = edittext_found_ip::meridian;

				y+=font.height*(key==GLUT_KEY_UP?-1:1); 
				
				break;
			
			case GLUT_KEY_HOME: x = -1000; break;
			case GLUT_KEY_END: x = m[2]; break;
			case GLUT_KEY_PAGE_UP: 
			case GLUT_KEY_PAGE_DOWN:				
				selector = key==GLUT_KEY_PAGE_UP?-1:1;
				y+=m[3]*selector;
				assert(scrollbar);				
				selector*=m[3]/font.height;
				scrollbar->set_int_val(selector+scrollbar->int_val);					
				break;				
			}						
			x+=x_abs+m[0]; 
			y+=y_abs+m[1]-scrollbar_val()*font.height;
			se = find_insertion_pt(x,y,true);			
			if(se==-1) return false;
		}		
	}

	/* Make sure GLUI.curr_caret_pt is in bounds */
	
	CLAMP(se,0,len); 

	/*** Update selection if shift key is down ***/
	if(~modifiers&GLUT_ACTIVE_SHIFT) sel_start = se;

	if(se!=sel_end||ss!=sel_start)
	{	
		sel_end = se;
	
		if(GLUI::set_caret(this,se==sel_start?se:-1))
		{
			if(selector)
			{
				GLUI.caret_x = edittext_found_ip::x;
				GLUI.caret_y = edittext_found_ip::y;
			}
			else find_insertion_xy(&GLUI.caret_x,&GLUI.caret_y);
		}
		
		/******** Now redraw text ***********/
		
		update_and_draw_text();
	}
	
	_dump("<- SPECIAL HANDLER");		

	return false; //return true; //2019
}

/****************************** GLUI_TextBox::activate() **********/

bool Text_Interface::_activate(int how)
{
	if(how==UI::ACTIVATE_TAB) 
	{
		if(!tab_navigate) return false; //2019: new default behavior
	}

	_dump(how?"-> ACTIVATE":"-> DEACTIVATE");

	if(how)  //Identical to EditText
	{
		//prev_text = text;

		/* Don't select everything if activated with mouse */
		if(how==UI::ACTIVATE_MOUSE) return true;

		sel_start = 0;
		sel_end = get_char_count();
	}
	else //deactivate()
	{	
		if(tab_navigate) //2019: Why discard selection?
		{
			sel_start = sel_end = 0; //-1; 
		}

		//2019: Moving this check higher up.
		//if(prev_text!=text) 
		if(GLUI::get_edited()&&GLUI::get_edited()->text!=text)
		{
			/***** Retrieve the current value from the text *****/
			/***** The live variable will be updated by set_text() ****/

			//THIS FEELS WRONG
			/* This will force callbacks and gfx refresh */
			//This resets the scrollbar position as if the text was reset
			//by the user.
			//set_text(text);
			_evaluate_limits();
			stage_live(&text); output_live(); //This doesn't.
			//set_text would update this too.
			if(spinner) spinner->set_val(float_val);
		
			/***** Now do callbacks if value changed ******/
			//if(prev_text!=text)
			{
				execute_callback();
			}
		}
	}

	_dump(how?"<- ACTIVATE":"<- DEACTIVATE");

	return true;
}

/************************** GLUI_EditText::update_substring_bounds() *********/

bool Text_Interface::update_substring_bounds()
{
	_dump("-> UPDATE SS");

	bool auto_hscroll = 1==get_line_limit(); 
		   	
	_CLAMP_substring();

	int old_start = substring_start, old_end = substring_end;

	int se = sel_end, len = text.size();

	int x,y; if(!auto_hscroll) //TextBox?
	{	  
		//The TextEdit code only called this from inside its activate method.
		//I don't think maintains substring_start/substring_end as it is now.
		//return false; (Update: It does now...)

		//EXPERIMENTAL
		//NEW: Count lines, and set substring_start/end to the visible range.
		_find_insertion_pt(&x,&y,len); 
	
	common_return: //auto_hscroll

		_dump("<- UPDATE SS");

		if(substring_start==old_start&&substring_end==old_end)
		{
			return false; /*** bounds did not change ***/
		}
		else //return true; /*** bounds did change ***/
		{
			//Assuming single line, caret may slide left/right.
			if(auto_hscroll&&this==GLUI::get_caret())
			{
				int pt = _find_insertion_pt(&x,&y,se); 
				
				if(se==pt||GLUI::set_caret(this,pt))
				{
					GLUI.caret_x = x; GLUI.caret_y = y;
				}
			}

			return true;
		}
	}

        ////// The rest applies to EditText or single-line /////


	/*** Calculate the width of the usable area of the edit box ***/
	int box_width = get_box_width();

	box_width-=3; //Making (minimum) right selection margin match.
	
	int sw = substring_width(); //GLUI.curr_caret_pt;
	
	assert(se>=0); //_CLAMP_substring
	if(/*se>=0&&*/se<substring_start) /* cursor moved left */
	{
		while(se<substring_start) sw+=substring_push_front(); 

		while(sw>box_width) sw-=substring_pop_back(); 
	}
	else if(se>substring_end) /* cursor moved right */
	{  
		while(substring_end<se) sw+=substring_push_back(); 

		while(sw>box_width) sw-=substring_pop_front();
	}
	else /* cursor between start/end */ 
	{
		while(sw>box_width) 
		{
			sw-=substring_pop_back();
		}		
		for(int cw;substring_end<len;sw+=cw)
		{
			cw = substring_push_back();

			//This extra care is needed to keep the range 
			//stable while jogging cursor with a keyboard.
			if(sw+cw>box_width)
			{
				substring_pop_back(); break;
			}
		}
	}
	
	//2019: Adding this (auto-scroll logic) for EditText.
	//It may not make sense for TextBox, but it shouldn't
	//hurt (Text_Interface doesn't distinguish among them.)
	for(int i=substring_start;i-->0&&sw<box_width;)
	{
		//2019: Recover start after glutReshapeWindow event.
		sw+=substring_push_front(); 
	}

	//while(sw>box_width) sw-=substring_pop_back();	
	{
		//Favor back, unless cursor is very near front, then keep 
		//the string legible.
		float pc = (se-substring_start)/
		float(substring_end-substring_start); //zero divide
		if(old_end<=substring_end&&pc>0.15f) //if(se>=substring_end)
		{
			while(sw>box_width) sw-=substring_pop_front();
		}
		else while(sw>box_width) sw-=substring_pop_back();
	}

	goto common_return;
}

/****************************** GLUI_EditText::find_word_break() **********/
/* It looks either left or right(depending on value of 'direction'       */
/* for the beginning of the next 'word', where word are characters       */
/* separated by one of the following tokens:  " :-.,"                    */
/* If there is no next word in the specified direction, this returns     */
/* the beginning of 'text', or the very end.                             */

int Text_Interface::find_word_break(int start, int direction)
{
	//I feel like (for English) these punctuation marks will have spaces.
	//const char *breaks = " :-.," "\n\t";
	const char breaks[] = " -/\\" "\n\t"; //TODO: handle \r and \r\n digraph.
	int num_break_chars = sizeof(breaks)-1; //(int)strlen(breaks);
	int text_len = get_char_count();

	/** If we're moving left, we have to start two back, in case we're either
	already at the beginning of a word, or on a separating token.
	Otherwise, this function would just return the word we're already at **/ 

	/*2019: Callers can do this subtraction at their call site if they want.
	if(direction==-1) start-=2;*/

	/***** Iterate over text in the specified direction *****/
	for(int i=start;i>=0&&i<text_len;i+=direction) 
	{
		/** For each character in text, iterate over list of separating tokens **/
		for(int j=0;j<num_break_chars;j++) 
		{
			if(text[i]==breaks[j]) 
			{
				/** character 'i' is a separating token, so we return i+1 **/
				int new_pt = i+1;

				CLAMP(new_pt,0,text_len);

				return new_pt;
			}
		}
	}

	if(direction>0)  /* Return the end of string */
	{
		return text_len;
	}
	else return 0; /* Return the beginning of the text */    
}

/******************************** GLUI_EditText::find_insertion_pt() *********/
/******************************** GLUI_TextBox::find_insertion_pt() *********/
/* This function returns the character numer *before which* the insertion    */
/* point goes                                                                */
 
bool Text_Interface::substring_wrap_compare(int string_width, int box_width)
{
	//This common comparison must be pixel perfect across algorithms.

	return string_width>box_width-4; //-4: Matches right/left margin.
}
int Text_Interface::_find_insertion_pt(int *found_x, int *found_y, int mode)
{	
	int len = get_char_count();

	bool held_down = false;

	bool find_xy = mode>=-1, find_pt = !find_xy;

	int local_x, local_y; if(find_pt) //find_pt
	{
		local_x = *found_x, local_y = *found_y;

		held_down = mode==-3;
			
		/*** See if we clicked outside box ***/
		if(!held_down&&!box_test(local_x,local_y)) return -1;

		found_x = &edittext_found_ip::x;
		found_y = &edittext_found_ip::y;
	}
	else //find_xy
	{
		*found_x = *found_y = 0;

		local_x = local_y = INT_MAX;		

		if(mode<=0||mode>len) return mode==0?0:-1;
	}
	
	/*** See if we clicked in an empty box ***/
	if(!len) return 0;
	
	if(1==get_line_limit()) //EditText path?
	{
	  //The idea is to merge these paths as soon
	  //as they're better understood.

		/* We move from right to left, looking to see if the mouse was clicked
		to the right of the ith character */

		int x,xx = x_abs
		+x_lr	             //The edittext box has a 2-pixel margin
		+UI_BOXINNERMARGINX;

		/** find mouse click in text **/
		_CLAMP_substring();

		if(held_down) xx-=substring_width(0,substring_start);

		int pt = held_down?0:substring_start;
		int se = held_down?len:substring_end; 
		if(find_xy)
		{
			if(mode<pt||mode>se) 
			{
				update_substring_bounds();
				pt = substring_start;
				if(mode<pt||mode>substring_end) 
				{
					return -1;
				}
			}
			se = mode;
		}
		int cw = 0;
		for(x=xx;pt<se&&local_x>x;pt++)
		{
			cw = char_width(text[pt]);
			x+=cw;
		}
		if(find_pt&&local_x<x) 
		{					 
			if(local_x<=x-cw/2&&pt>0) //NEW
			{				
				/* printf("-> %d\n",se); */

				x-=cw; pt--;
			}
		}
		*found_x = x-xx; *found_y = 0;
		return pt;
	}

	bool follow_cursor = false;

	int x,y, m[4], lh = get_line_height();
	get_margin_dims(m,UI_BOXINNERMARGINX);

	//These implement update_substring_bounds so that
	//the draw subroutine doesn't have to process the
	//entire text.	
	int visible_lines = m[3]/lh;
	int sss_line = scrollbar_val();
	int sse_line = sss_line+visible_lines-1;
	if(!sss_line) substring_start = 0;	

	int stop_ln;
	int stop_len; if(find_pt) //find_pt
	{	
		x = local_x-x_abs-m[0]; 
		y = local_y-y_abs-m[1];	

		stop_len = len; 
		stop_ln = std::max(0,sss_line+y/lh);
	}
	else //find_xy
	{
		stop_len = std::min(len,mode+1);
		stop_ln = x = y = INT_MAX;

		if(stop_len==len) //update_substring_bounds?
		{	
			//HACK: Must filter out wheel events, and scrollbar I think??
			//TODO: Prefer edittext_found_ip::c instead of active_control?
			follow_cursor = this==GLUI.active_control&&!GLUI.wheel_event;
		}
	}

	int box_width = get_box_width();
		
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _draw (TextBox)
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _draw (TextBox)
	//CAUTION: THIS MUST PRECISELY AGREE
	//WITH _draw (TextBox)

	const int se = sel_end;

	/* Find the line clicked. */		
	int sol = 0, eol = 0, line = 0, sel = 0;
	for(;;sol=eol,line++) 
	{
		if(line==sss_line) //update_substring_bounds?
		{
			if(stop_len==len) substring_start = sol;

			restart:; //Move scrollbar down?
		}

		//TRY TO MIRROR draw ALGORITHM HERE.

		int	tw,sw = 0; do
		{
			if(tw=substring_tab_width(sw,eol))
			{
				sw+=tw;

				if(substring_wrap_compare(sw,box_width))
				break;
			}
			else break; /* Skip newline */

		}while(++eol<len); //stop_len

		if(!tw) switch(text[eol]) //UNFINISHED
		{
		//case '\n': case '\r': case '\t':
		
		default: eol++; /* Skip newline */
		}
		else if(eol<len) //stop_len
		{
			eol = substring_wrap(sol,eol);

			//HACK: HOME gets stuck on wrapped lines without
			//some help. 1000 signals HOME.
			if(eol==se&&x==-1000) //GLUT_KEY_HOME?
			{
				if(find_pt&&line+1==stop_ln) stop_ln--;
			}
		} 
		
		if(stop_len==len) //update_substring_bounds?
		{
			if(line==sse_line) substring_end = eol;

			if(follow_cursor&&IN_BOUNDS(se,sol,eol-1)) 
			{
				sel = line; //update_substring_bounds

				if(sss_line>sel) //Move scrollbar up?
				{
					sse_line-=sss_line-sel;
					sss_line = sel; substring_start = sol;				
				}
				else if(sse_line<sel) //Move scrollbar down?
				{
					int restart = sss_line;
					sss_line+=sel-sse_line;
					sse_line = sel; substring_end = eol;
					//TODO: Some procedures for for finding
					//earlier/later lines are in order here.
					sol = eol = substring_start; line = restart;
					goto restart;					
				}
			}
		}

		if(eol>=stop_len //Beyond final line?
		||line==stop_ln) //Cap end of wrapped line?
		{	
			if(find_pt) stop_len = eol;
			
			eol = sol; break;
		}
	}

	// Now search to the end of this line for the closest insertion point

	int tw,sw;
	for(tw=sw=0;eol<stop_len;eol++)
	{
		if(tw=substring_tab_width(sw,eol))
		{
			sw+=tw;

			if(sw>=x) break;
		}
		else if('\n'==text[eol])
		{
			break; 
		}
	}
	sw-=tw; 
	if(eol==stop_len)
	{
		eol--;
	}

	// did we go far enough? (see if click was >1/2 width of last char)
	if(tw)
	{
		if(find_pt) 
		{
			int cmp = '\t'!=text[eol]?tw/2:tw-char_width(' ')/2;
			if(x>sw+cmp)
			{
				eol++; sw+=tw; //NOTE: eol may be get_char_count.
			}
		}
	}
	else sw++; //Set caret aside.
	
	if(stop_len>=len-1) //update_substring_bounds?
	{
		if(line<sse_line) 
		{
			substring_end = len;
		}			
		if(UI::ScrollBar*sb=scrollbar)
		{
			int bot = std::max(0,line+1-visible_lines);
			if(bot!=scrollbar_bot()) sb->set_range(bot,0);

			if(follow_cursor)
			{
				int lo = sb->int_val;
				int hi = sb->int_val+(visible_lines-1);
				if(sel<lo) lo = sel;
				if(sel>hi) lo+=(sel-hi);
				if(lo!=sb->int_val) sb->set_int_val(lo);
			}
		}
	}
			 
	*found_x = sw;
	*found_y = line*lh; return eol;
}

/*************************************** GLUI_EditText::_dump() **************/

#ifdef GLUI_DEBUG_STDOUT
void Text_Interface::_dump(const char *name)
{
	int len = get_char_count();
	fprintf(_dump_FILE,"%s(text@%p):  mod:%d  ins_pt:%d  subs:%d/%d  sel:%d/%d   len:%d\n",
	name,this,GLUI.curr_modifiers,GLUI.curr_caret_pt,substring_start,substring_end,sel_start,sel_end,len);
}
#endif

//---.
}//<-'
