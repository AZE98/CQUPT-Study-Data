// ���� vmeminfo 
#include <windows.h>
#include <iostream>
using namespace std;
#include <shlwapi.h>
#include <iomanip>
#pragma comment(lib, "shlwapi.lib")

void main() {
	// ���ȣ������ǻ��ϵͳ��Ϣ     
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// ��ʽ��
	TCHAR szPageSize[MAX_PATH];
	::StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);
	TCHAR dwAllocationGranularity[MAX_PATH];
	::StrFormatByteSize(si.dwAllocationGranularity, dwAllocationGranularity, MAX_PATH);
	DWORD dwMemSize = (DWORD)si.lpMaximumApplicationAddress - (DWORD)si.lpMinimumApplicationAddress + 1;
	TCHAR szMemSize[MAX_PATH];
	::StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH);

	// ���ڴ���Ϣ��ʾ����
	cout << "Virtual memory page size: " << szPageSize << endl;
	cout << "�����ڴ�����: " << dwAllocationGranularity << endl;

	cout.fill('0');
	cout << "Minimum application address: 0x" << hex << setw(8)
		<< (DWORD)si.lpMinimumApplicationAddress << endl;
	cout << "Maximum application address: 0x" << hex << setw(8)
		<< (DWORD)si.lpMaximumApplicationAddress << endl;
	cout << "Total available virtual memory: " << szMemSize << endl;
}