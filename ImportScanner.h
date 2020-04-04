#pragma once

#define MAX_IMPORT_DATA_SIZE 512
class ImportScanner : public ScannerInterface
{
public:
	ImportScanner()
	{
		m_ScannerType = ScannerInterface::IMPORT;
		ProtectDefaultImports();
	};

private:
	void ProtectDefaultImports();
	void AddItem(std::string name, void* pData, int nDataSize);
public:


	struct tImportData
	{
		std::string strName;
		int nSize;
		BYTE Data[MAX_IMPORT_DATA_SIZE];
		UINT64 nOffset;
		int nFailCount;
	};

private:
	std::vector<tImportData> m_pVecCDB;

public:
	virtual void OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData);
	virtual void Process();
};
