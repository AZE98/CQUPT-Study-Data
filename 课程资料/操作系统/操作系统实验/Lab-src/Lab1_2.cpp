// version��Ŀ
#include <windows.h>
#include <iostream>
using namespace std;
#pragma warning(disable: 4996)

// ���ý��̺Ͳ���ϵͳ�İ汾��Ϣ�ļ�ʾ��
int main() {
	// ��ȡ������̵� ID ��
	DWORD dwIdThis = ::GetCurrentProcessId();

	// �����һ���̺ͱ�������İ汾��Ҳ���Է���0�Ա�ָ����һ����
	DWORD dwVerReq = ::GetProcessVersion(dwIdThis);
	WORD wMajorReq = (WORD)(dwVerReq > 16);
	WORD wMinorReq = (WORD)(dwVerReq & 0xffff);
	cout << "Process ID: " << dwIdThis << ", requires OS: "
		<< wMajorReq << wMinorReq << endl;

	// ���ð汾��Ϣ�����ݽṹ���Ա㱣�����ϵͳ�İ汾��Ϣ
	OSVERSIONINFOEX osvix;
	::ZeroMemory(&osvix, sizeof(osvix));
	osvix.dwOSVersionInfoSize = sizeof(osvix);
	// ��ȡ�汾��Ϣ�ͱ���
	::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&osvix));
	cout << "Running on OS: " << osvix.dwMajorVersion << "."
		<< osvix.dwMinorVersion << endl;

	// ����� NTS (Windows  Server 2003) ϵͳ�������������Ȩ
	if (osvix.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		osvix.dwMajorVersion >= 5)
	{
		// �ı����ȼ�
		::SetPriorityClass(
			::GetCurrentProcess(), // ������һ����
			HIGH_PRIORITY_CLASS    // �ı�Ϊ high
		);

		// ������û�
		cout << "Task Manager should now now indicate this "
			"process is high priority��" << endl;
	}

	return 0;
}