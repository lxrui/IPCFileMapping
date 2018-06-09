#include "IPCFileMappingServer.h"

CIPCFileMappingServer::CIPCFileMappingServer(const char * mappingFileName, size_t cmdSize)
{
	_indx = 0;
	_bStartSendMsg = false;
	_cmdSize = cmdSize;
	_bufSize = _cmdSize * MAX_INDEX + sizeof(DWORD);

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
		return;
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
	//for (size_t i = 0; i < nSize; ++i)
	//{
	//	printf("%02x ", cmd[i]);
	//}
	//printf("sended\n");
	if (nullptr != _pBuf)
	{
		memcpy((PVOID)_pBuf, &_indx, sizeof(DWORD));
		memcpy((PVOID)(_pBuf + sizeof(DWORD) + (_indx * _cmdSize)), cmd, _cmdSize);
	}

}

void CIPCFileMappingServer::_indexIncrease()
{
	if (_bStartSendMsg)
	{
		_indx++;
		if (_indx >= MAX_INDEX)
		{
			_indx = 0;
		}
	}
	else
	{
		_bStartSendMsg = true;
	}
}