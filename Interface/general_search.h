﻿#pragma once
#include "standard_libraries.h"
#include "NLP.h"
#include "rhymes.h"
#include "kulik_search.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <future>
#include <mutex>

// данный заголовочный файл предназначен для выполнения следующих функций:
// - основная функция, которая объединяет в себе все функции, необходимые для создания вектора структур с содержательной информацией
// - функции для поиска рифм
 
// структура для хранения данных о найденном причастии
struct WordData
{
	string word;				  // строка с самим словом
	string part_of_speech;		  // строка с информацией о части речи
	vector<int> sentence_counter; // переменная для хранения номера предложения, в котором встретилось слово
	vector<string> rhymed_words;  // вектор для хранения слов, с которыми рифмуется слово
	int amount = 0;				  // количество встреч данного слова в тексте
	int rhymed_amount = 0;		  // количество слов, с которым рифмуется взятое слово
	vector<vector<int>> rhymed_words_sentences_number; // вектор векторов номеров предложений, в которых встречено рифмующееся слово

};


// функция поиска рифмующихся слов в предложениях
void find_number_of_sentences(vector<WordData>& data, vector<vector<string>>& sentences)
{
	unordered_map<string, vector<pair<int, WordData*>>> rhyme_map;

	// Создаем быстрый доступ: lowercase и UPPERCASE рифмующееся слово -> WordData
	for (WordData& wd : data)
	{
		wd.rhymed_words_sentences_number.resize(wd.rhymed_words.size());

		for (int j = 0; j < wd.rhymed_words.size(); ++j)
		{
			string low = lowFirstLetter(wd.rhymed_words[j]);
			string cap = capitalizeAllLetters(wd.rhymed_words[j]);

			rhyme_map[low].emplace_back(j, &wd);
			rhyme_map[cap].emplace_back(j, &wd);
		}
	}

	for (int i = 0; i < sentences.size(); ++i)
	{
		for (const string& word : sentences[i])
		{
			string low = lowFirstLetter(word);
			string cap = capitalizeAllLetters(word);

			// Проверка и по lower, и по UPPER варианту
			if (rhyme_map.count(low))
			{
				for (auto& [index, wd_ptr] : rhyme_map[low])
				{
					auto& vec = wd_ptr->rhymed_words_sentences_number[index];
					if (find(vec.begin(), vec.end(), i + 1) == vec.end())
						vec.push_back(i + 1);
				}
			}

			if (rhyme_map.count(cap))
			{
				for (auto& [index, wd_ptr] : rhyme_map[cap])
				{
					auto& vec = wd_ptr->rhymed_words_sentences_number[index];
					if (find(vec.begin(), vec.end(), i + 1) == vec.end())
						vec.push_back(i + 1);
				}
			}
		}
	}
}



// функция для загрузки морфемных признаки из файла
vector<string> load_morphemes(string filename)
{
	string file_path = get_filepath(filename);
	vector<string> morphemes;
	ifstream file;
	file.open(file_path, ios_base::in);

	if (!file.is_open()) {
		morphemes.push_back("error");
		return morphemes;
	}

	
	string morpheme;

	while (file >> morpheme) {
		morphemes.push_back(morpheme);
	}

	file.close();
	return morphemes;
}

// Функция разбора ANSI строки на отдельные сиволы
std::vector<std::string> ansiSplit(const std::string& str)
{
	std::vector<std::string> result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		result.emplace_back(std::string(1, str[i]));
	}
	return result;
}

