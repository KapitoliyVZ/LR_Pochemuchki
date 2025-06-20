//   Данный заголовочный файл содержит функции и константы для работы с файлами ввода и вывода, реализует :
//    - Проверку и открытие входных файлов, обработку ошибок открытия и чтения.
//    - Генерацию и проверку имён выходных файлов(TXT / HTML) для сохранения результатов анализа текста и рифм.
//    - Запись результатов анализа текста и рифм в текстовые и HTML - файлы с форматированием и цветовым выделением частей речи.
//    - Вспомогательные функции для преобразования кодировок, получения суффиксов частей речи и типа поиска, проверки существования файлов.
//    - Использует глобальные потоки для работы с файлами, а также структуры данных для хранения информации о словах, рифмах и предложениях.


#pragma once
#ifndef FILE_WORKING_H
#define FILE_WORKING_H

#include "UI_const.h"

ifstream file_input;         // файл для чтения
ofstream file_output_text;   // файл-текст для записи
ofstream file_output_rhymes; // файл-рифмы для записи

const string extension_txt = ".txt";   // расширение .txt
const int len_txt = 4;                 // длина расширения .txt
const string extension_html = ".html"; // расширение .html
const int len_html = 5;                // длина расширения .html

const string error_opening_file = utf8_to_ansi("Ошибка!: Не удалось открыть файл!");           // текст ошибки при открытии файла
const string error_empty_file = utf8_to_ansi("Не удалось выполнить чтение файла: Файл пуст!"); // текст ошибки при пустом файле
const string error_wrong_extension_file = utf8_to_ansi("Ошибка!: Неверное расширение!");       // текст ошибки при неверном расширении
const string error_empty_path_file = utf8_to_ansi("Ошибка!: Пустой путь к файлу!");            // текст ошибки при пустом пути к файлу
const string error_wrong_encoding_file = utf8_to_ansi("Ошибка!: Неверная кодировка файла!");   // текст ошибки при неверной кодировке файла

// Проверка наличия расширения .txt у файла для чтения
bool check_txt_extension(string file_name)
{
    if (file_name.length() >= len_txt && file_name.substr(file_name.length() - len_txt) == extension_txt)
        return true; // если есть расширение
    else
        return false; // если нет расширения
}

// Проверка, является ли файл для чтения пустым
bool check_inputFile_is_empty(const string file_name)
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
        return {false, error_empty_path_file}; // Путь к файлу пустой

    string path = inputFilePath;
    // Удаление кавычек в начале и конце строки, если они есть
    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    if (!check_txt_extension(path))
        return {false, error_wrong_extension_file}; // Неверное расширение

    // Открытие файла через глобальный ifstream file_input
    file_input.open(path, ios_base::in);

    if (!file_input.is_open())
        return {false, error_opening_file}; // Не удалось открыть файл

    if (check_inputFile_is_empty(path))
        return {false, error_empty_file}; // файл пустой

    string text_in_string; // текст файла

    // Чтение всего текста из файла
    getline(file_input, text_in_string, '\0'); // Чтение до конца файла
    file_input.close();                        // Закрываем файл

    return {true, text_in_string}; // Файл прошел все проверки
}

/////////////////////////////////////////////////////////////

