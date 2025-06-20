﻿//  Данный файл содержит реализацию функций для работы с пользовательским интерфейсом приложения а именно:
// - Создание главного окна приложения
// - Обработку сообщений окон и кнопок
// - Функции для работы с текстом и рифмами
// - Функции вывода текста и рифм в поля RichEdit



#include "UI_const.h"
#include "Heart_of_program.h"
#include "file_working.h"
#include "NLP.h"


// Старт приложения и создание стартовых процедур
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	// Загрузка Msftedit.dll для работы с RichEdit
    if (!LoadLibraryA("Msftedit.dll")) 
    {
        MessageBoxA(NULL, "Не удалось загрузить Msftedit.dll", "Ошибка", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Главный класс для создания параметров приветственного окна
    WNDCLASS MainWindClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
                                          LoadIcon(NULL, IDI_QUESTION), L"MainClass", SoftwareMainProcedure);

    // Регистрация класса
    if (!RegisterClassW(&MainWindClass))
        return 0;

    // Получение разрешения экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // Само создание с заложенными параметрами
    HWND hMainWnd = CreateWindow(L"MainClass", L"Приложение для поиска рифм", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);
    if (!hMainWnd) {
        MessageBox(NULL, L"Не удалось создать главное окно", L"Ошибка", MB_OK | MB_ICONERROR);
        return 0;
    }
    // Устанавливаем окно поверх всех окон
    SetWindowPos(hMainWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
    
    // Основной цикл работы проложения
    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);

        // Проверяем, что окно действительно существует, если hwnd не NULL
        if (msg.hwnd != NULL && !IsWindow(msg.hwnd)) {
            MessageBox(NULL, L"Недействительное окно в сообщении", L"Ошибка", MB_OK | MB_ICONERROR);
            break; // Прерываем цикл, если окно недействительно
        }

        DispatchMessage(&msg);
    }

    return 0;
}

// Проверка корректности кодировки
bool isFalseCoding(const string& str) 
{ 
    // Характерные символы неверной кодировки 
    static const std::vector<std::string> known_bad_prefixes = 
    {
        "Ѓ", "Ќ", "Рђ", "Р‘", "СЃ", "С‡"
    };

    // Если встречаются подряд нехарактерные символы - неверная кодировка
    for (const auto& prefix : known_bad_prefixes) 
    {
        size_t pos = str.find(prefix);
        if (pos != std::string::npos && pos + 1 < str.size()) 
        {
            unsigned char next_ch = static_cast<unsigned char>(str[pos + 1]);
            if (next_ch >= 0x80 && next_ch <= 0xBF) 
            {
                return true;
            }
        }
    }

    return false;
}

// Обновляем состояние кнопок в зависимости от флагов
void UpdateButtonStatesAndColors()
{
    // Обновляем состояние кнопок в зависимости от флагов
    bool isHomogeneousMode = buttons::ButtonFlags[7]; // Флаг однородного режима
    bool hasInput = GetWindowTextLengthW(buttons::widgets.hEditInputWord) > 0;

    // Обновляем доступность кнопок
	EnableWindow(buttons::widgets.hAllButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hVerbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdjectiveButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hNounButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hParticipleButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbialButton, !isHomogeneousMode || !hasInput);

    // Перерисовываем кнопки для обновления их цветов
	InvalidateRect(buttons::widgets.hAllButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hSearchType, NULL, TRUE);

    // Обновляем окно для применения изменений
	UpdateWindow(buttons::widgets.hAllButton);
    UpdateWindow(buttons::widgets.hVerbButton);
    UpdateWindow(buttons::widgets.hAdverbButton);
    UpdateWindow(buttons::widgets.hAdjectiveButton);
    UpdateWindow(buttons::widgets.hNounButton);
    UpdateWindow(buttons::widgets.hParticipleButton);
    UpdateWindow(buttons::widgets.hAdverbialButton);
    UpdateWindow(buttons::widgets.hSearchType);
}

// Создаем новое окно приложение
WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
    WNDCLASS NWC = {0};

    NWC.hIcon = Icon;
    NWC.hCursor = Cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = Name;
    NWC.hbrBackground = BGColor;
    NWC.lpfnWndProc = Procedure;

    return NWC;
}

// Проверка что слово зачеркнуто
bool IsRichEditStrikeout(HWND hRichEdit)
{
	CHARFORMAT2 cf = { 0 }; // Инициализация структуры CHARFORMAT2
    cf.cbSize = sizeof(CHARFORMAT2);
	if (SendMessage(hRichEdit, EM_GETCHARFORMAT, SCF_ALL, (LPARAM)&cf)) // Получаем формат символов
    {
        return (cf.dwMask & CFM_STRIKEOUT) && (cf.dwEffects & CFE_STRIKEOUT);
    }
    return false;
}

// Установка жирного текста
void SetRichEditBold(HWND hRichEdit, bool bold)
{
    SendMessage(hRichEdit, EM_SETSEL, 0, -1);  // Выделить весь текст

    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_BOLD;          // Указываем, что меняем жирность
    cf.dwEffects = bold ? CFE_BOLD : 0; // Включаем/выключаем жирный шрифт

    SendMessage(hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

// Установка зачеркивания
void SetRichEditStrikeout(HWND hRichEdit, bool strikeout)
{
    SendMessage(hRichEdit, EM_SETSEL, 0, -1);  // Выделить весь текст

    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_STRIKEOUT;          // Указываем, что меняем зачеркивание
    cf.dwEffects = strikeout ? CFE_STRIKEOUT : 0; // Включаем/выключаем зачеркивание

    SendMessage(hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

// Обновление состояния чекбоксов
void UpdateCheckboxStates()
{
    // Проверка условий
    bool isHomogeneousMode = buttons::ButtonFlags[7];
    bool partOfSpeechSelected;
    int selectedPartsOfSpeech = buttons::ButtonFlags.test(0) + buttons::ButtonFlags.test(1) +
        buttons::ButtonFlags.test(2) + buttons::ButtonFlags.test(3) +
        buttons::ButtonFlags.test(4) + buttons::ButtonFlags.test(5);

    // Получаем текст из поля редактирования
    char buffer[256] = { 0 };
    GetWindowTextA(buttons::widgets.hEditInputWord, buffer, sizeof(buffer));

	// Проверяем, режим поиска и введено ли слово
    if (isHomogeneousMode && strlen(buffer) > 0)
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"Однородный режим поиска");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true);
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"Введите слово");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, true);
        partOfSpeechSelected = (selectedPartsOfSpeech == 0);
    }
	// Если однородный режим и слово не введено
    else if(isHomogeneousMode && strlen(buffer) == 0)
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"Однородный режим поиска");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true);
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"Выберите НЕ МЕНЕЕ ОДНОЙ ч.р.");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, false);
        partOfSpeechSelected = (selectedPartsOfSpeech >= 1);
    }
	// Если неоднородный режим и слово введено
    else if(!isHomogeneousMode && strlen(buffer) > 0)
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"Неоднородный режим поиска");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true);
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"Выберите НЕ МЕНЕЕ ОДНОЙ ч.р.");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, false);
        partOfSpeechSelected = (selectedPartsOfSpeech > 0);
    }
	// Если неоднородный режим и слово не введено
    else if(!isHomogeneousMode && strlen(buffer) == 0)
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"Неоднородный режим поиска");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true);
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"Выберите НЕ МЕНЕЕ ДВУХ ч.р.");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, false);
        partOfSpeechSelected = (selectedPartsOfSpeech > 1);
    }

	// Обновляем текстовое поле с информацией о выбранных частях речи
    bool isFileSelected = !filename_str.empty();
    SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, isFileSelected);
    SetRichEditStrikeout(buttons::widgets.hStaticCheckBox1Info, partOfSpeechSelected);

    bool enableSearch = partOfSpeechSelected && isFileSelected;

	// Обновляем состояние кнопки поиска
    EnableWindow(buttons::widgets.hSearch, enableSearch);

    // Обновим внешний вид
    InvalidateRect(buttons::widgets.hSearch, NULL, TRUE);  

    // Обновляем окна
    UpdateWindow(buttons::widgets.hStaticCheckBox1Info);
    UpdateWindow(buttons::widgets.hStaticCheckBox2Info);
    UpdateWindow(buttons::widgets.hStaticCheckBox3Info);
    UpdateWindow(buttons::widgets.hSearch);
}

