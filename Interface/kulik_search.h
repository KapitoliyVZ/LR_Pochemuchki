#pragma once
#include "standard_libraries.h"

// Функция для определения длины символа в UTF-8
int utf8CharLength(unsigned char byte) {
    if ((byte & 0x80) == 0) return 1;       // ASCII символ (1 байт)
    if ((byte & 0xE0) == 0xC0) return 2;   // 2-байтовый символ
    if ((byte & 0xF0) == 0xE0) return 3;   // 3-байтовый символ
    if ((byte & 0xF8) == 0xF0) return 4;   // 4-байтовый символ
    return 0; // Некорректный байт
}


// функция удаления пробелов
void removeSpacesFromSentences(vector<vector<string>>& sentences) {
	if (sentences.empty()) return; // Если вектор пустой, ничего не делаем
    for (auto& sentence : sentences) {
		if (sentence.empty()) continue; // Если предложение пустое, пропускаем его
        for (auto& word : sentence) {
            // Удаляем все пробелы из строки
            word.erase(remove(word.begin(), word.end(), ' '), word.end());
        }
    }
}

// Функция для токенизации текста (слова и знаки пунктуации отдельно)
vector<string> tokenizeTextUTF8(const string& text) {
    vector<string> tokens;
    size_t i = 0;

    while (i < text.size()) {
        int char_len = utf8CharLength(static_cast<unsigned char>(text[i]));
        if (char_len == 0) {
            cerr << "Некорректная последовательность байт в позиции " << i << endl;
            break;
        }

        string utf8_char = text.substr(i, char_len);

        // Если это знак пунктуации, добавляем отдельно
        if (ispunct(static_cast<unsigned char>(utf8_char[0])) || utf8_char == " ") {
            tokens.push_back(utf8_char);
        }
        else {
            // Если это часть слова, объединяем с предыдущим словом
            if (!tokens.empty() && !ispunct(static_cast<unsigned char>(tokens.back()[0]))) {
                tokens.back() += utf8_char;
            }
            else {
                tokens.push_back(utf8_char);
            }
        }

        i += char_len;
    }

    return tokens;
}

// функция разделения текста на предложения
vector<vector<string>> splitTextIntoSentences(string str_text) 
{
    vector<vector<string>> sentences;

    // Токенизация текста
    vector<string> tokens = tokenizeTextUTF8(str_text);

    // Разделение на предложения
    vector<string> current_sentence;
    bool inside_quotes = false;

    // Список знаков пунктуации, которые определяют конец предложения
    const vector<string> sentence_endings = {
        ".", "!", "?", "...", "?!", "!?", "!...", "?..."
    };

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        // Проверка на кавычки
        if (token == "\"" || token == "«") {
            inside_quotes = !inside_quotes;
        }
        else if (token == "\"" || token == "»") {
            inside_quotes = !inside_quotes;
        }

        // Добавляем токен в текущее предложение
        if (!token.empty()) {
            current_sentence.push_back(token);
        }

        // Проверка конца предложения, если мы вне кавычек
        if (!inside_quotes &&
            (find(sentence_endings.begin(), sentence_endings.end(), token) != sentence_endings.end())) {

            // Проверяем, есть ли после текущего токена кавычки или другие символы, относящиеся к текущему предложению
            while (i + 1 < tokens.size() &&
                (tokens[i + 1] == "\"" || tokens[i + 1] == "«" || tokens[i + 1] == "»")) {
                current_sentence.push_back(tokens[++i]);
            }

            // Проверяем, не является ли текущий токен частью многосоставного конца
            if (i + 1 < tokens.size() &&
                find(sentence_endings.begin(), sentence_endings.end(), tokens[i + 1]) != sentence_endings.end()) {
                current_sentence.push_back(tokens[++i]);
            }

            // Добавляем предложение в список, если оно не пустое
            if (!current_sentence.empty()) {
                sentences.push_back(current_sentence);
            }

            // Очищаем текущее предложение
            current_sentence.clear();
        }
    }

    // Добавляем оставшиеся токены (если они есть)
    if (!current_sentence.empty()) {
        sentences.push_back(current_sentence);
    }

    removeSpacesFromSentences(sentences);

	// Удаляем последний элемент, если он пустой
	if (!sentences.empty())
        sentences.pop_back();

    return sentences;
}