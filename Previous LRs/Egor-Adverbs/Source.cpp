#include "Header.h" 

vector<string> exceptions = {
    "�����", "�����" , "���������" ,"�����", "��������" ,"�����", "�����", "������", "�����",
    "�����", "�������","�����", "��������", "��������", "�����"
    "�������", "�����", "������" ,"��������", "�������", "������", "�����",
    "�������", "��������", "������", "�������", "������",
    "������", "��������", "�������", "�������", "�������", "��������", "��������", "�����",
    //����� � ���������� "���"/"���" �� ��������, �.�. ��� ���������� � "��-", ��� �������� ��������� �������. 
    //����� � ���������� "��", "��", �� ������� � ����������, ��-�� �� �������� ����������. 
    //����� ������������ ����������, ��� ��������� ������ � ����������� �������������� 
    "��������", "�������", "�����", "�������", "������", "����", "������", "������", "���������",
    "������", "������", "������", "��������", "��������", "�����", "����", "������", "�����", "�����",
    "������", "�������", "����", "����", "����", "������", "�����", "�����", "������", "������",
    "�����", "�����", "������", "�����"
    //����� ������������ ����������, ��� ��������� ������ � ����������� �������� 
    "�����", "������" , "�������" ,"�����", "����" , "����", "������", "�����" ,"��������",
    "�������", "�����" ,"�����", "����", "������", "����", "�����", "������", "������",
    "������" , "������", "������", "�����", "������" , "����" ,"��������", "�������",
    "��������", "�����" ,"�����", "�����" , "�����", "������", "�������", "�����",
    "�����", "�������", "�������", "�������" , "�������", "�������", "����������", "��������",
    "�������", "��������", "������", "�������" , "������", "��������", "������", "������",
    "������", "�������", "��������", "��������",
    //����� ������������ ����������, ��� ��������� � ���������� ������� 
    "���", "���", "���", "���", "���������", "���������", "�����", "��������",
    "�����", "�����", "���", "�����"//, "���������", "���������", "�����", "��������"
};
vector<string> adjectivEendings = {
    "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "���", "���",
    "���", "���", "��", "��" , "��", "���", "���", "��", "��", "��" , "��", "��",
    "��", "��", "��", "��"
};
vector<string> verbEendings = {
    "���", "��", "��", "���" , "��", "��", "���", "��",
    "��", "���", "��", "��", "��", "����", "���", "��",
    "��"
};
vector<string> vowels = {
    "��", "��", "�", /*"�", "�", "�", "�", "�", "�", "�"*/
};

const vector<pair<char, char>> vow = { { '�', '�'}, { '�', '�' },
        { '�', '�' }, { '�', '�' }, { '�', '�' }, { '�', '�' } , { '�', '�'} , {'�','�'}};

const vector<pair<char, char>> cons = { { '�', '�'}, { '�', '�'}, { '�', '�' }, { '�', '�' },
    { '�', '�' }, { '�', '�' },  { '�', '�' },
    { '�', '�'},  { '�', '�' },{ '�', '�' }, { '�', '�' },  { '�', '�' } };

vector<string> colors = { //54 �����
    "255,0,0",    // �������
    "0,255,0",    // �������
    "0,0,255",    // �����
    "255,0,255",  // ����������
    "0,255,255",  // �������
    "128,0,0",    // �����-�������
    "0,128,0",    // �����-�������
    "0,0,128",    // �����-�����
    "128,128,0",  // ���������
    "128,0,128",  // �����-����������
    "0,128,128",  // �����-�������
    "255,165,0",  // ���������
    "255,69,0",   // ��������-�������
    "210,180,140",// �������
    "184,134,11", // �����-���������
    "102,205,170",// ������-�������������
    "186,85,211", // ������-����������
    "148,0,211",  // �����-����������
    "75,0,130",   // ������
    "233,150,122",// �����-���������
    "144,238,144",// ������-�������
    "175,238,238",// ������-�������
    "211,211,211",// ������-�����
    "255,215,0",  // �������
    "238,130,238",// ����������
    "220,220,220",// ������-�����
    "244,164,96", // ������-���������
    "210,105,30", // �����-���������
    "222,184,135",// ������-�������
    "188,143,143",// �������
    "250,128,114",// ���������
    "240,255,240",// ������-�������
    "230,230,250",// ����������
    "255,240,245",// ������-�������
    "139,69,19",  // ����������
    "255,228,225",// ����������
    "245,222,179",// ���������
    "240,230,140",// ����
    "245,245,220",// �������
    "255,248,220",// ����������
    "240,255,255",// �������������
    "240,255,240",// ������-�������
    "255,250,205",// �������� �����
    "175,238,238",// ������-�������
    "244,164,96", // ������-���������
    "255,228,225",// ����������
    "255,20,147", // ������-����������
    "210,180,140",// �����-�������
    "255,222,173",// ������ �����
    "255,250,240",// �������� �����
    "222,184,135",// ��������
    "255,228,196",// �������
    "255,245,238",// ������� �������
    "245,255,250",// �����-��������
    "240,255,255",// ����
};



