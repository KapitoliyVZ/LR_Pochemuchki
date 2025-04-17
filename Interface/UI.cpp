#include "UI_const.h"
#include "Heart_of_program.h"
#include "file_working.h"
#include "NLP.h"



// ����� ���������� � �������� ��������� ��������
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    /*
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("en_US.UTF-8"));
    */

    // ������� ����� ��� �������� ���������� ��������������� ����
    WNDCLASS MainWindClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
                                          LoadIcon(NULL, IDI_QUESTION), L"MainClass", SoftwareMainProcedure);

    // ����������� ������
    if (!RegisterClassW(&MainWindClass))
        return 0;

    // ��������� ���������� ������
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // ���� �������� � ����������� �����������
    HWND hMainWnd = CreateWindow(L"MainClass", L"���� ������������", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);
    if (!hMainWnd) {
        MessageBox(NULL, L"�� ������� ������� ������� ����", L"������", MB_OK | MB_ICONERROR);
        return 0;
    }
    // ������������� ���� ������ ���� ����
    SetWindowPos(hMainWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
    
    // �������� ���� ������ ����������
    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);

        // ���������, ��� ���� ������������� ����������, ���� hwnd �� NULL
        if (msg.hwnd != NULL && !IsWindow(msg.hwnd)) {
            MessageBox(NULL, L"���������������� ���� � ���������", L"������", MB_OK | MB_ICONERROR);
            break; // ��������� ����, ���� ���� ���������������
        }

        DispatchMessage(&msg);
    }

    return 0;
}
// ������� ����� ���� ����������
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

