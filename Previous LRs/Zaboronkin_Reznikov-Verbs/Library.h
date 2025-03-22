#pragma once

#include "Header.h"

int count_output = 0;

string entering_name_file(string);                         // ������� ���� ����� � �������� �����  ����� (2� ��������: input - ������� ����, output - �������� ����)
bool check_name_file(string);                              // ������� �������� ����� ����� �� ������� �������� ��������
bool check_txt_extension(string);                          // ������� �������� �� ������� ���������� '.txt' � ����� ����� �����
bool open_file(string, string);                            // ������� �������� �������� ����� ��� ������. (2� ��������: input - ������� ����, output - �������� ����)
bool check_inputFile_is_empty(string);                     // ������� �������� ����� �� �������
void print_text_string(string);                            // (�����) ������� ������ ������ �� ����� �� �������
void output_rhymes_in_file(map<string, vector<VerbInfo>>); // ������� ������ ������ � ����

// ������� ���� ����� ����� �����
string entering_name_file(string type_file)
{
    string user_answer = ""; // ������, �������� �������������
    bool flag_cycle = true;  // true - ����� �� �����
    // system("cls");
    do
    {
        if (type_file == "input")
            cout << "������� ��� �������� ����� ��� ������ ������.\n";
        else if (type_file == "output")
            cout << "������� ��� ��������� ����� ��� ������ � ���� ������.\n";
        else
        {
            cout << "!!!wrong type_file (input or output) in code!!! (29)\n";
            exit(2);
        }
        cout << "�� ������ ������������ ������ ��������� �����, ����� � '_'.\n"
             << "���������� '.txt' �������� �� �������.\n";

        cout << "\n~";
        getline(cin, user_answer);

        // �������� �� ������� ���������
        if (((int)user_answer[0] == FINISH_button or (int) user_answer[0] == BACK_button) and user_answer.length() == 1)
        {
            system("cls");
            return user_answer;
        }

        // �������� ����� �����
        if (!check_name_file(user_answer))
        {
            flag_cycle = false;
            continue;
        }
        else
        {
            string file_name = user_answer;

            // ���������� ���������� �����, ���� ��� ���
            if (!check_txt_extension(file_name))
                file_name += ".txt";

            // �������� �������� ����� � �������� �� �������
            if (open_file(file_name, type_file))
            {
                // ���� �������� ����� �������
                return file_name;
            }
            else
                flag_cycle = false; // ��������� ����
        }
    } while (!flag_cycle);
    return user_answer;
}
// ������� �������� ����� ����� �� ������� �������� ��������
bool check_name_file(string file_name)
{
    if (file_name.size() == 0)
    {
        system("cls");
        cout << "������!: ��� ����� �� ����� ���� ������.\n"
             << "���������� ������.\n\n";
        return false;
    }
    if (file_name.length() > MAX_FILENAME_LENGTH)
    {
        cout << "������!: ��� ����� ��������� ���������� ����� � "
             << MAX_FILENAME_LENGTH << " ��������!\n"
             << "���������� ������.\n\n";
        return false;
    }
    int count = 0; // ������� �������� ��������
    for (auto i = 0; i < file_name.length(); i++)
    {
        // �������� �� ��-�����, ��-����� � ��-"_" � ����� �����
        if (!isdigit(file_name[i]) && !isalpha(file_name[i]) && file_name[i] != '_' && file_name[i] != ')' && file_name[i] != '(')
            count++;
    }
    // �������� �� ���-�� �������� �������� � ������� ���������� �����
    if ((count == 1 and check_txt_extension(file_name)) or (count == 0))
        // ���� ��� �������� �������� (.txt �� �����������)
        return true;
    else // ���� ���� �������� �������
    {
        system("cls");
        cout << "������!: �������� ������ � ����� ����� '" << file_name << "'.\n"
             << "���������� ������.\n\n";
        return false;
    }
}
// ������� �������� ����� ����� �� ������� ���������� '.txt'
bool check_txt_extension(string file_name)
{
    if (file_name.length() >= len_txt && file_name.substr(file_name.length() - len_txt) == extension_txt)
        return true; // ���� ���� ����������
    else
        return false; // ���� ��� ����������
}
// ������� �������� ��������/�������� �����
bool open_file(string file_name, string type_file)
{
    if (type_file == "input")
    {
        // �������� ����� �� ������
        file_input.open(file_name, ios_base::in);

        // ���� ���� ��������
        if (file_input.is_open())
        {
            // ���� ���� �� ������ -> ������� - true
            if (!check_inputFile_is_empty(file_name))
            {
                // system("cls");
                // cout << "������� ���� '" << file_name << "' ������ ��� ������.\n";
                return true;
            }
            else
            {
                system("cls");
                cout << "������!: ������� ���� '" << file_name << "' �� �������� ������. ������ ����������.\n"
                     << "���������� ������.\n\n";
                return false;
            }
        }
        else
        {
            system("cls");
            cout << "������!: � ������� ����������� ���� � ������: '" << file_name << "'.\n"
                 << "���������� ������.\n\n";
            return false;
        }
    }
    else if (type_file == "output")
    {
        // �������� ����� �� ������
        file_output.open(file_name, ios_base::out);

        // ���� ���� �������� -> ������� - true
        if (file_output.is_open())
        {
            // system("cls");
            // cout << "�������� ���� '" << file_name << "' ������ � ������ ��� ������.\n";
            return true;
        }
        else
        {
            // system("cls");
            cout << "������ � �������� ����� '" << file_name << "' �� ������!\n";
            return false;
        }
    }
    else
    {
        cout << "!!!wrong type (rhymes or text) in code (170)!!!\n";
        exit(2);
    }
}
// ������� �������� ����� �� �������
bool check_inputFile_is_empty(string file_name)
{
    // true - ���� ������
    // false - ���� �� ������

    if (file_input.peek() == EOF) // ���� ������ ������ ��� ����� �����
    {
        system("cls");
        cout << "������� ���� '" << file_name << "' ����. ������ ����������.\n"
             << "���������� ������.\n";

        file_input.close();
        return true;
    }
    else
        return false;
}
// ������� �������� ������� ����� � ������ ������ � �������
bool check_outputFile_existence(string file_name)
{
    ifstream file(file_name); // �������� ������� ����

    // ���������, ������� �� ������� ����
    if (file.good()) // true - ���� ����������
    {
        // system("cls");
        // cout << "������!: ���� � ������ '" << file_name << "' ��� ����������.\n";
        return true;
    }
    else
        return false;
}

