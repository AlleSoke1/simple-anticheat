#pragma once

class WindowClassScanner : public ScannerInterface
{
public:
	WindowClassScanner() 
	{
		m_ScannerType = ScannerInterface::WINDOWCLASS;
	};

private:
	std::vector<std::wstring> m_pVecCDB;
	static BOOL CALLBACK OnWndScan(HWND hwnd, LPARAM lParam);

public:
	virtual void OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData);
	virtual void Process();
};