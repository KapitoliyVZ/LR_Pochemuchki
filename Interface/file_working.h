#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include <string>
#include "UI_const.h"

// Проверка наличия расширения .txt у файла
bool check_txt_extension(string file_name)
{
    int len_txt = 4;               // длина расширения .txt
    string extension_txt = ".txt"; // расширение .txt

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

// Функция проверки имени и открытия файла, возвращает:
// {false, Error_text} или {true, text_in_file)
pair <bool, string> checkName_openFile(const string &filePath) // filePath - путь к файлу
{
    if (filePath.empty())
        return {false, "Error!: FilePath is empty!"}; // Путь к файлу пустой

    string path = filePath;
    // Удаление кавычек в начале и конце строки, если они есть
    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    if (!check_txt_extension(path))
        return {false, "Error!: Wrong extension!"}; // Неверное расширение

    // Открытие файла через глобальный ifstream file_input
    file_input.open(path, ios_base::in);

    if (!file_input.is_open())
        return { false, "Error!: Fail in opening!" }; // Не удалось открыть файл

    if (check_inputFile_is_empty(path))
        return { false, "Error!: File is empty!" }; // файл пустой

    string text_in_string; // текст файла

    // Чтение всего текста из файла
    getline(file_input, text_in_string, '\0'); // Чтение до конца файла
    file_input.close();                        // Закрываем файл

	return { true, text_in_string }; // Файл прошел все проверки
}

#endif // FILE_WORKING_H