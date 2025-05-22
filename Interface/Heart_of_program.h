#include "general_search.h"
#include "file_working.h"
#include "UI_const.h"
#include "output_file_working.h"

using namespace std;

// функция-объединение для получения и передачи рифм, флагов, предложений
void unite_functions(vector<WordData>& wordData,        // адрес вектора для хранения данных о найденных рифмах по частям речи
    vector<vector<string>>& sentences, // адрес вектора текста с разделенными предложениями
    const string str_sentences,        // строка с текста 
    const string& word_to_compare,     // адрес слова для поиска рифм
    bitset<8>& button_flags)           // адрес флагов для частей речи
{

    // sentences = splitTextIntoSentences(str_sentences); // разделение текста на предложения
    sentences = parse_sentences(str_sentences);

    if (!sentences.empty())                                                  // если текст пустой, ничего не делаем
        deal_with_words(button_flags, sentences, word_to_compare, wordData); // работа с рифмами частей речи

    // добавляем нумерацию предложений
    addSentenceNumbers(sentences);

    // Добавляем информацию в wordData о найденных упоминаниях
    searchWordInSentences(wordData, sentences);

}


