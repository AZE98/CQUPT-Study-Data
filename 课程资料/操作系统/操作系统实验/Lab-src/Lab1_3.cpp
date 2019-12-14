// procterm��Ŀ
#include <windows.h>
#include <iostream>
#include <stdio.h>
static LPCTSTR g_szMutexName = "w2kdg.ProcTerm.mutex.Suicide";
using namespace std;

// ������ǰ���̵Ŀ�¡���̵ļ򵥷���
void StartClone() {
	// ��ȡ��ǰ��ִ���ļ����ļ���
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);

	// ��ʽ�������ӽ��̵������У�ָ������һ��EXE�ļ����ӽ���
	TCHAR szCmdLine[MAX_PATH];
	::sprintf_s(szCmdLine, "\"%s\" child", szFilename);

	// �ӽ��̵�������Ϣ�ṹ
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));
	si.cb = sizeof(si); // Ӧ���Ǵ˽ṹ�Ĵ�С

	// ���ص������ӽ��̵Ľ�����Ϣ
	PROCESS_INFORMATION pi;

	// ��ͬ���Ŀ�ִ���ļ����������д������̣���ָ������һ���ӽ���
	BOOL bCreateOK = ::CreateProcess(
		szFilename,         // ������Ӧ�ó������� (�� EXE �ļ�)
		szCmdLine,          // ������������һ���ӽ��̵ı�־
		NULL,               // ���ڽ��̵�ȱʡ�İ�ȫ��
		NULL,               // �����̵߳�ȱʡ��ȫ��
		FALSE,              // ���̳о��
		CREATE_NEW_CONSOLE, // �����´��ڣ�ʹ�����ֱ��
		NULL,               // �»���
		NULL,               // ��ǰĿ¼
		&si,                // ������Ϣ�ṹ
		&pi);               // ���صĽ�����Ϣ

	// �ͷ�ָ���ӽ��̵�����
	if (bCreateOK)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
}

void Parent()
{
	// ����"��ɱ"���������
	HANDLE hMutexSuicide = ::CreateMutex(
		NULL, // ȱʡ�İ�ȫ��
		TRUE, // ���ӵ�е�
		g_szMutexName); // Ϊ������

	if (hMutexSuicide != NULL)
	{
		// �����ӽ���
		cout << "Creating the child process." << endl;
		::StartClone();
		// ��ͣ
		::Sleep(5000);
		// ָ���ӽ���"ɱ"������
		cout << "Telling the child process to quit. " << endl;
		::ReleaseMutex(hMutexSuicide);
		// �������
		::CloseHandle(hMutexSuicide);
	}
}

void Child()
{
	// ��"��ɱ"������
	HANDLE hMutexSuicide = ::OpenMutex(
		SYNCHRONIZE, // ������ͬ��
		FALSE, // ����Ҫ���´���
		g_szMutexName); // ����
	if (hMutexSuicide != NULL)
	{
		// �������ڵȴ�ָ��
		cout << "Child waiting for suicide instructions. " << endl;
		::WaitForSingleObject(hMutexSuicide, INFINITE);
		// ׼������ֹ��������
		cout << "Child quiting. " << endl;
		::CloseHandle(hMutexSuicide);
		// �ȴ���ʾ��Ϣ
		::Sleep(5000);
	}
}

int main(int argc, char* argv[])
{
	// ��������Ϊ�Ǹ����̻����ӽ���
	if (argc > 1 && ::strcmp(argv[1], "child") == 0) {
		Child();
	}
	else {
		Parent();
	}

	return 0;
}