#pragma once

class RLKTGuard : public CSingleton<RLKTGuard>
{
public:
	RLKTGuard();
	~RLKTGuard();

	bool Initialize();

	void AddScanner(ScannerInterface::eScannerType type, ScannerInterface *pScanner, bool bRun);
	ScannerInterface* GetScanner(ScannerInterface::eScannerType type);

	static DWORD WINAPI ScannerThread(LPVOID ptr);

	void OnRecvCheatDatabase(char *pBuffer, int nBufferSize);

private:
	std::vector<ScannerInterface*> m_pVecScanners;
	DWORD m_dwScannerThreadID;
	HANDLE m_hScannerThread;
};