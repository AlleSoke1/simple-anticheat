#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include "ScannerInterface.h"
#include "ProcessNameScanner.h"

void ProcessNameScanner::Process()
{
	if (!m_bRun) return;

	for (auto &it : m_pVecCDB)
	{
		OnScan(it);
	}
}

void ProcessNameScanner::OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData)
{
	if (type != m_ScannerType) return;
}

void ProcessNameScanner::OnScan(std::string strProcName) {
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			for (DWORD i = 0; i < strlen(pe32.szExeFile); ++i)
				pe32.szExeFile[i] = tolower(pe32.szExeFile[i]);

			if (_stricmp(pe32.szExeFile, strProcName.c_str()) == 0)
			{

				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);  // Close detected process
				TerminateProcess(hProcess, NULL);
				Sleep(300);

				DWORD exitCode = 0;
				if (GetExitCodeProcess(hProcess, &exitCode) == FALSE)
				{
					//Handle error here.
				}

				if (exitCode != STILL_ACTIVE)
				{
					break;
				} else {
					ExitProcess(0);
				}

			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
}