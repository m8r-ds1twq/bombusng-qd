// ui.cpp : Defines the entry point for the application.
//

//#include "stdafx.h"

#include "ui.h"
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <aygshell.h>
#include <afxwin.h>
#include "wmuser.h"
#include "Notify.h"
#include "boostheaders.h"
#include "LogPanel.h"
#include "Socket.h"
#include "CETLSSocket.h"
#include <string>
#include "JabberDataBlock.h"
#include "JabberStream.h"
#include "JabberAccount.h"
#include "JabberListener.h"
#include "JabberDataBlockListener.h"
#include "ResourceContext.h"
#include "HostFeatures.h"
#include "DlgNEWS.h"
#include "EntityCaps.h"
#include "Roster.h"

#include "ProcessMUC.h"
#include "MucBookmarks.h"

#include "Captcha.h"

#include "DlgAbout.h"
#include "DlgAccount.h"
#include "DlgConfig.h"
#include "DlgStatus.h"
#include "DlgMucJoin.h"
#include "DlgMoods.h"
#include "VirtualListView.h"
#include "ChatView.h"
#include "TabCtrl.h"
#include "XDataForm.h"
#include "VcardForm.h"
#include "ServiceDiscovery.h"
#include "LastActivity.h"
#include "color.h"
#include "Auth.h"
#include "DlgBLAG.h"
#include "Sysinfo.h"

#include "Image.h"
#include "Smiles.h"
#include "History.h"

#include "utf8.hpp"

#include "config.h"

#include "dnsquery.h"
#include "boostheaders.h"

#define MAX_LOADSTRING 100
int sizecaps;
char **strokicaps;
#define TIMER_TIME 2000 //����� ������ �������
//#define TIMER_STATUS 60 //����� �����������
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#define MAIN_TIMER_ID 7852
int timerid=0;
HINSTANCE hInstance235;
LONG timstatus=0;
LONG timaliv=0;

std::string pamessage;//���������
presence::PresenceIndex astatus;
int idautostatus=0;//0-���� 1-������� 2-���������

#define TIMER_ALIV Config::getInstance()->ping_aliv//����� ��������
#define TIMER_ALIVP Config::getInstance()->pong_aliv //����� ��������
std::wstring out_title;//�� ������
std::wstring out_OriginalArtist;
std::wstring out_title_st;
std::wstring out_OriginalArtist_st;
std::string newsstring;
#define AKTIV_PLAY 15  //����� �� ������������
int  aktiv_wp=0;
long aktiv_wp_sc=0;//����� ������-�������� ����� 1-2 ��� 2-4 ��� 3-6��� 4-8���
long aktiv_wp_sc_not=0;//����� �� ������-�������� ����� 1-2 ��� 2-4 ��� 3-6��� 4-8���
bool aktiv_wp_sc_flag=1;//0-�� ������ 1-������
int aktiv_wp_s=1;//
/*
typedef struct {
  char date[32];
  char title[1024];
  char text[5120];
  char author[32];
} ITEM;

ITEM *it;


char *html_decode(char *s)
{
  int l=strlen(s);
  int c;
  char *d, *dest;
  d=dest=(char*)malloc(l+1);
  char *tag_strip=NULL;
  while((c=*s++))
  {
  L_START:
    if (c=='<')
    {
      if (!strncmp(s,"br>",3))
      {
        *d++=13;
        s+=3;
        continue;
      }
      if (!strncmp(s,"br />",5))
      {
        *d++=13;
        s+=5;
        continue;
      }
      if (!strncmp(s,"/td>",4))
      {
        *d++=' ';
        s+=4;
        continue;
      }
      if (!strncmp(s,"p>",2))
      {
        *d++=13;
        s+=2;
        continue;
      }
      // ����� ����� �� ����� ���, ����� ����� �)
      tag_strip=d;
      continue;
    }
    if (c=='>')
    {
      if (tag_strip)
      {
        d=tag_strip;
        tag_strip=NULL;
        continue;
      }
    }
    if (c=='&')
    {
      if (!strncmp(s,"quot;",5))
      {
        c='\"';
        s+=5;
        goto L_START;
      }
      if (!strncmp(s,"nbsp;",5))
      {
        c='_';
        s+=5;
        goto L_START;
      }
      if (!strncmp(s,"lt;",3))
      {
        c='<';
        s+=3;
        goto L_START;
      }
      if (!strncmp(s,"gt;",3))
      {
        c='>';
        s+=3;
        goto L_START;
      }
      if (!strncmp(s,"amp;",4))
      {
        c='&';
        s+=4;
        goto L_START;
      }
      if (!strncmp(s,"copy;",5))
      {
        c=0xA9;
        s+=5;
        goto L_START;
      }
      if (*s=='#')
      {
        int k;
        s++;
        c=0;
        while((k=*s++)!=';')
        {
          c*=10;
          c+=k-'0';
        }
        //c=char16to8(c);
        goto L_START;
      }
    }
    *d++=c;
  }
  *d=0;
  return dest;
}
*/

BOOL regmuz_mp(void){
HKEY rKey;
TCHAR title[515];
out_OriginalArtist_st=out_OriginalArtist;
out_title_st=out_title;
*title=0;
TCHAR OriginalArtist[515];
*OriginalArtist=0;
DWORD dwType = REG_BINARY;
DWORD dwType2 = REG_DWORD;
 int aktiv_wp2=aktiv_wp;//������
DWORD RegetPath = sizeof(title);
DWORD RegetPath2 = sizeof(OriginalArtist);
DWORD RegetPath3 = sizeof(aktiv_wp);
RegOpenKeyEx(HKEY_CURRENT_USER, L"System\\State\\MediaPlayer", 0, 0, &rKey);
RegQueryValueEx(rKey, L"Title", NULL,  &dwType, (LPBYTE)title, &RegetPath);
RegQueryValueEx(rKey, L"WM/OriginalArtist", NULL,  &dwType, (LPBYTE)OriginalArtist, &RegetPath2);
RegQueryValueEx(rKey, L"Elapsed", NULL,  &dwType2, (LPBYTE)&aktiv_wp, &RegetPath3);

RegCloseKey(rKey);
out_title.assign(title);
out_OriginalArtist.assign(OriginalArtist);
if(aktiv_wp_sc>500)aktiv_wp_sc=0;
if(aktiv_wp_sc_not>500)aktiv_wp_sc_not=0;
if(aktiv_wp2 ==aktiv_wp && !aktiv_wp_sc_flag){++aktiv_wp_sc_not;aktiv_wp_sc=0;}
if(aktiv_wp2 !=aktiv_wp && aktiv_wp_sc_flag){++aktiv_wp_sc;aktiv_wp_sc_not=0;}
/*if(out_OriginalArtist_st!=out_OriginalArtist || out_title_st!=out_title){aktiv_wp_s=2;}else{
	if(aktiv_wp_s!=1 && aktiv_wp2 !=aktiv_wp){aktiv_wp_s=2;}}
if(aktiv_wp2 ==aktiv_wp){++aktiv_wp_sc_not;aktiv_wp_sc=0;}else{++aktiv_wp_sc;aktiv_wp_sc_not=0;}
*/
return 1;
	
}



//utf8::wchar_utf8(out);




BOOL timealivid=0;//0-���� 1-������� 
// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndMenuBar;		// menu bar handle
HWND		mainWnd;
HCURSOR     cursorWait;
std::wstring statusMessage1;
std::string statusMessage2;
std::string typevs;
//ListViewRef logWnd;
TabsCtrlRef tabs;
int tabsconf;
VirtualListView::ref odrLog;
RosterListView::ref rosterWnd;
ResourceContextRef rc;
HINSTANCE m_hInstanceDll;
ImgListRef skin;