void inputFromFile(const char* fname, vector<string>& adverb, bool mode) {  // ������� ��� ���������� ������ �� ����� � ������ � ��� �������, ���� ��� ������ � ���� ��������������� �����������.
    string word, temp, str; 
    bool firstSentenseShowed = false;
    int stringNumber = 1;
    ifstream fin(fname); 
    if (!fin.is_open()) { cout << "������ �������� �����." << endl; }
    ofstream fout;
    if (!mode) {
        string filePath = (fname);
        filePath = filePath.substr(filePath.find_last_of('\\') + 1);
        filePath = "��������������� ����������� " + filePath;
        ofstream fout1(filePath); fout.open(filePath);
        if (!fout) { cout << "������ �������� �����." << endl; }
    }

    while (getline(fin, str)) {
        if (!str.empty()) {
            str = Utf8ToAnsi(str);
            int firstPos = 0, lastPos = 0, delta;
            while (str.find_first_not_of(" ", lastPos) != -1) {
                firstPos = str.find_first_not_of(" ", lastPos);
                lastPos = str.find_first_of(" ", firstPos);
                delta = lastPos - firstPos;
                word = str.substr(firstPos, delta); // ��������� �����: ��� ���������� � ������� �������, ������� �� ����� �������, �� ���������� ���������� �������.
                if (!mode) { if (!firstSentenseShowed && stringNumber == 1) { fout << "1) "; firstSentenseShowed = true; } }
                if (!mode) fout << word << " ";
                if (separation(word)) {                                                                                     // ������� ���������� �����������
                    word = checkSymbol(word); word = unification(word); 
                    word = to_string(stringNumber) + "'" + word;                                                            // ��������� � ����� ����� �����������, ���� ��� �������� ��������� � �����������, ��� �������������.
                    stringNumber++;
                    if (!mode) { fout << endl << stringNumber << ") "; }
                }
                else { word = checkSymbol(word); word = unification(word); word = to_string(stringNumber) + "'" + word; }   // ��������� � ����� ����� �����������, � ������� ��� ����������, ��� �������������.
                if (!word.empty() && (mode)) {
                    if ((adverb.empty() || find(adverb.begin(), adverb.end(), word) == adverb.end()) && (determinePartOfSpeech(numberOrAdverb(word, "word")))) { adverb.push_back(word);  } // ���� ����� �������� �������� - ������� ��� � ������ �������
                }
            }
        }
    }
    fin.close();
    fout.close();
}


bool determinePartOfSpeech(string word) { // ������� ��� ����������� �������.
    bool adverb = false;
    if (word.size() <= 3) { return false; }
    for (string str : exceptions) { if (word == str) { return true; } }
    if (word.size() > 3 && (word.substr(0, 3) == "��-")) { return true; }
    if (word.size() > 3 && (word.substr(word.size() - 3) == "-��")) { return false; }
    if (word.size() >= 4) {
        for (string str : vowels) { if (word.substr(word.size() - str.size()) == str) { adverb = true; } }
        for (string str : adjectivEendings) { if (word.substr(word.size() - str.size()) == str) { adverb = false; } }
        for (string str : verbEendings) { if (word.substr(word.size() - str.size()) == str) { adverb = false; } }
    }
    return adverb;
}



void doubleRhymeList(vector<string>& adverb, vector<string>& rhyme) {   // ������� ��� ������ ����������� �������.
    int  wordCount = 1; // ������� ������� ����������� �������
    mergeDuplicates(adverb);
    for (int i = 0; i < adverb.size(); ++i) { // ���������� �� ������� ������� �, ���� ��� ���������, ���������� � ������ ����������� �������
        for (int j = i + 1; j < adverb.size(); ++j) {
            if (isRhyme(unification(adverb[i]), unification(adverb[j]))) {
                if (wordCount == 1) rhyme.push_back(adverb[i]);
                rhyme.push_back(adverb[j]);
                adverb.erase(adverb.begin() + j);
                j--;
                wordCount++;

            }
        }
        wordCount = 1;
        rhyme.push_back(" ");
    }
}

