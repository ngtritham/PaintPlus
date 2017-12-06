// PaintPlus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PaintPlus.h"
#include "windowsx.h"
#include <vector>
#include <iostream>
#include "Shape.h"
#include "RibbonFramework.h"
#include "RibbonIDs.h"
#include <Objbase.h>
#include <UIRibbon.h>
#pragma comment(lib, "Ole32.lib")

using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		return FALSE;
	}

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PAINTPLUS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINTPLUS));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CoUninitialize();

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTPLUS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PAINTPLUS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable 

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

vector<CShape*> listShape;
POINT tempStart;
POINT tempEnd;
POINT currEnd;		// temp end point when left button down and still moving
CShape *tempShape = NULL;


COLORREF penColor = RGB(0, 0, 0);
int thickness = 1;
int penType = 0;

#define	LINE 2000
#define ELLIPSE 2001
#define RETANGLE 2002
int type = LINE;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool initSuccess;

	switch (message)
	{
	case WM_CREATE:
	{
		// Initializes the Ribbon framework
		initSuccess = InitializeFramework(hWnd);
		if (!initSuccess) {
			return -1;
		}
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		tempStart.x = GET_X_LPARAM(lParam);
		tempStart.y = GET_Y_LPARAM(lParam);
		SetCapture(hWnd);
		break;
	}

	case WM_MOUSEMOVE:
	{	
		if (wParam & MK_LBUTTON)
		{
			if (!listShape.empty()) {
				listShape.pop_back();
			}
				

			currEnd.x = GET_X_LPARAM(lParam);
			currEnd.y = GET_Y_LPARAM(lParam);
			
			switch (type)
			{
			case LINE:
				tempShape = new CLine(tempStart, currEnd);
				break;
			case ELLIPSE:
				tempShape = new CEllipse(tempStart, currEnd);
				break;
			case RETANGLE:
				tempShape = new CRetangle(tempStart, currEnd);
				break;
			default:
				break;
			}

			if (wParam & MK_SHIFT) {
				tempShape->setPresShift(true);
			}

			listShape.push_back(tempShape);
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		tempEnd.x = GET_X_LPARAM(lParam);
		tempEnd.y = GET_Y_LPARAM(lParam);


		switch (type)
		{
		case LINE:
			tempShape = new CLine(tempStart, tempEnd);
			break;
		case ELLIPSE:
			tempShape = new CEllipse(tempStart, tempEnd);
			break;
		case RETANGLE:
			tempShape = new CRetangle(tempStart, tempEnd);
			break;
		default:
			break;
		}

		if (wParam & MK_SHIFT) {
			tempShape->setPresShift(true);
		}

		listShape.push_back(tempShape);
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseCapture();
	}
	break;

	case WM_COMMAND:
	{
		HMENU hMenu = GetMenu(hWnd);

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_CMD_LINE:
		{
			type = LINE;
		}
		break;

		case ID_CMD_ELLIPSE:
		{
			type = ELLIPSE;
		}
		break;
		case ID_CMD_RECTANGLE:
		{
			type = RETANGLE;
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);

		HDC Memhdc = CreateCompatibleDC(hdc);
		RECT rect;
		GetClientRect(hWnd, &rect);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(Memhdc, hbitmap);
		FillRect(Memhdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		Graphics* graphic = new Graphics(Memhdc);

		for (int i = 0; i < listShape.size(); i++) {
			listShape[i]->Draw(graphic);
		}

		int ribbonHeight = GetRibbonHeight();
		BitBlt(hdc, 0, ribbonHeight, rect.right, rect.bottom, Memhdc, 0, ribbonHeight, SRCCOPY);
		
		delete graphic;
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		
		DestroyFramework();
		GdiplusShutdown(gdiplusToken);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
