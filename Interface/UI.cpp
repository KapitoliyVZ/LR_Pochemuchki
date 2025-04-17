#include "UI_const.h"
#include "Heart_of_program.h"
#include "file_working.h"
#include "NLP.h"



// Старт приложения и создание стартовых процедур
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
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

// Основной цикл программы
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static string filename = "";


    switch (msg)
    {
    case WM_CREATE:
    {
        MainWndAddMenus(hWnd);
        MainWndAddWidget(hWnd);
        SetOpenFileParams(hWnd, filename);
        buttons::ButtonFlags.reset();
        buttons::graphics.hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Фон окна (Gainsboro)
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons::graphics.hBrush);

        // Загрузка изображения
        buttons::graphics.hBitmap = (HBITMAP)LoadImageA(NULL, "Icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        buttons::graphics.hBitmap2 = (HBITMAP)LoadImageA(NULL, "7Institute.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!buttons::graphics.hBitmap || !buttons::graphics.hBitmap2)
        {
            MessageBoxA(hWnd, "Не удалось загрузить изображение", "Ошибка", MB_OK | MB_ICONERROR);
        }

        // Создание кистей для кнопки
        buttons::graphics.hBrushRed = CreateSolidBrush(RGB(205, 92, 92));
        buttons::graphics.hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
        buttons::graphics.hBrushGrey = CreateSolidBrush(RGB(178, 178, 178));
        buttons::graphics.hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        buttons::graphics.hBrushNeutral = CreateSolidBrush(RGB(127, 255, 212));
        break;
    }
    case WM_COMMAND:
    {
        if ((!file_input.is_open()))
        {
            SetWindowTextA(buttons::widgets.hOutputStatus, "");
        }
        // Проверяем, если уведомление пришло от поля редактирования
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == buttons::widgets.hEditInputWord)
        {
            // Получаем текст из поля редактирования
            char buffer[256] = { 0 };
            GetWindowTextA(buttons::widgets.hEditInputWord, buffer, sizeof(buffer));

            // Если поле не пустое, блокируем кнопки
            if (strlen(buffer) > 0)
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
        //break;
        if (LOWORD(wp) == buttons::buttonIDs.ButExit)
        {
            SetWindowText(buttons::widgets.hExitButton, L"");
            InvalidateRect(buttons::widgets.hExitButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hExitButton);
            ExitSoftware();
        }

        else if (LOWORD(wp) == buttons::buttonIDs.ButSearchType)
        {
            buttons::ButtonFlags.flip(7);
            SetWindowText(buttons::widgets.hSearchType, L"");
            InvalidateRect(buttons::widgets.hSearchType, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hSearchType);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButVerb)
        {
            buttons::ButtonFlags.flip(0);
            SetWindowText(buttons::widgets.hVerbButton, L"");
            InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hVerbButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverb)
        {
            buttons::ButtonFlags.flip(1);
            SetWindowText(buttons::widgets.hAdverbButton, L"");
            InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdverbButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdjective)
        {
            buttons::ButtonFlags.flip(2);
            SetWindowText(buttons::widgets.hAdjectiveButton, L"");
            InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdjectiveButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButNoun)
        {
            buttons::ButtonFlags.flip(3);
            SetWindowText(buttons::widgets.hNounButton, L"");
            InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hNounButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButParticiple)
        {
            buttons::ButtonFlags.flip(4);
            SetWindowText(buttons::widgets.hParticipleButton, L"");
            InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hParticipleButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverbial)
        {
            buttons::ButtonFlags.flip(5);
            SetWindowText(buttons::widgets.hAdverbialButton, L"");
            InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hAdverbialButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.OnExitSoftware)
        {
            ExitSoftware();
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ClearRhymes)
        {
            SetWindowText(buttons::widgets.hEditRhymes, L"");
            InvalidateRect(buttons::widgets.hEditRhymes, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hEditRhymes);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ClearText)
        {
            SetWindowText(buttons::widgets.hEditText, L"");
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons::widgets.hEditText);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.Search)
        {
			buttons::ButtonFlags[6] = 0; // Сброс флага поиска слова
            char WordToSearch[60] = ""; 
            GetWindowTextA(buttons::widgets.hEditInputWord, WordToSearch, 60);
			string word_to_compare = WordToSearch; // слово для поиска по нему рифм
			if (word_to_compare.length() != 0)
			{
                buttons::ButtonFlags.flip(6);
			}
            
            if (buttons::ButtonFlags.test(0) == 0 and buttons::ButtonFlags.test(1) == 0 and
                buttons::ButtonFlags.test(2) == 0 and buttons::ButtonFlags.test(3) == 0 and
                buttons::ButtonFlags.test(4) == 0 and buttons::ButtonFlags.test(5) == 0 and
                buttons::ButtonFlags.test(6) == 0)
            {
                MessageBoxA(hWnd, "Выберите часть речи или введите слово для поиска", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }
            if (buttons::ButtonFlags.count() < 2 and
                buttons::ButtonFlags.test(7) == 0)
            {
                MessageBoxA(hWnd, "Выберите несколько частей речи или введите слово для поиска", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }
            /*if (filename.length() == 0)
            {
                MessageBoxA(hWnd, "Выберите файл с текстом для поиска рифм", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }*/


            vector<WordData> rhymes_data;     // Найденные рифмы к данному слову
            vector<vector<string>> sentences; // Текст с разделенными предложениями

			// Получаем найденные рифмы, разделенные предложения и флаги 
            unite_functions(rhymes_data, sentences, word_to_compare, buttons::ButtonFlags);


            // Очищаем содержимое поля перед добавлением нового текста
            SetWindowTextA(buttons::widgets.hEditRhymes, "");
            SetWindowTextA(buttons::widgets.hEditText, "");

            // Проходим по всем словам
            if (rhymes_data.empty())
            {
                MessageBoxA(hWnd, "Не найдено рифм", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }
            int counter = 0;
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

                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)wordInfo.c_str());

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
            
        }
        // Нажатие кнопки "чтение файла"
        
        else if (LOWORD(wp) == buttons::buttonIDs.OnReadFile)
        {
            wchar_t filePath[MAX_PATH] = {}; // Обязательно выделяем буфер
            
            
            ////////////////////////////
            // штука для буфера
            OPENFILENAMEW OFN = { 0 };
            OFN.lStructSize = sizeof(OPENFILENAMEW);
            OFN.hwndOwner = nullptr; // или HWND твоего окна
            OFN.lpstrFile = filePath;
            OFN.nMaxFile = MAX_PATH;
            OFN.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
            OFN.nFilterIndex = 1;
            OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            ///////////////////////////////////

            std::string filename;
            // Проверка успешности выбора файла
            if (GetOpenFileNameW(&OFN)) // Вызов GetOpenFileNameW
            {
                filename = ConvertLPWSTRToString(OFN.lpstrFile); // Сохраняем выбранный файл

                if (checkName_openFile(filename)) // Проверка файла
                {
                    if (IsWindow(hWnd))
                    {
                        SetWinStatus(filename);
                    }
                    else
                    {
                        MessageBoxA(NULL, "Ошибка: недействительное окно!", "Ошибка", MB_OK | MB_ICONERROR);
                    }
                }
                else
                {
                    MessageBoxA(hWnd, "Файл не прошел проверку.", "Ошибка", MB_OK | MB_ICONERROR);
                }
            }
            else
            {
                MessageBoxA(hWnd, "Ошибка открытия файла.", "Ошибка", MB_OK | MB_ICONERROR);
            }
            break;
        }
        else if (LOWORD(wp) == buttons::buttonIDs.OnSaveFile)
        {
            if (GetSaveFileNameW(&OFN))
            {
            }
        }
        else if (LOWORD(wp) == buttons::buttonIDs.OnInfoClicked)
        {
            MessageBox(hWnd, L"Информация о программе и ее разработчиках:", L"Инфо", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
        MakeRoundButton(lpDrawItem);
        break;
    }

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

        HWND All_hwnd[8] = { buttons::widgets.hOutputStatus, buttons::widgets.hOutputStatusText, buttons::widgets.hEditRhymes,
                            buttons::widgets.hOutputRhymes, buttons::widgets.hOutputText, buttons::widgets.hEditText, buttons::widgets.hInputWord, buttons::widgets.hEditInputWord};

        for (int i = 0; i < 8; i++)
        {
            MakeFrame(hWnd, hdc, All_hwnd[i]);
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wp;
        HWND hStatic = (HWND)lp;

        if (hStatic == buttons::widgets.hOutputStatus || hStatic == buttons::widgets.hOutputStatusText || hStatic == buttons::widgets.hOutputStatus ||
            hStatic == buttons::widgets.hOutputRhymes || hStatic == buttons::widgets.hOutputText)
        {
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // Устанавливаем цвет фона (белый)
            SetTextColor(hdcStatic, RGB(0, 0, 0));     // Устанавливаем цвет текста (черный)
            return (INT_PTR)buttons::graphics.hBrush;            // Возвращаем кисть для фона
        }
        break;
    }
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
    case WM_QUIT:
    {

    }
    case WM_DESTROY:
        
        DeleteObject(buttons::graphics.hBrushRed);
        DeleteObject(buttons::graphics.hBrushGreen);
        DeleteObject(buttons::graphics.hBrushGrey);
        DeleteObject(buttons::graphics.hBrush);
        ExitSoftware();
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}

void FreeLPWSTR(LPWSTR lpwstr)
{
    delete[] lpwstr;
}

// Функция для преобразования LPWSTR в std::string
std::string ConvertLPWSTRToString(LPWSTR lpwstr)
{
    if (!lpwstr) {
        std::cerr << "Null LPWSTR" << std::endl;
        return std::string();
    }

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
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    LPWSTR lpwstr = new WCHAR[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, lpwstr, size_needed);
    return lpwstr;
}

void MakeFrame(HWND hWnd, HDC hdc, HWND Edit)
{
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

BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem)
{
    HDC hdc = lpDrawItem->hDC; // Используем локальную переменную HDC
    RECT rect = lpDrawItem->rcItem;

    // Создаем кисть цвета фона окна (чтобы скрыть старые границы)
    HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255)); // Цвет фона окна
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush); // Удаляем кисть после использования

    // Выбираем нужную кисть для кнопки
    HBRUSH hBrushes;
    BOOL isActive = FALSE;
    const char *buttonText;
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hBrushes = buttons::graphics.hBrushRed;
        buttonText = "Выход";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hToggleButton)
    {
        isActive = buttons::ShowInEdit;
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = isActive ? "Вывод на экран включен" : "Вывод на экран выключен";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hVerbButton)
    {
        isActive = buttons::ButtonFlags.test(0);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Глагол";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbButton)
    {
        isActive = buttons::ButtonFlags.test(1);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Наречие";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdjectiveButton)
    {
        isActive = buttons::ButtonFlags.test(2);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Прилагательное";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hNounButton)
    {
        isActive = buttons::ButtonFlags.test(3);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Существительное";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hParticipleButton)
    {
        isActive = buttons::ButtonFlags.test(4);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Причастие";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbialButton)
    {
        isActive = buttons::ButtonFlags.test(5);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "Деепричастие";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearchType)
    {
        isActive = buttons::ButtonFlags.test(7);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = isActive ? "Режим поиска: однородный" : "Режим поиска: неоднородный";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "Поиск";
    }
    else
    {
        return FALSE; // Неизвестная кнопка
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;

    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton ||
        lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // Синее обрамление для голубой кнопки
    }
    else
    {
        hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
    }
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
    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnSaveFile, L"Запись файла");

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
    int buttonWidth = screenWidth * 12 / 100;  // 15% от ширины экрана
    int buttonHeight = screenHeight * 4 / 100; // 5% от высоты экрана
    int marginX = screenWidth * 2 / 100;       // 2% от ширины экрана
    int marginY = screenHeight * 2 / 100;      // 2% от высоты экрана

    // Кнопки
    buttons::widgets.hExitButton = CreateButton("Выход", marginX, screenHeight - buttonHeight - marginY-100, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButExit);
    buttons::widgets.hVerbButton = CreateButton("Глагол", marginX, marginY + buttonHeight + marginY + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButVerb);
    buttons::widgets.hAdjectiveButton = CreateButton("Прилагательное", marginX, marginY + 2 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdjective);
    buttons::widgets.hNounButton = CreateButton("Существительное", marginX, marginY + 3 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButNoun);
    buttons::widgets.hAdverbialButton = CreateButton("Деепричастие", marginX, marginY + 4 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdverbial);
    buttons::widgets.hParticipleButton = CreateButton("Причастие", marginX, marginY + 5 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButParticiple);
    buttons::widgets.hAdverbButton = CreateButton("Наречие", marginX, marginY + 6 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdverb);
    buttons::widgets.hSearchType = CreateButton("Тип поиска", marginX, marginY + 7 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButSearchType);
    buttons::widgets.hSearch = CreateButton("Поиск", marginX, screenHeight - 2 * (buttonHeight + marginY) - 280, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.Search);


    // Статические элементы
    buttons::widgets.hOutputStatusText = CreateStatic("Выбранный файл: ", marginX + buttonWidth + marginX, marginY, buttonWidth-100, 20, hWnd);
    buttons::widgets.hInputWord = CreateStatic("Введите слово для поиска рифм", marginX, marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 11, buttonWidth, 20, hWnd);
    buttons::widgets.hOutputRhymes = CreateStatic("Найденные рифмы", marginX + buttonWidth + marginX, marginY + buttonHeight + marginY, (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, buttonHeight, hWnd);
    buttons::widgets.hOutputText = CreateStatic("Текст с найденными рифмами", marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, marginY + buttonHeight + marginY, (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, buttonHeight, hWnd);

    // Поля редактирования
    buttons::widgets.hOutputStatus = CreateStatic("",marginX + buttonWidth + marginX + buttonWidth - 99, marginY, buttonWidth*4, 20, hWnd);
    buttons::widgets.hEditInputWord = CreateEdit(
        marginX,
        marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 32, // Позиция сразу под hInputWord с зазором в 1 пиксель
        buttonWidth,
        60, // Высота поля
        hWnd,
        false
    );
    // Поля редактирования
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1; // Верхняя граница для полей редактирования (вплотную с hOutputRhymes и hOutputText)
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1; // Высота полей с учетом зазоров
    
    buttons::widgets.hEditRhymes = CreateEdit(
        marginX + buttonWidth + marginX,
        editTopMargin,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, // Ширина с учетом зазора
        editHeight,
        hWnd,
        true
    );

    buttons::widgets.hEditText = CreateEdit(
        marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 , // Сдвиг вправо с учетом зазора
        editTopMargin,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, // Ширина с учетом зазора
        editHeight,
        hWnd,
        true
    );
}

// Функция для создания кнопки
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id)
{
    HWND hButton = CreateWindowA(
        "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height, hWnd, (HMENU)id, NULL, NULL);

    if (!hButton)
    {
        MessageBoxA(hWnd, "Не удалось создать кнопку", "Ошибка", MB_OK | MB_ICONERROR);
    }

    return hButton;
}

// Функция для создания статического текста
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd)
{
    HWND hStatic = 0;
    if (text == "Выбранный файл: ")
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_LEFT,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
    else
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_CENTER,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
    if (!hStatic)
    {
        MessageBoxA(hWnd, "Не удалось создать статический текст", "Ошибка", MB_OK | MB_ICONERROR);
    }

    return hStatic;
}

// Функция для создания поля редактирования
HWND CreateEdit(int x, int y, int width, int height, HWND hWnd, bool readOnly)
{
    DWORD style = WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL;
    if (readOnly)
    {
        style |= ES_READONLY;
    }

    HWND hEdit = CreateWindowA(
        "edit", "", style,
        x, y, width, height, hWnd, NULL, NULL, NULL);

    if (!hEdit)
    {
        MessageBoxA(hWnd, "Не удалось создать поле редактирования", "Ошибка", MB_OK | MB_ICONERROR);
    }

    return hEdit;
}
// Запись в файл
void save_data(LPCSTR path)
{
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

    WriteFile(SAVEfile, data, SAVElength, &bytesIterated, NULL);
    CloseHandle(SAVEfile);
    delete[] data;
}

// Установка начальных параметров на открытие файлов для чтения и записи
void SetOpenFileParams(HWND hWnd, string filename)
{
    // Выделяем буфер для пути с учетом MAX_PATH
    WCHAR path[MAX_PATH] = L""; // Инициализируем пустой буфер

    // Инициализация структуры OPENFILENAME
    ZeroMemory(&OFN, sizeof(OFN)); // Очищаем структуру
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = path;  // Указываем буфер для хранения имени файла
    OFN.nMaxFile = MAX_PATH;  // Максимальная длина пути
    OFN.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0"; // Фильтр для файлов
    OFN.lpstrFileTitle = NULL;  // Название файла
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = L"D:"; // Начальная директория
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // Проверка существования пути и файла
}
// Записать в статус программы
void SetWinStatus(string status)
{
    SetWindowTextA(buttons::widgets.hOutputStatus, status.c_str());
}