SmileParser *smileParser;

std::wstring appRootPath;
std::wstring skinRootPath;
std::string appVersion;
std::string appName;

int tabHeight;
int rosterStatus = 0;
int reconnectTry = 3;
int socketError = 0;
Contact::ref bufc;
int COLORS[30];
int prepareAccount();
int initJabber(ResourceContextRef rc);
//void streamShutdown();
void streamShutdown(ResourceContextRef rc);
HWND hwnvs;
#include <WinInet.h>
#pragma comment(lib,"wininet") //�������� � �������������� ����������� ���������� WinInet.lib

const int IBUFFER_LENGTH = 8192;
const TCHAR pcszAppCaption[] = TEXT("NEWS");

//

#define NOTIFY_ID 997
static const GUID APP_GUID = { 0xbaeef0cf, 0xd06e, 0x490c, { 0x92, 0x6, 0x3b, 0x25, 0x25, 0xc2, 0x84, 0x9c } };
//boost::regex e3("((?:(?:ht|f)tps?://|www\\.)[^<\\s\\n]+)(?<![]\\.,:;!\\})<-])");


int news_parse(void){
	SHELLEXECUTEINFO info;
   info.cbSize = sizeof(info);
   info.fMask = SEE_MASK_FLAG_NO_UI;
   info.hwnd = NULL;
   info.lpVerb = L"open";
   info.lpFile = L"http://bombusng-qd.googlecode.com/files/news.html";
   info.lpParameters = L"";
   info.lpDirectory = L"";
   info.nShow = SW_SHOW;
   //info.hInstApp = AfxGetInstanceHandle(); // ����� �� ��������� ������ ����, � ��� ��������
   ShellExecuteEx(&info);
   return 1;
	//MessageBox(NULL, TEXT("NEWS start"), pcszAppCaption, 0);
 /*HINTERNET hInet = InternetOpen(TEXT("WinInet Test. Windows CE x.x"), NULL, NULL, NULL, 0);
    //HCURSOR OC=NULL;
	//OC=SetCursor(LoadCursor(NULL, IDC_WAIT));
 if(hInet)
 {
       // HINTERNET hConnection = InternetConnect(hInet, TEXT("bombusng-qd.googlecode.com"), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1u);
        //if(hConnection)
		//{

       //     HINTERNET hRequest = HttpOpenRequest(hConnection, TEXT("GET"), TEXT("/files/news.txt"),NULL, NULL, 0, INTERNET_FLAG_DONT_CACHE, 1);
         //   if(hRequest)
         //   {
           //      HttpSendRequest(hRequest, NULL, 0, NULL, 0);
	  HINTERNET hRequest= InternetOpenUrl(hInet,TEXT("http://bombusng-qd.googlecode.com/files/news.txt"),NULL, NULL, 0,0);
		if(hRequest)
				{std::string data;
				//std::wstring resul;
                    char szBuffer[IBUFFER_LENGTH];
                    DWORD dwBytesRead;
                    //��������� ������ IBUFFER_LENGTH ����
                    BOOL bRead = InternetReadFile(hRequest, szBuffer, IBUFFER_LENGTH-1, &dwBytesRead); 
					
						
                    if(bRead && dwBytesRead > 0)
                    {
						data.assign((const char*)&szBuffer);
					while(InternetReadFile(hRequest, szBuffer, IBUFFER_LENGTH-1, &dwBytesRead))
					{
						if(!dwBytesRead)break;
						data.append((const char*)&szBuffer);
					}
					//data.assign("123654");
//SetCursor(OC);
newsstring=data;//utf8::utf8_wchar(&szBuffer);
//lgNEWS(g_hInst, mainWnd);
			Log::getInstance()->msg(newsstring);

//	MessageBox(NULL, TEXT("NEWS has been read!!!"), pcszAppCaption, 0);
	if (!tabs->switchByWndRef(odrLog)) {
                    tabs->addWindow(odrLog);
                    tabs->switchByWndRef(odrLog);
	}

InternetCloseHandle(hRequest);
//InternetCloseHandle(hConnection);
InternetCloseHandle(hInet);
return 1;
					}
                    else
                    {//SetCursor(OC);
                        MessageBox(NULL, TEXT("NEWS cannot be read"), pcszAppCaption, MB_OK | MB_ICONWARNING);
                    return 0;}
		}
            // /   else
            // /   {//SetCursor(OC);
             //       MessageBox(NULL, TEXT("Unable to send request"), pcszAppCaption, MB_OK | MB_ICONWARNING);
             //    return 0;}
          //      InternetCloseHandle(hRequest);
			//}
       //     else
       //     {//SetCursor(OC);
       //         MessageBox(NULL, TEXT("Unable to open http request"), pcszAppCaption, MB_OK | MB_ICONWARNING);
       //      return 0;}
       //  /   InternetCloseHandle(hConnection);
	//	}
     //   else
     //   {//SetCursor(OC);
     //       MessageBox(NULL, TEXT("Unable to open internet connection"), pcszAppCaption, MB_OK | MB_ICONWARNING);
     //   return 0; }
//

        InternetCloseHandle(hInet);
 }
    else
    {//SetCursor(OC);
        MessageBox(NULL, TEXT("Cannot open Internet"), pcszAppCaption, MB_OK | MB_ICONWARNING);
    return 0; }*/
}

std::string encloseHTML(std::string ostr) {
  std::string::iterator s=ostr.begin(),smax=ostr.end();
  std::string str;
  str.reserve(smax-s);
  while(s!=smax) {
    switch(*s) { 
      case '&': str+="&amp;"; break;
      case '<': str+="&lt;"; break;
      case '>': str+="&gt;"; break;
      default: str+=*s;
    }
    s++;
  }
  return str;
}
void sbros_title(void){std::string muz;

muz+=utf8::wchar_utf8(out_title)+" "+utf8::wchar_utf8(out_OriginalArtist);
rc->presenceMessage=" ";
		out_OriginalArtist_st=out_OriginalArtist;
		out_title_st=out_title;
if(Config::getInstance()->tune_status){
				Log::getInstance()->msg("set notstatus",muz.c_str());
				rc->sendPresence();
			rc->roster->setMUCStatus(rc->status);
		}
if(Config::getInstance()->tune_status_pep){
JabberDataBlock iq=("iq");

iq.setAttribute("type","set");
iq.setAttribute("id",strtime::getRandom());
JabberDataBlockRef pubsub=iq.addChildNS("pubsub","http://jabber.org/protocol/pubsub");
JabberDataBlockRef publish=pubsub->addChild("publish");
publish->setAttribute("node", "http://jabber.org/protocol/tune");
JabberDataBlockRef item=publish->addChild("item");
JabberDataBlockRef tune=item->addChildNS("tune","http://jabber.org/protocol/tune");
//JabberDataBlockRef artist=tune->addChild("artist",utf8::wchar_utf8(out_OriginalArtist).c_str());
//JabberDataBlockRef title=tune->addChild("title",utf8::wchar_utf8(out_title).c_str());
rc->jabberStream->sendStanza(iq);
Log::getInstance()->msg("set notPEP",muz.c_str());
}
/*
				<iq type="set" id="mni">
	<pubsub xmlns='http://jabber.org/protocol/pubsub'>
		<publish node='http://jabber.org/protocol/tune'>
			<item>
				<tune xmlns='http://jabber.org/protocol/tune'>
					<artist>��� �� ������</artist>
					<title>������� � ����</title>
				</tune>
			</item>
		</publish>
	</pubsub>
</iq>

				*/

}
void vyvod_title(void){
std::string muz;

muz+=utf8::wchar_utf8(out_title)+" "+utf8::wchar_utf8(out_OriginalArtist);
if(idautostatus==1){rc->presenceMessage=muz+"("+Config::getInstance()->avtomessage+strtime::toLocalTime(strtime::getCurrentUtc())+")";}else{rc->presenceMessage=muz;}
		
if(Config::getInstance()->tune_status){
				Log::getInstance()->msg("set status",muz.c_str());
				rc->sendPresence();
			rc->roster->setMUCStatus(rc->status);
		}
if(Config::getInstance()->tune_status_pep){
JabberDataBlock iq=("iq");

iq.setAttribute("type","set");
iq.setAttribute("id",strtime::getRandom());
JabberDataBlockRef pubsub=iq.addChildNS("pubsub","http://jabber.org/protocol/pubsub");
JabberDataBlockRef publish=pubsub->addChild("publish");
publish->setAttribute("node", "http://jabber.org/protocol/tune");
JabberDataBlockRef item=publish->addChild("item");
JabberDataBlockRef tune=item->addChildNS("tune","http://jabber.org/protocol/tune");
JabberDataBlockRef artist=tune->addChild("artist",utf8::wchar_utf8(out_OriginalArtist).c_str());
JabberDataBlockRef title=tune->addChild("title",utf8::wchar_utf8(out_title).c_str());
rc->jabberStream->sendStanza(iq);
Log::getInstance()->msg("set PEP",muz.c_str());
}
/*
				<iq type="set" id="mni">
	<pubsub xmlns='http://jabber.org/protocol/pubsub'>
		<publish node='http://jabber.org/protocol/tune'>
			<item>
				<tune xmlns='http://jabber.org/protocol/tune'>
					<artist>��� �� ������</artist>
					<title>������� � ����</title>
				</tune>
			</item>
		</publish>
	</pubsub>
</iq>

				*/


		
}

