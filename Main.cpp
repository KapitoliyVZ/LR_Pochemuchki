#include "Header.h"


// Старт приложения и создание стартовых процедур 
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	hInstGlobal = hInst;

	// Главный класс для создания параметров приветственного окна
	WNDCLASS WelcomeClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"WelcomeClass", WelcomeProcedure);

	// Регистрация класса 
	if (!RegisterClassW(&WelcomeClass))
		return -1;

	// Получение разрешения экрана
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

	// Само создание с заложенными параметрами
	hWelcomeWnd = CreateWindow(L"WelcomeClass", L"ТРПО лабораторная", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);

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

LRESULT CALLBACK WelcomeProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(L"button", L"Вариант 1", WS_VISIBLE | WS_CHILD, 50, 50, 100, 30, hWnd, (HMENU)1, NULL, NULL);
		CreateWindow(L"button", L"Вариант 2", WS_VISIBLE | WS_CHILD, 50, 100, 100, 30, hWnd, (HMENU)2, NULL, NULL);
		CreateWindow(L"BUTTON", L"Выход", WS_VISIBLE | WS_CHILD, 20, 150, 150, 30, hWnd, (HMENU)3, NULL, NULL);

		break;

	case WM_COMMAND:
		if (LOWORD(wp) == 1 || LOWORD(wp) == 2)
		{
			ShowWindow(hWnd, SW_HIDE);
			if (!hMainWnd)
			{
				CreateMainWindow(hInstGlobal);
			}
			else
			{
				ShowWindow(hMainWnd, SW_SHOW);
			}
		}
		else if (LOWORD(wp) == 3)
		{
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

// Создание основного окна
void CreateMainWindow(HINSTANCE hInst)
{
	// Главный класс для создания параметров основного окна
	WNDCLASS MainWindClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"MainClass", SoftwareMainProcedure);

	// Регистрация класса 
	if (!RegisterClassW(&MainWindClass))
		return;

	// Получение разрешения экрана
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

	// Передаваемое сообщение
	MSG SoftawareMainMessage = { 0 };

	// Само создание с заложенными параметрами
	hMainWnd = CreateWindow(L"MainClass", L"ТРПО лабораторная", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);
	
	

}

