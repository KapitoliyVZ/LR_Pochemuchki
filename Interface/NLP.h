#pragma once

#include "standard_libraries.h"
#include <cwctype>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "MyStemProcess.h"

// данный заголовочный файл предназначен для выполнения следующих функций:
// - определение части речи по окончаниям и суффиксам из служебных файлов при однородном и неоднородном поиске без сравниваемого слова
// - определение части речи с помощью вспомогательной функции, которая работает с MyStem при однородном и неоднородном поиске со сравниваемым словом

MystemProcess mystem;

// функция получения пути к служебным файлам
std::string get_filepath(const std::string& filename) {
    
    char buffer[MAX_PATH];  // создаётся буфер для хранения полного пути до исполняемого файла
    
    // получение пути к текущему .exe файлу (без передачи hModule)
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);  

    if (length == 0)
    {
        return filename;  // если не удалось получить путь — возвращается имя файла как есть
    }

    // преобразование C-строки в std::string
    std::string fullPath(buffer, length);  

    // поиск последнего слэша или обратного слэша, чтобы отделить путь от имени файла
    size_t pos = fullPath.find_last_of("\\/");
    
    if (pos == std::string::npos)
    {
        return filename;  // если слэш не найден, путь некорректен — возвращается только имя файла
    }

    // вырезается путь до директории .exe и к нему добавляется переданное имя файла
    std::string path = fullPath.substr(0, pos + 1) + filename;  
    
    // возвращается полный путь к файлу, находящемуся в той же папке, что и исполняемый файл
    return path;  
}


// проверка на малосимвольные (местоимения, предлоги) части речи
bool match_others(const string& word, const vector<string>& others_parts)
{
    for (const string& others : others_parts) {
        if (others.empty()) continue;
        if (others==word) 
        {
            return true;
        }
    }
    return false;
}

// функция для проверки наличия полученной части слова в служебном файле
bool check_in_file(string fileName, string word_comp)
{
    // переменная-результат, по умолчанию false — слово не найдено
    bool res(false);

    // получение абсолютного пути к файлу, который должен находиться рядом с исполняемым файлом
    string file_path = get_filepath(fileName);

    // открытие входного потока для чтения файла
    ifstream file;
    file.open(file_path, ios_base::in);

    // если файл успешно открыт
    if (file.is_open())
    {
        string str;               // строка для построчного чтения
        string wordFile("");      // переменная для накопления слова из строки файла до пробела

        // цикл продолжается, пока не достигнут конец файла
        while (!file.eof())
        {
            // считывание следующей строки из файла
            getline(file, str);

            // перебор каждого символа в считанной строке
            for (int i(0); i < str.size(); i++)
            {
                // если текущий символ не пробел — добавляется к слову
                if (str[i] != ' ') wordFile += str[i];
                else
                {
                    // при обнаружении пробела происходит сравнение накопленного слова с входным значением
                    if (wordFile == word_comp)
                    {
                        // если найдено совпадение, устанавливается результат и выход из цикла
                        res = true;
                        break;
                    }
                    else
                        // если не совпало — очищается накопленное слово для следующей попытки
                        wordFile = "";
                }
            }

            // если результат уже найден — выход из внешнего цикла
            if (res) break;
        }
    }

    // закрытие файла (даже если он не открылся — вызов безопасен)
    file.close();

    // возвращение результата
    return res;
}

// функция для проверки - является ли взятая буква гласной
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

// функция проверки на то, что слово не является причастием
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

// функция проверки - оканчивается ли слово на один из суффиксов/окончаний
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

// функция проверки - содержит ли слово один из суффиксов (не только окончание)
bool contains_suffix(const string& word, const vector<string>& suffixes) {
    for (const string& suffix : suffixes) {
        if (suffix.empty()) continue;
        if (word.find(suffix) != string::npos) {
            return true;
        }
    }
    return false;
}

// функция проверки - является ли слово прилагательным
bool adj_check(string wordIn) {

    string word = wordIn;

    bool res(false);

    string wordEnd(""); //строка с окончанием слова
    string wordStart(""); //строка с началом слова (до окончания)

    int j(3);
    do {

        // запись окончания и начала слова в соотвествующие строки
        if (word.size() > j + 1)
        {
            for (int i(word.size() - j); i < word.size(); i++) wordEnd += word[i];
            for (int i(0); i < word.size() - j; i++) wordStart += word[i];
        }

        // проверка на наличие окончаний прилагательных
        if (check_in_file("adj_ends.txt", wordEnd)) res = true;

        // проверка на то, что слово не является местоимением
        if (res && check_in_file("pronouns.txt", wordStart)) res = false;

        // проверка на то, что слово не является числительным
        if (res && numerals_check(wordStart)) res = false;

        // проверка на то, что слово не является причастием
        if (res && participle_check(wordStart)) res = false;

        if (res) break;

        // очистка служебных строк
        wordEnd = "";
        if (j != 2) wordStart = "";

        j--;


    } while (j > 1);

    // проверка на то, что буква, предшествующая первой букве окончания является гласной
    if (res) if (vowel_check(wordStart[wordStart.size() - 1])) res = false;
    // проверка на то, что буква, предшествующая первой букве окончания является мягким знаком
    if (res) if (wordStart[wordStart.size() - 1] == 'ь') res = false;

    return res;
}


