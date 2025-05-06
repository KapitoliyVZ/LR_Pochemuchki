#include "UI_const.h"
#include "Heart_of_program.h"
#include "file_working.h"
#include "NLP.h"



void ShowLoadingWindow(HWND hWnd) {
    // ������� ����� ��� ���� ����
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0)); // ������������� ����� ���� ����
    // ������������� ����� � �������� ���� ����
    SetClassLongPtr(buttons::widgets.hLoadingWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
    HDC hdc = GetDC(buttons::widgets.hLoadingWnd);
    SetTextColor(hdc, RGB(0, 255, 0)); // ������������� ������ ���� ������
    // �������� ���������� ���� "��������� �����" � "����� � ���������� �������"
    if (buttons::widgets.hLoadingWnd == NULL) {
        buttons::widgets.hLoadingWnd = CreateWindowEx(
            0,
            L"STATIC",
            L"����� ����... ����������, ���������.",
            WS_POPUP | WS_VISIBLE ,
            CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,
            hWnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        RECT rectInputWord;
        GetWindowRect(buttons::widgets.hEditInputWord, &rectInputWord);

        // �������� ���������� ���� "��������� �����" � "����� � ���������� �������"
        RECT rectRhymes, rectText;
        GetWindowRect(buttons::widgets.hEditRhymes, &rectRhymes);
        GetWindowRect(buttons::widgets.hEditText, &rectText);

        // ��������� ������� ����� ����� ����� ������
        int centerX = (rectRhymes.left + rectRhymes.right + rectText.left + rectText.right) / 4;
        int centerY = (rectRhymes.top + rectRhymes.bottom + rectText.top + rectText.bottom) / 4;

        // ���������� ���� �������� ������������ ������� �����
        int x = centerX - 150; // 150 - �������� ������ ���� ��������
        int y = centerY - 50;  // 50 - �������� ������ ���� ��������
        SetWindowPos(buttons::widgets.hLoadingWnd, HWND_TOPMOST, x, y, 300, 100, SWP_SHOWWINDOW);

        //// ������������� ���� �������� ��� �������� "������� ����� ��� ������ ����"
        //int x = rectInputWord.left;
        //int y = rectInputWord.bottom + 10; // 10 - ������ �� ������ ������� �������
        //SetWindowPos(buttons::widgets.hLoadingWnd, HWND_TOPMOST, x, y, 290, 100, SWP_SHOWWINDOW);

    }
    else {
        ShowWindow(buttons::widgets.hLoadingWnd, SW_SHOW);
    }
    UpdateWindow(buttons::widgets.hLoadingWnd);
    // ��������� �������� ����
    EnableWindow(hWnd, FALSE);
}

void HideLoadingWindow(HWND hWnd) {
    if (buttons::widgets.hLoadingWnd != NULL) {
        ShowWindow(buttons::widgets.hLoadingWnd, SW_HIDE);
        // ������������ �������� ����
        EnableWindow(hWnd, TRUE);
        SetFocus(hWnd);
    }
}



// ����� ���������� � �������� ��������� ��������
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (!LoadLibraryA("Msftedit.dll")) 
    {
        MessageBoxA(NULL, "�� ������� ��������� Msftedit.dll", "������", MB_OK | MB_ICONERROR);
        return 0;
    }
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

void UpdateButtonStatesAndColors()
{
    // ��������� ��������� ������ � ����������� �� ������
    bool isHomogeneousMode = buttons::ButtonFlags[7]; // ���� ����������� ������
    bool hasInput = GetWindowTextLengthA(buttons::widgets.hEditInputWord) > 0;

    // ��������� ����������� ������
    EnableWindow(buttons::widgets.hVerbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdjectiveButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hNounButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hParticipleButton, !isHomogeneousMode || !hasInput);
    EnableWindow(buttons::widgets.hAdverbialButton, !isHomogeneousMode || !hasInput);

    // �������������� ������ ��� ���������� �� ������
    InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE);
    InvalidateRect(buttons::widgets.hSearchType, NULL, TRUE);

    // ��������� ���� ��� ���������� ���������
    UpdateWindow(buttons::widgets.hVerbButton);
    UpdateWindow(buttons::widgets.hAdverbButton);
    UpdateWindow(buttons::widgets.hAdjectiveButton);
    UpdateWindow(buttons::widgets.hNounButton);
    UpdateWindow(buttons::widgets.hParticipleButton);
    UpdateWindow(buttons::widgets.hAdverbialButton);
    UpdateWindow(buttons::widgets.hSearchType);
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


// ��������� ������� ������
void SetRichEditBold(HWND hRichEdit, bool bold)
{
    SendMessage(hRichEdit, EM_SETSEL, 0, -1);  // �������� ���� �����

    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_BOLD;          // ���������, ��� ������ ��������
    cf.dwEffects = bold ? CFE_BOLD : 0; // ��������/��������� ������ �����

    SendMessage(hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

// ��������� ������������
void SetRichEditStrikeout(HWND hRichEdit, bool strikeout)
{
    SendMessage(hRichEdit, EM_SETSEL, 0, -1);  // �������� ���� �����

    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_STRIKEOUT;          // ���������, ��� ������ ������������
    cf.dwEffects = strikeout ? CFE_STRIKEOUT : 0; // ��������/��������� ������������

    SendMessage(hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

void UpdateCheckboxStates()
{
    // �������� �������
    bool isHomogeneousMode = buttons::ButtonFlags[7];
    bool partOfSpeechSelected;
    int selectedPartsOfSpeech = buttons::ButtonFlags.test(0) + buttons::ButtonFlags.test(1) +
        buttons::ButtonFlags.test(2) + buttons::ButtonFlags.test(3) +
        buttons::ButtonFlags.test(4) + buttons::ButtonFlags.test(5);

    if (isHomogeneousMode)
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"���������� ����� ������");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true);
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"�������� ���� ����� ����");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, false);
        partOfSpeechSelected = (selectedPartsOfSpeech == 1);
    }
    else
    {
        SetWindowText(buttons::widgets.hStaticCheckBox3Info, L"������������ ����� ������");
        SetRichEditBold(buttons::widgets.hStaticCheckBox3Info, true); 
        SetWindowText(buttons::widgets.hStaticCheckBox1Info, L"�������� �� ����� ���� ������ ����");
        SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, false);
        partOfSpeechSelected = (selectedPartsOfSpeech > 1);
    }

    bool isFileSelected = !filename_str.empty();
    SetRichEditStrikeout(buttons::widgets.hStaticCheckBox2Info, isFileSelected);

    
    SetRichEditStrikeout(buttons::widgets.hStaticCheckBox1Info, partOfSpeechSelected);

    // ��������� ����
    UpdateWindow(buttons::widgets.hStaticCheckBox1Info);
    UpdateWindow(buttons::widgets.hStaticCheckBox2Info);
    UpdateWindow(buttons::widgets.hStaticCheckBox3Info);
}

