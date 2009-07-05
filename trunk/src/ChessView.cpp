#include "ChatView.h"
#include <utf8.hpp>
#include <commctrl.h>
#include <windowsx.h>
#include <aygshell.h>
#include <sipapi.h>
#include "ChessView.h"
#include "../vs2005/ui/ui.h"

#include "Sysinfo.h"
#include "wmuser.h"
#include "ResourceContext.h"
#include "JabberStream.h"
#include "JabberAccount.h"
#include "TabCtrl.h"
#include "ProcessMUC.h"

#include "Smiles.h"
#include "History.h"

#include "LastActivity.h"

#include "config.h"

#include "stringutils.h"
#include "VcardForm.h"	
#include <string.h>
#include "Log.h"
#include <windows.h> 
#include <Wingdi.h> 
extern std::wstring appRootPath;
extern HINSTANCE			g_hInst;
extern int tabHeight;
extern HWND	g_hWndMenuBar;		// menu bar handle
extern ResourceContextRef rc;
extern ImgListRef skin;
extern SmileParser *smileParser;
extern HWND		mainWnd;
extern TabsCtrlRef tabs;			/* to vCard ICO */
extern LONG timstatus;
extern int idautostatus;
//////////////////////////////////////////////////////////////////////////
// WARNING!!! ONLY FOR WM2003 and higher
//////////////////////////////////////////////////////////////////////////
#ifndef DT_END_ELLIPSIS
#define DT_END_ELLIPSIS 0x00008000
#endif

ATOM ChessView::windowClass=0;

ATOM ChessView::RegisterWindowClass() {
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = ChessView::WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hInst;
    wc.hIcon         = NULL;
    wc.hCursor       = 0;
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = _T("BombusChessV");

    return RegisterClass(&wc);
}

ChessView::ChessView( HWND parent, Contact::ref contact ) 
{
    BOOST_ASSERT(parent);

    if (windowClass==0)
        windowClass=RegisterWindowClass();
    if (windowClass==0) throw std::exception("Can't create window class");

    parentHWnd=parent;

    this->contact=contact;

    thisHWnd=CreateWindow((LPCTSTR)windowClass, _T("chat"), WS_CHILD |WS_VISIBLE,
        0, 0, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		parent, NULL, g_hInst, (LPVOID)this);
}

LRESULT CALLBACK ChessView::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
    HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// тута рисовать
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

const ODR * ChessView::getODR() const
{
	return contact.get();
}
