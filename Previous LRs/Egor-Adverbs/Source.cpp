#include "Header.h" 

vector<string> exceptions = {
    "долой", "домой" , "наперебой" ,"порой", "украдкой" ,"вдвое", "втрое", "надвое", "более",
    "далее", "заранее","менее", "наиболее", "наименее", "почти"
    "позднее", "ранее", "скорее" ,"невзирая", "впервые", "дорого", "итого",
    "намного", "недорого", "некого", "немного", "строго",
    "нечего", "вплотную", "вслепую", "впустую", "вручную", "зачастую", "напрямую", "более",
    //слова с окончанием "ому"/"ему" не записаны, т.к. все начинаются с "по-", что является признаком наречия. 
    //слова с окончанием "ом", "ем", не внесены в исключения, из-за их большого количества. 
    //свеху представлены исключения, для окончаний схожих с окончаниями прилагательных 
    "напролет", "незачем", "затем", "замужем", "живьем", "днем", "втроем", "вдвоем", "вчетвером",
    "видать", "вплоть", "вспять", "насмерть", "наизусть", "опять", "чуть", "тяжело", "около", "бегло",
    "весело", "всецело", "вяло", "мало", "мило", "наголо", "нагло", "назло", "немало", "светло",
    "смело", "тепло", "тускло", "умело"
    //свеху представлены исключения, для окончаний схожих с окончаниями глаголов 
    "бегом", "битком" , "босиком" ,"вверх", "вбок" , "вниз", "вглубь", "вдаль" ,"вдобавок",
    "вдоволь", "вдоль" ,"вдруг", "взад", "вконец", "вмиг", "вновь", "внутрь", "вокруг",
    "вперед" , "вплоть", "впредь", "впрок", "впрямь" , "враз" ,"врасплох", "всерьез",
    "вскользь", "вслед" ,"вслух", "замуж" , "затем", "кругом", "мельком", "набок",
    "назад", "наотрез", "наперед", "напоказ" , "наотрез", "наперед", "напоследок", "напролет",
    "напрочь", "насквозь", "наспех", "настежь" , "наугад", "невпопад", "пешком", "поверх",
    "подряд", "пополам", "прямиком", "чересчур",
    //свеху представлены исключения, для окончаний с согласными буквами 
    "ещё", "уже", "зря", "еле", "наперебой", "невзначай", "порой", "украдкой",
    "зачем", "потом", "как", "тогда"//, "наперебой", "невзначай", "порой", "украдкой"
};
vector<string> adjectivEendings = {
    "ый", "ий", "ой", "ое", "ее", "ая", "яя", "ые", "ие", "ом", "ем", "ыми", "ими",
    "ого", "его", "ей", "ых" , "их", "ому", "ему", "ым", "им", "ую" , "юю", "ых",
    "ье", "ью", "ья", "ья"
};
vector<string> verbEendings = {
    "ешь", "ет", "ем", "ете" , "ут", "ют", "ишь", "ит",
    "им", "ите", "ат", "ят", "ть", "ться", "тся", "ся",
    "ло"
};
vector<string> vowels = {
    "ну", "му", "о", /*"е", "а", "и", "э", "я", "ю", "ы"*/
};

const vector<pair<char, char>> vow = { { 'а', 'я'}, { 'и', 'ы' },
        { 'у', 'ю' }, { 'ы', 'и' }, { 'ю', 'у' }, { 'я', 'а' } , { 'у', 'о'} , {'о','у'}};

const vector<pair<char, char>> cons = { { 'б', 'п'}, { 'в', 'ф'}, { 'г', 'к' }, { 'д', 'т' },
    { 'ж', 'ш' }, { 'з', 'с' },  { 'к', 'г' },
    { 'п', 'б'},  { 'с', 'з' },{ 'т', 'д' }, { 'ф', 'в' },  { 'ш', 'ж' } };