int countWordPairs(string str) { // ������� ��� �������� ���������� ��� ���� � ������� ����������� �������.
    istringstream stream(str);
    string word;
    int wordCount = 0;
    while (stream >> word) { wordCount++; }
    return wordCount * (wordCount - 1) / 2;
}

void doubleRhymeToTextBox(vector<string>& rhyme, RichTextBox^ richTextBox, int* rhymeCount) {   // ������� ��� ��������� ���� � ������ �� � RichTextBox � ���������� ����������� �������.
    string currentLine, colorLine;
    int rhymeTypeNumber = 0; // ����� ���� �����, ������������ ��� ������ �����

    // ���������� ��� �������� ������� ����
    for (const auto& element : rhyme) {
        if (element == " ") {
            // ���� ������� �������� ��������, ��� �������� ����� ������� ������ �����
            if (!currentLine.empty()) {
                *rhymeCount += countWordPairs(currentLine); // ������������ ���������� ��� ���� � ������� ������
                colorLine = colors[rhymeTypeNumber]; // �������� ���� ��� ���������
                highlightWordsInRichTextBox(richTextBox, currentLine, colorLine); // �������� ����� � RichTextBox
                rhymeTypeNumber++; // ����������� ����� ���� �����
                if (rhymeTypeNumber == 54) { rhymeTypeNumber = 0; } // ���������� ����� ���� �����, ���� �� ������ ������������� ��������
                currentLine.clear(); // �������� ������� ������
            }
        }
        else {
            // ���� ������� �� �������� ��������, ��������� ��� � ������� ������
            if (!currentLine.empty()) {
                currentLine += " "; // ��������� ������ ����� ����� ���������
            }
            currentLine += numberOrAdverb(element, "word"); // ��������� ������� � ������
        }
    }

    // ����� ��������� ������, ���� ��� �� ������
    if (!currentLine.empty()) {
        *rhymeCount += countWordPairs(currentLine); // ������������ ���������� ��� ���� � ��������� ������
        colorLine = colors[rhymeTypeNumber]; // �������� ���� ��� ���������
        highlightWordsInRichTextBox(richTextBox, currentLine, colorLine); // �������� ����� � RichTextBox
        rhymeTypeNumber++; // ����������� ����� ���� �����
        if (rhymeTypeNumber == 54) { rhymeTypeNumber = 0; } // ���������� ����� ���� �����, ���� �� ������ ������������� ��������
        currentLine.clear(); // �������� ������� ������
    }
}

