#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include <string>
#include "UI_const.h"

int len_txt = 4;               // длина расширения .txt
string extension_txt = ".txt"; // расширение .txt

// Проверка наличия расширения .txt у файла
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

    if (file_input.peek() == EOF)
    {
        // Закрытие файла, если он пустой
        file_input.close();
        return true;
    }
    else
        return false;
}

// Проверка файла на наличие в пути (имени) расширения .txt и его существование
bool checkName_openFile(const string &filePath) // filePath - путь к файлу
{
    if (filePath.empty())
        return false; // Путь к файлу пустой

    string path = filePath;
    // Удаление кавычек в начале и конце строки, если они есть
    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    if (!check_txt_extension(path))
        return false; // Неверное расширение

    // Открытие файла через глобальный ifstream file_input
    file_input.open(path, ios_base::in);

    if (!file_input.is_open())
        return false; // Не удалось открыть файл

    if (check_inputFile_is_empty(path))
        return false; // файл пустой

    return true; // Файл прошел все проверки
}

#endif // FILE_WORKING_H