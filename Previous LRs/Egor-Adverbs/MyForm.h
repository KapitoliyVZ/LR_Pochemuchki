#pragma once
#include "Header.h"


namespace ProjectXXX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace msclr::interop;


	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog1;
	private: System::Windows::Forms::Button^ OpenButton;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;





	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label3;


	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ pairsCountLabel;
	private: System::Windows::Forms::Button^ InfoButton1;

	private: System::Windows::Forms::GroupBox^ groupBoxOpen;





	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->OpenButton = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->pairsCountLabel = (gcnew System::Windows::Forms::Label());
			this->InfoButton1 = (gcnew System::Windows::Forms::Button());
			this->groupBoxOpen = (gcnew System::Windows::Forms::GroupBox());
			this->groupBoxOpen->SuspendLayout();
			this->SuspendLayout();
			// 
			// OpenButton
			// 
			this->OpenButton->Location = System::Drawing::Point(54, 33);
			this->OpenButton->Name = L"OpenButton";
			this->OpenButton->Size = System::Drawing::Size(104, 23);
			this->OpenButton->TabIndex = 1;
			this->OpenButton->Text = L"Открыть файл";
			this->OpenButton->UseVisualStyleBackColor = true;
			this->OpenButton->Click += gcnew System::EventHandler(this, &MyForm::OpenButton_Click);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->textBox1->Location = System::Drawing::Point(13, 149);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(300, 400);
			this->textBox1->TabIndex = 2;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// richTextBox1
			// 
			this->richTextBox1->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->richTextBox1->Location = System::Drawing::Point(357, 149);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->richTextBox1->Size = System::Drawing::Size(300, 400);
			this->richTextBox1->TabIndex = 3;
			this->richTextBox1->Text = L"";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 133);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(139, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Исходный текст из файла";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(354, 133);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(129, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Результат поиска рифм";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(354, 99);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(121, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Количество пар рифм:";
			// 
			// pairsCountLabel
			// 
			this->pairsCountLabel->AutoSize = true;
			this->pairsCountLabel->Location = System::Drawing::Point(492, 99);
			this->pairsCountLabel->Name = L"pairsCountLabel";
			this->pairsCountLabel->Size = System::Drawing::Size(0, 13);
			this->pairsCountLabel->TabIndex = 13;
			// 
			// InfoButton1
			// 
			this->InfoButton1->Location = System::Drawing::Point(196, 11);
			this->InfoButton1->Name = L"InfoButton1";
			this->InfoButton1->Size = System::Drawing::Size(22, 22);
			this->InfoButton1->TabIndex = 14;
			this->InfoButton1->Text = L"\?";
			this->InfoButton1->UseVisualStyleBackColor = true;
			this->InfoButton1->Click += gcnew System::EventHandler(this, &MyForm::InfoButton_Click);
			// 
			// groupBoxOpen
			// 
			this->groupBoxOpen->Controls->Add(this->OpenButton);
			this->groupBoxOpen->Controls->Add(this->InfoButton1);
			this->groupBoxOpen->Location = System::Drawing::Point(49, 12);
			this->groupBoxOpen->Name = L"groupBoxOpen";
			this->groupBoxOpen->Size = System::Drawing::Size(224, 100);
			this->groupBoxOpen->TabIndex = 15;
			this->groupBoxOpen->TabStop = false;
			this->groupBoxOpen->Text = L"Открытие файла";
			// 
			// MyForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(684, 561);
			this->Controls->Add(this->groupBoxOpen);
			this->Controls->Add(this->pairsCountLabel);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->textBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Поисковик рифм";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBoxOpen->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	private: System::Void OpenButton_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ openFileName = ""; 
		string fileName;
		bool enabled = true;
		this->richTextBox1->Clear();
		this->textBox1->Clear();
		if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK) { 
			openFileName = openFileDialog1->FileName;
			// Преобразование System::String^ в std::string
			fileName = marshal_as<std::string>(openFileName);
			if (fileName.substr(fileName.length() - 4) != ".txt") {
				MessageBox::Show(this, "Файл не был открыт.", "Ошибка", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				pairsCountLabel->Text = "0"; enabled = false;
			}
		}
		try {
			if (enabled) {
				StreamReader^ file = File::OpenText(openFileName);
				textBox1->Text = file->ReadToEnd();
			}
		}
		catch (Exception^ e)
		{
			if (enabled) {
				MessageBox::Show(this, "Файл не был открыт.", "Ошибка", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				pairsCountLabel->Text = "0"; enabled = false;
			}
		}
		
		if (enabled) {
			String^ inputText = this->textBox1->Text;
			this->richTextBox1->Clear();
			richTextBox1->Text = textBox1->Text;
			
			int rhymeCount = 0;
			vector<string> adverbList = {};
			vector<string> rhymeList = {};

			string saveFileName = fileName.substr(fileName.find_last_of('\\') + 1);
			saveFileName = "Рифмы наречий " + saveFileName;
			string sentenceFileName = fileName.substr(fileName.find_last_of('\\') + 1);
			sentenceFileName = "Пронумерованные предложения " + sentenceFileName;

			// Анализируем файл на наличие рифм в наречиях и выводим в richTextBox1
			inputFromFile(fileName.c_str(), adverbList, true);
			int countZ = 0;
			for (auto ddd : adverbList) { cout << ddd << endl; countZ++; }
			cout << "Kolichestvo: " << countZ << endl;
			doubleRhymeList(adverbList, rhymeList);
			doubleRhymeToTextBox(rhymeList, this->richTextBox1, &rhymeCount);

			// Преобразование int в System::String^
			System::String^ rhymeCountStr = rhymeCount.ToString();

			// Установка значения rhymeCount в pairsCountLabel
			pairsCountLabel->Text = rhymeCountStr;
			if (rhymeCount == 0) { 
				MessageBox::Show(this, "Файл с рифмами не был сохранен, так как в тексте не было обнаружено рифм. " , "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Information);
				enabled = false; 
			}
			if (enabled) {
				// Проверяем, существует ли файл
				if (File::Exists(marshal_as<System::String^>(saveFileName))) {
					// Предлагаем пользователю перезаписать файл
					string message = "Файл c рифмами из текста " + fileName.substr(fileName.find_last_of('\\') + 1) + " уже существует. \nПерезаписать его? ";
					System::Windows::Forms::DialogResult result = MessageBox::Show(this, marshal_as<System::String^>(message), "Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question);
					if (result == System::Windows::Forms::DialogResult::Yes) {
						// Перезаписываем файл
						rhymePairsToFile(rhymeList, saveFileName, rhymeCount);
						inputFromFile(fileName.c_str(), adverbList, false);
						MessageBox::Show(this, "Файл с рифмами успешно сохранен и расположен в той же директории, где находится программа. \nНазвание файла: " + marshal_as<System::String^>(saveFileName) +
							"\nТакже был сохранен файл с пронумерованными предложениями из текста. \nНазвание файла: " + marshal_as<System::String^>(sentenceFileName)
							, "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Information);
					}
					else {
						MessageBox::Show(this, "Файл не был сохранен.", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Information);
					}
				}
				else {
					// Сохраняем файл
					rhymePairsToFile(rhymeList, saveFileName, rhymeCount);
					inputFromFile(fileName.c_str(), adverbList, false);
					MessageBox::Show(this, "Файл с рифмами успешно сохранен и расположен в той же директории, где находится программа. \nНазвание файла: " + marshal_as<System::String^>(saveFileName) + 
						"\nТакже был сохранен файл с пронумерованными предложениями из текста. \nНазвание файла: " + marshal_as<System::String^>(sentenceFileName)
						, "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Information);
				}
			}
		}
	}
		 
	private: System::Void InfoButton_Click(System::Object^ sender, System::EventArgs^ e) {
		MessageBox::Show(this, "Выберите файл с расширением 'txt' с кодировкой UTF-8", "Сообщение", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
	}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
}

};
}