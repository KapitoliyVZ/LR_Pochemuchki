#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include <string>
#include "UI_const.h"

ifstream file_input;           // файл для чтения
ofstream file_output_numbered; // файл-текст для записи
ofstream file_output_rhymes;   // файл-рифмы для записи

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

// Функция для работы с файлом для чтения: проверка имени и открытия файла, возвращает:
// {false, Error_text} или {true, text_in_file)
pair<bool, string> inputFile_working(const string &inputFilePath) // filePath - путь к файлу
{
    if (inputFilePath.empty())
        return {false, "Error!: FilePath is empty!"}; // Путь к файлу пустой

    string path = inputFilePath;
    // Удаление кавычек в начале и конце строки, если они есть
    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    if (!check_txt_extension(path))
        return {false, "Error!: Wrong extension!"}; // Неверное расширение

    // Открытие файла через глобальный ifstream file_input
    file_input.open(path, ios_base::in);

    if (!file_input.is_open())
        return {false, "Error!: Fail in opening!"}; // Не удалось открыть файл

    if (check_inputFile_is_empty(path))
        return {false, "Error!: File is empty!"}; // файл пустой

    string text_in_string; // текст файла

    // Чтение всего текста из файла
    getline(file_input, text_in_string, '\0'); // Чтение до конца файла
    file_input.close();                        // Закрываем файл

    return {true, text_in_string}; // Файл прошел все проверки
}

/////////////////////////////////////////////////////////////

// Функция для записи пронумерованного текста в файл (file_output_numbered)
void write_outputFiles(vector<vector<string>> sentences_numbered)
{
    // для проверки по точкам останова
    /*for (vector<string>& sentence : sentences_numbered)
    {
        bool firstWord = true;
        for (string& word : sentence)
        {
            word = utf8_to_ansi(word);
        }
    }*/

    const set<string> punctuationMarks = {".", ",", "!", "?", ":", ";", "-", "(", ")", "\"", "'"};
    for (const vector<string> &sentence : sentences_numbered)
    {
        bool firstWord = true;
        for (const string &word : sentence)
        {
            if (!firstWord && punctuationMarks.find(word) == punctuationMarks.end())
                file_output_numbered << " ";
            file_output_numbered << word;
            firstWord = false;
        }

        file_output_numbered << "\n"; // Перенос строки после предложения
    }
    file_output_numbered.close();
}
// Функция для записи рифм в файл (file_output_rhymes)
void write_outputFiles(vector<WordData> rhymes_data)
{
    file_output_rhymes << "тут информация о всех рифмах\n\n";

    for (WordData output : rhymes_data)
    {
        string wordInfo = "Слово: " + output.word;                                      // само слово
        string part_of_speech = "\nЧасть речи: " + ansi_to_utf8(output.part_of_speech); // часть речи слова
        string wordAmount = "\nКоличество найденных слов: " + to_string(output.amount); // Количество найденных слов
        string rhymesAmount = "\nКоличество рифм: " + to_string(output.rhymed_amount);  // Количество рифм
        string sentenceAmount = "\nНайдено в предложениях: ";                           // Кол-во предложений с упоминанием

        // вывод слова
        file_output_rhymes << wordInfo << part_of_speech << wordAmount << rhymesAmount;

        // вывод предложений, где встречается слово
        if (!output.sentence_counter.empty())
        {
            for (size_t i = 0; i < output.sentence_counter.size(); ++i)
            {
                if (i != 0)
                    sentenceAmount += ", ";
                sentenceAmount += to_string(output.sentence_counter[i]);
            }
        }

        // вывод рифм
        if (!output.rhymed_words.empty())
        {
            file_output_rhymes << "\nРифмы: ";
            for (string &word : output.rhymed_words)
                file_output_rhymes << "\n  - " << word;
        }
        file_output_rhymes << "\n\n";
    }

    file_output_rhymes.close();
}

// Функция проверки наличия файла с данным именем в системе
bool check_outputFile_existence(const string outputFileName)
{
    ifstream file(outputFileName); // Пытаемся открыть файл

    // Проверяем, удалось ли открыть файл
    if (file.good()) // true - файл существует
    {
        return true;
    }
    else
        return false;
}

// Функция для создания имён выходных файлов
pair<string, string> create_outputFileNames(const string inputFilePath)
{
    // Создание имен выходных файлов на основе полного пути входного файла
    string outputFileName_numbered = inputFilePath.substr(0, inputFilePath.find_last_of('.')) + "_numbered.txt"; // имя выходного файла-текста
    string outputFileName_rhymes = inputFilePath.substr(0, inputFilePath.find_last_of('.')) + "_rhymes.txt";     // имя выходного файла-рифм

    int count_numb = 1; // счетчик для нумерации

    // Проверка наличия файла-текста с данным именем
    while (check_outputFile_existence(outputFileName_numbered))
    {
        // Удалить ".txt"
        outputFileName_numbered.erase(outputFileName_numbered.length() - len_txt, len_txt);

        // Удалить "(n)", если есть
        size_t pos = outputFileName_numbered.rfind('(');
        if (pos != std::string::npos && outputFileName_numbered.back() == ')')
            outputFileName_numbered.erase(pos); // Удаляет от '(' до конца

        // Добавить новую нумерацию
        outputFileName_numbered += "(" + to_string(count_numb) + ").txt";
        count_numb++;
    }

    int count_rhymes = 1; // счетчик для нумерации

    // Проверка наличия файла-рифм с данным именем
    while (check_outputFile_existence(outputFileName_rhymes))
    {
        // Удалить ".txt"
        outputFileName_rhymes.erase(outputFileName_rhymes.length() - len_txt, len_txt);

        // Удалить "(n)", если есть
        size_t pos = outputFileName_rhymes.rfind('(');
        if (pos != std::string::npos && outputFileName_rhymes.back() == ')')
            outputFileName_rhymes.erase(pos); // Удаляет от '(' до конца

        // Добавить новую нумерацию
        outputFileName_rhymes += "(" + to_string(count_rhymes) + ").txt";
        count_rhymes++;
    }

    return {outputFileName_numbered, outputFileName_rhymes};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
// ВЫЗОВ ФУНКЦИИ ДЛЯ ВЫВОДА В ФАЙЛ
string outputFileName_numbered; // имя выходного файла-текста
string outputFileName_rhymes; // имя выходного файла-рифм
outputFiles_working(filename_str, outputFileName_numbered, outputFileName_rhymes, sentences, rhymes_data);
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Функция для создания файла и его открытия
bool outputFiles_working(const string inputFilePath,                      // путь входного файла
                         string &outputFileName_numbered,                 // адрес-имя выходного файла-текста
                         string &outputFileName_rhymes,                   // адрес-имя выходного файла-рифм
                         const vector<vector<string>> sentences_numbered, // вектор пронумерованных предложений
                         const vector<WordData> rhymes_data)              // информация о рифмах
{
    // получаем имена выходных файлов из пути входного файла
    pair<string, string> fromFunct = create_outputFileNames(inputFilePath);
    outputFileName_numbered = fromFunct.first;
    outputFileName_rhymes = fromFunct.second;

    // открытие файлов на запись
    file_output_numbered.open(outputFileName_numbered, ios_base::out | ios::trunc);
    file_output_rhymes.open(outputFileName_rhymes, ios_base::out | ios::trunc);

    if (file_output_numbered.is_open() and file_output_rhymes.is_open())
    {
        write_outputFiles(sentences_numbered); // запись пронумерованного текста
        write_outputFiles(rhymes_data);        // запись рифм
    }
    else
        return false;
}

#endif // FILE_WORKING_H