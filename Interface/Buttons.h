//  Данный заголовочный файл содержит основную функцию - объединение идентификаторов:
//  - Идентификаторы кнопок, виджетов и графических ресурсов для интерфейса приложения.
//  - Флаги состояния для кнопок и битовые флаги для управления состоянием интерфейса.
//  - Структуры для хранения идентификаторов кнопок, виджетов и графических ресурсов.


#pragma once

#include <Windows.h>
#include <bitset>
#include <unordered_map>
#include <vector>
#include <string>


namespace buttons
{
    // Структура для хранения идентификаторов кнопок
    struct ButtonIDs
    {
        int OnInfoClicked = 1; // идентификатор кнопки для показа информации;
		int OnExitSoftware = 2; // идентификатор кнопки для выхода из программы;
		int Search = 5; // идентификатор кнопки для поиска;
		int ButVerb = 8; // идентификатор кнопки для поиска по глаголу;
		int ButAdverb = 9; // идентификатор кнопки для поиска по наречию;
		int ButAdjective = 10; // идентификатор кнопки для поиска по прилагательному;
		int ButNoun = 11; // идентификатор кнопки для поиска по существительному;
		int ButParticiple = 12; // идентификатор кнопки для поиска по причастию;
		int ButAdverbial = 13; // идентификатор кнопки для поиска по деепричастию;
		int ButExit = 14; // идентификатор кнопки для выхода из программы;
		int ButSearchType = 15; // идентификатор кнопки для переключения типа поиска (однородный/неоднородный);
		int CheckBox1 = 16; // идентификатор первого флажка;
		int CheckBox2 = 17; // идентификатор второго флажка;
		int CheckBox3 = 18; // идентификатор третьего флажка;
		int OnHelp = 19; // идентификатор кнопки для вызова справки;
		int ButOpenFile = 20; // идентификатор кнопки для открытия файла;
		int ButSaveFile = 21; // идентификатор кнопки для сохранения файла;
		int ButAll = 22; // идентификатор кнопки для выбора всех частей речи;
    };

    // Структура для хранения виджетов
    struct Widgets
    {
		HWND hAllButton = nullptr; // дескриптор кнопки выбора всех частей речи;
		HWND hEditRhymes = nullptr; // дескриптор поля для вывода рифм;
		HWND hEditText = nullptr; // дескриптор поля для вывода текста;
		HWND hSearch = nullptr; // дескриптор кнопки поиска;
		HWND hOutputStatusText = nullptr; // дескриптор статического поля для вывода статуса;
		HWND hOutputStatus = nullptr; // дескриптор статического поля для вывода статуса;
		HWND hOutputRhymes = nullptr; // дескриптор поля для вывода найденных рифм;
		HWND hOutputText = nullptr; // дескриптор поля для вывода текста с найденными рифмами;
		HWND hToggleButton = nullptr; // дескриптор переключателя для однородного/неоднородного поиска;
		HWND hVerbButton = nullptr; // дескриптор кнопки для поиска по глаголу;
		HWND hAdverbButton = nullptr; // дескриптор кнопки для поиска по наречию;
		HWND hAdjectiveButton = nullptr; // дескриптор кнопки для поиска по прилагательному;
		HWND hNounButton = nullptr; // дескриптор кнопки для поиска по существительному;
		HWND hParticipleButton = nullptr; // дескриптор кнопки для поиска по причастию;
		HWND hAdverbialButton = nullptr; // дескриптор кнопки для поиска по деепричастию;
		HWND hButton = nullptr; // дескриптор кнопки для выхода из программы;
		HWND hWelcomeButton1 = nullptr; // дескриптор кнопки приветствия 1;
		HWND hWelcomeButton2 = nullptr; // дескриптор кнопки приветствия 2;
		HWND hExitButton = nullptr; // дескриптор кнопки для выхода из программы;
		HWND hInputWord = nullptr; // дескриптор поля для ввода слова для поиска рифм;
		HWND hEditInputWord = nullptr; // дескриптор поля для редактирования введенного слова;
		HWND hSearchType = nullptr; // дескриптор кнопки для переключения типа поиска (однородный/неоднородный);
		HWND hStaticCheckBox1Info = nullptr; // дескриптор статического поля для информации о первом флажке;
		HWND hStaticCheckBox2Info = nullptr; // дескриптор статического поля для информации о втором флажке;
		HWND hStaticCheckBox3Info = nullptr; // дескриптор статического поля для информации о третьем флажке;
		HWND hPathSaveFileText = nullptr; // дескриптор поля для отображения пути сохранения файла с текстом;
		HWND hPathSaveFileData = nullptr; // дескриптор поля для отображения пути сохранения файла с данными;
		HWND hPathSaveFileRhymes = nullptr; // дескриптор поля для отображения пути сохранения файла с рифмами;
		HWND hOpenFile = nullptr; // дескриптор кнопки для открытия файла;
		HWND hSaveFile = nullptr; // дескриптор кнопки для сохранения файла;
		HWND hHelpButton = nullptr; // дескриптор кнопки для вызова справки;
		HWND hLoadingWnd = nullptr; // дескриптор окна загрузки программы;
    };

    // Структура для хранения графических ресурсов
    struct Graphics
    {
        HBRUSH hBrush = nullptr; // кисть для фона;
		HBRUSH hEditBackgroundBrush = nullptr; // кисть для фона поля редактирования;
		HBRUSH hBrushToggleButton = nullptr; // кисть для фона переключателя;
		HBRUSH hBrushRed = nullptr; // кисть для красного цвета;
		HBRUSH hBrushGreen = nullptr; // кисть для зеленого цвета;
		HBRUSH hBrushGrey = nullptr; // кисть для серого цвета;
		HBRUSH hBrushNeutral = nullptr; // кисть для нейтрального цвета;
		HDC hdcMem = nullptr; // контекст устройства для памяти;
		HPEN hPenBlack = nullptr; // черная ручка для рисования рамок;

    };

    // Флаги состояния
	bool ShowInEdit = false; // Флаг для отображения в поле редактирования;

    // Экземпляры структур
    ButtonIDs buttonIDs; 
    Widgets widgets;
    Graphics graphics;

    // Флаги для кнопок
	std::bitset<8> ButtonFlags = 0;  // Биты нажатых кнопок
	std::bitset<8> SaveButtonFlags = 0;  // Биты нажатых кнопок для сохранения
    // Биты
    //  0 - поиск по глаголу (1 - выбран, 0 - не выбран)
    //  1 - поиск по наречию (1 - выбран, 0 - не выбран)
    //  2 - поиск по прилагательному (1 - выбран, 0 - не выбран)
    //  3 - поиск по существительному (1 - выбран, 0 - не выбран)
    //  4 - поиск по причастия (1 - выбран, 0 - не выбран)
    //  5 - поиск по деепричастию (1 - выбран, 0 - не выбран)
    //  6 - поиск по слову (1 - введено, 0 - не введено)
    //  7 - режим поиска (1 - однородный, 0 - неоднородный)

    std::unordered_map<std::string, std::vector<std::string>> morphemeRules; 
}