// Функция для расчета расстояния Дамерау-Левенштейна
int damerauLevenshteinDistance(const string& ending_1, const string& ending_2)
{
	int len_1 = ending_1.size();
	int len_2 = ending_2.size();

	// Матрица для хранения промежуточных результатов
	vector<vector<int>> dist(len_1 + 1, vector<int>(len_2 + 1));

	// Инициализация первой строки и первого столбца
	for (int i = 0; i <= len_1; ++i)
		dist[i][0] = i;
	for (int j = 0; j <= len_2; ++j)
		dist[0][j] = j;

	// Основной цикл для вычисления расстояний
	for (int i = 1; i <= len_1; ++i)
	{
		for (int j = 1; j <= len_2; ++j)
		{
			int cost = (ending_1[i - 1] == ending_2[j - 1]) ? 0 : 1;

			// Минимум из удаления, вставки или замены
			dist[i][j] = min({
				dist[i - 1][j] + 1,		  // удаление
				dist[i][j - 1] + 1,		  // вставка
				dist[i - 1][j - 1] + cost // замена
				});

			// Транспозиция символов
			if (i > 1 && j > 1 && ending_1[i - 1] == ending_2[j - 2] && ending_1[i - 2] == ending_2[j - 1])
			{
				dist[i][j] = min(dist[i][j], dist[i - 2][j - 2] + 1); // транспозиция
			}
		}
	}
	return dist[len_1][len_2];
}

// функция проверки наличия причастия в структуре причастий
bool existence_of_participle(vector<WordData>& data, string& comparing_participle)
{
	for (const WordData& check : data)
	{
		if (comparing_participle == check.word)
			return true;
	}
	return false;
};

// функция поиска максимальной длины идущего подряд совпадений в окончаниях слов
int countMaxConsecutiveMatches(const std::string& s1, const std::string& s2) {
	int maxCount = 0;
	int currentCount = 0;
	for (size_t i = 0; i < std::min(s1.size(), s2.size()); ++i) {
		if (s1[i] == s2[i]) {
			currentCount++;
			maxCount = std::max(maxCount, currentCount);
		}
		else {
			currentCount = 0;
		}
	}
	return maxCount;
}

// функция проверки рифмования двух взятых слов
bool areWordsRhymed(const std::string& word_1, const std::string& word_2) {
	
	// Получаем количество слогов
	int syllables_1 = countSyllables(word_1);
	int syllables_2 = countSyllables(word_2);

	// Если количество слогов не совпадает, то слова не рифмуются
	if (std::abs(syllables_1 - syllables_2) > 1) return false;

	// Функция для подсчета максимального количества подряд совпадающих символов
	auto countMaxConsecutiveMatches = [](const std::vector<std::string>& s1, const std::vector<std::string>& s2) {
		int maxCount = 0;
		int currentCount = 0;
		int len = std::min(s1.size(), s2.size());
		for (int i = 0; i < len; ++i) {
			if (s1[s1.size() - len + i] == s2[s2.size() - len + i]) {
				currentCount++;
				maxCount = std::max(maxCount, currentCount);
			}
			else {
				currentCount = 0;
			}
		}
		return maxCount;
		};

	std::vector<std::string> letters_1 = ansiSplit(word_1);
	std::vector<std::string> letters_2 = ansiSplit(word_2);

	int len_1 = letters_1.size();
	int len_2 = letters_2.size();

	if (len_1 > 4 && len_2 > 4) {
		int diff_count = 0;

		// Сравниваем последние 4 символа по позициям
		for (int i = 0; i < 4; ++i) {
			std::string char1 = letters_1[len_1 - 4 + i];
			std::string char2 = letters_2[len_2 - 4 + i];

			if (char1 != char2) {
				diff_count++;
				if (diff_count > 2) return false;
			}
		}

		// Проверка на последние 2 символа
		std::string suffix1_last2 = letters_1[len_1 - 2] + letters_1[len_1 - 1];
		std::string suffix2_last2 = letters_2[len_2 - 2] + letters_2[len_2 - 1];
		if (suffix1_last2 != suffix2_last2) return false;

		// Дополнительная проверка: есть ли хотя бы 2 подряд совпадающих символа
		std::vector<std::string> suffix1(letters_1.end() - 4, letters_1.end());
		std::vector<std::string> suffix2(letters_2.end() - 4, letters_2.end());
		int maxConsecutive = countMaxConsecutiveMatches(suffix1, suffix2);

		return maxConsecutive >= 2;
	}
	else {
		// Берём последние 2 символа
		if (len_1 < 2 || len_2 < 2) return false; // безопасности ради

		std::string suffix1 = letters_1[len_1 - 2] + letters_1[len_1 - 1];
		std::string suffix2 = letters_2[len_2 - 2] + letters_2[len_2 - 1];

		return suffix1 == suffix2;
	}
}

