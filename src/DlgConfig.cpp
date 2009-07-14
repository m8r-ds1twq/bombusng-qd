//#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <aygshell.h>

#include "ResourceContext.h"
#include "JabberAccount.h"

#include "DlgConfig.h"
#include "DlgUtils.h"
#include "config.h"

#include "..\vs2005\ui\resourceppc.h"

#include "utf8.hpp"
#include "stringutils.h"

//////////////////////////////////////////////////////////////////////////////////
wchar_t *statusNames2 []= { TEXT(" "),
    TEXT("Online"),         TEXT("Free for chat"),  TEXT("Away"), 
    TEXT("Extended Away"),  TEXT("DND"),            TEXT("Offline") 
};

INT_PTR CALLBACK DlgProcConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int npage);
INT_PTR CALLBACK DlgProcConfigP1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return DlgProcConfig(hDlg, message, wParam, lParam, 0);
}
INT_PTR CALLBACK DlgProcConfigP2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return DlgProcConfig(hDlg, message, wParam, lParam, 1);
}
INT_PTR CALLBACK DlgProcConfigP3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return DlgProcConfig(hDlg, message, wParam, lParam, 2);
}
INT_PTR CALLBACK DlgProcConfigP4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return DlgProcConfig(hDlg, message, wParam, lParam, 3);
}

INT_PTR CALLBACK DlgProcConfigP5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return DlgProcConfig(hDlg, message, wParam, lParam, 4);
}
void DialogConfigMP(HINSTANCE g_hInst, HWND parent) {

    PROPSHEETPAGE pages[5];
    pages[0].dwSize=sizeof(PROPSHEETPAGE);
    pages[1].dwSize=sizeof(PROPSHEETPAGE);
    pages[2].dwSize=sizeof(PROPSHEETPAGE);
    pages[3].dwSize=sizeof(PROPSHEETPAGE);
	pages[4].dwSize=sizeof(PROPSHEETPAGE);
    pages[0].hInstance=g_hInst;
    pages[1].hInstance=g_hInst;
    pages[2].hInstance=g_hInst;
    pages[3].hInstance=g_hInst;
    pages[4].hInstance=g_hInst;

    pages[0].dwFlags=PSP_DEFAULT;
    pages[1].dwFlags=PSP_DEFAULT;
    pages[2].dwFlags=PSP_DEFAULT;
    pages[3].dwFlags=PSP_DEFAULT;
    pages[4].dwFlags=PSP_DEFAULT;

    pages[0].pszTemplate=(LPCTSTR)IDD_OPTIONS1;
    pages[1].pszTemplate=(LPCTSTR)IDD_OPTIONS2;
    pages[2].pszTemplate=(LPCTSTR)IDD_OPTIONS3;
    pages[3].pszTemplate=(LPCTSTR)IDD_OPTIONS4;
    pages[4].pszTemplate=(LPCTSTR)IDD_STATUSY;

    pages[0].pfnDlgProc=DlgProcConfigP1;
    pages[1].pfnDlgProc=DlgProcConfigP2;
    pages[2].pfnDlgProc=DlgProcConfigP3;
    pages[3].pfnDlgProc=DlgProcConfigP4;
    pages[4].pfnDlgProc=DlgProcConfigP5;

    pages[0].lParam=0;
    pages[1].lParam=1;
    pages[2].lParam=2;
    pages[3].lParam=3;
    pages[4].lParam=4;

    PROPSHEETHEADER psh;
    psh.dwSize=sizeof(PROPSHEETHEADER);
    psh.dwFlags=PSH_MAXIMIZE | PSH_PROPSHEETPAGE | PSH_USECALLBACK;
    psh.hwndParent=parent;
    psh.hInstance=g_hInst;
    psh.pszCaption=L"Options";
    psh.nPages=5;
    psh.nStartPage=0;
    psh.ppsp=pages;
	psh.pfnCallback = PropSheetCallback;

    PropertySheet(&psh);

    Config::getInstance()->save();
}


