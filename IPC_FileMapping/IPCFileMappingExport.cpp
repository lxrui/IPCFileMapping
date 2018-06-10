#define IPCAPI extern "C" __declspec(dllexport)
#include "IPCFileMappingExport.h"
#include "IPCFileMappingClient.h"
#include "IPCFileMappingServer.h"

CIPCFileMappingServer * pIPCServer;
CIPCFileMappingClient * pIPCClient;

IPCAPI void WINAPI IPCFileMapping::createServer(const char * mappingFileName, size_t cmdSize)
{
	pIPCServer = new CIPCFileMappingServer(mappingFileName, cmdSize);
}

IPCAPI void WINAPI IPCFileMapping::destroyServer()
{
	delete pIPCServer;
	pIPCServer = nullptr;
}

IPCAPI void WINAPI IPCFileMapping::sendCmd(void * cmdArray, size_t nCmdSize)
{
	if (nullptr != pIPCServer)
	{
		pIPCServer->m_addCommand(nCmdSize, cmdArray);
	}
}

IPCAPI void WINAPI IPCFileMapping::createClient(const char * mappingFileName, size_t cmdSize)
{
	pIPCClient = new CIPCFileMappingClient(mappingFileName, cmdSize);
}

IPCAPI void WINAPI IPCFileMapping::destroyClient()
{
	delete pIPCClient;
	pIPCClient = nullptr;
}

IPCAPI bool WINAPI IPCFileMapping::recvCmd(void * cmdArray, size_t nCmdSize)
{
	if (nullptr != pIPCClient)
	{
		return pIPCClient->m_updateCmds(nCmdSize, cmdArray);
	}
	return false;
}