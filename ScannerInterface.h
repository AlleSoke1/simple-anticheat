#pragma once


class ScannerInterface
{
public:
	enum eScannerType {
		IMPORT,
		MEMORY,
		PROCESS,
		WINDOWNAME,
		WINDOWCLASS,
		MAX,
	};

	struct CheatDefinition
	{
		int nDataSize;
		char *pData;
		DWORD dwOffset;
		CheatDefinition(int nDataSize, char *pData, DWORD dwOffset = NULL ) {
			this->nDataSize = nDataSize;
			this->pData = pData;
			this->dwOffset = dwOffset;
		}
	};

	ScannerInterface() { 
		m_ScannerType = MAX; 
		m_bRun = false; 
		m_bReportCheat = false; 
	}

	ScannerInterface(ScannerInterface::eScannerType type) { 
		m_ScannerType = type; 
		m_bRun = false;  
		m_bReportCheat = false; 
	}


protected:
	bool m_bRun;
	eScannerType m_ScannerType;
	bool m_bReportCheat;

	
public:
	void Process() { };
	void OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData) { };
	void CheatReport() { };

	eScannerType GetType() { return m_ScannerType; }

	bool IsRunning() { return m_bRun; }
	void Run(bool bRun) { m_bRun = bRun; }
};