INT_PTR CALLBACK DlgProcConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int npage) {
    switch (message) {
    case WM_INITDIALOG:
        {
            // Create a Done button and size it.  
            SHINITDLGINFO shidi;
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
            shidi.hDlg = hDlg;
            //SHInitDialog(&shidi);

            Config::ref cfg=Config::getInstance();

            if (npage==0) {
                SetDlgCheckBox(hDlg, IDC_X_OFFLINES, cfg->showOfflines);
                SetDlgCheckBox(hDlg, IDC_X_GROUPS, cfg->showGroups);
                
                SetDlgCheckBox(hDlg, IDC_X_CLIENT, cfg->confclient);
				SetDlgCheckBox(hDlg, IDC_XML_LOG, cfg->xmllog);
				SetDlgItemInt(hDlg, IDC_X_AWAT,cfg->avatarWidth, false);
				

				
            }
            if (npage==1) {
                SetDlgCheckBox(hDlg, IDC_X_COMPOSING, cfg->composing);
                SetDlgCheckBox(hDlg, IDC_X_DELIVERY, cfg->delivered);
                SetDlgCheckBox(hDlg, IDC_X_HISTORY, cfg->history);
                SetDlgCheckBox(hDlg, IDC_X_SIP, cfg->raiseSIP);

                SetDlgCheckBox(hDlg, IDM_SAVE_HISTORY_MUC, cfg->saveHistoryMuc);
                SetDlgCheckBox(hDlg, IDM_SAVE_HISTORY_IN_HTML, cfg->saveHistoryHtml);
				SetDlgCheckBox(hDlg, IDC_EDITX2, cfg->editx2);
				
				SetDlgCheckBox(hDlg, IDC_X_HIS_CH_D, cfg->his_ch_d);
				SetDlgCheckBox(hDlg, IDC_X_HIS_MUCD, cfg->his_muc_d);

            }
            if (npage==2) {
                SetDlgCheckBox(hDlg, IDC_X_VIBRA, cfg->vibra);
                SetDlgCheckBox(hDlg, IDC_X_SOUNDS, cfg->sounds);
				SetDlgCheckBox(hDlg, IDC_SOUNDCOM, cfg->scomposing);
				SetDlgCheckBox(hDlg, IDC_X_BLINK1, cfg->blink);
				SetDlgCheckBox(hDlg, IDC_VS_STATUS, cfg->vs_status);
				SetDlgCheckBox(hDlg, IDC_SOUND_STATUS, cfg->sounds_status);
				SetDlgCheckBox(hDlg, IDC_X_BLINK2, cfg->blink2);
				SetDlgCheckBox(hDlg, IDC_X_VSMESS, cfg->vsmess);
				SetDlgCheckBox(hDlg, IDC_X_VSTRYMESS, cfg->vstrymess);
				SetDlgCheckBox(hDlg, IDC_VS_CONFCHAT, cfg->confchat);

			}
            if (npage==3) {
				SetDlgItemInt(hDlg, IDC_PINGAKIV ,cfg->ping_aliv, false);
				SetDlgItemInt(hDlg, IDC_PONGALIV ,cfg->pong_aliv, false);
                SetDlgCheckBox(hDlg, IDC_X_AUTOCONNECT, cfg->connectOnStartup);
				SetDlgItemInt(hDlg, IDC_TAB_CONF, cfg->tabconf , false);
				SetDlgItemInt(hDlg, IDC_RECONNECT_TRIES, cfg->reconnectTries , false);
				SetDlgItemInt(hDlg, IDC_X_MSG_FONT_HEIGHT, cfg->msg_font_height , false);
				SetDlgItemInt(hDlg, IDC_X_MSG_FONT_WIDTH,cfg->msg_font_width, false);
				SetDlgItemInt(hDlg, IDC_X_MSG_FONT_TOLSH,cfg->tolshina, false);

				SetDlgItemInt(hDlg, IDC_X_ROSTER_FONT_HEIGHT, cfg->roster_font_height, false);
				SetDlgItemInt(hDlg, IDC_X_ROSTER_FONT_WIDTH, cfg->roster_font_width, false);
				/*
				EDITTEXT        IDC_X_MSG_FONT_HEIGHT,88,31,18,10,ES_LOWERCASE | ES_AUTOHSCROLL
				EDITTEXT        IDC_X_MSG_FONT_WIDTH,88,43,18,10,ES_AUTOHSCROLL | ES_NUMBER
				EDITTEXT        IDC_X_ROSTER_FONT_HEIGHT,88,61,18,10,ES_LOWERCASE | ES_AUTOHSCROLL
				EDITTEXT        IDC_X_ROSTER_FONT_WIDTH,88,73,18,10,ES_AUTOHSCROLL | ES_NUMBER
				*/
            }
			if (npage==4) {
				SetDlgCheckBox(hDlg, IDC_AUTOJOINROOM, cfg->autojoinroom);				
				SetDlgCheckBox(hDlg, IDC_X_PRESENCESORT, cfg->sortByStatus);
				SetDlgCheckBox(hDlg, IDC_AVTOSTATUS, cfg->avtostatus);
				SetDlgItemInt(hDlg, IDC_TIME_AVTOSTATUS,cfg->time_avtostatus, false);
 for (int i=0; i<6; i++)
                SendDlgItemMessage(hDlg,IDC_ID_AVTOSTATUS, CB_ADDSTRING, 0, (LPARAM) statusNames2[i]);
            SendDlgItemMessage(hDlg, IDC_ID_AVTOSTATUS, CB_SETCURSEL, cfg->id_avtostatus, 0);
            SetDlgItemText(hDlg, IDC_AVTOSTATUS_MESS, cfg->avtomessage);
			SetDlgCheckBox(hDlg, IDM_SHOW_STATUS_IN_MUC, cfg->showMucPresences);
                SetDlgCheckBox(hDlg, IDM_SHOW_STATUS_IN_SIMPLE_CHAT, cfg->showStatusInSimpleChat);
				SetDlgCheckBox(hDlg, IDC_TUN_STATUS, cfg->tune_status);
				SetDlgCheckBox(hDlg, IDC_TUN_PEP, cfg->tune_status_pep);
			
			}
            //finally
        }
        return (INT_PTR)TRUE;

    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam; 
            if (pnmh->code == PSN_APPLY) {

                Config::ref cfg=Config::getInstance();

                if (npage==0) {BOOL awat1;
                    GetDlgCheckBox(hDlg, IDC_X_OFFLINES, cfg->showOfflines);
                    GetDlgCheckBox(hDlg, IDC_X_GROUPS, cfg->showGroups);

                    GetDlgCheckBox(hDlg, IDC_X_CLIENT, cfg->confclient);
					GetDlgCheckBox(hDlg, IDC_XML_LOG, cfg->xmllog);
					
					cfg->avatarWidth = GetDlgItemInt(hDlg, IDC_X_AWAT, &awat1 , false);
					if (!awat1) cfg->avatarWidth = 50;

				
                }
                if (npage==1) {
                    GetDlgCheckBox(hDlg, IDC_X_COMPOSING, cfg->composing);
                    GetDlgCheckBox(hDlg, IDC_X_DELIVERY, cfg->delivered);
                    GetDlgCheckBox(hDlg, IDC_X_HISTORY, cfg->history);
                    GetDlgCheckBox(hDlg, IDC_X_SIP, cfg->raiseSIP);
					
                    GetDlgCheckBox(hDlg, IDM_SAVE_HISTORY_MUC, cfg->saveHistoryMuc);
					GetDlgCheckBox(hDlg, IDM_SAVE_HISTORY_IN_HTML, cfg->saveHistoryHtml);
					GetDlgCheckBox(hDlg, IDC_EDITX2, cfg->editx2);
					
					GetDlgCheckBox(hDlg, IDC_X_HIS_MUCD, cfg->his_muc_d);
					GetDlgCheckBox(hDlg, IDC_X_HIS_CH_D, cfg->his_ch_d);
					

                }
                if (npage==2) {
                    GetDlgCheckBox(hDlg, IDC_X_VIBRA, cfg->vibra);
                    GetDlgCheckBox(hDlg, IDC_X_SOUNDS, cfg->sounds);
					GetDlgCheckBox(hDlg, IDC_SOUNDCOM, cfg->scomposing);
					GetDlgCheckBox(hDlg, IDC_X_BLINK1, cfg->blink);
					GetDlgCheckBox(hDlg, IDC_VS_STATUS, cfg->vs_status);
					GetDlgCheckBox(hDlg, IDC_SOUND_STATUS, cfg->sounds_status);
					GetDlgCheckBox(hDlg, IDC_X_BLINK2, cfg->blink2);
					GetDlgCheckBox(hDlg, IDC_X_VSMESS, cfg->vsmess);
					GetDlgCheckBox(hDlg, IDC_X_VSTRYMESS, cfg->vstrymess);
					GetDlgCheckBox(hDlg, IDC_VS_CONFCHAT, cfg->confchat);
                }
                if (npage==3) {
                    GetDlgCheckBox(hDlg, IDC_X_AUTOCONNECT, cfg->connectOnStartup);

					BOOL f1Int,f2Int,f3Int,f4Int,reconint,tabc2,tol2,pingal1,pongal2;
					extern int tabHeight;

					tabHeight-=cfg->tabconf;


					cfg->tabconf = GetDlgItemInt(hDlg,  IDC_TAB_CONF, &tabc2 , false);
					    if (!tabc2) cfg->tabconf = 12;
						tabHeight+=cfg->tabconf;
					cfg->reconnectTries = GetDlgItemInt(hDlg, IDC_RECONNECT_TRIES, &reconint , false);
					    if (!reconint) cfg->reconnectTries = 3;

					cfg->tolshina = GetDlgItemInt(hDlg, IDC_X_MSG_FONT_TOLSH, &tol2 , false);
					    if (!tol2) cfg->tolshina = 400;

					cfg->msg_font_height = GetDlgItemInt(hDlg, IDC_X_MSG_FONT_HEIGHT, &f1Int , false);
					    if (!f1Int) cfg->msg_font_height = 14;
					cfg->msg_font_width = GetDlgItemInt(hDlg, IDC_X_MSG_FONT_WIDTH, &f2Int , false);
					    if (!f2Int) cfg->msg_font_width = 5;

					cfg->roster_font_height = GetDlgItemInt(hDlg, IDC_X_ROSTER_FONT_HEIGHT, &f3Int , false);
					    if (!f3Int) cfg->roster_font_height = 13;
					cfg->roster_font_width = GetDlgItemInt(hDlg, IDC_X_ROSTER_FONT_WIDTH, &f4Int , false);
					    if (!f4Int) cfg->roster_font_width = 5;

						cfg->ping_aliv = GetDlgItemInt(hDlg, IDC_PINGAKIV, &pingal1 , false);
					if (!pingal1) cfg->ping_aliv = 150;

					cfg->pong_aliv = GetDlgItemInt(hDlg, IDC_PONGALIV, &pongal2 , false);
					if (!pongal2) cfg->pong_aliv = 90;

				}
				if (npage==4) {BOOL stat2;
				GetDlgCheckBox(hDlg, IDC_AUTOJOINROOM, cfg->autojoinroom);
				GetDlgCheckBox(hDlg, IDC_X_PRESENCESORT, cfg->sortByStatus);
				GetDlgCheckBox(hDlg, IDC_AVTOSTATUS, cfg->avtostatus);
					cfg->time_avtostatus = GetDlgItemInt(hDlg, IDC_TIME_AVTOSTATUS, &stat2 , false);
					if (!stat2) cfg->time_avtostatus = 300;

					cfg->id_avtostatus =(int) SendDlgItemMessage(hDlg, IDC_ID_AVTOSTATUS, CB_GETCURSEL, 0,0);
						
					if (!(cfg->id_avtostatus)) cfg->id_avtostatus = 3;
					GetDlgItemText(hDlg, IDC_AVTOSTATUS_MESS, cfg->avtomessage);
					if(!(cfg->avtomessage.c_str())){cfg->avtomessage="Autostatus: ";}
					GetDlgCheckBox(hDlg, IDC_TUN_PEP, cfg->tune_status_pep);
					GetDlgCheckBox(hDlg, IDC_TUN_STATUS, cfg->tune_status);
                    GetDlgCheckBox(hDlg, IDM_SHOW_STATUS_IN_MUC, cfg->showMucPresences);
                    GetDlgCheckBox(hDlg, IDM_SHOW_STATUS_IN_SIMPLE_CHAT, cfg->showStatusInSimpleChat);

				
				}
                return TRUE;
            }
            return FALSE;
        }

    case WM_COMMAND:

        if (LOWORD(wParam) == IDCANCEL) {
            return TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
