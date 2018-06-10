#pragma once
#ifndef IPCAPI 
#define IPCAPI extern "C" __declspec(dllimport)  
#endif 
#include <windows.h>

namespace IPCFileMapping {
	IPCAPI void WINAPI createServer(const char * mappingFileName, size_t cmdSize);
	IPCAPI void WINAPI destroyServer();
	IPCAPI void WINAPI sendCmd(void * cmdArray, size_t nCmdSize);

	IPCAPI void WINAPI createClient(const char * mappingFileName, size_t cmdSize);
	IPCAPI void WINAPI destroyClient();
	IPCAPI bool WINAPI recvCmd(void * cmdArray, size_t nCmdSize);
}