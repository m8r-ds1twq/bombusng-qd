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
std::wstring filePathCHESS;
extern HINSTANCE			g_hInst;
extern int tabHeight;
extern HWND	g_hWndMenuBar;		// menu bar handle
extern ResourceContextRef rc;

extern SmileParser *smileParser;
extern HWND		mainWnd;
extern TabsCtrlRef tabs;			/* to vCard ICO */
#define RAZMER 30
#define RAZMERSKIN 32
//////////////////////////////////////////////////////////////////////////
// WARNING!!! ONLY FOR WM2003 and higher
//////////////////////////////////////////////////////////////////////////
#ifndef DT_END_ELLIPSIS
#define DT_END_ELLIPSIS 0x00008000
#endif

HBITMAP bmpc;
BITMAP bmc;
HDC hdcSKIN;
HDC hMemDC1;
HBITMAP hBmp1;
COLORREF transparentColorCH;
HWND _HWND;
HWND hWin;
 
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
{   _HWND=parent;
filePathCHESS=appRootPath+L"\\games\\chess\\chess_1.png";
bmpc=SHLoadImageFile(filePathCHESS.c_str());
/*GetObject(bmpc, sizeof(bmc), &bmc);*/
HDC hdcImage2=CreateCompatibleDC(NULL);
    SelectObject(hdcSKIN, bmpc);
SelectObject(hdcImage2, bmpc);
flagaktiv=0;
transparentColorCH=GetPixel(hdcImage2, 0, 0);DeleteDC(hdcImage2);


int Chesspoleinit[9][9]={
	0 , 0, 0, 0, 0, 0, 0, 0, 0,
	0 ,12,13,14,15,16,14,13,12,
	0 ,11,11,11,11,11,11,11,11,
	0 , 0, 0, 0, 0, 0, 0, 0, 0,
	0 , 0, 0, 0, 0, 0, 0, 0, 0,
	0 , 0, 0, 0, 0, 0, 0, 0, 0,
	0 , 0, 0, 0, 0, 0, 0, 0, 0,
	0 , 1, 1, 1, 1, 1, 1, 1, 1,
	0 , 2, 3, 4, 5, 6, 4, 3, 2


};
int cvtp=1;

	for(int x=1;x<=8;x++){for(int y=1;y<=8;y++){
		
		Chesspole[y][x]=Chesspoleinit[y][x];
	
	}}
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
{int cvtp=1;int figura;RECT rc2;BOOL postv;BOOL postvput;
rc2.left=rc2.top=0;
rc2.bottom=rc2.right=8*RAZMER+2;
	PAINTSTRUCT ps;
   HDC hdc;
HDC  hdc2;
int xf,yf;//для постановки-куда предпологаем ставить
int z;int z2;

ChessView *p=(ChessView *) GetWindowLong(hWnd, GWL_USERDATA);
	switch (message)
	{case WM_CLOSE:
		case WM_DESTROY:
			
			DeleteObject(hBmp1);//Удалим
			DeleteDC(hMemDC1);
		
	case WM_CREATE:

		p=(ChessView *) (((CREATESTRUCT *)lParam)->lpCreateParams);
        SetWindowLong(hWnd, GWL_USERDATA, (LONG) p );
		
		break;
	case WM_PAINT:
		
		// тута рисовать
		cvtp=1;
		hdc=GetDC(hWnd);//Получим HDC
		//hdc = BeginPaint(hWnd, &ps);
		hMemDC1=CreateCompatibleDC(hdc);
		hBmp1=CreateCompatibleBitmap(hdc,rc2.bottom,rc2.bottom);
		SelectObject(hMemDC1,hBmp1);
		
		for(int x=1;x<=8;x++){
			for(int y=1;y<=8;y++){
		
			if(cvtp==1){ TransparentImage(hMemDC1,(x-1)*RAZMER,(y-1)*RAZMER,RAZMER,RAZMER, bmpc, 13*RAZMERSKIN, 0, RAZMERSKIN, RAZMERSKIN, transparentColorCH);}else{TransparentImage(hMemDC1,(x-1)*RAZMER,(y-1)*RAZMER,RAZMER,RAZMER, bmpc, 14*RAZMERSKIN, 0, RAZMERSKIN, RAZMERSKIN, transparentColorCH);}
			if(cvtp==2){cvtp=1;}else{cvtp=2;}//рисуем квадрат
			//рисуем фигуру
			if(p->Chesspole[y][x]>6){figura=p->Chesspole[y][x]-4;}else{figura=p->Chesspole[y][x];}
						 TransparentImage(hMemDC1,(x-1)*RAZMER,(y-1)*RAZMER,RAZMER,RAZMER, bmpc, figura*RAZMERSKIN, 0, RAZMERSKIN, RAZMERSKIN, transparentColorCH);
			if(p->flagaktiv==1 && p->fokus_x==x && p->fokus_y==y){TransparentImage(hMemDC1,(x-1)*RAZMER,(y-1)*RAZMER,RAZMER,RAZMER, bmpc, 15*RAZMERSKIN, 0, RAZMERSKIN, RAZMERSKIN, transparentColorCH);}
			
			}
			if(cvtp==2){cvtp=1;}else{cvtp=2;}
		}
	
		hdc2 = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		
		BitBlt(hdc,0,0,rc2.bottom,rc2.bottom,hMemDC1,0,0,SRCCOPY);//Скопируем рисунок из памяти
	//	EndPaint(hWnd, &ps);
		
		break;

	case  IDC_WM_VYBOR:{
        
				
					p->fokus_x=(p->pt2.x)/RAZMER+1;
					p->fokus_y=(p->pt2.y)/RAZMER+1;
				p->fokus_f=p->Chesspole[p->fokus_y][p->fokus_x];
				p->flagaktiv=1;//нажали
				if(p->fokus_f==0)p->flagaktiv=0;
				InvalidateRect(hWnd,&rc2,1);
				PostMessage (hWnd, WM_PAINT, 0, 0);

					  // UpdateWindow(hWnd);
					   }
			
			
					   break;

	case IDC_WM_STAV:
		xf=(p->pt2.x)/RAZMER+1;
		yf=(p->pt2.y)/RAZMER+1;
				postvput=0;
				postv=0;
				if(p->fokus_f<7 && (p->Chesspole[yf][xf]>=7 || p->Chesspole[yf][xf]==0))postv=1;//на чужие или пусто
				if(p->fokus_f>6 && (p->Chesspole[yf][xf]<=6))postv=1;
               
				if(postv){//тут обрабатываем правильность хода в зависимости от фигуры
					if(p->fokus_f==1){postvput=0;//пешка
						if(p->fokus_y==7){//перепрыгнем на 2 вначале
							if(yf==5 && p->fokus_x==xf && p->Chesspole[6][xf]==0 && p->Chesspole[5][xf]==0){postvput=1;}else if(yf==6 && p->fokus_x==xf && p->Chesspole[6][xf]==0){postvput=1;}//
						
						}else{//на 1вперёд
						
							if(yf==p->fokus_y-1  && p->fokus_x==xf && p->Chesspole[yf][xf]==0){postvput=1;}//вперед на фигуру не идём
							if(yf==p->fokus_y-1  && (p->fokus_x==xf-1 || p->fokus_x==xf+1) && p->Chesspole[yf][xf]>6)postvput=1;//бьём на искось

						}


					//пешка-end
					}else if(p->fokus_f==11){postvput=0;//чёрнпешка
						
						if(p->fokus_y==2){//перепрыгнем на 2 вначале
							if(yf==4 && p->fokus_x==xf && p->Chesspole[3][xf]==0 && p->Chesspole[4][xf]==0){postvput=1;}else if(yf==3 && p->fokus_x==xf){postvput=1;}//
						
						}else{//на 1вперёд
						
							if(yf==p->fokus_y+1  && p->fokus_x==xf && p->Chesspole[yf][xf]==0){postvput=1;}//вперед на фигуру не идём
							if(yf==p->fokus_y+1  && (p->fokus_x==xf-1 || p->fokus_x==xf+1) && p->Chesspole[yf][xf]<7)postvput=1;//бьём на искось

							}

					//чёрнпешка-end
					}else if(p->fokus_f==2 || p->fokus_f==12){//тура
						if(yf==p->fokus_y){postvput=1;//горизонталь
						if(xf>p->fokus_x){//вправо
							for(z=p->fokus_x+1;z<xf;z++){if(p->Chesspole[yf][z]!=0)postvput=0;}}else if(xf<p->fokus_x){//ВЛЕВО
							for(z=p->fokus_x-1;z>xf;z--){if(p->Chesspole[yf][z]!=0)postvput=0;}}

						

						}else if(xf==p->fokus_x){postvput=1;//ВЕРТИКАЛЬ
						if(yf>p->fokus_y){//ВВЕРХ
							for(z=p->fokus_y+1;z<yf;z++){if(p->Chesspole[z][xf]!=0)postvput=0;}}else if(yf<p->fokus_y){//ВНИЗ
							for(z=p->fokus_y-1;z>yf;z--){if(p->Chesspole[z][xf]!=0)postvput=0;}}

						

						}


//тура-end
					}else if(p->fokus_f==3 || p->fokus_f==13){//конь

						if(xf==p->fokus_x-2 && yf==p->fokus_y-1)postvput=1;
						if(xf==p->fokus_x-1 && yf==p->fokus_y-2)postvput=1;
						if(xf==p->fokus_x+1 && yf==p->fokus_y-2)postvput=1;
						if(xf==p->fokus_x+2 && yf==p->fokus_y-1)postvput=1;
						if(xf==p->fokus_x+2 && yf==p->fokus_y+1)postvput=1;
						if(xf==p->fokus_x+1 && yf==p->fokus_y+2)postvput=1;
						if(xf==p->fokus_x-1 && yf==p->fokus_y+2)postvput=1;
						if(xf==p->fokus_x-2 && yf==p->fokus_y+1)postvput=1;

				
//конь-конец
					}else if(p->fokus_f==4 || p->fokus_f==14){//слон

						if(abs(p->fokus_x-xf)==abs(p->fokus_y-yf)){postvput=1;
						z2=0;
						if(p->fokus_x<xf){//право
							for(z=p->fokus_x+1;z<xf;z++){
								if(p->fokus_y<yf && p->Chesspole[p->fokus_y+1+z2][z]!=0){postvput=0;}
								else 
									if(p->fokus_y>yf && p->Chesspole[p->fokus_y-1-z2][z]!=0){postvput=0;}
							++z2;
							}
						}else 
							if(p->fokus_x>xf){
							for(z=p->fokus_x-1;z>xf;z--){
								if(p->fokus_y<yf && p->Chesspole[p->fokus_y+1+z2][z]!=0){postvput=0;}
								else 
									if(p->fokus_y>yf && p->Chesspole[p->fokus_y-1-z2][z]!=0){postvput=0;}
							++z2;
						}
						
							}}
						
						

//слон-конец
					}else if(p->fokus_f==5 || p->fokus_f==15){//ферзь

//как слон 
					if(abs(p->fokus_x-xf)==abs(p->fokus_y-yf)){postvput=1;
						z2=0;
						if(p->fokus_x<xf){//право
							for(z=p->fokus_x+1;z<xf;z++){
								if(p->fokus_y<yf && p->Chesspole[p->fokus_y+1+z2][z]!=0){postvput=0;}
								else 
									if(p->fokus_y>yf && p->Chesspole[p->fokus_y-1-z2][z]!=0){postvput=0;}
							++z2;
							}
						}else 
							if(p->fokus_x>xf){
							for(z=p->fokus_x-1;z>xf;z--){
								if(p->fokus_y<yf && p->Chesspole[p->fokus_y+1+z2][z]!=0){postvput=0;}
								else 
									if(p->fokus_y>yf && p->Chesspole[p->fokus_y-1-z2][z]!=0){postvput=0;}
							++z2;
						}
						
							}}else{
//или тура
						if(yf==p->fokus_y){postvput=1;//горизонталь
						if(xf>p->fokus_x){//вправо
							for(z=p->fokus_x+1;z<xf;z++){if(p->Chesspole[yf][z]!=0)postvput=0;}}else if(xf<p->fokus_x){//ВЛЕВО
							for(z=p->fokus_x-1;z>xf;z--){if(p->Chesspole[yf][z]!=0)postvput=0;}}

						

						}else if(xf==p->fokus_x){postvput=1;//ВЕРТИКАЛЬ
						if(yf>p->fokus_y){//ВВЕРХ
							for(z=p->fokus_y+1;z<yf;z++){if(p->Chesspole[z][xf]!=0)postvput=0;}}else if(yf<p->fokus_y){//ВНИЗ
							for(z=p->fokus_y-1;z>yf;z--){if(p->Chesspole[z][xf]!=0)postvput=0;}}

						

						}}


//конец ферз
					}else if(p->fokus_f==6 || p->fokus_f==16){postvput=0;//король

						if(abs(yf-p->fokus_y)==1  || abs(p->fokus_x-xf)==1){postvput=1;
						if(p->fokus_f==6)p->rocb=4;
						if(p->fokus_f==16)p->rokch=4;
						
						
						}//тут будет рокоровка else if()

//конец король
					}

				}
			
				//поставили
				if( postvput){
				p->Chesspole[p->fokus_y][p->fokus_x]=0;
				p->fokus_x=(p->pt2.x)/RAZMER+1;
				p->fokus_y=(p->pt2.y)/RAZMER+1;
				
				p->Chesspole[p->fokus_y][p->fokus_x]=p->fokus_f;

				}
				//UpdateWindow(hWnd);//
				InvalidateRect(hWnd,&rc2,1);
				PostMessage (hWnd, WM_PAINT,0 , 0);
					p->flagaktiv=0;
					 
					 break;

		case WM_LBUTTONDOWN:
			
		

			SetFocus(hWnd);
            SHRGINFO    shrg;
            shrg.cbSize = sizeof(shrg);
            shrg.hwndClient = hWnd;
            shrg.ptDown.x = LOWORD(lParam);
            shrg.ptDown.y = HIWORD(lParam);
            shrg.dwFlags = SHRG_RETURNCMD /*| SHRG_NOANIMATION*/;

			if (SHRecognizeGesture(&shrg) == GN_CONTEXTMENU) {

              
 
                HMENU hmenu = CreatePopupMenu();
                if (hmenu==NULL) break;

				
                
                if(p->flagaktiv==0)AppendMenu(hmenu, MF_STRING, WM_VYBOR, TEXT("Выбрать") );
				if(p->flagaktiv==1){AppendMenu(hmenu, MF_STRING, WM_STAV, TEXT("Поставить") );
				AppendMenu(hmenu, MF_STRING, WM_OTMENA_VYBOR, TEXT("Отмена выбора") );}
                AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hmenu, MF_STRING, EM_UNDO, TEXT("Отмена") );
				AppendMenu(hmenu, MF_STRING, SB_, TEXT("Пригласить") );
				AppendMenu(hmenu, MF_STRING, SB_, TEXT("Сдаться") );
				AppendMenu(hmenu, MF_STRING, SB_, TEXT("Ничья") );
POINT pt;		 p->pt2.x=GET_X_LPARAM(lParam);
				p->pt2.y=GET_Y_LPARAM(lParam);
               pt.x=GET_X_LPARAM(lParam);
				pt.y=GET_Y_LPARAM(lParam);
                ClientToScreen(hWnd, &pt);

                int cmdId=TrackPopupMenuEx(hmenu,
                    TPM_BOTTOMALIGN | TPM_RETURNCMD,
                    pt.x, pt.y,
                    hWnd,
                    NULL);
                if (cmdId==WM_VYBOR)PostMessage(hWnd, IDC_WM_VYBOR,0 , 0);
                if (cmdId==WM_STAV)PostMessage(hWnd, IDC_WM_STAV,0 , 0);

                return 0;
			}

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


