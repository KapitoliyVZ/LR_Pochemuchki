#include "Library.h"
#include "Header.h"

vector<set<string>> verbEndings = {}; // вектор окончаний для распознавания глагола

int main()
{
    setlocale(LC_ALL, "Russian");

    start_menu();
    return 0;
}

// Функция начального меню
void start_menu()
{
    bool flag_cycle = false; // true - выход из цикла

    do
    {
        flag_cycle = false;
        char user_answer; // команда, вводимая пользователем

        system("cls");
        cout << setw(50) << "WElCOME\n"
             << setw(20) << " Программа предназначена для поиска рифмующихся глаголов из входного текста. \n"
             << setw(20) << " Глаголы распознаются на основе окончаний, содержащихся в файле 'VerbEndings.txt' в каталоге программы.\n"
             << setw(20) << " Входной файл с текстом должен иметь формат '.txt.' \n\n"
             << setw(20) << " Найденные пары рифм записываются в новый файл с именем файла исходного текст + (Rhymes).\n"
             << setw(20) << " Входной текст с указанием нумерации предложениq записывается в новый файл с именем файла исходного текста + (NumbText).\n"
             << setw(20) << " Для избежания создания файла с именем существующего файла программа автоматически присваивает нумерацию созданным файлам.\n\n"
             << setw(10) << " На каждом меню у пользователя есть возможность воспользоваться клавишами навигации для:\n"
             << setw(10) << "'ENTER'- выбрать/подтвердить действие и перейти в следующее меню.\n"
             << setw(10) << "  '/'  - вернуться назад к предыдущему меню без сохранения настроек.\n"
             << setw(10) << "  '*'  - завершить программу.\n\n\n"
             << setw(70) << "Нажмите любую клавишу для старта программы.";
        user_answer = _getch();

        if (user_answer != FINISH_button) // если не-клавиша-завершить, то
            flag_cycle = make_input();
        else
            flag_cycle = true;

    } while (!flag_cycle);
}

// Функция открытия файла с окончаниями для глаголов
bool make_verb_endings()
{
    file_input.open("VerbEndings.txt");
    if (!file_input.is_open())
    {
        cout << "Ошибка!: Файл окончаний глагола отсутствует в каталоге проекта. Добавьте его и попробуйте снова. \n";
        _getch();
        return false;
    }

    fill_vector_by_endings(); // считать весь текст в вектор
    file_input.close();
    return true;
}

// Функция заполнения вектора окончаний глаголов из файла
void fill_vector_by_endings()
{
    // vector<set<string>> all_endings; // Двумерный вектор для хранения окончаний
    string line, word;
    bool flag = false;
    vector<string> words;
    // Чтение всего текста из файла
    while (!file_input.eof())
    {
        getline(file_input, line); // Чтение до конца файла
        if (line[0] == '1' and flag == false)
        {
            flag = true;
        }
        line.erase(0, 3);
        if (flag)
        {
            stringstream ss(line);
            while (ss >> word)
            {

                // Удаляем знаки в конце слова, если они есть
                if (!word.empty() && (word.back() == ',' || word.back() == '.'))
                    word.erase(word.size() - 1);

                words.push_back(word); // Добавляем слово в предложение
                set<string> bobobo;
                bobobo.insert(word);
                verbEndings.push_back(bobobo); // Добавляем предложение в общий массив
            }
        }
    }
}

// Функция для старта меню открытия файла на чтение
bool make_input()
{
    bool flag_cycle = false; // true - выход из цикла
    do
    {
        system("cls");

        if (!make_verb_endings()) // проверка наличия файла окончания
            return false;

        // Ввод имени файла
        string input_file_name = entering_name_file("input"); // (input - входной файл на чтение, output - выходной файл на запись)

        if (input_file_name[0] == FINISH_button)    // Если при вводе имени файла была кнопка "завершить", то
            return true;                            // выйти из фунции
        else if (input_file_name[0] == BACK_button) // Если при вводе имени файла была кнопка "назад", то
            return false;                           // повторить цикл

        // чтение текста входного файла
        pair<vector<vector<string>>, string> text = fill_vector_by_text();
        vector<vector<string>> text_in_vector = text.first; // текст в векторной записи
        string text_in_string = text.second;                // текст в строковой записи

        // найденные глаголы
        vector<VerbInfo> foundVerbs = findVerbsInSentences(text_in_vector, verbEndings);

        // найденные цепочки рифм
        map<string, vector<VerbInfo>> all_rhymes = findRhymingVerbs(foundVerbs);

        // вызов функции для выходных файлов
        if (make_output(all_rhymes, input_file_name, text_in_vector, text_in_string))
            return true;

    } while (!flag_cycle);
}

