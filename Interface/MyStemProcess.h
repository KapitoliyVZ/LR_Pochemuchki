#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

// получение пути к mystem.exe
std::string getMystemPath() {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return "\"mystem.exe\"";  // fallback с кавычками
    }

    std::string fullPath(buffer, length);

    // Найдём позицию последнего слэша
    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos) {
        return "\"mystem.exe\"";
    }

    // Соберём путь и добавим кавычки
    std::string mystemPath = fullPath.substr(0, pos + 1) + "mystem.exe";
    return "\"" + mystemPath + "\"";
}


std::string get_filepath(const std::string& filename) {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return filename;  // fallback без кавычек
    }
    std::string fullPath(buffer, length);

    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos) {
        return filename;
    }

    std::string path = fullPath.substr(0, pos + 1) + filename;
    return path;
}


class MystemProcess {
private:
    HANDLE hChildStd_IN_Wr = NULL;
    HANDLE hChildStd_OUT_Rd = NULL;
    PROCESS_INFORMATION piProcInfo{};

public:
    MystemProcess() {
        SECURITY_ATTRIBUTES saAttr{};
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hChildStd_IN_Rd = NULL;
        HANDLE hChildStd_OUT_Wr = NULL;

        // Создание пайпов для stdin и stdout
        if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0))
            throw std::runtime_error("Stdout pipe creation failed.");

        if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdout read handle inheritance failed.");

        if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0))
            throw std::runtime_error("Stdin pipe creation failed.");

        if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdin write handle inheritance failed.");

        STARTUPINFOA siStartInfo{};
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = hChildStd_OUT_Wr;
        siStartInfo.hStdOutput = hChildStd_OUT_Wr;
        siStartInfo.hStdInput = hChildStd_IN_Rd;
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        std::string command = getMystemPath() + " -nig";

        std::vector<char> cmd(command.begin(), command.end());
        cmd.push_back('\0');

        if (!CreateProcessA(NULL, cmd.data(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo))
            throw std::runtime_error("Failed to launch mystem.exe process");

        // Закрываем ненужные концы пайпов
        CloseHandle(hChildStd_IN_Rd);
        CloseHandle(hChildStd_OUT_Wr);
    }

    std::string analyze(const std::string& text) {
        DWORD written;
        std::string input = text + "\n";

        // Записываем в пайп
        WriteFile(hChildStd_IN_Wr, input.c_str(), static_cast<DWORD>(input.size()), &written, NULL);


        // Добавляем задержку перед чтением данных, чтобы процесс успел записать их в выходной поток
        std::this_thread::sleep_for(std::chrono::microseconds(1));  // Задержка в 1 микросекунду

        // Чтение вывода
        char buffer[4096];
        DWORD read;
        std::string result;

        while (true) {
            BOOL success = ReadFile(hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &read, NULL);
            if (!success || read == 0) break;
            buffer[read] = '\0';
            result += buffer;

            // mystem выводит по строкам. Можно остановиться на первой строке.
            if (result.find('\n') != std::string::npos) break;
        }

        return result;
    }


    ~MystemProcess() {
        CloseHandle(hChildStd_IN_Wr);
        CloseHandle(hChildStd_OUT_Rd);

        TerminateProcess(piProcInfo.hProcess, 0);
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
    }
};
