#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>



// Размер буфера
#define TextBufferSize      90

using namespace std;


char BufferReceive[TextBufferSize] = { 0 };


char filename[265] = {};

bitset<8> ButtonFlags;

struct Buttons
{
	// Кнопки
	int OnInfoClicked;
	int OnExitSoftware;
	int OnSaveFile;
	int OnReadFile;
	int Search;
	int ClearRhymes;
	int ClearText;
	int ButVerb;
	int ButAdverb;
	int ButAdjective;
	int ButNoun;
	int ButParticiple;
	int ButAdverbial;
	int ButExit;
	int OnToggleButtonClicked;
	
	// Виджеты
	HWND hClearRhymes;
	HWND hClearText;
	HWND hEditRhymes;
	HWND hEditText;
	HWND hSearch;
	HWND hOutputStatusText;
	HWND hOutputStatus;
	HWND hOutputRhymes;
	HWND hOutputText;
	HWND hToggleButton;
	HWND hVerbButton;
	HWND hAdverbButton;
	HWND hAdjectiveButton;
	HWND hNounButton;
	HWND hParticipleButton;
	HWND hAdverbialButton;
	HWND hButton;
	HWND hWelcomeButton1;
	HWND hWelcomeButton2;
	HWND hExitButton;
	HWND hInputWord;
	HWND hEditInputWord;
	HBRUSH hBrush;
	HBRUSH hEditBackgroundBrush;
	HBRUSH hBrushToggleButton;
	HBRUSH hBrushRed;
	HBRUSH hBrushGreen;
	HBRUSH hBrushBlue;
	HDC hdcMem;
	HPEN hPenBlack;
	HBITMAP hBitmap;
	HBITMAP hBitmap2;
	// Флаги нажатия кнопок
	bool IsToggled;
	bool IsVerbToggled;
	bool IsAdverbToggled;
	bool IsAdjectiveToggled;
	bool IsNounToggled;
	bool IsParticipleToggled;
	bool IsAdverbialToggled;
};



OPENFILENAMEA OFN;





// Прототипы функций
void SetOpenFileParams(HWND hWnd);
void read_data(LPCSTR path);
void save_data(LPCSTR path, Buttons& buttons);
BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem, Buttons& buttons);
void ExitSoftware();
void MainWndAddMenus(HWND hWnd, const Buttons& buttons);
void MainWndAddWidget(HWND hWnd, Buttons& buttons);
void SetWinStatus(string status, const Buttons& buttons);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void SetEditBackgroundColor(HWND hEdit, COLORREF color, Buttons& buttons);
WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MakeFrame(HWND hWnd, HDC hdc, HWND Edit, Buttons buttons);