// Функция для переноса всего текста в вектор (или строку)
pair<vector<vector<string>>, string> fill_vector_by_text()
{
    vector<vector<string>> text_in_vector; // Двумерный вектор для хранения текста

    string text_in_string, sentence;

    // Чтение всего текста из файла
    getline(file_input, text_in_string, '\0'); // Чтение до конца файла
    file_input.close();                        // Закрываем файл

    //!!! ОБЪЯСНИТЬ

    // Используем регулярное выражение для разделения текста по точке, восклицательному и вопросительному знаку
    regex sentenceEndRegex("[.!?]+"); // Регулярное выражение для поиска разделителей (точка, восклицательный и вопросительный знаки)
    sregex_token_iterator sentenceIterator(text_in_string.begin(), text_in_string.end(), sentenceEndRegex, -1);
    sregex_token_iterator endIterator;

    // Проходим по каждому предложению
    while (sentenceIterator != endIterator)
    {
        sentence = *sentenceIterator++;
        if (!sentence.empty())
        {
            stringstream wordStream(sentence);
            string word;
            vector<string> words;

            // Чтение слов в предложении
            while (wordStream >> word)
            {
                // Удаляем все символы, кроме букв
                word.erase(remove_if(word.begin(),
                                     word.end(),
                                     [](unsigned char c)
                                     { return !isalpha(c); }),
                           word.end());

                // Если после удаления остались буквы, добавляем слово в список
                if (!word.empty())
                    words.push_back(word);
            }
            text_in_vector.push_back(words); // Добавляем предложение в общий вектор
        }
    }

    return {text_in_vector, text_in_string};
}

// Функция для записи глаголов в вектор
vector<VerbInfo> findVerbsInSentences(vector<vector<string>> text_in_vector, vector<set<string>> verbEndings)
{
    // Вектор для хранения информации о найденных глаголах
    vector<VerbInfo> foundVerbs;

    for (size_t i = 0; i < text_in_vector.size(); ++i)
        for (size_t j = 0; j < text_in_vector[i].size(); ++j)
        {
            const string &word = text_in_vector[i][j];
            if (isVerb(word, verbEndings))
                // Добавляем информацию о найденном глаголе в вектор
                foundVerbs.push_back({word, i + 1, j + 1}); // i + 1 и j + 1 для 1-индексации
        }

    // Вывод найденных глаголов на консоль
    // print_FoundVerbs(foundVerbs);

    return foundVerbs;
}

// Проверка, является ли слово глаголом на основе окончаний
bool isVerb(string word, vector<set<string>> verbEndings)
{
    // Вернуть:
    // true - если глагол
    // false - если не глагол

    for (const auto &endings : verbEndings)
        for (const auto &ending : endings)
            if (word.length() >= ending.length() && word.compare(word.length() - ending.length(), ending.length(), ending) == 0)
                return true; // Найдено соответствие, слово является глаголом

    return false;
}

