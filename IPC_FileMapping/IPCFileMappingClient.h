#pragma once
#include "IPCCommon.h"
class CIPCFileMappingClient
{
public:
	CIPCFileMappingClient(const char * mappingFileName, size_t cmdSize);
	~CIPCFileMappingClient();
	DWORD m_getIndx();
	bool m_updateCmds(const size_t nCmdSize, void * B);
private:
	HANDLE _hMapFile;
	unsigned char * _pBuf;
	DWORD _clientIndx;
	std::string _sMappingFileName;
	bool _initClient();
	bool _bInitialized;
	bool _bIsUpToDate;
	DWORD _serverIndx;
	size_t _cmdSize;
	size_t _bufSize;
};