// Конвертация строки ANSI (Windows-1251) в wstring (UTF-16)
wstring ansi_to_wstring(const std::string& ansi_str) 
{
    if (ansi_str.empty()) return std::wstring();

    // Получаем необходимый размер для wide строки
    int size_needed = MultiByteToWideChar(1251 /*CP_ACP*/, 0, ansi_str.c_str(), -1, nullptr, 0);
    if (size_needed == 0) {
        // Ошибка конвертации, можно кинуть исключение или вернуть пустую строку
        return std::wstring();
    }

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(1251 /*CP_ACP*/, 0, ansi_str.c_str(), -1, &wstr[0], size_needed);

    // MultiByteToWideChar возвращает строку с завершающим нулём,
    // поэтому можно удалить последний символ
    if (!wstr.empty() && wstr.back() == L'\0') {
        wstr.pop_back();
    }

    return wstr;
}

// Конвертация wstring (UTF-16) в ANSI-строку (Windows-1251)
string wstring_to_ansi(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();

    // Получаем необходимый размер для ansi-строки
    int size_needed = WideCharToMultiByte(1251 /*CP_ACP*/, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size_needed == 0) {
        // Ошибка конвертации, можно кинуть исключение или вернуть пустую строку
        return std::string();
    }

    std::string ansi_str(size_needed, 0);
    WideCharToMultiByte(1251 /*CP_ACP*/, 0, wstr.c_str(), -1, &ansi_str[0], size_needed, nullptr, nullptr);

    // WideCharToMultiByte возвращает строку с завершающим нулём,
    // поэтому можно удалить последний символ
    if (!ansi_str.empty() && ansi_str.back() == '\0') {
        ansi_str.pop_back();
    }

    return ansi_str;
}

// Структура для хранения данных о словах и их рифмах
struct WordInfo {
	std::string part_of_speech; // Часть речи слова
	std::vector<std::string> rhymed_words; // Список рифмующихся слов
};

// Вывод текста в поле
void OutputTextInfo(const vector<vector<string>>& sentences, const vector<WordData>& rhymes_data, string& compare_word)
{
    bool capitalize = false;
    bool firstWord = true;
    COLORREF save_color;
    unordered_map<std::string, WordInfo> wordToInfo;
    unordered_map<std::string, std::string> rhymeWordToPartOfSpeech;
    
	// Заполняем словарь wordToInfo и rhymeWordToPartOfSpeech
    for (const auto& wordData : rhymes_data)
    {
        wordToInfo[wordData.word] = { wordData.part_of_speech, wordData.rhymed_words };
        for (const auto& rhyme : wordData.rhymed_words)
        {
            rhymeWordToPartOfSpeech[rhyme] = wordData.part_of_speech;
        }
    }
	// Очистка поля RichEdit перед выводом
    SetWindowTextW(buttons::widgets.hEditText, L"");

	// Выводим каждое предложение
    for (const auto& sentence : sentences)
    {
        bool firstWord = true;
		// Выводим каждое слово в предложении
        for (const auto& word : sentence)
        {
            capitalize = false;
			// Если не первое слово, выбираем цвет
            if (!firstWord)
            {
                CHARFORMAT2 cf_def = { sizeof(cf_def) };
                cf_def.dwMask = CFM_COLOR;
                cf_def.crTextColor = RGB(0, 0, 0);
                SendMessageW(buttons::widgets.hEditText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf_def);
                SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L" ");
            }

            // Определяем цвет слова
            COLORREF color = RGB(0, 0, 0);
            std::string part;
            bool isMainWord = false;
            auto it = wordToInfo.find(word);

			// Если слово найдено в словаре wordToInfo
            if (it != wordToInfo.end()) {
                part = it->second.part_of_speech;
                isMainWord = true;
            }
			// Если слово не найдено в словаре wordToInfo, проверяем на рифмы
            else {
                auto rit = rhymeWordToPartOfSpeech.find(word);
                if (rit != rhymeWordToPartOfSpeech.end())
                {
                    part = rit->second;
                    capitalize = true; // Если слово рифмуется, то делаем его заглавным
                }
            }
			// Если слово не найдено в словаре и не рифмуется, то пропускаем его
            if (!part.empty())
            { 
				// Если слово рифмуется, то берем его часть речи из словаря
                if (compare_word == "")
                {
                    // Всегда цвет по части речи
                    if (part == "глагол") color = RGB(200, 0, 0);
                    else if (part == "существительное") color = RGB(0, 0, 200);
                    else if (part == "прилагательное") color = RGB(0, 150, 0);
                    else if (part == "наречие") color = RGB(150, 0, 150);
                    else if (part == "причастие") color = RGB(0, 128, 128);
                    else if (part == "деепричастие") color = RGB(184, 134, 11);
                }
				// Если введено слово для поиска
                else
                { 
					// Если слово найдено в словаре wordToInfo, то берем его часть речи 
                    if (isMainWord)
                    {
                        // Для слов из wordToInfo — всегда цвет по части речи
                        if (part == "глагол") color = RGB(200, 0, 0);
                        else if (part == "существительное") color = RGB(0, 0, 200);
                        else if (part == "прилагательное") color = RGB(0, 150, 0);
                        else if (part == "наречие") color = RGB(150, 0, 150);
                        else if (part == "причастие") color = RGB(0, 128, 128);
                        else if (part == "деепричастие") color = RGB(184, 134, 11);
                    }
					// Если слово не найдено в словаре wordToInfo, то проверяем на рифмы
                    else
                    {
                        // Для рифмующихся слов — розовый
                        save_color = color;
                        color = RGB(255, 0, 255);
                    }
                }
            }

            // Устанавливаем цвет для слова
            CHARFORMAT2 cf = { sizeof(cf) };
            cf.dwMask = CFM_COLOR;
            cf.crTextColor = color;
            SendMessageW(buttons::widgets.hEditText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

            string capitalized_word;
            wstring wword;
			// Если слово рифмуется или нужно сделать заглавным, то конвертируем его
            if (capitalize == true)
            {
                capitalized_word = capitalizeAllLetters(word);
                wword = ansi_to_wstring(capitalized_word);
            }
			// Если слово не рифмуется и не нужно делать заглавным, то конвертируем его как есть
            else
            {
                wword = ansi_to_wstring(word);
            }
			// Если введено слово и оно найдено в тексте, то выделяем его в цвет части речи
            if (word == compare_word)
            {
                cf = { sizeof(cf) };
                cf.dwMask = CFM_COLOR;
                cf.crTextColor = save_color;
                SendMessageW(buttons::widgets.hEditText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
            }
			// Если найдено слово из рифмующихся пар, то выделяем его в розовый
            else
            {
                cf = { sizeof(cf) };
                cf.dwMask = CFM_COLOR;
                cf.crTextColor = color;
                SendMessageW(buttons::widgets.hEditText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
            }

            SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)wword.c_str());

            firstWord = false;
        } 
        // Сбрасываем цвет текста на черный 
        CHARFORMAT2 cf_def = { sizeof(cf_def) };
        cf_def.dwMask = CFM_COLOR;
        cf_def.crTextColor = RGB(0, 0, 0);
        SendMessageW(buttons::widgets.hEditText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf_def);
        SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
    }
}

