// proccreate��Ŀ
#include <windows.h>
#include <iostream>
#include <stdio.h>
using namespace std;

// �������ݹ����Ľ��̵Ŀ�¡���̲�������IDֵ
void StartClone(int nCloneID) {
	// ��ȡ���ڵ�ǰ��ִ���ļ����ļ���
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	// ��ʽ�������ӽ��̵������в�֪ͨ��EXE�ļ����Ϳ�¡ID
	TCHAR szCmdLine[MAX_PATH];
	::sprintf_s(szCmdLine, "\"%s\" %d", szFilename, nCloneID);
	// �����ӽ��̵�STARTUPINFO�ṹ
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));
	si.cb = sizeof(si);     // �����Ǳ��ṹ�Ĵ�С

	// ���ص������ӽ��̵Ľ�����Ϣ
	PROCESS_INFORMATION pi;

	// ����ͬ���Ŀ�ִ���ļ��������д������̣����������ӽ��̵�����
	BOOL bCreateOK = ::CreateProcess(
		szFilename,         // ������� EXE ��Ӧ�ó��������
		szCmdLine,          // ��������Ϊ��һ���ӽ��̵ı�־
		NULL,               // ȱʡ�Ľ��̰�ȫ��
		NULL,               // ȱʡ���̰߳�ȫ��
		FALSE,              // ���̳о��
		CREATE_NEW_CONSOLE, // ʹ���µĿ���̨
		NULL,               // �µĻ���
		NULL,               // ��ǰĿ¼
		&si,                // ������Ϣ
		&pi);               // ���صĽ�����Ϣ

	// ���ӽ����ͷ�����
	if (bCreateOK)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
}

int main(int argc, char* argv[]) {

	// ȷ���������б��е�λ��
	int nClone(0);
	if (argc > 1)
	{
		// �ӵڶ�����������ȡ��¡ID
		::sscanf_s(argv[1], "%d", &nClone);
	}

	// ��ʾ����λ��
	cout << "Process ID: " << ::GetCurrentProcessId()
		<< ", Clone ID: " << nClone
		<< endl;

	// ����Ƿ��д����ӽ��̵���Ҫ
	const int C_nCloneMax = 25;
	if (nClone < C_nCloneMax)
	{
		// �����½��̵������кͿ�¡��
		StartClone(++nClone);
	}
	// ����ֹ֮ǰ��ͣһ�� (l/2 ��)
	::Sleep(500);

	return 0;
}