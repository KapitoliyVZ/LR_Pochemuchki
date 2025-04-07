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
    if (!RegisterClassW(&MainWindClass))
        return 0;

    // Получение разрешения экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // Само создание с заложенными параметрами
    HWND hMainWnd = CreateWindow(L"MainClass", L"ТРПО лабораторная", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
    static Buttons buttons = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	static bitset<8> ButtonFlags; // Флаги для кнопок
	static string filename = "";
    switch (msg)
    {
    case WM_CREATE:
    {
        MainWndAddMenus(hWnd, buttons);
        MainWndAddWidget(hWnd, buttons);
        SetOpenFileParams(hWnd, filename);
        ButtonFlags.reset();
        buttons.hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Фон окна (Gainsboro)
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons.hBrush);

        // Загрузка изображения
        buttons.hBitmap = (HBITMAP)LoadImageA(NULL, "Icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        buttons.hBitmap2 = (HBITMAP)LoadImageA(NULL, "7Institute.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!buttons.hBitmap || !buttons.hBitmap2)
        {
            MessageBoxA(hWnd, "Не удалось загрузить изображение", "Ошибка", MB_OK | MB_ICONERROR);
        }

        // Создание кистей для кнопки
        buttons.hBrushRed = CreateSolidBrush(RGB(255, 129, 129));
        buttons.hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
        buttons.hBrushGrey = CreateSolidBrush(RGB(178, 178, 178));
        buttons.hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); 
        buttons.hBrushNeutral = CreateSolidBrush(RGB(127, 255, 212));
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
            SetWindowText(buttons.hToggleButton, L"");
            InvalidateRect(buttons.hToggleButton, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hToggleButton);
        }
        else if (LOWORD(wp) == buttons.ButSearchType)
        {
            ButtonFlags.flip(7);
            SetWindowText(buttons.hSearchType, L"");
            InvalidateRect(buttons.hSearchType, NULL, TRUE); // Перерисовываем кнопку
            UpdateWindow(buttons.hSearchType);
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
            char WordToSearch[60] = "";
            GetWindowTextA(buttons.hEditInputWord, WordToSearch, 60);
			string data = WordToSearch;
            ButtonFlags.flip(6);
			// Доделать запуск обработки поиска рифм к введеному слову
        }
        else if (LOWORD(wp) == buttons.OnReadFile)
        {
            
            string filename = "";
            if (GetOpenFileNameW(&OFN)) // Исправлен вызов GetOpenFileNameW для чтения
            {
                filename = ConvertLPWSTRToString(OFN.lpstrFile);
                
            }
        }
        else if (LOWORD(wp) == buttons.OnSaveFile)
        {
            if (GetSaveFileNameW(&OFN))
            {
                
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
        MakeRoundButton(lpDrawItem, buttons, ButtonFlags);
        break;
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
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wp;
        HWND hStatic = (HWND)lp;

        if (hStatic == buttons.hOutputStatus || hStatic == buttons.hOutputStatusText 
            || hStatic == buttons.hOutputStatus ||
            hStatic == buttons.hOutputRhymes || hStatic == buttons.hOutputText)
        {
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // Устанавливаем цвет фона (белый)
            SetTextColor(hdcStatic, RGB(0, 0, 0)); // Устанавливаем цвет текста (черный)
            return (INT_PTR)buttons.hBrush; // Возвращаем кисть для фона
        }
        break;
    }

    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wp;
        HWND hEdit = (HWND)lp;

        if (hEdit == buttons.hEditRhymes || hEdit == buttons.hEditText || hEdit == buttons.hEditInputWord)
        {
            SetBkColor(hdcEdit, RGB(255, 255, 255)); // Устанавливаем цвет фона (белый)
            SetTextColor(hdcEdit, RGB(0, 0, 0)); // Устанавливаем цвет текста (черный)
            return (INT_PTR)buttons.hBrush; // Возвращаем кисть для фона
        }
        break;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lp);
        int height = HIWORD(lp);

        // Изменения размера и позиции кнопок
        MoveWindow(buttons.hClearRhymes, width* initialDimensions[0].x / 1400, height* initialDimensions[0].y / 1050, width* initialDimensions[0].width / 1400, height* initialDimensions[0].height / 1050, TRUE);
        MoveWindow(buttons.hClearText, width* initialDimensions[1].x / 1400, height* initialDimensions[1].y / 1050, width* initialDimensions[1].width / 1400, height* initialDimensions[1].height / 1050, TRUE);
        MoveWindow(buttons.hExitButton, width* initialDimensions[2].x / 1400, height - height * 150 / 1050, width* initialDimensions[2].width / 1400, height* initialDimensions[2].height / 1050, TRUE);
        MoveWindow(buttons.hVerbButton, width* initialDimensions[3].x / 1400, height* initialDimensions[3].y / 1050, width* initialDimensions[3].width / 1400, height* initialDimensions[3].height / 1050, TRUE);
        MoveWindow(buttons.hAdjectiveButton, width* initialDimensions[4].x / 1400, height* initialDimensions[4].y / 1050, width* initialDimensions[4].width / 1400, height* initialDimensions[4].height / 1050, TRUE);
        MoveWindow(buttons.hNounButton, width* initialDimensions[5].x / 1400, height* initialDimensions[5].y / 1050, width* initialDimensions[5].width / 1400, height* initialDimensions[5].height / 1050, TRUE);
        MoveWindow(buttons.hAdverbialButton, width* initialDimensions[6].x / 1400, height* initialDimensions[6].y / 1050, width* initialDimensions[6].width / 1400, height* initialDimensions[6].height / 1050, TRUE);
        MoveWindow(buttons.hParticipleButton, width* initialDimensions[7].x / 1400, height* initialDimensions[7].y / 1050, width* initialDimensions[7].width / 1400, height* initialDimensions[7].height / 1050, TRUE);
        MoveWindow(buttons.hAdverbButton, width* initialDimensions[8].x / 1400, height* initialDimensions[8].y / 1050, width* initialDimensions[8].width / 1400, height* initialDimensions[8].height / 1050, TRUE);
        MoveWindow(buttons.hToggleButton, width* initialDimensions[9].x / 1400, height* initialDimensions[9].y / 1050, width* initialDimensions[9].width / 1400, height* initialDimensions[9].height / 1050, TRUE);
        MoveWindow(buttons.hOutputStatusText, width* initialDimensions[10].x / 1400, height* initialDimensions[10].y / 1050, width* initialDimensions[10].width / 1400, height* initialDimensions[10].height / 1050, TRUE);
        MoveWindow(buttons.hOutputStatus, width* initialDimensions[11].x / 1400, height* initialDimensions[11].y / 1050, width* initialDimensions[11].width / 1400, height* initialDimensions[11].height / 1050, TRUE);
        MoveWindow(buttons.hInputWord, width* initialDimensions[12].x / 1400, height* initialDimensions[12].y / 1050, width* initialDimensions[12].width / 1400, height* initialDimensions[12].height / 1050, TRUE);
        MoveWindow(buttons.hEditInputWord, width* initialDimensions[13].x / 1400, height* initialDimensions[13].y / 1050, width* initialDimensions[13].width / 1400, height* initialDimensions[13].height / 1400, TRUE);
        MoveWindow(buttons.hSearch, width* initialDimensions[14].x / 1400, height* initialDimensions[14].y / 1050, width* initialDimensions[14].width / 1400, height* initialDimensions[14].height / 1050, TRUE);
        MoveWindow(buttons.hOutputRhymes, width* initialDimensions[15].x / 1400, height* initialDimensions[15].y / 1050, width* initialDimensions[15].width / 1400, height* initialDimensions[15].height / 1050, TRUE);
        MoveWindow(buttons.hEditRhymes, width* initialDimensions[16].x / 1400, height* initialDimensions[16].y / 1050, width - width * 315 / 1400, height * 350 / 1050, TRUE);
        MoveWindow(buttons.hOutputText, width* initialDimensions[17].x / 1400, height* initialDimensions[16].y / 1050 + height * 360 / 1050, width* initialDimensions[17].width / 1400, height* initialDimensions[17].height / 1050, TRUE);
        MoveWindow(buttons.hEditText, width* initialDimensions[18].x / 1400, height* initialDimensions[16].y / 1050 + height * 390 / 1050, width - width * 315 / 1400, height * 350 / 1050, TRUE);
        MoveWindow(buttons.hSearchType, width* initialDimensions[19].x / 1400, height* initialDimensions[19].y / 1050, width* initialDimensions[19].width / 1400, height* initialDimensions[19].height / 1050, TRUE);

        // Изменение размера шрифта
        int newFontSize = (width + height) / 160; // Примерная формула для изменения размера шрифта
        HFONT hFont = CreateFont(newFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

        HWND controls[] = {
            buttons.hClearRhymes, buttons.hClearText, buttons.hExitButton, buttons.hVerbButton, buttons.hAdjectiveButton,
            buttons.hNounButton, buttons.hAdverbialButton, buttons.hParticipleButton, buttons.hAdverbButton, buttons.hToggleButton,
            buttons.hOutputStatusText, buttons.hOutputStatus, buttons.hInputWord, buttons.hEditInputWord, buttons.hSearch,
            buttons.hOutputRhymes, buttons.hEditRhymes, buttons.hOutputText, buttons.hEditText, buttons.hSearchType
        };

        for (HWND control : controls) {
            SendMessage(control, WM_SETFONT, (WPARAM)hFont, TRUE);
        }

        // Перерисовываем все элементы управления
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);

        break;
    }
    case WM_DESTROY:
        ExitSoftware();
        DeleteObject(buttons.hBrushRed);
        DeleteObject(buttons.hBrushGreen);
        DeleteObject(buttons.hBrushGrey);
        DeleteObject(buttons.hBrush);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    
  }
  return 0;
}