// ������� ��������������� ����� ����� ����� ��� ����
pair<bool, string> auto_name_OutputFile(string input_file_name, string type)
{
    int count = 1;
    input_file_name.erase(input_file_name.length() - len_txt, len_txt); // ������� ���������� � �������� �����
    string file_name;
    if (type == "rhymes")
        file_name = input_file_name + "(Rhymes).txt"; // ��� ��������� ����� � �������
    else if (type == "text")
        file_name = input_file_name + "(NumbText).txt"; // ��� ��������� ����� � ��������������� �������
    else
    {
        cout << "!!!wrong type (rhymes or text) in code!!! (220)\n";
        exit(2);
    }
    // ���� ��� ����� � ������� ����������
    while (check_outputFile_existence(file_name))
    {
        file_name.erase(file_name.length() - len_txt, len_txt); // ������� ����������
        if (count > 1)
            file_name.erase(file_name.length() - 3, 3); // ������� ���������, ���� ��� ��� ����
        file_name += "(" + to_string(count) + ").txt";
        count++;
    }

    if (open_file(file_name, "output"))
        return {true, file_name};
    else
        return {false, file_name};
}

// ������� ���������� ����� �������
void output_rhymes_in_file(map<string, vector<VerbInfo>> all_rhymes)
{
    int counter_verb = 0;
    int selection_num = 0;
    int unique_verbs_num = 0;

    file_output << "����� � ����� ����� �������������� ��������� ����������: \n"
                   "'������'(����� �����������, ���������� ����� ����� ������� � ���� �����������) \n"
                   "��������: �������� (2,3)"
                << ".\n";
    file_output << "----------------------------------------------------" << endl;
    for (const auto &elm : all_rhymes)
    {
        if (elm.second.size() <= 1)
        {
            continue;
        }
        selection_num += 1;
        file_output << "����� ����������� ��� #" << selection_num << "\n"
                    << endl;

        unique_verbs_num += elm.second.size();

        // // ������ ������� ���������� ������� ��������
        // for (auto &[key, verbVector] : all_rhymes)
        // {
        //     sort(verbVector.begin(), verbVector.end(), [](const VerbInfo &a, const VerbInfo &b)
        //          {
        //              return a.word < b.word; // ��������� �� ���� `word`
        //          });
        // }

        for (auto it = all_rhymes.begin(); it != all_rhymes.end(); ++it)
        {
            auto &verbVector = it->second;
            sort(verbVector.begin(), verbVector.end(), [](const VerbInfo &a, const VerbInfo &b)
                 { return a.word < b.word; });
        }

        for (size_t i = 0; i < elm.second.size(); i++)
        {
            unique_verbs_num = 0;
            for (size_t j = 0; j < elm.second.size(); j++)
            {
                if (elm.second[i].word == elm.second[j].word)
                {
                    continue;
                }
                file_output << elm.second[i].word << "(" << elm.second[i].sentenceNum << "," << elm.second[i].wordNum << ") - "
                            << elm.second[j].word << "(" << elm.second[j].sentenceNum << "," << elm.second[j].wordNum << ")" << endl;
                counter_verb++;
                unique_verbs_num++;
            }
            file_output << endl
                        << "����� '" << elm.second[i].word << "' ������� � �������� ����� " << unique_verbs_num << " ���." << endl
                        << endl;
        }
        file_output << "----------------------------------------------------" << endl;
    }
    file_output << "\n����� ����� ���: " << counter_verb << endl;
    file_output << "\n����� ����� ������� ����: " << selection_num << endl;
    file_output << "----------------------------------------------------" << endl;
    file_output.close();
}

