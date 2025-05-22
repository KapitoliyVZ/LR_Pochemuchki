#pragma once
#include "standard_libraries.h"



// Функция для удаления символов переноса строки из текста
void removeNewlines(string& text) {
	text.erase(remove(text.begin(), text.end(), '\n'), text.end());
}

/*
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
	// Убираем все переносы строк
	removeNewlines(str_text);

	vector<vector<string>> sentences;

	// Токенизация текста
	vector<string> tokens = tokenizeTextUTF8(str_text);

	// Разделение на предложения
	vector<string> current_sentence;
	bool inside_quotes = false;

	const vector<string> sentence_endings = { ".", "!", "?", "...", "?!", "!?", "!...", "?..." };

	for (size_t i = 0; i < tokens.size(); ++i) {
		const auto& token = tokens[i];

		if (token == "\"" || token == "«") {
			inside_quotes = !inside_quotes;
		}
		else if (token == "\"" || token == "»") {
			inside_quotes = !inside_quotes;
		}

		if (!token.empty()) {
			current_sentence.push_back(token);
		}

		if (!inside_quotes &&
			find(sentence_endings.begin(), sentence_endings.end(), token) != sentence_endings.end()) {

			while (i + 1 < tokens.size() &&
				(tokens[i + 1] == "\"" || tokens[i + 1] == "«" || tokens[i + 1] == "»")) {
				current_sentence.push_back(tokens[++i]);
			}

			if (i + 1 < tokens.size() &&
				find(sentence_endings.begin(), sentence_endings.end(), tokens[i + 1]) != sentence_endings.end()) {
				current_sentence.push_back(tokens[++i]);
			}

			if (!current_sentence.empty()) {
				sentences.push_back(current_sentence);
			}

			current_sentence.clear();
		}
	}

	if (!current_sentence.empty()) {
		sentences.push_back(current_sentence);
	}

	removeSpacesFromSentences(sentences);

	if (!sentences.empty()) {
		sentences.pop_back();
	}

	return sentences;
}
*/

vector<vector<string>> parse_sentences(string text)   //разделение текста на предложения
{
	vector<vector<string>> sentences = {};
	string line = "";
	string word = "";
	bool quote_is_open = false; //флаг открытия кавычек
	vector<string> current_sentence;
	removeNewlines(text);

	line = text;

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] != '.' && line[i] != '!' && line[i] != '?' && line[i] != '…')
		{
			if (line[i] == ',' || line[i] == ':')  //если встретилась запятая или двоеточие
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == '«')  //если встретились открывающие кавычки
			{
				quote_is_open = true;
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == '»')  //если встретились закрывающие кавычки
			{
				quote_is_open = false;

				//если после кавычек идет тире со словами автора с маленькой буквы, продолжаем предложение. 
				//("Привет!" - сказал он.)
				if ((i < line.size() - 4) && (line[i + 2] == '–' && (line[i + 4] >= 'а' && line[i + 4] <= 'я')) || (line[i + 1] == ',') || (line[i + 1] == '.'))
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}

				//проверка, не был ли перед кавычками завершающий знак предложения; 
				//если он был, то тогда записываем завершающие кавычки и завершаем предложение; (Пример: "Привет!")
				if (line[i - 1] == '?!' || line[i - 1] == '!' || line[i - 1] == '?' || line[i - 1] == '…' || line[i - 1] == '.')
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					sentences.push_back(current_sentence);
					current_sentence.clear();
					word = "";
					continue;
				}
			}

			if (line[i] == ' ') //если встретился пробел
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
			}
			else  //если буква
			{
				word += line[i];
			}
		}


		else   //если встретился завершающий знак предложения
		{
			if (quote_is_open == true)  //продолжаем записывать предложение, если кавычки ОТКРЫТЫ, но встретился завершающий знак предложения  (Пример:  "Я пришел!")         
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == '…' && i < line.size() - 2)  //если троеточие встретилось внутри предложения, не в конце строки, то продолжаем записывать (Пример: Тихо... хорошо)
			{
				if (line[i + 2] >= 'а' && line[i + 2] <= 'я') //если буква через пробел маленькая, то это одно предложение 
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}

			}

			//если точка встретилась внутри скобок, продолжаем записывать  
			if (line[i] == '.' && i < line.size() - 1)
			{
				if (line[i + 1] == ')')    //если следующий после точки символ ) , то записываем скобку последним символом в текущее предложение              
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					word += line[i + 1];
					word = "";
					current_sentence.push_back(word);
					sentences.push_back(current_sentence);
					current_sentence.clear();
					i++;      //увеличиваем самостоятельно счетчик, так как записали сразу скобку, идущую после точки
					continue;
				}

			}

			//для диалога: если после ?; ...; !; . идет тире со словами автора с маленькой буквы, продолжаем предложение
			if ((line[i] == '!' || line[i] == '?' || line[i] == '.' || line[i] == '…') && (i < line.size() - 4))
			{
				if (line[i + 2] == '–' && (line[i + 4] >= 'а' && line[i + 4] <= 'я'))   //проверка, что через пробел идет "-", а потом через пробел маленькая буква (Пример: - Привет! - сказала она.)
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}
			}

			if (word != "")
			{
				current_sentence.push_back(word);
				word = "";
			}
			word += line[i];
			current_sentence.push_back(word);
			sentences.push_back(current_sentence);
			current_sentence.clear();
			word = "";
		}
	}

	return sentences;


}