// Основной цикл программы
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		MainWndAddMenus(hWnd);
		MainWndAddWidget(hWnd);
		SetOpenFileParams(hWnd);
		ButtonFlags.reset();
		hBrush = CreateSolidBrush(RGB(220, 220, 220)); // Фон окна (Gainsboro)
		SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);

		// Создание кистей для кнопки
		hBrushRed = CreateSolidBrush(RGB(255, 129, 129));
		hBrushGreen = CreateSolidBrush(RGB(129, 255, 129));
		hBrushBlue = CreateSolidBrush(RGB(178, 178, 178));
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wp) == ButBack)
		{
			SetWindowTextA(hBackButton, "");
			InvalidateRect(hBackButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hBackButton);

			ShowWindow(hMainWnd, SW_HIDE);
			ShowWindow(hWelcomeWnd, SW_SHOW);
		}
		else if (LOWORD(wp) == OnToggleButtonClicked)
		{
			ButtonFlags.flip(6);
			SetWindowTextA(hToggleButton, "");
			InvalidateRect(hToggleButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hToggleButton);
		}
		else if (LOWORD(wp) == ButVerb)
		{
			ButtonFlags.flip(0);
			SetWindowTextA(hVerbButton, "");
			InvalidateRect(hVerbButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hVerbButton);
		}
		else if (LOWORD(wp) == ButAdverb)
		{
			ButtonFlags.flip(1);
			SetWindowTextA(hAdverbButton, "");
			InvalidateRect(hAdverbButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hAdverbButton);
		}
		else if (LOWORD(wp) == ButAdjective)
		{
			ButtonFlags.flip(2);
			SetWindowTextA(hAdjectiveButton, "");
			InvalidateRect(hAdjectiveButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hAdjectiveButton);
		}
		else if (LOWORD(wp) == ButNoun)
		{
			ButtonFlags.flip(3);
			SetWindowTextA(hNounButton, "");
			InvalidateRect(hNounButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hNounButton);
		}
		else if (LOWORD(wp) == ButParticiple)
		{
			ButtonFlags.flip(4);
			SetWindowTextA(hParticipleButton, "");
			InvalidateRect(hParticipleButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hParticipleButton);
		}
		else if (LOWORD(wp) == ButAdverbial)
		{
			ButtonFlags.flip(5);
			SetWindowTextA(hAdverbialButton, "");
			InvalidateRect(hAdverbialButton, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hAdverbialButton);
		}
		else if (LOWORD(wp) == OnExitSoftware)
		{
			PostQuitMessage(0);
		}
		else if (LOWORD(wp) == ClearRhymes)
		{
			SetWindowTextA(hEditRhymes, "");
			InvalidateRect(hEditRhymes, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hEditRhymes);
		}
		else if (LOWORD(wp) == ClearText)
		{
			SetWindowTextA(hEditText, "");
			InvalidateRect(hEditText, NULL, TRUE); // Перерисовываем кнопку
			UpdateWindow(hEditText);
		}
		else if (LOWORD(wp) == EnterWord)
		{
			// Доделать
		}
		else if (LOWORD(wp) == OnReadFile)
		{
			if (GetOpenFileNameA(&OFN)) // Исправлен вызов GetOpenFileNameA для чтения
			{
				read_data(filename);
			}
		}
		else if (LOWORD(wp) == OnSaveFile)
		{
			if (GetSaveFileNameA(&OFN))
			{
				save_data(filename);
			}
		}
		else if (LOWORD(wp) == OnInfoClicked)
		{
			MessageBox(hWnd, L"Информация о программе и ее разработчиках:", L"Инфо", MB_OK | MB_ICONINFORMATION);
		}
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wp;
		SetBkColor(hdcStatic, RGB(220, 220, 220)); // Устанавливаем цвет фона
		SetTextColor(hdcStatic, RGB(0, 0, 0)); // Устанавливаем цвет текста
		return (INT_PTR)hBrush;
	}
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
		MakeRoundButton(lpDrawItem);
		break;
	}
	case WM_DESTROY:
		ExitSoftware();
		DeleteObject(hBrushRed);
		DeleteObject(hBrushGreen);
		DeleteObject(hBrushBlue);
		DeleteObject(hBrush);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem)
{
	HDC hdc = lpDrawItem->hDC;  // Используем локальную переменную HDC
	RECT rect = lpDrawItem->rcItem;

	// Создаем кисть цвета фона окна (чтобы скрыть старые границы)
	HBRUSH hBgBrush = CreateSolidBrush(RGB(220, 220, 220)); // Цвет фона окна
	FillRect(hdc, &rect, hBgBrush);
	DeleteObject(hBgBrush); // Удаляем кисть после использования

	// Выбираем нужную кисть для кнопки
	HBRUSH hBrush;
	BOOL isActive = FALSE;
	const char* buttonText;
	if (lpDrawItem->hwndItem == hBackButton)
	{
		hBrush = hBrushBlue;
		buttonText = "Назад";
	}
	else if (lpDrawItem->hwndItem == hToggleButton)
	{
		isActive = ButtonFlags.test(6);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = isActive ? "Вывод на экран включен" : "Вывод на экран выключен";
	}
	else if (lpDrawItem->hwndItem == hVerbButton)
	{
		isActive = ButtonFlags.test(0);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Глагол";
	}
	else if (lpDrawItem->hwndItem == hAdverbButton)
	{
		isActive = ButtonFlags.test(1);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Наречие";
	}
	else if (lpDrawItem->hwndItem == hAdjectiveButton)
	{
		isActive = ButtonFlags.test(2);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Прилагательное";
	}
	else if (lpDrawItem->hwndItem == hNounButton)
	{
		isActive = ButtonFlags.test(3);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Существительное";
	}
	else if (lpDrawItem->hwndItem == hParticipleButton)
	{
		isActive = ButtonFlags.test(4);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Причастие";
	}
	else if (lpDrawItem->hwndItem == hAdverbialButton)
	{
		isActive = ButtonFlags.test(5);
		hBrush = isActive ? hBrushGreen : hBrushRed;
		buttonText = "Деепричастие";
	}
	else if (lpDrawItem->hwndItem == hClearRhymes)
	{
		hBrush = hBrushBlue;
		buttonText = "Очистить поле рифм";
	}
	else if (lpDrawItem->hwndItem == hClearText)
	{
		hBrush = hBrushBlue;
		buttonText = "Очистить поле текста";
	}

	else
	{
		return FALSE;  // Неизвестная кнопка
	}

	HPEN hPen = NULL;
	HPEN hOldPen = NULL;
	HBRUSH hOldBrush = NULL;

	if (lpDrawItem->hwndItem == hBackButton || lpDrawItem->hwndItem == hClearRhymes || lpDrawItem->hwndItem == hClearText)
	{
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // Синее обрамление для голубой кнопки
	}
	else
	{
		hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
	}
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
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
void SetEditBackgroundColor(HWND hEdit, COLORREF color) {
	if (hEditBackgroundBrush) {
		DeleteObject(hEditBackgroundBrush); // Удаляем старую кисть, если она существует
	}
	hEditBackgroundBrush = CreateSolidBrush(color); // Создаем новую кисть с нужным цветом
	InvalidateRect(hEdit, NULL, TRUE); // Перерисовываем Edit
	UpdateWindow(hEdit); // Обновляем окно
}

// Завершение работы приложения
void ExitSoftware()
{
	IsConnected = false;
	PostQuitMessage(0);
}

// Создание пунктов меню
void MainWndAddMenus(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();



	// Создание основного меню 
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Файл");
	AppendMenu(RootMenu, MF_STRING, OnReadFile, L"Чтение файла");
	AppendMenu(RootMenu, MF_STRING, OnSaveFile, L"Запись файла");
	

	// Создание подменю Файл
	AppendMenu(SubMenu, MF_STRING, OnInfoClicked, L"Инфо");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Завершить работу");


	SetMenu(hWnd, RootMenu);
}

// Создание виджетов
void MainWndAddWidget(HWND hWnd)
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

	hClearRhymes = CreateWindowA("BUTTON", "Очистить поле рифм", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 195, 20, 150, 40, hWnd, (HMENU)ClearRhymes, NULL, NULL);
	hClearText = CreateWindowA("BUTTON", "Отправить поле текста", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 350, 20, 150, 40, hWnd, (HMENU)ClearText, NULL, NULL);
	//hEditWord = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 20, 30, 200, 30, hWnd, (HMENU)EnterWord, NULL, NULL);

	hBackButton = CreateWindowA("BUTTON", "Назад", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, screenHeight-150, 154, 34, hWnd, (HMENU)ButBack, NULL, NULL);

	hVerbButton = CreateWindowA("BUTTON", "Глагол", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 82, 150, 30, hWnd, (HMENU)ButVerb, NULL, NULL);
	hAdjectiveButton = CreateWindowA("BUTTON", "Прилагательное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 130, 150, 30, hWnd, (HMENU)ButAdjective, NULL, NULL);
	hNounButton = CreateWindowA("BUTTON", "Существительное", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 180, 150, 30, hWnd, (HMENU)ButNoun, NULL, NULL);
	hAdverbialButton = CreateWindowA("BUTTON", "Деепричастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 230, 150, 30, hWnd, (HMENU)ButAdverbial, NULL, NULL);
	hParticipleButton = CreateWindowA("BUTTON", "Причастие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 280, 150, 30, hWnd, (HMENU)ButParticiple, NULL, NULL);
	hAdverbButton = CreateWindowA("BUTTON", "Наречие", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 330, 150, 30, hWnd, (HMENU)ButAdverb, NULL, NULL);
	

	// Создаем кнопку с флагом BS_OWNERDRAW для кастомной отрисовки
	hToggleButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 505, 20, 200, 40, hWnd, (HMENU)OnToggleButtonClicked, NULL, NULL);
	
	// Диалоговые окна и их подписи
	hOutputStatusText = CreateWindowA("static", "Статус выполнения: ", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 5, 150, 30, hWnd, NULL, NULL, NULL);
	hOutputStatus = CreateWindowA("static", "Первый запуск", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 30, 150, 30, hWnd, NULL, NULL, NULL);

	
	// Окно передачи данных
	hOutputRhymes = CreateWindowA("static", "Найденные рифмы", WS_VISIBLE | WS_CHILD | ES_CENTER, 195, 80, screenWidth - 215, 15, hWnd, NULL, NULL, NULL);
	hEditRhymes = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 195, 100, screenWidth - 215, screenHeight /2 - 100, hWnd, NULL, NULL, NULL);
	 
	// Окно приема данных
	hOutputText = CreateWindowA("static", "Текст с найденными рифмами", WS_VISIBLE | WS_CHILD | ES_CENTER, 195, screenHeight / 2 + 10, screenWidth - 215, 15, hWnd, NULL, NULL, NULL);
	hEditText = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 195, screenHeight / 2 + 30, screenWidth - 215, screenHeight / 2 - 100, hWnd, NULL, NULL, NULL);
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
		NULL
	);
	int SAVElength = GetWindowTextLength(hEditWord) + 1;
	char* data = new char[SAVElength];
	
	SAVElength = GetWindowTextA(hEditWord, data, SAVElength);

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
void SetWinStatus(string status)
{
	SetWindowTextA(hOutputStatus, status.c_str());
}