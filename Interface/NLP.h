#pragma once

#include "standard_libraries.h"
#include <cwctype>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <vector>

// Конвертация UTF-8 <-> Wide
std::wstring utf8_to_wstring(const std::string& utf8_str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], size_needed);
    if (!wstr.empty()) wstr.pop_back();
    return wstr;
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


std::string wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    if (!str.empty()) str.pop_back();
    return str;
}

// Преобразование регистра
std::string lowFirstLetter(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    if (!wstr.empty()) wstr[0] = std::towlower(wstr[0]);
    return wstring_to_utf8(wstr);
}

std::string capitalizeAllLetters(const std::string& input) {
    std::wstring wstr = utf8_to_wstring(input);
    for (auto& ch : wstr) ch = std::towupper(ch);
    return wstring_to_utf8(wstr);
}

// Удаление пунктуации
std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (!std::ispunct(static_cast<unsigned char>(ch))) {
            result += ch;
        }
    }
    return result;
}

// Работа с временными файлами
std::string create_temp_file(const std::string& prefix, const std::string& content) {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);

    char tempFileName[MAX_PATH];
    GetTempFileNameA(tempPath, prefix.c_str(), 0, tempFileName);

    std::ofstream out(tempFileName, std::ios::binary);
    out << content;
    out.close();

    return std::string(tempFileName);
}

std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

std::string run_mystem_on_word(const std::string& word) {
    std::string inputFile = create_temp_file("in", word);
    std::string outputFile = create_temp_file("out", "");

    std::string command = "\"C:\\Study\\MyStem\\mystem.exe\" -i \"" + inputFile + "\" \"" + outputFile + "\"";

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::vector<char> cmd(command.begin(), command.end());
    cmd.push_back('\0');

    BOOL success = CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (!success) throw std::runtime_error("CreateProcessA failed");

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::string result = read_file(outputFile);

    DeleteFileA(inputFile.c_str());
    DeleteFileA(outputFile.c_str());

    return result;
}

// Кэш для хранения результатов
std::unordered_map<std::string, std::string> cache;

std::string getPartOfSpeech(const std::string& word) {
    if (cache.find(word) != cache.end()) return cache[word];

    std::string result = run_mystem_on_word(word);

    result = utf8_to_ansi(result);

    size_t pos = result.find('{');
    if (pos != std::string::npos) {
        size_t endPos = result.find('}', pos);
        if (endPos != std::string::npos) {
            std::string analysis = result.substr(pos + 1, endPos - pos - 1);
            if (analysis.find("прич") != std::string::npos) cache[word] = "прич";
            else if (analysis.find("деепр") != std::string::npos) cache[word] = "деепр";
            else if (analysis.find("SPRO") != std::string::npos) cache[word] = "SPRO";
            else if (analysis.find("ADV") != std::string::npos) cache[word] = "ADV";
            else if (analysis.find("A=") != std::string::npos) cache[word] = "A";
            else {
                size_t equalsPos = analysis.find('=');
                if (equalsPos != std::string::npos && equalsPos + 1 < analysis.size()) {
                    cache[word] = std::string(1, analysis[equalsPos + 1]);
                }
            }
            return cache[word];
        }
    }
    return "";
}

std::vector<std::string> findWordsByPartOfSpeech(std::vector<std::vector<std::string>>& sentences, const std::string& targetPartOfSpeech) {
    std::vector<std::string> foundWords;

    for (auto& sentence : sentences) {
        for (auto& word : sentence) {
            word = removePunctuation(word);
            std::string partOfSpeech = getPartOfSpeech(word);
            if (partOfSpeech == targetPartOfSpeech) {
                foundWords.push_back(lowFirstLetter(capitalizeAllLetters(word)));
                word = capitalizeAllLetters(word);
            }
        }
    }
    return foundWords;
}

std::vector<std::string> get_comparing_word_part(std::string& comparing_word) {
    std::vector<std::string> part_of_speech(6, "");
    if (comparing_word == " ") return part_of_speech;

    std::string part = getPartOfSpeech(comparing_word);

    if (part == "V") part_of_speech[0] = comparing_word;
    else if (part == "ADV") part_of_speech[1] = comparing_word;
    else if (part == "A") part_of_speech[2] = comparing_word;
    else if (part == "S") part_of_speech[3] = comparing_word;
    else if (part == "прич") part_of_speech[4] = comparing_word;
    else if (part == "деепр") part_of_speech[5] = comparing_word;

    return part_of_speech;
}
