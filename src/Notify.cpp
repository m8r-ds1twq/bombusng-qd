#include "Notify.h"

#include "config.h"

#include <nled.h>

//#include <Pwinuser.h>
// 
// Defines and structures taken from pwingdi.h in the Platform Builder 
// 
#define QUERYESCSUPPORT 8 
#define SETPOWERMANAGEMENT 6147 
#define GETPOWERMANAGEMENT 6148 

typedef enum _VIDEO_POWER_STATE { 
VideoPowerOn = 1, 
VideoPowerStandBy, 
VideoPowerSuspend, 
VideoPowerOff 
} VIDEO_POWER_STATE, *PVIDEO_POWER_STATE; 

typedef struct _VIDEO_POWER_MANAGEMENT { 
ULONG Length; 
ULONG DPMSVersion; 
ULONG PowerState; 
} VIDEO_POWER_MANAGEMENT, *PVIDEO_POWER_MANAGEMENT; 

//---------------------------------------------------------------------- 
// SetVideoPower - Turns on or off the display 
// 
int SetVideoPower (BOOL fOn) { 
VIDEO_POWER_MANAGEMENT vpm; 
int rc, fQueryEsc; 
HDC hdc; 

// Get the display dc. 
hdc = GetDC (NULL); 
// See if supported. 
fQueryEsc = SETPOWERMANAGEMENT; 
rc = ExtEscape (hdc, QUERYESCSUPPORT, sizeof (fQueryEsc), 
(LPSTR)&fQueryEsc, 0, 0); 
if (rc == 0) { 
// No support, fail. 
ReleaseDC (NULL, hdc); 
return -1; 
} 
// Fill in the power management structure. 
vpm.Length = sizeof (vpm); 
vpm.DPMSVersion = 1; 
if (fOn) 
vpm.PowerState = VideoPowerOn; 
else 
vpm.PowerState = VideoPowerOff; 

// Tell the driver to turn on or off the display. 
rc = ExtEscape (hdc, SETPOWERMANAGEMENT, sizeof (vpm), 
(LPSTR)&vpm, 0, 0); 

// Always release what you get. 
ReleaseDC (NULL, hdc); 
return 0; 
} 


extern "C" { 
    BOOL WINAPI NLedGetDeviceInfo( UINT nInfoId, void *pOutput ); 
    BOOL WINAPI NLedSetDevice( UINT nDeviceId, void *pInput ); 
};

void doSmartPhoneVibra();


#ifdef _WIN32_WCE
DWORD vibraThread(LPVOID param) {
#else
DWORD WINAPI vibraThread(LPVOID param) {
#endif

    NLED_SETTINGS_INFO nsi;
    nsi.LedNum=1;
    nsi.OnTime=1000;
    nsi.OffTime=300;
    nsi.TotalCycleTime=1300;
    nsi.MetaCycleOn=2;
    nsi.MetaCycleOff=2;

    nsi.OffOnBlink=1;

    NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
    Sleep(400);

    nsi.OffOnBlink=0;
    NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
    Sleep(200);

    Notify::vibraOn=FALSE;

    return 1;
}

extern std::wstring appRootPath;


void Notify::PlayNotify(int soundst) {//0-message.wav 1-conference.wav 2-composing.wav 3-connected.wav 4-disconnected.wav 5-reconnected.wav
    //doSmartPhoneVibra();
   int vid;
    std::wstring soundName(appRootPath);
	std::wstring soundName2(appRootPath);
	std::wstring soundName3(appRootPath);
	std::wstring soundName4(appRootPath);
	std::wstring soundName5(appRootPath);
	std::wstring soundName6(appRootPath);
    soundName+=TEXT("sounds\\message.wav");
    soundName2+=TEXT("sounds\\conference.wav");
	soundName3+=TEXT("sounds\\composing.wav");
	soundName4+=TEXT("sounds\\connected.wav");
	soundName5+=TEXT("sounds\\disconnected.wav");
	soundName6+=TEXT("sounds\\reconnected.wav");
	if(Config::getInstance()->blink){
if(soundst==1){
vid=SetVideoPower(TRUE);Sleep(200);
		 vid=SetVideoPower(FALSE);
		 if(!(Config::getInstance()->blink2)){
		 Sleep(200);
		 vid=SetVideoPower(TRUE);}
		 
		Sleep(0);}
if(soundst==0){
		vid=SetVideoPower(TRUE);
		Sleep(200);
		vid=SetVideoPower(FALSE);Sleep(200);
		 
		vid=SetVideoPower(TRUE);
		Sleep(200);
		 vid=SetVideoPower(FALSE);
		 if(!(Config::getInstance()->blink2)){Sleep(200);
		 vid=SetVideoPower(TRUE);}
		 Sleep(0);
	}
	}
	if (Config::getInstance()->sounds){
	switch (soundst) {
			case 0:
				PlaySound(soundName.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);
			 break;
			 case 1:
				PlaySound(soundName2.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);
			 break;
			 case 2:
				 if (Config::getInstance()->scomposing){PlaySound(soundName3.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);}
			 break;
			 case 3:
				 if (Config::getInstance()->sounds_status){PlaySound(soundName4.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);}
			 break;
			 case 4:
				 if (Config::getInstance()->sounds_status){PlaySound(soundName5.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);}
			 break;
			 case 5:
				PlaySound(soundName6.c_str(), NULL, SND_ASYNC | /*SND_NOWAIT |*/SND_FILENAME);
			 break;
	}
	}

    if (Notify::vibraOn) return;

    if (!(Config::getInstance()->vibra)) return;

	switch (soundst) { 
		case 2:
		case 3:
		case 4:
		case 5:
		return;}
    Notify::vibraOn=TRUE;
    HANDLE thread=CreateThread(NULL, 0, vibraThread, NULL, 0, NULL);
    SetThreadPriority(thread, THREAD_PRIORITY_IDLE);
}

BOOL Notify::vibraOn=FALSE;


typedef struct
{
    WORD wDuration;
    BYTE bAmplitude;  
    BYTE bFrequency;
} VIBRATENOTE; 

void doSmartPhoneVibra() {
    HINSTANCE hInst = LoadLibrary(_T("aygshell.dll"));
    if (hInst) {
        HRESULT (*Vibrate)(DWORD cvn, const VIBRATENOTE * rgvn, BOOL fRepeat, DWORD dwTimeout);
        (FARPROC&)Vibrate = GetProcAddress(hInst, _T("Vibrate"));
        if (Vibrate) {
            HRESULT retval=Vibrate(0, NULL, true, 2000);
            retval++;
        }
        FreeLibrary(hInst);
    }
}