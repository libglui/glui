
#ifndef GLUI_BUILD_EXAMPLES
#define GLUI_BUILD_EXAMPLES
#endif

#include "../../include/GL/glui.hpp"

int main(int argc, char* argv[])
{
	puts("Usage: example [Demonstration number]\n"
	"Demonstrations are numbered 1 (F1) through 6 (F6) "
	"and are loaded by their keys: F1, F2, F3, F4, F5, F6.");

	#ifdef _WIN32 //_MSC_VER
	extern void win_example(); win_example();
	#else
	extern bool freeglut_wheel(GLUI_Library::UI*,int,int,int,int);
	GLUI_Library::GLUI.mouse_callback = freeglut_wheel;
	#endif

	int demo = argc>1?*argv[1]-'0':1;
	if(demo<1||demo>6) demo = 1;
	return GLUI_Library::glui_example(demo,argc,argv);
}

#ifndef _WIN32
extern bool freeglut_wheel(GLUI_Library::UI *ui, int bt, int st, int x, int y)
{
	if(bt<=2) return true;
	
	if(!st) ui->wheel(4==bt?1:-1,x,y); return false;
}
#else
#include <Windows.h>
#include <Windowsx.h> //GET_X_LPARAM
//SetWindowSubclass
#include <commctrl.h> 
#pragma comment(lib,"Comctl32.lib")
static HHOOK win_hook = NULL;
static HWND win_created = NULL;
//This used to not be necessary before I switched icons???
static WPARAM win_icon = (WPARAM)
LoadIconA(GetModuleHandle(0),MAKEINTRESOURCEA(101)); //IDI_ICON1
static LRESULT CALLBACK win_proc(HWND hw, UINT msg, WPARAM w, LPARAM l, UINT_PTR, DWORD_PTR)
{
	switch(msg)
	{
 	case WM_MOUSEWHEEL:
	{
		GLUI_Library::UI *ui;
		(LONG_PTR&)ui = GetWindowLongPtr(hw,GWLP_USERDATA);
		if(!ui) break;

		short delta = GET_WHEEL_DELTA_WPARAM(w);
		assert(0==delta%WHEEL_DELTA);
		
		int cm = w&MK_SHIFT?ui->WHEEL_SHIFT:0; 
		if(w&MK_CONTROL) cm|=ui->WHEEL_CTRL;
		//if(w&MK_ALT) cm|=ui->WHEEL_HORZ;
		if(GetKeyState(VK_MENU)<0) cm|=ui->WHEEL_HORZ;
		POINT pt = {GET_X_LPARAM(l),GET_Y_LPARAM(l)};
		MapWindowPoints(0,hw,&pt,1);
		ui->wheel(delta/-WHEEL_DELTA,pt.x,pt.y,cm);

		break;
	}}		
	return DefSubclassProc(hw,msg,w,l);
}
static LRESULT CALLBACK win_hook_proc(int code, WPARAM w, LPARAM l)
{
	LRESULT out = CallNextHookEx(win_hook,code,w,l);
	if(code==HCBT_CREATEWND)
	{
		//Tried to use this to set owners, but it didn't go anywhere.
		CREATESTRUCTW *cs = ((CBT_CREATEWND*)l)->lpcs;
		if((ULONG_PTR)cs->lpszClass<=65535||wcscmp(cs->lpszClass,L"FREEGLUT"))
		{
			if(!cs->cx //Window_root?
			||!cs->lpszName||wcscmp(cs->lpszName,L"GLUT")) //IME, etc.
			return out;
		}

		win_created = (HWND)w;
		SetWindowSubclass(win_created,win_proc,0,0);				
		PostMessage(win_created,WM_SETICON,ICON_BIG,win_icon);
		PostMessage(win_created,WM_SETICON,ICON_SMALL,win_icon);
	}
	return out;
}
static void glui_create_cb(GLUI_Library::UI *ui)
{
	#ifdef _MSC_VER			
	ui->user = (void*)win_created;
	SetWindowLongPtr(win_created,GWL_USERDATA,(LONG_PTR)ui);
	#endif
}
extern void win_example()
{
	GLUI_Library::GLUI.create_callback = glui_create_cb;

	HWND cons = GetConsoleWindow();
	PostMessage(cons,WM_SETICON,ICON_BIG,win_icon);
	PostMessage(cons,WM_SETICON,ICON_SMALL,win_icon);
	win_hook = SetWindowsHookEx(WH_CBT,win_hook_proc,0,GetCurrentThreadId());
}
#endif