// ����� ������ � ����
void OutputTextInfo(const vector<vector<string>>& sentences)
{
    vector<vector<wstring>> Wsentences;

    // �����������
    for (const vector<string>& sentence : sentences)
    {
        vector<wstring> wsentence;
        for (const string& word : sentence)
        {
            wsentence.push_back(utf8_to_wstring(word)); // ���������� ������-������� ��� ��������������
        }
        Wsentences.push_back(wsentence);
    }
    // ������� ����� � ����
    for (vector<wstring>& sentence : Wsentences)
    {
        bool firstWord = true;
        for (wstring& word : sentence)
        {

            wstring tmp_word = word;
            static const set<wstring> punctuationMarks = { L".", L",", L"!", L"?", L":", L";", L"-", L"(", L")", L"\"", L"'" };

            if (!firstWord && punctuationMarks.find(tmp_word) == punctuationMarks.end())
            {
                // ��������� ������ ����� ������ (���� ��� �� ���������� � �� ������ �����)
                SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L" ");
            }

            SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)tmp_word.c_str());

            firstWord = false;
        }

        // ������� ������ ����� �����������
        SendMessageW(buttons::widgets.hEditText, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
    }
}

// ����� ���� � ����
void OutputRhymeInfo(const vector<WordData>& rhymes_data) 
{
    
    // ������� ���� ����� ������� ����� ����������
    SetWindowTextW(buttons::widgets.hEditRhymes, L"");

    int counter = 0;
    for (const WordData& output : rhymes_data) {
        counter++;

        // �������� ���������� � �����
        wstring wordInfo;
        if (counter == 1) {
            wordInfo = L"�����: ";
        }
        else {
            wordInfo = L"\r\n\r\n�����: ";
        }

        wordInfo += utf8_to_wstring(output.word);

        // ����� ����
        wordInfo += L"\r\n����� ����: " + utf8_to_wstring(output.part_of_speech);

        // ���������� ��������� ����
        wordInfo += L"\r\n���������� ��������� ����: " + to_wstring(output.amount);

        // ���������� ����
        wordInfo += L"\r\n���������� ����: " + to_wstring(output.rhymed_amount);

        // ��������� �������� ����������
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)wordInfo.c_str());

        // �����������, ��� ����������� �����
        std::wstring sentenceInfo = L"\r\n������� � ������������: ";
        if (!output.sentence_counter.empty()) 
        {
            for (size_t i = 0; i < output.sentence_counter.size(); ++i) 
            {
                if (i != 0) sentenceInfo += L", ";
                sentenceInfo += to_wstring(output.sentence_counter[i]);
            }
        }
        SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)sentenceInfo.c_str());

        // �����
        if (!output.rhymed_words.empty()) 
        {
            SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n�����:");
            for (const auto& word : output.rhymed_words) 
            {
                std::wstring rhyme = L"\r\n  � " + utf8_to_wstring(word);
                SendMessageW(buttons::widgets.hEditRhymes, EM_REPLACESEL, FALSE, (LPARAM)rhyme.c_str());
            }
        }
    }

    // ������������ � ������
    SendMessageW(buttons::widgets.hEditRhymes, EM_SETSEL, 0, 0);
    SendMessageW(buttons::widgets.hEditRhymes, EM_SCROLLCARET, 0, 0);
}

