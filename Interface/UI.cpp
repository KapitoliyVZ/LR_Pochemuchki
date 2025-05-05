#include "UI_const.h"
#include "Heart_of_program.h"
#include "file_working.h"
#include "NLP.h"



void ShowLoadingWindow(HWND hWnd) {
    // Создаем кисть для фона окна
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0)); // Устанавливаем белый цвет фона
    // Устанавливаем кисть в качестве фона окна
    SetClassLongPtr(buttons::widgets.hLoadingWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
    HDC hdc = GetDC(buttons::widgets.hLoadingWnd);
    SetTextColor(hdc, RGB(0, 255, 0)); // Устанавливаем черный цвет текста
    // Получаем координаты окон "Найденные рифмы" и "Текст с найденными рифмами"
    if (buttons::widgets.hLoadingWnd == NULL) {
        buttons::widgets.hLoadingWnd = CreateWindowEx(
            0,
            L"STATIC",
            L"Поиск рифм... Пожалуйста, подождите.",
            WS_POPUP | WS_VISIBLE ,
            CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,
            hWnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        RECT rectInputWord;
        GetWindowRect(buttons::widgets.hEditInputWord, &rectInputWord);

        // Получаем координаты окон "Найденные рифмы" и "Текст с найденными рифмами"
        RECT rectRhymes, rectText;
        GetWindowRect(buttons::widgets.hEditRhymes, &rectRhymes);
        GetWindowRect(buttons::widgets.hEditText, &rectText);

        // Вычисляем среднюю точку между этими окнами
        int centerX = (rectRhymes.left + rectRhymes.right + rectText.left + rectText.right) / 4;
        int centerY = (rectRhymes.top + rectRhymes.bottom + rectText.top + rectText.bottom) / 4;

        // Центрируем окно загрузки относительно средней точки
        int x = centerX - 150; // 150 - половина ширины окна загрузки
        int y = centerY - 50;  // 50 - половина высоты окна загрузки
        SetWindowPos(buttons::widgets.hLoadingWnd, HWND_TOPMOST, x, y, 300, 100, SWP_SHOWWINDOW);

        //// Позиционируем окно загрузки под виджетом "Введите слово для поиска рифм"
        //int x = rectInputWord.left;
        //int y = rectInputWord.bottom + 10; // 10 - отступ от нижней границы виджета
        //SetWindowPos(buttons::widgets.hLoadingWnd, HWND_TOPMOST, x, y, 290, 100, SWP_SHOWWINDOW);

    }
    else {
        ShowWindow(buttons::widgets.hLoadingWnd, SW_SHOW);
    }
    UpdateWindow(buttons::widgets.hLoadingWnd);
    // Блокируем основное окно
    EnableWindow(hWnd, FALSE);
}

void HideLoadingWindow(HWND hWnd) {
    if (buttons::widgets.hLoadingWnd != NULL) {
        ShowWindow(buttons::widgets.hLoadingWnd, SW_HIDE);
        // Разблокируем основное окно
        EnableWindow(hWnd, TRUE);
        SetFocus(hWnd);
    }
}



// Старт приложения и создание стартовых процедур
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (!LoadLibraryA("Msftedit.dll")) 
    {
        MessageBoxA(NULL, "Не удалось загрузить Msftedit.dll", "Ошибка", MB_OK | MB_ICONERROR);
        return 0;
    }
    /*
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("en_US.UTF-8"));
    */

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
    HWND hMainWnd = CreateWindow(L"MainClass", L"ТРПО лабораторная", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

void UpdateButtonStatesAndColors()
{
    // Обновляем состояние кнопок в зависимости от флагов
    bool isHomogeneousMode = buttons::ButtonFlags[7]; // Флаг однородного режима
    bool hasInput = GetWindowTextLengthA(buttons::widgets.hEditInputWord) > 0;

    // Обновляем доступность кнопок
    EnableWindow(buttons::widgets.hVerbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdjectiveButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hNounButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hParticipleButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbialButton, !isHomogeneousMode || !hasInput);

    // Перерисовываем кнопки для обновления их цветов
    InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hSearchType, NULL, TRUE);

    // Обновляем окно для применения изменений
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



void UpdateCheckboxStates()
{
    // Проверка первого чекбокса
    bool isHomogeneousMode = buttons::ButtonFlags[7]; // Флаг однородного режима
    int selectedPartsOfSpeech = buttons::ButtonFlags.test(0) + buttons::ButtonFlags.test(1) +
        buttons::ButtonFlags.test(2) + buttons::ButtonFlags.test(3) +
        buttons::ButtonFlags.test(4) + buttons::ButtonFlags.test(5);

    if (isHomogeneousMode || selectedPartsOfSpeech > 1) {
        SendMessage(buttons::widgets.hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);
    }
    else {
        SendMessage(buttons::widgets.hCheckBox3, BM_SETCHECK, BST_UNCHECKED, 0);
    }

    // Проверка второго чекбокса
    bool isFileSelected = !filename_str.empty(); // Проверяем, выбран ли файл
    if (isFileSelected) {
        SendMessage(buttons::widgets.hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);
    }
    else {
        SendMessage(buttons::widgets.hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
    }

    // Проверка третьего чекбокса
    if (selectedPartsOfSpeech > 0) {
        SendMessage(buttons::widgets.hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);
    }
    else {
        SendMessage(buttons::widgets.hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
    }

    // Обновляем окна для применения изменений
    UpdateWindow(buttons::widgets.hCheckBox1);
    UpdateWindow(buttons::widgets.hCheckBox2);
    UpdateWindow(buttons::widgets.hCheckBox3);
}

// Основной цикл программы
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static string str_sentences;                        // Строка для хранения текста
    static vector<vector<string>> sentences;    // Вектор текста с разделенными предложениями
    static vector<WordData> rhymes_data;        // Найденные рифмы к данному слову

    switch (msg)
    {
		// Создание окна
    case WM_CREATE:
    {
		// Создание меню
        MainWndAddMenus(hWnd);

		// Создание виджетов
        MainWndAddWidget(hWnd);

        // Настройка параметров чтения файла
        SetOpenFileParams(hWnd);

		// Сброс флагов кнопок
        buttons::ButtonFlags.reset();

		// Создание фона
        buttons::graphics.hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons::graphics.hBrush);

        // Загрузка изображения
        buttons::graphics.hBitmap = (HBITMAP)LoadImageA(NULL, "Icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        buttons::graphics.hBitmap2 = (HBITMAP)LoadImageA(NULL, "7Institute.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!buttons::graphics.hBitmap || !buttons::graphics.hBitmap2)
        {
            MessageBoxA(hWnd, "Не удалось загрузить изображение", "Ошибка", MB_OK | MB_ICONERROR);
        }

        // Создание кистей для кнопок
        buttons::graphics.hBrushRed = CreateSolidBrush(RGB(205, 92, 92));
        buttons::graphics.hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
        buttons::graphics.hBrushGrey = CreateSolidBrush(RGB(178, 178, 178));
        buttons::graphics.hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        buttons::graphics.hBrushNeutral = CreateSolidBrush(RGB(127, 255, 212)); 
        // Показываем всплывающие подсказки
        break;
    }
    case WM_COMMAND:
    {
        


        // Проверяем, если есть используемый файл
        if (filename_str == "")
        {
            SetWindowTextA(buttons::widgets.hOutputStatus, "");
        }
        else
        {
            SendMessage(buttons::widgets.hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);
            UpdateWindow(buttons::widgets.hCheckBox2);
        }


        // Проверяем, если уведомление пришло от поля ввода слова для поиска по нему рифм
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == buttons::widgets.hEditInputWord)
        {
            // Получаем текст из поля редактирования
            char buffer[256] = { 0 };
            GetWindowTextA(buttons::widgets.hEditInputWord, buffer, sizeof(buffer));

            // Если поле не пустое, блокируем кнопки
            if ( (strlen(buffer) > 0) && buttons::ButtonFlags[7] == true)
            {
                EnableWindow(buttons::widgets.hVerbButton, FALSE);
                EnableWindow(buttons::widgets.hAdverbButton, FALSE);
                EnableWindow(buttons::widgets.hAdjectiveButton, FALSE);
                EnableWindow(buttons::widgets.hNounButton, FALSE);
                EnableWindow(buttons::widgets.hParticipleButton, FALSE);
                EnableWindow(buttons::widgets.hAdverbialButton, FALSE);
            }
            else // Если поле пустое, разблокируем кнопки
            {
                EnableWindow(buttons::widgets.hVerbButton, TRUE);
                EnableWindow(buttons::widgets.hAdverbButton, TRUE);
                EnableWindow(buttons::widgets.hAdjectiveButton, TRUE);
                EnableWindow(buttons::widgets.hNounButton, TRUE);
                EnableWindow(buttons::widgets.hParticipleButton, TRUE);
                EnableWindow(buttons::widgets.hAdverbialButton, TRUE);
            }
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
		// Нажата кнопка "Вывод на экран"
        else if (LOWORD(wp) == buttons::buttonIDs.ButSearchType)
        {
            buttons::ButtonFlags.flip(7);
            // Перерисовываем кнопку
            UpdateButtonStatesAndColors();
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
            char WordToSearch[60] = ""; 
            GetWindowTextA(buttons::widgets.hEditInputWord, WordToSearch, 60);

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

            // Показываем окно загрузки
            ShowLoadingWindow(hWnd);

            // Получаем найденные рифмы, разделенные предложения и флаги 
            unite_functions(rhymes_data, sentences, str_sentences, word_to_compare, buttons::ButtonFlags);

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // ВЫЗОВ ФУНКЦИИ ДЛЯ ВЫВОДА В ФАЙЛ
            string outputFileName_numbered; // имя выходного файла-текста
            string outputFileName_rhymes; // имя выходного файла-рифм
            outputFiles_working(filename_str, outputFileName_numbered, outputFileName_rhymes, sentences, rhymes_data);
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            // Скрываем окно загрузки
            HideLoadingWindow(hWnd);

            // Очищаем содержимое поля перед добавлением нового текста
            SetWindowTextA(buttons::widgets.hEditRhymes, "");
            SetWindowTextA(buttons::widgets.hEditText, "");

			// Если не найдено рифм, выводим сообщение
            if (rhymes_data.empty())
            {
                MessageBoxA(hWnd, "Не найдено рифм", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }
            if (sentences.empty())
            {
				MessageBoxA(hWnd, "Не найдено предложений", "Ошибка", MB_OK | MB_ICONERROR);
				break;
            }
            vector<vector<wstring>> Wsentences;

            
            // Копирование с преобразованием через лямбда-функцию
            for (const vector<string>& sentence : sentences)
            {
                vector<wstring> wsentence;
                for (const string& word : sentence)
                {
                    wsentence.push_back(utf8_to_wstring(word)); // Используем лямбда-функцию для преобразования
                }
                Wsentences.push_back(wsentence);
            }
            // Выводим текст в поле
            for (vector<wstring>& sentence : Wsentences)
            {
                bool firstWord = true;
                for (wstring& word : sentence)
                {
                    
                    wstring tmp_word = word;
                    static const set<wstring> punctuationMarks = { L".", L",", L"!", L"?", L":", L";", L"-", L"(", L")", L"\"", L"'" };

                    if (!firstWord && punctuationMarks.find(tmp_word) == punctuationMarks.end())
                    {
                        // Добавляем пробел перед словом (если это не пунктуация и не первое слово)
                        SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L" ");
                    }

                    SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)tmp_word.c_str());

                    firstWord = false;
                }

                // Перенос строки после предложения
                SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
            }
            int counter = 0;
            // Проходим по всем словам
            for (WordData& output : rhymes_data)
            {
                counter++;
                string wordInfo;
                if (counter == 1)
                {
                    wordInfo = "Слово: ";
                }
                else
                {
					wordInfo = "\r\nСлово: ";
                }

                output.word = utf8_to_ansi(output.word);
                
                wordInfo += output.word;

                // Вывод части речи
                string part_of_speech = "\r\nЧасть речи: " + output.part_of_speech;
				string amount_number = "\r\nКоличество найденных слов: " + to_string(output.amount);
				string rhymes_number = "\r\nКоличество рифм: " + to_string(output.rhymed_amount);
				wordInfo += utf8_to_ansi(amount_number);
				wordInfo += utf8_to_ansi(rhymes_number);
				

				wordInfo += utf8_to_ansi(part_of_speech);
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)wordInfo.c_str());
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)part_of_speech.c_str());
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)amount_number.c_str());
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)rhymes_number.c_str());

				string setence_counter = "\r\nНайдено в следующих предложениях: ";
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)setence_counter.c_str());
                string buffer;
				for (int& word : output.sentence_counter)
				{
					buffer += to_string(word);
					buffer += ", ";
				}
				// Удаляем последний символ ", "
				if (!buffer.empty())
				{
					buffer.erase(buffer.length() - 2);
				}
				
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)buffer.c_str());
                // Если есть рифмы, добавляем их
                if (!output.rhymed_words.empty())
                {
                    SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)"\r\nРифмы: ");
                    for (string& word : output.rhymed_words)
                    {
                        word = utf8_to_ansi(word);
                        string rhyme = "\r\n  - " + word;
                        SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)rhyme.c_str());
                    }
                }

                // Добавляем разделитель между словами
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)"\n");
            }

            // Перерисовываем поле текста
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE);
            UpdateWindow(buttons::widgets.hEditText);
            buttons::ButtonFlags.reset();
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();
        }
        
        // Нажатие кнопки "чтение файла"
        else if (LOWORD(wp) == buttons::buttonIDs.OnReadFile)
        {
          
            // Проверка успешности выбора файла
            if (GetOpenFileNameA(&OFN)) // Вызов GetOpenFileNameW
            {
                filename_str = filename;
                pair<bool, string> fromFunct = inputFile_working(filename_str);
                bool fromFunctStatus = fromFunct.first;         // статус проверки файла
                string fromFunctText = fromFunct.second;        // текст в строковой записи или ошибка файла


				if (fromFunctStatus == false)
				{
					MessageBoxA(hWnd, fromFunctText.c_str(), "Ошибка", MB_OK | MB_ICONERROR);
					break;
				}
                
				str_sentences = fromFunctText; // Получаем текст из функции
                SetWindowTextA(buttons::widgets.hOutputStatus, filename_str.c_str());

                // open_file(filename_str);
                UpdateCheckboxStates();
            }
            else
            {
                MessageBoxA(hWnd, "Ошибка выбора файла.", "Ошибка", MB_OK | MB_ICONERROR);
            }


            break;
        }
        // Есди нажата кнопка "Инфо"
        else if (LOWORD(wp) == buttons::buttonIDs.OnInfoClicked)
        {
            MessageBox(hWnd, L"Информация о программе и ее разработчиках:", L"Инфо", MB_OK | MB_ICONINFORMATION);
        }
        // Нажатие кнопки "Справка"
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

        BITMAP bitmap;
        BITMAP bitmap2;
        // Отображение первого изображения
        if (buttons::graphics.hBitmap)
        {
            buttons::graphics.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons::graphics.hdcMem, buttons::graphics.hBitmap);

            GetObject(buttons::graphics.hBitmap, sizeof(BITMAP), &bitmap);
            BitBlt(hdc, 20, 0, bitmap.bmWidth, bitmap.bmHeight, buttons::graphics.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons::graphics.hdcMem, hOldBitmap);
            DeleteDC(buttons::graphics.hdcMem);
        }

        // Отображение второго изображения
        if (buttons::graphics.hBitmap2)
        {
            buttons::graphics.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons::graphics.hdcMem, buttons::graphics.hBitmap2);

            GetObject(buttons::graphics.hBitmap2, sizeof(BITMAP), &bitmap2);
            BitBlt(hdc, bitmap.bmWidth + 20, 0, bitmap2.bmWidth, bitmap2.bmHeight, buttons::graphics.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons::graphics.hdcMem, hOldBitmap);
            DeleteDC(buttons::graphics.hdcMem);
        }

        // Рисуем черную рамку вокруг статических полей

        HPEN hOldPen = (HPEN)SelectObject(hdc, buttons::graphics.hPenBlack);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        HWND All_hwnd[8] = { buttons::widgets.hOutputStatus, buttons::widgets.hOutputStatusText, 
                             buttons::widgets.hEditRhymes, buttons::widgets.hOutputRhymes, 
                             buttons::widgets.hOutputText, buttons::widgets.hEditText, 
                             buttons::widgets.hInputWord, buttons::widgets.hEditInputWord};

        for (int i = 0; i < 8; i++)
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
        DeleteObject(buttons::graphics.hBitmap);
        DeleteObject(buttons::graphics.hBitmap2);
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
std::string ConvertLPWSTRToString(LPWSTR lpwstr)
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
    HBRUSH hBrushes;
    BOOL isActive = FALSE;
    const char *buttonText;
	// Определяем текст кнопки в зависимости от ее идентификатора
	// Кнопка "Выход"
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hBrushes = buttons::graphics.hBrushRed;
        buttonText = "Выход";
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
        buttonText = isActive ? "Режим поиска: однородный" : "Режим поиска: неоднородный";
    }

	// Кнопка "Поиск"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "Поиск";
    }

	// Иное значение кнопки
    else
    {
        return FALSE; // Неизвестная кнопка
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;
     
	// Создаем обрамление для кнопки
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton ||
        lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // Синее обрамление для голубой кнопки
    }
    else
    {
        hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
    }

	// Устанавливаем цвет текста и кисть для кнопки
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrushes);
    SetTextColor(hdc, RGB(0, 0, 0));

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
    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnReadFile, L"Чтение файла");
    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnHelp, L"Справка"); // Пункт справки

    // Создание подменю Файл
    AppendMenu(SubMenu, MF_STRING, buttons::buttonIDs.OnInfoClicked, L"Инфо");
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
    buttons::widgets.hExitButton = CreateButton("Выход", marginX, screenHeight - buttonHeight - marginY-100, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButExit);
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
    buttons::widgets.hSearch = CreateButton("Поиск", marginX, screenHeight - 2 * (buttonHeight + marginY) - 120, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.Search);
    //buttons::widgets.hHelpButton = CreateButton("Справка", marginX, screenHeight - buttonHeight - marginY - 70, buttonWidth, buttonHeight,
       // hWnd, buttons::buttonIDs.OnHelp);


    // Статические элементы
    buttons::widgets.hOutputStatusText = CreateStatic("Выбранный файл: ", 
        marginX + buttonWidth + marginX, marginY, buttonWidth-100, 20, hWnd);
    buttons::widgets.hInputWord = CreateStatic("Введите слово для поиска рифм", 
        marginX, marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 11, buttonWidth, 20, hWnd);
    buttons::widgets.hOutputRhymes = CreateStatic("Найденные рифмы", 
        marginX + buttonWidth + marginX, marginY + buttonHeight + marginY, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, buttonHeight, hWnd);
    buttons::widgets.hOutputText = CreateStatic("Текст с найденными рифмами", 
        marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, marginY + buttonHeight + marginY, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, buttonHeight, hWnd);


    // Поля редактирования
    buttons::widgets.hOutputStatus = CreateStatic("",marginX + buttonWidth + marginX + buttonWidth - 99, 
        marginY, buttonWidth*4, 20, hWnd);
    buttons::widgets.hEditInputWord = CreateEdit(marginX, 
        marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 32, buttonWidth, 60, hWnd, false);

    // Верхняя граница для полей редактирования (вплотную с hOutputRhymes и hOutputText)
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1; 

    // Высота полей с учетом зазоров
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1; 
    
    buttons::widgets.hEditRhymes = CreateEdit(marginX + buttonWidth + marginX, editTopMargin, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, editHeight, hWnd, true);
    buttons::widgets.hEditText = CreateEdit(marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, editTopMargin, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, editHeight, hWnd, true);


    buttons::widgets.hCheckBox1 = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_CHECKBOX ,marginX, screenHeight - 2 * (buttonHeight + marginY) - 150,
        12, buttonHeight-10,hWnd, (HMENU)buttons::buttonIDs.CheckBox1, NULL, NULL);
	buttons::widgets.hStaticCheckBox1Info = CreateStatic("Выберите часть речи", marginX+15, screenHeight - 2 * (buttonHeight + marginY) - 143, 
        buttonWidth-10, 20, hWnd);
    buttons::widgets.hCheckBox2 = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_CHECKBOX ,marginX, screenHeight - 2 * (buttonHeight + marginY) - 170,
        12, buttonHeight - 10, hWnd, (HMENU)buttons::buttonIDs.CheckBox2, NULL, NULL);
    buttons::widgets.hStaticCheckBox2Info = CreateStatic("Выберите файл", marginX + 15, screenHeight - 2 * (buttonHeight + marginY) - 163, 
        buttonWidth - 10, 20, hWnd);
    buttons::widgets.hCheckBox3 = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_CHECKBOX , marginX, screenHeight - 2 * (buttonHeight + marginY) - 190,
        12, buttonHeight - 10,hWnd, (HMENU)buttons::buttonIDs.CheckBox3, NULL, NULL);
	buttons::widgets.hStaticCheckBox3Info = CreateStatic("Выберите тип поиска", marginX + 15, screenHeight - 2 * (buttonHeight + marginY) - 183, 
        buttonWidth - 10, 20, hWnd);

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
    if (text == "Выбранный файл: " or text == "Выберите часть речи" or text == "Выберите файл" or text == "Выберите тип поиска")
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
// Запись в файл
void save_data(LPCSTR path)
{
	// Создаем файл для записи
    HANDLE SAVEfile = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    int SAVElength = GetWindowTextLength(buttons::widgets.hSearch) + 1;
    char *data = new char[SAVElength];

    SAVElength = GetWindowTextA(buttons::widgets.hSearch, data, SAVElength);

    DWORD bytesIterated;
	// Записываем данные в файл
    WriteFile(SAVEfile, data, SAVElength, &bytesIterated, NULL);
    CloseHandle(SAVEfile);
    delete[] data;
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
    OFN.lpstrFilter = "*.txt";
    OFN.lpstrFileTitle = NULL;
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = "D:\\";
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

// Записать в статус программы
void SetWinStatus(string status)
{
    SetWindowTextA(buttons::widgets.hOutputStatus, status.c_str());
}

