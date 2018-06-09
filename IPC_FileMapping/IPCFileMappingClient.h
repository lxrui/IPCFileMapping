#pragma once
#include "IPCCommon.h"
class CIPCFileMappingClient
{
public:
	CIPCFileMappingClient(const char * mappingFileName, size_t cmdSize);
	~CIPCFileMappingClient();
	unsigned char m_getServerIdx0();
	unsigned char m_getServerIdx1();
	bool m_updateCmds(const size_t nCmdSize, void * B);
private:
	HANDLE _hMapFile;
	unsigned char * _pBuf;
	//µÍÎ»
	unsigned char _clientIdx0;
	//¸ßÎ»
	unsigned char _clientIdx1;
	std::string _sMappingFileName;
	bool _initClient();
	bool _bInitialized;
	bool _bIsUpToDate;
	unsigned char _serverIndex0;
	unsigned char _serverIndex1;
	size_t _cmdSize;
	size_t _bufSize;
};