// Универсальная функция для вывода строки с цветным словом-цветом в RichEdit
void OutputColoredPart(HWND hEdit, const std::wstring& partName, const std::wstring& colorName, COLORREF color)
{
    wstring prefix;
	// Если слово-цвет пустое, то выводим только часть речи
    if (colorName == L"пусто")
    {
		prefix = partName + L"\n";
        SendMessageW(hEdit, EM_REPLACESEL, FALSE, (LPARAM)prefix.c_str());
        return;
	}
	// Если выделять розовым
    else if (colorName == L" выделено в тексте розовым")
    {
        prefix = partName;
        SendMessageW(hEdit, EM_REPLACESEL, FALSE, (LPARAM)prefix.c_str());
    }
	// Если слово-цвет не пустое, то выводим часть речи с цветом
    else
    {
        prefix = partName + L" - ";
        SendMessageW(hEdit, EM_REPLACESEL, FALSE, (LPARAM)prefix.c_str());
    }

    // Устанавливаем цвет для слова-цвета
    CHARFORMAT2 cf = { sizeof(cf) };
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = color;
    SendMessageW(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    // Выводим слово-цвет
    SendMessageW(hEdit, EM_REPLACESEL, FALSE, (LPARAM)colorName.c_str());

    // Сброс цвета на чёрный
    cf.crTextColor = RGB(0, 0, 0);
    SendMessageW(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    // Перевод строки
    SendMessageW(hEdit, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
}

// Функция для загрузки морфем из файла
vector<string> OpenFileMorphemes(HWND hWnd)
{
	vector<string> morphemesError;
    // инициализация векторов окончаний и суффиксов всех 6 частей речи
    // Загрузка морфемных правил из файлов
    buttons::morphemeRules = {
        {"nouns_endings", load_morphemes("nouns_endings.txt")},
        {"nouns_suffixes", load_morphemes("nouns_suffixes.txt")},
        {"adjectives_endings", load_morphemes("adjectives_endings.txt")},
        {"adjectives_suffixes", load_morphemes("adjectives_suffixes.txt")},
        {"participles_endings", load_morphemes("participles_endings.txt")}, // Новое
        {"participles_suffixes", load_morphemes("participles_suffixes.txt")}, // Новое
        {"verbs_endings", load_morphemes("verbs_endings.txt")},
        {"verbs_suffixes", load_morphemes("verbs_suffixes.txt")},
        {"gerunds_endings", load_morphemes("gerunds_endings.txt")},
        {"gerunds_suffixes", load_morphemes("gerunds_suffixes.txt")},
        {"adverbs_endings", load_morphemes("adverbs_endings.txt")},
        {"adverbs_suffixes", load_morphemes("adverbs_suffixes.txt")},
        {"others_list", load_morphemes("others_list.txt")}
    };
	// Проверка на успешную загрузку
	for (const auto& [key, value] : buttons::morphemeRules) {
		if (value[0] == "error") 
        {
			morphemesError.push_back(key);
			
		}
	}
    return morphemesError;
}

// Функция для вывода информации о рифмах в поле RichEdit
void OutputRhymeInfo(const vector<WordData>& rhymes_data, string& compare_word) 
{
	// Группировка рифм по частям речи
    unordered_map<string, vector<WordData>> grouped = {
        { "глагол", {}},
        {"наречие", {}},
        {"прилагательное", {}},
        {"существительное", {}},
        {"причастие", {}},
        {"деепричастие", {}}
    };

	// Группируем рифмы по частям речи
    for (const auto& word : rhymes_data) 
    {
        grouped[word.part_of_speech].push_back(word);
    }

    // Очистка поля
    SetWindowTextW(buttons::widgets.hEditRhymes, L"");

    // Заголовок
    wstring output_text;
    // Вывод в поле рифм при поиске без слова
    if (compare_word == "")
    {
        output_text = L"Результаты поиска рифм по частям речи:\r\n\r\n";
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)output_text.c_str());
        output_text = L"";
        // Для каждой активной части речи
        if (buttons::ButtonFlags.test(0))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Глагол", L"выделен в тексте красным", RGB(200, 0, 0));
        if (buttons::ButtonFlags.test(1))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Наречие", L"выделено в тексте фиолетовым", RGB(150, 0, 150));
        if (buttons::ButtonFlags.test(3))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Существительное", L"выделено в тексте синим", RGB(0, 0, 200));
        if (buttons::ButtonFlags.test(2))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Прилагательное", L"выделено в тексте зелёным", RGB(0, 150, 0));
        if (buttons::ButtonFlags.test(5))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Деепричастие", L"выделено в тексте жёлтым", RGB(184, 134, 11));
        if (buttons::ButtonFlags.test(4))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Причастие", L"выделено в тексте бирюзовым", RGB(0, 128, 128));
    }
    // Вывод в поле рифм при неоднородном поиске по слову 
    else if (buttons::ButtonFlags.test(7) == false)
    {
        output_text = L"Результаты поиска рифм для слова \"" + ansi_to_wstring(compare_word) + (buttons::ButtonFlags.test(7) == 1 ? L"\" по части речи слова:\r\n\r\n" : L"\" по частям речи:\r\n\r\n");
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)output_text.c_str());
        output_text = L"";
        // Для каждой активной части речи
        if (buttons::ButtonFlags.test(0))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Глагол", L"пусто", RGB(200, 0, 0));
        if (buttons::ButtonFlags.test(1))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Наречие", L"пусто", RGB(150, 0, 150));
        if (buttons::ButtonFlags.test(3))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Существительное", L"пусто", RGB(0, 0, 200));
        if (buttons::ButtonFlags.test(2))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Прилагательное", L"пусто", RGB(0, 150, 0));
        if (buttons::ButtonFlags.test(5))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Деепричастие", L"пусто", RGB(184, 134, 11));
        if (buttons::ButtonFlags.test(4))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Причастие", L"пусто", RGB(0, 128, 128));

        OutputColoredPart(buttons::widgets.hEditRhymes, L"\nРифмованные пары будут", L"выделены в тексте розовым", RGB(255, 0, 255));
    }
    // Вывод в поле рифм при однородном поиске по слову 
    else
    {
        output_text = L"Результаты поиска рифм для слова \"" + ansi_to_wstring(compare_word) + (buttons::ButtonFlags.test(7) == 1 ? L"\" по части речи слова:\r\n\r\n" : L"\" по частям речи:\r\n\r\n");
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)output_text.c_str());
        output_text = L"";
        // Для каждой активной части речи
        if (buttons::ButtonFlags.test(0))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Глагол", L"выделен в тексте красным", RGB(200, 0, 0));
        if (buttons::ButtonFlags.test(1))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Наречие", L"выделено в тексте фиолетовым", RGB(150, 0, 150));
        if (buttons::ButtonFlags.test(3))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Существительное", L"выделено в тексте синим", RGB(0, 0, 200));
        if (buttons::ButtonFlags.test(2))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Прилагательное", L"выделено в тексте зелёным", RGB(0, 150, 0));
        if (buttons::ButtonFlags.test(5))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Деепричастие", L"выделено в тексте жёлтым", RGB(184, 134, 11));
        if (buttons::ButtonFlags.test(4))
            OutputColoredPart(buttons::widgets.hEditRhymes, L"• Причастие", L"выделено в тексте бирюзовым", RGB(0, 128, 128));

        OutputColoredPart(buttons::widgets.hEditRhymes, L"\nРифмованные пары будут", L"выделены в тексте розовым", RGB(255, 0, 255));

        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)output_text.c_str());
    }
    
    output_text += L"\r\n";
    output_text += L"Тип поиска: " + wstring(buttons::ButtonFlags.test(7) ? L"Однородный" : L"Неоднородный");
    output_text += L"\r\nВывод слов осуществляется по мере их появлений в тексте";
    SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)output_text.c_str());

    // Порядок вывода
    const vector<string> part_order = {
        "глагол",
        "наречие",
        "прилагательное",
        "существительное",
        "причастие",
        "деепричастие"
    };
	// Выводим слова по частям речи в заданном порядке
    for (const auto& [part_key, words] : grouped)
    {
        // Цвет текста
        COLORREF color = RGB(0, 0, 0);
         // Выбираем цвет в зависимости от части речи
            if (part_key == "глагол") color = RGB(200, 0, 0);
            else if (part_key == "наречие") color = RGB(150, 0, 150);
            else if (part_key == "прилагательное") color = RGB(0, 150, 0);
            else if (part_key == "существительное") color = RGB(0, 0, 200);
            else if (part_key == "причастие") color = RGB(0, 128, 128);
            else if (part_key == "деепричастие") color = RGB(184, 134, 11);

        CHARFORMAT2 cf = { sizeof(cf) };
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = color;
        SendMessageW(buttons::widgets.hEditRhymes, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

		if (words.empty()) continue;
        wstring header;
        
        // Вывод шапки блока частей речи 
            header = L"\r\n\r\n====================\r\n" +
                ansi_to_wstring(capitalizeAllLetters(part_key)) +
                L"\r\n====================";
		
            
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)header.c_str());

		COLORREF save_color = color; // Сохраняем цвет для сброса
		// Сброс цвета
        color = RGB(0, 0, 0);
        cf = { sizeof(cf) };
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = color;
        SendMessageW(buttons::widgets.hEditRhymes, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

        // Вывод информации о каждом слове
        for (const WordData& output : words)
        {
            // Формируем строку "Слово: "
            wstring word_info = L"\r\n\r\nСлово: ";
            // Выводим заголовок для слова
            SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)word_info.c_str());

            // Устанавливаем цвет для основного слова (по части речи)
            color = save_color;
            cf = { sizeof(cf) };
            cf.dwMask = CFM_COLOR;
            cf.crTextColor = color;
            SendMessageW(buttons::widgets.hEditRhymes, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

            // Преобразуем слово в wstring и выводим его
            word_info = ansi_to_wstring(output.word);
            SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)word_info.c_str());

            // Сбрасываем цвет на чёрный для последующего текста
            color = RGB(0, 0, 0);
            cf = { sizeof(cf) };
            cf.dwMask = CFM_COLOR;
            cf.crTextColor = color;
            SendMessageW(buttons::widgets.hEditRhymes, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

            // Формируем и выводим строку с частью речи и количеством появлений
            word_info = L"\r\nЧасть речи: " + ansi_to_wstring(output.part_of_speech) +
                L"\r\nКоличество появлений в тексте: " + to_wstring(output.amount);
            SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)word_info.c_str());

            // Формируем строку с номерами предложений, где найдено слово
            wstring sentence_info = L"\r\nНайдено в предложении(-ях): ";
            if (!output.sentence_counter.empty())
            {
                // Перечисляем номера предложений через запятую
                for (size_t i = 0; i < output.sentence_counter.size(); ++i)
                {
                    if (i > 0) sentence_info += L", ";
                    sentence_info += to_wstring(output.sentence_counter[i]);
                }
            }
            else
            {
                // Если не найдено ни в одном предложении
                sentence_info += L"не найдено";
            }
            SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)sentence_info.c_str());

            // Если есть рифмующиеся слова
            if (!output.rhymed_words.empty())
            {
                // Выводим заголовок для рифмующихся слов
                SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)L"\r\nРифмующееся(-иеся) слово(-а):");
                int index = 0;
                // Для каждого рифмующегося слова
                for (const auto& rhymed : output.rhymed_words)
                {
                    // Формируем строку с рифмующимся словом и номерами предложений
                    wstring rhyme = L"\r\n  • " + ansi_to_wstring(rhymed);
                    rhyme += L" (предложение(-я): ";
                    for (auto pos : output.rhymed_words_sentences_number[index])
                    {
                        rhyme += to_wstring(pos);
                        // Если не последний номер, добавляем запятую
                        if (pos != output.rhymed_words_sentences_number[index].back())
                        {
                            rhyme += L", ";
                        }
                    }
                    rhyme += L")";
                    // Выводим рифмующееся слово с номерами предложений
                    SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)rhyme.c_str());
                    index++;
                }
            }
            else
            {
                // Если рифмующихся слов нет
                SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)L"\r\nРифмующихся слов нет");
            }
        }

        // Сброс цвета на чёрный после вывода всех слов
        cf.crTextColor = RGB(0, 0, 0);
        SendMessageW(buttons::widgets.hEditRhymes, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    }

    // Прокрутка вверх
    SendMessageW(buttons::widgets.hEditRhymes, EM_SETSEL, 0, 0);
    SendMessageW(buttons::widgets.hEditRhymes, EM_SCROLLCARET, 0, 0);
}

