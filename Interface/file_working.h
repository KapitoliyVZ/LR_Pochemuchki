#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include "UI_const.h"

ifstream file_input;           // файл для чтения
ofstream file_output_numbered; // файл-текст для записи
ofstream file_output_rhymes;   // файл-рифмы для записи

const string extension_txt = ".txt";   // расширение .txt
const int len_txt = 4;                 // длина расширения .txt
const string extension_html = ".html"; // расширение .html
const int len_html = 5;                // длина расширения .html

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
        return {false, "Ошибка!: Пустой путь к файлу!"}; // Путь к файлу пустой

    string path = inputFilePath;
    // Удаление кавычек в начале и конце строки, если они есть
    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    if (!check_txt_extension(path))
        return {false, "Ошибка!: Неверное расширение!"}; // Неверное расширение

    // Открытие файла через глобальный ifstream file_input
    file_input.open(path, ios_base::in);

    if (!file_input.is_open())
        return {false, "Ошибка!: Не удалось открыть файл!"}; // Не удалось открыть файл

    if (check_inputFile_is_empty(path))
        return {false, "Ошибка!: Файл пуст!"}; // файл пустой

    string text_in_string; // текст файла

    // Чтение всего текста из файла
    getline(file_input, text_in_string, '\0'); // Чтение до конца файла
    file_input.close();                        // Закрываем файл

    return {true, text_in_string}; // Файл прошел все проверки
}

/////////////////////////////////////////////////////////////

// Функция для записи пронумерованного текста в TXT-файл (file_output_numbered.txt)
void write_outputFile_text_txt(vector<vector<string>> sentences_numbered)
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
// Функция для записи рифм в TXT-файл (file_output_rhymes.txt)
void write_outputFile_rhymes_txt(vector<WordData> rhymes_data)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    wstring wparts_of_speech;
    string parts_of_speech;

    parts_of_speech += "Поиск выполнялся по следующим частям речи: ";
    wparts_of_speech += GetActivePartsOfSpeech(buttons::SaveButtonFlags);
    parts_of_speech += wstring_to_utf8(wparts_of_speech);
    parts_of_speech += "\r\n";
    parts_of_speech += "Тип поиска: ";
    parts_of_speech += buttons::SaveButtonFlags.test(7) ? "Однородный" : "Неоднородный";
    file_output_rhymes << parts_of_speech << "\n\n"; // Запись частей речи в файл
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (WordData output : rhymes_data)
    {
        string wordInfo = "Слово: " + output.word;                                                // само слово
        string part_of_speech = "\nЧасть речи: " + output.part_of_speech;                         // часть речи слова
        string wordAmount = "\nКоличество встреч в тексте: " + to_string(output.amount);          // Количество найденных слов
        string rhymesAmount = "\nКоличество рифмующихся пар: " + to_string(output.rhymed_amount); // Количество рифм
        string sentenceAmount = "\nНайдено в следующих предложениях: ";                           // Кол-во предложений с упоминанием

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
        else
            sentenceAmount += "не найдено";

        // вывод рифм
        if (!output.rhymed_words.empty())
        {
            file_output_rhymes << "\nРифмующиеся пары: ";
            for (string &word : output.rhymed_words)
                file_output_rhymes << "\n  - " << word;
        }
        else
            file_output_rhymes << "\nРифмующихся пар не найдено";
        file_output_rhymes << "\n\n";
    }

    file_output_rhymes.close();
}

