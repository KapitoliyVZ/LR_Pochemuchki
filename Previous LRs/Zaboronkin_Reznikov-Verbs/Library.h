#pragma once

#include "Header.h"

int count_output = 0;

string entering_name_file(string);                         // функция меню ввода и проверки имени  файла (2й параметр: input - входной файл, output - выходной файл)
bool check_name_file(string);                              // функция проверки имени файла на наличие неверных символов
bool check_txt_extension(string);                          // функция проверки на наличия расширения '.txt' в конце имени файла
bool open_file(string, string);                            // функция проверки открытия файла для чтения. (2й параметр: input - входной файл, output - выходной файл)
bool check_inputFile_is_empty(string);                     // функция проверки файла на пустоту
void print_text_string(string);                            // (ВЫВОД) функция вывода данных из файла на консоль
void output_rhymes_in_file(map<string, vector<VerbInfo>>); // Функция вывода данных в файл

// Функция меню ввода имени файла
string entering_name_file(string type_file)
{
    string user_answer = ""; // данные, вводимые пользователем
    bool flag_cycle = true;  // true - выход из цикла
    // system("cls");
    do
    {
        if (type_file == "input")
            cout << "Введите имя входного файла для чтения данных.\n";
        else if (type_file == "output")
            cout << "Введите имя выходного файла для записи в него данных.\n";
        else
        {
            cout << "!!!wrong type_file (input or output) in code!!! (29)\n";
            exit(2);
        }
        cout << "Вы можете использовать только латинские буквы, цифры и '_'.\n"
             << "Расширение '.txt' вводится по желанию.\n";

        cout << "\n~";
        getline(cin, user_answer);

        // проверка на клавиши навигации
        if (((int)user_answer[0] == FINISH_button or (int) user_answer[0] == BACK_button) and user_answer.length() == 1)
        {
            system("cls");
            return user_answer;
        }

        // проверка имени файла
        if (!check_name_file(user_answer))
        {
            flag_cycle = false;
            continue;
        }
        else
        {
            string file_name = user_answer;

            // добавление расширения файла, если его нет
            if (!check_txt_extension(file_name))
                file_name += ".txt";

            // проверка открытия файла и проверка на пустоту
            if (open_file(file_name, type_file))
            {
                // если открытие файла успешно
                return file_name;
            }
            else
                flag_cycle = false; // повторить цикл
        }
    } while (!flag_cycle);
    return user_answer;
}
// Функция проверки имени файла на наличие неверных символов
bool check_name_file(string file_name)
{
    if (file_name.size() == 0)
    {
        system("cls");
        cout << "Ошибка!: Имя файла не может быть пустым.\n"
             << "Попробуйте заново.\n\n";
        return false;
    }
    if (file_name.length() > MAX_FILENAME_LENGTH)
    {
        cout << "Ошибка!: Имя файла превышает допустимую длину в "
             << MAX_FILENAME_LENGTH << " символов!\n"
             << "Попробуйте заново.\n\n";
        return false;
    }
    int count = 0; // счетчик неверных символов
    for (auto i = 0; i < file_name.length(); i++)
    {
        // проверка на не-цифры, не-буквы и не-"_" в имени файла
        if (!isdigit(file_name[i]) && !isalpha(file_name[i]) && file_name[i] != '_' && file_name[i] != ')' && file_name[i] != '(')
            count++;
    }
    // проверка на кол-во неверных символов и наличия расширения файла
    if ((count == 1 and check_txt_extension(file_name)) or (count == 0))
        // если нет неверных символов (.txt не учитывается)
        return true;
    else // если есть неверные символы
    {
        system("cls");
        cout << "Ошибка!: Неверный символ в имени файла '" << file_name << "'.\n"
             << "Попробуйте заново.\n\n";
        return false;
    }
}
// Функция проверки имени файла на наличия расширения '.txt'
bool check_txt_extension(string file_name)
{
    if (file_name.length() >= len_txt && file_name.substr(file_name.length() - len_txt) == extension_txt)
        return true; // если есть расширение
    else
        return false; // если нет расширения
}
// Функция проверки открытия/создания файла
bool open_file(string file_name, string type_file)
{
    if (type_file == "input")
    {
        // открытие файла на чтение
        file_input.open(file_name, ios_base::in);

        // если файл открылся
        if (file_input.is_open())
        {
            // если файл не пустой -> вернуть - true
            if (!check_inputFile_is_empty(file_name))
            {
                // system("cls");
                // cout << "Входной файл '" << file_name << "' открыт для чтения.\n";
                return true;
            }
            else
            {
                system("cls");
                cout << "Ошибка!: Входной файл '" << file_name << "' не содержит данных. Чтение невозможно.\n"
                     << "Попробуйте заново.\n\n";
                return false;
            }
        }
        else
        {
            system("cls");
            cout << "Ошибка!: В системе отсутствует файл с именем: '" << file_name << "'.\n"
                 << "Попробуйте заново.\n\n";
            return false;
        }
    }
    else if (type_file == "output")
    {
        // открытие файла на запись
        file_output.open(file_name, ios_base::out);

        // если файл открылся -> вернуть - true
        if (file_output.is_open())
        {
            // system("cls");
            // cout << "Выходной файл '" << file_name << "' создан и открыт для записи.\n";
            return true;
        }
        else
        {
            // system("cls");
            cout << "Ошибка в создании файла '" << file_name << "' на запись!\n";
            return false;
        }
    }
    else
    {
        cout << "!!!wrong type (rhymes or text) in code (170)!!!\n";
        exit(2);
    }
}
// Функция проверки файла на пустоту
bool check_inputFile_is_empty(string file_name)
{
    // true - файл пустой
    // false - файл не пустой

    if (file_input.peek() == EOF) // если первый символ это конец файла
    {
        system("cls");
        cout << "Входной файл '" << file_name << "' пуст. Чтение невозможно.\n"
             << "Попробуйте заново.\n";

        file_input.close();
        return true;
    }
    else
        return false;
}
// Функция проверки наличия файла с данным именем в системе
bool check_outputFile_existence(string file_name)
{
    ifstream file(file_name); // Пытаемся открыть файл

    // Проверяем, удалось ли открыть файл
    if (file.good()) // true - файл существует
    {
        // system("cls");
        // cout << "Ошибка!: Файл с именем '" << file_name << "' уже существует.\n";
        return true;
    }
    else
        return false;
}

