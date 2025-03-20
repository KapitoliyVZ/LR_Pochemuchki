#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <cmath>

// Для Меню
#define OnInfoClicked			1001
#define OnExitSoftware			1002
#define OnSaveFile				1003
#define OnReadFile				1004

// Для Виджетов
#define EnterWord				1
#define ClearRhymes				2
#define ClearText				3


#define ButVerb					517
#define ButAdverb				518
#define ButAdjective			519
#define ButNoun					520
#define ButParticiple			521
#define ButAdverbial			522
#define ButBack					523

#define	OnToggleButtonClicked	524


// Размер буфера
#define TextBufferSize			90

using namespace std;


char BufferReceive[TextBufferSize] = {0};


char filename[265] = {};

bitset<8> ButtonFlags;

HWND hClearRhymes;
HWND hClearText;
HWND hEditRhymes;
HWND hEditText;
HWND hEditWord;
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
HWND hBackButton;

HWND hWelcomeWnd = NULL;
HWND hMainWnd = NULL;

HINSTANCE hInstGlobal;

//HDC hdc;
HDC hdc1;

OPENFILENAMEA OFN;

HBRUSH hBrush				= NULL;
HBRUSH hEditBackgroundBrush = NULL;
HBRUSH hBrushToggleButton	= NULL;
HBRUSH hBrushRed			= NULL;
HBRUSH hBrushGreen			= NULL;
HBRUSH hBrushBlue			= NULL;

bool IsConnected			= false;
bool IsToggled				= false;
bool IsVerbToggled			= false;
bool IsAdverbToggled		= false;
bool IsAdjectiveToggled		= false;
bool IsNounToggled			= false;
bool IsParticipleToggled	= false;
bool IsAdverbialToggled		= false;
bool isHovered				= false;

bool isHoveredBackButton		= false;
bool isHoveredToggleButton		= false;
bool isHoveredVerbButton		= false;
bool isHoveredAdjectiveButton	= false;
bool isHoveredNounButton		= false;
bool isHoveredAdverbialButton	= false;
bool isHoveredParticipleButton	= false;
bool isHoveredAdverbButton		= false;


WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow);
LRESULT CALLBACK SoftwareMainProcedure(HWND Hwnd, UINT msg, WPARAM wp, LPARAM lp);
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidget(HWND hWnd);
void save_data(LPCSTR path);
void read_data(LPCSTR path);
void SetOpenFileParams(HWND hWnd);
void SetWinStatus(string status);
void ExitSoftware();
void SetEditBackgroundColor(HWND hEdit, COLORREF color);
BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem);

LRESULT CALLBACK WelcomeProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MainProcedure(HWND, UINT, WPARAM, LPARAM);
void CreateMainWindow(HINSTANCE);
