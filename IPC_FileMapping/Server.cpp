#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "IPCFileMappingExport.h"

#if 1

int main()
{
	IPCFileMapping::createServer("Local\\WWJIPCFileMappingObject", 32 * sizeof(float));
	float B[32] = { 0.f };
	float uB = 0.f;
	while (!(_kbhit() && _getch() == 0x1b))
	{
		for (int i = 0; i < 32; ++i)
		{
			if (0==i)
			{
				B[i] = 0xaa;
			}
			else if (31 == i)
			{
				B[i] = 0xee;
			}
			else
			{
				uB += 0.618f;
				if (uB > 10000.f)
				{
					uB -= 10000.f;
				}
				B[i] = uB;
			}
			printf("%4.3f ", B[i]);
		}
		printf("\n");
		IPCFileMapping::sendCmd(B, 32 * sizeof(float));
		Sleep(15);
	}
	IPCFileMapping::destroyServer();
	return 0;
}
#endif



#if 0

#define BUF_SIZE 8194
#define MAX_INDEX0 4
#define MAX_INDEX1 64
#define CMD_SIZE 32
//char szName[] = "Local\\MyFileMappingObject";
//char szMsg[] = "Message from first process.";
char szName[] = "Local\\WWJIPCFileMappingObject";

int main()
{
	HANDLE hMapFile;
	unsigned char * pBuf;
	
	hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		printf("Could not create file mapping object (%d).\n",
			GetLastError());
		return 1;
	}
	pBuf = (unsigned char *)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		printf("Could not map view of file (%d).\n",
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}

	unsigned char B[32] = { 0 };
	unsigned char B1[32] = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		B[i] = 0x1b;
		B1[i] = 0x66;
	}

	memcpy((PVOID)pBuf, B, (32 * sizeof(unsigned char)));
	memcpy((PVOID)(pBuf + 32), B1, (32 * sizeof(unsigned char)));
	printf("B:%x B1:%x WRITED.\n", B[0], B1[0]);
	_getch();

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}
#endif