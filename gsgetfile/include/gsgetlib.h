#ifndef __GSGETLIB_H__
#define __GSGETLIB_H__

#include <commdlg.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma comment(lib,"gsgetlib.lib")

BOOL GetOpenFileNameEx(OPENFILENAME* pofn);
BOOL GetSaveFileNameEx(OPENFILENAME* pofn);

#ifdef __cplusplus
};
#endif

#endif