// Функция для получения активных частей речи из битового флага
wstring GetActivePartsOfSpeech(const bitset<8>& ButtonFlags) 
{
    wstring result;
    bool first = true;

    // Проверяем каждый бит и добавляем соответствующую часть речи
    if (ButtonFlags.test(0)) { // Глагол
        if (!first) result += L", ";
        result += L"Глагол";
        first = false;
    }
    if (ButtonFlags.test(1)) { // Наречие
        if (!first) result += L", ";
        result += L"Наречие";
        first = false;
    }
    if (ButtonFlags.test(2)) { // Прилагательное
        if (!first) result += L", ";
        result += L"Прилагательное";
        first = false;
    }
    if (ButtonFlags.test(3)) { // Существительное
        if (!first) result += L", ";
        result += L"Существительное";
        first = false;
    }
    if (ButtonFlags.test(4)) { // Причастие
        if (!first) result += L", ";
        result += L"Причастие";
        first = false;
    }
    if (ButtonFlags.test(5)) { // Деепричастие
        if (!first) result += L", ";
        result += L"Деепричастие";
        first = false;
    }

    if (result.empty()) {
        result = L"Нет выбранных частей речи";
    }

    return result;
}

// Основной цикл программы
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static string str_sentences;                        // Строка для хранения текста
    static vector<vector<string>> sentences;    // Вектор текста с разделенными предложениями
    static vector<WordData> rhymes_data;        // Найденные рифмы к данному слову
	static string compare_word;                 // Слово для поиска рифм
    switch (msg)
    {
		// Создание окна
    case WM_CREATE:
    {
		// Создание меню
        MainWndAddMenus(hWnd);

		// Создание виджетов
        MainWndAddWidget(hWnd);

		// Создание словаря для хранения частей речи
        
        EnableWindow(buttons::widgets.hSaveFile, FALSE);  // Блокировка кнопки
        EnableWindow(buttons::widgets.hSearch, FALSE);  // Блокировка кнопки
        UpdateWindow(buttons::widgets.hSaveFile);
        void UpdateCheckboxStates();

        // Настройка параметров чтения файла
        SetOpenFileParams(hWnd);

		// Сброс флагов кнопок
        buttons::ButtonFlags.reset();

		// Создание фона
        buttons::graphics.hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons::graphics.hBrush);

        

        // Создание кистей для кнопок
        buttons::graphics.hBrushRed = CreateSolidBrush(RGB(205, 92, 92));
        buttons::graphics.hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
        buttons::graphics.hBrushGrey = CreateSolidBrush(RGB(178, 178, 178));
        buttons::graphics.hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        buttons::graphics.hBrushNeutral = CreateSolidBrush(RGB(127, 255, 212)); 
		SetWindowTextW(buttons::widgets.hEditInputWord, L"");
        SetWindowTextW(buttons::widgets.hEditText, L"");
        break;
    }

    case WM_COMMAND:
    {
        // Проверяем, если уведомление пришло от поля ввода слова
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == buttons::widgets.hEditInputWord)
        {
            if (buttons::ButtonFlags.test(7) == true)
            {
                buttons::ButtonFlags.reset(0);
                buttons::ButtonFlags.reset(1);
                buttons::ButtonFlags.reset(2);
                buttons::ButtonFlags.reset(3);
                buttons::ButtonFlags.reset(4);
                buttons::ButtonFlags.reset(5);
            }
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Выход"
        if (LOWORD(wp) == buttons::buttonIDs.ButExit)
        {
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hExitButton, L"");
            InvalidateRect(buttons::widgets.hExitButton, NULL, TRUE); 
            UpdateWindow(buttons::widgets.hExitButton);
            ExitSoftware();
        }
		// Нажата кнопка "Тип поиска"
        else if (LOWORD(wp) == buttons::buttonIDs.ButSearchType)
        {
            buttons::ButtonFlags.flip(7);
            bool hasInput = GetWindowTextLengthW(buttons::widgets.hEditInputWord) > 0;

			// Если нажата кнопка "Однородный поиск" и введено слово для поиска
            if (buttons::ButtonFlags.test(7) == true and hasInput == true)
            {
                buttons::ButtonFlags.reset(0);
                buttons::ButtonFlags.reset(1);
                buttons::ButtonFlags.reset(2);
                buttons::ButtonFlags.reset(3);
                buttons::ButtonFlags.reset(4);
                buttons::ButtonFlags.reset(5);
            }
            // Перерисовываем кнопку
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Все части речи"
		else if (LOWORD(wp) == buttons::buttonIDs.ButAll)
		{
			if (buttons::ButtonFlags.test(0) == true and buttons::ButtonFlags.test(1) == true and
                buttons::ButtonFlags.test(2) == true and buttons::ButtonFlags.test(3) == true and
                buttons::ButtonFlags.test(4) == true and buttons::ButtonFlags.test(5) == true)
			{
                buttons::ButtonFlags.reset(0).reset(1).reset(2).reset(3).reset(4).reset(5);
			}
            else
            {
                buttons::ButtonFlags.set(0).set(1).set(2).set(3).set(4).set(5);
            }
			
			// Перерисовываем кнопку
			SetWindowText(buttons::widgets.hAllButton, L"");
			InvalidateRect(buttons::widgets.hAllButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE); // Перерисовываем кнопку
            InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(buttons::widgets.hAllButton);
            UpdateWindow(buttons::widgets.hVerbButton);
            UpdateWindow(buttons::widgets.hAdverbButton);
            UpdateWindow(buttons::widgets.hAdjectiveButton);
            UpdateWindow(buttons::widgets.hNounButton);
            UpdateWindow(buttons::widgets.hParticipleButton);
            UpdateWindow(buttons::widgets.hAdverbialButton);
			UpdateCheckboxStates();
		}
		// Нажата кнопка "Глагол"
        else if (LOWORD(wp) == buttons::buttonIDs.ButVerb)
        {
            buttons::ButtonFlags.flip(0);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hVerbButton, L"");
            InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hVerbButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Наречие"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverb)
        {
            buttons::ButtonFlags.flip(1);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hAdverbButton, L"");
            InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdverbButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Прилагательное"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdjective)
        {
            buttons::ButtonFlags.flip(2);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hAdjectiveButton, L"");
            InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdjectiveButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Существительное"
        else if (LOWORD(wp) == buttons::buttonIDs.ButNoun)
        {
            buttons::ButtonFlags.flip(3);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hNounButton, L"");
            InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hNounButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Причастие"
        else if (LOWORD(wp) == buttons::buttonIDs.ButParticiple)
        {
            buttons::ButtonFlags.flip(4);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hParticipleButton, L"");
            InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hParticipleButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Деепричастие"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverbial)
        {
            buttons::ButtonFlags.flip(5);
            // Перерисовываем кнопку
            SetWindowText(buttons::widgets.hAdverbialButton, L"");
            InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdverbialButton);
            UpdateCheckboxStates();
        }
		// Нажата кнопка "Выход из программы"
        else if (LOWORD(wp) == buttons::buttonIDs.OnExitSoftware)
        {
            ExitSoftware();
        }
		// Нажата кнопка "Поиск"
        else if (LOWORD(wp) == buttons::buttonIDs.Search)
        {
            // Сброс флага поиска слова
			buttons::ButtonFlags[6] = 0; 

			// Получаем текст из поля редактирования
            char WordToSearch[120] = ""; 
            GetWindowTextA(buttons::widgets.hEditInputWord, WordToSearch, 120);

            // слово для поиска по нему рифм
			string word_to_compare = WordToSearch; 
			

			// Проверяем, если поле ввода пустое
			if (word_to_compare.length() != 0)
			{
                buttons::ButtonFlags[6] = true;
			}
			// Проверяем, если кнопки не нажаты
            if (buttons::ButtonFlags.test(0) == 0 and buttons::ButtonFlags.test(1) == 0 and
                buttons::ButtonFlags.test(2) == 0 and buttons::ButtonFlags.test(3) == 0 and
                buttons::ButtonFlags.test(4) == 0 and buttons::ButtonFlags.test(5) == 0 and
                buttons::ButtonFlags.test(6) == 0)
            {
                MessageBoxA(hWnd, "Выберите часть речи или введите слово для поиска", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }
			// Проверяем, если выбрано несколько частей речи и выбран режим неоднородного поиска
            if (buttons::ButtonFlags.count() < 2 and
                buttons::ButtonFlags.test(7) == 0)
            {
                MessageBoxA(hWnd, "Выберите несколько частей речи или введите слово для поиска", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }

            wstring wword = ansi_to_wstring(word_to_compare);
            wstring filtered;
            bool started = false;
            bool wordChanged = false; // Флаг для отслеживания изменения слова
            bool prolonged_ckeck = false;
            bool error_word = false;

			// Фильтрация слова: удаление пробелов в начале и конце, проверка на наличие букв
            for (wchar_t wc : wword)
            {
                if (!started)
                {
                    if (iswspace(wc)) continue; // Пропускаем ведущие пробелы
                    if (iswalpha(wc))
                    {
                        started = true;
                        filtered += wc;
                        wordChanged = true; // Устанавливаем флаг, если слово изменяется
                    }
                }
                else
                {
                    if (iswspace(wc)) prolonged_ckeck = true; // Остановиться на первом пробеле после слова
                    if (iswalpha(wc) and prolonged_ckeck == false)
                    {
                        filtered += wc;
                        wordChanged = true; // Устанавливаем флаг, если слово изменяется
                    }
                    if (prolonged_ckeck == true)
                    {
                        if (iswalpha(wc))
                        {
                            error_word = true;
                            break;
                        }
                    }
                }
            }

            word_to_compare = wstring_to_ansi(filtered);
            compare_word = word_to_compare;

            // Проверяем на пустоту и изменение слова
            if (buttons::ButtonFlags[6] == true)
            {
                if (compare_word.empty() or error_word == true)
                {
                    MessageBoxA(hWnd, "Проверьте корректность слова для поиска рифм", "Ошибка", MB_OK | MB_ICONERROR);
                    buttons::ButtonFlags.reset();
                    UpdateButtonStatesAndColors();
                    UpdateCheckboxStates();
                    EnableWindow(buttons::widgets.hSaveFile, FALSE);
                    UpdateWindow(buttons::widgets.hSaveFile);
                    break;
                }
            }
            
           
            // Показываем окно загрузки
            ShowLoadingWindow(hWnd);

            // Получаем найденные рифмы, разделенные предложения и флаги 
            unite_functions(rhymes_data, sentences, str_sentences, word_to_compare, buttons::ButtonFlags);

            

            // Очищаем содержимое поля перед добавлением нового текста
            SetWindowTextA(buttons::widgets.hEditRhymes, "");
            SetWindowTextA(buttons::widgets.hEditText, "");
            UpdateWindow(buttons::widgets.hLoadingWnd);

			// Если не найдено рифм, выводим сообщение
            if (rhymes_data.empty())
            {
                HideLoadingWindow(hWnd);
                MessageBoxA(hWnd, "Не найдено рифм!", "Предупреждение", MB_OK | MB_ICONERROR);
                EnableWindow(buttons::widgets.hSaveFile, FALSE);
                UpdateWindow(buttons::widgets.hSaveFile);
                buttons::ButtonFlags.reset();
                UpdateButtonStatesAndColors();
                UpdateCheckboxStates();
                break;
            }
            else if(sentences.empty())
            {
                HideLoadingWindow(hWnd);
                MessageBoxA(hWnd, "Не найдено предложений", "Предупреждение", MB_OK | MB_ICONERROR);
                EnableWindow(buttons::widgets.hSaveFile, FALSE);
                UpdateWindow(buttons::widgets.hSaveFile);
                buttons::ButtonFlags.reset();
                UpdateButtonStatesAndColors();
                UpdateCheckboxStates();
                break;
            }
            else
            {
                EnableWindow(buttons::widgets.hSaveFile, TRUE);
                UpdateWindow(buttons::widgets.hSaveFile);
            }
            
            // Вывод рифм
            OutputRhymeInfo(rhymes_data, compare_word);

            UpdateWindow(buttons::widgets.hLoadingWnd);

            // Вывод текста
            OutputTextInfo(sentences, rhymes_data, compare_word);
            
            

            // Скрываем окно загрузки
            HideLoadingWindow(hWnd);

            // Перерисовываем поле текста
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE);
            UpdateWindow(buttons::widgets.hEditText);
            UpdateWindow(buttons::widgets.hEditRhymes);
            buttons::SaveButtonFlags = buttons::ButtonFlags;
            buttons::ButtonFlags.reset();
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();

            /*
            buttons::ButtonFlags.reset();
                UpdateButtonStatesAndColors();
                UpdateCheckboxStates();
                EnableWindow(buttons::widgets.hSaveFile, FALSE);
                UpdateWindow(buttons::widgets.hSaveFile);
            */


        }
		// Нажата кнопка "Сохранить файл"
		else if (LOWORD(wp) == buttons::buttonIDs.ButSaveFile)
		{
            // ВЫЗОВ ФУНКЦИИ ДЛЯ ВЫВОДА В ФАЙЛ
            string outputFileName_numbered; // имя выходного файла-текста
            string outputFileName_rhymes; // имя выходного файла-рифм

            outputFiles_working(filename_str, outputFileName_numbered, outputFileName_rhymes, sentences, rhymes_data, compare_word);
            wstring wtext, wrhymes;
            wtext += L"Файл с пронумерованным текстом: " + utf8_to_wstring(ansi_to_utf8(outputFileName_numbered));
            wrhymes += L"Файл с найденными рифмами: " + utf8_to_wstring(ansi_to_utf8(outputFileName_rhymes));
            // Устанавливаем текст в поле "Выбранный файл"
            SetWindowTextW(buttons::widgets.hPathSaveFileData, wtext.c_str());
            SetWindowTextW(buttons::widgets.hPathSaveFileRhymes, wrhymes.c_str());
			compare_word = "";
            UpdateWindow(buttons::widgets.hPathSaveFileData);
            UpdateWindow(buttons::widgets.hPathSaveFileRhymes);
            EnableWindow(buttons::widgets.hSaveFile, FALSE);  // Блокировка кнопки
            UpdateWindow(buttons::widgets.hSaveFile);
		}
        // Нажатие кнопки "чтение файла"
        else if (LOWORD(wp) == buttons::buttonIDs.ButOpenFile)
        {
            vector <string> ErrorFiles = OpenFileMorphemes(hWnd);

			// Проверка наличия ошибок при открытии файлов
            if (!ErrorFiles.empty())
            {
                string errors = "Не удалось загрузить файл(ы): ";
                for (auto it = ErrorFiles.begin(); it != ErrorFiles.end(); ++it)
                {
                    errors += *it;
                    if (std::next(it) != ErrorFiles.end())
                        errors += ", ";
                }
                MessageBoxA(hWnd, errors.c_str(), "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }

            // Проверка успешности выбора файла
            if (GetOpenFileNameA(&OFN)) // Вызов GetOpenFileNameW
            {
                filename_str = filename;
                pair<bool, string> fromFunct = inputFile_working(filename_str);

                if (fromFunct.first == false)
                {
                    if (fromFunct.second == "Ошибка!: Пустой путь к файлу!")
                    {
                        MessageBoxA(hWnd, "Пустой путь к файлу!", "Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                    else if (fromFunct.second == "Ошибка!: Неверное расширение!")
                    {
                        MessageBoxA(hWnd, "Неверное расширение!", "Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                    else if (fromFunct.second == "Ошибка!: Не удалось открыть файл!")
                    {
                        MessageBoxA(hWnd, "Не удалось открыть файл!", "Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                    else if (fromFunct.second == "Не удалось выполнить чтение файла: Файл пуст!")
                    {
                        MessageBoxA(hWnd, "Не удалось выполнить чтение файла: Файл пуст!", "Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                    else if (fromFunct.second == "Ошибка!: Неверная кодировка файла!")
                    {
                        MessageBoxA(hWnd, "Неверная кодировка файла!", "Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                }
				
                else
                {
                    str_sentences = fromFunct.second; // Получаем текст из функции
					if (isFalseCoding(str_sentences) == true)
					{
						// Если кодировка файла не ANSI, выводим сообщение об ошибке
						// и не сохраняем текст в поле вывода
						str_sentences = "";
						rhymes_data.clear();
						sentences.clear();
						//buttons::ButtonFlags.reset();
						SetWindowTextA(buttons::widgets.hEditRhymes, "");
						SetWindowTextA(buttons::widgets.hEditText, "");
						SetWindowTextA(buttons::widgets.hOutputStatus, ""); // Очищаем поле статуса
					
						MessageBoxA(hWnd, "Кодировка файла не ANSI", "Ошибка", MB_OK | MB_ICONERROR);
                        EnableWindow(buttons::widgets.hSaveFile, FALSE);  // Блокировка кнопки
                        UpdateWindow(buttons::widgets.hSaveFile);
						break;
					}
                    SetWindowTextA(buttons::widgets.hOutputStatus, filename_str.c_str());
                    UpdateCheckboxStates();
                }
            }
            break;
        }
        // Нажата кнопка "Инфо"
        else if (LOWORD(wp) == buttons::buttonIDs.OnInfoClicked)
        {
            MessageBox(hWnd, L"Информация о программе и ее разработчиках:", L"Инфо", MB_OK | MB_ICONINFORMATION);
        }
        // Нажата кнопка "Справка"
        else if (LOWORD(wp) == buttons::buttonIDs.OnHelp) {
            MessageBox(hWnd, L"Инструкции по использованию программы:\n"
                L"1. Выберите часть речи или введите слово для поиска.\n"
                L"2. Нажмите кнопку 'Поиск' для начала поиска рифм.\n"
                L"3. Результаты поиска будут отображены в соответствующих полях.",
                L"Справка", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
	// Отработка визуализации кнопок
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
        MakeRoundButton(lpDrawItem);
        break;
    }
	// Отработка визуализации картинок и рамок
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Рисуем черную рамку вокруг статических полей

        HPEN hOldPen = (HPEN)SelectObject(hdc, buttons::graphics.hPenBlack);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

		// Массив всех HWND для статических полей
        HWND All_hwnd[11] = { buttons::widgets.hOutputStatus, buttons::widgets.hOutputStatusText, 
                             buttons::widgets.hEditRhymes, buttons::widgets.hOutputRhymes, 
                             buttons::widgets.hOutputText, buttons::widgets.hEditText, 
                             buttons::widgets.hInputWord, buttons::widgets.hEditInputWord,
                             buttons::widgets.hPathSaveFileData, buttons::widgets.hPathSaveFileRhymes,
                             buttons::widgets.hPathSaveFileText };

		// Рисуем рамки для всех статических полей
        for (int i = 0; i < 11; i++)
        {
            MakeFrame(hWnd, hdc, All_hwnd[i]);
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);

        EndPaint(hWnd, &ps);
        break;
    }
	// Отработка визуализации статических полей
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wp;
        HWND hStatic = (HWND)lp;

		// Проверяем, является ли статическое поле одним из тех, которые мы хотим стилизовать
        if (hStatic == buttons::widgets.hOutputStatus || hStatic == buttons::widgets.hOutputStatusText || hStatic == buttons::widgets.hOutputStatus ||
            hStatic == buttons::widgets.hOutputRhymes || hStatic == buttons::widgets.hOutputText || hStatic == buttons::widgets.hLoadingWnd)
        {
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // Устанавливаем цвет фона (белый)
            SetTextColor(hdcStatic, RGB(0, 0, 0));     // Устанавливаем цвет текста (черный)
            return (INT_PTR)buttons::graphics.hBrush;            // Возвращаем кисть для фона
        }
        break;
    }
	// Отработка визуализации полей ввода
    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wp;
        HWND hEdit = (HWND)lp;

        if (hEdit == buttons::widgets.hEditRhymes || hEdit == buttons::widgets.hEditText || hEdit == buttons::widgets.hEditInputWord)
        {
            SetBkColor(hdcEdit, RGB(255, 255, 255)); // Устанавливаем цвет фона (белый)
            SetTextColor(hdcEdit, RGB(0, 0, 0));     // Устанавливаем цвет текста (черный)
            return (INT_PTR)buttons::graphics.hBrush;          // Возвращаем кисть для фона
        }
        break;
    }
	
    // Отработка завершения работы
    case WM_DESTROY:
    {	// Освобождаем ресурсы
        DeleteObject(buttons::graphics.hBrushRed);
        DeleteObject(buttons::graphics.hBrushGreen);
        DeleteObject(buttons::graphics.hBrushGrey);
        DeleteObject(buttons::graphics.hBrush);
        DeleteObject(buttons::graphics.hBrushNeutral);
        DeleteObject(buttons::graphics.hPenBlack);
        DeleteObject(buttons::graphics.hdcMem);
        ExitSoftware();
        break;
    }
	// Дефолтная обработка сообщений
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}

// Функция для освобождения памяти, выделенной для LPWSTR
void FreeLPWSTR(LPWSTR lpwstr)
{
    delete[] lpwstr;
}

// Функция для преобразования LPWSTR в std::string
string ConvertLPWSTRToString(LPWSTR lpwstr)
{
	// Проверяем, что lpwstr не является нулевым указателем
    if (!lpwstr) {
        std::cerr << "Null LPWSTR" << std::endl;
        return std::string();
    }
	// Преобразуем LPWSTR в std::string
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) {
        DWORD error = GetLastError();
        std::cerr << "WideCharToMultiByte failed, error: " << error << std::endl;
        return std::string();
    }

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, &result[0], size_needed, nullptr, nullptr);

    // Удаляем завершающий нуль
    if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }

    return result;
}

// Функция для преобразования std::string в LPWSTR
LPWSTR ConvertStringToLPWSTR(const std::string &str)
{
	// Проверяем, что строка не пустая
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    LPWSTR lpwstr = new WCHAR[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, lpwstr, size_needed);
    return lpwstr;
}

// Отрисовка рамок статических полей и полей ввода
void MakeFrame(HWND hWnd, HDC hdc, HWND Edit)
{
	// Получаем размеры элемента управления
    RECT rect;
    GetWindowRect(Edit, &rect);
    MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);

    // Рисуем черную рамку вокруг элемента управления
    HPEN hOldPen = (HPEN)SelectObject(hdc, buttons::graphics.hPenBlack);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
}

// Функция для создания закругленной кнопки
BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem)
{
    HDC hdc = lpDrawItem->hDC;
    RECT rect = lpDrawItem->rcItem;

    // Создаем кисть цвета фона окна (чтобы скрыть старые границы)
    HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255)); // Цвет фона окна
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush); // Удаляем кисть после использования

    // Выбираем нужную кисть для кнопки
    HBRUSH hBrushes = nullptr;
    BOOL isActive = FALSE;
    const char *buttonText = nullptr;
	// Определяем текст кнопки в зависимости от ее идентификатора
	// Кнопка "Выход"
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hBrushes = buttons::graphics.hBrushRed;
        buttonText = "Выход";
    }
	// Кнопка "Все части речи"
	else if (lpDrawItem->hwndItem == buttons::widgets.hAllButton)
	{
		isActive = buttons::ButtonFlags.all();
		hBrushes = buttons::graphics.hBrushNeutral;
		buttonText = "Все части речи";
	}
	// Кнопка "Глагол"
    else if (lpDrawItem->hwndItem == buttons::widgets.hVerbButton)
    {
        isActive = buttons::ButtonFlags.test(0);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Глагол";
    }

	// Кнопка "Наречие"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbButton)
    {
        isActive = buttons::ButtonFlags.test(1);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Наречие";
    }

	// Кнопка "Прилагательное"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdjectiveButton)
    {
        isActive = buttons::ButtonFlags.test(2);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Прилагательное";
    }

	// Кнопка "Существительное"
    else if (lpDrawItem->hwndItem == buttons::widgets.hNounButton)
    {
        isActive = buttons::ButtonFlags.test(3);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Существительное";
    }

	// Кнопка "Причастие"
    else if (lpDrawItem->hwndItem == buttons::widgets.hParticipleButton)
    {
        isActive = buttons::ButtonFlags.test(4);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Причастие";
    }

	// Кнопка "Деепричастие"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbialButton)
    {
        isActive = buttons::ButtonFlags.test(5);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Деепричастие";
    }

	// Кнопка "Режим поиска"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearchType)
    {
        isActive = buttons::ButtonFlags.test(7);
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = isActive ? "Текущий режим: однородный" : "Текущий режим: неоднородный";
    }

	// Кнопка "Поиск"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        BOOL enabled = IsWindowEnabled(buttons::widgets.hSearch);
        hBrushes = enabled ? buttons::graphics.hBrushNeutral : buttons::graphics.hBrushGrey;
        buttonText = "Поиск";
    }
    // Кнопка "Открыть файл"
    else if (lpDrawItem->hwndItem == buttons::widgets.hOpenFile)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "Открыть файл";
    }
    // Кнопка "Сохранить файл"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSaveFile)
    {
        // Проверяем, активна ли кнопка "Сохранить файл"
        if (IsWindowEnabled(lpDrawItem->hwndItem)) {
            hBrushes = buttons::graphics.hBrushNeutral; // Голубой (если активна)
        }
        else {
            hBrushes = buttons::graphics.hBrushGrey; // Серый (если заблокирована)
        }
        buttonText = "Сохранить файл";
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;
     
    // Создаем обрамление для кнопки
    if (lpDrawItem->hwndItem == buttons::widgets.hSearchType ||
        lpDrawItem->hwndItem == buttons::widgets.hOpenFile ||
        lpDrawItem->hwndItem == buttons::widgets.hAllButton)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // Синее обрамление для голубой кнопки
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        BOOL enabled = IsWindowEnabled(buttons::widgets.hSearch);
        hPen = CreatePen(PS_SOLID, 1, enabled ? RGB(127, 255, 212) : RGB(205, 92, 92));
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(205, 92, 92));
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSaveFile)
    {
        BOOL enabled = IsWindowEnabled(buttons::widgets.hSaveFile);
        hPen = CreatePen(PS_SOLID, 1, enabled ? RGB(127, 255, 212) : RGB(205, 92, 92));
    }
    else
    {
        hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
    }


	// Устанавливаем цвет текста и кисть для кнопки
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrushes);

    if (IsWindowEnabled(lpDrawItem->hwndItem)) 
    {
        SetTextColor(hdc, RGB(0, 0, 0)); // Ярко-чёрный для активной кнопки
    }
    else 
    {
        SetTextColor(hdc, RGB(120, 120, 120)); // Бледно-серый для неактивной
    }

    // Если кнопка нажата, смещаем прямоугольник и изменяем цвет фона
    if (lpDrawItem->itemState & ODS_SELECTED)
    {
        OffsetRect(&rect, 4, 4);
        rect.right -= 4;
        rect.bottom -= 4;
    }

    // Рисуем закругленный прямоугольник
    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 25, 25);

    // Отключаем стандартный фон и устанавливаем цвет текста
    SetBkMode(hdc, TRANSPARENT);

    // Центрируем текст внутри кнопки
    DrawTextA(hdc, buttonText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Очищаем ненужные ресурсы GDI
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    return TRUE;
}

