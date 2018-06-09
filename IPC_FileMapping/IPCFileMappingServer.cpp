#include "IPCFileMappingServer.h"

CIPCFileMappingServer::CIPCFileMappingServer(const char * mappingFileName, size_t cmdSize)
{
	_idx0 = 0x0;
	_idx1 = 0x0;
	_bStartSendMsg = false;
	_cmdSize = cmdSize;
	_bufSize = _cmdSize * MAX_INDEX0 * MAX_INDEX1 + 2 * sizeof(unsigned char);

	_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		_bufSize,                // maximum object size (low-order DWORD)
		mappingFileName);                 // name of mapping object

	if (_hMapFile == nullptr)
	{
		//printf("Could not create file mapping object (%d).\n", GetLastError());
		return;
	}
	_pBuf = (unsigned char *)MapViewOfFile(_hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		_bufSize);

	if (_pBuf == nullptr)
	{
		//printf("Could not map view of file (%d).\n", GetLastError());
	}
}


CIPCFileMappingServer::~CIPCFileMappingServer()
{
	UnmapViewOfFile(_pBuf);
	_pBuf = nullptr;
	CloseHandle(_hMapFile);
	_hMapFile = nullptr;
}

void CIPCFileMappingServer::m_addCommand(const size_t nSize, const void * cmd)
{
	if (_cmdSize != nSize)
	{
		//printf("Command length is incorrect (%zd != %d).\n", nSize, _cmdSize);
		return;
	}
	//increase index
	_indexIncrease();
	unsigned char indexes[2] = { _idx0, _idx1 };
	//for (size_t i = 0; i < nSize; ++i)
	//{
	//	printf("%02x ", cmd[i]);
	//}
	//printf("sended\n");
	if (nullptr != _pBuf)
	{
		memcpy((PVOID)_pBuf, indexes, 2 * sizeof(unsigned char));
		memcpy((PVOID)(_pBuf + 2 * sizeof(unsigned char) + ((size_t)_idx0 + (size_t)_idx1 * MAX_INDEX1) * _cmdSize), cmd, _cmdSize);
	}

}

unsigned char CIPCFileMappingServer::m_getIdx0()
{
	return _idx0;
}

unsigned char CIPCFileMappingServer::m_getIdx1()
{
	return _idx1;
}

void CIPCFileMappingServer::_idx0Add1()
{
	_idx0++;
	if (MAX_INDEX1 == _idx0)
	{
		_idx1Add1();
	}
	_idx0 = 0x3f & _idx0;
}
void CIPCFileMappingServer::_idx0Minus1()
{
	_idx0--;
	_idx0 = 0x3f & _idx0;
}
void CIPCFileMappingServer::_idx1Add1()
{
	_idx1++;
	_idx1 = 0x3 & _idx1;
}
void CIPCFileMappingServer::_idx1Minus1()
{
	_idx1--;
	_idx1 = 0x3 & _idx1;
}

void CIPCFileMappingServer::_indexIncrease()
{
	if (_bStartSendMsg)
	{
		_idx0Add1();
	}
	else
	{
		_bStartSendMsg = true;
	}
}