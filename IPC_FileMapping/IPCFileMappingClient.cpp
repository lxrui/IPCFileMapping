#include "IPCFileMappingClient.h"

CIPCFileMappingClient::CIPCFileMappingClient(const char * mappingFileName, size_t cmdSize)
{
	_hMapFile = nullptr;
	_pBuf = nullptr;
	_clientIndx = 0;
	_serverIndx = 0;
	_bInitialized = false;
	_bIsUpToDate = false;
	_sMappingFileName = mappingFileName;
	_cmdSize = cmdSize;
	_bufSize = _cmdSize * MAX_INDEX + sizeof(DWORD);
	_bInitialized = _initClient();
}

bool CIPCFileMappingClient::_initClient()
{
	_hMapFile = OpenFileMappingA(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		_sMappingFileName.c_str());               // name of mapping object

	if (_hMapFile == nullptr)
	{
		//printf("Could not open file mapping object (%d).\n", GetLastError());
		return false;
	}
	_pBuf = (unsigned char *)MapViewOfFile(_hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		_bufSize);

	if (_pBuf == nullptr)
	{
		//printf("Could not map view of file (%d).\n", GetLastError());
		return false;
	}
	_clientIndx = m_getIndx();
	_serverIndx = _clientIndx;
	return true;
}

CIPCFileMappingClient::~CIPCFileMappingClient()
{
	UnmapViewOfFile(_pBuf);
	_pBuf = nullptr;
	CloseHandle(_hMapFile);
	_hMapFile = nullptr;
}

bool CIPCFileMappingClient::m_updateCmds(const size_t nCmdSize, void * output)
{
	if (_bInitialized)
	{
		if (_cmdSize != nCmdSize)
		{
			//printf("command size wrong.\n");
			return false;
		}
		//size_t nClientPos = _clientIdx1 * MAX_INDEX1 + _clientIdx0;
		//size_t nServerPos = m_getServerIdx1() * MAX_INDEX1 + m_getServerIdx0();
		if (_bIsUpToDate)
		{
			_serverIndx = m_getIndx();
		}

		if (_clientIndx == _serverIndx)
		{
			//printf("the cmd is up to date.\n");
			_bIsUpToDate = true;
			return false;
		}
		else/* if (nClientPos < nServerPos)*/
		{
			++_clientIndx;
			if (_clientIndx == MAX_INDEX)
			{
				_clientIndx = 0;
			}
			if (nullptr != _pBuf)
			{
				memcpy(output, _pBuf + sizeof(DWORD) + (_clientIndx) * _cmdSize, nCmdSize);
				_bIsUpToDate = false;
			}
			return true;
		}
	}
	else
	{
		_bInitialized = _initClient();
		return false;
	}
	Sleep(3);
	return true;
}

DWORD CIPCFileMappingClient::m_getIndx()
{
	if (_pBuf == nullptr)
	{
		return 0;
	}
	DWORD * p = (DWORD *)_pBuf;
	return *p;
}