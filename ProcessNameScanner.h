#pragma once

class ProcessNameScanner : public ScannerInterface
{
public:
	ProcessNameScanner() {
		m_ScannerType = ScannerInterface::WINDOWCLASS;
	};
	~ProcessNameScanner();

private:
	std::vector<std::string> m_pVecCDB;
	void OnScan(std::string strProcName);

public:
	virtual void OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData);
	virtual void Process();
};