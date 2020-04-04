#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include "ScannerInterface.h"
#include "MemoryScanner.h"

void MemoryScanner::Process()
{
	if (!m_bRun) return;

	OnScan();
}

void MemoryScanner::OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData)
{
	if (type != m_ScannerType) return;
}


void MemoryScanner::OnScan() {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (hProcess != NULL)
				{
					if (ScanProcess(hProcess))
					{
						TerminateProcess(hProcess, NULL);

						Sleep(300);

						DWORD exitCode = 0;
						if (GetExitCodeProcess(hProcess, &exitCode) == FALSE)
						{
						}
						if (exitCode != STILL_ACTIVE)
						{
							break;
						}
						else {
							ExitProcess(0);
						}
					}
				}
			} while (Process32Next(hProcessSnap, &pe32));
		}
	}

	CloseHandle(hProcessSnap);
}

bool MemoryScanner::ScanProcess(HANDLE hProcess)
{
	for (auto &it : m_pVecCDB)
	{
		char aTmpBuffer[MAX_DUMP_SIZE];
		SIZE_T aBytesRead = 0;

		BOOL bSuccessRead = ReadProcessMemory(hProcess, (LPCVOID)it.dwOffset, (LPVOID)aTmpBuffer, sizeof(aTmpBuffer), &aBytesRead);
		if (!bSuccessRead) continue; //Not enough permissions?

		if (memcmp(aTmpBuffer, it.bufferData, MAX_DUMP_SIZE) == 0)
		{
			return true;
		}
	}
	return false;
}