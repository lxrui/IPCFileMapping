#pragma once
#include "IPCCommon.h"

class CIPCFileMappingServer
{
public:
	CIPCFileMappingServer(const char * mappingFileName, size_t cmdSize);
	~CIPCFileMappingServer();
	void m_addCommand(const size_t nSize, const void * cmd);
private:
	HANDLE _hMapFile;
	unsigned char * _pBuf;
	
	DWORD _indx;

	bool _bStartSendMsg;
	size_t _cmdSize;
	DWORD _bufSize;

protected:
	void _indexIncrease();
};