HRESULT AddNotification(HWND hWnd,LPCTSTR notmess,int flagvs)
{    //������ ���
	//flagvs:
	//0-���������
	//1-������
 SHNOTIFICATIONDATA sn = {0};
 sn.cbStruct = sizeof(SHNOTIFICATIONDATA);
 sn.dwID = NOTIFY_ID;
 sn.clsid = APP_GUID;
 sn.hwndSink = hWnd;
 sn.npPriority = SHNP_INFORM;
 
 sn.grfFlags = SHNF_FORCEMESSAGE;
 if(flagvs==0){
	 sn.csDuration = 15;
	 sn.pszTitle = TEXT("����� ��������� BombusQD");
	 sn.hicon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MESS_IC));
 }
 if(flagvs==1){
	 sn.csDuration = 3;
	 sn.pszTitle = TEXT("����������� BombusQD");
	 sn.hicon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_BOMBUS));
 }
 sn.pszHTML = notmess;


 return SHNotificationAdd(&sn);
}
HRESULT DeleteNotification()
{
	return SHNotificationRemove(&APP_GUID, NOTIFY_ID);
}
//


void Shell_NotifyIcon(bool show, HWND hwnd);

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

  

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;
hInstance235=hInstance;
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}
  
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UI));
   // 
	reconnectTry = Config::getInstance()->reconnectTries;

	// Main message loop:
    //while (GetMessage(&msg, NULL, 0, 0)) {
	while (true) {
        if (rc) if (rc->jabberStream) rc->jabberStream->parseStream();

        if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  Sleep(50); continue; }
        if (msg.message==WM_QUIT) break;
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}

	}
    Shell_NotifyIcon(false, NULL);
    Config::getInstance()->save();
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOMBUS));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
const std::string responseMd5Digest( const std::string &user, const std::string &pass, const std::string &realm, const std::string &digestUri, const std::string &nonce, const std::string cnonce);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
 wchar_t wbuf[1024];
    GetModuleFileName(hInstance, wbuf, sizeof(wbuf));
    appRootPath=wbuf;

    int namePos=appRootPath.find_last_of(_T("\\"))+1;
    appRootPath.erase(namePos, appRootPath.length()-namePos);
/*std::wstring dllname(appRootPath);//�� ����� :(
dllname+=TEXT("lang.dll");
HINSTANCE hInst2 = LoadLibrary(dllname.c_str());
if(hInst2){
	AfxSetResourceHandle(hInst2);}else{int result2=MessageBox(NULL, dllname.c_str(), TEXT("error dll"), MB_OKCANCEL | MB_ICONEXCLAMATION );}
   */
	TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_UI, szWindowClass, MAX_LOADSTRING);

    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

   
    wchar_t * skinRelPath;

//
    tabsconf=Config::getInstance()->tabconf;
    if (sysinfo::screenIsVGA()) {
        skinRelPath=TEXT("vga\\");
        tabHeight=34 + tabsconf; //TODO: remove hardcode
    } else {
        skinRelPath=TEXT("qvga\\");
        tabHeight=18 + tabsconf; //TODO: remove hardcode
    }
    skinRootPath=appRootPath+skinRelPath;

    LoadString(g_hInst, IDS_VERSION, wbuf, sizeof(wbuf));
    appVersion=utf8::wchar_utf8(wbuf);
    appName="BombusQD-NG";//
     colorsload();//load color
timerid=SetTimer(0,MAIN_TIMER_ID,TIMER_TIME,TimerProc);
    if (!MyRegisterClass(hInstance, szWindowClass)) 	return FALSE;

    mainWnd=hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