vector<string> colors = { //54 ЦВЕТА
    "255,0,0",    // Красный
    "0,255,0",    // Зеленый
    "0,0,255",    // Синий
    "255,0,255",  // Фиолетовый
    "0,255,255",  // Голубой
    "128,0,0",    // Темно-красный
    "0,128,0",    // Темно-зеленый
    "0,0,128",    // Темно-синий
    "128,128,0",  // Оливковый
    "128,0,128",  // Темно-фиолетовый
    "0,128,128",  // Темно-голубой
    "255,165,0",  // Оранжевый
    "255,69,0",   // Оранжево-красный
    "210,180,140",// Бежевый
    "184,134,11", // Темно-оливковый
    "102,205,170",// Средне-аквамариновый
    "186,85,211", // Средне-фиолетовый
    "148,0,211",  // Темно-фиолетовый
    "75,0,130",   // Индиго
    "233,150,122",// Темно-салатовый
    "144,238,144",// Светло-зеленый
    "175,238,238",// Светло-голубой
    "211,211,211",// Светло-серый
    "255,215,0",  // Золотой
    "238,130,238",// Виолетовый
    "220,220,220",// Светло-серый
    "244,164,96", // Светло-оранжевый
    "210,105,30", // Темно-оранжевый
    "222,184,135",// Бледно-золотой
    "188,143,143",// Розовый
    "250,128,114",// Салатовый
    "240,255,240",// Бледно-зеленый
    "230,230,250",// Лавандовый
    "255,240,245",// Бледно-розовый
    "139,69,19",  // Коричневый
    "255,228,225",// Миндальный
    "245,222,179",// Пшеничный
    "240,230,140",// Хаки
    "245,245,220",// Бежевый
    "255,248,220",// Кукурузный
    "240,255,255",// Аквамариновый
    "240,255,240",// Бледно-зеленый
    "255,250,205",// Лимонный шифон
    "175,238,238",// Бледно-голубой
    "244,164,96", // Светло-оранжевый
    "255,228,225",// Миндальный
    "255,20,147", // Розово-фиолетовый
    "210,180,140",// Темно-бежевый
    "255,222,173",// Навахо белый
    "255,250,240",// Слоновая кость
    "222,184,135",// Бурливуд
    "255,228,196",// Бисквит
    "255,245,238",// Морская ракушка
    "245,255,250",// Мятно-кремовый
    "240,255,255",// Азур
};



void inputFromFile(const char* fname, vector<string>& adverb, bool mode) {  // Функция для считывания текста из файла и поиска в нем наречий, либо для записи в файл пронумерованных предложений.
    string word, temp, str; 
    bool firstSentenseShowed = false;
    int stringNumber = 1;
    ifstream fin(fname); 
    if (!fin.is_open()) { cout << "Ошибка открытия файла." << endl; }
    ofstream fout;
    if (!mode) {
        string filePath = (fname);
        filePath = filePath.substr(filePath.find_last_of('\\') + 1);
        filePath = "Пронумерованные предложения " + filePath;
        ofstream fout1(filePath); fout.open(filePath);
        if (!fout) { cout << "Ошибка открытия файла." << endl; }
    }

    while (getline(fin, str)) {
        if (!str.empty()) {
            str = Utf8ToAnsi(str);
            int firstPos = 0, lastPos = 0, delta;
            while (str.find_first_not_of(" ", lastPos) != -1) {
                firstPos = str.find_first_not_of(" ", lastPos);
                lastPos = str.find_first_of(" ", firstPos);
                delta = lastPos - firstPos;
                word = str.substr(firstPos, delta); // Формируем слово: оно начинается с первого символа, который не равен пробелу, до следующего ближайшего пробела.
                if (!mode) { if (!firstSentenseShowed && stringNumber == 1) { fout << "1) "; firstSentenseShowed = true; } }
                if (!mode) fout << word << " ";
                if (separation(word)) {                                                                                     // Считаем количество предложений
                    word = checkSymbol(word); word = unification(word); 
                    word = to_string(stringNumber) + "'" + word;                                                            // Добавляем к слову номер предложения, если оно является последним в предложении, для идентификации.
                    stringNumber++;
                    if (!mode) { fout << endl << stringNumber << ") "; }
                }
                else { word = checkSymbol(word); word = unification(word); word = to_string(stringNumber) + "'" + word; }   // Добавляем к слову номер предложения, в котором оно содержится, для идентификации.
                if (!word.empty() && (mode)) {
                    if ((adverb.empty() || find(adverb.begin(), adverb.end(), word) == adverb.end()) && (determinePartOfSpeech(numberOrAdverb(word, "word")))) { adverb.push_back(word);  } // Если слово является наречием - заносим его в вектор наречий
                }
            }
        }
    }
    fin.close();
    fout.close();
}


