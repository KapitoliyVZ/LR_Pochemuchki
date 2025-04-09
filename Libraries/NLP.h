#pragma once

#include "standard_libraries.h"
#include <cwctype>

std::wstring utf8_to_wstring(const std::string& utf8_str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], size_needed);
    wstr.pop_back(); // убрать лишний нуль-терминатор
    return wstr;
}

std::string wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    str.pop_back(); // убрать лишний нуль-терминатор
    return str;
}

std::string lowFirstLetter(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    if (!wstr.empty()) {
        wstr[0] = std::towlower(wstr[0]);
    }
    return wstring_to_utf8(wstr);
}

std::string capitalizeAllLetters(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    for (auto& ch : wstr) {
        ch = std::towupper(ch);
    }
    return wstring_to_utf8(wstr);
}



// Функция для перекодировки из UTF-8 в ANSI (Windows-1251)
std::string utf8_to_ansi(const std::string& utf8_str) {
    // Преобразуем UTF-8 в UTF-16 (широкие символы)
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_size);

    // Преобразуем UTF-16 в ANSI (Windows-1251)
    int ansi_size = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string ansi_str(ansi_size, 0);
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &ansi_str[0], ansi_size, nullptr, nullptr);

    return ansi_str;
}

// Функция для выполнения команды и получения вывода
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// Кэш для хранения результатов
std::unordered_map<std::string, std::string> cache;

// Функция для получения части речи слова
std::string getPartOfSpeech(const std::string& word) {
    if (cache.find(word) != cache.end()) {
        return cache[word]; // Возвращаем результат из кэша
    }

    std::string command = "echo " + word + " | \"C:\\Study\\MyStem\\mystem.exe\" -i ";
    std::string result = exec(command.c_str());


    // НОВАЯ ФУНКЦИЯ ПЕРЕВОДА В АНСИ
    result = utf8_to_ansi(result);

    size_t pos = result.find('{');
    if (pos != std::string::npos) {
        size_t endPos = result.find('}', pos);
        if (endPos != std::string::npos) {
            std::string analysis = result.substr(pos + 1, endPos - pos - 1);
            // Определяем часть речи
            if (analysis.find("прич") != std::string::npos) {
                cache[word] = "прич"; // Причастие

            }
            else if (analysis.find("деепр") != std::string::npos) {
                cache[word] = "деепр"; // Деепричастие
            }
            else if (analysis.find("SPRO") != std::string::npos) {
                cache[word] = "SPRO"; // Местоимение
            }
            else if (analysis.find("ADV") != std::string::npos) {
                cache[word] = "ADV"; // Наречие
            }
            else if (analysis.find("A=") != std::string::npos) {
                cache[word] = "A"; // Прилагательное
            }
            else {
                size_t equalsPos = analysis.find('=');
                if (equalsPos != std::string::npos && equalsPos + 1 < analysis.size()) {
                    cache[word] = std::string(1, analysis[equalsPos + 1]); // Обычная часть речи
                }
            }
            return cache[word];
        }
    }
    return "";
}

// Функция для удаления знаков пунктуации из слова
std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (!std::ispunct(static_cast<unsigned char>(ch))) {
            result += ch;
        }
    }
    return result;
}

// Функция для поиска слов по части речи
std::vector<std::string> findWordsByPartOfSpeech(vector<vector<string>>& sentences, const std::string& targetPartOfSpeech) {
    
   
    std::vector<std::string> foundWords;

    for (vector<string>& sentence : sentences)
    {
        for (string& word : sentence)
        {
            word = removePunctuation(word); // Удаляем знаки пунктуации
            std::string partOfSpeech = getPartOfSpeech(word);
            if (partOfSpeech == targetPartOfSpeech) {
                foundWords.push_back(word);
                word = capitalizeAllLetters(word);
                foundWords.back() = lowFirstLetter(foundWords.back());
            }
        }
    }
    

    return foundWords;
}

// функция получения части речи у сравниваемого слова
vector <string> get_comparing_word_part(string& comparing_word)
{
    // вектор, в котором будет храниться слово для сравнения в зависимости от его части речи
    vector <string> part_of_speech;
    part_of_speech.assign(6, "");

    if (comparing_word == " ")
        return part_of_speech;


    string part_of_speech_symbol = getPartOfSpeech(comparing_word);
    
    // заполнение вектора в зависимости от чати речи
    if (part_of_speech_symbol == "V")
    {
        part_of_speech[0] = comparing_word;
    }

    if (part_of_speech_symbol == "ADV")
    {
        part_of_speech[1] = comparing_word;
    }

    if (part_of_speech_symbol == "A")
    {
        part_of_speech[2] = comparing_word;
    }

    if (part_of_speech_symbol == "S")
    {
        part_of_speech[3] = comparing_word;
    }

    if (part_of_speech_symbol == "прич")
    {
        part_of_speech[4] = comparing_word;
    }

    if (part_of_speech_symbol == "деепр")
    {
        part_of_speech[5] = comparing_word;
    }

    return part_of_speech;
}