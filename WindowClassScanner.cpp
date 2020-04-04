#include <Windows.h>
#include <string>
#include <vector>
#include "ScannerInterface.h"
#include "WindowClassScanner.h"


void WindowClassScanner::Process()
{
	if (!m_bRun) return;

	EnumWindows(OnWndScan, NULL);
}

void WindowClassScanner::OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData)
{
	if (type != m_ScannerType) return;
}

BOOL CALLBACK WindowClassScanner::OnWndScan(HWND hwnd, LPARAM lParam)
{
	WindowClassScanner *pScanner = (WindowClassScanner*)lParam;
	if (pScanner) return FALSE;

	wchar_t wclass[255];

	GetClassNameW(hwnd, wclass, sizeof(wclass));

	for (auto &it : pScanner->m_pVecCDB)
	{
		if (wcsstr(wclass, it.c_str()) != 0)
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