hwnvs=mainWnd;
    if (!hWnd)
    {
        return FALSE;
    }

    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    Shell_NotifyIcon(true, hWnd);
    prepareAccount();

    if (Config::getInstance()->connectOnStartup) {
        rc->status=presence::ONLINE;
        initJabber(rc);
    }


    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{std::wstring bufff=L"";
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
WndRef chat2;int result;
    static SHACTIVATEINFO s_sai;
	Serialize s(L"config\\status", Serialize::READ);
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId) {
                case IDM_HELP_ABOUT: {

                    DlgAbout(g_hInst, hWnd);

				    break;
                }

				 case ID_BLAGO: {

                    DlgBLAG(g_hInst, hWnd);

				    break;
                }
				case ID_TOOLS_COLORRE:
					

//
					 //colorsload();
					break;
				case NEWS:
result=MessageBox(NULL, TEXT("������� �������? ��� ������������ �������� ��������"), TEXT("NEWS"), MB_YESNO | MB_ICONWARNING );
					if (result==IDYES){
						news_parse();
					}
					

				break;

				case MOODS_AKTIV:
					DlgMoods::createDialog(hWnd, rc);
					break;
				case AKTIVW:
					SetForegroundWindow(mainWnd);
					SetActiveWindow(mainWnd);
					SetFocus(mainWnd);
//������� ;)	MessageBox(hWnd, TEXT("Clicked"), TEXT("!"), 0);
				chat2=tabs->getWindowByODR(bufc);
					 if (!chat2) {
        //Contact::ref r=roster.lock()->findContact(c->jid.getJid());
					 chat2=WndRef(new ChatView(tabs->getHWnd(), bufc));
                    tabs->addWindow(chat2);
                      }
					 tabs->switchByWndRef(chat2);

					DeleteNotification();
				break;
                case ID_JABBER_OPTIONS:
                    DialogConfigMP(g_hInst, hWnd);
                    rc->myCaps=MyCapsRef(new MyCaps());
                    if (!rc->isLoggedIn()) break;
                    rc->roster->makeViewList();
                    break;

                case IDM_JABBER_ACCOUNT:
                    DialogAccountMP(g_hInst, hWnd, rc->account);
                    break;
                case IDM_EXIT:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);	
					KillTimer(0,timerid);
                    break;

                case IDM_JABBER_STATUS:
                    DlgStatus::createDialog(hWnd, rc);
                    break;

				/* ����� ������� �� ������ ������ */
				/* ������ ��� ����� �������� ����... */
				/* ���� �� �������� ��� ������������� :(
				 ���� ;)
				*/
				
				case IDM_STATUS_ONLINE:
				case IDM_STATUS_FFC:	
				case IDM_STATUS_AWAY:		
				case IDM_STATUS_EXTENDEDAWAY:		
				case IDM_STATUS_DND:		
				case IDM_STATUS_OFFLINE:
					switch(wmId)
					{
					case IDM_STATUS_ONLINE:
						rc->status=presence::ONLINE;
						if (rc->roster)rc->roster->setMUCStatus(presence::ONLINE);
						break;
					case IDM_STATUS_FFC:
						rc->status=presence::CHAT;
						if (rc->roster)rc->roster->setMUCStatus(presence::CHAT);
						break;
					case IDM_STATUS_AWAY:
						rc->status=presence::AWAY;
						if (rc->roster)rc->roster->setMUCStatus(presence::AWAY);
						break;
					case IDM_STATUS_EXTENDEDAWAY:
						rc->status=presence::XA;
						if (rc->roster)rc->roster->setMUCStatus(presence::XA);
						break;
					case IDM_STATUS_DND:
						rc->status=presence::DND;
						if (rc->roster)rc->roster->setMUCStatus(presence::DND);
						break;
					case IDM_STATUS_OFFLINE:
						rc->status=presence::OFFLINE;
						if (rc->roster)rc->roster->setMUCStatus(presence::OFFLINE);
						break;
					}
					idautostatus=0;
					timstatus=0;
					s.streamString(rc->presenceMessage, "");
					s.streamInt(rc->priority, 0);
					rosterWnd->setIcon(rc->status);
					rc->sendPresence();
					initJabber(rc);
					break;
				/* !�����! �������� */ 
			
				/*case IDM_JABBER_ONLINE:
					initJabber();
					break;

                case IDM_JABBER_OFFLINE:
                    streamShutdown();
                    break;*/

                case ID_JABBER_JOINCONFERENCE:
                    if (rc->isLoggedIn())
                        DlgMucJoin::createDialog(hWnd, rc, "");
                    break;

				case ID_SUPP:
                    if (rc->isLoggedIn())
                        DlgMucJoin::createDialog(hWnd, rc, "siemensclub@conference.jabber.ru");
                    break;


                case ID_TOOLS_MYVCARD:
                    if (rc->isLoggedIn()) {
                        WndRef vc=VcardForm::createVcardForm(tabs->getHWnd(), rc->account->getBareJid(), rc, true);
                        tabs->addWindow(vc);
                        tabs->switchByWndRef(vc);
                        break;
                    }

                case ID_TOOLS_SERVICEDISCOVERY:
                    {
                        ServiceDiscovery::ref disco=ServiceDiscovery::createServiceDiscovery(tabs->getHWnd(), rc, rc->account->getServer() , "", false);
						tabs->addWindow(disco);
                        tabs->switchByWndRef(disco);
                    }
                    break;

				case IDM_JABBER_STREAMINFO:
                    Log::getInstance()->msg(
						rc->jabberStream->connection->getStatistics().c_str()
						);
					break;

				case IDM_WINDOWS_LOG:
                    if (!tabs->switchByWndRef(odrLog)) {
                        tabs->addWindow(odrLog);
                        tabs->switchByWndRef(odrLog);
                    }
                    break;


				/*case IDM_WINDOWS_ROSTER:
                    tabs->switchByWndRef(rosterWnd);
					break;*/
                case IDS_WINDOWS:
                    SendMessage(tabs->getHWnd(), WM_COMMAND, IDS_WINDOWS, 0);

                default:
                    if (tabs) tabs->fwdWMCommand(wmId);
                    //return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;
            mbi.dwFlags = SHCMBF_HMENU;

            cursorWait=LoadCursor(NULL, IDC_WAIT);
            //skin=ImgListRef(new ImgArray(TEXT("skin.png"), 8, 6));
            skin=ImgListRef(new Skin(TEXT("")));

            smileParser=new SmileParser();
            //skin->setGridSize(8, 6);

			//editWnd=DoCreateEditControl(hWnd);
            tabs=TabsCtrlRef(new MainTabs(hWnd));
            //tabs->setParent(hWnd);

            rosterWnd=RosterListView::ref(new RosterListView(tabs->getHWnd(), std::string("Roster")));
            tabs->addWindow(rosterWnd);

            { 
                odrLog = VirtualListView::ref(new VirtualListView(tabs->getHWnd(), std::string("Log")));
                tabs->addWindow(odrLog);
                LogPanel::bindLV(odrLog); 
            }

            /*#ifdef DEBUG
            {
                XDataForm::ref testXdata=XDataForm::createXDataForm(tabs->getHWnd(), "", rc);
                tabs->addWindow(testXdata);
                testXdata->formTest();
            }
            #endif*/

			//listWnd=logWnd;
			//dropdownWnd=DoCreateComboControl(hWnd);

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);

            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            /*{
                skin->drawElement(hdc, 0, 0,0);                
            }*/
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;

		case WM_SIZE: 
			{ 
				HDWP hdwp; 
				
				int height=GET_Y_LPARAM(lParam);
								
				// Size the tab control to fit the client area. 
				hdwp = BeginDeferWindowPos(1);

				DeferWindowPos(hdwp, tabs->getHWnd(), HWND_TOP, 0, 0 /*tabHeight*/, 
					GET_X_LPARAM(lParam), height /* -tabHeight */, 
					SWP_NOZORDER 
					);
				
				EndDeferWindowPos(hdwp); 
				
				break; 
			} 

        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;

		//case WM_CTLCOLORLISTBOX:
        
        
        /*case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT: 
			{
                HDC hdc=(HDC) wParam;
                HWND hwnd=(HWND) lParam;
				SetBkColor(hdc, 0x000000);
				SetTextColor(hdc, 0xffffff);
				return (BOOL) GetStockObject(BLACK_BRUSH);
				break;
			}*/

        case SHELLNOTIFYICON:
            SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));            
            break;
        case WM_FORWARD_STANZA:
            {
                JabberDataBlockRef *rf=(JabberDataBlockRef *)lParam; //������
                if (rf==NULL) break; 
				if (rc->jabberStanzaDispatcher2){;rc->jabberStanzaDispatcher2->dispatchDataBlock(*rf);}
                delete rf;
            }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////
/*class MTForwarder: public JabberDataBlockListener {
public:
    MTForwarder(ResourceContextRef rc) {
        //this->rc=rc;
    };
    ~MTForwarder(){};
    virtual const char * getTagName() const { return NULL; }
    virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult MTForwarder::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc) {
    JabberDataBlockRef *p=new JabberDataBlockRef(block); //������
    //rc->jabberStanzaDispatcher2->dispatchDataBlock(block);
    PostMessage(mainWnd, WM_FORWARD_STANZA, 0, (LPARAM)p);

    return BLOCK_PROCESSED;
}*/
//////////////////////////////////////////////////////////////
class GetRoster : public JabberDataBlockListener {
public:
    GetRoster() {}
	~GetRoster(){};
	virtual const char * getType() const{ return "result"; }
	virtual const char * getId() const{ return "roster"; }
	virtual const char * getTagName() const { return "iq"; }
	virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};