// функция преобразует строку из кодировки Windows-1251 (ANSI) в UTF-8
std::string ansi_to_utf8(const std::string& ansi_str) {
    // сначала конвертация из ANSI в wide-строку (UTF-16)
    int wide_size = MultiByteToWideChar(1251, 0, ansi_str.c_str(), -1, nullptr, 0);  // получение необходимого размера для буфера
    std::wstring wide_str(wide_size, 0);  // выделение буфера под wide-строку
    MultiByteToWideChar(1251, 0, ansi_str.c_str(), -1, &wide_str[0], wide_size);  // реальное преобразование в wide-строку

    // теперь конвертация из wide-строки (UTF-16) в UTF-8
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);  // получение нужного размера
    std::string utf8_str(utf8_size, 0);  // выделение буфера
    WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, &utf8_str[0], utf8_size, nullptr, nullptr);  // реальное преобразование

    // удаление завершающего '\0', т.к. std::string его не требует
    if (!utf8_str.empty()) utf8_str.pop_back();
    return utf8_str;
}


// функция преобразует строку из UTF-8 в wide-строку (UTF-16), которая используется внутри Windows API
std::wstring utf8_to_wstring(const std::string& utf8_str) {
    // определение необходимого размера для wide-строки
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);  // выделение буфера

    // конвертация из UTF-8 в wide-строку
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], size_needed);

    // удаление завершающего символа '\0', не нужного в std::wstring
    if (!wstr.empty()) wstr.pop_back();
    return wstr;
}

// функция преобразует строку из UTF-8 в ANSI (Windows-1251) через промежуточную wide-строку
std::string utf8_to_ansi(const std::string& utf8_str) {
    // сначала преобразование UTF-8 → wide-строка
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_size);

    // затем преобразование wide-строки → ANSI (кодировка 1251)
    int ansi_size = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string ansi_str(ansi_size, 0);
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &ansi_str[0], ansi_size, nullptr, nullptr);

    // удаление лишнего завершающего символа '\0', если он был добавлен
    if (!ansi_str.empty() && ansi_str.back() == '\0') {
        ansi_str.pop_back();
    }

    return ansi_str;
}



// функция преобразует wide-строку (UTF-16) в строку UTF-8
std::string wstring_to_utf8(const std::wstring& wstr) {
    // определение размера буфера для UTF-8 строки
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);  // выделение буфера

    // выполнение конвертации
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

    // удаление завершающего нуля, добавленного API
    if (!str.empty()) str.pop_back();
    return str;
}

// функция очистки строки от знаков пунктуации (необходимо для проверки на знаки пунктуации 
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


// функция преобразования первого символа строки с загловного на строчный
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


// функция замены всех символов на заглавные
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

/*
// Чтение содержимого файла в строку
std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}
*/

// Кэш, чтобы не вызывать повторно mystem для одного и того же слова
std::unordered_map<std::string, std::string> cache;

