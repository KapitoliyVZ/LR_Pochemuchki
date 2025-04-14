#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include <string>
#include "UI_const.h"

int len_txt = 4;               // длина расширения .txt
string extension_txt = ".txt"; // расширение .txt

// Проверка расширения .txt
bool check_txt_extension(string file_name)
{
    if (file_name.length() >= len_txt && file_name.substr(file_name.length() - len_txt) == extension_txt)
        return true; // если есть расширение
    else
        return false; // если нет расширения
}

// Проверка, является ли файл пустым
bool check_inputFile_is_empty(string file_name)
{
    // true - файл пустой
    // false - файл не пустой

    if (file_input.peek() == EOF) // если первый символ это конец файла
    {
        // закрываем файл
        file_input.close();
        return true;
    }
    else
        return false;
}

// Проверка, имеет ли файл расширение .txt, существует ли он, можно ли его открыть, и не пустой ли он
bool checkTxtFile(const std::string &filePath)
{
    // Проверка расширения .txt вручную
    std::string path = filePath;

    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2); // Удаление кавычек

    if (check_txt_extension(filePath))
        return false; // Неверное расширение

    // Проверка, существует ли файл
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
        return false; // Файл не существует

    // Открытие файла через глобальный ifstream
    file_input.open(filePath, ios_base::in);

    // Проверка, удалось ли открыть файл
    if (!file_input.is_open())
        return false; // Не удалось открыть файл

    if (check_inputFile_is_empty(filePath))
        return false; // файл пустой

    return true; // Файл прошел все проверки
}

#endif // FILE_WORKING_H