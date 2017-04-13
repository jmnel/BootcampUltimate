#include <cassert>
#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>

static TCHAR szWindowClass[] = _T( "wind32app" );
static TCHAR szTitle[] = _T( "Bootcamp Ultimate" );
static HINSTANCE hInst = 0;
static HWND hWnd = 0;

LRESULT CALLBACK wndProc( HWND hWnd,
	UINT msg, WPARAM wParam, LPARAM lParam );

HHOOK hHook = 0;

LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam ) {
	if( nCode == HC_ACTION ) {
		KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT*)lParam;
		if( p->vkCode == VK_LWIN ) {
			if( wParam == WM_KEYDOWN ) {
				keybd_event( VK_LCONTROL, 0xA2, KEYEVENTF_EXTENDEDKEY | 0, 0 );
			}
			else if( wParam == WM_KEYUP ) {
				keybd_event( VK_LCONTROL, 0xA2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
			}
			return WM_CHAR;
		}
		if( p->vkCode == VK_LCONTROL ) {
			if( wParam == WM_KEYDOWN ) {
				keybd_event( VK_LWIN, 0x5B, KEYEVENTF_EXTENDEDKEY | 0, 0 );
			}
			else if( wParam == WM_KEYUP ) {
				keybd_event( VK_LWIN, 0x5B, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
			}
			return WM_CHAR;
		}
	}
	return CallNextHookEx( hHook, nCode, wParam, lParam );
}

// -- WinMain function : Application entry point --
int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nShowCmd ) {

	hInst = hInstance;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)GetStockObject( DEFAULT_PALETTE );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );
	
	auto success = RegisterClassEx( &wcex );
	assert( success );

	hWnd = CreateWindow( szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL );
	assert( hWnd );

	ShowWindow( hWnd, nShowCmd );
	UpdateWindow( hWnd );

	hHook = SetWindowsHookEx( WH_KEYBOARD_LL, &LowLevelKeyboardProc, hInstance, NULL );
	assert( hHook );

	MSG msg;

	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	/*while( true ) {
		while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if( msg.message == WM_QUIT ) {
			break;;
		}
	}*/

	return (int)msg.wParam;
}

LRESULT CALLBACK wndProc( HWND hWnd, UINT msg, 
	WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
		break;
	}
}