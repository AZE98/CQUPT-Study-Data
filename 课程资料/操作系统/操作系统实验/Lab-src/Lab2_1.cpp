// event 项目
#include <windows.h>
#include <iostream>
using namespace std;

// 以下是句柄事件。实际中很可能使用共享的包含文件来进行通讯
static LPCTSTR g_szContinueEvent = "w2kdg.EventDemo.event.Continue";

// 本方法只是创建了一个进程的副本，以子进程模式 (由命令行指定) 工作 
BOOL CreateChild()
{
	// 提取当前可执行文件的文件名
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);

	// 格式化用于子进程的命令行，指明它是一个EXE文件和子进程
	TCHAR szCmdLine[MAX_PATH];
	::sprintf_s(szCmdLine, "\"%s\" child", szFilename);
	// 子进程的启动信息结构
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));
	si.cb = sizeof(si); // 必须是本结构的大小
	// 返回的子进程的进程信息结构
	PROCESS_INFORMATION pi;
	// 使用同一可执行文件和告诉它是一个子进程的命令行创建进程
	BOOL bCreateOK = ::CreateProcess(
		szFilename, // 生成的可执行文件名
		szCmdLine,  // 指示其行为与子进程一样的标志
		NULL, // 子进程句柄的安全性
		NULL, // 子线程句柄的安全性
		FALSE, // 不继承句柄
		0,  // 特殊的创建标志
		NULL, // 新环境
		NULL, // 当前目录
		&si,  // 启动信息结构
		&pi); // 返回的进程信息结构

	// 释放对子进程的引用
	if (bCreateOK) {
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	return (bCreateOK);
}

// 下面的方法创建一个事件和一个子进程，然后等待子进程在返回前向事件发出信号
void WaitForChild()
{
	// create a new event object for the child process
	// to use when releasing this process
	HANDLE hEventContinue = ::CreateEvent(
		NULL,   // 缺省的安全性，子进程将具有访问权限
		TRUE,   // 手工重置事件
		FALSE,  // 初始时是非接受信号状态
		g_szContinueEvent); // 事件名称

	if (hEventContinue != NULL)
	{
		cout << "event created " << endl;
		// 创建子进程
		if (::CreateChild())
		{
			cout << "child created" << endl;
			// 等待，直到子进程发出信号
			cout << "Parent waiting on child." << endl;
			::WaitForSingleObject(hEventContinue, INFINITE);
			::Sleep(1500); // 删去这句试试
			cout << "parent received the event signaling from child" << endl;
		}

		// 清除句柄
		::CloseHandle(hEventContinue);
		hEventContinue = INVALID_HANDLE_VALUE;
	}
}

// 以下方法在子进程模式下被调用，其功能只是向父进程发出终止信号
void SignalParent()
{
	// 尝试打开句柄
	cout << "child process begining......" << endl;
	HANDLE hEventContinue = ::OpenEvent(
		EVENT_MODIFY_STATE,  // 所要求的最小访问权限
		FALSE,  // 不是可继承的句柄
		g_szContinueEvent);  // 事件名称

	if (hEventContinue != NULL)
	{
		::SetEvent(hEventContinue);
		cout << "event signaled" << endl;
	}

	// 清除句柄
	::CloseHandle(hEventContinue);
	hEventContinue = INVALID_HANDLE_VALUE;
}

int main(int argc, char* argv[])
{
	// 检查父进程或是子进程是否启动
	if (argc > 1 && ::strcmp(argv[1], "child") == 0)
	{
		// 向父进程创建的事件发出信号
		::SignalParent();
	}else{
		// 创建一个事件并等待子进程发出信号
		::WaitForChild();
		::Sleep(1500);
		cout << "Parent released." << endl;
	}
	return 0;
}