// Завершение работы приложения
void ExitSoftware()
{
    PostQuitMessage(0);
}

// Создание пунктов меню
void MainWndAddMenus(HWND hWnd)
{
    HMENU RootMenu = CreateMenu();
    HMENU SubMenu = CreateMenu();

    // Создание основного меню
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Файл");

    //AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnHelp, L"Справка"); // Пункт справки

    // Создание подменю Файл

    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, buttons::buttonIDs.OnExitSoftware, L"Завершить работу");

    SetMenu(hWnd, RootMenu);
}

// Создание виджетов
void MainWndAddWidget(HWND hWnd)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // Пропорции для кнопок и виджетов
    int buttonWidth = screenWidth * 15 / 100;  // 15% от ширины экрана
    int buttonHeight = screenHeight * 4 / 100; // 5% от высоты экрана
    int marginX = screenWidth * 2 / 100;       // 2% от ширины экрана
    int marginY = screenHeight * 2 / 100;      // 2% от высоты экрана

    // Кнопки
    buttons::widgets.hExitButton = CreateButton("Выход", marginX, screenHeight - buttonHeight - marginY-80, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButExit);
	buttons::widgets.hAllButton = CreateButton("Все части речи", marginX, marginY + 20, buttonWidth, buttonHeight,
		hWnd, buttons::buttonIDs.ButAll);
    buttons::widgets.hVerbButton = CreateButton("Глагол", marginX, marginY + buttonHeight + marginY + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButVerb);
    buttons::widgets.hAdjectiveButton = CreateButton("Прилагательное", marginX, marginY + 2 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdjective);
    buttons::widgets.hNounButton = CreateButton("Существительное", marginX, marginY + 3 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButNoun);
    buttons::widgets.hAdverbialButton = CreateButton("Деепричастие", marginX, marginY + 4 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdverbial);
    buttons::widgets.hParticipleButton = CreateButton("Причастие", marginX, marginY + 5 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButParticiple);
    buttons::widgets.hAdverbButton = CreateButton("Наречие", marginX, marginY + 6 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdverb);
    buttons::widgets.hSearchType = CreateButton("Тип поиска", marginX, marginY + 7 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButSearchType);
    buttons::widgets.hSearch = CreateButton("Поиск", marginX, screenHeight - 2 * (buttonHeight + marginY) - 100, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.Search);
    buttons::widgets.hOpenFile = CreateButton("Открыть файл", 2 * marginX + buttonWidth, marginY-10, buttonWidth, 30,
        hWnd, buttons::buttonIDs.ButOpenFile);
    buttons::widgets.hSaveFile = CreateButton("Сохранить файл", 2 * marginX + buttonWidth, marginY+30, buttonWidth, 30,
        hWnd, buttons::buttonIDs.ButSaveFile);
    

    // Статические элементы
    buttons::widgets.hOutputStatusText = CreateRichEdit(L"Открыт файл: ", 
        10*marginX + buttonWidth, marginY-10, buttonWidth-80, 25, hWnd, true);
    buttons::widgets.hPathSaveFileText = CreateRichEdit(L"Сохранения: ",
        10 * marginX + buttonWidth, marginY+30, buttonWidth - 80, 25, hWnd, true);
    buttons::widgets.hInputWord = CreateRichEdit(L"Слово для поиска рифм", 
        marginX, marginY + 7 * (buttonHeight + marginY) + 20 + buttonHeight + 11, buttonWidth, 25, hWnd, true);
    buttons::widgets.hOutputRhymes = CreateRichEdit(L"Найденные рифмы", 
        marginX + buttonWidth + marginX, marginY + buttonHeight + marginY+50, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 301, buttonHeight, hWnd, true);
    buttons::widgets.hOutputText = CreateRichEdit(L"Текст с найденными рифмами", 
        marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 300, marginY + buttonHeight + marginY+50, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2+300, buttonHeight, hWnd, true);


    // Поля редактирования
    buttons::widgets.hOutputStatus = CreateRichEdit(L"",10*marginX + buttonWidth + buttonWidth - 79, 
        marginY-10, buttonWidth*3, 25, hWnd, true);
    buttons::widgets.hPathSaveFileData = CreateRichEdit(L"", 10 * marginX + buttonWidth + buttonWidth - 79,
        marginY+30, buttonWidth * 3, 25, hWnd, true);
    buttons::widgets.hPathSaveFileRhymes = CreateRichEdit(L"", 10 * marginX + buttonWidth + buttonWidth - 79,
        marginY + 56, buttonWidth * 3, 25, hWnd, true);
    buttons::widgets.hEditInputWord = CreateRichEdit(L"",marginX,
        marginY + 7 * (buttonHeight + marginY) + 25 + buttonHeight + 32, buttonWidth, 40, hWnd);
    SendMessage(buttons::widgets.hEditInputWord, EM_SETEVENTMASK, 0, ENM_CHANGE);

    // Верхняя граница для полей редактирования (вплотную с hOutputRhymes и hOutputText)
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1; 

    // Высота полей с учетом зазоров
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1; 
    
    buttons::widgets.hEditRhymes = CreateRichEdit(L"",marginX + buttonWidth + marginX, editTopMargin + 50,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 301, editHeight-50, hWnd, true);
    buttons::widgets.hEditText = CreateRichEdit(L"text",marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2-300, editTopMargin + 50,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2+300, editHeight-50, hWnd, true);
    

    
    buttons::widgets.hStaticCheckBox1Info = CreateRichEdit(L"Выберите НЕ МЕНЕЕ ДВУХ ч.р.",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 143,
        buttonWidth, 30, hWnd, true); 

    buttons::widgets.hStaticCheckBox2Info = CreateRichEdit(L"Выберите файл",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 173,
        buttonWidth, 30, hWnd, true);

    buttons::widgets.hStaticCheckBox3Info = CreateRichEdit(L"Неоднородный режим поиска",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 203,
        buttonWidth, 30, hWnd, true);

}

