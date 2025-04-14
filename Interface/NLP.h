#pragma once

#include "standard_libraries.h"
#include <cwctype>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <vector>

// получение пути к mystem.exe
std::string getMystemPath() {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return "\"mystem.exe\"";  // fallback с кавычками
    }

    std::string fullPath(buffer, length);

    // Найдём позицию последнего слэша
    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos) {
        return "\"mystem.exe\"";
    }

    // Соберём путь и добавим кавычки
    std::string mystemPath = fullPath.substr(0, pos + 1) + "mystem.exe";
    return "\"" + mystemPath + "\"";
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

// Преобразует первую букву строки в нижний регистр
std::string lowFirstLetter(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    if (!wstr.empty()) wstr[0] = std::towlower(wstr[0]);
    return wstring_to_utf8(wstr);
}

// Преобразует все символы строки в верхний регистр
std::string capitalizeAllLetters(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    for (auto& ch : wstr) ch = std::towupper(ch);
    return wstring_to_utf8(wstr);
}

// Удаляет знаки пунктуации из слова
std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (!std::ispunct(static_cast<unsigned char>(ch))) {
            result += ch;
        }
    }
    return result;
}

// Создаёт временный файл с заданным содержимым и возвращает его путь
std::string create_temp_file(const std::string& prefix, const std::string& content) {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath); // Получаем путь к системной временной папке

    char tempFileName[MAX_PATH];
    GetTempFileNameA(tempPath, prefix.c_str(), 0, tempFileName); // Создаём уникальное имя файла

    std::ofstream out(tempFileName, std::ios::binary); // Записываем содержимое
    out << content;
    out.close();

    return std::string(tempFileName);
}

// Чтение содержимого файла в строку
std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

// Запуск mystem.exe на слове через временные файлы
std::string run_mystem_on_word(const std::string& word) {
    std::string inputFile = create_temp_file("in", word); // Создаём входной файл
    std::string outputFile = create_temp_file("out", ""); // Создаём пустой выходной файл

    // есть функция подмены пути к mystem, нужна проверка
    // Формируем команду для запуска mystem

    // добавление пути к mystem
    string mystem_file_path = getMystemPath();

    //std::string command = "\"C:\\Study\\MyStem\\mystem.exe\" -i \"" + inputFile + "\" \"" + outputFile + "\"";

    std::string command = mystem_file_path + " -i \"" + inputFile + "\" \"" + outputFile + "\"";

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::vector<char> cmd(command.begin(), command.end());
    cmd.push_back('\0'); // Завершающий ноль обязателен для CreateProcess

    // Запускаем mystem.exe через CreateProcessA
    BOOL success = CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (!success) throw std::runtime_error("CreateProcessA failed");

    WaitForSingleObject(pi.hProcess, INFINITE); // Ждём завершения
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::string result = read_file(outputFile); // Читаем результат

    // Удаляем временные файлы
    DeleteFileA(inputFile.c_str());
    DeleteFileA(outputFile.c_str());

    return result;
}

// Кэш, чтобы не вызывать повторно mystem для одного и того же слова
std::unordered_map<std::string, std::string> cache;

// Получение части речи слова через mystem
std::string getPartOfSpeech(const std::string& word) {
    if (cache.find(word) != cache.end()) return cache[word]; // Используем кэш

    std::string result = run_mystem_on_word(word); // Запускаем mystem
    result = utf8_to_ansi(result); // Перекодируем результат в ANSI

    // Ищем часть речи по шаблону в строке
    size_t pos = result.find('{');
    if (pos != std::string::npos) {
        size_t endPos = result.find('}', pos);
        if (endPos != std::string::npos) {
            std::string analysis = result.substr(pos + 1, endPos - pos - 1);

            // Приоритетная проверка некоторых частей речи
            if (analysis.find("прич") != std::string::npos) cache[word] = "прич";
            else if (analysis.find("деепр") != std::string::npos) cache[word] = "деепр";
            else if (analysis.find("SPRO") != std::string::npos) cache[word] = "SPRO";
            else if (analysis.find("ADV") != std::string::npos) cache[word] = "ADV";
            else if (analysis.find("A=") != std::string::npos) cache[word] = "A";
            else {
                // Универсальный способ — взять символ после =
                size_t equalsPos = analysis.find('=');
                if (equalsPos != std::string::npos && equalsPos + 1 < analysis.size()) {
                    cache[word] = std::string(1, analysis[equalsPos + 1]);  
                }
            }
            return cache[word];
        }
    }
    return ""; // Если не удалось определить часть речи
}

// Поиск слов по части речи в предложениях
std::vector<std::string> findWordsByPartOfSpeech(std::vector<std::vector<std::string>>& sentences, const std::string& targetPartOfSpeech) {
    std::vector<std::string> foundWords;

    for (auto& sentence : sentences) {
        for (auto& word : sentence) {
            word = removePunctuation(word); // Удаляем пунктуацию
            std::string partOfSpeech = getPartOfSpeech(word);
            if (partOfSpeech == targetPartOfSpeech) {
                foundWords.push_back(lowFirstLetter(capitalizeAllLetters(word))); // Сохраняем найденное слово с обработкой регистра
                word = capitalizeAllLetters(word); // Обновляем слово в предложении
            }
        }
    }
    return foundWords;
}

// Возвращает слово, если его часть речи соответствует одной из категорий
std::vector<std::string> get_comparing_word_part(std::string& comparing_word) {
    std::vector<std::string> part_of_speech(6, "");
    if (comparing_word == " ") return part_of_speech;

    std::string part = getPartOfSpeech(comparing_word);

    // Проверка всех целевых частей речи
    if (part == "V") part_of_speech[0] = comparing_word;
    else if (part == "ADV") part_of_speech[1] = comparing_word;
    else if (part == "A") part_of_speech[2] = comparing_word;
    else if (part == "S") part_of_speech[3] = comparing_word;
    else if (part == "прич") part_of_speech[4] = comparing_word;
    else if (part == "деепр") part_of_speech[5] = comparing_word;

    return part_of_speech;
}