// функция проверки наличия одинаковых рифмованных слов
bool existenceRhymedParticiples(WordData& candidate, const string& word)
{

	for (const string& check_participle : candidate.rhymed_words)
	{
		if (word == check_participle)
			return true;
	}
	return false;
}

// функция взятия название части речи взятого слова
std::string get_output_part_of_speech(string part_of_speech)
{
	// основная проверка части речи

	if (part_of_speech == "V")
		return ("глагол");

	if (part_of_speech == "ADV")
		return ("наречие");

	if (part_of_speech == "A")
		return ("прилагательное");

	if (part_of_speech == "прич")
		return ("причастие");

	if (part_of_speech == "деепр")
		return ("деепричастие");

	if (part_of_speech == "S")
		return ("существительное");


};

// функция взятия суффикса слова
string get_suffix(const string& word, size_t length = 3) {
	if (word.length() < length) return word;
	return word.substr(word.length() - length, length);
}


// основная функция поиска рифм
vector<WordData> find_rhymes_fast
(
	const vector<vector<string>>& words_text_collection,
	bitset<8> button_flags,
	const vector<string>& word_to_compare,
	const vector<string>& parts_of_speech)
{
	vector<WordData> data;                  // результирующий список объектов WordData с рифмами
	unordered_set<string> added_words;      // множество уже обработанных слов (для исключения повторов)
	unordered_map<string, vector<string>> rhyme_cache;  // кэш рифм: слово → список уже найденных рифм

	// определение режима работы на основе переданных флагов и наличия слова для сравнения
	int variant = 0;
	if (button_flags.test(7)) variant = word_to_compare.empty() ? 0 : 1;
	else variant = word_to_compare.empty() ? 2 : 3;

	// строим индекс по суффиксам слов (для ускоренного доступа при поиске рифм)
	vector<unordered_map<string, vector<string>>> suffix_indices(6);
	for (int i = 0; i < 6; ++i) {
		for (const auto& word : words_text_collection[i]) {
			suffix_indices[i][get_suffix(word)].push_back(word);  // группировка слов по суффиксу
		}
	}

	// анонимная функция (лямбда), возвращающая рифмующиеся слова к заданному слову
	auto get_rhymes = [&](const string& word, int pos, bool cross_pos) -> vector<string> {
		// проверка, есть ли слово в кэше
		if (rhyme_cache.find(word) != rhyme_cache.end())
			return rhyme_cache[word];

		vector<string> rhymes;
		string suff = get_suffix(word);     // получаем суффикс слова
		unordered_set<string> seen;         // для исключения повторов

		// если разрешён поиск рифм по другим частям речи
		if (cross_pos) {
			for (int j = 0; j < 6; ++j) {
				if (j == pos) continue;  // пропускаем исходную часть речи
				if (suffix_indices[j].count(suff)) {
					for (const auto& candidate : suffix_indices[j][suff]) {
						// проверка, что слово не совпадает само с собой и ещё не встречалось
						if (candidate != word && seen.insert(candidate).second) {
							// проверка, являются ли слова рифмующимися
							if (areWordsRhymed(word, candidate)) {
								rhymes.push_back(candidate);
							}
						}
					}
				}
			}
		}
		// иначе ищем рифмы только внутри одной части речи
		else {
			if (suffix_indices[pos].count(suff)) {
				for (const auto& candidate : suffix_indices[pos][suff]) {
					if (candidate != word && seen.insert(candidate).second) {
						if (areWordsRhymed(word, candidate)) {
							rhymes.push_back(candidate);
						}
					}
				}
			}
		}

		// сохраняем найденные рифмы в кэш
		rhyme_cache[word] = rhymes;
		return rhymes;
		};

	// обработка одного слова: формирование объекта WordData и добавление в результат
	auto process_word = [&](const string& word, int pos, bool cross_pos) {
		if (word.empty()) return;  // пропуск пустых слов
		if (!added_words.insert(word).second) return;  // пропуск уже обработанных слов

		WordData wd;
		wd.word = word;
		wd.rhymed_words = get_rhymes(word, pos, cross_pos);  // получение рифм
		wd.part_of_speech = get_output_part_of_speech(parts_of_speech[pos]);  // определение ЧР для вывода
		wd.rhymed_amount = wd.rhymed_words.size();  // подсчёт рифм
		data.push_back(std::move(wd));  // добавление в результирующий список
		};

	// основная логика в зависимости от определённого выше варианта
	switch (variant) {
	case 0:
		// вариант 0: однородный поиск без сравниваемого слова
		for (int i = 0; i < 6; ++i)
			for (const auto& w : words_text_collection[i])
				process_word(w, i, false);
		break;

	case 1:
		// вариант 1: однородный поиск со сравниваемым словом
		for (int i = 0; i < 6; ++i)
			process_word(word_to_compare[i], i, false);
		break;

	case 2:
		// вариант 2: неоднородный поиск без сравниваемого слова
		for (int i = 0; i < 6; ++i)
			for (const auto& w : words_text_collection[i])
				process_word(w, i, true);
		break;

	case 3:
		// вариант 3: неоднородный поиск со сравниваемым словом
		for (int i = 0; i < 6; ++i)
			process_word(word_to_compare[i], i, true);
		break;
	}

	return data;
}


