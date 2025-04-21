#pragma once
#ifndef OUTPUT_FILE_WORKING_H
#define OUTPUT_FILE_WORKING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "NLP.h"
#include "general_search.h"
using namespace std;

// Функция для добавления информации об упоминании слова в других предложениях
// и для выделения этого слова в предложении
void searchWordInSentences(vector<WordData>& data, vector<vector<string>>& sentences)
{
    int sentence_number;
    for (vector<string>& sentence : sentences)
    {
        for (size_t i = 0; i < sentence.size(); i++)
        {
            for (WordData& wordData : data)
            {
                if (wordData.word == sentence[i])
                {
                    sentence_number = stoi(sentence[0]);                  // Получаем номер предложения
                    wordData.sentence_counter.push_back(sentence_number); // Добавляем номер предложения
                    wordData.amount++;                                    // Увеличиваем количество упоминаний
                    sentence[i] = "{" + sentence[i] + "}";                // Выделяем слово фигурными скобками
                }
            }
        }
    }
}

// Функция для добавления нумерации предложений в вектор
void addSentenceNumbers(vector<vector<string>>& sentences)
{
    for (size_t i = 0; i < sentences.size(); ++i)
    {
        // Преобразуем номер предложения в строку
        string sentenceNumber = to_string(i + 1) + ")";

        // Вставляем номер предложения в начало текущего предложения
        sentences[i].insert(sentences[i].begin(), sentenceNumber);
    }
}