// Функция для преобразования LPWSTR в std::string
string ConvertLPWSTRToString(LPWSTR lpwstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Функция для преобразования std::string в LPWSTR
LPWSTR ConvertStringToLPWSTR(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    LPWSTR lpwstr = new WCHAR[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, lpwstr, size_needed);
    return lpwstr;
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

BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem, Buttons& buttons, bitset<8>& ButtonFlags)
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
        hBrushes = buttons.hBrushNeutral;
        buttonText = "Выход";
    }
    else if (lpDrawItem->hwndItem == buttons.hToggleButton)
    {
        isActive = ButtonFlags.test(6);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = isActive ? "Вывод на экран включен" : "Вывод на экран выключен";
    }
    else if (lpDrawItem->hwndItem == buttons.hVerbButton)
    {
        isActive = ButtonFlags.test(0);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Глагол";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdverbButton)
    {
        isActive = ButtonFlags.test(1);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Наречие";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdjectiveButton)
    {
        isActive = ButtonFlags.test(2);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Прилагательное";
    }
    else if (lpDrawItem->hwndItem == buttons.hNounButton)
    {
        isActive = ButtonFlags.test(3);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Существительное";
    }
    else if (lpDrawItem->hwndItem == buttons.hParticipleButton)
    {
        isActive = ButtonFlags.test(4);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Причастие";
    }
    else if (lpDrawItem->hwndItem == buttons.hAdverbialButton)
    {
        isActive = ButtonFlags.test(5);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = "Деепричастие";
    }
    else if (lpDrawItem->hwndItem == buttons.hSearchType)
    {
        isActive = ButtonFlags.test(7);
        hBrushes = isActive ? buttons.hBrushGreen : buttons.hBrushGrey;
        buttonText = isActive ? "Режим поиска: однородный" : "Режим поиска: неоднородный";
    }
    else if (lpDrawItem->hwndItem == buttons.hClearRhymes)
    {
        hBrushes = buttons.hBrushNeutral;
        buttonText = "Очистить поле рифм";
    }
    else if (lpDrawItem->hwndItem == buttons.hClearText)
    {
        hBrushes = buttons.hBrushNeutral;
        buttonText = "Очистить поле текста";
    }
    else if (lpDrawItem->hwndItem == buttons.hSearch)
    {
        hBrushes = buttons.hBrushNeutral;
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
    4) и 5) расположение верхнего левого угла виджета в окне
    6) и 7) ширина и высота виджета в окне
    8) главное окно
    */
    // Кнопки приложения

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    buttons.hClearRhymes = CreateWindowA("BUTTON", "Очистить поле рифм", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        295, 20, 150, 40, hWnd, (HMENU)buttons.ClearRhymes, NULL, NULL);
    initialDimensions[0] = { 295, 20, 150, 40 };
    
    buttons.hClearText = CreateWindowA("BUTTON", "Отправить поле текста", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        450, 20, 150, 40, hWnd, (HMENU)buttons.ClearText, NULL, NULL);
    initialDimensions[1] = { 450, 20, 150, 40 };

    buttons.hExitButton = CreateWindowA("BUTTON", "Выход", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, screenHeight - 150, 250, 40, hWnd, (HMENU)buttons.ButExit, NULL, NULL);
    initialDimensions[2] = { 20, screenHeight - 150, 250, 40 };

    buttons.hVerbButton = CreateWindowA("BUTTON", "Глагол", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 82, 250, 40, hWnd, (HMENU)buttons.ButVerb, NULL, NULL);
    initialDimensions[3] = { 20, 82, 250, 40 };

    buttons.hAdjectiveButton = CreateWindowA("BUTTON", "Прилагательное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 130, 250, 40, hWnd, (HMENU)buttons.ButAdjective, NULL, NULL);
    initialDimensions[4] = { 20, 130, 250, 40 };

    buttons.hNounButton = CreateWindowA("BUTTON", "Существительное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 180, 250, 40, hWnd, (HMENU)buttons.ButNoun, NULL, NULL);
    initialDimensions[5] = { 20, 180, 250, 40 };

    buttons.hAdverbialButton = CreateWindowA("BUTTON", "Деепричастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 230, 250, 40, hWnd, (HMENU)buttons.ButAdverbial, NULL, NULL);
    initialDimensions[6] = { 20, 230, 250, 40 };

    buttons.hParticipleButton = CreateWindowA("BUTTON", "Причастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 280, 250, 40, hWnd, (HMENU)buttons.ButParticiple, NULL, NULL);
    initialDimensions[7] = { 20, 280, 250, 40 };

    buttons.hAdverbButton = CreateWindowA("BUTTON", "Наречие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 330, 250, 40, hWnd, (HMENU)buttons.ButAdverb, NULL, NULL);
    initialDimensions[8] = { 20, 330, 250, 40 };

    // Создаем кнопку с флагом BS_OWNERDRAW для кастомной отрисовки
    buttons.hToggleButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        605, 20, 200, 40, hWnd, (HMENU)buttons.OnToggleButtonClicked, NULL, NULL);
    initialDimensions[9] = { 605, 20, 200, 40 };

    // Кнопка однородного поиска
    buttons.hSearchType = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        810, 20, 200, 40, hWnd, (HMENU)buttons.ButSearchType, NULL, NULL);
    initialDimensions[19] = { 810, 20, 200, 40 };

    // Диалоговые окна и их подписи
    buttons.hOutputStatusText = CreateWindowA("static", "Лог выполнения", WS_VISIBLE | WS_CHILD | ES_CENTER,
        20, 385, 250, 30, hWnd, NULL, NULL, NULL);
    initialDimensions[10] = { 20, 385, 250, 30 };

    buttons.hOutputStatus = CreateWindowA("static", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        20, 415, 250, 150, hWnd, NULL, NULL, NULL);
    initialDimensions[11] = { 20, 415, 250, 150 };

    // Поле ввода слова для поиска рифмующихся пар с этим словом
    buttons.hInputWord = CreateWindowA("static", "Введите слово для поиска рифм", WS_VISIBLE | WS_CHILD | ES_CENTER,
        20, 600, 250, 30, hWnd, NULL, NULL, NULL);
    initialDimensions[12] = { 20, 600, 250, 30 };

    buttons.hEditInputWord = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        20, 630, 250, 50, hWnd, NULL, NULL, NULL);
    initialDimensions[13] = { 20, 630, 250, 50 };

    buttons.hSearch = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        20, 690, 250, 40, hWnd, (HMENU)buttons.Search, NULL, NULL);
    initialDimensions[14] = { 20, 690, 250, 40 };

    // Окно передачи данных
    buttons.hOutputRhymes = CreateWindowA("static", "Найденные рифмы", WS_VISIBLE | WS_CHILD | ES_CENTER,
        295, 80, 300, 30, hWnd, NULL, NULL, NULL);
    initialDimensions[15] = { 295, 80, 300, 30 };

    buttons.hEditRhymes = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        295, 110, screenWidth - 315,400, hWnd, NULL, NULL, NULL);
    initialDimensions[16] = { 295, 110, screenWidth - 315, 400 };

    // Окно приема данных
    buttons.hOutputText = CreateWindowA("static", "Текст с найденными рифмами", WS_VISIBLE | WS_CHILD | ES_CENTER,
        295, 620, 300, 30, hWnd, NULL, NULL, NULL);
    initialDimensions[17] = { 295, 620, 300, 30 };

    buttons.hEditText = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
        295, 650, screenWidth - 315, 400, hWnd, NULL, NULL, NULL);
    initialDimensions[18] = { 295, 650, screenWidth - 315, 400 };
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

// Установка начальных параметров на открытие файлов для чтения и записи
void SetOpenFileParams(HWND hWnd, string filename)
{
    LPWSTR path = ConvertStringToLPWSTR(filename);
    ZeroMemory(&OFN, sizeof(OFN));
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = path;
    OFN.nMaxFile = MAX_PATH; // Используем MAX_PATH для корректного размера буфера
    OFN.lpstrFilter = L"*.txt";
    OFN.lpstrFileTitle = NULL;
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = L"D:\\Рабочий стол\\WindowProjectTEst";
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}
// Записать в статус программы
void SetWinStatus(string status, const Buttons& buttons)
{
    SetWindowTextA(buttons.hOutputStatus, status.c_str());
}

