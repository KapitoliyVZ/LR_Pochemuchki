#pragma once
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


 
// структура для хранения данных о найденном причастии
struct WordData
{
	string word;				  // строка с самим словом
	string part_of_speech;		  // строка с информацией о части речи
	vector<int> sentence_counter; // переменная для хранения номера предложения, в котором встретилось слово
	vector<string> rhymed_words;  // вектор для хранения слов, с которыми рифмуется слово
	int amount = 0;				  // количество встреч данного слова в тексте
	int rhymed_amount = 0;		  // количество слов, с которым рифмуется взятое слово
};



// Загружает морфемные признаки из файла
vector<string> load_morphemes(string filename)
{
	string file_path = get_filepath(filename);

	ifstream file;
	file.open(file_path, ios_base::in);

	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл " + filename);
	}

	vector<string> morphemes;
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

// Функция для извлечения окончания слова (например, последние 4 символа)
std::string getSuffix(const std::string& word, int suffix_length = 8)
{
	std::vector<std::string> letters = ansiSplit(word);

	std::string suffix;
	int start = (letters.size() > suffix_length) ? letters.size() - suffix_length : 0;

	for (int i = start; i < letters.size(); ++i)
	{
		suffix += letters[i];
	}

	return suffix;
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

// функция проверки наличия одинаковых рифмованных причастий
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
	// перевод в ANSI, чтобы проверка проходила для случая кириллицы (причастие, деепричастие)
	//part_of_speech = utf8_to_ansi(part_of_speech);

	// { "V", "ADV", "A", "S", "прич", "деепр" }

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

string get_suffix(const string& word, size_t length = 3) {
	if (word.length() < length) return word;
	return word.substr(word.length() - length, length);
}

vector<WordData> find_rhymes_fast(
	const vector<vector<string>>& words_text_collection,
	bitset<8> button_flags,
	const vector<string>& word_to_compare,
	const vector<string>& parts_of_speech)
{
	vector<WordData> data;
	unordered_set<string> added_words;
	unordered_map<string, vector<string>> rhyme_cache;

	int variant = 0;
	if (button_flags.test(7)) variant = word_to_compare.empty() ? 0 : 1;
	else variant = word_to_compare.empty() ? 2 : 3;

	// Создаем индекс по суффиксам для каждого POS
	vector<unordered_map<string, vector<string>>> suffix_indices(6);
	for (int i = 0; i < 6; ++i) {
		for (const auto& word : words_text_collection[i]) {
			suffix_indices[i][get_suffix(word)].push_back(word);
		}
	}

	// Лямбда для получения рифм через индекс
	auto get_rhymes = [&](const string& word, int pos, bool cross_pos) -> vector<string> {
		if (rhyme_cache.find(word) != rhyme_cache.end())
			return rhyme_cache[word];

		vector<string> rhymes;
		string suff = get_suffix(word);
		unordered_set<string> seen;

		if (cross_pos) {
			for (int j = 0; j < 6; ++j) {
				if (j == pos) continue;
				if (suffix_indices[j].count(suff)) {
					for (const auto& candidate : suffix_indices[j][suff]) {
						if (candidate != word && seen.insert(candidate).second) {
							if (areWordsRhymed(word, candidate)) {
								rhymes.push_back(candidate);
							}
						}
					}
				}
			}
		}
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

		rhyme_cache[word] = rhymes;
		return rhymes;
		};

	auto process_word = [&](const string& word, int pos, bool cross_pos) {
		if (word.empty()) return;
		if (!added_words.insert(word).second) return;

		WordData wd;
		wd.word = word;
		wd.rhymed_words = get_rhymes(word, pos, cross_pos);
		wd.part_of_speech = get_output_part_of_speech(parts_of_speech[pos]);
		wd.rhymed_amount = wd.rhymed_words.size();
		data.push_back(std::move(wd));
		};

	switch (variant) {
	case 0:
		for (int i = 0; i < 6; ++i)
			for (const auto& w : words_text_collection[i])
				process_word(w, i, false);
		break;
	case 1:
		for (int i = 0; i < 6; ++i)
			process_word(word_to_compare[i], i, false);
		break;
	case 2:
		for (int i = 0; i < 6; ++i)
			for (const auto& w : words_text_collection[i])
				process_word(w, i, true);
		break;
	case 3:
		for (int i = 0; i < 6; ++i)
			process_word(word_to_compare[i], i, true);
		break;
	}

	return data;
}


// основная функция работы с рифмами частей речи
void deal_with_words(bitset<8>& button_flags, vector<vector<string>>& numbered_sentences, string word_to_compare, vector<WordData>& data)
{
	// инициализация векторов окончаний и суффиксов всех 6 частей речи
	// Загрузка морфемных правил из файлов
	unordered_map<string, vector<string>> morphemeRules = {
		{"nouns_endings", load_morphemes("nouns_endings.txt")},
		{"nouns_suffixes", load_morphemes("nouns_suffixes.txt")},
		{"adjectives_endings", load_morphemes("adjectives_endings.txt")},
		{"adjectives_suffixes", load_morphemes("adjectives_suffixes.txt")},
		{"participles_endings", load_morphemes("participles_endings.txt")}, // Новое
		{"participles_suffixes", load_morphemes("participles_suffixes.txt")}, // Новое
		{"verbs_endings", load_morphemes("verbs_endings.txt")},
		{"verbs_suffixes", load_morphemes("verbs_suffixes.txt")},
		{"gerunds_endings", load_morphemes("gerunds_endings.txt")},
		{"gerunds_suffixes", load_morphemes("gerunds_suffixes.txt")},
		{"adverbs_endings", load_morphemes("adverbs_endings.txt")},
		{"adverbs_suffixes", load_morphemes("adverbs_suffixes.txt")},
		{"others_list", load_morphemes("others_list.txt")}
	};

	// количество частей речи, которые можно найти
	const int amount_of_parts_of_speech = 6;

	// вектор векторов на каждую часть речи
	vector<vector<string>> words_text_collection(amount_of_parts_of_speech);

	vector<string> comparing_word_part_of_speech;

	// подстановка сравниваемого слова на соответствующую позицию в векторе для корректного последующего сравнения
	if (!word_to_compare.empty())
	{
		word_to_compare = lowFirstLetter(word_to_compare);

		comparing_word_part_of_speech = get_comparing_word_part(word_to_compare, morphemeRules);

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
	findWordsByPartOfSpeech(numbered_sentences,button_flags, parts_of_speech, words_text_collection, morphemeRules, word_to_compare);

	// поиск рифм
	data = find_rhymes_fast(words_text_collection, button_flags, comparing_word_part_of_speech, parts_of_speech);

	// поднятие всех регистров для красивого вывода и поиска слов по рифмам в
	for (WordData& record : data)
		record.word = capitalizeAllLetters(record.word);



};