// Функция для создания RichEdit поля
HWND CreateRichEdit(LPCWSTR text, int x, int y, int width, int height, HWND hParent, bool readonly)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // Пропорции для кнопок и виджетов
    int buttonWidth = screenWidth * 15 / 100;  // 15% от ширины экрана
    int buttonHeight = screenHeight * 4 / 100; // 5% от высоты экрана
    int marginX = screenWidth * 2 / 100;       // 2% от ширины экрана
    int marginY = screenHeight * 2 / 100;      // 2% от высоты экрана
    // Верхняя граница для полей редактирования (вплотную с hOutputRhymes и hOutputText)
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1;

    // Высота полей с учетом зазоров
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1;

    // Убедись, что библиотека загружена
    static bool richEditLoaded = false;
    if (!richEditLoaded) {
        LoadLibrary(TEXT("Msftedit.dll"));
        richEditLoaded = true;
    }
    DWORD style = WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | WS_BORDER;
    if (readonly)
    {
        style |= ES_READONLY;
    }
    if (text == L"text")
    {
        style |= ES_LEFT;
    }
    else
    {
        style |= ES_CENTER;
    }
    HWND hRich = CreateWindowEx(
        0,
        MSFTEDIT_CLASS,  // "RichEdit50W"
        text,
        style,
        x, y, width, height,
        hParent, NULL, GetModuleHandle(NULL), NULL
    );

    if (!hRich) return NULL;

    // Если нужно сделать текст жирным
    if (text == L"Неоднородный режим поиска" or text == L"Найденные рифмы" or text == L"Текст с найденными рифмами"
        or text == L"Открыт файл: " or text == L"Сохранения: " or text == L"Слово для поиска рифм")
    {
        CHARFORMAT2 cf = { 0 };
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_BOLD;  // Указываем, что меняем жирность
        cf.dwEffects = CFE_BOLD; // Включаем жирный шрифт

        // Выделяем весь текст и применяем форматирование
        SendMessage(hRich, EM_SETSEL, 0, -1);
        SendMessage(hRich, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    return hRich;
}

// Функция для создания кнопки
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id)
{
	// Создаем кнопку с заданными параметрами
    HWND hButton = CreateWindowA(
        "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
	// Проверяем, была ли кнопка успешно создана
    if (!hButton)
    {
        MessageBoxA(hWnd, "Не удалось создать кнопку", "Ошибка", MB_OK | MB_ICONERROR);
    }

    return hButton;
}

// Функция для создания статического текста
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd)
{
	// Создаем статический текст с заданными параметрами
    HWND hStatic = 0;
	// Устанавливаем стиль текста в зависимости от текста
    if (text == "Открыт файл: " or text == "Сохранения: " or text == "Выберите НЕ МЕНЕЕ ДВУХ ч.р." or text == "Выберите файл" or text == "Неоднородный режим поиска")
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_LEFT,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
	// Если текст не соответствует заданным условиям, создаем обычный статический текст
    else
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_CENTER,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
	// Проверяем, был ли статический текст успешно создан
    if (!hStatic)
    {
        MessageBoxA(hWnd, "Не удалось создать статический текст", "Ошибка", MB_OK | MB_ICONERROR);
    }

    return hStatic;
}

