#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

// ������ ������������ ���� ������������ ��� ������ MyStem � ��������� ����� ���� ������� �� ���� MyStem �����

// ��������� ���� � mystem.exe
std::string getMystemPath() {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return "\"mystem.exe\"";  // fallback � ���������
    }

    std::string fullPath(buffer, length);

    // ����� ������� ���������� �����
    size_t pos = fullPath.find_last_of("\\/");  // ������ ��������� ���� ��� �������� ���� ��� ��������� ���������� �� �����
    if (pos == std::string::npos) {
        return "\"mystem.exe\"";  // ���� ���� �� ������ � ������� ������ ��� ������������ �����
    }

    // ������ ���� � ������� �������
    std::string mystemPath = fullPath.substr(0, pos + 1) + "mystem.exe";  // ���� � ���������� + ��� ������������ �����
    return "\"" + mystemPath + "\"";  // ����������� ���� � �������, ����� �������� ������� � ���������
}

class MystemProcess {
private:
    HANDLE hChildStd_IN_Wr = NULL;      // ���������� ��� ������ � stdin ��������� ��������
    HANDLE hChildStd_OUT_Rd = NULL;     // ���������� ��� ������ �� stdout ��������� ��������
    PROCESS_INFORMATION piProcInfo{};   // ��������� � ����������� � ��������� ��������

public:
    MystemProcess() {
        SECURITY_ATTRIBUTES saAttr{};
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;  // ����������� ����� ������������� �������� ���������
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hChildStd_IN_Rd = NULL;
        HANDLE hChildStd_OUT_Wr = NULL;

        // �������� ������ ��� stdin � stdout
        if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0))
            throw std::runtime_error("Stdout pipe creation failed.");  // ���� ��� ������ stdout �� ������� �������

        if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdout read handle inheritance failed.");  // ������ �� ������������ ����������� ������ stdout

        if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0))
            throw std::runtime_error("Stdin pipe creation failed.");  // ���� ��� ������ � stdin �� ������� �������

        if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdin write handle inheritance failed.");  // ������ �� ������������ ����������� ������ stdin

        STARTUPINFOA siStartInfo{};
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = hChildStd_OUT_Wr;   // ��������������� stderr ��������� �������� � ����
        siStartInfo.hStdOutput = hChildStd_OUT_Wr;  // ��������������� stdout ��������� �������� � ����
        siStartInfo.hStdInput = hChildStd_IN_Rd;    // stdin ��������� �������� ����� �������� �� �����
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;  // �������� ������������ ���������������� ������

        std::string command = getMystemPath() + " -nig";  // ������������ ������� ������� MyStem � �����������

        std::vector<char> cmd(command.begin(), command.end());
        cmd.push_back('\0');  // ���������� ������������ ���� ��� ���������� �������� ������� � CreateProcess

        if (!CreateProcessA(NULL, cmd.data(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo))
            throw std::runtime_error("Failed to launch mystem.exe process");  // ������ ��������� �������� �� ������

        // ��������� �������� ����� ������
        CloseHandle(hChildStd_IN_Rd);    // ������ �� ����� � ������������ ��������
        CloseHandle(hChildStd_OUT_Wr);   // �������� �� ����� � stdout ��������� ��������
    }

    std::string analyze(const std::string& text) {
        DWORD written;
        std::string input = text + "\n";  // mystem ������� ���������� ����

        // ���������� � ����
        WriteFile(hChildStd_IN_Wr, input.c_str(), static_cast<DWORD>(input.size()), &written, NULL);  // �������� ������ � stdin mystem

        // ������ ������
        char buffer[4096];
        DWORD read;
        std::string result;

        while (true) {
            BOOL success = ReadFile(hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &read, NULL);  // �������� �� stdout �����
            if (!success || read == 0) break;  // ����� ��� ������ ��� ��������� ������
            buffer[read] = '\0';  // ���������� ���������� ������
            result += buffer;

            // mystem ������� �� �������. ����� ������������ �� ������ ������.
            if (result.find('\n') != std::string::npos) break;  // ��������������� ����� ��������� ������ ������ ����������
        }

        return result;
    }

    ~MystemProcess() {
        CloseHandle(hChildStd_IN_Wr);      // �������� ����������� ������
        CloseHandle(hChildStd_OUT_Rd);     // �������� ����������� ������

        TerminateProcess(piProcInfo.hProcess, 0);  // ���������� ��������� ��������
        CloseHandle(piProcInfo.hProcess);          // ������������ ����������� ��������
        CloseHandle(piProcInfo.hThread);           // ������������ ����������� ������
    }
};

