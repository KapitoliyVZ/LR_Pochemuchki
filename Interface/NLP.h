#pragma once

#include "standard_libraries.h"
#include <cwctype>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "MyStemProcess.h"


//MystemProcess mystem;

std::string get_filepath(const std::string& filename) {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return filename;  // fallback без кавычек
    }
    std::string fullPath(buffer, length);

    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos) {
        return filename;
    }

    std::string path = fullPath.substr(0, pos + 1) + filename;
    return path;
}

// функции Харитонов начало

// проверка на малосимвольные (местоимения, предлоги) части речи
bool match_others(const string& word, const vector<string>& others_parts)
{
    for (const string& others : others_parts) {
        if (others.empty()) continue;
        if (others==word)//||(word.find('-'))) 
        {
            return true;
        }
    }
    return false;
}

bool check_in_file(string fileName, string word_comp)
{

    bool res(false);

    string file_path = get_filepath(fileName);

    ifstream file;
    file.open(file_path, ios_base::in);

    if (file.is_open())
    {
        string str;
        string wordFile("");    //инициализация строки для сверки со строкой, подающейся на вход

        while (!file.eof())
        {
            getline(file, str); //считывание строки из файла

            for (int i(0); i < str.size(); i++)
            {
                if (str[i] != ' ') wordFile += str[i];  //считывание слова до пробела из строки
                else
                {
                    //при пробеле сравнение с входной строкой считанного из файла слова
                    if (wordFile == word_comp)
                    {

                        res = true;
                        break;
                    }
                    else wordFile = ""; //если не соотвествует, очистка считанной строки и переход к считыванию следующего слова
                }
            }
            if (res) break;
        }
    }
    file.close();

    return res;
}

bool vowel_check(char letter)
{
    bool res(false);
    ifstream file("rus_vowels.txt");
    string str;

    if (file.is_open())
    {
        //считывание строки из файла
        getline(file, str);
        //сравнение входной буквы с буквами строки из файла
        for (int i(0); i < str.size(); i++)
            if (letter == str[i])
            {
                res = true; break;
            }
        file.close();
    }
    return res;
}

bool participle_check(string wordSt) {

    bool res(false);
    string suffix("");
    //если основа слова больше 2 символов
    if (wordSt.size() > 2)
    {
        for (int i(wordSt.size() - 2); i < wordSt.size(); i++) suffix += wordSt[i]; //запись суффикса из начала слова

        res = check_in_file("suffix_part.txt", suffix); //сверка с файлом с суффиксами
    }

    suffix = "";

    //если основа слова больше 4 символов
    if (!res && wordSt.size() > 4)
    {
        for (int i(wordSt.size() - 3); i < wordSt.size(); i++) suffix += wordSt[i]; //запись суффикса из начала слова
        if (suffix == "енн")
        {
            if (vowel_check(wordSt[wordSt.size() - 5])) res = true; //если буква перед суффиксом -енн- гласная, то это причастие
            //проверка на слова исключения
            if (check_in_file("exceptions.txt", wordSt)) res = false;   //
        }
        //суффикс -ённ- не присущ прилагательным
        if (suffix == "ённ") res = true;
    }
    return res;

}

//функция проверки на то, что слово относится к схожим числительным
//wordSt - основа слова
//check_part - часть основы слова, считанная из файла
//на выходе - результат проверки
bool numerals_part_check(string wordSt, string check_part)
{
    bool res(false);
    string ending("");
    int from_end = check_part.size(); //размер части основы
    if (wordSt.size() > from_end - 1)
    {
        for (int i(wordSt.size() - from_end); i < wordSt.size(); i++) ending += wordSt[i]; //запись окончания основы
        if (ending == check_part) res = true;   //сравнение окончания основы с окончанием основы из файла
    }
    return res;
}