ProcessResult GetRoster::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){
	Log::getInstance()->msg("Roster arrived");

    rc->roster->blockArrived(block, rc); // forwarding to dispatch roster stanza
    
    //rc->jabberStanzaDispatcherRT->addListener(JabberDataBlockListenerRef(new MTForwarder(rc)));
    //rc->jabberStanzaDispatcher2->addListener(rc->roster);

    rc->jabberStanzaDispatcherRT->addListener(rc->roster);

    rosterWnd->setIcon(rc->status);
    rc->sendPresence();
	rosterStatus = 1;

	std::string nick = rc->account->getNickname();
	std::wstring temp(nick.length(),L' ');
	std::copy(nick.begin(), nick.end(), temp.begin());
	LPCTSTR nick_l=temp.c_str();
/*�������� ���� ������� �������
	int result=MessageBox(
		rosterWnd->getHWnd(),
		nick_l, 
		L"Join support room?", 
		MB_YESNO | MB_ICONWARNING);
	if (result==IDYES) {
		DlgMucJoin::createDialog(rosterWnd->getHWnd(), rc, "qd-ng@conference.jabber.ru");
	}
*/
 
	return LAST_BLOCK_PROCESSED;
}
//////////////////////////////////////////////////////////////
class Version : public JabberDataBlockListener {
public:
    Version() {}
    ~Version(){};
    virtual const char * getType() const{ return "get"; }
    virtual const char * getId() const{ return NULL; }
    virtual const char * getTagName() const { return "iq"; }
    virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult Version::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){

    JabberDataBlockRef query=block->getChildByName("query");
    if (!query) return BLOCK_REJECTED;
    if (query->getAttribute("xmlns")!="jabber:iq:version") return BLOCK_REJECTED;

    Log::getInstance()->msg("version request ", block->getAttribute("from").c_str());
	std::string vga;
	if(sysinfo::screenIsVGA()){vga.append(" VGA ");}else{vga.append(" QVGA ");}
	std::string version=sysinfo::getOsVersion()+vga;

 

    JabberDataBlock result("iq");
    result.setAttribute("to", block->getAttribute("from"));
    result.setAttribute("type", "result");
    result.setAttribute("id", block->getAttribute("id"));
    result.addChild(query);

    query->addChild("name",::appName.c_str());
    query->addChild("version",::appVersion.c_str());
    query->addChild("os",version.c_str());

    rc->jabberStream->sendStanza(result);
    return BLOCK_PROCESSED;
}
//////////////////////////////////////////////////////////////
class Ping : public JabberDataBlockListener {
public:
    Ping() {}
    ~Ping(){};
    virtual const char * getType() const{ return "get"; }
    virtual const char * getId() const{ return NULL; }
    virtual const char * getTagName() const { return "iq"; }
    virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult Ping::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){

    JabberDataBlockRef ping=block->findChildNamespace("ping","urn:xmpp:ping");
    if (!ping) return BLOCK_REJECTED;

    Log::getInstance()->msg("Ping from ", block->getAttribute("from").c_str());

    JabberDataBlock pong("iq");
    pong.setAttribute("to", block->getAttribute("from"));
    pong.setAttribute("type", "result");
    pong.setAttribute("id", block->getAttribute("id"));

 
    rc->jabberStream->sendStanza(pong);
    return BLOCK_PROCESSED;
}
//////////////////////////////////////////////////////////////
class EntityTime : public JabberDataBlockListener {
public:
    EntityTime() {}
    ~EntityTime(){};
    virtual const char * getType() const{ return "get"; }
    virtual const char * getId() const{ return NULL; }
    virtual const char * getTagName() const { return "iq"; }
    virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult EntityTime::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){

    int rslt=BLOCK_REJECTED;
    JabberDataBlockRef time=block->findChildNamespace("time","urn:xmpp:time"); 
    if (time) {
 
        //Log::getInstance()->msg("Time query: ", block->getAttribute("from").c_str());

        PackedTime ct=strtime::getCurrentUtc();
        time->addChild("utc",strtime::toXep0080Time(ct).c_str());
        time->addChild("tzo",strtime::getLocalZoneOffset().c_str());
        rslt=BLOCK_PROCESSED;
    }

    JabberDataBlockRef query=block->findChildNamespace("query","jabber:iq:time"); 
	if (query) {
		//Log::getInstance()->msg("Time query: ", block->getAttribute("from").c_str());

		PackedTime ct=strtime::getCurrentUtc();
		query->addChild("utc",strtime::toIso8601(ct).c_str());
		query->addChild("display",strtime::toLocalDateTime(ct).c_str());
		query->addChild("tz",strtime::getLocalZoneOffset().c_str());
        rslt=BLOCK_PROCESSED;
	}

    if (rslt==BLOCK_REJECTED) return BLOCK_REJECTED;

    JabberDataBlock result("iq");
    result.setAttribute("to", block->getAttribute("from"));
    result.setAttribute("type", "result");
    result.setAttribute("id", block->getAttribute("id"));

    if (time) result.addChild(time);
    if (query) result.addChild(query);
    
    rc->jabberStream->sendStanza(result);
    return BLOCK_PROCESSED;
}
//////////////////////////////////////////////////////////////
class MessageRecv : public JabberDataBlockListener {
public:
	MessageRecv() {}
	~MessageRecv(){};
	virtual const char * getType() const{ return NULL; }
	virtual const char * getId() const{ return NULL; }
	virtual const char * getTagName() const { return "message"; }
	virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult MessageRecv::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){
    std::string from=block->getAttribute("from");
    const std::string & id=block->getAttribute("id");
    const std::string & body=block->getChildText("body");
    const std::string & subj=block->getChildText("subject");

 
    //JabberDataBlockRef xfwd=block->findChildNamespace("x","jabber:x:forward");
    //if (xfwd) {
	//  //old method
    //  std::string ofrom=xfwd->getChildText("from");
    //}

	std::string ofrom;
	std::string oto;
    // xep-0033 Extended stanza addressing
    JabberDataBlockRef addresses=block->findChildNamespace("addresses", "http://jabber.org/protocol/address");
    if (addresses && rc->myJid.getBareJid() == Jid(from).getBareJid() ) {
        JabberDataBlockRefList::iterator i=addresses->getChilds()->begin();
        while (i!=addresses->getChilds()->end()) {
            JabberDataBlockRef addr=*(i++);
            if (addr->getAttribute("type")=="ofrom") ofrom=addr->getAttribute("jid");
            if (addr->getAttribute("type")=="oto")   oto=addr->getAttribute("jid");
        }
    }

    if (ofrom.length()) from=ofrom;
    //StringRef orig=block->toXML();

    std::string nick;

    bool mucMessage= block->getAttribute("type")=="groupchat";
    Contact::ref c;
    if (mucMessage) {
        Jid roomNode;
        roomNode.setJid(from);

        MucGroup::ref roomGrp;
        roomGrp=boost::dynamic_pointer_cast<MucGroup> (rc->roster->findGroup(roomNode.getBareJid()));
        BOOST_ASSERT(roomGrp);
        if (!roomGrp) return BLOCK_PROCESSED;
        c=roomGrp->room;

        nick=roomNode.getResource();

    } else {
        c=rc->roster->getContactEntry(from);
        nick=c->getName();
    }

    //xep-085
    if (block->findChildNamespace("active", "http://jabber.org/protocol/chatstates")) {
        c->composing=false;
        c->acceptComposing=true;
    }
    if (block->findChildNamespace("paused", "http://jabber.org/protocol/chatstates")) {
        c->composing=false;
        c->acceptComposing=true;
    }
    if (block->findChildNamespace("composing", "http://jabber.org/protocol/chatstates")) {
        c->composing=true;
        c->acceptComposing=true;
		Notify::PlayNotify(2);
    }
	if (block->findChildNamespace("gone", "http://jabber.org/protocol/chatstates")) {
		c->composing=false;
        c->acceptComposing=false;
		
    }
    //end xep-0085

    //xep-0184
    if (Config::getInstance()->delivered) {
        if (block->findChildNamespace("request","urn:xmpp:receipts")) {
            // reply
            JabberDataBlock delivered("message");
            delivered.setAttribute("to", from);
            delivered.setAttribute("id",id);
            delivered.addChildNS("received","urn:xmpp:receipts");
            rc->jabberStream->sendStanza(delivered);
        }

        if (block->findChildNamespace("received","urn:xmpp:receipts")) {
            c->messageDelivered(id);
        }
    }
    //end of xep-0184

    //processing jabber:x:event - deprecated xep-0022
    JabberDataBlockRef x=block->findChildNamespace("x","jabber:x:event");
    if (x) {
        std::string xid=x->getChildText("id");
        //delivery notifications
        if (x->getChildByName("delivered"))   if (Config::getInstance()->delivered) {
            if (xid.empty()) {
				/*if (boost::dynamic_pointer_cast<MucContact>c) {
					if (c->status==presence.OFFLINE) return;
				}*/
                JabberDataBlock delivered("message");
                delivered.setAttribute("to", from);
                JabberDataBlockRef x=delivered.addChildNS("x", "jabber:x:event");
                x->addChild("id", block->getAttribute("id").c_str() );
                x->addChild("delivered", NULL);
                rc->jabberStream->sendStanza(delivered);
            } else {
                c->messageDelivered(xid);
            }
        }
        //composing events
        bool composing = false;
        if (x->getChildByName("composing")) {
            c->acceptComposing=true;
            composing=body.empty();
		
        }
		
        c->composing=composing;

        if (composing) {
            //todo: repaint
        }
    }
    // end of xep-0022

    Message::ref msg;

    if (body.length() || subj.length() ) {
        //constructing message and raising message event
        Log::getInstance()->msg("Message from ", from.c_str()); 
		if(body.length()){
        msg=Message::ref(new Message(body, nick, mucMessage, Message::INCOMING, Message::extractXDelay(block) ));
		}else{msg=Message::ref(new Message(subj, nick, mucMessage, Message::INCOMING, Message::extractXDelay(block) ));
		}

		
		


//��������� ;)
		
		if(Config::getInstance()->vsmess ){
   

		if ((hwnvs != GetActiveWindow()) || (Config::getInstance()->vstrymess)){

//std::wstring messn2;
std::wstring messn1;
//std::wstring mesfrom2;

char * cnotif=new char[55];
std::string cnotifs;
//messn2=utf8::utf8_wchar(msg->body.c_str());
char *format =" <body bgcolor=\"#%06X\"><br><b><font color=\"#%06X\">--%s--</font></b><br><font color=\"#%06X\">%s</font></body>";
messn1=L"<input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" >";
if (mucMessage  ) {if((Config::getInstance()->confchat)){
	
cnotifs=boost::str(boost::format(format) 
        % COLORS[13]
		% COLORS[14]
        % from
        % COLORS[15]
        % encloseHTML(msg->body.c_str())
        );
bufc=c;
messn1+=utf8::utf8_wchar(cnotifs);
SHNotificationRemove(&APP_GUID, NOTIFY_ID);

AddNotification(hwnvs,(LPCTSTR)messn1.c_str(),0);}
}else{ 

//std::string avatarsjid=c->rosterJid;
size_t i3=0;
                while (i3<c->rosterJid.length()) {
                    if (c->rosterJid[i3]=='/') {
                       c->rosterJid[i3]='.';
                        continue;
                    }
                    i3++;
				}
std::wstring filePathavatar5=appRootPath+L"userdata\\avatars\\"+utf8::utf8_wchar(c->rosterJid)+L".jpg";
HBITMAP bmp3=SHLoadImageFile(filePathavatar5.c_str());

filePathavatar5=L"file://"+filePathavatar5;

//std::wstring filePathavatar4=appRootPath+L"userdata\\avatars\\"+utf8::utf8_wchar(c->jid.getBareJid())+L".jpg";

//int result=MessageBox(NULL, filePathavatar5.c_str(), TEXT("�������"), MB_YESNOCANCEL | MB_ICONWARNING );
BITMAP bm3;
GetObject(bmp3, sizeof(bm3), &bm3);
LONG avataraWidth = Config::getInstance()->avatarWidth ;
LONG avWidth;
LONG avHeight;
if (bmp3) {if(bm3.bmWidth==bm3.bmHeight){avWidth=avataraWidth;
	avHeight=avWidth;}else{
	if(bm3.bmWidth>bm3.bmHeight){
	avWidth=avataraWidth;
    avHeight=(avataraWidth*bm3.bmHeight*100)/(100*bm3.bmWidth);
		}else{
	avHeight=avataraWidth;
	avWidth=(avataraWidth*bm3.bmWidth*100)/(100*bm3.bmHeight);
	}}
char *format2 = "<img  width=%d height=%d alt=\"";
cnotifs=boost::str(boost::format(format2) 
        % avWidth
		% avHeight
        );
messn1=utf8::utf8_wchar(cnotifs)+filePathavatar5+L"\" src=\""+filePathavatar5+L"\"  />"+L"<input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" >";
}

if (bmp3) DeleteObject(bmp3);
 //char *format2 =" <body bgcolor=\"#%06X\"><br><b><font color=\"#%06X\">--%s--</font></b><br><font color=\"#%06X\">%s</font></body>";
cnotifs=boost::str(boost::format(format) 
        % COLORS[16]
		% COLORS[17]
        % c->getFullName()
        % COLORS[18]
        % encloseHTML(msg->body.c_str())
        );
	 
messn1+= utf8::utf8_wchar(cnotifs);

	
//int result=MessageBox(NULL, messn1.c_str(), TEXT("�������"), MB_YESNOCANCEL | MB_ICONWARNING );

	bufc=c;
SHNotificationRemove(&APP_GUID, NOTIFY_ID);
AddNotification(hwnvs,(LPCTSTR)messn1.c_str(),0);
}
		}

		}
if (mucMessage) {Notify::PlayNotify(1);}else{
			Notify::PlayNotify(0);}
	}





    ChatView *cv = dynamic_cast<ChatView *>(tabs->getWindowByODR(c).get());
    bool ascroll=(cv==NULL)? false: cv->autoScroll();

    if (msg) {
        c->nUnread++;
        c->messageList->push_back(msg);
		if (Config::getInstance()->history)
		{
          if (mucMessage)
		  {
			  if (Config::getInstance()->saveHistoryMuc){
				  History::getInstance()->appendHistory(c, msg,true);
			  }
		  }else {
			History::getInstance()->appendHistory(c, msg,false);
		  }
	    }

        if (ascroll) /*if (cv)*/ {
            cv->moveEnd();
        }
        //tabs->switchByODR(c); 
    }

    //repainting
    if (rc->roster->needUpdateView) rc->roster->makeViewList();

    if (cv) { 
        if (IsWindowVisible(cv->getHWnd())) cv->redraw();
        InvalidateRect(tabs->getHWnd(), NULL, FALSE);
    }

    InvalidateRect(rosterWnd->getHWnd(),NULL, FALSE);

	return BLOCK_PROCESSED;
}