// Функция для записи пронумерованного текста в TXT-файл (file_output_text.txt)
void write_outputFile_text_txt(vector<vector<string>> sentences_numbered)
{
    const set<string> punctuationMarks = {".", ",", "!", "?", ":", ";", "-", "(", ")", "\"", "'"};
    for (const vector<string> &sentence : sentences_numbered)
    {
        bool firstWord = true;
        for (const string &word : sentence)
        {
            if (!firstWord && punctuationMarks.find(word) == punctuationMarks.end())
                file_output_text << " ";
            file_output_text << word;
            firstWord = false;
        }

        file_output_text << "\n"; // Перенос строки после предложения
    }
    file_output_text.close();
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

// Функция возвращает цвет HTML по части речи
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
        return "DarkGoldenRod"; // желтый
    return "black";
}
// Функция для записи пронумерованного текста в HTML-файл (file_output_text.html)
void write_outputFile_text_html(const vector<WordData> &rhymes_data, const vector<vector<string>> &sentences_numbered)
{
    // Создаем отображение слово+часть_речи -> цвет
    unordered_map<string, string> word_to_color;
    for (const auto &entry : rhymes_data)
    {
        string key = entry.word + "|" + entry.part_of_speech;
        word_to_color[key] = get_color(entry.part_of_speech);
    }

    file_output_text << "<!DOCTYPE html>\n<html>\n<head><meta charset=\"UTF-8\"><title>Текст</title></head>\n<body>\n";

    // проходим по предложениям
    for (size_t i = 0; i < sentences_numbered.size(); ++i)
    {
        // проходим по словам
        for (const auto &word : sentences_numbered[i])
        {
            bool colored = false; // Переменная для проверки, было ли слово раскрашено

            // проходим по рифмам
            for (const auto &entry : rhymes_data)
            {
                // Проверяем, совпадает ли слово с текущим словом из рифм
                if (word == entry.word)
                {
                    string key = entry.word + "|" + entry.part_of_speech;
                    string color = word_to_color[key];
                    file_output_text << "<span style=\"color:" << color << "\">" << word << "</span> ";
                    colored = true;
                    break;
                }
            }
            // Если не найдено, ищем среди рифмующихся слов (выделяем розовым)
            if (!colored)
            {
                bool rhymed = false;
                for (const auto &entry : rhymes_data)
                {
                    // Проверяем, есть ли слово в рифмующихся словах и есть ли слово для сравнения
                    if (std::find(entry.rhymed_words.begin(), entry.rhymed_words.end(), word) != entry.rhymed_words.end() && buttons::SaveButtonFlags.test(6))
                    {
                        file_output_text << "<span style=\"color:fuchsia\">" << word << "</span> ";
                        rhymed = true;
                        break;
                    }
                }
                if (!rhymed)
                {
                    file_output_text << word << " ";
                }
            }
        }
        file_output_text << "<br>\n";
    }

    file_output_text << "</body>\n</html>";
    file_output_text.close();
}
// Функция для записи рифм в HTML-файл (file_output_rhymes.html)
void write_outputFile_rhymes_html(const vector<WordData> &rhymes_data)
{
    // Заголовок HTML
    file_output_rhymes << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n"
                       << "<title>Рифмы</title>\n</head>\n<body>\n";

    file_output_rhymes << "<p>Поиск выполнялся по следующим частям речи:</p>\n<ul>\n";

    vector<tuple<int, string, string>> parts = {
        {0, "глагол", "выделен в тексте красным"},
        {1, "наречие", "выделено в тексте фиолетовым"},
        {2, "прилагательное", "выделено в тексте зелёным"},
        {3, "существительное", "выделено в тексте синим"},
        {4, "причастие", "выделено в тексте бирюзовым"},
        {5, "деепричастие", "выделено в тексте жёлтым"}};

    // Вывод заголовка частей речи с их цветами и описаниями
    for (const auto &[flag_index, part, description] : parts)
    {
        // Проверяем, активна ли данная часть речи
        if (buttons::SaveButtonFlags.test(flag_index))
        {
            string color = get_color(part);
            string label = static_cast<char>(toupper(part[0])) + part.substr(1); // Первая буква заглавная
            file_output_rhymes << "<li>" << label << " - "
                               << "<span style='color:" << color << ";'>" << description << "</span></li>\n";
        }
    }
    file_output_rhymes << "</ul>\n";

    // Проверяем, есть ли рифмующиеся слова хотя бы у одного объекта
    bool has_rhymed = false;
    for (const auto &entry : rhymes_data)
    {
        if ((!entry.rhymed_words.empty() || !entry.rhymed_words_sentences_number.empty()) && buttons::SaveButtonFlags.test(6))
        {
            has_rhymed = true;
            break;
        }
    }
    if (has_rhymed)
    {
        file_output_rhymes << "Рифмующиеся слова в тексте — <span style='color:fuchsia;'>выделены в тексте розовым</span></li>\n";
    }

    file_output_rhymes << "</ul>\n";

    // Тип поиска
    file_output_rhymes << "<p><strong>Тип поиска:</strong> "
                       << (buttons::SaveButtonFlags.test(7) ? "Однородный" : "Неоднородный") << "<br>\n"
                       << "Вывод слов осуществляется по мере их появлений в тексте.</p><hr>\n";

    // Вывод рифм
    for (const auto &output : rhymes_data)
    {
        string color = get_color(output.part_of_speech);

        file_output_rhymes << "<div style='margin-bottom:20px;'>\n";
        file_output_rhymes << "<strong>Слово:</strong> "
                           << "<span style='color:" << color << ";'>" << output.word << "</span><br>\n";

        file_output_rhymes << "<strong>Часть речи:</strong> " << output.part_of_speech << "<br>\n";
        file_output_rhymes << "<strong>Количество появлений в тексте:</strong> " << output.amount << "<br>\n";
        file_output_rhymes << "<strong>Количество рифмующихся пар:</strong> " << output.rhymed_amount << "<br>\n";

        file_output_rhymes << "<strong>Найдено в следующем(-их) предложении(-ях):</strong> ";
        if (!output.sentence_counter.empty())
        {
            for (size_t i = 0; i < output.sentence_counter.size(); ++i)
            {
                if (i > 0)
                    file_output_rhymes << ", ";
                file_output_rhymes << output.sentence_counter[i]; // вывод номера предложения данного слова.
            }
        }
        else
        {
            file_output_rhymes << "не найдено";
        }
        file_output_rhymes << "<br>\n";

        if (!output.rhymed_words.empty())
        {
            file_output_rhymes << "<strong>Рифмующееся(-иеся) слово(-а) (номер(-а) предложения(-ий), в котором(-ых) появляется(-ются)):</strong><ul>\n";
            for (size_t i = 0; i < output.rhymed_words.size(); ++i)
            {
                file_output_rhymes << "<li>" << output.rhymed_words[i];
                // Выводим номера предложений для этого слова
                if (i < output.rhymed_words_sentences_number.size() && !output.rhymed_words_sentences_number[i].empty())
                {
                    file_output_rhymes << " (номер(-а) предложения(-й): ";
                    for (size_t j = 0; j < output.rhymed_words_sentences_number[i].size(); ++j)
                    {
                        if (j > 0)
                            file_output_rhymes << ", ";
                        file_output_rhymes << output.rhymed_words_sentences_number[i][j];
                    }
                    file_output_rhymes << ")";
                }
                file_output_rhymes << "</li>\n";
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

/////////////////////////////////////////////////////////////

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

// Функция для получения суффикса частей речи
string get_parts_of_speech_suffix()
{
    vector<string> parts;
    if (buttons::SaveButtonFlags.test(0))
        parts.push_back("verb"); // глагол
    if (buttons::SaveButtonFlags.test(1))
        parts.push_back("adv"); // наречие
    if (buttons::SaveButtonFlags.test(2))
        parts.push_back("adj"); // прилагательное
    if (buttons::SaveButtonFlags.test(3))
        parts.push_back("noun"); // существительное
    if (buttons::SaveButtonFlags.test(4))
        parts.push_back("participle"); // причастие
    if (buttons::SaveButtonFlags.test(5))
        parts.push_back("adverbial"); // деепричастие

    string result;
    for (size_t i = 0; i < parts.size(); ++i)
    {
        if (i > 0)
            result += "-";
        result += parts[i];
    }
    return result;
}

// Функция для получения суффикса типа поиска
string get_search_type_suffix()
{
    return buttons::SaveButtonFlags.test(7) ? "homo" : "hetero";
}

// Функция для создания имён выходных файлов
pair<string, string> create_outputFileNames(const string inputFilePath, string &compare_word)
{
    // Создание имен выходных файлов на основе полного пути входного файла

    string baseName = inputFilePath.substr(0, inputFilePath.find_last_of('.'));
    string part_suffix = get_parts_of_speech_suffix();
    string type_suffix = get_search_type_suffix();

    string outputFileName_numbered = baseName + "_TEXT_" + (compare_word == "" ? (part_suffix + "_" + type_suffix) : (type_suffix == "homo" ? (compare_word + "_" + type_suffix) : (compare_word + "_" + part_suffix + "_" + type_suffix))) + ".html";
    string outputFileName_rhymes = baseName + "_RHYMES_" + (compare_word == "" ? (part_suffix + "_" + type_suffix) : (type_suffix == "homo" ? (compare_word + "_" + type_suffix) : (compare_word + "_" + part_suffix + "_" + type_suffix))) + ".html";

    int count_numb = 1; // счетчик для нумерации

    // Проверка наличия файла-текста с данным именем
    while (check_outputFile_existence(outputFileName_numbered))
    {
        // Удалить ".html"
        outputFileName_numbered.erase(outputFileName_numbered.length() - len_html, len_html);

        // Удалить "(n)", если есть
        size_t pos = outputFileName_numbered.rfind('(');
        if (pos != string::npos && outputFileName_numbered.back() == ')')
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
        if (pos != string::npos && outputFileName_rhymes.back() == ')')
            outputFileName_rhymes.erase(pos); // Удаляет от '(' до конца

        // Добавить новую нумерацию
        outputFileName_rhymes += "(" + to_string(count_rhymes) + ").html";
        count_rhymes++;
    }

    return {outputFileName_numbered, outputFileName_rhymes};
}

// Функция для преобразования данных рифм в UTF-8
void convert_rhymes_data_to_utf8(vector<WordData> &rhymes_data)
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
void convert_sentences_to_utf8(vector<vector<string>> &sentences_numbered)
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
                         vector<WordData> rhymes_data,              // информация о рифмах
                         string &compare_word)
{
    convert_rhymes_data_to_utf8(rhymes_data);      // преобразование данных рифм в UTF-8
    convert_sentences_to_utf8(sentences_numbered); // преобразование текста в UTF-8

    // получаем имена выходных файлов из пути входного файла
    pair<string, string> fromFunct = create_outputFileNames(inputFilePath, compare_word);
    outputFileName_numbered = fromFunct.first; // имя выходного файла-текста
    outputFileName_rhymes = fromFunct.second;  // имя выходного файла-рифм

    // открытие файлов для записи
    file_output_text.open(outputFileName_numbered, ios_base::out | ios::trunc);
    file_output_rhymes.open(outputFileName_rhymes, ios_base::out | ios::trunc);

    if (file_output_text.is_open() and file_output_rhymes.is_open())
    {
        write_outputFile_text_html(rhymes_data, sentences_numbered); // запись пронумерованного текста
        write_outputFile_rhymes_html(rhymes_data);                   // запись рифм
    }
    else
        return false;
}

#endif // FILE_WORKING_H