//функция проверки на то, что слово является числительным
//wordSt - основа слова
//на выходе - результат проверки
bool numerals_check(string wordSt) {

    bool res(false);

    //проверка на то, что основа слова соотвествует списку оснований числительных из файла
    res = check_in_file("numerals.txt", wordSt);

    if (!res) {

        //проверка на то, что часть основы слова соотвествует списку частей оснований числителньых из файла
        ifstream file("numeral_ends.txt");

        if (file.is_open()) {

            string str;
            string check_part("");

            while (!file.eof()) {

                //считывание строки из файла с частями основ чистельных из файла
                getline(file, str);

                for (int i(0); i < str.size(); i++) {

                    if (str[i] != ' ') check_part += str[i]; //запись части в соотвествующую переменную до пробела
                    else {

                        res = numerals_part_check(wordSt, check_part);  //сравнение части основания слова со считанным из файла при пробеле 

                        check_part = "";
                        if (res) break;
                    }
                }
                if (res) break;
            }
            file.close();
        }
    }
    return res;
}

// Проверяет, оканчивается ли слово на один из суффиксов/окончаний
bool ends_with(const string& word, const vector<string>& suffixes) {
    for (const string& suffix : suffixes) {
        if (suffix.empty()) continue;
        if (word.length() >= suffix.length() &&
            word.substr(word.length() - suffix.length()) == suffix) {
            return true;
        }
    }
    return false;
}

// Проверяет, содержит ли слово один из суффиксов (не только окончание)
bool contains_suffix(const string& word, const vector<string>& suffixes) {
    for (const string& suffix : suffixes) {
        if (suffix.empty()) continue;
        if (word.find(suffix) != string::npos) {
            return true;
        }
    }
    return false;
}

bool adj_check(string wordIn) {

    string word = wordIn;

    bool res(false);

    string wordEnd(""); //строка с окончанием слова
    string wordStart(""); //строка с началом слова (до окончания)

    int j(3);
    do {

        //запись окончания и начала слова в соотвествующие строки
        if (word.size() > j + 1)
        {
            for (int i(word.size() - j); i < word.size(); i++) wordEnd += word[i];
            for (int i(0); i < word.size() - j; i++) wordStart += word[i];
        }

        //проверка на наличие окончаний прилагательных
        if (check_in_file("adj_ends.txt", wordEnd)) res = true;

        //проверка на то, что слово не является местоимением
        if (res && check_in_file("pronouns.txt", wordStart)) res = false;

        //проверка на то, что слово не является числительным
        if (res && numerals_check(wordStart)) res = false;

        //проверка на то, что слово не является причастием
        if (res && participle_check(wordStart)) res = false;

        if (res) break;

        //очистка служебных строк
        wordEnd = "";
        if (j != 2) wordStart = "";

        j--;


    } while (j > 1);

    //проверка на то, что буква, предшествующая первой букве окончания является гласной
    if (res) if (vowel_check(wordStart[wordStart.size() - 1])) res = false;
    //проверка на то, что буква, предшествующая первой букве окончания является мягким знаком
    if (res) if (wordStart[wordStart.size() - 1] == 'ь') res = false;

    return res;
}

// Функции Харитонов конец


// функция перевода ANSI в UTF-8
std::string ansi_to_utf8(const std::string& ansi_str) {
    int wide_size = MultiByteToWideChar(1251, 0, ansi_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(1251, 0, ansi_str.c_str(), -1, &wide_str[0], wide_size);

    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8_str(utf8_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, &utf8_str[0], utf8_size, nullptr, nullptr);

    if (!utf8_str.empty()) utf8_str.pop_back(); // Удаляем завершающий ноль
    return utf8_str;
}

// Конвертация строки из UTF-8 в wide-строку (UTF-16)
std::wstring utf8_to_wstring(const std::string& utf8_str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], size_needed);
    if (!wstr.empty()) wstr.pop_back(); // Удаляем завершающий ноль
    return wstr;
}

