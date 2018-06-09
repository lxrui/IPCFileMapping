#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "..\IPC_FileMapping\IPCFileMappingExport.h"

#if 1
int main()
{
	IPCFileMapping::createClient("Local\\WWJIPCFileMappingObject", 32 * sizeof(float));
	float B[32] = { 0.f };
	float uB = 0.f;
	while (!(_kbhit() && _getch() == 0x1b))
	{
		if (IPCFileMapping::recvCmd(B, 32 * sizeof(float)))
		{
			for (int i = 0; i < 32; ++i)
			{
				printf("%4.3f ", B[i]);
			}
			printf("\n");
		}
		Sleep(5);
		//Sleep(1000);
	}
	IPCFileMapping::destroyClient();
	return 0;
}
#endif

#if 0
//#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
char szName[] = "Local\\MyFileMappingObject";

int main()
{
	HANDLE hMapFile;
	unsigned char * pBuf;

	hMapFile = OpenFileMappingA(
		FILE_MAP_READ,   // read/write access
		FALSE,                 // do not inherit the name
		szName/*"Local\\WWJIPCFileMappingObject"*/);               // name of mapping object

	if (hMapFile == NULL)
	{
		printf("Could not open file mapping object (%d).\n",
			GetLastError());
		return 1;
	}

	pBuf = (unsigned char *)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_READ,  // read/write permission
		0,
		0,
		8194);

	if (pBuf == NULL)
	{
		printf("Could not map view of file (%d).\n",
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}
	unsigned char B[32] = { 0 };
	memcpy(B, pBuf, 32 * sizeof(unsigned char));
	memcpy(B, pBuf + 32, 32 * sizeof(unsigned char));

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

#endif