// Функция автоматического ввода имени файла для рифм
pair<bool, string> auto_name_OutputFile(string input_file_name, string type)
{
    int count = 1;
    input_file_name.erase(input_file_name.length() - len_txt, len_txt); // Удаляем расширение у входного файла
    string file_name;
    if (type == "rhymes")
        file_name = input_file_name + "(Rhymes).txt"; // Имя выходного файла с рифмами
    else if (type == "text")
        file_name = input_file_name + "(NumbText).txt"; // Имя выходного файла с преобразованным текстом
    else
    {
        cout << "!!!wrong type (rhymes or text) in code!!! (220)\n";
        exit(2);
    }
    // если имя файла с рифмами существует
    while (check_outputFile_existence(file_name))
    {
        file_name.erase(file_name.length() - len_txt, len_txt); // Удаляем расширение
        if (count > 1)
            file_name.erase(file_name.length() - 3, 3); // Удаляем нумерацию, если она уже была
        file_name += "(" + to_string(count) + ").txt";
        count++;
    }

    if (open_file(file_name, "output"))
        return {true, file_name};
    else
        return {false, file_name};
}

// Функция заполнения файла рифмами
void output_rhymes_in_file(map<string, vector<VerbInfo>> all_rhymes)
{
    int counter_verb = 0;
    int selection_num = 0;
    int unique_verbs_num = 0;

    file_output << "Далее в файле будет использоваться следующее сокращение: \n"
                   "'Глагол'(Номер предложения, порядковый номер этого глагола в этом предложении) \n"
                   "Например: работать (2,3)"
                << ".\n";
    file_output << "----------------------------------------------------" << endl;
    for (const auto &elm : all_rhymes)
    {
        if (elm.second.size() <= 1)
        {
            continue;
        }
        selection_num += 1;
        file_output << "Набор рифмованных пар #" << selection_num << "\n"
                    << endl;

        unique_verbs_num += elm.second.size();

        // // Лямбда функция сортировки вектора структур
        // for (auto &[key, verbVector] : all_rhymes)
        // {
        //     sort(verbVector.begin(), verbVector.end(), [](const VerbInfo &a, const VerbInfo &b)
        //          {
        //              return a.word < b.word; // Сравнение по полю `word`
        //          });
        // }

        for (auto it = all_rhymes.begin(); it != all_rhymes.end(); ++it)
        {
            auto &verbVector = it->second;
            sort(verbVector.begin(), verbVector.end(), [](const VerbInfo &a, const VerbInfo &b)
                 { return a.word < b.word; });
        }

        for (size_t i = 0; i < elm.second.size(); i++)
        {
            unique_verbs_num = 0;
            for (size_t j = 0; j < elm.second.size(); j++)
            {
                if (elm.second[i].word == elm.second[j].word)
                {
                    continue;
                }
                file_output << elm.second[i].word << "(" << elm.second[i].sentenceNum << "," << elm.second[i].wordNum << ") - "
                            << elm.second[j].word << "(" << elm.second[j].sentenceNum << "," << elm.second[j].wordNum << ")" << endl;
                counter_verb++;
                unique_verbs_num++;
            }
            file_output << endl
                        << "Слово '" << elm.second[i].word << "' найдено в качестве рифмы " << unique_verbs_num << " раз." << endl
                        << endl;
        }
        file_output << "----------------------------------------------------" << endl;
    }
    file_output << "\nОбщее число пар: " << counter_verb << endl;
    file_output << "\nОбщее число наборов рифм: " << selection_num << endl;
    file_output << "----------------------------------------------------" << endl;
    file_output.close();
}