// Конвертация UTF-8 → Windows-1251 (ANSI) через wide-строку
std::string utf8_to_ansi(const std::string& utf8_str) {
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_size);

    int ansi_size = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string ansi_str(ansi_size, 0);
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &ansi_str[0], ansi_size, nullptr, nullptr);

    // Удаляем завершающий нулевой символ, если он есть
    if (!ansi_str.empty() && ansi_str.back() == '\0') {
        ansi_str.pop_back();
    }

    return ansi_str;
}

// Обратная конвертация wide-строки в UTF-8
std::string wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    if (!str.empty()) str.pop_back(); // Удаляем завершающий ноль
    return str;
}

std::wstring keepOnlyRussianLetters(const std::wstring& input) {
    std::wstring result;
    for (wchar_t wc : input) {
        // Проверяем диапазоны русских букв: А-Я, а-я, Ё, ё
        if ((wc >= L'А' && wc <= L'я') || wc == L'Ё' || wc == L'ё') {
            result += wc;
        }
    }
    return result;
}

std::string cleanRussianOnly(const std::string& input) {
    std::string result;
    for (unsigned char ch : input) {
        if ((ch >= 0xC0 && ch <= 0xDF) ||  // А-Я
            (ch >= 0xE0 && ch <= 0xFF) ||  // а-я
            ch == 0xA8 ||                   // Ё
            ch == 0xB8)                     // ё
        {
            result += static_cast<char>(ch);
        }
        // иначе пропускаем символ
    }
    return result;
}

std::string lowFirstLetter(const std::string& input) {
    static const std::unordered_map<unsigned char, unsigned char> to_lower = {
        {0xC0, 0xE0}, // А->а
        {0xC1, 0xE1}, // Б->б
        {0xC2, 0xE2}, // В->в
        {0xC3, 0xE3}, // Г->г
        {0xC4, 0xE4}, // Д->д
        {0xC5, 0xE5}, // Е->е
        {0xA8, 0xB8}, // Ё->ё
        {0xC6, 0xE6}, // Ж->ж
        {0xC7, 0xE7}, // З->з
        {0xC8, 0xE8}, // И->и
        {0xC9, 0xE9}, // Й->й
        {0xCA, 0xEA}, // К->к
        {0xCB, 0xEB}, // Л->л
        {0xCC, 0xEC}, // М->м
        {0xCD, 0xED}, // Н->н
        {0xCE, 0xEE}, // О->о
        {0xCF, 0xEF}, // П->п
        {0xD0, 0xF0}, // Р->р
        {0xD1, 0xF1}, // С->с
        {0xD2, 0xF2}, // Т->т
        {0xD3, 0xF3}, // У->у
        {0xD4, 0xF4}, // Ф->ф
        {0xD5, 0xF5}, // Х->х
        {0xD6, 0xF6}, // Ц->ц
        {0xD7, 0xF7}, // Ч->ч
        {0xD8, 0xF8}, // Ш->ш
        {0xD9, 0xF9}, // Щ->щ
        {0xDA, 0xFA}, // Ъ->ъ
        {0xDB, 0xFB}, // Ы->ы
        {0xDC, 0xFC}, // Ь->ь
        {0xDD, 0xFD}, // Э->э
        {0xDE, 0xFE}, // Ю->ю
        {0xDF, 0xFF}  // Я->я
    };

    if (input.empty()) return input;

    std::string result = input;
    unsigned char first_char = static_cast<unsigned char>(result[0]);
    auto it = to_lower.find(first_char);
    if (it != to_lower.end()) {
        result[0] = static_cast<char>(it->second);
    }
    return result;
}

