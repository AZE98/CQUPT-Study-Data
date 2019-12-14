// event ��Ŀ
#include <windows.h>
#include <iostream>
using namespace std;

// �����Ǿ���¼���ʵ���кܿ���ʹ�ù���İ����ļ�������ͨѶ
static LPCTSTR g_szContinueEvent = "w2kdg.EventDemo.event.Continue";

// ������ֻ�Ǵ�����һ�����̵ĸ��������ӽ���ģʽ (��������ָ��) ���� 
BOOL CreateChild()
{
	// ��ȡ��ǰ��ִ���ļ����ļ���
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);

	// ��ʽ�������ӽ��̵������У�ָ������һ��EXE�ļ����ӽ���
	TCHAR szCmdLine[MAX_PATH];
	::sprintf_s(szCmdLine, "\"%s\" child", szFilename);
	// �ӽ��̵�������Ϣ�ṹ
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));
	si.cb = sizeof(si); // �����Ǳ��ṹ�Ĵ�С
	// ���ص��ӽ��̵Ľ�����Ϣ�ṹ
	PROCESS_INFORMATION pi;
	// ʹ��ͬһ��ִ���ļ��͸�������һ���ӽ��̵������д�������
	BOOL bCreateOK = ::CreateProcess(
		szFilename, // ���ɵĿ�ִ���ļ���
		szCmdLine,  // ָʾ����Ϊ���ӽ���һ���ı�־
		NULL, // �ӽ��̾���İ�ȫ��
		NULL, // ���߳̾���İ�ȫ��
		FALSE, // ���̳о��
		0,  // ����Ĵ�����־
		NULL, // �»���
		NULL, // ��ǰĿ¼
		&si,  // ������Ϣ�ṹ
		&pi); // ���صĽ�����Ϣ�ṹ

	// �ͷŶ��ӽ��̵�����
	if (bCreateOK) {
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	return (bCreateOK);
}

// ����ķ�������һ���¼���һ���ӽ��̣�Ȼ��ȴ��ӽ����ڷ���ǰ���¼������ź�
void WaitForChild()
{
	// create a new event object for the child process
	// to use when releasing this process
	HANDLE hEventContinue = ::CreateEvent(
		NULL,   // ȱʡ�İ�ȫ�ԣ��ӽ��̽����з���Ȩ��
		TRUE,   // �ֹ������¼�
		FALSE,  // ��ʼʱ�Ƿǽ����ź�״̬
		g_szContinueEvent); // �¼�����

	if (hEventContinue != NULL)
	{
		cout << "event created " << endl;
		// �����ӽ���
		if (::CreateChild())
		{
			cout << "child created" << endl;
			// �ȴ���ֱ���ӽ��̷����ź�
			cout << "Parent waiting on child." << endl;
			::WaitForSingleObject(hEventContinue, INFINITE);
			::Sleep(1500); // ɾȥ�������
			cout << "parent received the event signaling from child" << endl;
		}

		// ������
		::CloseHandle(hEventContinue);
		hEventContinue = INVALID_HANDLE_VALUE;
	}
}

// ���·������ӽ���ģʽ�±����ã��书��ֻ���򸸽��̷�����ֹ�ź�
void SignalParent()
{
	// ���Դ򿪾��
	cout << "child process begining......" << endl;
	HANDLE hEventContinue = ::OpenEvent(
		EVENT_MODIFY_STATE,  // ��Ҫ�����С����Ȩ��
		FALSE,  // ���ǿɼ̳еľ��
		g_szContinueEvent);  // �¼�����

	if (hEventContinue != NULL)
	{
		::SetEvent(hEventContinue);
		cout << "event signaled" << endl;
	}

	// ������
	::CloseHandle(hEventContinue);
	hEventContinue = INVALID_HANDLE_VALUE;
}

int main(int argc, char* argv[])
{
	// ��鸸���̻����ӽ����Ƿ�����
	if (argc > 1 && ::strcmp(argv[1], "child") == 0)
	{
		// �򸸽��̴������¼������ź�
		::SignalParent();
	}else{
		// ����һ���¼����ȴ��ӽ��̷����ź�
		::WaitForChild();
		::Sleep(1500);
		cout << "Parent released." << endl;
	}
	return 0;
}