void rhymePairsToFile(vector<string>& rhyme, string filePath, int rhymeCount) { // ������� ������������� ��� ������ ��� ���� �� ������� ����� � ����. 
    ofstream outputFile(filePath, ios::trunc); // ��������� ���� ��� ������ � ������ ����������
    outputFile << "���������� ��� ����: " << to_string(rhymeCount) << endl << endl; // ���������� ���������� ��� ����

    vector<string> sortedAdverbs = rhyme;
    string str;
    sortedAdverbs.erase(remove(sortedAdverbs.begin(), sortedAdverbs.end(), " "), sortedAdverbs.end());
    bool swapped;
    do {                                            // ���������� ���������
        swapped = false;
        for (size_t i = 0; i < sortedAdverbs.size() - 1; ++i) {
            if (numberOrAdverb(sortedAdverbs[i], "word") > numberOrAdverb(sortedAdverbs[i + 1], "word")) {      // ���������� ����� � ������ �������, ���� ��� ��������� � ������������ �������
                swap(sortedAdverbs[i], sortedAdverbs[i + 1]); swapped = true;
            }
        }
    } while (swapped);
    for (auto& adverb : sortedAdverbs) {        // �������� �� ���������������� ������� ����
        int meeting�ount = 1;
        auto it = find(rhyme.begin(), rhyme.end(), adverb);
        if (it != rhyme.end()) {                 // ������� ������ � ����� ������ ����
            auto start = it;
            while (start != rhyme.begin() && *start != " ") { --start; }
            if (*start == " ") { ++start; }
            auto end = it;
            while (end != rhyme.end() && *end != " ") { ++end; }
            for (char ch : numberOrAdverb(adverb, "number")) { if (ch == ',') { meeting�ount++; } }             // ������� ���������� ������ �����
            if (numberOrAdverb(adverb, "number").find(',') != string::npos) {                                   // ��������� ������ ��� ������ � ����
                str = "����� ��� ����� '" + numberOrAdverb(adverb, "word") + "'. ���������� ������ �����: " + to_string(meeting�ount) + ". ����� '" + numberOrAdverb(adverb, "word") + "' ����������� � ������������: " + numberOrAdverb(adverb, "number");
            }
            else str = "����� ��� ����� '" + numberOrAdverb(adverb, "word") + "'. ���������� ������ �����: 1. ����� '" + numberOrAdverb(adverb, "word") + "' ����������� � �����������: " + numberOrAdverb(adverb, "number");
            outputFile << endl << str << endl;
            for (auto rhymeIt = start; rhymeIt != end; ++rhymeIt) {                                             // ���������� ���� ���� ��� ������� �����
                meeting�ount = 1;
                if (numberOrAdverb(*rhymeIt, "word") != numberOrAdverb(adverb, "word")) {
                    int size = 20 - (numberOrAdverb(*rhymeIt, "word")).length();
                    string padding(size, ' ');
                    for (char ch : numberOrAdverb(*rhymeIt, "number")) { if (ch == ',') { meeting�ount++; } }   // ������� ���������� ������ �����
                    if (numberOrAdverb(*rhymeIt, "number").find(',') != string::npos) {
                        str = "- '" + numberOrAdverb(*rhymeIt, "word") + "'" + padding + "���������� ������ �����: " + to_string(meeting�ount) + ". ����� '" + numberOrAdverb(*rhymeIt, "word") + "' ����������� � ������������: " + numberOrAdverb(*rhymeIt, "number");
                    }
                    else { str = "- '" + numberOrAdverb(*rhymeIt, "word") + "'" + padding + "���������� ������ �����: 1. ����� '" + numberOrAdverb(*rhymeIt, "word") + "' ����������� � �����������: " + numberOrAdverb(*rhymeIt, "number"); }
                    outputFile << str << endl;
                }
            }
        }
    }
    outputFile.close();    // ��������� ����
}


void highlightWordsInRichTextBox(RichTextBox^ richTextBox, string wordsToHighlight,string colorStr) { // ������� ��� ��������� ����������� ������� �������.
    // ����������� ������ wordsToHighlight � ����� ����� ��� �������� ������
    unordered_set<string> wordSet;
    size_t start = 0;
    size_t end = wordsToHighlight.find(' ');
    while (end != string::npos) {
        string word = wordsToHighlight.substr(start, end - start);
        transform(word.begin(), word.end(), word.begin(), ::tolower); // ����������� � ������ �������
        wordSet.insert(word);
        start = end + 1;
        end = wordsToHighlight.find(' ', start);
    }
    string word = wordsToHighlight.substr(start);
    transform(word.begin(), word.end(), word.begin(), ::tolower); // ����������� � ������ �������
    wordSet.insert(word);

    // ����������� ������ ����� � ������ Color
    cli::array<String^>^ colorParts = (gcnew String(colorStr.c_str()))->Split(',');
    int r = Int32::Parse(colorParts[0]);
    int g = Int32::Parse(colorParts[1]);
    int b = Int32::Parse(colorParts[2]);
    Color highlightColor = Color::FromArgb(r, g, b);

    // �������� ����� �� RichTextBox
    String^ text = richTextBox->Text;

    // �������� �� ������ � �������� �����
    int startIndex = 0;
    int length = text->Length;
    while (startIndex < length) {
        // ������� ������ �����
        while (startIndex < length && !Char::IsLetterOrDigit(text[startIndex])) {
            startIndex++;
        }
        if (startIndex >= length) break;

        // ������� ����� �����
        int endIndex = startIndex + 1;
        while (endIndex < length && Char::IsLetterOrDigit(text[endIndex])) {
            endIndex++;
        }

        // ��������� �����
        String^ word = text->Substring(startIndex, endIndex - startIndex);
        string nativeWord = marshal_as<std::string>(word); // ����������� � std::string
        nativeWord = unification(nativeWord); // ����������� � ������ �������

        // ���������, ���� �� ����� � ������
        if (wordSet.find(nativeWord) != wordSet.end()) {
            // �������� ����� �������� ������
            richTextBox->Select(startIndex, endIndex - startIndex);
            richTextBox->SelectionColor = highlightColor;
        }

        // ��������� � ���������� �����
        startIndex = endIndex;
    }

    // ���������� ���������
    richTextBox->Select(0, 0);
}

