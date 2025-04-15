#pragma once
#ifndef UI_CONST_H
#define UI_CONST_H

#include <iostream>
#include <string>
#include <fstream>
#include "Buttons.h"



// ������ ������
//#define TextBufferSize      90

using namespace std;

OPENFILENAMEW OFN;
//char BufferReceive[TextBufferSize] = { 0 };

ifstream file_input; // ���� ��� ������


struct InitialDimensions {
	int x;
	int y;
	int width;
	int height;
};

//InitialDimensions initialDimensions[20];





// OPENFILENAMEW OFN;





// ��������� �������
void SetOpenFileParams(HWND hWnd, string filename); 
void read_data(LPCSTR path);
void save_data(LPCSTR path);
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

HWND CreateEdit(int x, int y, int width, int height, HWND hWnd, bool readOnly = false);
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd);
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id);

#endif // UI_CONST_H