//////////////////////////////////////////////////////////////
class PresenceRecv : public JabberDataBlockListener {
public:
    PresenceRecv() {}
    ~PresenceRecv(){};
    virtual const char * getType() const{ return NULL; }
    virtual const char * getId() const{ return NULL; }
    virtual const char * getTagName() const { return "presence"; }
    virtual ProcessResult blockArrived(JabberDataBlockRef block, const ResourceContextRef rc);
};
ProcessResult PresenceRecv::blockArrived(JabberDataBlockRef block, const ResourceContextRef rc){
    std::string from=block->getAttribute("from");


    Contact::ref contact=rc->roster->getContactEntry(from);
    ChatView *cv = dynamic_cast<ChatView *>(tabs->getWindowByODR(contact).get());
    bool ascroll=(cv==NULL)? false: cv->autoScroll();

std::wstring statvs;
/*if(typevs=="offline"){statvs=L"<br>�����<br>";}else{*/
	
//}
    contact->processPresence(block);

	rc->roster->makeViewList();

	if(typevs=="chat" || typevs=="away" || typevs=="xa" || typevs=="dnd" || typevs=="online" || typevs=="offline"){
    statvs=L"<br>��������� ������:<br>"+utf8::utf8_wchar(typevs);
statusMessage2=encloseHTML(contact->getStatusMessage());
		if(typevs=="chat")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#53ff53\"><br><b><br><font color=\"#000000\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";
		if(typevs=="away")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#00cccc\"><br><b><font color=\"#000000\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";
		if(typevs=="xa")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#7975cb\"><br><b><font color=\"#000000\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";
		if(typevs=="dnd")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#cc0000\"><br><b><font color=\"#000000\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";
		if(typevs=="online")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#ffffff\"><br><b><font color=\"#000000\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";
		if(typevs=="offline")	statusMessage1=L"<br><input type=\"button\" value=\"�������\" name=\"cmd:42350\" ><input type=\"button\" value=\"�������\" name=\"cmd:700\" ><body bgcolor=\"#777777\"><br><b><font color=\"#ffffff\">"+utf8::utf8_wchar(contact->getFullName())+statvs+L" </b><br>"+utf8::utf8_wchar(statusMessage2)+L" </font></body>";


if (Config::getInstance()->vs_status && Config::getInstance()->vsmess){
if ((hwnvs != GetActiveWindow()) || (Config::getInstance()->vstrymess)){bufc=contact;
SHNotificationRemove(&APP_GUID, NOTIFY_ID);
AddNotification(hwnvs,(LPCTSTR)statusMessage1.c_str(),1);}}
	if(typevs=="offline"){Notify::PlayNotify(4);}else{Notify::PlayNotify(3);}
	}

	
    


    if (ascroll) /*if(cv)*/ {
        cv->moveEnd();
    }
    if (cv) if (IsWindowVisible(cv->getHWnd())) cv->redraw();

    return BLOCK_PROCESSED;
}