// ������� ���������� ����� ��������������� �������
void output_newText_in_file(vector<vector<string>> text_in_vector)
{
    for (size_t i = 0; i < text_in_vector.size(); ++i)
    {
        file_output << (i + 1) << ". "; // ����� �����������
        for (const auto &word : text_in_vector[i])
        {
            file_output << word << " "; // ����� ���� ����� ������
        }
        file_output << "\n"; // ������� �� ����� ������ ����� ������� �����������
    }

    file_output.close();
}

// ������� ���������� ����� ��������������� �������
void output_newText_in_file(string text_in_string)
{

    // ���������� ��������� ��� ���������� ������ �� �����������
    // ��������� �����������, ��������������� �� !?., � ���� ����� ��� ���� �������, ������
    // regex sentence_regex(R"([^.?!]+[.?!])"); 
    regex sentence_regex(R"([^.?!]+[.?!]['"��)()]?)");
    //regex sentence_regex(R"((�.*?�|".*?"|[^.?!]+[.?!]))");
    sregex_iterator begin(text_in_string.begin(), text_in_string.end(), sentence_regex);
    sregex_iterator end;

    // �������� ���� �������� �������� ������
    // text_in_string.erase(remove(text_in_string.begin(), text_in_string.end(), '\n'), text_in_string.end());

    int sentence_number = 1;
    for (auto it = begin; it != end; ++it)
    {
        string sentence = it->str();

        sentence.erase(0, sentence.find_first_not_of(' ')); // ������� ������� � ������ �����������
        // sentence.erase(0, sentence.find_first_not_of('�'));
        // sentence.erase(0, sentence.find_first_not_of(')'));
        // sentence.erase(0, sentence.find_first_not_of('\n'));
        sentence.erase(remove(sentence.begin(), sentence.end(), '\n'), sentence.end()); // �������� ���� �������� �������� ������ 

        // ���������� ����������� � ���� � ����������
        file_output << sentence_number++ << ". " << sentence << "\n";
    }
    file_output.close();
}