std::string capitalizeAllLetters(const std::string& input) {
    static const std::unordered_map<unsigned char, unsigned char> to_upper = {
        {0xE0, 0xC0}, // а->А
        {0xE1, 0xC1}, // б->Б
        {0xE2, 0xC2}, // в->В
        {0xE3, 0xC3}, // г->Г
        {0xE4, 0xC4}, // д->Д
        {0xE5, 0xC5}, // е->Е
        {0xB8, 0xA8}, // ё->Ё
        {0xE6, 0xC6}, // ж->Ж
        {0xE7, 0xC7}, // з->З
        {0xE8, 0xC8}, // и->И
        {0xE9, 0xC9}, // й->Й
        {0xEA, 0xCA}, // к->К
        {0xEB, 0xCB}, // л->Л
        {0xEC, 0xCC}, // м->М
        {0xED, 0xCD}, // н->Н
        {0xEE, 0xCE}, // о->О
        {0xEF, 0xCF}, // п->П
        {0xF0, 0xD0}, // р->Р
        {0xF1, 0xD1}, // с->С
        {0xF2, 0xD2}, // т->Т
        {0xF3, 0xD3}, // у->У
        {0xF4, 0xD4}, // ф->Ф
        {0xF5, 0xD5}, // х->Х
        {0xF6, 0xD6}, // ц->Ц
        {0xF7, 0xD7}, // ч->Ч
        {0xF8, 0xD8}, // ш->Ш
        {0xF9, 0xD9}, // щ->Щ
        {0xFA, 0xDA}, // ъ->Ъ
        {0xFB, 0xDB}, // ы->Ы
        {0xFC, 0xDC}, // ь->Ь
        {0xFD, 0xDD}, // э->Э
        {0xFE, 0xDE}, // ю->Ю
        {0xFF, 0xDF}  // я->Я
    };

    std::string result = input;
    for (auto& ch : result) {
        unsigned char c = static_cast<unsigned char>(ch);
        auto it = to_upper.find(c);
        if (it != to_upper.end()) {
            ch = static_cast<char>(it->second);
        }
    }
    return result;
}

// Удаляет знаки пунктуации из слова
std::string removePunctuation(const std::string& word) {
    // Преобразуем UTF-8 → UTF-16
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, word.c_str(), -1, nullptr, 0);
    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, word.c_str(), -1, &wideStr[0], wideSize);
    if (!wideStr.empty()) wideStr.pop_back(); // Удаляем завершающий ноль

    // Фильтруем только символы, которые НЕ являются пунктуацией/разделителями
    std::wstring filtered;
    for (wchar_t wc : wideStr) {
        if (!iswpunct(wc) && !iswspace(wc) && !iswcntrl(wc) &&
            wc != L'«' && wc != L'»' && wc != L'“' && wc != L'”' &&
            wc != L'–' && wc != L'—' && wc != L'…') {
            filtered += wc;
        }
    }

    // Преобразуем обратно UTF-16 → UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, filtered.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, filtered.c_str(), -1, &utf8Str[0], utf8Size, nullptr, nullptr);
    if (!utf8Str.empty()) utf8Str.pop_back(); // Удаляем завершающий ноль

    return utf8Str;
}

// Чтение содержимого файла в строку
std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

// Кэш, чтобы не вызывать повторно mystem для одного и того же слова
std::unordered_map<std::string, std::string> cache;

