// mutex 项目
#include <windows.h>
#include <iostream>
using namespace std;

// 利用互斥体来保护同时访问的共享资源
class CCountUpDown
{
public:
	// 创建者创建两个线程来访问共享值
	CCountUpDown(int nAccesses) : m_hThreadInc(INVALID_HANDLE_VALUE), m_hThreadDec(INVALID_HANDLE_VALUE),
		m_hMutexValue(INVALID_HANDLE_VALUE), m_nValue(0), m_nAccess(nAccesses)
	{
		// 创建互斥体用于访问数值
		m_hMutexValue = ::CreateMutex(
			NULL,  // 缺省的安全性
			TRUE,  // 初始时拥有，在所有的初始化结束时将释放
			NULL); // 匿名的

		m_hThreadInc = ::CreateThread(
			NULL,                           // 缺省的安全性
			0,                              // 缺省堆栈
			IncThreadProc,                  // 类线程进程
			reinterpret_cast<LPVOID>(this), // 线程参数
			0,                              // 无特殊的标志
			NULL);                          // 忽略返回的 id

		m_hThreadDec = ::CreateThread(
			NULL,                           // 缺省的安全性
			0,                              // 缺省堆栈
			DecThreadProc,                  // 类线程进程
			reinterpret_cast<LPVOID>(this), // 线程参数
			0,                              // 无特殊的标志
			NULL);                          // 忽略返回的 id

		// 允许另一线程获得互斥体
		::ReleaseMutex(m_hMutexValue);
	}

	// 解除程序释放对对象的引用 
	virtual ~CCountUpDown()
	{
		::CloseHandle(m_hThreadInc);
		::CloseHandle(m_hThreadDec);
		::CloseHandle(m_hMutexValue);
	}

	// 简单的等待方法，在两个线程终止之前可暂停主调者
	virtual void WaitForCompletion()
	{
		// 确保所有对象都已准备好
		if (m_hThreadInc != INVALID_HANDLE_VALUE &&
			m_hThreadDec != INVALID_HANDLE_VALUE)
		{
			// 等待两者完成 (顺序并不重要)
			::WaitForSingleObject(m_hThreadInc, INFINITE);
			::WaitForSingleObject(m_hThreadDec, INFINITE);
		}
	}

protected:

	// 改变共享资源的简单的方法
	virtual void DoCount(int nStep)
	{
		// 循环，直到所有的访问都结束为止
		while (m_nAccess > 0)
		{
			// 等待访问数值
			::WaitForSingleObject(m_hMutexValue, INFINITE);
			// 改变并显示该值
			m_nValue += nStep;
			cout << "thread: " << ::GetCurrentThreadId()
				<< " value: " << m_nValue
				<< " access: " << m_nAccess << endl;
			// 发出访问信号并允许线程切换
			--m_nAccess;
			::Sleep(1000); // 使显示速度放慢
			// 释放对数值的访问
			::ReleaseMutex(m_hMutexValue);
		}
	}

	static DWORD WINAPI IncThreadProc(LPVOID lpParam)
	{
		// 将参数解释为 'this' 指针
		CCountUpDown* pThis =
			reinterpret_cast<CCountUpDown*>(lpParam);
		// 调用对象的增加方法并返回一个值
		pThis->DoCount(+1);
		return (0);
	}

	static DWORD WINAPI DecThreadProc(LPVOID lpParam)
	{
		// 将参数解释为 'this' 指针
		CCountUpDown* pThis =
			reinterpret_cast<CCountUpDown*>(lpParam);
		// 调用对象的减少方法并返回一个值
		pThis->DoCount(-1);
		return (0);
	}

protected:
	HANDLE m_hThreadInc;
	HANDLE m_hThreadDec;
	HANDLE m_hMutexValue;
	int m_nValue;
	int m_nAccess;
};

int main()
{
	CCountUpDown ud(50);
	ud.WaitForCompletion();
	return 0;
}