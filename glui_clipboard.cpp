/****************************************************************************

  GLUI User Interface Toolkit
  ---------------------------

     glui_clipboard_x11.cpp - GLUI_Clipboard_Object class

	 This file -is- in the public domain. Circa summer 2017.

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

  This program is -not- in the public domain.

*****************************************************************************/

#include "glui_internal_control.h"

#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#else
#ifdef GLUI_GLX
#include <GL/glx.h>
//Xlib/X crazily pollute the global namespace.
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif
#endif

GLUI_Clipboard_Object GLUI_Clipboard("CLIPBOARD");
GLUI_Clipboard_Object GLUI_Selection("PRIMARY");

#ifdef GLUI_GLX
/**EXPERIMENTAL
 * This can stand to be rewritten.
 */
struct glui_clipboard_x11 
{
	static Display *d; static Window w;
	
	typedef std::vector<std::pair<long,std::string> >selection;
	static selection selected;
	
	//Lazy initialization. Unfortunately the first use 
	//can incur some delay.
	glui_clipboard_x11()
	{
		if(d!=nullptr) return;
		d = glXGetCurrentDisplay();
		if(d==nullptr) return;
		//This is opening a separate event channel from
		//freeglut's.
		d = XOpenDisplay(d==nullptr?nullptr:XDisplayString(d));
		if(d!=nullptr)		
		w = XCreateSimpleWindow(d,DefaultRootWindow(d),0,0,1,1,0,0,0);
	}
	/**
	 * This is concerned with hanging the system.
	 */
	static Bool predicate(Display *d2, XEvent *ev2, XPointer)
	{
		if(d!=d2)
		{
			//If this happens this is hopeless, since freeglut will
			//be receiving d's events and it will happily ignore them.
			assert(0); return 0;
		}
		if(w==ev2->xany.window) switch(ev2->type)
		{				
		default:
        
			//This is really stupid, but XErrorEvent.type is undocumented.
			if(ev2->type<2||ev2->type>=LASTEvent)
			{
				assert(0);
			}
			else break;

		case SelectionNotify: case DestroyNotify: return 1;
		}
		return 0;
	}	
};
Display *glui_clipboard_x11::d = nullptr;
Window glui_clipboard_x11::w = 0;
glui_clipboard_x11::selection glui_clipboard_x11::selected; 
#endif

GLUI_Clipboard_Object::GLUI_Clipboard_Object(const char *name)
:name(name)
{}

void GLUI_Clipboard_Object::set_text(const char *sel, size_t len)
{
	if(len==0) return;
	
	if(this!=&GLUI_Clipboard)
	{
		if(0!=name.compare("CLIPBOARD")) return;
	}

	#ifdef _WIN32
	if(0==OpenClipboard(0))
	return;
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE,len+1);	
	((char*)memcpy(GlobalLock(h),sel,len))[len] = '\0';
	GlobalUnlock(h);	
	EmptyClipboard();
	if(0==SetClipboardData(CF_TEXT,h))
	GlobalFree(h);
	CloseClipboard();	  
	#endif
	
	#ifdef GLUI_GLX		
	glui_clipboard_x11 p; 
    if(p.w==0) return;
	Atom xa = XInternAtom(p.d,name.c_str(),0);
	XSetSelectionOwner(p.d,xa,p.w,CurrentTime);
	for(size_t i=0;i<p.selected.size();i++)
	if(xa==p.selected[i].first)
	{
		p.selected[i].second.assign(sel,len);
		return;
	}	
	p.selected.push_back(std::make_pair(xa,""));
	p.selected.back().second.assign(sel,len);	
	#endif
}
extern void glui_clipboard_try_SelectionRequest()
{
	#ifdef GLUI_GLX	
	if(nullptr==glui_clipboard_x11::d) 
	return; 	
	XEvent ev,ev2;
	glui_clipboard_x11 p;
	for(int i=XPending(p.d);i-->0;)
	{
		XNextEvent(p.d,&ev);
		if(ev.type!=SelectionRequest)
		continue;
		
		enum{ xa_targetsN=4};
		static Atom xa_targets[1+xa_targetsN] = 
		{
			XInternAtom(p.d,"XA_TARGETS",0),			
			XInternAtom(p.d,"UTF8_STRING",0),
			XInternAtom(p.d,"XA_STRING",0),
			XInternAtom(p.d,"COMPOUND_TEXT",0), //Xming
			XInternAtom(p.d,"TEXT",0), //Mentioned in ICCCM manual.
		};
			
		Atom xa = ev.xselectionrequest.target;		
		ev2.xselection.type = SelectionNotify;
		ev2.xselection.requestor = ev.xselectionrequest.requestor;
		ev2.xselection.selection = ev.xselectionrequest.selection;
		ev2.xselection.target = xa;
		ev2.xselection.property = None; 
		ev2.xselection.time = ev.xselectionrequest.time;
				
		if(xa==xa_targets[0])
		{
			XChangeProperty(p.d,
			ev2.xselection.requestor,
			ev2.xselection.property=ev.xselectionrequest.property,
			xa,CHAR_BIT*sizeof(Atom),PropModeReplace,
			reinterpret_cast<const unsigned char*>(xa_targets+1),xa_targetsN);
		}
		else for(int i=1;i<=xa_targetsN;i++) if(xa==xa_targets[i])
		{
			for(size_t i=0;i<p.selected.size();i++)
			if(p.selected[i].first==ev2.xselection.selection)
			{
				XChangeProperty(p.d,
				ev2.xselection.requestor,
				ev2.xselection.property=ev.xselectionrequest.property,
				xa,8,PropModeReplace,
				reinterpret_cast<const unsigned char*>(p.selected[i].second.c_str()),
				p.selected[i].second.size());
			}		
		}
		
		if(ev2.xselection.property==None)
		{
			#ifndef NDEBUG
			char *mystery = XGetAtomName(p.d,xa);
			if(mystery!=nullptr) printf(mystery);
			XFree(mystery);
			#endif
		}
		
		XSendEvent(p.d,ev2.xselection.requestor,1,0,&ev2);
	}	
	#endif
}

