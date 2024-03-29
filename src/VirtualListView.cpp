#include "VirtualListView.h"

#include <commctrl.h>
#include <windowsx.h>
#include <aygshell.h>
#include "utf8.hpp"

#include "wmuser.h"
#include "TabCtrl.h"    
extern TabsCtrlRef tabs;
extern HINSTANCE			g_hInst;
#define VK_3 0x33
#define VK_9 0x39
extern int tabHeight;
extern int COLORS[];
ATOM VirtualListView::RegisterWindowClass() {
    WNDCLASS wc;

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc   = VirtualListView::WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hInst;
    wc.hIcon         = NULL;
    wc.hCursor       = 0;
    wc.hbrBackground = NULL;//(HBRUSH)COLOR_WINDOW;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = _T("BombusVLV");

    return RegisterClass(&wc);
}


LRESULT CALLBACK VirtualListView::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    VirtualListView *p=(VirtualListView *) GetWindowLong(hWnd, GWL_USERDATA);
 int klav=0;
    switch (message) {
    case WM_CREATE:
        {
            p=(VirtualListView *) (((CREATESTRUCT *)lParam)->lpCreateParams);
            SetWindowLong(hWnd, GWL_USERDATA, (LONG) p );

            //p->listScrollHWND=CreateWindow(_T("SCROLLBAR"), NULL, 
            //    SBS_VERT | WS_VISIBLE | WS_CHILD,
            //    0, 0, CW_USEDEFAULT, CW_USEDEFAULT, 
            //    hWnd, NULL, g_hInst, NULL); 

            break;

        }

    case WM_PAINT:

        {
            PAINTSTRUCT ps;
            HDC hdc;
            HDC wnd;
            wnd = BeginPaint(hWnd, &ps);

            HBITMAP buf=CreateCompatibleBitmap(wnd, p->clientRect.right, p->clientRect.bottom);

            hdc=CreateCompatibleDC(NULL);
            SelectObject(hdc, buf);

            PatBlt(hdc, 0, 0, p->clientRect.right, p->clientRect.bottom, WHITENESS);
            // TODO: Add any drawing code here...

            SetBkMode(hdc, OPAQUE);

            int y=-p->winTop;
            int index=0;

            if (p->odrlist.get()) {
                ODRList::const_iterator i=p->odrlist->begin();
                while (i!=p->odrlist->end()) {
                    ODRRef odr=*i; i++; 
                    bool oddIndex=(index & 1) && p->colorInterleaving;
                    index++;

                    bool focused = (odr.get()==p->cursorPos.get());

                    RECT ritem={0, y, p->clientRect.right, y} ;
                    odr->measure(hdc, ritem);
                    //int iHeight=odr->getHeight();
                    //y+=iHeight;
                    y=ritem.bottom;
                    //index++;

                    if (ritem.bottom < 0) continue;
                    if (ritem.top > p->clientRect.bottom) continue;

                    int bkColor=COLORS[12];
                    if (focused) {
                        // focused item
                        SetTextColor(hdc, COLORS[8]);
                        bkColor=(GetFocus()==hWnd)?COLORS[9]:COLORS[10];
                        //DrawFocusRect(hdc, &ritem);
                    } else {
                        //usual item
                        SetTextColor(hdc, odr->getColor());
                        bkColor= (oddIndex)? COLORS[11] : COLORS[12];
                    }
                    HBRUSH bkBrush=CreateSolidBrush(bkColor);
                    SetBkColor(hdc, bkColor);
                    ritem.right=p->clientRect.right; //full window-wide cursor
                    FillRect(hdc, &ritem, bkBrush);
                    DeleteObject(bkBrush);
                    odr->draw(hdc, ritem);
                }
            }

            RECT rc = {0, 0, 100, 100};

            SCROLLINFO si;
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_PAGE |SIF_POS |SIF_RANGE;
            si.nPage=p->clientRect.bottom;
            si.nPos=p->winTop;
            si.nMin=0;
            si.nMax=y+p->winTop;

            SetScrollInfo(p->thisHWnd, SB_VERT, &si, TRUE);

            /*int titleBgnd=0x808080;
            HBRUSH tb=CreateSolidBrush(titleBgnd);
            SetBkColor(hdc, titleBgnd);
            SetTextColor(hdc, 0x000000);

            LPCTSTR t=p->title.c_str();
            DrwText(hdc, t, -1, &rc, DT_CALCRECT | DT_LEFT | DT_TOP);
            rc.right=p->clientRect.right;
            FillRect(hdc, &rc, tb);
            DrwText(hdc, t, -1, &rc, DT_LEFT | DT_TOP);
            DeleteObject(tb);*/

            BitBlt(wnd, 0,0,p->clientRect.right, p->clientRect.bottom, hdc, 0,0, SRCCOPY);
            DeleteDC(hdc);
            DeleteObject(buf);

            EndPaint(hWnd, &ps);
            break;
        }

    case WM_SIZE: 
        { 
            //HDWP hdwp; 

            int height=GET_Y_LPARAM(lParam);
            int width=GET_X_LPARAM(lParam);
            // Calculate the display rectangle, assuming the 
            // tab control is the size of the client area. 
            SetRect(&(p->clientRect), 0, 0, width, height ); 

            /*hdwp = BeginDeferWindowPos(1);

            DeferWindowPos(hdwp, p->listScrollHWND, HWND_TOP, width-SCROLLWIDTH, tabHeight, 
            SCROLLWIDTH, height-tabHeight, 
            SWP_NOZORDER 
            );
            EndDeferWindowPos(hdwp); */

            p->cursorFit();
            SCROLLINFO si;
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_PAGE |SIF_POS;
            si.nPage=height;
            si.nPos=p->winTop;

            SetScrollInfo(p->thisHWnd, SB_VERT, &si, TRUE);

            break; 
        } 

    case WM_LBUTTONDOWN:
        {
            SetFocus(hWnd);
            ODRRef focused=p->moveCursorTo(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            if (!(focused)) break;
            InvalidateRect(p->getHWnd(), NULL, true);

            SHRGINFO    shrg;
            shrg.cbSize = sizeof(shrg);
            shrg.hwndClient = hWnd;
            shrg.ptDown.x = LOWORD(lParam);
            shrg.ptDown.y = HIWORD(lParam);
            shrg.dwFlags = SHRG_RETURNCMD /*| SHRG_NOANIMATION*/;

            if (SHRecognizeGesture(&shrg) == GN_CONTEXTMENU) {

                HMENU hmenu = p->getContextMenu();

                VirtualListElement *velement=dynamic_cast<VirtualListElement *>(focused.get());
                if (velement) hmenu=velement->getContextMenu(hmenu);

                if (hmenu==NULL) break;

                POINT pt={LOWORD(lParam), HIWORD(lParam) };
                ClientToScreen(hWnd, &pt);
                int cmdId=TrackPopupMenuEx(hmenu,
                    /*TPM_LEFTALIGN |*/ TPM_TOPALIGN | TPM_RETURNCMD, 
                    pt.x, pt.y,
                    hWnd,
                    NULL);

                bool cmdProcessed=false;
                if (velement) 
					cmdProcessed=velement->OnMenuCommand(cmdId, p->getHWnd(), p->hEditBox);

                if (!cmdProcessed)
                    p->OnCommand(cmdId, NULL);

                DestroyMenu(hmenu);
            }
            break;
        }
    case WM_LBUTTONDBLCLK:
        {
            ODRRef oldCursor=p->cursorPos;
            ODRRef focused=p->moveCursorTo(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            if (!(focused)) break;
            InvalidateRect(p->getHWnd(), NULL, true);
            if (focused==oldCursor) p->eventOk();
            break;
        }

    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        {
            InvalidateRect(p->getHWnd(), NULL, true);
            break;
        }
    case WM_KEYDOWN:
        {bool kl2=1;
          int vKey=(int)wParam;
            int lkeyData=lParam;
            if (lkeyData & 0x80000000) break; //keyRelease 
            switch (vKey) {
                        /* UFO START */
                        
                        case VK_RIGHT:
                                PostMessage(tabs->getHWnd(), WM_COMMAND, TabsCtrl::NEXTTAB, 0);
                                break;

						case VK_3:kl2=0;
                              //  MessageBox(hWnd, TEXT("Clicked 3"), TEXT("!"), 0);
                               SCROLLINFO si2;
            si2.cbSize=sizeof(SCROLLINFO);
            si2.fMask=SIF_ALL;

            GetScrollInfo(p->thisHWnd, SB_VERT, &si2);

            //TODO: flicker-free scrolling
          
           /* case SB_LINEDOWN: *///  si2.nPos+=tabHeight; break;
            //case SB_LINEUP:    
			si2.nPos-=tabHeight; 
            //case SB_ENDSCROLL:  break;
            /*//case SB_PAGEUP:    */// si2.nPos-=si2.nPage; 
            //case SB_PAGEDOWN:   si2.nPos+=si2.nPage;  break;
           
      


            if (si2.nPos<0) si2.nPos=0; 
            if (si2.nPos+(int)si2.nPage >= si2.nMax) si2.nPos=si2.nMax-si2.nPage; 

            p->winTop= si2.nPos;

            //TODO: flicker-free scrolling
            InvalidateRect(p->getHWnd(), NULL, true);

            si2.fMask=SIF_POS;
            SetScrollInfo(p->thisHWnd, SB_VERT, &si2, TRUE); 
							break;
								
						case VK_9:kl2=0;
                                SCROLLINFO si3;
            si3.cbSize=sizeof(SCROLLINFO);
            si3.fMask=SIF_ALL;

            GetScrollInfo(p->thisHWnd, SB_VERT, &si3);

            //TODO: flicker-free scrolling
          
           /* case SB_LINEDOWN: *///  
			si3.nPos+=tabHeight; 
            //case SB_LINEUP:    si2.nPos-=tabHeight; 
            //case SB_ENDSCROLL:  break;
            /*//case SB_PAGEUP:    */// si2.nPos-=si2.nPage; 
            //case SB_PAGEDOWN:   si2.nPos+=si2.nPage;  break;
           
      


            if (si3.nPos<0) si3.nPos=0; 
            if (si3.nPos+(int)si3.nPage >= si3.nMax) si3.nPos=si3.nMax-si3.nPage; 

            p->winTop= si3.nPos;

            //TODO: flicker-free scrolling
            InvalidateRect(p->getHWnd(), NULL, true);

            si3.fMask=SIF_POS;
            SetScrollInfo(p->thisHWnd, SB_VERT, &si3, TRUE); 
							break;
							// MessageBox(hWnd, TEXT("Clicked 3"), TEXT("!"), 0);
                               
                        case VK_LEFT:
                                PostMessage(tabs->getHWnd(), WM_COMMAND, TabsCtrl::PREVTAB, 0);
                                break;
                        /* UFO END */
                        case VK_UP:
                p->moveCursor(-1);
                break;
				case VK_DOWN: 
                p->moveCursor(1);
                break;

           


                        case VK_RETURN:
                if (lkeyData &0xc0000000) break;
                p->eventOk();
                                break;
			}
            if(kl2)p->cursorFit();
            InvalidateRect(p->getHWnd(), NULL, true);


            SCROLLINFO si;
            si.cbSize=sizeof(SCROLLINFO);
            si.nPos=p->winTop;
            si.fMask=SIF_POS;
            SetScrollInfo(p->thisHWnd, SB_VERT, &si, TRUE);


            break;
        }
    case WM_VSCROLL:
        {
            int scrollCode=(int)LOWORD(wParam);
            int nPos=(int)HIWORD(wParam);

            SCROLLINFO si;
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_ALL;

            GetScrollInfo(p->thisHWnd, SB_VERT, &si);

            //TODO: flicker-free scrolling
            switch (scrollCode) {
            case SB_LINEDOWN:   si.nPos+=tabHeight; break;
            case SB_LINEUP:     si.nPos-=tabHeight; break;
            case SB_ENDSCROLL:  break;
            case SB_PAGEUP:     si.nPos-=si.nPage;  break;
            case SB_PAGEDOWN:   si.nPos+=si.nPage;  break;
            case SB_THUMBTRACK:
            case SB_THUMBPOSITION: si.nPos=si.nTrackPos; break;
            //default:            si.nPos=si.nTrackPos; break;
            }


            if (si.nPos<0) si.nPos=0; 
            if (si.nPos+(int)si.nPage >= si.nMax) si.nPos=si.nMax-si.nPage; 

            p->winTop= si.nPos;

            //TODO: flicker-free scrolling
            InvalidateRect(p->getHWnd(), NULL, true);

            si.fMask=SIF_POS;
            SetScrollInfo(p->thisHWnd, SB_VERT, &si, TRUE);
            return true;

        }

    case WM_COMMAND: 
        {
            int cmdId=LOWORD(wParam);
            p->OnCommand(cmdId, lParam);
            break;
        }

    case WM_VIRTUALLIST_REPLACE:
        {
            //TODO: create interconnecting message object to avoid pointers
            ODRListRef r=ODRListRef((ODRList *)lParam);
            p->bindODRList(r); //������ �5

            //p->bindODRList(ODRSet::ref((ODRSet *)lParam)); //������ �3
            p->notifyListUpdate(true);
            break;
        }

    case WM_DESTROY:
        //TODO: Destroy all child data associated eith this window

        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


ODRRef VirtualListView::moveCursorTo( int x, int y ) 
{
    y+=winTop;
    if (y<0) return ODRRef();
    if (!odrlist) return ODRRef();

    int yTop=0;

    for (ODRList::const_iterator i = odrlist->begin(); i!=odrlist->end(); i++ ) {
        int yBot=yTop+i->get()->getHeight();

        if (yTop<=y) {
            if (yBot>y) {
                cursorPos=*i;

                cursorFit();
                return cursorPos;
            }
        }
        yTop=yBot;
    }
    return ODRRef();
}

bool VirtualListView::moveCursorEnd() {
    if (odrlist.get()==NULL) return false;
    if (odrlist->empty()) {
        cursorPos=ODRRef();
    } else {
        cursorPos=odrlist->back();
    }
    cursorFit();
    return true;
}

bool VirtualListView::cursorAtEnd() {
    if (odrlist.get()==NULL) return false;
    if (odrlist->empty()) return false;
    return cursorPos==odrlist->back();
}


void VirtualListView::cursorFit() {
    if (!cursorPos) return;

    int yTop=0;
    for (ODRList::const_iterator i = odrlist->begin(); i!=odrlist->end(); i++) {
        int yBot=yTop+ i->get()->getHeight();

        if ( (*i).get()==cursorPos.get() ) {
            if (yBot>winTop+clientRect.bottom) winTop=yBot-(clientRect.bottom);
            if (yTop<winTop) winTop=yTop;
            //break;
        }
        yTop=yBot;
    }
    //finally: align list bottom with window bottom, if list bottom is visile and list is long enough
    int listHeight=yTop;
    // test if window bottom is lower than list bottom
    if (winTop+clientRect.bottom>listHeight) winTop=listHeight-clientRect.bottom;
    // test if window top is not upper than list top
    if (winTop<0) winTop=0;
}


VirtualListView::VirtualListView()  : colorInterleaving(FALSE)
{ /*init(); - MUST NOT be called before setting up parentHWnd */ }

void VirtualListView::init() {
    BOOST_ASSERT(parentHWnd);

    if (windowClass==0)
        windowClass=RegisterWindowClass();
    if (windowClass==0) throw std::exception("Can't create window class");
    thisHWnd=CreateWindow((LPCTSTR)windowClass, _T("VList"), WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHWnd, NULL, g_hInst, (LPVOID)this);
    wrapList=true;
    colorInterleaving=false;
    winTop=0;
}

VirtualListView::VirtualListView( HWND parent, const std::string & title ) {
	hEditBox = NULL;
    parentHWnd=parent;
    init();

    SetParent(thisHWnd, parent);

    this->title=utf8::utf8_wchar(title);

    wt=WndTitleRef(new WndTitle(this, 0));
    cursorPos=ODRRef();//odrlist->front();
    odrlist=ODRListRef(new ODRList());
}

const wchar_t * VirtualListView::getWindowTitle() const{
    return title.c_str();
}

VirtualListView::~VirtualListView() {}

const ODR * VirtualListView::getODR() const { return wt.get(); }

void VirtualListView::notifyListUpdate( bool redraw ) {
    if (!odrlist) return;
    if (!redraw) return;
    /*int lastY=0;
    for (ODRList::const_iterator i = odrlist->begin(); i!=odrlist->end(); i++) {
        lastY+= i->get()->getHeight();
    }

    SCROLLINFO si;
    si.cbSize=sizeof(SCROLLINFO);
    si.fMask=SIF_PAGE | SIF_RANGE;
    si.nPage=clientRect.bottom;
    si.nMin=0;
    si.nMax=lastY;

    SetScrollInfo(thisHWnd, SB_VERT, &si, TRUE);*/

    InvalidateRect(getHWnd(), NULL, true);
}

void VirtualListView::eventOk() { 
    VirtualListElement *velement=dynamic_cast<VirtualListElement *>(cursorPos.get());
    if (velement) 
        velement->OnMenuCommand(IDOK, getHWnd(), NULL);
}

void VirtualListView::moveCursor( int direction ) {
    for (ODRList::const_iterator i=odrlist->begin(); i!=odrlist->end(); i++) {
        ODRRef r=*i;
        if (r.get()==cursorPos.get()) {
            if (direction>0) {
                i++;
                if (i==odrlist->end()) {
                    if (wrapList) i=odrlist->begin();
                    else i--;
                }
            } else {
                if (i==odrlist->begin()) {
                    if (wrapList) i=odrlist->end(); 
                    else return;
                }
                i--;
            }
            cursorPos=*i;
            return;
        }
    }
    if (odrlist->size()) cursorPos=odrlist->front();
}

void VirtualListView::addODR( ODRRef odr, bool redraw ) {
    odrlist->push_back(odr);
    notifyListUpdate(redraw);
}

HMENU VirtualListView::getContextMenu() { return NULL; }
void VirtualListView::OnCommand( int cmdId, LONG lParam ) {};

void VirtualListView::setCursorPos( ODRRef newPos ) {
    if (!odrlist) return;
    cursorPos=newPos;
    cursorFit();
}

ATOM VirtualListView::windowClass=0;

