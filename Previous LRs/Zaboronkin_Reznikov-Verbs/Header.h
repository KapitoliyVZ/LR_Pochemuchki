// Объявление и описание всех констант и библиотек
#ifndef HEADER
#define HEADER

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <utility>

using namespace std;

/// Клавиши в значениях ASCII
const int ESC_button = 27;   // 'esc'
const int ENTER_button = 13; // 'enter'
const int STAR_button = 42;  // '*'
const int SLASH_button = 47; // '/'
const int button_1 = 49;     // '1'
const int button_2 = 50;     // '2'
const int button_3 = 51;     // '3'
const int button_4 = 52;     // '4'
const int button_5 = 53;     // '5'
const int button_6 = 54;     // '6'
const int button_7 = 55;     // '7'
const int button_8 = 56;     // '8'

/// Клавиши навигации
const int FINISH_button = STAR_button;   // кнопка завершения программы
const int BACK_button = SLASH_button;    // кнопка возврата в предыдущее меню
const int CONFIRM_button = ENTER_button; // кнопка подтверждения выбора

const string extension_txt = ".txt";           // строковая запись расширения .txt
const size_t len_txt = extension_txt.length(); // длина строковой записи расширения
const size_t MAX_FILENAME_LENGTH = 255;

ifstream file_input;  // файл для чтения
ofstream file_output; // файл на запись

// Структура для хранения информации о глаголе
struct VerbInfo
{
    string word;        // Слово (глагол)
    size_t sentenceNum; // Номер предложения
    size_t wordNum;     // Номер слова в предложении
};

void start_menu(); // функция меню начального текста

bool make_verb_endings();      // Функция открытия файла с окончаниями для глаголов
void fill_vector_by_endings(); // Функция заполнения вектора окончаний глаголов из файла

bool make_input();                                                                  // Функция для старта меню открытия файла на чтение
pair<vector<vector<string>>, string> fill_vector_by_text();                         // Функция для переноса всего текста в вектор
vector<VerbInfo> findVerbsInSentences(vector<vector<string>>, vector<set<string>>); // Функция для записи глаголов в вектор
bool isVerb(string, vector<set<string>>);                                           // Проверка, является ли слово глаголом на основе окончаний
map<string, vector<VerbInfo>> findRhymingVerbs(vector<VerbInfo>);                   // Функция нахождения рифм глаголов
bool make_output(map<string, vector<VerbInfo>>, string, vector<vector<string>>, string);    // Функция старта меню для открытия файла на запись

#endif