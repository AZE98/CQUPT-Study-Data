#include <Windows.h>
#include <iostream>
using namespace std;
// Defines the entry point for the console application.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void GetMemSta(void);
//The one and only application object CWinApp theApp;

int main()
{
	int nRetCode = 0;
	LPVOID BaseAddr;
	char* str;

	GetMemSta();

	printf("Now Allocate 32M Virsual Memory and 2M Physical Memory\n\n");

	BaseAddr = ::VirtualAlloc(NULL, 1024 * 1024 * 32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);  //���������ڴ�
	if (BaseAddr == NULL)
		printf("Virsual Allocate Fail.\n");
	str = (char*)malloc(1024 * 1024 * 2);  //�����ڴ�

	GetMemSta();

	printf("Now Release 32M Virsual Memory and 2M Physical Memory\n\n");
	if (::VirtualFree(BaseAddr, 0, MEM_RELEASE) == 0)  //�ͷ������ڴ�
		printf("Release Allocate Fail.\n");
	free(str);  //�ͷ��ڴ�

	GetMemSta();

	return nRetCode;
}

void GetMemSta(void)
{
	MEMORYSTATUS MemInfo;
	GlobalMemoryStatus(&MemInfo);

	printf("Current Memory Status is :\n");
	printf("\t Total Physical Memory is %d MB\n", MemInfo.dwTotalPhys / (1024 * 1024));
	printf("\t Available Physical Memory is %d MB\n", MemInfo.dwAvailPhys / (1024 * 1024));
	printf("\t Total Page File is %d MB\n", MemInfo.dwTotalPageFile / (1024 * 1024));
	printf("\t Available Page File is %d MB\n", MemInfo.dwAvailPageFile / (1024 * 1024));
	printf("\t Total Virtual Memory is %d MB\n", MemInfo.dwTotalVirtual / (1024 * 1024));
	printf("\t Available Virsual memory is %d MB\n", MemInfo.dwAvailVirtual / (1024 * 1024));
	printf("\t Memory Load is %d %%\n\n", MemInfo.dwMemoryLoad);
}