// Функция для создания поля редактирования
HWND CreateEdit(int x, int y, int width, int height, HWND hWnd, bool readOnly)
{
    LoadLibraryA("Msftedit.dll");
	// Создаем поле редактирования с заданными параметрами
    DWORD style = WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL;
	// Если поле редактирования только для чтения, добавляем соответствующий стиль
    if (readOnly)
    {
        style |= ES_READONLY;
    }
	// Создаем поле редактирования
    HWND hEdit = CreateWindowExA(
        0, "RICHEDIT50W", "", style,
        x, y, width, height,
        hWnd, NULL, GetModuleHandle(NULL), NULL);
	// Проверяем, было ли поле редактирования успешно создано
    if (!hEdit)
    {
        MessageBoxA(hWnd, "Не удалось создать поле редактирования", "Ошибка", MB_OK | MB_ICONERROR);
    }

    // Снимаем лимит, RichEdit и так поддерживает большие объёмы, но можно явно задать
    SendMessageA(hEdit, EM_EXLIMITTEXT, 0, (WPARAM)-1); // -1 = максимально возможное

    return hEdit;
}

// Установка начальных параметров на открытие файлов для чтения и записи
void SetOpenFileParams(HWND hWnd)
{
	// Устанавливаем начальные параметры для открытия файла
    ZeroMemory(&OFN, sizeof(OFN));
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = filename;
    OFN.nMaxFile = sizeof(filename);
    OFN.lpstrFilter = "Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0";
    OFN.lpstrFileTitle = NULL;
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = "С:\\";
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

// Записать в статус программы
void SetWinStatus(string status)
{
    SetWindowTextA(buttons::widgets.hOutputStatus, status.c_str());
}

// Показать окно загрузки
void ShowLoadingWindow(HWND hWnd) {
    // Отключаем все кнопки, кроме кнопок тулбара
	EnableWindow(buttons::widgets.hAllButton, FALSE);
    EnableWindow(buttons::widgets.hVerbButton, FALSE);
    EnableWindow(buttons::widgets.hAdjectiveButton, FALSE);
    EnableWindow(buttons::widgets.hNounButton, FALSE);
    EnableWindow(buttons::widgets.hAdverbialButton, FALSE);
    EnableWindow(buttons::widgets.hParticipleButton, FALSE);
    EnableWindow(buttons::widgets.hAdverbButton, FALSE);
    EnableWindow(buttons::widgets.hSearchType, FALSE);
    EnableWindow(buttons::widgets.hSearch, FALSE);
    EnableWindow(buttons::widgets.hOpenFile, FALSE);

    // Размеры окна загрузки
    const int loadingWidth = 300;
    const int loadingHeight = 100;

    // Центрируем относительно главного окна
    RECT mainRect;
    GetWindowRect(hWnd, &mainRect);
    int centerX = mainRect.left + ((mainRect.right - mainRect.left) - loadingWidth) / 2;
    int centerY = mainRect.top + ((mainRect.bottom - mainRect.top) - loadingHeight) / 2;

    if (buttons::widgets.hLoadingWnd == NULL) {
        buttons::widgets.hLoadingWnd = CreateWindowEx(
            WS_EX_TOPMOST,
            L"STATIC",
            L"Поиск рифм... Пожалуйста, подождите.",
            WS_POPUP | WS_VISIBLE | WS_BORDER | SS_CENTER,
            centerX, centerY, loadingWidth, loadingHeight,
            NULL, // Важно: не делаем дочерним!
            NULL,
            GetModuleHandle(NULL),
            NULL
        );
    }
    else {
        SetWindowPos(buttons::widgets.hLoadingWnd, HWND_TOPMOST, centerX, centerY, loadingWidth, loadingHeight, SWP_SHOWWINDOW);
        ShowWindow(buttons::widgets.hLoadingWnd, SW_SHOW);
    }

    SetForegroundWindow(buttons::widgets.hLoadingWnd);
    UpdateWindow(buttons::widgets.hLoadingWnd);
}

// Скрыть окно загрузки
void HideLoadingWindow(HWND hWnd) {
    if (buttons::widgets.hLoadingWnd != NULL) {
        ShowWindow(buttons::widgets.hLoadingWnd, SW_HIDE);
        // Включаем все кнопки
		EnableWindow(buttons::widgets.hAllButton, TRUE);
        EnableWindow(buttons::widgets.hVerbButton, TRUE);
        EnableWindow(buttons::widgets.hAdjectiveButton, TRUE);
        EnableWindow(buttons::widgets.hNounButton, TRUE);
        EnableWindow(buttons::widgets.hAdverbialButton, TRUE);
        EnableWindow(buttons::widgets.hParticipleButton, TRUE);
        EnableWindow(buttons::widgets.hAdverbButton, TRUE);
        EnableWindow(buttons::widgets.hSearchType, TRUE);
        EnableWindow(buttons::widgets.hSearch, TRUE);
        EnableWindow(buttons::widgets.hOpenFile, TRUE);

        SetFocus(hWnd);
    }
}