class JabberStreamEvents : public JabberListener{
public:
    JabberStreamEvents(ResourceContextRef rc) {this->rc=rc;}

    virtual bool connect();
    virtual void beginConversation(JabberDataBlockRef streamHeader);
    virtual void endConversation(const std::exception *ex);
    virtual void loginSuccess();
    virtual void loginFailed(const char * errMsg);

private:
    ResourceContextRef rc;

};

void JabberStreamEvents::beginConversation(JabberDataBlockRef streamHeader){
    if (streamHeader->getAttribute("version")=="1.0") {
        rc->jabberStanzaDispatcherRT->addListener(JabberDataBlockListenerRef(new SASLAuth(rc, streamHeader)));
    } else {
        rc->jabberStanzaDispatcherRT->addListener(JabberDataBlockListenerRef(new NonSASLAuth(rc, streamHeader)));
    }
}
void JabberStreamEvents::endConversation(const std::exception *ex){
    if (ex!=NULL)  Log::getInstance()->msg(ex->what());
    Log::getInstance()->msg("End Conversation");
    rc->roster->setAllOffline();
    rc->roster->makeViewList();
    //tabs->

    rosterWnd->setIcon(presence::OFFLINE);
}

void setSocketError(int n)
{
	socketError = n;
}

void JabberStreamEvents::loginSuccess(){
    Log::getInstance()->msg("Login ok");

    HostFeatures::discoverFeatures(rc);
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new GetRoster() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new ProcessMuc(rc) ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new PresenceRecv() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new CaptchaListener() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new MessageRecv() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new Version() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new Ping() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new LastActivity() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new EntityTime() ));
    rc->jabberStanzaDispatcherRT->addListener( JabberDataBlockListenerRef( new EntityCaps() ));
    LastActivity::update();

    JabberDataBlock getRoster("iq");
    getRoster.setAttribute("type","get");
    getRoster.setAttribute("id","roster");

    getRoster.addChildNS("query", "jabber:iq:roster");

    rc->jabberStream->sendStanza(getRoster);
    rc->bookmarks->doQueryBookmarks(rc);
	reconnectTry = Config::getInstance()->reconnectTries;

}

void JabberStreamEvents::loginFailed(const char * errMsg){
    Log::getInstance()->msg("Login failed: ", errMsg);
    rc->jabberStream->sendXmppEndHeader();
    rosterWnd->setIcon(icons::ICON_ERROR_INDEX);
}

bool JabberStreamEvents::connect(){

    Socket::initWinsocks();

    if (rc->account->networkUp) {
        Log::getInstance()->msg("Raising up network");
        Socket::networkUp();
    }

    std::string host;
    int port=5222;

    if (rc->account->useSRV) {
        Log::getInstance()->msg("Searching SRV for ", rc->account->getServer().c_str() );

        dns::DnsSrvQuery d;
        int retries=3;
        do { 
            if (d.doQuery( "_xmpp-client._tcp."+rc->account->getServer())) {
                if (d.getCount()>0) {
                    dns::SRVAnswer::ref a=d.getResult(0);
                    BOOST_ASSERT(a);

                    host=a->target;
                    port=a->port;

                    Log::getInstance()->msg(boost::str(boost::format("Using %s:%d") % host.c_str() % port));
                    break;
                }
            }
        } while (retries--) ;
    } else {
        host=rc->account->hostNameIp;
        port=rc->account->port;
    }

    if (host.empty()) host=rc->account->getServer();

    if (rc->account->legacySSL && rc->account->useEncryption) {
        if (port==5222) port=5223;
    }

    Log::getInstance()->msg("Resolving ", host.c_str());

    long ip=Socket::resolveUrl(host);

    Log::getInstance()->msg(boost::str(boost::format("Connecting to %u.%u.%u.%u:%u") 
        % (ip &0xff) % ((ip>>8) &0xff) % ((ip>>16) &0xff) % ((ip>>24)&0xff) % port));

    if (rc->account->useEncryption) {
        CeTLSSocket::ref tlsCon=CeTLSSocket::ref( new CeTLSSocket(ip, port));
        rc->jabberStream->connection=tlsCon;
        if (!rc->account->useSASL) 
            tlsCon->startTls(rc->account->getServer(), rc->account->ignoreSslWarnings);
    }
    else
        rc->jabberStream->connection=ConnectionRef( new Socket(ip, port));

    /*if (rc->jabberStream->connection==NULL) {
        Log::getInstance()->msg("Failed to open connection");
        return false;
    }
    BOOST_ASSERT(rc->jabberStream->connection);
    */

    rc->jabberStream->sendXmlVersion();
    rc->jabberStream->sendXmppBeginHeader();

    return true;
}
//////////////////////////////////////////////////////////////
int prepareAccount(){
    rc=ResourceContextRef(new ResourceContext());
    //Log::getInstance()=new Log();
    //rc->account=JabberAccountRef(new JabberAccount("bombus_mobilus@jivesoftware.com", "bombus-ng"));
    //rc->account->hostNameIp="213.180.203.19";
    //rc->account->password="l12sx95a";

    rc->account=JabberAccountRef(new JabberAccount(TEXT("defAccount.bin")));
    rc->myJid.setJid( rc->account->getJid() );

    //rc->account->useSASL=true;
    //rc->account->useEncryption=true;
    //rc->account->useCompression=true;
    return 0;
}

