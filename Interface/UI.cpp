#include "UI_const.h"


// Старт приложения и создание стартовых процедур 
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Главный класс для создания параметров приветственного окна
    WNDCLASS MainWindClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
        LoadIcon(NULL, IDI_QUESTION), L"MainClass", SoftwareMainProcedure);


    // Регистрация класса
    //sgmniusijnsb
    if (!RegisterClassW(&MainWindClass))
        return 0;

    // Получение разрешения экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // Само создание с заложенными параметрами
    HWND hMainWnd = CreateWindow(L"MainClass", L"ТРПО лабораторная", WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);

    // Устанавливаем окно поверх всех окон
    SetWindowPos(hMainWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);

    // Основной цикл работы проложения
    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
// Создаем новое окно приложение
WNDCLASS NewWindClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
    WNDCLASS NWC = { 0 };

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
    static Buttons buttons = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
        false,false,false,false,false,false,false };
    switch (msg)
    {
    case WM_CREATE:
    {
        MainWndAddMenus(hWnd, buttons);
        MainWndAddWidget(hWnd, buttons);
        SetOpenFileParams(hWnd);
        ButtonFlags.reset();
        buttons.hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Фон окна (Gainsboro)
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons.hBrush);

        // Загрузка изображения
		// картинки теперь у всех загружаются
        buttons.hBitmap = (HBITMAP)LoadImage(NULL, L"Icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        buttons.hBitmap2 = (HBITMAP)LoadImage(NULL, L"7Institute.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!buttons.hBitmap || !buttons.hBitmap2)
        {
            MessageBox(hWnd, L"Не удалось загрузить изображение", L"Ошибка", MB_OK | MB_ICONERROR);
        }

        // Создание кистей для кнопки
        buttons.hBrushRed = CreateSolidBrush(RGB(255, 129, 129));
        buttons.hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
        buttons.hBrushBlue = CreateSolidBrush(RGB(178, 178, 178));
        buttons.hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wp) == buttons.ButExit)
        {
            SetWindowText(buttons.hExitButton, L"");
            InvalidateRect(buttons.hExitButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hExitButton);
            PostQuitMessage(0);
        }
        else if (LOWORD(wp) == buttons.OnToggleButtonClicked)
        {
            ButtonFlags.flip(6);
            SetWindowText(buttons.hToggleButton, L"");
            InvalidateRect(buttons.hToggleButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hToggleButton);
        }
        else if (LOWORD(wp) == buttons.ButVerb)
        {
            ButtonFlags.flip(0);
            SetWindowText(buttons.hVerbButton, L"");
            InvalidateRect(buttons.hVerbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hVerbButton);
        }
        else if (LOWORD(wp) == buttons.ButAdverb)
        {
            ButtonFlags.flip(1);
            SetWindowText(buttons.hAdverbButton, L"");
            InvalidateRect(buttons.hAdverbButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hAdverbButton);
        }
        else if (LOWORD(wp) == buttons.ButAdjective)
        {
            ButtonFlags.flip(2);
            SetWindowText(buttons.hAdjectiveButton, L"");
            InvalidateRect(buttons.hAdjectiveButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hAdjectiveButton);
        }
        else if (LOWORD(wp) == buttons.ButNoun)
        {
            ButtonFlags.flip(3);
            SetWindowText(buttons.hNounButton, L"");
            InvalidateRect(buttons.hNounButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hNounButton);
        }
        else if (LOWORD(wp) == buttons.ButParticiple)
        {
            ButtonFlags.flip(4);
            SetWindowText(buttons.hParticipleButton, L"");
            InvalidateRect(buttons.hParticipleButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hParticipleButton);
        }
        else if (LOWORD(wp) == buttons.ButAdverbial)
        {
            ButtonFlags.flip(5);
            SetWindowText(buttons.hAdverbialButton, L"");
            InvalidateRect(buttons.hAdverbialButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hAdverbialButton);
        }
        else if (LOWORD(wp) == buttons.OnExitSoftware)
        {
            PostQuitMessage(0);
        }
        else if (LOWORD(wp) == buttons.ClearRhymes)
        {
            SetWindowText(buttons.hEditRhymes, L"");
            InvalidateRect(buttons.hEditRhymes, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hEditRhymes);
        }
        else if (LOWORD(wp) == buttons.ClearText)
        {
            SetWindowText(buttons.hEditText, L"");
            InvalidateRect(buttons.hEditText, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hEditText);
        }
        else if (LOWORD(wp) == buttons.Search)
        {
            // Доделать
        }
        else if (LOWORD(wp) == buttons.OnReadFile)
        {
            if (GetOpenFileNameA(&OFN)) // Исправлен вызов GetOpenFileNameA для чтения
            {
                read_data(filename);
            }
        }
        else if (LOWORD(wp) == buttons.OnSaveFile)
        {
            if (GetSaveFileNameA(&OFN))
            {
                save_data(filename, buttons);
            }
        }
        else if (LOWORD(wp) == buttons.OnInfoClicked)
        {
            MessageBox(hWnd, L"Информация о программе и ее разработчиках:", L"Инфо", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
        MakeRoundButton(lpDrawItem, buttons);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wp;
        SetBkColor(hdcStatic, RGB(255, 255, 255)); // Устанавливаем цвет фона
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // Устанавливаем цвет текста
        return (INT_PTR)buttons.hBrush;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        BITMAP bitmap;
        BITMAP bitmap2;
        // Отображение первого изображения
        if (buttons.hBitmap)
        {
            buttons.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons.hdcMem, buttons.hBitmap);


            GetObject(buttons.hBitmap, sizeof(BITMAP), &bitmap);
            BitBlt(hdc, 20, 0, bitmap.bmWidth, bitmap.bmHeight, buttons.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons.hdcMem, hOldBitmap);
            DeleteDC(buttons.hdcMem);
        }

        // Отображение второго изображения
        if (buttons.hBitmap2)
        {
            buttons.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons.hdcMem, buttons.hBitmap2);


            GetObject(buttons.hBitmap2, sizeof(BITMAP), &bitmap2);
            BitBlt(hdc, bitmap.bmWidth + 20, 0, bitmap2.bmWidth, bitmap2.bmHeight, buttons.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons.hdcMem, hOldBitmap);
            DeleteDC(buttons.hdcMem);
        }

        // Рисуем черную рамку вокруг статических полей

        HPEN hOldPen = (HPEN)SelectObject(hdc, buttons.hPenBlack);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        HWND All_hwnd[8] = { buttons.hOutputStatus ,buttons.hOutputStatusText ,buttons.hEditRhymes ,
            buttons.hOutputRhymes ,buttons.hOutputText ,buttons.hEditText, buttons.hInputWord, buttons.hEditInputWord };

        for (int i = 0; i < 8; i++)
        {
            MakeFrame(hWnd, hdc, All_hwnd[i], buttons);
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);



        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        ExitSoftware();
        DeleteObject(buttons.hBrushRed);
        DeleteObject(buttons.hBrushGreen);
        DeleteObject(buttons.hBrushBlue);
        DeleteObject(buttons.hBrush);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    
  }
  return 0;
}

