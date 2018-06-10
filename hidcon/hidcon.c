#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <limits.h>
#include <string.h>

//---------------------------------------------------------------------------
  HWND hWnd;
  HINSTANCE hInst;
  LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//---------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)


{
	// Disable FS redirection
	// @see http://msdn.microsoft.com/en-us/library/aa365743(VS.85).aspx
#ifdef DISABLE_WOW64_REDIRECTION
	PVOID OldValue;
	BOOL bRet = Wow64DisableWow64FsRedirection (&OldValue);
#endif
	STARTUPINFO	si; 
	PROCESS_INFORMATION	pi;

	hInst = hInstance;
	
	// Create process
	// http://msdn.microsoft.com/en-us/library/ms682512(VS.85).aspx
		
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );
	
	if(!CreateProcess( NULL, lpCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) ) {
        return 1;
    }


	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );
	
	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
#ifdef DISABLE_WOW64_REDIRECTION	
	// Restore the previous WOW64 file system redirection value.
	Wow64RevertWow64FsRedirection (OldValue);
#endif
	return 0;
}