// Функция заполнения файла преобразованным текстом
void output_newText_in_file(vector<vector<string>> text_in_vector)
{
    for (size_t i = 0; i < text_in_vector.size(); ++i)
    {
        file_output << (i + 1) << ". "; // Номер предложения
        for (const auto &word : text_in_vector[i])
        {
            file_output << word << " "; // Вывод слов через пробел
        }
        file_output << "\n"; // Переход на новую строку после каждого предложения
    }

    file_output.close();
}

// Функция заполнения файла преобразованным текстом
void output_newText_in_file(string text_in_string)
{

    // Регулярное выражение для разделения текста на предложения
    // разделить предложения, заканчивающиеся на !?., и если после них идут ковычки, скобки
    // regex sentence_regex(R"([^.?!]+[.?!])"); 
    regex sentence_regex(R"([^.?!]+[.?!]['"»«)()]?)");
    //regex sentence_regex(R"((«.*?»|".*?"|[^.?!]+[.?!]))");
    sregex_iterator begin(text_in_string.begin(), text_in_string.end(), sentence_regex);
    sregex_iterator end;

    // Удаление всех символов переноса строки
    // text_in_string.erase(remove(text_in_string.begin(), text_in_string.end(), '\n'), text_in_string.end());

    int sentence_number = 1;
    for (auto it = begin; it != end; ++it)
    {
        string sentence = it->str();

        sentence.erase(0, sentence.find_first_not_of(' ')); // Удаляем пробелы в начале предложения
        // sentence.erase(0, sentence.find_first_not_of('»'));
        // sentence.erase(0, sentence.find_first_not_of(')'));
        // sentence.erase(0, sentence.find_first_not_of('\n'));
        sentence.erase(remove(sentence.begin(), sentence.end(), '\n'), sentence.end()); // Удаление всех символов переноса строки 

        // Записываем предложение в файл с нумерацией
        file_output << sentence_number++ << ". " << sentence << "\n";
    }
    file_output.close();
}