// �������� ���� ���������
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
        buttons::graphics.hBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��� ���� (Gainsboro)
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)buttons::graphics.hBrush);

        // �������� �����������
        buttons::graphics.hBitmap = (HBITMAP)LoadImageA(NULL, "Icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        buttons::graphics.hBitmap2 = (HBITMAP)LoadImageA(NULL, "7Institute.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!buttons::graphics.hBitmap || !buttons::graphics.hBitmap2)
        {
            MessageBoxA(hWnd, "�� ������� ��������� �����������", "������", MB_OK | MB_ICONERROR);
        }

        // �������� ������ ��� ������
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
        // ���������, ���� ����������� ������ �� ���� ��������������
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == buttons::widgets.hEditInputWord)
        {
            // �������� ����� �� ���� ��������������
            char buffer[256] = { 0 };
            GetWindowTextA(buttons::widgets.hEditInputWord, buffer, sizeof(buffer));

            // ���� ���� �� ������, ��������� ������
            if (strlen(buffer) > 0)
            {
                EnableWindow(buttons::widgets.hVerbButton, FALSE);
                EnableWindow(buttons::widgets.hAdverbButton, FALSE);
                EnableWindow(buttons::widgets.hAdjectiveButton, FALSE);
                EnableWindow(buttons::widgets.hNounButton, FALSE);
                EnableWindow(buttons::widgets.hParticipleButton, FALSE);
                EnableWindow(buttons::widgets.hAdverbialButton, FALSE);
            }
            else // ���� ���� ������, ������������ ������
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
            InvalidateRect(buttons::widgets.hExitButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hExitButton);
            ExitSoftware();
        }

        else if (LOWORD(wp) == buttons::buttonIDs.ButSearchType)
        {
            buttons::ButtonFlags.flip(7);
            SetWindowText(buttons::widgets.hSearchType, L"");
            InvalidateRect(buttons::widgets.hSearchType, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hSearchType);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButVerb)
        {
            buttons::ButtonFlags.flip(0);
            SetWindowText(buttons::widgets.hVerbButton, L"");
            InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hVerbButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverb)
        {
            buttons::ButtonFlags.flip(1);
            SetWindowText(buttons::widgets.hAdverbButton, L"");
            InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdverbButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdjective)
        {
            buttons::ButtonFlags.flip(2);
            SetWindowText(buttons::widgets.hAdjectiveButton, L"");
            InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdjectiveButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButNoun)
        {
            buttons::ButtonFlags.flip(3);
            SetWindowText(buttons::widgets.hNounButton, L"");
            InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hNounButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButParticiple)
        {
            buttons::ButtonFlags.flip(4);
            SetWindowText(buttons::widgets.hParticipleButton, L"");
            InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hParticipleButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverbial)
        {
            buttons::ButtonFlags.flip(5);
            SetWindowText(buttons::widgets.hAdverbialButton, L"");
            InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdverbialButton);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.OnExitSoftware)
        {
            ExitSoftware();
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ClearRhymes)
        {
            SetWindowText(buttons::widgets.hEditRhymes, L"");
            InvalidateRect(buttons::widgets.hEditRhymes, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hEditRhymes);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.ClearText)
        {
            SetWindowText(buttons::widgets.hEditText, L"");
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hEditText);
        }
        else if (LOWORD(wp) == buttons::buttonIDs.Search)
        {
			buttons::ButtonFlags[6] = 0; // ����� ����� ������ �����
            char WordToSearch[60] = ""; 
            GetWindowTextA(buttons::widgets.hEditInputWord, WordToSearch, 60);
			string word_to_compare = WordToSearch; // ����� ��� ������ �� ���� ����
			if (word_to_compare.length() != 0)
			{
                buttons::ButtonFlags.flip(6);
			}
            
            if (buttons::ButtonFlags.test(0) == 0 and buttons::ButtonFlags.test(1) == 0 and
                buttons::ButtonFlags.test(2) == 0 and buttons::ButtonFlags.test(3) == 0 and
                buttons::ButtonFlags.test(4) == 0 and buttons::ButtonFlags.test(5) == 0 and
                buttons::ButtonFlags.test(6) == 0)
            {
                MessageBoxA(hWnd, "�������� ����� ���� ��� ������� ����� ��� ������", "������", MB_OK | MB_ICONERROR);
                break;
            }
            if (buttons::ButtonFlags.count() < 2 and
                buttons::ButtonFlags.test(7) == 0)
            {
                MessageBoxA(hWnd, "�������� ��������� ������ ���� ��� ������� ����� ��� ������", "������", MB_OK | MB_ICONERROR);
                break;
            }
            /*if (filename.length() == 0)
            {
                MessageBoxA(hWnd, "�������� ���� � ������� ��� ������ ����", "������", MB_OK | MB_ICONERROR);
                break;
            }*/


            vector<WordData> rhymes_data;     // ��������� ����� � ������� �����
            vector<vector<string>> sentences; // ����� � ������������ �������������

			// �������� ��������� �����, ����������� ����������� � ����� 
            unite_functions(rhymes_data, sentences, word_to_compare, buttons::ButtonFlags);


            // ������� ���������� ���� ����� ����������� ������ ������
            SetWindowTextA(buttons::widgets.hEditRhymes, "");
            SetWindowTextA(buttons::widgets.hEditText, "");

            // �������� �� ���� ������
            if (rhymes_data.empty())
            {
                MessageBoxA(hWnd, "�� ������� ����", "������", MB_OK | MB_ICONERROR);
                break;
            }
            int counter = 0;
            for (WordData& output : rhymes_data)
            {
                counter++;
                string wordInfo;
                if (counter == 1)
                {
                    wordInfo = "�����: ";
                }
                else
                {
					wordInfo = "\r\n�����: ";
                }

                output.word = utf8_to_ansi(output.word);
                
                wordInfo += output.word;

                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)wordInfo.c_str());

                // ���� ���� �����, ��������� ��
                if (!output.rhymed_words.empty())
                {
                    SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)"\r\n�����: ");
                    for (string& word : output.rhymed_words)
                    {
                        word = utf8_to_ansi(word);
                        string rhyme = "\r\n  - " + word;
                        SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)rhyme.c_str());
                    }
                }

                // ��������� ����������� ����� �������
                SendMessageA(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)"\n");
            }

            // �������������� ���� ������
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE);
            UpdateWindow(buttons::widgets.hEditText);
            
        }
        // ������� ������ "������ �����"
        
        else if (LOWORD(wp) == buttons::buttonIDs.OnReadFile)
        {
            wchar_t filePath[MAX_PATH] = {}; // ����������� �������� �����
            
            
            ////////////////////////////
            // ����� ��� ������
            OPENFILENAMEW OFN = { 0 };
            OFN.lStructSize = sizeof(OPENFILENAMEW);
            OFN.hwndOwner = nullptr; // ��� HWND ������ ����
            OFN.lpstrFile = filePath;
            OFN.nMaxFile = MAX_PATH;
            OFN.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
            OFN.nFilterIndex = 1;
            OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            ///////////////////////////////////

            std::string filename;
            // �������� ���������� ������ �����
            if (GetOpenFileNameW(&OFN)) // ����� GetOpenFileNameW
            {
                filename = ConvertLPWSTRToString(OFN.lpstrFile); // ��������� ��������� ����

                if (checkName_openFile(filename)) // �������� �����
                {
                    if (IsWindow(hWnd))
                    {
                        SetWinStatus(filename);
                    }
                    else
                    {
                        MessageBoxA(NULL, "������: ���������������� ����!", "������", MB_OK | MB_ICONERROR);
                    }
                }
                else
                {
                    MessageBoxA(hWnd, "���� �� ������ ��������.", "������", MB_OK | MB_ICONERROR);
                }
            }
            else
            {
                MessageBoxA(hWnd, "������ �������� �����.", "������", MB_OK | MB_ICONERROR);
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
            MessageBox(hWnd, L"���������� � ��������� � �� �������������:", L"����", MB_OK | MB_ICONINFORMATION);
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
        // ����������� ������� �����������
        if (buttons::graphics.hBitmap)
        {
            buttons::graphics.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons::graphics.hdcMem, buttons::graphics.hBitmap);

            GetObject(buttons::graphics.hBitmap, sizeof(BITMAP), &bitmap);
            BitBlt(hdc, 20, 0, bitmap.bmWidth, bitmap.bmHeight, buttons::graphics.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons::graphics.hdcMem, hOldBitmap);
            DeleteDC(buttons::graphics.hdcMem);
        }

        // ����������� ������� �����������
        if (buttons::graphics.hBitmap2)
        {
            buttons::graphics.hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(buttons::graphics.hdcMem, buttons::graphics.hBitmap2);

            GetObject(buttons::graphics.hBitmap2, sizeof(BITMAP), &bitmap2);
            BitBlt(hdc, bitmap.bmWidth + 20, 0, bitmap2.bmWidth, bitmap2.bmHeight, buttons::graphics.hdcMem, 0, 0, SRCCOPY);

            SelectObject(buttons::graphics.hdcMem, hOldBitmap);
            DeleteDC(buttons::graphics.hdcMem);
        }

        // ������ ������ ����� ������ ����������� �����

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
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // ������������� ���� ���� (�����)
            SetTextColor(hdcStatic, RGB(0, 0, 0));     // ������������� ���� ������ (������)
            return (INT_PTR)buttons::graphics.hBrush;            // ���������� ����� ��� ����
        }
        break;
    }
    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wp;
        HWND hEdit = (HWND)lp;

        if (hEdit == buttons::widgets.hEditRhymes || hEdit == buttons::widgets.hEditText || hEdit == buttons::widgets.hEditInputWord)
        {
            SetBkColor(hdcEdit, RGB(255, 255, 255)); // ������������� ���� ���� (�����)
            SetTextColor(hdcEdit, RGB(0, 0, 0));     // ������������� ���� ������ (������)
            return (INT_PTR)buttons::graphics.hBrush;          // ���������� ����� ��� ����
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

// ������� ��� �������������� LPWSTR � std::string
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

    // ������� ����������� ����
    if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }

    return result;
}

