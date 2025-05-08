#pragma once

#include <Windows.h>
#include <bitset>


namespace buttons
{
    // Структура для хранения идентификаторов кнопок
    struct ButtonIDs
    {
        int OnInfoClicked = 1;
        int OnExitSoftware = 2;
        int Search = 5;
        int ClearRhymes = 6;
        int ClearText = 7;
        int ButVerb = 8;
        int ButAdverb = 9;
        int ButAdjective = 10;
        int ButNoun = 11;
        int ButParticiple = 12;
        int ButAdverbial = 13;
        int ButExit = 14;
        int ButSearchType = 15;
		int CheckBox1 = 16;
		int CheckBox2 = 17;
		int CheckBox3 = 18;
        int OnHelp = 19;
        int ButOpenFile = 20;
        int ButSaveFile = 21;
    };

    // Структура для хранения виджетов
    struct Widgets
    {
        HWND hEditRhymes = nullptr;
        HWND hEditText = nullptr;
        HWND hSearch = nullptr;
        HWND hOutputStatusText = nullptr;
        HWND hOutputStatus = nullptr;
        HWND hOutputRhymes = nullptr;
        HWND hOutputText = nullptr;
        HWND hToggleButton = nullptr;
        HWND hVerbButton = nullptr;
        HWND hAdverbButton = nullptr;
        HWND hAdjectiveButton = nullptr;
        HWND hNounButton = nullptr;
        HWND hParticipleButton = nullptr;
        HWND hAdverbialButton = nullptr;
        HWND hButton = nullptr;
        HWND hWelcomeButton1 = nullptr;
        HWND hWelcomeButton2 = nullptr;
        HWND hExitButton = nullptr;
        HWND hInputWord = nullptr;
        HWND hEditInputWord = nullptr;
        HWND hSearchType = nullptr;
		HWND hStaticCheckBox1Info = nullptr;
		HWND hStaticCheckBox2Info = nullptr;
		HWND hStaticCheckBox3Info = nullptr;
        HWND hPathSaveFileText = nullptr;
        HWND hPathSaveFileData = nullptr;
        HWND hPathSaveFileRhymes = nullptr;
        HWND hOpenFile = nullptr;
        HWND hSaveFile = nullptr;
        HWND hHelpButton = nullptr;
        HWND hLoadingWnd = NULL;
    };

    // Структура для хранения графических ресурсов
    struct Graphics
    {
        HBRUSH hBrush = nullptr;
        HBRUSH hEditBackgroundBrush = nullptr;
        HBRUSH hBrushToggleButton = nullptr;
        HBRUSH hBrushRed = nullptr;
        HBRUSH hBrushGreen = nullptr;
        HBRUSH hBrushGrey = nullptr;
        HBRUSH hBrushNeutral = nullptr;
        HDC hdcMem = nullptr;
        HPEN hPenBlack = nullptr;

    };

    // Флаги состояния
    bool ShowInEdit = false;

    // Экземпляры структур
    ButtonIDs buttonIDs;
    Widgets widgets;
    Graphics graphics;

    // Флаги для кнопок
    std::bitset<8> ButtonFlags = 0; 
}