// Функция нахождения рифм глаголов
map<string, vector<VerbInfo>> findRhymingVerbs(vector<VerbInfo> foundVerbs)
{
    map<string, vector<VerbInfo>> rhymeGroups; // вектор сгруппированных глаголов по окончаниям
    vector<VerbInfo> line;
    vector<string> all_verbs;
    set<string> processedVerbs;
    // Проходим по каждому набору окончаний
    // Для каждого окончания в наборе
    int end_len = 3;

    for (const auto &verb : foundVerbs)
    {
        end_len = 3;
        if (verb.word.length() >= end_len)
        {
            string ending = verb.word.substr(verb.word.length() - end_len, end_len);

            if (ending[2] == 'л' or ending[1] == 'л')
            {
                ending.erase(0, 1);
                end_len -= 1;
            }
            // cout << ending << endl;
            if (processedVerbs.find(verb.word) != processedVerbs.end())
                continue;

            vector<VerbInfo> group; // Группа глаголов с одним и тем же окончанием
            for (const auto &candidate : foundVerbs)
            {
                // Проверяем, совпадает ли окончание и был ли глагол уже обработан
                if (candidate.word.length() >= end_len &&
                    candidate.word.substr(candidate.word.length() - end_len, end_len) == ending &&
                    processedVerbs.find(candidate.word) == processedVerbs.end())
                {
                    group.push_back(candidate);
                    processedVerbs.insert(candidate.word); // Отмечаем глагол как обработанный
                }
            }

            // Добавляем группу в карту, если найдены совпадения
            if (!group.empty())
            {
                rhymeGroups[ending] = group;
            }
        }
    }
    // _getch();
    return rhymeGroups;
}

// Функция старта меню для открытия файла на запись
bool make_output(map<string, vector<VerbInfo>> all_rhymes, string input_file_name, vector<vector<string>> text_in_vector, string text_in_string)
{
    system("cls");

    // вызов функции автоматического наименования файла рифм
    pair<bool, string> result_rhymes = auto_name_OutputFile(input_file_name, "rhymes");
    bool succes_opening_rhymes = result_rhymes.first; // флаг успешности создания файла рифм
    string file_name_rhymes = result_rhymes.second;   // имя выходного файла рифм

    if (succes_opening_rhymes)
    {
        output_rhymes_in_file(all_rhymes); // Запись рифм в файл
        cout << "Пары рифм успешно сохранены в файле '" << file_name_rhymes << "'.\n";
    }

    // вызов функции автоматического наименования файла текст
    pair<bool, string> result_text = auto_name_OutputFile(input_file_name, "text");
    bool success_opening_text = result_text.first; // флаг успешности создания файла текста
    string file_name_text = result_text.second;    // имя выходного файла текста

    if (success_opening_text)
    {
        // output_newText_in_file(text_in_vector); // Запись текста-вектор в файл
        output_newText_in_file(text_in_string); // Запись текста-строка в файл
        cout << "Преобразованный текст сохранен в файле '" << file_name_text << "'.\n\n";
    }

    cout << "Нажмите любую кнопку, чтобы начать поиск рифмующихся глаголов в другом файле.\n"
         << "'*'  - завершить программу.\n";

    char user_answer = _getch(); // команда, вводимая пользователем

    if (user_answer == FINISH_button) // если клавиша-завершить, то
        return true;                  // выйти из программы
    else                              // иначе
        return false;                 // повторить цикл программы
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// (ВЫВОД) Функция для вывода на консоль всех слов из вектора с указанием номера предложения и номера слова в предложении
void print_text_vector(vector<vector<string>> text_in_vector)
{
    for (size_t i = 0; i < text_in_vector.size(); ++i)
    {
        cout << "Предложение " << i + 1 << ":" << endl;
        for (size_t j = 0; j < text_in_vector[i].size(); ++j)
        {
            cout << "Слово " << j + 1 << ": " << text_in_vector[i][j] << endl;
        }
    }
    _getch();
}

// (ВЫВОД) Функция для вывода на консоль найденных глаголов
void print_FoundVerbs(vector<VerbInfo> foundVerbs)
{
    int count = 1;
    cout << "Найденные глаголы:" << endl;
    for (const auto &verbInfo : foundVerbs)
    {
        cout << count << ") " << verbInfo.word << " (Предложение " << verbInfo.sentenceNum
             << ", Слово " << verbInfo.wordNum << ")" << endl;
        count++;
    }
    _getch();
}

// (ВЫВОД) Функция для вывода на консоль текста файла на консоль
void print_text_string(string text_in_string)
{
    // string buffer = "";
    // getline(file_input, buffer); // чтение данных из файла
    // cout << buffer;

    // // Чтение файла построчно
    // while (getline(file_input, buffer))
    //     cout << buffer << endl; // Выводим каждую строку на экран

    cout << text_in_string;
    _getch();
}