// ������� ��� �������������� std::string � LPWSTR
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

    // ������ ������ ����� ������ �������� ����������
    HPEN hOldPen = (HPEN)SelectObject(hdc, buttons::graphics.hPenBlack);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
}

BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem)
{
    HDC hdc = lpDrawItem->hDC; // ���������� ��������� ���������� HDC
    RECT rect = lpDrawItem->rcItem;

    // ������� ����� ����� ���� ���� (����� ������ ������ �������)
    HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255)); // ���� ���� ����
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush); // ������� ����� ����� �������������

    // �������� ������ ����� ��� ������
    HBRUSH hBrushes;
    BOOL isActive = FALSE;
    const char *buttonText;
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hBrushes = buttons::graphics.hBrushRed;
        buttonText = "�����";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hToggleButton)
    {
        isActive = buttons::ShowInEdit;
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = isActive ? "����� �� ����� �������" : "����� �� ����� ��������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hVerbButton)
    {
        isActive = buttons::ButtonFlags.test(0);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbButton)
    {
        isActive = buttons::ButtonFlags.test(1);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "�������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdjectiveButton)
    {
        isActive = buttons::ButtonFlags.test(2);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "��������������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hNounButton)
    {
        isActive = buttons::ButtonFlags.test(3);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "���������������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hParticipleButton)
    {
        isActive = buttons::ButtonFlags.test(4);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "���������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbialButton)
    {
        isActive = buttons::ButtonFlags.test(5);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "������������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearchType)
    {
        isActive = buttons::ButtonFlags.test(7);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = isActive ? "����� ������: ����������" : "����� ������: ������������";
    }
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "�����";
    }
    else
    {
        return FALSE; // ����������� ������
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;

    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton ||
        lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // ����� ���������� ��� ������� ������
    }
    else
    {
        hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
    }
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrushes);
    SetTextColor(hdc, RGB(0, 0, 0));

    // ���� ������ ������, ������� ������������� � �������� ���� ����
    if (lpDrawItem->itemState & ODS_SELECTED)
    {
        OffsetRect(&rect, 4, 4);
        rect.right -= 4;
        rect.bottom -= 4;
    }

    // ������ ������������ �������������
    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 25, 25);

    // ��������� ����������� ��� � ������������� ���� ������
    SetBkMode(hdc, TRANSPARENT);

    // ���������� ����� ������ ������
    DrawTextA(hdc, buttonText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ������� �������� ������� GDI
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    return TRUE;
}

// ���������� ������ ����������
void ExitSoftware()
{
    PostQuitMessage(0);
}

// �������� ������� ����
void MainWndAddMenus(HWND hWnd)
{
    HMENU RootMenu = CreateMenu();
    HMENU SubMenu = CreateMenu();

    // �������� ��������� ����
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"����");
    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnReadFile, L"������ �����");
    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnSaveFile, L"������ �����");

    // �������� ������� ����
    AppendMenu(SubMenu, MF_STRING, buttons::buttonIDs.OnInfoClicked, L"����");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, buttons::buttonIDs.OnExitSoftware, L"��������� ������");

    SetMenu(hWnd, RootMenu);
}