//void writeOutputFiles(vector<WordData> &wordData, vector<vector<string>> &sentences, const string &sentencesFilePath, const string &rhymesFilePath);
//void findWordsInSentences(vector<WordData> &wordData, const vector<string> &sentences);
//vector<string> enumerateSentences(vector<vector<string>> &sentences, const vector<WordData> &wordData);
//string Numeric_Root(string Root);
//
///* writeOutputFiles.txt */
//// Функция записи в выходные файлы
//void writeOutputFiles(vector<WordData> &wordData,
//                      vector<vector<string>> &sentences,
//                      const string &sentencesFilePath,
//                      const string &rhymesFilePath)
//{
//    // Заполняем sentence_counter и amount
//    vector<string> flatSentences;
//    for (const auto &sent : sentences)
//    {
//        string joined;
//        bool first = true;
//        for (const auto &word : sent)
//        {
//            if (!first)
//                joined += " ";
//            joined += word;
//            first = false;
//        }
//        flatSentences.push_back(joined);
//    }
//    findWordsInSentences(wordData, flatSentences);
//
//    // Получаем нумерованные предложения с преобразованными словами
//    vector<string> numberedSentences = enumerateSentences(sentences, wordData);
//
//    // Запись предложений в файл
//    ofstream sentencesFile(sentencesFilePath);
//    for (const auto &sentence : numberedSentences)
//    {
//        sentencesFile << sentence << "\n";
//    }
//    sentencesFile.close();
//
//    // Запись информации о рифмах в файл
//    ofstream rhymesFile(rhymesFilePath);
//    for (const auto &word : wordData)
//    {
//        rhymesFile << "Слово: " << word.word << "\n";
//        rhymesFile << "Количество рифм с этим словом: " << word.rhymed_amount << "\n";
//        rhymesFile << "Слова, рифмующиеся с данным словом: ";
//        if (word.rhymed_words.empty())
//        {
//            rhymesFile << "нет\n";
//        }
//        else
//        {
//            for (size_t i = 0; i < word.rhymed_words.size(); ++i)
//            {
//                rhymesFile << word.rhymed_words[i];
//                if (i < word.rhymed_words.size() - 1)
//                {
//                    rhymesFile << ", ";
//                }
//            }
//            rhymesFile << "\n";
//        }
//        rhymesFile << "Количество таких слов в тексте: " << word.amount << "\n";
//        rhymesFile << "Встречается в предложениях под номерами: ";
//        if (word.sentence_counter.empty())
//        {
//            rhymesFile << "нет\n";
//        }
//        else
//        {
//            for (size_t i = 0; i < word.sentence_counter.size(); ++i)
//            {
//                rhymesFile << word.sentence_counter[i];
//                if (i < word.sentence_counter.size() - 1)
//                {
//                    rhymesFile << ", ";
//                }
//            }
//            rhymesFile << "\n";
//        }
//        rhymesFile << "\n";
//    }
//    rhymesFile.close();
//}
//
///* findWordsInSentences.txt */
//// Функция поиска слов в предложениях
//void findWordsInSentences(vector<WordData> &wordData, const vector<string> &sentences)
//{
//    for (auto &wordEntry : wordData)
//    {
//        // Преобразуем все символы строки в верхний регистр
//        wordEntry.word = capitalizeAllLetters(wordEntry.word);
//
//        wordEntry.sentence_counter.clear();
//        wordEntry.amount = 0;
//
//        for (size_t i = 0; i < sentences.size(); ++i)
//        {
//            string sentence = sentences[i];
//            transform(sentence.begin(), sentence.end(), sentence.begin(), ::tolower);
//            string targetWord = wordEntry.word;
//            transform(targetWord.begin(), targetWord.end(), targetWord.begin(), ::tolower);
//
//            if (sentence.find(targetWord) != string::npos)
//            {
//                wordEntry.sentence_counter.push_back(i + 1);
//                wordEntry.amount++;
//            }
//        }
//    }
//}
//
///* enumerateSentences.txt */
//// Функция для нумерации предложений
//vector<string> enumerateSentences(vector<vector<string>> &sentences,
//                                  const vector<WordData> &wordData)
//{
//    vector<string> resultSentences;
//
//    // Создаем словарь слов из wordData для быстрого поиска
//    vector<string> targetWords;
//    for (const auto &wd : wordData)
//    {
//        targetWords.push_back(wd.word);
//    }
//
//    for (size_t i = 0; i < sentences.size(); ++i)
//    {
//        if (sentences[i].empty())
//        {
//            resultSentences.push_back(to_string(i + 1) + ". ");
//            continue;
//        }
//
//        string modifiedSentence;
//        bool firstWord = true;
//
//        // Обрабатываем каждое слово в предложении
//        for (auto &word : sentences[i])
//        {
//            string wordToWrite = word;
//            // Проверяем, есть ли слово в wordData
//            for (const auto &targetWord : targetWords)
//            {
//                string wordLower = word;
//                string targetLower = targetWord;
//                transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);
//                transform(targetLower.begin(), targetLower.end(), targetLower.begin(), ::tolower);
//
//                if (wordLower == targetLower)
//                {
//                    wordToWrite = capitalizeAllLetters(word);
//                    break;
//                }
//            }
//            // Формируем предложение
//            if (!firstWord)
//            {
//                modifiedSentence += " ";
//            }
//            modifiedSentence += wordToWrite;
//            firstWord = false;
//        }
//
//        // Добавляем номер предложения
//        resultSentences.push_back(to_string(i + 1) + ". " + modifiedSentence);
//    }
//
//    return resultSentences;
//}
//
///* Функция_пронумерованных_предложений.txt*/
//// Функция
//string Numeric_Root(string Root) // принимаем на вход путь к исходному файлу,
//{
//    ifstream Text_File(Root); // открываем
//
//    string File_Extension = ".txt"; // удаляем расширение
//    int position = Root.find(File_Extension);
//    if (position != -1)
//    {
//        Root.erase(position);
//    }
//
//    Root = Root + "_numeric"; // дописка к названию (значение опционально)
//    bool Correct_Name = false;
//    int i = 1;
//    do
//    {
//        ifstream Numeric_File(Root); // открываем файл на чтение
//        if (Numeric_File)            // если он существует, значит нельзя создавать выходной файл с таким именем
//        {
//            //================================================
//            // убираем неудачно добавленную цифру, если она была добавлена
//            position = Root.find(" (" + to_string(i - 1) + ")");
//            if (position != -1)
//            {
//                Root.erase(position);
//            }
//            //================================================
//            Numeric_File.close();
//
//            Root = Root + " (" + to_string(i) + ")"; // дописываем число по порядку возрастанию
//        }
//        else // если файл не открылся, значит его нет. Значит имя подходит для работы
//        {
//            Correct_Name = true;
//        }
//        i++; // увеличиваем индекс, если файл с номером
//    } while (Correct_Name == false); // пока этот файл открывается, значит он существует, и поэтому его нельзя трогать.
//
//    return Root + File_Extension; // дописываем расширение
//}

/////////////////////////////////////



#endif // OUTPUT_FILE_WORKING_H