// основная функция работы со словами текста и заполнением вектора струтур слов содержательной информацией
void deal_with_words(bitset<8>& button_flags, vector<vector<string>>& numbered_sentences, string word_to_compare, vector<WordData>& data)
{

	// количество частей речи, которые можно найти
	const int amount_of_parts_of_speech = 6;

	// вектор векторов на каждую часть речи
	vector<vector<string>> words_text_collection(amount_of_parts_of_speech);

	vector<string> comparing_word_part_of_speech;

	// подстановка сравниваемого слова на соответствующую позицию в векторе части речи для корректного последующего сравнения
	if (!word_to_compare.empty())
	{
		word_to_compare = lowFirstLetter(word_to_compare);

		comparing_word_part_of_speech = get_comparing_word_part(word_to_compare);
		

		for (string& word : comparing_word_part_of_speech)
		{
			if (!word.empty() && button_flags.test(7) == 1)
			{
				for (int i = 0; i < 6; i++)
				{
					if (!comparing_word_part_of_speech[i].empty())
						button_flags.set(i);
				}
			}
			else if (!word.empty() && button_flags.test(7) == 0)
			{
				for (int i = 0; i < 6; i++)
				{
					if (!comparing_word_part_of_speech[i].empty())
						button_flags.set(i, 0);
				}

			}
		}
	}

	// обозначения частей речи для MyStem
	const vector<string> parts_of_speech{ "V", "ADV", "A", "S", "прич", "деепр" };

	// поиск слов по частям речи
	findWordsByPartOfSpeech(numbered_sentences,button_flags, parts_of_speech, words_text_collection, buttons::morphemeRules, word_to_compare);

	// поиск рифм
	data = find_rhymes_fast(words_text_collection, button_flags, comparing_word_part_of_speech, parts_of_speech);

	find_number_of_sentences(data, numbered_sentences);

	// поднятие всех регистров для красивого вывода и поиска слов по рифмам в
	for (WordData& record : data)
		record.word = capitalizeAllLetters(record.word);

};