void mergeDuplicates(vector<string>& words) {   // ������� ��� �������� ���������� �� ������� �������.
    for (int i = 0; i < words.size(); ++i) {
        for (int j = i + 1; j < words.size(); ++j) {
            if (numberOrAdverb(words[i], "word") == numberOrAdverb(words[j], "word")) {
                words[i] = numberOrAdverb(words[j], "number") + "," + words[i];         // ��������� ����� ������� ����� � �������
                words.erase(words.begin() + j);                                         // ������� ������ ����� �� �������
                --j;                                                                    // ������������ ������ ����� ��������
            }
        }
    }
}


string Utf8ToAnsi(string utf8String) { // ������� ��� �������������� ������ �� ��������� UTF-8 � ��������� ANSI.
    // �������������� UTF-8 � ������� ������� (wchar_t)
    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
    if (wideCharCount == 0) {
        std::cerr << "������ �������������� UTF-8 � ������� �������." << std::endl;
        return "";
    }

    std::wstring wideString(wideCharCount, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, &wideString[0], wideCharCount);

    // �������������� ������� �������� � ANSI
    int ansiCharCount = WideCharToMultiByte(1251, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ansiCharCount == 0) {
        std::cerr << "������ �������������� ������� �������� � ANSI." << std::endl;
        return "";
    }

    std::string ansiString(ansiCharCount, '\0');
    WideCharToMultiByte(1251, 0, wideString.c_str(), -1, &ansiString[0], ansiCharCount, nullptr, nullptr);

    return ansiString;
}

string removeConsonants(string input) { // ������� ��� �������� ��������� ���� �� ������.
    const string vowels = "���������"; // ������� ����� �������� ����� 
    string temp = unification(input);
    string result;
    for (char ch : temp) {
        if (vowels.find(ch) != string::npos) { result += ch; }
    }
    if (result.length() > 1) { return result; }
    else return input;
}

string checkSymbol(string pas) { // ��������������� ������� ��� �������� ������������ ������ �� ����������� ������� � � ����������� ��������������.
    string result;
    for (char c : pas) { if ((c >= '�' && c <= '�') || (c >= '�' && c <= '�') || (c == '-')) { result += c; } }
    if (result.empty()) { cout << "����� " << pas << " ��������� �������� �� �������������� �������� � ���� �������. " << endl; }
    return result;
}

string unification(string str) { // ������� ��� ���������� ������ � ������� ��������.
    string result;
    for (char c : str) {
        if ((c >= '�' && c <= '�') || (c == '-')) { result += c; }
        else { if ((c >= '�' && c <= '�')) { c += 32; result += c; } }
    }
    return result;
}

string numberOrAdverb(string str, string mode) { // ������� ��� ������ ������� ���� ������ �����������, � ������� ��� ����������.
    istringstream iss(str);
    string part1, part2;
    // ��������� ������ �� ��� ����� �� �������
    getline(iss, part1, '\'');
    getline(iss, part2);
    // � ����������� �� ������ ���������� ���� �����, ���� �����
    if (mode == "number") { return part1; }
    else { return part2; }
}



bool isRhyme(string word1, string word2) { // ������� ��� ����������� �������� �������� ����.
    bool rhyme = false;
    if (word1 == word2) return false;   // � ������ ���� ����� ���������, ���������� false, ����� �� ��������� ������ ����������� ����.
    if (checkLastThreeLetters(word1, word2, vow, cons)) rhyme = true;
    if ((removeConsonants(word1.substr(word1.size() - 4)) == removeConsonants(word2.substr(word2.size() - 4))) && samePositions(word1.substr(word1.size() - 4), word2.substr(word2.size() - 4))
        && arePair(word1[word1.length() - 2], word2[word2.length() - 2], vow) && arePair(word1[word1.length() - 3], word2[word2.length() - 3], vow)) rhyme = true;
    if (word1.substr(word1.size() - 4) == word2.substr(word2.size() - 4)) rhyme = true;
    if (checkWordWithSameLength(word1, word2, vow)) rhyme = true;

    return rhyme;
}