template<class T>
static void glui_clipboard_urlencode(GLUI_String &container, T *p)
{
	if(p!=nullptr)	
	for(int len;*p!=L'\0';p++) if((unsigned)*p>127)
	{
		char utf8[4],hex[3];
		#ifndef _WIN32
		len = 1; utf8[0] = *p; 
		#else
		len = WideCharToMultiByte(CP_UTF8,0,p,1,utf8,4,0,0);
		#endif
		for(int i=0;i<len;i++)
		{
			//GLUI doesn't display Unicode, so treating like a URL
			//until it does so.
			container.push_back('%');
			snprintf(hex,3,"%02x",(unsigned char)utf8[i]); //itoa
			container.push_back(hex[0]);
			container.push_back(hex[1]);
		}
	}
	else container.push_back((char)*p);
}

bool GLUI_Clipboard_Object::get_text(GLUI_String &container)
{
	container.clear();

	if(this!=&GLUI_Clipboard)
	{
		if(0!=name.compare("CLIPBOARD")) return false;
	}

	#ifdef _WIN32
	if(0==OpenClipboard(0))
	return false;
	HGLOBAL h = GetClipboardData(CF_UNICODETEXT);
	if(h==nullptr)
	return false;
	wchar_t *p = (wchar_t*)GlobalLock(h);
	container.reserve(wcslen(p));
	glui_clipboard_urlencode(container,p);	
	GlobalUnlock(h);
	CloseClipboard(); 
	#endif

	#ifdef GLUI_GLX	
	glui_clipboard_x11 p;
    if(p.w==0) return false;	
	//I tried a lot to get this to work with Xming. I looked at the few examples
	//online, and it seems very tempermental (when it should not be.)
	//So unless it's not working, I guess leave it be.
	Atom xa = XInternAtom(p.d,name.c_str(),0),unused1;
	if(p.w==XGetSelectionOwner(p.d,xa))
	{
		//Single-threaded.
		for(size_t i=0;i<p.selected.size();i++)
		if(xa==p.selected[i].first)
		container = p.selected[i].second;
		return true;
	}
    if(0==XConvertSelection(p.d,xa,XInternAtom(p.d,"UTF8_STRING",0),xa,p.w,CurrentTime))
	return false;
    XFlush(p.d); 
	unsigned long partial_content=0,content,char_remains;
    for(char_remains=1;char_remains!=0;) 
	{
		XEvent ev;
		XIfEvent(p.d,&ev,p.predicate,nullptr);
        if(SelectionNotify==ev.type)
		if(None!=ev.xselection.property) //Causes problems.
		{
			//xa does not equal ev.xselection.property. Maybe it would work either way.
			unsigned char *utf8_string; int unused2;
            if(0!=XGetWindowProperty
			(p.d,p.w,ev.xselection.property,partial_content,INT_MAX,0,0
			,&unused1,&unused2,&content,&char_remains,&utf8_string))
			goto error;
            container.reserve(partial_content/sizeof(char)*char_remains);
			assert(utf8_string==nullptr||'\0'==utf8_string[content]);
			glui_clipboard_urlencode(container,utf8_string);
            partial_content+=content;
			XFree(utf8_string);
		}
		else error: char_remains = 0;
    }    
	#endif

	return !container.empty();
}