bool determinePartOfSpeech(string word) { // Функция для определения наречий.
    bool adverb = false;
    if (word.size() <= 3) { return false; }
    for (string str : exceptions) { if (word == str) { return true; } }
    if (word.size() > 3 && (word.substr(0, 3) == "по-")) { return true; }
    if (word.size() > 3 && (word.substr(word.size() - 3) == "-то")) { return false; }
    if (word.size() >= 4) {
        for (string str : vowels) { if (word.substr(word.size() - str.size()) == str) { adverb = true; } }
        for (string str : adjectivEendings) { if (word.substr(word.size() - str.size()) == str) { adverb = false; } }
        for (string str : verbEendings) { if (word.substr(word.size() - str.size()) == str) { adverb = false; } }
    }
    return adverb;
}



void doubleRhymeList(vector<string>& adverb, vector<string>& rhyme) {   // Функция для поиска рифмующихся наречий.
    int  wordCount = 1; // Счетчик цепочек рифмующихся наречий
    mergeDuplicates(adverb);
    for (int i = 0; i < adverb.size(); ++i) { // Проходимся по вектору наречий и, если они рифмуется, записываем в вектор рифмующихся наречий
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

int countWordPairs(string str) { // Функция для подсчета количества пар рифм в цепочке рифмующихся наречий.
    istringstream stream(str);
    string word;
    int wordCount = 0;
    while (stream >> word) { wordCount++; }
    return wordCount * (wordCount - 1) / 2;
}

void doubleRhymeToTextBox(vector<string>& rhyme, RichTextBox^ richTextBox, int* rhymeCount) {   // Функция для обработки рифм и вывода их в RichTextBox с выделением рифмующихся наречий.
    string currentLine, colorLine;
    int rhymeTypeNumber = 0; // Номер типа рифмы, используется для выбора цвета

    // Перебираем все элементы вектора рифм
    for (const auto& element : rhyme) {
        if (element == " ") {
            // Если элемент является пробелом, это означает конец текущей строки рифмы
            if (!currentLine.empty()) {
                *rhymeCount += countWordPairs(currentLine); // Подсчитываем количество пар слов в текущей строке
                colorLine = colors[rhymeTypeNumber]; // Выбираем цвет для выделения
                highlightWordsInRichTextBox(richTextBox, currentLine, colorLine); // Выделяем слова в RichTextBox
                rhymeTypeNumber++; // Увеличиваем номер типа рифмы
                if (rhymeTypeNumber == 54) { rhymeTypeNumber = 0; } // Сбрасываем номер типа рифмы, если он достиг максимального значения
                currentLine.clear(); // Обнуляем текущую строку
            }
        }
        else {
            // Если элемент не является пробелом, добавляем его в текущую строку
            if (!currentLine.empty()) {
                currentLine += " "; // Добавляем пробел перед новым элементом
            }
            currentLine += numberOrAdverb(element, "word"); // Добавляем элемент в строку
        }
    }

    // Вывод последней строки, если она не пустая
    if (!currentLine.empty()) {
        *rhymeCount += countWordPairs(currentLine); // Подсчитываем количество пар слов в последней строке
        colorLine = colors[rhymeTypeNumber]; // Выбираем цвет для выделения
        highlightWordsInRichTextBox(richTextBox, currentLine, colorLine); // Выделяем слова в RichTextBox
        rhymeTypeNumber++; // Увеличиваем номер типа рифмы
        if (rhymeTypeNumber == 54) { rhymeTypeNumber = 0; } // Сбрасываем номер типа рифмы, если он достиг максимального значения
        currentLine.clear(); // Обнуляем текущую строку
    }
}

void rhymePairsToFile(vector<string>& rhyme, string filePath, int rhymeCount) { // Функция предназначена для записи пар рифм из вектора строк в файл. 
    ofstream outputFile(filePath, ios::trunc); // Открываем файл для записи в режиме добавления
    outputFile << "Количество пар рифм: " << to_string(rhymeCount) << endl << endl; // Записываем количество пар рифм

    vector<string> sortedAdverbs = rhyme;
    string str;
    sortedAdverbs.erase(remove(sortedAdverbs.begin(), sortedAdverbs.end(), " "), sortedAdverbs.end());
    bool swapped;
    do {                                            // Сортировка пузырьком
        swapped = false;
        for (size_t i = 0; i < sortedAdverbs.size() - 1; ++i) {
            if (numberOrAdverb(sortedAdverbs[i], "word") > numberOrAdverb(sortedAdverbs[i + 1], "word")) {      // Сравниваем слова и меняем местами, если они находятся в неправильном порядке
                swap(sortedAdverbs[i], sortedAdverbs[i + 1]); swapped = true;
            }
        }
    } while (swapped);
    for (auto& adverb : sortedAdverbs) {        // Проходим по отсортированному вектору рифм
        int meetingСount = 1;
        auto it = find(rhyme.begin(), rhyme.end(), adverb);
        if (it != rhyme.end()) {                 // Находим начало и конец группы рифм
            auto start = it;
            while (start != rhyme.begin() && *start != " ") { --start; }
            if (*start == " ") { ++start; }
            auto end = it;
            while (end != rhyme.end() && *end != " ") { ++end; }
            for (char ch : numberOrAdverb(adverb, "number")) { if (ch == ',') { meetingСount++; } }             // Считаем количество встреч слова
            if (numberOrAdverb(adverb, "number").find(',') != string::npos) {                                   // Формируем строку для записи в файл
                str = "Рифмы для слова '" + numberOrAdverb(adverb, "word") + "'. Количество встреч слова: " + to_string(meetingСount) + ". Слово '" + numberOrAdverb(adverb, "word") + "' встречается в предложениях: " + numberOrAdverb(adverb, "number");
            }
            else str = "Рифмы для слова '" + numberOrAdverb(adverb, "word") + "'. Количество встреч слова: 1. Слово '" + numberOrAdverb(adverb, "word") + "' встречается в предложении: " + numberOrAdverb(adverb, "number");
            outputFile << endl << str << endl;
            for (auto rhymeIt = start; rhymeIt != end; ++rhymeIt) {                                             // Записываем пары рифм для каждого слова
                meetingСount = 1;
                if (numberOrAdverb(*rhymeIt, "word") != numberOrAdverb(adverb, "word")) {
                    int size = 20 - (numberOrAdverb(*rhymeIt, "word")).length();
                    string padding(size, ' ');
                    for (char ch : numberOrAdverb(*rhymeIt, "number")) { if (ch == ',') { meetingСount++; } }   // Считаем количество встреч слова
                    if (numberOrAdverb(*rhymeIt, "number").find(',') != string::npos) {
                        str = "- '" + numberOrAdverb(*rhymeIt, "word") + "'" + padding + "Количество встреч слова: " + to_string(meetingСount) + ". Слово '" + numberOrAdverb(*rhymeIt, "word") + "' встречается в предложениях: " + numberOrAdverb(*rhymeIt, "number");
                    }
                    else { str = "- '" + numberOrAdverb(*rhymeIt, "word") + "'" + padding + "Количество встреч слова: 1. Слово '" + numberOrAdverb(*rhymeIt, "word") + "' встречается в предложении: " + numberOrAdverb(*rhymeIt, "number"); }
                    outputFile << str << endl;
                }
            }
        }
    }
    outputFile.close();    // Закрываем файл
}


void highlightWordsInRichTextBox(RichTextBox^ richTextBox, string wordsToHighlight,string colorStr) { // Функция для выделения рифмующихся наречий цветами.
    // Преобразуем строку wordsToHighlight в набор строк для быстрого поиска
    unordered_set<string> wordSet;
    size_t start = 0;
    size_t end = wordsToHighlight.find(' ');
    while (end != string::npos) {
        string word = wordsToHighlight.substr(start, end - start);
        transform(word.begin(), word.end(), word.begin(), ::tolower); // Преобразуем в нижний регистр
        wordSet.insert(word);
        start = end + 1;
        end = wordsToHighlight.find(' ', start);
    }
    string word = wordsToHighlight.substr(start);
    transform(word.begin(), word.end(), word.begin(), ::tolower); // Преобразуем в нижний регистр
    wordSet.insert(word);

    // Преобразуем строку цвета в объект Color
    cli::array<String^>^ colorParts = (gcnew String(colorStr.c_str()))->Split(',');
    int r = Int32::Parse(colorParts[0]);
    int g = Int32::Parse(colorParts[1]);
    int b = Int32::Parse(colorParts[2]);
    Color highlightColor = Color::FromArgb(r, g, b);

    // Получаем текст из RichTextBox
    String^ text = richTextBox->Text;

    // Проходим по тексту и выделяем слова
    int startIndex = 0;
    int length = text->Length;
    while (startIndex < length) {
        // Находим начало слова
        while (startIndex < length && !Char::IsLetterOrDigit(text[startIndex])) {
            startIndex++;
        }
        if (startIndex >= length) break;

        // Находим конец слова
        int endIndex = startIndex + 1;
        while (endIndex < length && Char::IsLetterOrDigit(text[endIndex])) {
            endIndex++;
        }

        // Извлекаем слово
        String^ word = text->Substring(startIndex, endIndex - startIndex);
        string nativeWord = marshal_as<std::string>(word); // Преобразуем в std::string
        nativeWord = unification(nativeWord); // Преобразуем в нижний регистр

        // Проверяем, есть ли слово в наборе
        if (wordSet.find(nativeWord) != wordSet.end()) {
            // Выделяем слово заданным цветом
            richTextBox->Select(startIndex, endIndex - startIndex);
            richTextBox->SelectionColor = highlightColor;
        }

        // Переходим к следующему слову
        startIndex = endIndex;
    }

    // Сбрасываем выделение
    richTextBox->Select(0, 0);
}

void mergeDuplicates(vector<string>& words) {   // Функция для удаления дубликатов из вектора наречий.
    for (int i = 0; i < words.size(); ++i) {
        for (int j = i + 1; j < words.size(); ++j) {
            if (numberOrAdverb(words[i], "word") == numberOrAdverb(words[j], "word")) {
                words[i] = numberOrAdverb(words[j], "number") + "," + words[i];         // Добавляем номер второго слова к первому
                words.erase(words.begin() + j);                                         // Удаляем второе слово из вектора
                --j;                                                                    // Корректируем индекс после удаления
            }
        }
    }
}


string Utf8ToAnsi(string utf8String) { // Функция для преобразования строки из кодировки UTF-8 в кодировку ANSI.
    // Преобразование UTF-8 в широкие символы (wchar_t)
    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
    if (wideCharCount == 0) {
        std::cerr << "Ошибка преобразования UTF-8 в широкие символы." << std::endl;
        return "";
    }

    std::wstring wideString(wideCharCount, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, &wideString[0], wideCharCount);

    // Преобразование широких символов в ANSI
    int ansiCharCount = WideCharToMultiByte(1251, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ansiCharCount == 0) {
        std::cerr << "Ошибка преобразования широких символов в ANSI." << std::endl;
        return "";
    }

    std::string ansiString(ansiCharCount, '\0');
    WideCharToMultiByte(1251, 0, wideString.c_str(), -1, &ansiString[0], ansiCharCount, nullptr, nullptr);

    return ansiString;
}

string removeConsonants(string input) { // Функция для удаления согласных букв из строки.
    const string vowels = "аеёиоуыэюя"; // Гласные буквы русского языка 
    string temp = unification(input);
    string result;
    for (char ch : temp) {
        if (vowels.find(ch) != string::npos) { result += ch; }
    }
    if (result.length() > 1) { return result; }
    else return input;
}

string checkSymbol(string pas) { // Вспомогательная функция для проверки передаваемой строки на специальные символы и её дальнейшего форматирования.
    string result;
    for (char c : pas) { if ((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') || (c == '-')) { result += c; } }
    if (result.empty()) { cout << "Слово " << pas << " полностью состояло из непредвиденных символов и было удалено. " << endl; }
    return result;
}

string unification(string str) { // Функция для приведения строки к нижнему регистру.
    string result;
    for (char c : str) {
        if ((c >= 'а' && c <= 'я') || (c == '-')) { result += c; }
        else { if ((c >= 'А' && c <= 'Я')) { c += 32; result += c; } }
    }
    return result;
}

string numberOrAdverb(string str, string mode) { // Функция для вывода наречия либо номера предложения, в котором оно содержится.
    istringstream iss(str);
    string part1, part2;
    // Разделяем строку на две части по запятой
    getline(iss, part1, '\'');
    getline(iss, part2);
    // В зависимости от режима возвращаем либо номер, либо слово
    if (mode == "number") { return part1; }
    else { return part2; }
}



bool isRhyme(string word1, string word2) { // Функция для определения взаимной рифмовки слов.
    bool rhyme = false;
    if (word1 == word2) return false;   // В случае если слова идентичны, возвращаем false, чтобы не заполнять вектор дубликатами слов.
    if (checkLastThreeLetters(word1, word2, vow, cons)) rhyme = true;
    if ((removeConsonants(word1.substr(word1.size() - 4)) == removeConsonants(word2.substr(word2.size() - 4))) && samePositions(word1.substr(word1.size() - 4), word2.substr(word2.size() - 4))
        && arePair(word1[word1.length() - 2], word2[word2.length() - 2], vow) && arePair(word1[word1.length() - 3], word2[word2.length() - 3], vow)) rhyme = true;
    if (word1.substr(word1.size() - 4) == word2.substr(word2.size() - 4)) rhyme = true;
    if (checkWordWithSameLength(word1, word2, vow)) rhyme = true;

    return rhyme;
}

bool separation(string word) {  // Функция  проверяет, заканчивается ли строка символами, которые обычно используются для разделения предложений.
    bool isSeparation = false;
    if (word.length() > 4) {
        if ((word[word.length() - 2] == '.') && (word[word.length() - 3] != '.')) isSeparation = true;            // Проверка на точку "."
        if ((word[word.length() - 2] == '.') && (word[word.length() - 3] == '.')) isSeparation = true;          // Проверка на троеточие "..."
        if ((word[word.length() - 3] == '!') && (word[word.length() - 2] == '.')) isSeparation = true;          // Проверка на "!.."
        if ((word[word.length() - 3] == '?') && (word[word.length() - 2] == '.')) isSeparation = true;          // Проверка на "?.."
    }
    if (word.length() > 2) {
        if (word[word.length() - 2] == '!') isSeparation = true;          // Проверка на восклицательный знак, как предпоследний символ "! "
        if (word[word.length() - 2] == '?') isSeparation = true;          // Проверка на вопросительный знак, как предпоследний символ "? "
    }
    if ((word[word.length() - 1] == '.') && (word[word.length() - 2] != '.') && (word.length() > 3)) isSeparation = true;   // Проверка на точку "."
    if (word[word.length() - 1] == '!') isSeparation = true;          // Проверка на восклицательный знак, как последний символ "!"
    if (word[word.length() - 1] == '?') isSeparation = true;          // Проверка на вопросительный знак, как последний символ "?"
    return isSeparation;
}

bool isVowel(char c) { // Функция для проверки символа на принадлежность к гласным.
    const string vowels = "аеёиоуыэюя"; // Гласные буквы русского языка
    return vowels.find(c) != string::npos;
}

bool samePositions(string word1, string word2) {  // Функция для проверки совпадения позиций гласных букв в двух словах.
    if (word1.length() != word2.length()) { return false; }
    for (size_t i = 0; i < word1.length(); ++i) {
        bool vowel1 = isVowel(word1[i]);
        bool vowel2 = isVowel(word2[i]);
        if (vowel1 != vowel2) return false;
    }
    return true;
}

bool arePair(char a, char b, const vector<pair<char, char>>& pairs) {   // Функция для проверки, являются ли две буквы парными.
    for (const auto& pair : pairs) {
        if ((a == pair.first && b == pair.second) || (a == pair.second && b == pair.first) || (a == b)) {
            return true;
        }
    }
    return false;
}

bool checkWordWithSameLength(string word1, string word2, const vector<pair<char, char>>& vow) { // Функция для проверки на взаимную рифмовку слов с одинаковой длинной.
    bool check=false;
    // Если слова имеют одинаковую длину и буквы в слогах являются парными, то слова рифмуются
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

bool checkLastThreeLetters(string word1, string word2, const vector<pair<char, char>>& vow, const vector<pair<char, char>>& cons) { // Функция для проверки созвучности последних трех букв двух слов.
    if (word1.length() < 2 || word2.length() < 2) return false; // Если одно из слов короче двух букв, возвращаем false

    char last1 = word1[word1.length() - 1];
    char last2 = word2[word2.length() - 1];
    char secondLast1 = word1[word1.length() - 2];
    char secondLast2 = word2[word2.length() - 2];
    char thirdLast1 = word1[word1.length() - 3];
    char thirdLast2 = word2[word2.length() - 3];
    char fourdLast1 = word1[word1.length() - 4];
    char fourdLast2 = word2[word2.length() - 4];


    // Проверяем последние буквы
    bool lastLettersMatch = (last1 == last2) || arePair(last1, last2, vow) || arePair(last1, last2, cons);
    // Проверяем предпоследние буквы
    bool secondLastLettersMatch = (secondLast1 == secondLast2) || arePair(secondLast1, secondLast2, vow) || arePair(secondLast1, secondLast2, cons);
    // Проверяем предпредпоследние буквы
    bool thirdLastLettersMatch = (thirdLast1 == thirdLast2) || arePair(thirdLast1, thirdLast2, vow) || arePair(thirdLast1, thirdLast2, cons);
    return lastLettersMatch && secondLastLettersMatch && thirdLastLettersMatch;
}