//////////////////////////////////////////////////////////////
// TODO: refactoring: move into ResourceContext
//////////////////////////////////////////////////////////////
int initJabber(ResourceContextRef rc) {
    if (rc->jabberStream ) return 1;
    rc->jabberStanzaDispatcherRT=JabberStanzaDispatcherRef(new JabberStanzaDispatcher(rc));
    rc->jabberStanzaDispatcher2=JabberStanzaDispatcherRef(new JabberStanzaDispatcher(rc));

    //TODO: roster caching
    if (!rc->roster)
        rc->roster=RosterRef(new Roster(rc));
    rc->roster->bindWindow(rosterWnd);
    rosterWnd->setIcon(icons::ICON_PROGRESS_INDEX);
    rosterWnd->roster=rc->roster;

    rc->bookmarks=MucBookmarksRef(new MucBookmarks());

    rc->jabberStream=JabberStreamRef(new JabberStream(rc, JabberListenerRef(new JabberStreamEvents(rc))));

	return 0;
}
void reconnect()//������� ��������
{
	if (socketError)
		if (reconnectTry > 0)
		{
			reconnectTry--;
			socketError = 0;
			Notify::PlayNotify(5);

			initJabber(rc);
		}
}

//////////////////////////////////////////////////////////////
// TODO: refactoring: move into ResourceContext
//////////////////////////////////////////////////////////////////////////
void streamShutdown(ResourceContextRef rc){
    if (!rc->jabberStream) return;
    rc->jabberStream->sendXmppEndHeader();
}


NOTIFYICONDATA nid={0};

void Shell_NotifyIcon(bool show, HWND hwnd){
    // This code will add a Shell_NotifyIcon notificaion on PocketPC and Smartphone
    
    if (show) {
        nid.cbSize = sizeof(nid);
        nid.uID = 100;      // Per WinCE SDK docs, values from 0 to 12 are reserved and should not be used.
        nid.uFlags = NIF_ICON | NIF_MESSAGE;
        nid.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_BOMBUS));
        nid.uCallbackMessage=SHELLNOTIFYICON;
        nid.hWnd=hwnd;

        //Add the notification to the tray
        Shell_NotifyIcon(NIM_ADD, &nid);
    } else {
        //remove the notification from the tray
        Shell_NotifyIcon(NIM_DELETE, &nid);
        DeleteObject(nid.hIcon);
    }

    //Update the icon of the notification
    /*nid.uFlags = NIF_ICON;
    nid.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_SAMPLEICON2));
    Shell_NotifyIcon(NIM_MODIFY, &nid);*/


    return;
}


void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{//������
	if(Config::getInstance()->tune_status || Config::getInstance()->tune_status_pep){
	
	if(rosterStatus){

		if(regmuz_mp()){
			
			
			if(aktiv_wp_s==1){
				if( aktiv_wp_sc>=AKTIV_PLAY){ vyvod_title();aktiv_wp_sc_flag=0;aktiv_wp_s=2;}}else{
				if(aktiv_wp_s==2){
					if(aktiv_wp_sc_not>=AKTIV_PLAY || out_OriginalArtist_st!=out_OriginalArtist || out_title_st!=out_title){if(aktiv_wp_sc_not>=AKTIV_PLAY)sbros_title();
					aktiv_wp_sc_flag=1;aktiv_wp_s=1;aktiv_wp_sc=0;
					}
			
				}}
		}
	}}


	//����������
presence::PresenceIndex avnstatusconf;


timaliv=timaliv+2;
if(Config::getInstance()->avtostatus){
timstatus=timstatus+2;
if(Config::getInstance()->id_avtostatus==1){avnstatusconf=presence::ONLINE;}else{
	if(Config::getInstance()->id_avtostatus==2){avnstatusconf=presence::CHAT;}else{
		if(Config::getInstance()->id_avtostatus==3){avnstatusconf=presence::AWAY;}else{
			if(Config::getInstance()->id_avtostatus==4){avnstatusconf=presence::XA;}else{
			if(Config::getInstance()->id_avtostatus==5){avnstatusconf=presence::DND;}}}}
}
if(timstatus>=(Config::getInstance()->time_avtostatus)){
	timstatus=0;

if(idautostatus==0 &&(rosterStatus)){
	idautostatus=1;

	//rc->status=presence::AWAY;
  Log::getInstance()->msg("Autostatus ");
//s.streamString(rc->presenceMessage, "");
  pamessage=(rc->presenceMessage);
  astatus=(rc->status);
					rc->status=avnstatusconf;
					
					rosterWnd->setIcon(rc->status);
					//std::string t=strtime::getLocalZoneOffset();
					rc->presenceMessage=Config::getInstance()->avtomessage+strtime::toLocalTime(strtime::getCurrentUtc());
               
					rc->sendPresence();
				rc->roster->setMUCStatus(avnstatusconf);
		}
			}

if(idautostatus==2){
	idautostatus=0;
	
  
					rc->status=astatus;//��������������
					rosterWnd->setIcon(rc->status);
					rc->presenceMessage=pamessage;
               
					rc->sendPresence();
				rc->roster->setMUCStatus(astatus);
}}
std::string idVer;


//�������
///�� -�� �����-����� ���������
if(timealivid  ){if(timaliv>=(TIMER_ALIV+TIMER_ALIVP)){
timealivid=0;//��������
timaliv=0;
Log::getInstance()->msg("ERROR pong ",rc->account->getServer().c_str());
//��� ����� ���� �������� ��� ���� �� �����
//MessageBox(mainWnd,  TEXT("������ ����� ������������� ���������"),TEXT("������ "), 0);
Notify::PlayNotify(5);Notify::PlayNotify(5);Notify::PlayNotify(5);
if (reconnectTry > 0)
		{Notify::PlayNotify(5);
			/*reconnectTry--;
	
			
			rc->status=presence::OFFLINE;
rosterWnd->setIcon(rc->status);

if (rc->roster)rc->roster->setMUCStatus(presence::OFFLINE);
rosterStatus=0;
std::exception ex;
//

rc->roster->setAllOffline();

    rc->roster->makeViewList();
   
rc->sendPresence();
   socketError = 1;
rc->jabberStream->sendXmppEndHeader();
streamShutdown(rc); rc->jabberStream->connection->close();

initJabber(rc);
rc->status=presence::ONLINE;
rc->sendPresence();
        initJabber(rc);
//JabberStream * _stream;
rc->jabberStream->jabberListener->endConversation(&ex);
rc->jabberStream->isRunning=false;

JabberStream::run((JabberStream *)&(rc->jabberStream));
JabberStream::run(NULL); 

rc->status=presence::ONLINE;
rc->sendPresence();
        initJabber(rc);*/

}

}}

if(!timealivid &&(rosterStatus)){if(timaliv>=TIMER_ALIV){timealivid=1;//������� ����
JabberDataBlockRef qry;
Log::getInstance()->msg("ping ",rc->account->getServer().c_str());
JabberDataBlock req("iq");
    req.setAttribute("to", rc->account->getServer().c_str());
    req.setAttribute("type", "get");

    req.setAttribute("id", idVer=strtime::getRandom() );
    qry=req.addChildNS("ping", "jabber:iq:version");
	rc->jabberStream->sendStanza(req);}
}


//int result=MessageBox(NULL, TEXT("����� ����"), TEXT("������� URL?2"), MB_YESNO | MB_ICONWARNING );

}