bool separation(string word) {  // �������  ���������, ������������� �� ������ ���������, ������� ������ ������������ ��� ���������� �����������.
    bool isSeparation = false;
    if (word.length() > 4) {
        if ((word[word.length() - 2] == '.') && (word[word.length() - 3] != '.')) isSeparation = true;            // �������� �� ����� "."
        if ((word[word.length() - 2] == '.') && (word[word.length() - 3] == '.')) isSeparation = true;          // �������� �� ��������� "..."
        if ((word[word.length() - 3] == '!') && (word[word.length() - 2] == '.')) isSeparation = true;          // �������� �� "!.."
        if ((word[word.length() - 3] == '?') && (word[word.length() - 2] == '.')) isSeparation = true;          // �������� �� "?.."
    }
    if (word.length() > 2) {
        if (word[word.length() - 2] == '!') isSeparation = true;          // �������� �� ��������������� ����, ��� ������������� ������ "! "
        if (word[word.length() - 2] == '?') isSeparation = true;          // �������� �� �������������� ����, ��� ������������� ������ "? "
    }
    if ((word[word.length() - 1] == '.') && (word[word.length() - 2] != '.') && (word.length() > 3)) isSeparation = true;   // �������� �� ����� "."
    if (word[word.length() - 1] == '!') isSeparation = true;          // �������� �� ��������������� ����, ��� ��������� ������ "!"
    if (word[word.length() - 1] == '?') isSeparation = true;          // �������� �� �������������� ����, ��� ��������� ������ "?"
    return isSeparation;
}

bool isVowel(char c) { // ������� ��� �������� ������� �� �������������� � �������.
    const string vowels = "���������"; // ������� ����� �������� �����
    return vowels.find(c) != string::npos;
}

bool samePositions(string word1, string word2) {  // ������� ��� �������� ���������� ������� ������� ���� � ���� ������.
    if (word1.length() != word2.length()) { return false; }
    for (size_t i = 0; i < word1.length(); ++i) {
        bool vowel1 = isVowel(word1[i]);
        bool vowel2 = isVowel(word2[i]);
        if (vowel1 != vowel2) return false;
    }
    return true;
}

bool arePair(char a, char b, const vector<pair<char, char>>& pairs) {   // ������� ��� ��������, �������� �� ��� ����� �������.
    for (const auto& pair : pairs) {
        if ((a == pair.first && b == pair.second) || (a == pair.second && b == pair.first) || (a == b)) {
            return true;
        }
    }
    return false;
}

bool checkWordWithSameLength(string word1, string word2, const vector<pair<char, char>>& vow) { // ������� ��� �������� �� �������� �������� ���� � ���������� �������.
    bool check=false;
    // ���� ����� ����� ���������� ����� � ����� � ������ �������� �������, �� ����� ���������
    if (word1.length() == word2.length()) {
        if (samePositions(word1, word2)) {
            if (removeConsonants(word1).length() == removeConsonants(word2).length()) {
                for (int i = 0; i < removeConsonants(word1).length(); i++) {
                    if (!arePair(removeConsonants(word1)[i], removeConsonants(word2)[i], vow)) check = true;
                    else check = false;
                }
            }
        }
    }
    return check;
}

bool checkLastThreeLetters(string word1, string word2, const vector<pair<char, char>>& vow, const vector<pair<char, char>>& cons) { // ������� ��� �������� ����������� ��������� ���� ���� ���� ����.
    if (word1.length() < 2 || word2.length() < 2) return false; // ���� ���� �� ���� ������ ���� ����, ���������� false

    char last1 = word1[word1.length() - 1];
    char last2 = word2[word2.length() - 1];
    char secondLast1 = word1[word1.length() - 2];
    char secondLast2 = word2[word2.length() - 2];
    char thirdLast1 = word1[word1.length() - 3];
    char thirdLast2 = word2[word2.length() - 3];
    char fourdLast1 = word1[word1.length() - 4];
    char fourdLast2 = word2[word2.length() - 4];


    // ��������� ��������� �����
    bool lastLettersMatch = (last1 == last2) || arePair(last1, last2, vow) || arePair(last1, last2, cons);
    // ��������� ������������� �����
    bool secondLastLettersMatch = (secondLast1 == secondLast2) || arePair(secondLast1, secondLast2, vow) || arePair(secondLast1, secondLast2, cons);
    // ��������� ����������������� �����
    bool thirdLastLettersMatch = (thirdLast1 == thirdLast2) || arePair(thirdLast1, thirdLast2, vow) || arePair(thirdLast1, thirdLast2, cons);
    return lastLettersMatch && secondLastLettersMatch && thirdLastLettersMatch;
}


