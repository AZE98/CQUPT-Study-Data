// �����̵������ַ�ռ�
// ���� vmwalker
#include <windows.h>
#include <iostream>
using namespace std;
#include <shlwapi.h>
#include <iomanip>
#pragma comment(lib, "Shlwapi.lib")

// �Կɶ���ʽ���û���ʾ�����ĸ���������
// ������Ǳ�ʾ����Ӧ�ó�����ڴ���з��ʵ�����
// �Լ�����ϵͳǿ�Ʒ��ʵ�����
inline bool TestSet(DWORD dwTarget, DWORD dwMask) {
	return ((dwTarget & dwMask) == dwMask);
}

# define SHOWMASK(dwTarget, type) \
if (TestSet(dwTarget, PAGE_##type) ) \
    {cout << ", " << #type;}

void ShowProtection(DWORD dwTarget)
{
	SHOWMASK(dwTarget, READONLY);
	SHOWMASK(dwTarget, GUARD);
	SHOWMASK(dwTarget, NOCACHE);
	SHOWMASK(dwTarget, READWRITE);
	SHOWMASK(dwTarget, WRITECOPY);
	SHOWMASK(dwTarget, EXECUTE);
	SHOWMASK(dwTarget, EXECUTE_READ);
	SHOWMASK(dwTarget, EXECUTE_READWRITE);
	SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
	SHOWMASK(dwTarget, NOACCESS);
}

// �������������ڴ沢���û���ʾ�����ԵĹ�������ķ���
void WalkVM(HANDLE hProcess) {
	// ���ȣ����ϵͳ��Ϣ
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// ����Ҫ�����Ϣ�Ļ�����     
	MEMORY_BASIC_INFORMATION mbi;
	::ZeroMemory(&mbi, sizeof(mbi));

	// ѭ������Ӧ�ó����ַ�ռ�
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		// �����һ�������ڴ�����Ϣ       
		if (::VirtualQueryEx(
			hProcess,  // ��صĽ���
			pBlock,  // ��ʼλ��
			&mbi,  // ������
			sizeof(mbi)) == sizeof(mbi))  // ��С��ȷ��
		{
			// �����Ľ�β�����С
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			::StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);

			// ��ʾ���ַ�ʹ�С
			cout.fill('0');
			cout << hex << setw(8) << (DWORD)pBlock
				<< "-" << hex << setw(8) << (DWORD)pEnd
				//<< " (" << szSize                 
				<< (::strlen(szSize) == 7 ? " (" : " ( ") << szSize
				<< ") ";

			// ��ʾ���״̬
			switch (mbi.State) {
			case MEM_COMMIT:
				cout << "Committed";
				break;
			case MEM_FREE:
				cout << "Free";
				break;
			case MEM_RESERVE:
				cout << "Reserved";
				break;
			}

			// ��ʾ����
			if (mbi.Protect == 0 && mbi.State != MEM_FREE)
			{
				mbi.Protect = PAGE_READONLY;
			}
			ShowProtection(mbi.Protect);

			// ��ʾ����
			switch (mbi.Type) {
			case MEM_IMAGE:
				cout << ", Image";
				break;
			case MEM_MAPPED:
				cout << ", Mapped";
				break;
			case MEM_PRIVATE:
				cout << ", Private";
				break;
			}

			// �����ִ�е�Ӱ��             
			TCHAR szFilename[MAX_PATH];
			if (::GetModuleFileName(
				(HMODULE)pBlock,  // ʵ�������ڴ��ģ����
				szFilename,  // ��ȫָ�����ļ�����
				MAX_PATH) > 0)  // ʵ��ʹ�õĻ�������С
			{
				// ��ȥ·������ʾ
				::PathStripPath(szFilename);
				cout << ", Module: " << szFilename;
			}
			cout << endl;// �ƶ���ָ���Ի����һ�¸���
			pBlock = pEnd;
		}
	}
}

void main() {
	// ������ǰ���̵������ڴ�
	::WalkVM(::GetCurrentProcess());
}
