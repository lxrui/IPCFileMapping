#pragma once
#include "IPCCommon.h"

class CIPCFileMappingServer
{
public:
	CIPCFileMappingServer(const char * mappingFileName, size_t cmdSize);
	~CIPCFileMappingServer();
	void m_addCommand(const size_t nSize, const void * cmd);
	unsigned char m_getIdx0();
	unsigned char m_getIdx1();
private:
	HANDLE _hMapFile;
	unsigned char * _pBuf;
	//µÍÎ»
	unsigned char _idx0;
	//¸ßÎ»
	unsigned char _idx1;

	bool _bStartSendMsg;
	size_t _cmdSize;
	size_t _bufSize;

protected:
	void _indexIncrease();
	void _idx0Add1();
	void _idx0Minus1();
	void _idx1Add1();
	void _idx1Minus1();
};

