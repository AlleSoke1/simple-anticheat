#include <Windows.h>
#include <string>
#include <vector>
#include "ScannerInterface.h"
#include "WindowNameScanner.h"


void WindowNameScanner::Process()
{
	if (!m_bRun) return;

	EnumWindows(OnWndScan, reinterpret_cast<LPARAM>(this));
}

void WindowNameScanner::OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData)
{
	if (type != m_ScannerType) return;

	for (auto &it : vecData)
	{
		OutputDebugString(it.pData);
	}
}

BOOL CALLBACK WindowNameScanner::OnWndScan(HWND hwnd, LPARAM lParam)
{
	WindowNameScanner *pScanner = (WindowNameScanner*)lParam;
	if (pScanner) return FALSE;

	wchar_t title[255];

	GetWindowTextW(hwnd, title, sizeof(title));

	for (auto &it : pScanner->m_pVecCDB)
	{
		if (wcsstr(title, it.c_str()) != 0)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			if (pScanner->m_bReportCheat)
			{
				pScanner->CheatReport();
			}
		}
	}

	return TRUE;
}
