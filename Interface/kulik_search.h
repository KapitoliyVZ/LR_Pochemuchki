#pragma once
#include "standard_libraries.h"

// ������ ������������ ���� ������������ ��� ���������� ��������� ��������� ������ �� ������ �������� �����������

// ������� ��� �������� �������� �������� ������ �� ������
void removeNewlines(string& text) {
	text.erase(remove(text.begin(), text.end(), '\n'), text.end());
}

vector<vector<string>> parse_sentences(string text)   //���������� ������ �� �����������
{
	vector<vector<string>> sentences = {};
	string line = "";
	string word = "";
	bool quote_is_open = false; //���� �������� �������
	vector<string> current_sentence;
	removeNewlines(text);

	line = text;

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] != '.' && line[i] != '!' && line[i] != '?' && line[i] != '�')
		{
			if (line[i] == ',' || line[i] == ':')  //���� ����������� ������� ��� ���������
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

			if (line[i] == '�')  //���� ����������� ����������� �������
			{
				quote_is_open = true;
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == '�')  //���� ����������� ����������� �������
			{
				quote_is_open = false;

				//���� ����� ������� ���� ���� �� ������� ������ � ��������� �����, ���������� �����������. 
				//("������!" - ������ ��.)
				if ((i < line.size() - 4) && (line[i + 2] == '�' && (line[i + 4] >= '�' && line[i + 4] <= '�')) || (line[i + 1] == ',') || (line[i + 1] == '.'))
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

				//��������, �� ��� �� ����� ��������� ����������� ���� �����������; 
				//���� �� ���, �� ����� ���������� ����������� ������� � ��������� �����������; (������: "������!")
				if (line[i - 1] == '?!' || line[i - 1] == '!' || line[i - 1] == '?' || line[i - 1] == '�' || line[i - 1] == '.')
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

			if (line[i] == ' ') //���� ���������� ������
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
			}
			else  //���� �����
			{
				word += line[i];
			}
		}


		else   //���� ���������� ����������� ���� �����������
		{
			if (quote_is_open == true)  //���������� ���������� �����������, ���� ������� �������, �� ���������� ����������� ���� �����������  (������:  "� ������!")         
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

			if (line[i] == '�' && i < line.size() - 2)  //���� ��������� ����������� ������ �����������, �� � ����� ������, �� ���������� ���������� (������: ����... ������)
			{
				if (line[i + 2] >= '�' && line[i + 2] <= '�') //���� ����� ����� ������ ���������, �� ��� ���� ����������� 
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

			//���� ����� ����������� ������ ������, ���������� ����������  
			if (line[i] == '.' && i < line.size() - 1)
			{
				if (line[i + 1] == ')')    //���� ��������� ����� ����� ������ ) , �� ���������� ������ ��������� �������� � ������� �����������              
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
					i++;      //����������� �������������� �������, ��� ��� �������� ����� ������, ������ ����� �����
					continue;
				}

			}

			//��� �������: ���� ����� ?; ...; !; . ���� ���� �� ������� ������ � ��������� �����, ���������� �����������
			if ((line[i] == '!' || line[i] == '?' || line[i] == '.' || line[i] == '�') && (i < line.size() - 4))
			{
				if (line[i + 2] == '�' && (line[i + 4] >= '�' && line[i + 4] <= '�'))   //��������, ��� ����� ������ ���� "-", � ����� ����� ������ ��������� ����� (������: - ������! - ������� ���.)
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