#include "general_search.h"
#include "file_working.h"
#include "UI_const.h"

using namespace std;

vector<WordData> unite_functions( bitset<8> &button_flags, const string &word_to_compare)
{
    vector<vector<string>> sentences = splitTextIntoSentences(); // разделение текста на предложения

    vector<WordData> data; // вектор для хранения данных о найденных рифм по частям речи

    deal_with_words(button_flags, sentences, word_to_compare, data); // работа с рифмами частей речи

    return data; // возвращаем вектор с рифмами
}
