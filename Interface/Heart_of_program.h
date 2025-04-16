#include "general_search.h"
#include "file_working.h"
#include "UI_const.h"

using namespace std;

// функция-объединение для получения и передачи рифм, флагов, предложений
void unite_functions(vector<WordData> &rhymes_data,     // адрес вектора для хранения данных о найденных рифмах по частям речи
                     vector<vector<string>> &sentences, // адрес вектора текста с разделенными предложениями
                     const string &word_to_compare,     // адрес слова для поиска рифм
                     bitset<8> &button_flags)           // адрес флагов для частей речи
{
    sentences = splitTextIntoSentences(); // разделение текста на предложения

    if (!sentences.empty())                                                    // если текст пустой, ничего не делаем
        deal_with_words(button_flags, sentences, word_to_compare, rhymes_data); // работа с рифмами частей речи
}

// vector<WordData> unite_functions(bitset<8> &button_flags, const string &word_to_compare)
//{
//     vector<vector<string>> sentences = splitTextIntoSentences(); // разделение текста на предложения
//
//     vector<WordData> rhymes_data; // вектор для хранения данных о найденных рифм по частям речи
//
//     if (!sentences.empty())                                                    // если вектор пустой, ничего не делаем
//         deal_with_words(button_flags, sentences, word_to_compare, rhymes_data); // работа с рифмами частей речи
//
//     return rhymes_data; // возвращаем вектор с рифмами
// }
