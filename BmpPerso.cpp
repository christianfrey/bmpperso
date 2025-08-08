#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "resource.h"

HBITMAP hBmp = 0;
HINSTANCE hInst = 0;
SIZE SizeBmp = {128, 128};
char szAppName[] = "BmpPerso";

LRESULT CALLBACK AppWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
		case WM_CREATE:
			hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP));
			SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
			return 0;
		case WM_ERASEBKGND: {
			HDC hDC, hMemDC;
			HBITMAP hOldBmp;
			hDC = (HDC)wParam;
			if(hBmp) {
				hMemDC = CreateCompatibleDC(hDC);
				hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
				BitBlt(hDC, 0, 0, SizeBmp.cx, SizeBmp.cy, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hMemDC, hOldBmp);
				DeleteDC(hMemDC); 
			}
	      }
			return 1;
		case WM_NCHITTEST:
			return HTCAPTION;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		case WM_DESTROY:
			DeleteObject(hBmp);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int InitInstance()
{
	WNDCLASSEX wclsx;
	wclsx.cbSize        = sizeof(WNDCLASSEX);
	wclsx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wclsx.lpfnWndProc   = AppWndProc;
	wclsx.cbClsExtra    = 0;
	wclsx.cbWndExtra    = 0;
	wclsx.hInstance     = hInst;
	wclsx.hIcon         = 0;
	wclsx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wclsx.hbrBackground = 0;
	wclsx.lpszMenuName  = 0;
	wclsx.lpszClassName = szAppName;
	wclsx.hIconSm       = 0; 
	return RegisterClassEx(&wclsx);
}

DWORD VerifWin2KMini()
{
	OSVERSIONINFO osvi;
	osvi.dwPlatformId = osvi.dwMajorVersion = 0;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if(osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) return 0;
	return (osvi.dwMajorVersion >= 5);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int)
{
	MSG msg;
	if(!VerifWin2KMini()) return 0;
	hInst = hInstance;
	if(!InitInstance()) return 0;
	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szAppName, szAppName, 
										WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS,
										CW_USEDEFAULT, CW_USEDEFAULT, SizeBmp.cx, SizeBmp.cy,
										0, 0, hInst, 0);
	ShowWindow(hWnd, SW_SHOW); UpdateWindow(hWnd);
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}