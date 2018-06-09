#include "IPCFileMappingClient.h"

CIPCFileMappingClient::CIPCFileMappingClient(const char * mappingFileName, size_t cmdSize)
{
	_hMapFile = nullptr;
	_pBuf = nullptr;
	//µÍÎ»
	_clientIdx0 = 0;
	//¸ßÎ»
	_clientIdx1 = 0;
	_serverIndex0 = 0;
	_serverIndex1 = 0;
	_bInitialized = false;
	_bIsUpToDate = false;
	_sMappingFileName = mappingFileName;
	_cmdSize = cmdSize;
	_bufSize = _cmdSize * MAX_INDEX0 * MAX_INDEX1 + 2;
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
	_clientIdx0 = m_getServerIdx0();
	_clientIdx1 = m_getServerIdx1();
	_serverIndex0 = _clientIdx0;
	_serverIndex1 = _clientIdx1;
	return true;
}

CIPCFileMappingClient::~CIPCFileMappingClient()
{
	UnmapViewOfFile(_pBuf);
	_pBuf = nullptr;
	CloseHandle(_hMapFile);
	_hMapFile = nullptr;
}

bool CIPCFileMappingClient::m_updateCmds(const size_t nCmdSize, void * B)
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
			_serverIndex1 = m_getServerIdx1();
			_serverIndex0 = m_getServerIdx0();
		}

		if (_clientIdx1 * MAX_INDEX1 + _clientIdx0 == _serverIndex1 * MAX_INDEX1 + _serverIndex0)
		{
			//printf("the cmd is up to date.\n");
			_bIsUpToDate = true;
			return false;
		}
		else/* if (nClientPos < nServerPos)*/
		{
			++_clientIdx0;
			if (_clientIdx1 * MAX_INDEX1 + _clientIdx0 == MAX_INDEX0 * MAX_INDEX1)
			{
				_clientIdx0 = 0;
				_clientIdx1 = 0;
			}
			if (nullptr != _pBuf)
			{
				memcpy(B, _pBuf + 2 * sizeof(unsigned char) + (_clientIdx1 * MAX_INDEX1 + _clientIdx0) * _cmdSize, nCmdSize);
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

unsigned char CIPCFileMappingClient::m_getServerIdx0()
{
	if (_pBuf == nullptr)
	{
		return 0;
	}
	return _pBuf[0];
}
unsigned char CIPCFileMappingClient::m_getServerIdx1()
{
	if (_pBuf == nullptr)
	{
		return 0;
	}
	return _pBuf[1];
}