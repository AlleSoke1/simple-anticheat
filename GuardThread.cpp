#include <windows.h>
#include <vector>
#include <string>
#include "ScannerInterface.h"
#include "ImportScanner.h"
#include "MemoryScanner.h"
#include "ProcessNameScanner.h"
#include "WindowNameScanner.h"
#include "WindowClassScanner.h"
#include "Singleton.h"
#include "Stream.h"
#include "GuardThread.h"


#define SECURITY_WIN32 
#define SCAN_INTERVAL 1000

RLKTGuard::RLKTGuard()
{
}

RLKTGuard::~RLKTGuard()
{
	for (auto& it : m_pVecScanners)
	{
		delete it;
	}

	m_pVecScanners.clear();
}

bool RLKTGuard::Initialize()
{
	AddScanner(ScannerInterface::eScannerType::IMPORT, new ImportScanner(), true);
	AddScanner(ScannerInterface::eScannerType::MEMORY, new MemoryScanner(), true);
	AddScanner(ScannerInterface::eScannerType::PROCESS, new ProcessNameScanner(), true);
	AddScanner(ScannerInterface::eScannerType::WINDOWCLASS, new WindowNameScanner(), true);
	AddScanner(ScannerInterface::eScannerType::WINDOWNAME, new WindowClassScanner(), true);

	HANDLE m_hScannerThread = CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ScannerThread), this, 0, &m_dwScannerThreadID);
	if (m_hScannerThread == NULL)
	{
		//Failed to start.
		DWORD dwError = GetLastError();
		//
		return false;
	}

	return true;
}

void RLKTGuard::AddScanner(ScannerInterface::eScannerType type, ScannerInterface *pScanner, bool bRun)
{
	pScanner->Run(bRun);

	m_pVecScanners.push_back(pScanner);
}

ScannerInterface * RLKTGuard::GetScanner(ScannerInterface::eScannerType type)
{
	for(auto &it : m_pVecScanners)
	{
		if (it->GetType() == type)
			return it;
	}

	return NULL;
}

void RLKTGuard::OnRecvCheatDatabase(char *pBuffer, int nBufferSize)
{
	CMemoryStream Stream;
	Stream.Initialize(pBuffer, nBufferSize);
	
	int nCurrentPos = Stream.Tell();
	int Header;
	Stream.Read(&Header, sizeof(int));

	ScannerInterface *pScanner = NULL;
	int nSize = 0; 
	Stream.Read(&nSize, sizeof(int));	
	std::vector<ScannerInterface::CheatDefinition> pVec;

	//MEMORY
	for (int i = 0; i < nSize; i++)
	{
		int nDataSize = 0;
		DWORD dwOffset;
		Stream.Read(&nDataSize, sizeof(int));
		char *pData = (char*)malloc(nDataSize);
		Stream.ReadBuffer(pData, nDataSize);
		Stream.Read(&dwOffset, sizeof(DWORD));

		pVec.push_back(ScannerInterface::CheatDefinition(nDataSize, pData, dwOffset));
	}
	pScanner = GetScanner(ScannerInterface::eScannerType::MEMORY);
	if (pScanner)
		pScanner->OnRecvCheatDBPacket(ScannerInterface::eScannerType::MEMORY, pVec);
	

	//PROCESS	
	Stream.Read(&nSize, sizeof(int));		
	pVec.clear();
	for (int i = 0; i < nSize; i++)
	{
		int nDataSize = 0;
		Stream.Read(&nDataSize, sizeof(int));
		char *pData = (char*)malloc(nDataSize);
		Stream.ReadBuffer(pData, nDataSize);

		pVec.push_back(ScannerInterface::CheatDefinition(nDataSize, pData));
	}
	pScanner = GetScanner(ScannerInterface::eScannerType::PROCESS);
	if (pScanner)
		pScanner->OnRecvCheatDBPacket(ScannerInterface::eScannerType::PROCESS, pVec);

	//WINDOWCLASS	
	Stream.Read(&nSize, sizeof(int));
	pVec.clear();
	for (int i = 0; i < nSize; i++)
	{
		int nDataSize = 0;
		Stream.Read(&nDataSize, sizeof(int));
		char *pData = (char*)malloc(nDataSize);
		Stream.ReadBuffer(pData, nDataSize);

		pVec.push_back(ScannerInterface::CheatDefinition(nDataSize, pData));
	}
	pScanner = GetScanner(ScannerInterface::eScannerType::WINDOWCLASS);
	if (pScanner)
		pScanner->OnRecvCheatDBPacket(ScannerInterface::eScannerType::WINDOWCLASS, pVec);

	//WINDOWNAME	
	Stream.Read(&nSize, sizeof(int));
	pVec.clear();
	for (int i = 0; i < nSize; i++)
	{
		int nDataSize = 0;
		Stream.Read(&nDataSize, sizeof(int));
		char *pData = (char*)malloc(nDataSize);
		Stream.ReadBuffer(pData, nDataSize);

		pVec.push_back(ScannerInterface::CheatDefinition(nDataSize, pData));
	}
	pScanner = GetScanner(ScannerInterface::eScannerType::WINDOWNAME);
	if (pScanner)
		pScanner->OnRecvCheatDBPacket(ScannerInterface::eScannerType::WINDOWNAME, pVec);

}

DWORD WINAPI RLKTGuard::ScannerThread(LPVOID ptr)
{
	RLKTGuard *pClass = (RLKTGuard*)ptr;
	if (!pClass) return 0;

	do
	{
		for (auto &it : pClass->m_pVecScanners)
		{
			it->Process();
		}

		Sleep(SCAN_INTERVAL);
	} while (1);

	return 0;
}