// Получение части речи слова через mystem
std::string getPartOfSpeech(const std::string& word, unordered_map<string, vector<string>>& morphemeRules) {

    //string word_ANSI = utf8_to_ansi(word); // Перекодируем результат в ANSI

    // подсчет количества слогов
    int amount_syllables = countSyllables(word);

    // идентификация наречий, междометий и тд
    if (match_others(word, morphemeRules.at("others_list")))
    {
        return "others";
    }

    //|| ((ends_with(word, morphemeRules.at("nouns_endings")))|| (word.size() < 5) )
    // Проверка на существительное
    if (((ends_with(word, morphemeRules.at("nouns_endings")) &&
        contains_suffix(word, morphemeRules.at("nouns_suffixes"))) || ((amount_syllables<=2) && (ends_with(word, morphemeRules.at("nouns_endings"))))) && word.size() >= 2) {
        return "S";
    }

    // Проверка на глагол
    if (((ends_with(word, morphemeRules.at("verbs_endings")) &&
        contains_suffix(word, morphemeRules.at("verbs_suffixes"))) || ((amount_syllables <= 2) && (ends_with(word, morphemeRules.at("verbs_endings"))))) && word.size() >= 2) {
        return "V";
    }

    // Проверка на прилагательное
    if ((adj_check(word) || ((amount_syllables <= 3) && (ends_with(word, morphemeRules.at("adjectives_endings"))))) && word.size() >= 4)
    {
        return "A";
    }

    // Проверка на наречие
    if (((ends_with(word, morphemeRules.at("adverbs_endings")) &&
        contains_suffix(word, morphemeRules.at("adverbs_suffixes"))) || ((amount_syllables <= 2) && (ends_with(word, morphemeRules.at("adverbs_endings"))))) && word.size() >= 4) {
        return "ADV";
    }

    //причастие
    if (((ends_with(word, morphemeRules.at("participles_endings")) &&
        contains_suffix(word, morphemeRules.at("participles_suffixes"))) || ((amount_syllables <= 2) && (ends_with(word, morphemeRules.at("participles_endings"))))) && word.size() >= 5) {
        return "прич";
    }

    

    if (((ends_with(word, morphemeRules.at("gerunds_endings")) &&
        contains_suffix(word, morphemeRules.at("gerunds_suffixes"))) || ((amount_syllables <= 2) && (ends_with(word, morphemeRules.at("gerunds_endings"))))) && word.size() >= 5) {
        return "деепр";
    }

    // учет нулевых окончаний
    return "S";

};



// Поиск слов по части речи в предложениях
void findWordsByPartOfSpeech(std::vector<std::vector<std::string>>& sentences,bitset<8>& button_flags, const vector<string> parts_of_speech,vector<vector<string>>& words_text_collection, unordered_map<string, vector<string>>& morphemeRules, string& word_to_compare) {

    // переменная для проверки наличия пунктуации на месте части вектора векторов предложений
    string tmp_word;

    for (auto& sentence : sentences) {
        for (auto& word : sentence) {

            // TMP 24.04.2025
            //word = removePunctuation(word); // Удаляем пунктуацию

            // с помощью вспомогательной переменной проверяем, не является ли слово знаком пунктуации
            tmp_word = word;
            tmp_word = cleanRussianOnly(tmp_word);
            if (tmp_word.empty())
                continue;

            tmp_word = lowFirstLetter(tmp_word);
            std::string partOfSpeech = getPartOfSpeech(tmp_word, morphemeRules);

            for (int i = 0; i < 6; i++)
            {
                // выход из поиска нужного вектора векторов, если слово не одна из 6 речи, которые искались
                if (partOfSpeech == "unknown" || partOfSpeech == "others")
                    break;

                if ((partOfSpeech == parts_of_speech[i]) && button_flags.test(i))
                {
                    
                        word = lowFirstLetter(word);
                        
                        words_text_collection[i].push_back(word); // Сохраняем найденное слово с обработкой регистра

                        if (word_to_compare.empty() || (!word_to_compare.empty() && word_to_compare == word))
                            word = capitalizeAllLetters(word); // Обновляем слово в предложении

                        // выход из поиска - слово найдено
                        break;
                    
                }
            
            }
        }
    }
}

// Возвращает слово, если его часть речи соответствует одной из категорий
std::vector<std::string> get_comparing_word_part(std::string& comparing_word,unordered_map<string, vector<string>>& morphemeRules) {
    std::vector<std::string> part_of_speech(6, "");
    if (comparing_word == " ") return part_of_speech;

    std::string part = getPartOfSpeech(comparing_word, morphemeRules);

    // Проверка всех целевых частей речи
    if (part == "V") part_of_speech[0] = comparing_word;
    else if (part == "ADV") part_of_speech[1] = comparing_word;
    else if (part == "A") part_of_speech[2] = comparing_word;
    else if (part == "S") part_of_speech[3] = comparing_word;
    else if (part == "прич") part_of_speech[4] = comparing_word;
    else if (part == "деепр") part_of_speech[5] = comparing_word;

    return part_of_speech;
}
