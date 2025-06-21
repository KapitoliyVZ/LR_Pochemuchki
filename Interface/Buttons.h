//  ������ ������������ ���� �������� �������� ������� - ����������� ���������������:
//  - �������������� ������, �������� � ����������� �������� ��� ���������� ����������.
//  - ����� ��������� ��� ������ � ������� ����� ��� ���������� ���������� ����������.
//  - ��������� ��� �������� ��������������� ������, �������� � ����������� ��������.


#pragma once

#include <Windows.h>
#include <bitset>
#include <unordered_map>
#include <vector>
#include <string>


namespace buttons
{
    // ��������� ��� �������� ��������������� ������
    struct ButtonIDs
    {
        int OnInfoClicked = 1; // ������������� ������ ��� ������ ����������;
		int OnExitSoftware = 2; // ������������� ������ ��� ������ �� ���������;
		int Search = 5; // ������������� ������ ��� ������;
		int ButVerb = 8; // ������������� ������ ��� ������ �� �������;
		int ButAdverb = 9; // ������������� ������ ��� ������ �� �������;
		int ButAdjective = 10; // ������������� ������ ��� ������ �� ���������������;
		int ButNoun = 11; // ������������� ������ ��� ������ �� ����������������;
		int ButParticiple = 12; // ������������� ������ ��� ������ �� ���������;
		int ButAdverbial = 13; // ������������� ������ ��� ������ �� ������������;
		int ButExit = 14; // ������������� ������ ��� ������ �� ���������;
		int ButSearchType = 15; // ������������� ������ ��� ������������ ���� ������ (����������/������������);
		int CheckBox1 = 16; // ������������� ������� ������;
		int CheckBox2 = 17; // ������������� ������� ������;
		int CheckBox3 = 18; // ������������� �������� ������;
		int OnHelp = 19; // ������������� ������ ��� ������ �������;
		int ButOpenFile = 20; // ������������� ������ ��� �������� �����;
		int ButSaveFile = 21; // ������������� ������ ��� ���������� �����;
		int ButAll = 22; // ������������� ������ ��� ������ ���� ������ ����;
    };

    // ��������� ��� �������� ��������
    struct Widgets
    {
		HWND hAllButton = nullptr; // ���������� ������ ������ ���� ������ ����;
		HWND hEditRhymes = nullptr; // ���������� ���� ��� ������ ����;
		HWND hEditText = nullptr; // ���������� ���� ��� ������ ������;
		HWND hSearch = nullptr; // ���������� ������ ������;
		HWND hOutputStatusText = nullptr; // ���������� ������������ ���� ��� ������ �������;
		HWND hOutputStatus = nullptr; // ���������� ������������ ���� ��� ������ �������;
		HWND hOutputRhymes = nullptr; // ���������� ���� ��� ������ ��������� ����;
		HWND hOutputText = nullptr; // ���������� ���� ��� ������ ������ � ���������� �������;
		HWND hToggleButton = nullptr; // ���������� ������������� ��� �����������/������������� ������;
		HWND hVerbButton = nullptr; // ���������� ������ ��� ������ �� �������;
		HWND hAdverbButton = nullptr; // ���������� ������ ��� ������ �� �������;
		HWND hAdjectiveButton = nullptr; // ���������� ������ ��� ������ �� ���������������;
		HWND hNounButton = nullptr; // ���������� ������ ��� ������ �� ����������������;
		HWND hParticipleButton = nullptr; // ���������� ������ ��� ������ �� ���������;
		HWND hAdverbialButton = nullptr; // ���������� ������ ��� ������ �� ������������;
		HWND hButton = nullptr; // ���������� ������ ��� ������ �� ���������;
		HWND hWelcomeButton1 = nullptr; // ���������� ������ ����������� 1;
		HWND hWelcomeButton2 = nullptr; // ���������� ������ ����������� 2;
		HWND hExitButton = nullptr; // ���������� ������ ��� ������ �� ���������;
		HWND hInputWord = nullptr; // ���������� ���� ��� ����� ����� ��� ������ ����;
		HWND hEditInputWord = nullptr; // ���������� ���� ��� �������������� ���������� �����;
		HWND hSearchType = nullptr; // ���������� ������ ��� ������������ ���� ������ (����������/������������);
		HWND hStaticCheckBox1Info = nullptr; // ���������� ������������ ���� ��� ���������� � ������ ������;
		HWND hStaticCheckBox2Info = nullptr; // ���������� ������������ ���� ��� ���������� � ������ ������;
		HWND hStaticCheckBox3Info = nullptr; // ���������� ������������ ���� ��� ���������� � ������� ������;
		HWND hPathSaveFileText = nullptr; // ���������� ���� ��� ����������� ���� ���������� ����� � �������;
		HWND hPathSaveFileData = nullptr; // ���������� ���� ��� ����������� ���� ���������� ����� � �������;
		HWND hPathSaveFileRhymes = nullptr; // ���������� ���� ��� ����������� ���� ���������� ����� � �������;
		HWND hOpenFile = nullptr; // ���������� ������ ��� �������� �����;
		HWND hSaveFile = nullptr; // ���������� ������ ��� ���������� �����;
		HWND hHelpButton = nullptr; // ���������� ������ ��� ������ �������;
		HWND hLoadingWnd = nullptr; // ���������� ���� �������� ���������;
    };

    // ��������� ��� �������� ����������� ��������
    struct Graphics
    {
        HBRUSH hBrush = nullptr; // ����� ��� ����;
		HBRUSH hEditBackgroundBrush = nullptr; // ����� ��� ���� ���� ��������������;
		HBRUSH hBrushToggleButton = nullptr; // ����� ��� ���� �������������;
		HBRUSH hBrushRed = nullptr; // ����� ��� �������� �����;
		HBRUSH hBrushGreen = nullptr; // ����� ��� �������� �����;
		HBRUSH hBrushGrey = nullptr; // ����� ��� ������ �����;
		HBRUSH hBrushNeutral = nullptr; // ����� ��� ������������ �����;
		HDC hdcMem = nullptr; // �������� ���������� ��� ������;
		HPEN hPenBlack = nullptr; // ������ ����� ��� ��������� �����;

    };

    // ����� ���������
	bool ShowInEdit = false; // ���� ��� ����������� � ���� ��������������;

    // ���������� ��������
    ButtonIDs buttonIDs; 
    Widgets widgets;
    Graphics graphics;

    // ����� ��� ������
	std::bitset<8> ButtonFlags = 0;  // ���� ������� ������
	std::bitset<8> SaveButtonFlags = 0;  // ���� ������� ������ ��� ����������
    // ����
    //  0 - ����� �� ������� (1 - ������, 0 - �� ������)
    //  1 - ����� �� ������� (1 - ������, 0 - �� ������)
    //  2 - ����� �� ��������������� (1 - ������, 0 - �� ������)
    //  3 - ����� �� ���������������� (1 - ������, 0 - �� ������)
    //  4 - ����� �� ��������� (1 - ������, 0 - �� ������)
    //  5 - ����� �� ������������ (1 - ������, 0 - �� ������)
    //  6 - ����� �� ����� (1 - �������, 0 - �� �������)
    //  7 - ����� ������ (1 - ����������, 0 - ������������)

    std::unordered_map<std::string, std::vector<std::string>> morphemeRules; 
}