void MakeFrame(HWND hWnd, HDC hdc, HWND Edit, Buttons buttons)
{
    RECT rect;
    GetWindowRect(Edit, &rect);
    MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);

    // Рисуем черную рамку вокруг элемента управления
    HPEN hOldPen = (HPEN)SelectObject(hdc, buttons.hPenBlack);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
}

BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem, Buttons& buttons)
{
    HDC hdc = lpDrawItem->hDC;  // Используем локальную переменную HDC
    RECT rect = lpDrawItem->rcItem;

    // Создаем кисть цвета фона окна (чтобы скрыть старые границы)
    HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255)); // Цвет фона окна
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush); // Удаляем кисть после использования

    // Выбираем нужную кисть для кнопки
    HBRUSH hBrushes;
    BOOL isActive = FALSE;
    const char* buttonText;
    if (lpDrawItem->hwndItem == buttons.hExitButton)
    {
        hBrushes = buttons.hBrushBlue;
        buttonText = "Выход";
    }
    else if (lpDrawItem->hwndItem == buttons.hToggleButton)
    {
        isActive = ButtonFlags.test(6);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = isActive ? "Вывод на экран включен" : "Вывод на экран выключен";
    }
    else if (lpDrawItem->hwndItem == buttons.hVerbButton)
    {
        isActive = ButtonFlags.test(0);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Глагол";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdverbButton)
    {
        isActive = ButtonFlags.test(1);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Наречие";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdjectiveButton)
    {
        isActive = ButtonFlags.test(2);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Прилагательное";
    }
    else if (lpDrawItem->hwndItem == buttons.hNounButton)
    {
        isActive = ButtonFlags.test(3);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Существительное";
    }
    else if (lpDrawItem->hwndItem == buttons.hParticipleButton)
    {
        isActive = ButtonFlags.test(4);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Причастие";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdverbialButton)
    {
        isActive = ButtonFlags.test(5);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushRed;
        buttonText = "Деепричастие";
    }
    else if (lpDrawItem->hwndItem == buttons.hClearRhymes)
    {
        hBrushes = buttons.hBrushBlue;
        buttonText = "Очистить поле рифм";
    }
    else if (lpDrawItem->hwndItem == buttons.hClearText)
    {
        hBrushes = buttons.hBrushBlue;
        buttonText = "Очистить поле текста";
    }
    else if (lpDrawItem->hwndItem == buttons.hSearch)
    {
        hBrushes = buttons.hBrushBlue;
        buttonText = "Поиск";
    }
    else
    {
        return FALSE;  // Неизвестная кнопка
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;

    if (lpDrawItem->hwndItem == buttons.hExitButton ||
        lpDrawItem->hwndItem == buttons.hClearRhymes ||
        lpDrawItem->hwndItem == buttons.hClearText ||
        lpDrawItem->hwndItem == buttons.hSearch)
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

void SetEditBackgroundColor(HWND hEdit, COLORREF color, Buttons& buttons) {
    if (buttons.hEditBackgroundBrush) {
        DeleteObject(buttons.hEditBackgroundBrush); // Удаляем старую кисть, если она существует
    }
    buttons.hEditBackgroundBrush = CreateSolidBrush(color); // Создаем новую кисть с нужным цветом
    InvalidateRect(hEdit, NULL, TRUE); // Перерисовываем Edit
    UpdateWindow(hEdit); // Обновляем окно
}

// Завершение работы приложения
void ExitSoftware()
{
    PostQuitMessage(0);
}

// Создание пунктов меню
void MainWndAddMenus(HWND hWnd, const Buttons& buttons)
{
    HMENU RootMenu = CreateMenu();
    HMENU SubMenu = CreateMenu();



    // Создание основного меню 
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Файл");
    AppendMenu(RootMenu, MF_STRING, buttons.OnReadFile, L"Чтение файла");
    AppendMenu(RootMenu, MF_STRING, buttons.OnSaveFile, L"Запись файла");


    // Создание подменю Файл
    AppendMenu(SubMenu, MF_STRING, buttons.OnInfoClicked, L"Инфо");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, buttons.OnExitSoftware, L"Завершить работу");


    SetMenu(hWnd, RootMenu);
}

// Создание виджетов
void MainWndAddWidget(HWND hWnd, Buttons& buttons)
{
    /* Создание окна 1) класс окна
    2) текст
    3) стиль окна
    4) и 5) расположение верхнего левого угла  виджета в окне
    6) и 7) ширина и высота виджета в окне
    8) главное окно
    */
    // Кнопки приложения

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    buttons.hClearRhymes = CreateWindowA("BUTTON", "Очистить поле рифм", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        195, 20, 150, 40, hWnd, (HMENU)buttons.ClearRhymes, NULL, NULL);
    buttons.hClearText = CreateWindowA("BUTTON", "Отправить поле текста", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        350, 20, 150, 40, hWnd, (HMENU)buttons.ClearText, NULL, NULL);
    buttons.hSearch = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        970, 20, 150, 40, hWnd, (HMENU)buttons.Search, NULL, NULL);

    buttons.hExitButton = CreateWindowA("BUTTON", "Выход", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, screenHeight - 150, 154, 34, hWnd, (HMENU)buttons.ButExit, NULL, NULL);

    buttons.hVerbButton = CreateWindowA("BUTTON", "Глагол", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 82, 150, 30, hWnd, (HMENU)buttons.ButVerb, NULL, NULL);
    buttons.hAdjectiveButton = CreateWindowA("BUTTON", "Прилагательное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 130, 150, 30, hWnd, (HMENU)buttons.ButAdjective, NULL, NULL);
    buttons.hNounButton = CreateWindowA("BUTTON", "Существительное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 180, 150, 30, hWnd, (HMENU)buttons.ButNoun, NULL, NULL);
    buttons.hAdverbialButton = CreateWindowA("BUTTON", "Деепричастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 230, 150, 30, hWnd, (HMENU)buttons.ButAdverbial, NULL, NULL);
    buttons.hParticipleButton = CreateWindowA("BUTTON", "Причастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 280, 150, 30, hWnd, (HMENU)buttons.ButParticiple, NULL, NULL);
    buttons.hAdverbButton = CreateWindowA("BUTTON", "Наречие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 330, 150, 30, hWnd, (HMENU)buttons.ButAdverb, NULL, NULL);

    // Создаем кнопку с флагом BS_OWNERDRAW для кастомной отрисовки
    buttons.hToggleButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        505, 20, 200, 40, hWnd, (HMENU)buttons.OnToggleButtonClicked, NULL, NULL);

    // Диалоговые окна и их подписи
    buttons.hOutputStatusText = CreateWindowA("static", "Лог выполнения", WS_VISIBLE | WS_CHILD | ES_CENTER,
        20, 370, 150, 30, hWnd, NULL, NULL, NULL);
    buttons.hOutputStatus = CreateWindowA("static", "Первый запуск", WS_VISIBLE | WS_CHILD | ES_LEFT,
        20, 400, 150, screenHeight - 150 - 410, hWnd, NULL, NULL, NULL);


    // Окно передачи данных
    buttons.hOutputRhymes = CreateWindowA("static", "Найденные рифмы", WS_VISIBLE | WS_CHILD | ES_CENTER,
        195, 80, 150, 15, hWnd, NULL, NULL, NULL);
    buttons.hEditRhymes = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        195, 96, screenWidth - 215, screenHeight / 2 - 100, hWnd, NULL, NULL, NULL);

    // Окно приема данных
    buttons.hOutputText = CreateWindowA("static", "Текст с найденными рифмами", WS_VISIBLE | WS_CHILD | ES_CENTER,
        195, screenHeight / 2 + 1, 150, 15, hWnd, NULL, NULL, NULL);
    buttons.hEditText = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        195, screenHeight / 2 + 17, screenWidth - 215, screenHeight / 2 - 100, hWnd, NULL, NULL, NULL);

    buttons.hInputWord = CreateWindowA("static", "Введите слово для поиска рифм", WS_VISIBLE | WS_CHILD | ES_LEFT,
        750, 19, 200, 20, hWnd, NULL, NULL, NULL);
    buttons.hEditInputWord = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        750, 36, 200, 30, hWnd, NULL, NULL, NULL);
}

// Запись в файл
void save_data(LPCSTR path, Buttons& buttons)
{
    HANDLE SAVEfile = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    int SAVElength = GetWindowTextLength(buttons.hSearch) + 1;
    char* data = new char[SAVElength];

    SAVElength = GetWindowTextA(buttons.hSearch, data, SAVElength);

    DWORD bytesIterated;

    WriteFile(SAVEfile, data, SAVElength, &bytesIterated, NULL);
    CloseHandle(SAVEfile);
    delete[] data;
}


// Чтение из файла
void read_data(LPCSTR path)
{
    HANDLE READfile = CreateFileA(
        path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    DWORD bytesIterated;

    ReadFile(READfile, BufferReceive, TextBufferSize, &bytesIterated, NULL);

    CloseHandle(READfile);
}

// Установка начальных параметров на открытие файлов для чтения и записи
void SetOpenFileParams(HWND hWnd)
{
    ZeroMemory(&OFN, sizeof(OFN));
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = filename;
    OFN.nMaxFile = sizeof(filename);
    OFN.lpstrFilter = "*.txt";
    OFN.lpstrFileTitle = NULL;
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = "D:\\Рабочий стол\\WindowProjectTEst";
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

// Записать в статус программы
void SetWinStatus(string status, const Buttons& buttons)
{
    SetWindowTextA(buttons.hOutputStatus, status.c_str());
}

