#include <Windows.h>
#include <string>
#include <vector>
#include "ScannerInterface.h"
#include "ImportScanner.h"

void ImportScanner::AddItem(std::string name, void* pData, int nDataSize)
{
	tImportData data;
	memset(&data, 0, sizeof(tImportData));

	data.strName = name;
	data.nSize = nDataSize;
#ifdef _WIN32
	data.nOffset = (DWORD)pData;
#elif _WIN64
	data.nOffset = (UINT64)pData;
#endif

	memcpy_s(&data.Data, MAX_IMPORT_DATA_SIZE, pData, nDataSize);
};

void ImportScanner::ProtectDefaultImports()
{
	AddItem("GetTickCount", GetTickCount, 8); //used by all Speed Hacks
	AddItem("GetTickCount64", GetTickCount64, 8); //used by all Speed Hacks
	AddItem("QueryPerformanceCounter", QueryPerformanceCounter, 8); //used by all Speed Hacks
	//AddItem("RtlQueryPerformanceCounter", RtlQueryPerformanceCounter, 8); //Kernel function, can't hook from userspace.
	AddItem("send", send, 8); //used by packet capture/editors
	AddItem("recv", recv, 8); //used by packet capture/editors
}

void ImportScanner::Process()
{
	if (!m_bRun) return;

	for (auto& it : m_pVecCDB)
	{
		if (it.nFailCount > 10)
		{
			//Something is wrong with this import
			
			//try restore?

			//check for any JMP/Trampoline?
		}

		if (_memicmp(it.Data, (LPVOID)it.nOffset, it.nSize) != 0)
		{
			CheatReport();
			it.nFailCount++;
		}
	}
}

void ImportScanner::OnRecvCheatDBPacket(ScannerInterface::eScannerType type, std::vector<ScannerInterface::CheatDefinition> vecData)
{
	if (type != m_ScannerType) return;

	for (auto& it : vecData)
	{
		OutputDebugString(it.pData);
	}
}