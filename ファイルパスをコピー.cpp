#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <winnetwk.h>
#include <shlobj.h>
#include <shellapi.h>

BOOL GetPathName(LPCTSTR path, LPTSTR *lpszLongPath)
{
	TCHAR Buffer[MAX_PATH];
	LPTSTR lpFilePart;
	DWORD len;

	if (SUCCEEDED(GetFullPathName(path, MAX_PATH, Buffer, &lpFilePart))) {
		len = GetLongPathName(Buffer, *lpszLongPath, MAX_PATH);
		if (SUCCEEDED(len)) {
			*lpszLongPath += len;
			return TRUE;
		}
	}

	return FALSE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	if( __argc >= 2 ) {
		HGLOBAL hClipText;
		LPTSTR pClipText;
		hClipText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, 
			sizeof(TCHAR) * MAX_PATH * 256);
		pClipText = (LPTSTR)GlobalLock(hClipText);

		LPTSTR lpszLongPath = pClipText;
		LPTSTR lpszLongPathe = pClipText + MAX_PATH * 256;

		for( int i = 1 ; i < __argc; i++ )
		{
			LPCTSTR lpszFileIn = __targv[i];
			if (SUCCEEDED(GetPathName(lpszFileIn, &lpszLongPath))){
				if(__argc == 2) break;

				*lpszLongPath = '\n';
				lpszLongPath++;
			}
		}

		GlobalUnlock(hClipText);

		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hClipText);
		CloseClipboard();
	}
	return FALSE;
}