// �������� ��������
void MainWndAddWidget(HWND hWnd)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 45;

    // ��������� ��� ������ � ��������
    int buttonWidth = screenWidth * 12 / 100;  // 15% �� ������ ������
    int buttonHeight = screenHeight * 4 / 100; // 5% �� ������ ������
    int marginX = screenWidth * 2 / 100;       // 2% �� ������ ������
    int marginY = screenHeight * 2 / 100;      // 2% �� ������ ������

    // ������
    buttons::widgets.hExitButton = CreateButton("�����", marginX, screenHeight - buttonHeight - marginY-100, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButExit);
    buttons::widgets.hVerbButton = CreateButton("������", marginX, marginY + buttonHeight + marginY + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButVerb);
    buttons::widgets.hAdjectiveButton = CreateButton("��������������", marginX, marginY + 2 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdjective);
    buttons::widgets.hNounButton = CreateButton("���������������", marginX, marginY + 3 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButNoun);
    buttons::widgets.hAdverbialButton = CreateButton("������������", marginX, marginY + 4 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdverbial);
    buttons::widgets.hParticipleButton = CreateButton("���������", marginX, marginY + 5 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButParticiple);
    buttons::widgets.hAdverbButton = CreateButton("�������", marginX, marginY + 6 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButAdverb);
    buttons::widgets.hSearchType = CreateButton("��� ������", marginX, marginY + 7 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.ButSearchType);
    buttons::widgets.hSearch = CreateButton("�����", marginX, screenHeight - 2 * (buttonHeight + marginY) - 280, buttonWidth, buttonHeight, hWnd, buttons::buttonIDs.Search);


    // ����������� ��������
    buttons::widgets.hOutputStatusText = CreateStatic("��������� ����: ", marginX + buttonWidth + marginX, marginY, buttonWidth-100, 20, hWnd);
    buttons::widgets.hInputWord = CreateStatic("������� ����� ��� ������ ����", marginX, marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 11, buttonWidth, 20, hWnd);
    buttons::widgets.hOutputRhymes = CreateStatic("��������� �����", marginX + buttonWidth + marginX, marginY + buttonHeight + marginY, (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, buttonHeight, hWnd);
    buttons::widgets.hOutputText = CreateStatic("����� � ���������� �������", marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, marginY + buttonHeight + marginY, (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, buttonHeight, hWnd);

    // ���� ��������������
    buttons::widgets.hOutputStatus = CreateStatic("",marginX + buttonWidth + marginX + buttonWidth - 99, marginY, buttonWidth*4, 20, hWnd);
    buttons::widgets.hEditInputWord = CreateEdit(
        marginX,
        marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 32, // ������� ����� ��� hInputWord � ������� � 1 �������
        buttonWidth,
        60, // ������ ����
        hWnd,
        false
    );
    // ���� ��������������
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1; // ������� ������� ��� ����� �������������� (�������� � hOutputRhymes � hOutputText)
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1; // ������ ����� � ������ �������
    
    buttons::widgets.hEditRhymes = CreateEdit(
        marginX + buttonWidth + marginX,
        editTopMargin,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, // ������ � ������ ������
        editHeight,
        hWnd,
        true
    );

    buttons::widgets.hEditText = CreateEdit(
        marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 , // ����� ������ � ������ ������
        editTopMargin,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, // ������ � ������ ������
        editHeight,
        hWnd,
        true
    );
}

// ������� ��� �������� ������
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id)
{
    HWND hButton = CreateWindowA(
        "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height, hWnd, (HMENU)id, NULL, NULL);

    if (!hButton)
    {
        MessageBoxA(hWnd, "�� ������� ������� ������", "������", MB_OK | MB_ICONERROR);
    }

    return hButton;
}

// ������� ��� �������� ������������ ������
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd)
{
    HWND hStatic = 0;
    if (text == "��������� ����: ")
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
        MessageBoxA(hWnd, "�� ������� ������� ����������� �����", "������", MB_OK | MB_ICONERROR);
    }

    return hStatic;
}

// ������� ��� �������� ���� ��������������
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
        MessageBoxA(hWnd, "�� ������� ������� ���� ��������������", "������", MB_OK | MB_ICONERROR);
    }

    return hEdit;
}
// ������ � ����
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

// ��������� ��������� ���������� �� �������� ������ ��� ������ � ������
void SetOpenFileParams(HWND hWnd, string filename)
{
    // �������� ����� ��� ���� � ������ MAX_PATH
    WCHAR path[MAX_PATH] = L""; // �������������� ������ �����

    // ������������� ��������� OPENFILENAME
    ZeroMemory(&OFN, sizeof(OFN)); // ������� ���������
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = path;  // ��������� ����� ��� �������� ����� �����
    OFN.nMaxFile = MAX_PATH;  // ������������ ����� ����
    OFN.lpstrFilter = L"��������� ����� (*.txt)\0*.txt\0��� ����� (*.*)\0*.*\0"; // ������ ��� ������
    OFN.lpstrFileTitle = NULL;  // �������� �����
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = L"D:"; // ��������� ����������
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // �������� ������������� ���� � �����
}
// �������� � ������ ���������
void SetWinStatus(string status)
{
    SetWindowTextA(buttons::widgets.hOutputStatus, status.c_str());
}
