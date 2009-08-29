#include "Sysinfo.h"

#include <windows.h>
#include "boostheaders.h"
#include "utf8.hpp"

const std::string sysinfo::getOsVersion() {


    //SystemParametersInfo(SPI_GETPLATFORMTYPE, 128, buf, 0);  
    //os << buf << " ";

    OSVERSIONINFO osv;
    GetVersionEx(&osv);

    wchar_t buf[256];
    SystemParametersInfo(SPI_GETOEMINFO, sizeof(buf)/sizeof(wchar_t), buf, 0);  
    const std::string & platform=utf8::wchar_utf8( std::wstring(buf));

    char *format = "%s (WCE%d.%d.%d)/%s memoryload=%d %%  total/free=%dMb/%dMb ";
    char *name="Unknown";

    if (osv.dwMajorVersion<=3) name="PocketPC 2002";
    if (osv.dwMajorVersion==4 && osv.dwMinorVersion<21) name="WindowsMobile 2003";
    if (osv.dwMajorVersion==4 && osv.dwMinorVersion>=21) name="WindowsMobile 2003 SE";
    if (osv.dwMajorVersion==5 && osv.dwMinorVersion==1) name="WindowsMobile 5";
    if (osv.dwMajorVersion==5 && osv.dwMinorVersion==2) name="WindowsMobile 6/6.1";
LPMEMORYSTATUS lpBuffer;
lpBuffer->dwLength=sizeof(MEMORYSTATUS);
GlobalMemoryStatus( lpBuffer);
unsigned long Total=((unsigned long)lpBuffer->dwTotalPhys)>>20;
unsigned long Avail=((unsigned long)lpBuffer->dwAvailPhys)>>20;
		return boost::str(boost::format(format) 
        % name 
        % osv.dwMajorVersion 
        % osv.dwMinorVersion
        % osv.dwBuildNumber
        % platform
		% lpBuffer->dwMemoryLoad
		% Total
		% Avail
        );
		   
}

bool sysinfo::screenIsRotate()
{
	RECT ws;
	SystemParametersInfo(SPI_GETWORKAREA, sizeof(ws), &ws, 0);
	
	return (ws.right>ws.bottom); 
}


bool sysinfo::screenIsVGA() {
    RECT ws;
    SystemParametersInfo(SPI_GETWORKAREA, sizeof(ws), &ws, 0);
    return (ws.right - ws.left > 320); // VGA will have 480 or 640 pixels width
}
