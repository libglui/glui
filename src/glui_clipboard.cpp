
#include "../include/glui_internal.h" //PCH

//#include <limits>
//#include <vector>

//Assuming glui_internal.h includes these.
//#ifdef _WIN32
//#include <windows.h>
//#endif
#ifdef GLUI_GLX
//#include <GL/glx.h>
//Xlib/X crazily pollute the global namespace.
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

namespace GLUI_Library
{//-.
//<-'

class Clipboard Clipboard("CLIPBOARD"), Selection("PRIMARY");

#ifdef GLUI_GLX
/**EXPERIMENTAL
 * This can stand to be rewritten.
 */
struct glui_clipboard_x11 
{
	static ::Display *d;

	static ::Window w; //Window is unsigned long type.
	
	typedef std::vector<std::pair<long,std::string>/**/>selection;
	static selection selected;

	bool try_to_close()
	{
		if(d&&selected.empty())
		{
			XCloseDisplay(d); d = NULL; w = 0; //NULL
		}
		return !d;
	}
	
	//Lazy initialization. Unfortunately the first use 
	//can incur some delay.
	glui_clipboard_x11()
	{
		if(d) return;
		d = glXGetCurrentDisplay();
		if(!d) return;
		//This is opening a separate event channel from
		//freeglut's.
		d = XOpenDisplay(!d?NULL:XDisplayString(d));
		if(d)		
		w = XCreateSimpleWindow(d,DefaultRootWindow(d),0,0,1,1,0,0,0);
	}
	/**
	 * This is concerned with hanging the system.
	 */
	static Bool predicate(::Display *d2, XEvent *ev2, XPointer)
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
::Display *glui_clipboard_x11::d = NULL;
::Window glui_clipboard_x11::w = 0;
glui_clipboard_x11::selection glui_clipboard_x11::selected; 
static void glui_clipboard_try_SelectionRequest(int ms=250)
{
	#ifndef _DEBUG
	#error Maybe use w instead of d here? No: XPending
	#endif
	#ifdef GLUI_GLX	
	if(!glui_clipboard_x11::d) 
	return; 	
	::Atom xa;
	XEvent ev,ev2;
	glui_clipboard_x11 p; 
	//Doing after loop in case debugger or something jams up.
	//glutSetTimer(250,glui_clipboard_try_SelectionRequest,0);
	for(int i=XPending(p.d);i-->0;)
	{
		XNextEvent(p.d,&ev);
		switch(ev.type)
		{
		case SelectionClear: //NEW/UNTESETD		
			xa = ev.xselectionclear.selection;
			for(size_t i=0;i<p.selected.size();i++)
			if(xa==p.selected[i].first)
			{
				p.selected.erase(p.selected.begin()+i);
				if(p.try_to_close())
				return;
				break;
			}		 
		default: continue;		
		case SelectionRequest: break;
		}
		
		enum{ xa_targetsN=4};
		static ::Atom xa_targets[1+xa_targetsN] = 
		{
			XInternAtom(p.d,"XA_TARGETS",0),			
			XInternAtom(p.d,"UTF8_STRING",0),
			XInternAtom(p.d,"XA_STRING",0),
			XInternAtom(p.d,"COMPOUND_TEXT",0), //Xming
			XInternAtom(p.d,"TEXT",0), //Mentioned in ICCCM manual.
		};
			
		xa = ev.xselectionrequest.target;		
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
			xa,CHAR_BIT*sizeof(::Atom),PropModeReplace,
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
			if(mystery) puts(mystery);
			XFree(mystery);
			#endif
		}
		
		XSendEvent(p.d,ev2.xselection.requestor,1,0,&ev2);
	}
	glutTimerFunc(ms,glui_clipboard_try_SelectionRequest,ms);
	#endif
}
#endif
void Clipboard::set_text(C_String sel, size_t len)
{
	if(len==~size_t()) len = strlen(sel.str);

	if(len-1>INT_MAX/2)
	{
		assert(!len);		
		//X11 has new clearing logic for empty strings.
		#ifdef _WIN32
		return; 
		#endif
		if(len!=0) return;
	}
	else if(!sel.str) return;

	#ifdef _WIN32
	if(this!=&GLUI_Library::Clipboard)
	{
		if(name.compare("CLIPBOARD")) return;
	}
	if(0==::OpenClipboard(0))
	return;
	::HGLOBAL h = ::GlobalAlloc(GMEM_MOVEABLE,len+1);	
	((char*)memcpy(::GlobalLock(h),sel.str,len))[len] = '\0';
	::GlobalUnlock(h);	
	::EmptyClipboard();
	if(0==::SetClipboardData(CF_TEXT,h))
	::GlobalFree(h);
	::CloseClipboard();	  
	#endif
	
	#ifdef GLUI_GLX
	glui_clipboard_x11 p; 
	if(p.w==0) return;
	bool install_timer = p.selected.empty();
	::Atom xa = XInternAtom(p.d,name.c_str(),0);
	//NEW: I'm not 100% certain, but I think None
	//means no selection. So I'm using this for an
	//empty string.
	::Window owner = sel.str&&len?p.w:None;
	XSetSelectionOwner(p.d,xa,owner,CurrentTime);
	for(size_t i=0;i<p.selected.size();i++)
	if(xa==p.selected[i].first)
	{
		if(!owner)
		{
			p.selected.erase(p.selected.begin()+i);
			p.try_to_close();
			return;
		}
		p.selected[i].second.assign(sel.str,len);
		break;
	}	
	if(!owner) return;
	p.selected.push_back(std::make_pair(xa,""));
	p.selected.back().second.assign(sel.str,len);
	//NEW: Not using GLUI idle to serve selection.
	//Hopefully it won't get congested.	
	if(install_timer)
	glui_clipboard_try_SelectionRequest();
	#endif
}

template<class T>
static void glui_clipboard_urlencode(String &container, T *p)
{
	if(p) for(int len;*p!=L'\0';p++) if((unsigned)*p>127)
	{
		char utf8[4],hex[3];
		#ifndef _WIN32
		len = 1; utf8[0] = *p; 
		#else
		len = ::WideCharToMultiByte(CP_UTF8,0,p,1,utf8,4,0,0);
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

bool Clipboard::get_text(String &container)
{
	container.clear();

	if(this!=&GLUI_Library::Clipboard)
	{
		if(0!=name.compare("CLIPBOARD")) return false;
	}

	#ifdef _WIN32
	if(0==::OpenClipboard(0))
	return false;

	::HGLOBAL h = ::GetClipboardData(CF_UNICODETEXT);
	if(!h) return false;

	wchar_t *p = (wchar_t*)::GlobalLock(h);
	container.reserve(wcslen(p));
	glui_clipboard_urlencode(container,p);	
	::GlobalUnlock(h);
	::CloseClipboard(); 
	#endif

	#ifdef GLUI_GLX	
	glui_clipboard_x11 p;
	if(p.w==0) return false;	
	//I tried a lot to get this to work with Xming. I looked at the few examples
	//online, and it seems very tempermental (when it should not be.)
	//So unless it's not working, I guess leave it be.
	::Atom xa = XInternAtom(p.d,name.c_str(),0),unused1;
	if(p.w==XGetSelectionOwner(p.d,xa))
	{
		//Single-threaded.
		for(size_t i=0;i<p.selected.size();i++)
		if(xa==p.selected[i].first)
		container = p.selected[i].second;

		close: //return true;
		{
			p.try_to_close();
			return !container.empty();
		}
	}
	if(!XConvertSelection(p.d,xa,XInternAtom(p.d,"UTF8_STRING",0),xa,p.w,CurrentTime))
	{
		goto close; //return false;
	}
	XFlush(p.d); 
	unsigned long partial_content=0,content,char_remains;
	for(char_remains=1;char_remains!=0;) 
	{
		XEvent ev;
		XIfEvent(p.d,&ev,p.predicate,NULL);
        if(SelectionNotify==ev.type
		&&None!=ev.xselection.property) //Causes problems.
		{
			//xa does not equal ev.xselection.property. Maybe it would work either way.
			unsigned char *utf8_string; int unused2;
           	if(XGetWindowProperty
			(p.d,p.w,ev.xselection.property,partial_content,INT_MAX,0,0,
			&unused1,&unused2,&content,&char_remains,&utf8_string))
			goto error;
            container.reserve(partial_content/sizeof(char)*char_remains);
			assert(!utf8_string||'\0'==utf8_string[content]);
			glui_clipboard_urlencode(container,utf8_string);
            partial_content+=content;
			XFree(utf8_string);
		}
		else error: char_remains = 0;
	}    
	goto close;
	#endif

	return !container.empty();
}

//---.
}//<-'