// Получение части речи слова через mystem
std::string getPartOfSpeech(const std::string& word, unordered_map<string, vector<string>>& morphemeRules) {

    
    // ввод флагов для обнаружения отличий между существительными и прилагательными
    bool nouns_found = false;
    bool adjective_found = false;

    // подсчет количества слогов
    int amount_syllables = countSyllables(word);

    // идентификация наречий, междометий и тд
    if (match_others(word, morphemeRules.at("others_list")))
    {
        return "others";
    }

    
    // Проверка на существительное
    if (((ends_with(word, morphemeRules.at("nouns_endings")) &&
        contains_suffix(word, morphemeRules.at("nouns_suffixes"))) || ((amount_syllables<=2) && (ends_with(word, morphemeRules.at("nouns_endings"))))) && word.size() >= 2) {
        nouns_found = true;
        
    }

    // Проверка на глагол
    if (((ends_with(word, morphemeRules.at("verbs_endings")) &&
        contains_suffix(word, morphemeRules.at("verbs_suffixes"))) || ((amount_syllables <= 4) && (ends_with(word, morphemeRules.at("verbs_endings"))))) && word.size() >= 2) {
        return "V";
    }

    // Проверка на прилагательное
    if ((adj_check(word) || ((amount_syllables <= 3) && (ends_with(word, morphemeRules.at("adjectives_endings"))))) && word.size() >= 4)
    {
        if (nouns_found == true)
            adjective_found = true;
        else
            return "A";
    }


    // дополнительная проверка существительных и прилагательных
    if (nouns_found == true && adjective_found == true)
    {
        // в случае если по окончаниям оба true, то вероятнее всего - прилагательное, так как окончания приалагательных - несколько символов
        if (ends_with(word, morphemeRules.at("nouns_endings")) && ends_with(word, morphemeRules.at("adjectives_endings")))
            return "A";
    }

    // если слово определено только как существительное
    if (nouns_found == true && adjective_found == false)
        return "S";

    // Проверка на наречие
    if (((ends_with(word, morphemeRules.at("adverbs_endings")) &&
        contains_suffix(word, morphemeRules.at("adverbs_suffixes"))) || ((amount_syllables <= 3) && (ends_with(word, morphemeRules.at("adverbs_endings"))))) && word.size() >= 4) {
        return "ADV";
    }

    //причастие
    if (((ends_with(word, morphemeRules.at("participles_endings")) &&
        contains_suffix(word, morphemeRules.at("participles_suffixes"))) || ((amount_syllables <= 3) && (ends_with(word, morphemeRules.at("participles_endings"))))) && word.size() >= 5) {
        return "прич";
    }

    
    // деепричастие
    if (((ends_with(word, morphemeRules.at("gerunds_endings")) &&
        contains_suffix(word, morphemeRules.at("gerunds_suffixes"))) || ((amount_syllables <= 3) && (ends_with(word, morphemeRules.at("gerunds_endings"))))) && word.size() >= 5) {
        return "деепр";
    }

  
    // если ни одна проверка не пройдена, то возвращение значение "неизввестно"
    return "unknown";

};



// Поиск слов по части речи в предложениях
void findWordsByPartOfSpeech(std::vector<std::vector<std::string>>& sentences,bitset<8>& button_flags, const vector<string> parts_of_speech,vector<vector<string>>& words_text_collection, unordered_map<string, vector<string>>& morphemeRules, string& word_to_compare) {

    // переменная для проверки наличия пунктуации на месте части вектора векторов предложений
    string tmp_word;

    for (auto& sentence : sentences) {
        for (auto& word : sentence) {

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


// Получение части речи слова через mystem
std::string get_Mystem_part_of_speech(const std::string& word) {
    if (cache.find(word) != cache.end()) return cache[word]; // Используем кэш

    std::string result = mystem.analyze(word); // Запускаем mystem
    result = utf8_to_ansi(result); // Перекодируем результат в ANSI

    // Ищем разбор в фигурных скобках
    size_t pos = result.find('{');
    if (pos != std::string::npos) {
        size_t endPos = result.find('}', pos);
        if (endPos != std::string::npos) {
            std::string analysis = result.substr(pos + 1, endPos - pos - 1);

            // Получаем только первую часть до '|'
            size_t pipePos = analysis.find('|');
            std::string firstParse = (pipePos != std::string::npos)
                ? analysis.substr(0, pipePos)
                : analysis;

            // Приоритетная проверка частей речи
            if (firstParse.find("прич") != std::string::npos) cache[word] = "прич";
            else if (firstParse.find("деепр") != std::string::npos) cache[word] = "деепр";
            else if (firstParse.find("SPRO") != std::string::npos) cache[word] = "SPRO";
            else if (firstParse.find("ADV") != std::string::npos) cache[word] = "ADV";
            else if (firstParse.find("A=") != std::string::npos) cache[word] = "A";
            else {
                // взятие символа после =
                size_t equalsPos = firstParse.find('=');
                if (equalsPos != std::string::npos && equalsPos + 1 < firstParse.size()) {
                    // До первого символа "," или "=" после =
                    size_t endPos = firstParse.find_first_of(",=", equalsPos + 1);
                    std::string posTag = (endPos != std::string::npos)
                        ? firstParse.substr(equalsPos + 1, endPos - equalsPos - 1)
                        : firstParse.substr(equalsPos + 1);
                    cache[word] = posTag;
                }
            }
            return cache[word];
        }
    }
    return ""; // Если не удалось определить часть речи
}


// Возвращает слово, если его часть речи соответствует одной из категорий
std::vector<std::string> get_comparing_word_part(std::string& comparing_word) {
    std::vector<std::string> part_of_speech(6, "");
    if (comparing_word == " ") return part_of_speech;

    string comparing_word_UTF8 = ansi_to_utf8(comparing_word);
    // вызов mystem
    std::string part = get_Mystem_part_of_speech(comparing_word_UTF8);

    // Проверка всех целевых частей речи
    if (part == "V") part_of_speech[0] = comparing_word;
    else if (part == "ADV") part_of_speech[1] = comparing_word;
    else if (part == "A") part_of_speech[2] = comparing_word;
    else if (part == "S") part_of_speech[3] = comparing_word;
    else if (part == "прич") part_of_speech[4] = comparing_word;
    else if (part == "деепр") part_of_speech[5] = comparing_word;

    return part_of_speech;
}