// �������� ���� ���������
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static string str_sentences;                        // ������ ��� �������� ������
    static vector<vector<string>> sentences;    // ������ ������ � ������������ �������������
    static vector<WordData> rhymes_data;        // ��������� ����� � ������� �����

    switch (msg)
    {
		// �������� ����
    case WM_CREATE:
    {
		// �������� ����
        MainWndAddMenus(hWnd);

		// �������� ��������
        MainWndAddWidget(hWnd);

        void UpdateCheckboxStates();

        // ��������� ���������� ������ �����
        SetOpenFileParams(hWnd);

		// ����� ������ ������
        buttons::ButtonFlags.reset();

		// �������� ����
        buttons::graphics.hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
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
        // ���������� ����������� ���������
        break;
    }
    case WM_COMMAND:
    {
       
        // ���������, ���� ����������� ������ �� ���� ����� ����� ��� ������ �� ���� ����
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == buttons::widgets.hEditInputWord)
        {
            // �������� ����� �� ���� ��������������
            char buffer[256] = { 0 };
            GetWindowTextA(buttons::widgets.hEditInputWord, buffer, sizeof(buffer));

            // ���� ���� �� ������, ��������� ������
            if ( (strlen(buffer) > 0) && buttons::ButtonFlags[7] == true)
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
		// ������ ������ "�����"
        if (LOWORD(wp) == buttons::buttonIDs.ButExit)
        {
            // �������������� ������
            SetWindowText(buttons::widgets.hExitButton, L"");
            InvalidateRect(buttons::widgets.hExitButton, NULL, TRUE); 
            UpdateWindow(buttons::widgets.hExitButton);
            ExitSoftware();
        }
		// ������ ������ "��� ������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButSearchType)
        {
            buttons::ButtonFlags.flip(7);
            // �������������� ������
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();
        }
		// ������ ������ "������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButVerb)
        {
            buttons::ButtonFlags.flip(0);
            // �������������� ������
            SetWindowText(buttons::widgets.hVerbButton, L"");
            InvalidateRect(buttons::widgets.hVerbButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hVerbButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "�������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverb)
        {
            buttons::ButtonFlags.flip(1);
            // �������������� ������
            SetWindowText(buttons::widgets.hAdverbButton, L"");
            InvalidateRect(buttons::widgets.hAdverbButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdverbButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "��������������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdjective)
        {
            buttons::ButtonFlags.flip(2);
            // �������������� ������
            SetWindowText(buttons::widgets.hAdjectiveButton, L"");
            InvalidateRect(buttons::widgets.hAdjectiveButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdjectiveButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "���������������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButNoun)
        {
            buttons::ButtonFlags.flip(3);
            // �������������� ������
            SetWindowText(buttons::widgets.hNounButton, L"");
            InvalidateRect(buttons::widgets.hNounButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hNounButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "���������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButParticiple)
        {
            buttons::ButtonFlags.flip(4);
            // �������������� ������
            SetWindowText(buttons::widgets.hParticipleButton, L"");
            InvalidateRect(buttons::widgets.hParticipleButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hParticipleButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "������������"
        else if (LOWORD(wp) == buttons::buttonIDs.ButAdverbial)
        {
            buttons::ButtonFlags.flip(5);
            // �������������� ������
            SetWindowText(buttons::widgets.hAdverbialButton, L"");
            InvalidateRect(buttons::widgets.hAdverbialButton, NULL, TRUE); // �������������� ������
            UpdateWindow(buttons::widgets.hAdverbialButton);
            UpdateCheckboxStates();
        }
		// ������ ������ "����� �� ���������"
        else if (LOWORD(wp) == buttons::buttonIDs.OnExitSoftware)
        {
            ExitSoftware();
        }
		// ������ ������ "�����"
        else if (LOWORD(wp) == buttons::buttonIDs.Search)
        {
            // ����� ����� ������ �����
			buttons::ButtonFlags[6] = 0; 

			// �������� ����� �� ���� ��������������
            char WordToSearch[60] = ""; 
            GetWindowTextA(buttons::widgets.hEditInputWord, WordToSearch, 60);

            // ����� ��� ������ �� ���� ����
			string word_to_compare = WordToSearch; 
            
			// ���������, ���� ���� ����� ������
			if (word_to_compare.length() != 0)
			{
                buttons::ButtonFlags[6] = true;
			}
			// ���������, ���� ������ �� ������
            if (buttons::ButtonFlags.test(0) == 0 and buttons::ButtonFlags.test(1) == 0 and
                buttons::ButtonFlags.test(2) == 0 and buttons::ButtonFlags.test(3) == 0 and
                buttons::ButtonFlags.test(4) == 0 and buttons::ButtonFlags.test(5) == 0 and
                buttons::ButtonFlags.test(6) == 0)
            {
                MessageBoxA(hWnd, "�������� ����� ���� ��� ������� ����� ��� ������", "������", MB_OK | MB_ICONERROR);
                break;
            }
			// ���������, ���� ������� ��������� ������ ���� � ������ ����� ������������� ������
            if (buttons::ButtonFlags.count() < 2 and
                buttons::ButtonFlags.test(7) == 0)
            {
                MessageBoxA(hWnd, "�������� ��������� ������ ���� ��� ������� ����� ��� ������", "������", MB_OK | MB_ICONERROR);
                break;
            }

            // ���������� ���� ��������
            ShowLoadingWindow(hWnd);

            // �������� ��������� �����, ����������� ����������� � ����� 
            unite_functions(rhymes_data, sentences, str_sentences, word_to_compare, buttons::ButtonFlags);

            // �������� ���� ��������
            HideLoadingWindow(hWnd);

            // ������� ���������� ���� ����� ����������� ������ ������
            SetWindowTextA(buttons::widgets.hEditRhymes, "");
            SetWindowTextA(buttons::widgets.hEditText, "");

			// ���� �� ������� ����, ������� ���������
            if (rhymes_data.empty())
            {
                MessageBoxA(hWnd, "�� ������� ����", "������", MB_OK | MB_ICONERROR);
                break;
            }
            if (sentences.empty())
            {
				MessageBoxA(hWnd, "�� ������� �����������", "������", MB_OK | MB_ICONERROR);
				break;
            }

            // ����� ������
            OutputTextInfo(sentences);
            
            // ����� ����
            OutputRhymeInfo(rhymes_data);
            // �������������� ���� ������
            InvalidateRect(buttons::widgets.hEditText, NULL, TRUE);
            UpdateWindow(buttons::widgets.hEditText);
            UpdateWindow(buttons::widgets.hEditRhymes);
            buttons::ButtonFlags.reset();
            UpdateButtonStatesAndColors();
            UpdateCheckboxStates();
        }
		// ������ ������ "��������� ����"
		else if (LOWORD(wp) == buttons::buttonIDs.ButSaveFile)
		{
            // ����� ������� ��� ������ � ����
            string outputFileName_numbered; // ��� ��������� �����-������
            string outputFileName_rhymes; // ��� ��������� �����-����
            outputFiles_working(filename_str, outputFileName_numbered, outputFileName_rhymes, sentences, rhymes_data);

            wstring wtext = utf8_to_wstring(outputFileName_numbered);
            wstring wrhymes = utf8_to_wstring(outputFileName_rhymes);
            // ������������� ����� � ���� "��������� ����"
            SetWindowTextA(buttons::widgets.hPathSaveFileData, outputFileName_numbered.c_str());
            SetWindowTextA(buttons::widgets.hPathSaveFileRhymes, outputFileName_rhymes.c_str());
            UpdateWindow(buttons::widgets.hPathSaveFileData);
            UpdateWindow(buttons::widgets.hPathSaveFileRhymes);
		}
        // ������� ������ "������ �����"
        else if (LOWORD(wp) == buttons::buttonIDs.ButOpenFile)
        {
          
            // �������� ���������� ������ �����
            if (GetOpenFileNameA(&OFN)) // ����� GetOpenFileNameW
            {
                filename_str = filename;
                pair<bool, string> fromFunct = inputFile_working(filename_str);
                bool fromFunctStatus = fromFunct.first;         // ������ �������� �����
                string fromFunctText = fromFunct.second;        // ����� � ��������� ������ ��� ������ �����


				if (fromFunctStatus == false)
				{
					MessageBoxA(hWnd, fromFunctText.c_str(), "������", MB_OK | MB_ICONERROR);
					break;
				}
                
				str_sentences = fromFunctText; // �������� ����� �� �������
                SetWindowTextA(buttons::widgets.hOutputStatus, filename_str.c_str());

                // open_file(filename_str);
                UpdateCheckboxStates();
            }
            else
            {
                MessageBoxA(hWnd, "������ ������ �����.", "������", MB_OK | MB_ICONERROR);
            }


            break;
        }
        // ������ ������ "����"
        else if (LOWORD(wp) == buttons::buttonIDs.OnInfoClicked)
        {
            MessageBox(hWnd, L"���������� � ��������� � �� �������������:", L"����", MB_OK | MB_ICONINFORMATION);
        }
        // ������ ������ "�������"
        else if (LOWORD(wp) == buttons::buttonIDs.OnHelp) {
            MessageBox(hWnd, L"���������� �� ������������� ���������:\n"
                L"1. �������� ����� ���� ��� ������� ����� ��� ������.\n"
                L"2. ������� ������ '�����' ��� ������ ������ ����.\n"
                L"3. ���������� ������ ����� ���������� � ��������������� �����.",
                L"�������", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
	// ��������� ������������ ������
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
        MakeRoundButton(lpDrawItem);
        break;
    }
	// ��������� ������������ �������� � �����
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

        HWND All_hwnd[11] = { buttons::widgets.hOutputStatus, buttons::widgets.hOutputStatusText, 
                             buttons::widgets.hEditRhymes, buttons::widgets.hOutputRhymes, 
                             buttons::widgets.hOutputText, buttons::widgets.hEditText, 
                             buttons::widgets.hInputWord, buttons::widgets.hEditInputWord,
                             buttons::widgets.hPathSaveFileData, buttons::widgets.hPathSaveFileRhymes,
                             buttons::widgets.hPathSaveFileText };

        for (int i = 0; i < 11; i++)
        {
            MakeFrame(hWnd, hdc, All_hwnd[i]);
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);

        EndPaint(hWnd, &ps);
        break;
    }
	// ��������� ������������ ����������� �����
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wp;
        HWND hStatic = (HWND)lp;

        if (hStatic == buttons::widgets.hOutputStatus || hStatic == buttons::widgets.hOutputStatusText || hStatic == buttons::widgets.hOutputStatus ||
            hStatic == buttons::widgets.hOutputRhymes || hStatic == buttons::widgets.hOutputText || hStatic == buttons::widgets.hLoadingWnd)
        {
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // ������������� ���� ���� (�����)
            SetTextColor(hdcStatic, RGB(0, 0, 0));     // ������������� ���� ������ (������)
            return (INT_PTR)buttons::graphics.hBrush;            // ���������� ����� ��� ����
        }
        break;
    }
	// ��������� ������������ ����� �����
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
    // ��������� ���������� ������
    case WM_DESTROY:
    {	// ����������� �������
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
	// ��������� ��������� ���������
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}
// ������� ��� ������������ ������, ���������� ��� LPWSTR
void FreeLPWSTR(LPWSTR lpwstr)
{
    delete[] lpwstr;
}


// ������� ��� �������������� LPWSTR � std::string
std::string ConvertLPWSTRToString(LPWSTR lpwstr)
{
	// ���������, ��� lpwstr �� �������� ������� ����������
    if (!lpwstr) {
        std::cerr << "Null LPWSTR" << std::endl;
        return std::string();
    }
	// ����������� LPWSTR � std::string
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
	// ���������, ��� ������ �� ������
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    LPWSTR lpwstr = new WCHAR[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, lpwstr, size_needed);
    return lpwstr;
}

// ��������� ����� ����������� ����� � ����� �����
void MakeFrame(HWND hWnd, HDC hdc, HWND Edit)
{
	// �������� ������� �������� ����������
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

// ������� ��� �������� ������������ ������
BOOL MakeRoundButton(LPDRAWITEMSTRUCT lpDrawItem)
{
    HDC hdc = lpDrawItem->hDC;
    RECT rect = lpDrawItem->rcItem;

    // ������� ����� ����� ���� ���� (����� ������ ������ �������)
    HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255)); // ���� ���� ����
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush); // ������� ����� ����� �������������

    // �������� ������ ����� ��� ������
    HBRUSH hBrushes;
    BOOL isActive = FALSE;
    const char *buttonText;
	// ���������� ����� ������ � ����������� �� �� ��������������
	// ������ "�����"
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton)
    {
        hBrushes = buttons::graphics.hBrushRed;
        buttonText = "�����";
    }
	// ������ "������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hVerbButton)
    {
        isActive = buttons::ButtonFlags.test(0);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "������";
    }

	// ������ "�������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbButton)
    {
        isActive = buttons::ButtonFlags.test(1);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "�������";
    }

	// ������ "��������������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdjectiveButton)
    {
        isActive = buttons::ButtonFlags.test(2);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "��������������";
    }

	// ������ "���������������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hNounButton)
    {
        isActive = buttons::ButtonFlags.test(3);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "���������������";
    }

	// ������ "���������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hParticipleButton)
    {
        isActive = buttons::ButtonFlags.test(4);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "���������";
    }

	// ������ "������������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hAdverbialButton)
    {
        isActive = buttons::ButtonFlags.test(5);
        hBrushes = isActive ? buttons::graphics.hBrushGreen : buttons::graphics.hBrushGrey;
        buttonText = "������������";
    }

	// ������ "����� ������"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearchType)
    {
        isActive = buttons::ButtonFlags.test(7);
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = isActive ? "����� ������: ����������" : "����� ������: ������������";
    }

	// ������ "�����"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSearch)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "�����";
    }
    // ������ "������� ����"
    else if (lpDrawItem->hwndItem == buttons::widgets.hOpenFile)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "������� ����";
    }
    // ������ "��������� ����"
    else if (lpDrawItem->hwndItem == buttons::widgets.hSaveFile)
    {
        hBrushes = buttons::graphics.hBrushNeutral;
        buttonText = "��������� ����";
    }

	// ���� �������� ������
    else
    {
        return FALSE; // ����������� ������
    }

    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hOldBrush = NULL;
     
	// ������� ���������� ��� ������
    if (lpDrawItem->hwndItem == buttons::widgets.hExitButton ||
        lpDrawItem->hwndItem == buttons::widgets.hSearch ||
        lpDrawItem->hwndItem == buttons::widgets.hSearchType ||
        lpDrawItem->hwndItem == buttons::widgets.hOpenFile ||
        lpDrawItem->hwndItem == buttons::widgets.hSaveFile)
    {
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 210, 210)); // ����� ���������� ��� ������� ������
    }
    else
    {
        hPen = CreatePen(PS_SOLID, 1, isActive ? RGB(0, 200, 0) : RGB(200, 0, 0));
    }

	// ������������� ���� ������ � ����� ��� ������
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

    AppendMenu(RootMenu, MF_STRING, buttons::buttonIDs.OnHelp, L"�������"); // ����� �������

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
    int buttonWidth = screenWidth * 15 / 100;  // 15% �� ������ ������
    int buttonHeight = screenHeight * 4 / 100; // 5% �� ������ ������
    int marginX = screenWidth * 2 / 100;       // 2% �� ������ ������
    int marginY = screenHeight * 2 / 100;      // 2% �� ������ ������

    // ������
    buttons::widgets.hExitButton = CreateButton("�����", marginX, screenHeight - buttonHeight - marginY-80, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButExit);
    buttons::widgets.hVerbButton = CreateButton("������", marginX, marginY + buttonHeight + marginY + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButVerb);
    buttons::widgets.hAdjectiveButton = CreateButton("��������������", marginX, marginY + 2 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdjective);
    buttons::widgets.hNounButton = CreateButton("���������������", marginX, marginY + 3 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButNoun);
    buttons::widgets.hAdverbialButton = CreateButton("������������", marginX, marginY + 4 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdverbial);
    buttons::widgets.hParticipleButton = CreateButton("���������", marginX, marginY + 5 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButParticiple);
    buttons::widgets.hAdverbButton = CreateButton("�������", marginX, marginY + 6 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButAdverb);
    buttons::widgets.hSearchType = CreateButton("��� ������", marginX, marginY + 7 * (buttonHeight + marginY) + 20, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.ButSearchType);
    buttons::widgets.hSearch = CreateButton("�����", marginX, screenHeight - 2 * (buttonHeight + marginY) - 100, buttonWidth, buttonHeight, 
        hWnd, buttons::buttonIDs.Search);
    buttons::widgets.hOpenFile = CreateButton("������� ����", 2 * marginX + buttonWidth, marginY-10, buttonWidth, 30,
        hWnd, buttons::buttonIDs.ButOpenFile);
    buttons::widgets.hSaveFile = CreateButton("��������� ����", 2 * marginX + buttonWidth, marginY+30, buttonWidth, 30,
        hWnd, buttons::buttonIDs.ButSaveFile);
    

    // ����������� ��������
    buttons::widgets.hOutputStatusText = CreateRichEdit(L"������ ����: ", 
        10*marginX + buttonWidth, marginY-10, buttonWidth-80, 30, hWnd);
    buttons::widgets.hPathSaveFileText = CreateRichEdit(L"����� ����������: ",
        10 * marginX + buttonWidth, marginY+30, buttonWidth - 80, 30, hWnd);
    buttons::widgets.hInputWord = CreateRichEdit(L"������� ����� ��� ������ ����", 
        marginX, marginY + 7 * (buttonHeight + marginY) + 20 + buttonHeight + 11, buttonWidth, 30, hWnd);
    buttons::widgets.hOutputRhymes = CreateRichEdit(L"��������� �����", 
        marginX + buttonWidth + marginX, marginY + buttonHeight + marginY+50, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, buttonHeight, hWnd);
    buttons::widgets.hOutputText = CreateRichEdit(L"����� � ���������� �������", 
        marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, marginY + buttonHeight + marginY+50, 
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, buttonHeight, hWnd);


    // ���� ��������������
    buttons::widgets.hOutputStatus = CreateRichEdit(L"",10*marginX + buttonWidth + buttonWidth - 79, 
        marginY-10, buttonWidth*3, 30, hWnd);
    buttons::widgets.hPathSaveFileData = CreateRichEdit(L"", 10 * marginX + buttonWidth + buttonWidth - 79,
        marginY+30, buttonWidth * 3, 30, hWnd);
    buttons::widgets.hPathSaveFileRhymes = CreateRichEdit(L"", 10 * marginX + buttonWidth + buttonWidth - 79,
        marginY + 61, buttonWidth * 3, 30, hWnd);
    buttons::widgets.hEditInputWord = CreateRichEdit(L"",marginX,
        marginY + 7 * (buttonHeight + marginY) + 30 + buttonHeight + 32, buttonWidth, 60, hWnd);

    // ������� ������� ��� ����� �������������� (�������� � hOutputRhymes � hOutputText)
    int editTopMargin = marginY + buttonHeight + marginY + buttonHeight + 1; 

    // ������ ����� � ������ �������
    int editHeight = screenHeight - editTopMargin - 2 * (buttonHeight + marginY) - 1; 
    
    buttons::widgets.hEditRhymes = CreateRichEdit(L"",marginX + buttonWidth + marginX, editTopMargin + 50,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2 - 1, editHeight-50, hWnd);
    buttons::widgets.hEditText = CreateRichEdit(L"",marginX + buttonWidth + marginX + (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, editTopMargin + 50,
        (screenWidth - (2 * marginX + buttonWidth + marginX)) / 2, editHeight-50, hWnd);


    
    buttons::widgets.hStaticCheckBox1Info = CreateRichEdit(L"�������� �� ����� ���� ������ ����",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 143,
        buttonWidth, 30, hWnd); 

    buttons::widgets.hStaticCheckBox2Info = CreateRichEdit(L"�������� ����",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 173,
        buttonWidth, 30, hWnd);

    buttons::widgets.hStaticCheckBox3Info = CreateRichEdit(L"������������ ����� ������",
        marginX, screenHeight - 2 * (buttonHeight + marginY) - 203,
        buttonWidth, 30, hWnd);

}

HWND CreateRichEdit(LPCWSTR text, int x, int y, int width, int height, HWND hParent) 
{
    // �������, ��� ���������� ���������
    static bool richEditLoaded = false;
    if (!richEditLoaded) {
        LoadLibrary(TEXT("Msftedit.dll"));
        richEditLoaded = true;
    }

    HWND hRich = CreateWindowEx(
        0,
        MSFTEDIT_CLASS,  // "RichEdit50W"
        text,
        WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER | ES_MULTILINE | WS_BORDER,
        x, y, width, height,
        hParent, NULL, GetModuleHandle(NULL), NULL
    );

    if (!hRich) return NULL;

    // ���� ����� ������� ����� ������
    if (text == L"������������ ����� ������" or text == L"��������� �����" or text == L"����� � ���������� �������"
        or text == L"������ ����: " or text == L"����� ����������: " or text == L"������� ����� ��� ������ ����")
    {
        CHARFORMAT2 cf = { 0 };
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_BOLD;  // ���������, ��� ������ ��������
        cf.dwEffects = CFE_BOLD; // �������� ������ �����

        // �������� ���� ����� � ��������� ��������������
        SendMessage(hRich, EM_SETSEL, 0, -1);
        SendMessage(hRich, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    return hRich;
}

// ������� ��� �������� ������
HWND CreateButton(const char* text, int x, int y, int width, int height, HWND hWnd, int id)
{
	// ������� ������ � ��������� �����������
    HWND hButton = CreateWindowA(
        "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
	// ���������, ���� �� ������ ������� �������
    if (!hButton)
    {
        MessageBoxA(hWnd, "�� ������� ������� ������", "������", MB_OK | MB_ICONERROR);
    }

    return hButton;
}

// ������� ��� �������� ������������ ������
HWND CreateStatic(const char* text, int x, int y, int width, int height, HWND hWnd)
{
	// ������� ����������� ����� � ��������� �����������
    HWND hStatic = 0;
	// ������������� ����� ������ � ����������� �� ������
    if (text == "������ ����: " or text == "����� ����������: " or text == "�������� �� ����� ���� ������ ����" or text == "�������� ����" or text == "������������ ����� ������")
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_LEFT,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
	// ���� ����� �� ������������� �������� ��������, ������� ������� ����������� �����
    else
    {
        hStatic = CreateWindowA(
            "static", text, WS_VISIBLE | WS_CHILD | ES_CENTER,
            x, y, width, height, hWnd, NULL, NULL, NULL);
    }
	// ���������, ��� �� ����������� ����� ������� ������
    if (!hStatic)
    {
        MessageBoxA(hWnd, "�� ������� ������� ����������� �����", "������", MB_OK | MB_ICONERROR);
    }

    return hStatic;
}

// ������� ��� �������� ���� ��������������
HWND CreateEdit(int x, int y, int width, int height, HWND hWnd, bool readOnly)
{
    LoadLibraryA("Msftedit.dll");
	// ������� ���� �������������� � ��������� �����������
    DWORD style = WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL;
	// ���� ���� �������������� ������ ��� ������, ��������� ��������������� �����
    if (readOnly)
    {
        style |= ES_READONLY;
    }
	// ������� ���� ��������������
    HWND hEdit = CreateWindowExA(
        0, "RICHEDIT50W", "", style,
        x, y, width, height,
        hWnd, NULL, GetModuleHandle(NULL), NULL);
	// ���������, ���� �� ���� �������������� ������� �������
    if (!hEdit)
    {
        MessageBoxA(hWnd, "�� ������� ������� ���� ��������������", "������", MB_OK | MB_ICONERROR);
    }

    // ������� �����, RichEdit � ��� ������������ ������� ������, �� ����� ���� ������
    SendMessageA(hEdit, EM_EXLIMITTEXT, 0, (WPARAM)-1); // -1 = ����������� ���������

    return hEdit;
}


// ��������� ��������� ���������� �� �������� ������ ��� ������ � ������
void SetOpenFileParams(HWND hWnd)
{
	// ������������� ��������� ��������� ��� �������� �����
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

// �������� � ������ ���������
void SetWinStatus(string status)
{
    SetWindowTextA(buttons::widgets.hOutputStatus, status.c_str());
}

