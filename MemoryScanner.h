#pragma once

#define MAX_DUMP_SIZE 32

class MemoryScanner : public ScannerInterface
{
public:
	MemoryScanner() {
		m_ScannerType = ScannerInterface::WINDOWCLASS;
	};
	
	struct tProcessData
	{
		DWORD dwOffset;
		BYTE bufferData[MAX_DUMP_SIZE];
	};


private:
	std::vector<tProcessData> m_pVecCDB;
	void OnScan();

	bool ScanProcess(HANDLE hProcess);

public:
	virtual void OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData);
	virtual void Process();
};