// Возвращает цвет HTML по части речи
string get_color(const string &part_of_speech)
{
    if (part_of_speech == "глагол")
        return "red"; // красный
    if (part_of_speech == "наречие")
        return "purple"; // фиолетовый
    if (part_of_speech == "прилагательное")
        return "green"; // зеленый
    if (part_of_speech == "существительное")
        return "blue"; // синий
    if (part_of_speech == "причастие")
        return "Teal"; // бирюзовый
    if (part_of_speech == "деепричастие")
        return "DarkGoldenRod"; // коричневый
    return "black";
}
// Функция для записи пронумерованного текста в HTML-файл (file_output_numbered.html)
void write_outputFile_text_html(const vector<WordData> &rhymes_data, const vector<vector<string>> &sentences_numbered)
{
    // Создаем отображение слово+часть_речи -> цвет
    unordered_map<string, string> word_to_color;
    for (const auto &entry : rhymes_data)
    {
        string key = entry.word + "|" + entry.part_of_speech;
        word_to_color[key] = get_color(entry.part_of_speech);
    }

    file_output_numbered << "<!DOCTYPE html>\n<html>\n<head><meta charset=\"UTF-8\"><title>Text</title></head>\n<body>\n";

    for (size_t i = 0; i < sentences_numbered.size(); ++i)
    {
        for (const auto &word : sentences_numbered[i])
        {
            bool colored = false;
            for (const auto &entry : rhymes_data)
            {
                if (word == entry.word)
                {
                    string key = entry.word + "|" + entry.part_of_speech;
                    string color = word_to_color[key];
                    file_output_numbered << "<span style=\"color:" << color << "\">" << word << "</span> ";
                    colored = true;
                    break;
                }
            }
            if (!colored)
            {
                file_output_numbered << word << " ";
            }
        }
        file_output_numbered << "<br>\n";
    }

    file_output_numbered << "</body>\n</html>";
    file_output_numbered.close();
}
// Функция для записи рифм в HTML-файл (file_output_rhymes.html)
void write_outputFile_rhymes_html(const vector<WordData> &rhymes_data)
{
    // Заголовок HTML
    file_output_rhymes << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n"
                       << "<title>Рифмы</title>\n</head>\n<body>\n";

    file_output_rhymes << "<p>Поиск выполнялся по следующим частям речи:</p>\n<ul>\n";

    vector<pair<string, string>> parts = {
        {"глагол", "выделен в тексте красным"},
        {"наречие", "выделено в тексте фиолетовым"},
        {"прилагательное", "выделено в тексте зелёным"},
        {"существительное", "выделено в тексте синим"},
        {"причастие", "выделено в тексте бирюзовым"},
        {"деепричастие", "выделено в тексте коричневым"}};

    for (const auto &[pos, description] : parts)
    {
        std::string color = get_color(pos);
        file_output_rhymes << "<li>" << static_cast<char>(toupper(pos[0])) << pos.substr(1)
                           << " - <span style='color:" << color << ";'>" << description << "</span></li>\n";
    }

    file_output_rhymes << "<p><strong>Тип поиска:</strong> "
                       << (buttons::SaveButtonFlags.test(7) ? "Однородный" : "Неоднородный") << "<br>\n\n\n";

    // Вывод рифм
    for (const auto &output : rhymes_data)
    {
        string color = get_color(output.part_of_speech);

        file_output_rhymes << "<div style='margin-bottom:20px;'>\n";
        file_output_rhymes << "<strong>Слово:</strong> "
                           << "<span style='color:" << color << ";'>" << output.word << "</span><br>\n";

        file_output_rhymes << "<strong>Часть речи:</strong> " << output.part_of_speech << "<br>\n";
        file_output_rhymes << "<strong>Количество встреч в тексте:</strong> " << output.amount << "<br>\n";
        file_output_rhymes << "<strong>Количество рифмующихся пар:</strong> " << output.rhymed_amount << "<br>\n";

        file_output_rhymes << "<strong>Найдено в следующих предложениях:</strong> ";
        if (!output.sentence_counter.empty())
        {
            for (size_t i = 0; i < output.sentence_counter.size(); ++i)
            {
                if (i > 0)
                    file_output_rhymes << ", ";
                file_output_rhymes << output.sentence_counter[i];
            }
        }
        else
        {
            file_output_rhymes << "не найдено";
        }
        file_output_rhymes << "<br>\n";

        if (!output.rhymed_words.empty())
        {
            file_output_rhymes << "<strong>Рифмующиеся пары:</strong><ul>\n";
            for (const std::string &rhyme : output.rhymed_words)
            {
                file_output_rhymes << "<li>" << rhyme << "</li>\n";
            }
            file_output_rhymes << "</ul>\n";
        }
        else
        {
            file_output_rhymes << "<strong>Рифмующихся пар не найдено</strong><br>\n";
        }

        file_output_rhymes << "</div>\n<hr>\n";
    }

    file_output_rhymes << "</body>\n</html>";
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
    string outputFileName_numbered = inputFilePath.substr(0, inputFilePath.find_last_of('.')) + "_numbered.html"; // имя выходного файла-текста
    string outputFileName_rhymes = inputFilePath.substr(0, inputFilePath.find_last_of('.')) + "_rhymes.html";     // имя выходного файла-рифм

    int count_numb = 1; // счетчик для нумерации

    // Проверка наличия файла-текста с данным именем
    while (check_outputFile_existence(outputFileName_numbered))
    {
        // Удалить ".html"
        outputFileName_numbered.erase(outputFileName_numbered.length() - len_html, len_html);

        // Удалить "(n)", если есть
        size_t pos = outputFileName_numbered.rfind('(');
        if (pos != std::string::npos && outputFileName_numbered.back() == ')')
            outputFileName_numbered.erase(pos); // Удаляет от '(' до конца

        // Добавить новую нумерацию
        outputFileName_numbered += "(" + to_string(count_numb) + ").html";
        count_numb++;
    }

    int count_rhymes = 1; // счетчик для нумерации

    // Проверка наличия файла-рифм с данным именем
    while (check_outputFile_existence(outputFileName_rhymes))
    {
        // Удалить ".html"
        outputFileName_rhymes.erase(outputFileName_rhymes.length() - len_html, len_html);

        // Удалить "(n)", если есть
        size_t pos = outputFileName_rhymes.rfind('(');
        if (pos != std::string::npos && outputFileName_rhymes.back() == ')')
            outputFileName_rhymes.erase(pos); // Удаляет от '(' до конца

        // Добавить новую нумерацию
        outputFileName_rhymes += "(" + to_string(count_rhymes) + ").html";
        count_rhymes++;
    }

    return {outputFileName_numbered, outputFileName_rhymes};
}

