#pragma once
#ifndef UI_CONST_H
#define UI_CONST_H

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include "Buttons.h"
#include <richedit.h>
#include "general_search.h"
#include <map>
#include <windowsx.h>

using namespace std;


OPENFILENAMEA OFN;




string filename_str = "";
char filename[265] = {};


struct InitialDimensions {
	int x;
	int y;
	int width;
	int height;
};



// Прототипы функций
void SetOpenFileParams(HWND hWnd); 
BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem);
void ExitSoftware();
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidget(HWND hWnd);
void SetWinStatus(string status);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
string ConvertLPWSTRToString(LPWSTR lpwstr);
void MakeFrame(HWND hWnd, HDC hdc, HWND Edit);
LPWSTR ConvertStringToLPWSTR(const std::string& str);
void FreeLPWSTR(LPWSTR lpwstr);

wstring GetActivePartsOfSpeech(const bitset<8>& ButtonFlags);
bool IsRichEditStrikeout(HWND hRichEdit);
HWND CreateEdit(int x, int y, int width, int height, HWND hWnd, bool readOnly = false);
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd);
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id);
void SetRichEditBold(HWND hRichEdit, bool bold);
void SetRichEditStrikeout(HWND hRichEdit, bool strikeout);
void UpdateButtonStatesAndColors();
void ShowLoadingWindow(HWND hWnd);
void HideLoadingWindow(HWND hWnd);
void OutputTextInfo(const vector<vector<string>>& sentences, const vector<WordData>& rhymes_data, string& compare_word);
void OutputRhymeInfo(const vector<WordData>& rhymes_data, string& compare_word);
HWND CreateRichEdit(LPCWSTR text, int x, int y, int width, int height, HWND hParent, bool readonly = false);

#endif // UI_CONST_H