// mutex ��Ŀ
#include <windows.h>
#include <iostream>
using namespace std;

// ���û�����������ͬʱ���ʵĹ�����Դ
class CCountUpDown
{
public:
	// �����ߴ��������߳������ʹ���ֵ
	CCountUpDown(int nAccesses) : m_hThreadInc(INVALID_HANDLE_VALUE), m_hThreadDec(INVALID_HANDLE_VALUE),
		m_hMutexValue(INVALID_HANDLE_VALUE), m_nValue(0), m_nAccess(nAccesses)
	{
		// �������������ڷ�����ֵ
		m_hMutexValue = ::CreateMutex(
			NULL,  // ȱʡ�İ�ȫ��
			TRUE,  // ��ʼʱӵ�У������еĳ�ʼ������ʱ���ͷ�
			NULL); // ������

		m_hThreadInc = ::CreateThread(
			NULL,                           // ȱʡ�İ�ȫ��
			0,                              // ȱʡ��ջ
			IncThreadProc,                  // ���߳̽���
			reinterpret_cast<LPVOID>(this), // �̲߳���
			0,                              // ������ı�־
			NULL);                          // ���Է��ص� id

		m_hThreadDec = ::CreateThread(
			NULL,                           // ȱʡ�İ�ȫ��
			0,                              // ȱʡ��ջ
			DecThreadProc,                  // ���߳̽���
			reinterpret_cast<LPVOID>(this), // �̲߳���
			0,                              // ������ı�־
			NULL);                          // ���Է��ص� id

		// ������һ�̻߳�û�����
		::ReleaseMutex(m_hMutexValue);
	}

	// ��������ͷŶԶ�������� 
	virtual ~CCountUpDown()
	{
		::CloseHandle(m_hThreadInc);
		::CloseHandle(m_hThreadDec);
		::CloseHandle(m_hMutexValue);
	}

	// �򵥵ĵȴ��������������߳���ֹ֮ǰ����ͣ������
	virtual void WaitForCompletion()
	{
		// ȷ�����ж�����׼����
		if (m_hThreadInc != INVALID_HANDLE_VALUE &&
			m_hThreadDec != INVALID_HANDLE_VALUE)
		{
			// �ȴ�������� (˳�򲢲���Ҫ)
			::WaitForSingleObject(m_hThreadInc, INFINITE);
			::WaitForSingleObject(m_hThreadDec, INFINITE);
		}
	}

protected:

	// �ı乲����Դ�ļ򵥵ķ���
	virtual void DoCount(int nStep)
	{
		// ѭ����ֱ�����еķ��ʶ�����Ϊֹ
		while (m_nAccess > 0)
		{
			// �ȴ�������ֵ
			::WaitForSingleObject(m_hMutexValue, INFINITE);
			// �ı䲢��ʾ��ֵ
			m_nValue += nStep;
			cout << "thread: " << ::GetCurrentThreadId()
				<< " value: " << m_nValue
				<< " access: " << m_nAccess << endl;
			// ���������źŲ������߳��л�
			--m_nAccess;
			::Sleep(1000); // ʹ��ʾ�ٶȷ���
			// �ͷŶ���ֵ�ķ���
			::ReleaseMutex(m_hMutexValue);
		}
	}

	static DWORD WINAPI IncThreadProc(LPVOID lpParam)
	{
		// ����������Ϊ 'this' ָ��
		CCountUpDown* pThis =
			reinterpret_cast<CCountUpDown*>(lpParam);
		// ���ö�������ӷ���������һ��ֵ
		pThis->DoCount(+1);
		return (0);
	}

	static DWORD WINAPI DecThreadProc(LPVOID lpParam)
	{
		// ����������Ϊ 'this' ָ��
		CCountUpDown* pThis =
			reinterpret_cast<CCountUpDown*>(lpParam);
		// ���ö���ļ��ٷ���������һ��ֵ
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