// Функция для преобразования данных рифм в UTF-8
void convert_rhymes_data_to_utf8(std::vector<WordData> &rhymes_data)
{
    for (auto &entry : rhymes_data)
    {
        entry.word = ansi_to_utf8(entry.word);
        entry.part_of_speech = ansi_to_utf8(entry.part_of_speech);
        for (auto &rhymed_word : entry.rhymed_words)
        {
            rhymed_word = ansi_to_utf8(rhymed_word);
        }
    }
}

//  Функция для преобразования текста в UTF-8
void convert_sentences_to_utf8(std::vector<std::vector<std::string>> &sentences_numbered)
{
    for (auto &sentence : sentences_numbered)
    {
        for (auto &word : sentence)
        {
            word = ansi_to_utf8(word);
        }
    }
}

// Функция для создания файла и его открытия
bool outputFiles_working(const string inputFilePath,                // путь входного файла
                         string &outputFileName_numbered,           // адрес-имя выходного файла-текста
                         string &outputFileName_rhymes,             // адрес-имя выходного файла-рифм
                         vector<vector<string>> sentences_numbered, // вектор пронумерованных предложений
                         vector<WordData> rhymes_data)              // информация о рифмах
{
    convert_rhymes_data_to_utf8(rhymes_data);      // преобразование данных рифм в UTF-8
    convert_sentences_to_utf8(sentences_numbered); // преобразование текста в UTF-8

    // получаем имена выходных файлов из пути входного файла
    pair<string, string> fromFunct = create_outputFileNames(inputFilePath);
    outputFileName_numbered = fromFunct.first; // имя выходного файла-текста
    outputFileName_rhymes = fromFunct.second;  // имя выходного файла-рифм

    // открытие файлов на запись
    file_output_numbered.open(outputFileName_numbered, ios_base::out | ios::trunc);
    file_output_rhymes.open(outputFileName_rhymes, ios_base::out | ios::trunc);

    if (file_output_numbered.is_open() and file_output_rhymes.is_open())
    {
        write_outputFile_text_html(rhymes_data, sentences_numbered); // запись пронумерованного текста
        write_outputFile_rhymes_html(rhymes_data);                   // запись рифм
    }
    else
        return false;
}

#endif // FILE_WORKING_H