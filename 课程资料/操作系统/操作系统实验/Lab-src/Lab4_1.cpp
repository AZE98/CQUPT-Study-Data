#include <windows.h>
#include <iostream>
using namespace std;
#include <winioctl.h>
#include <string.h>

struct Disk //���� Disk �ṹ�Ķ���
{
	HANDLE handle;
	DISK_GEOMETRY disk_info;
};

Disk disk;
HANDLE Floppy;
static _int64 sector;
bool flag;
Disk physicDisk(char driverLetter);

void main(void)
{
	char DriverLetter;
	cout << "��������̺ţ�a/c" << endl;
	cin >> DriverLetter;//ѡ��Ҫ�鿴�Ĵ���
	disk = physicDisk(DriverLetter);
}

Disk physicDisk(char driverLetter) //
{
	flag = true;
	DISK_GEOMETRY* temp = new DISK_GEOMETRY;
	char device[9] = "\\\\.\\c:";
	device[4] = driverLetter;
	Floppy = CreateFile(device,                             //��Ҫ�򿪵���������
		GENERIC_READ,                       //��ȡ��Ȩ��
		FILE_SHARE_READ | FILE_SHARE_WRITE, // �����Ȩ��
		NULL,                               //Ĭ������λ
		OPEN_EXISTING,                      //�����������ķ�ʽ
		0,                                  //��������������������
		NULL);                           //ָ��ģ���ļ��ľ��
	if (GetLastError() == ERROR_ALREADY_EXISTS)    //���ʧ�ܣ����ش������
	{
		cout << "���ܴ򿪴���" << endl;
		cout << GetLastError() << endl;
		flag = false;
		return disk;
	}

	DWORD bytereturned;
	BOOL Result;
	disk.handle = Floppy;
	Result = DeviceIoControl(Floppy,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		temp,
		sizeof(*temp),
		&bytereturned,
		(LPOVERLAPPED)NULL);
	if (!Result) //���ʧ�ܣ����ش������
	{
		cout << "��ʧ��" << endl;
		cout << "�������Ϊ��" << GetLastError() << endl;
		flag = false;
		return disk;
	}

	disk.disk_info = *temp;//�������������̵���Ϣ
	cout << driverLetter << "����: " << endl;
	cout << "������Ϊ��" << (unsigned long)disk.disk_info.Cylinders.QuadPart << endl;
	cout << "ÿ����Ĵŵ���Ϊ��" << disk.disk_info.TracksPerCylinder << endl;
	cout << "ÿ�ŵ���������Ϊ��" << disk.disk_info.SectorsPerTrack << endl;
	cout << "ÿ�������ֽ���Ϊ��" << disk.disk_info.BytesPerSector << endl;
	sector = disk.disk_info.Cylinders.QuadPart * (disk.disk_info.TracksPerCylinder) * (disk.disk_info.SectorsPerTrack);
	double DiskSize = (double)disk.disk_info.Cylinders.QuadPart * (disk.disk_info.TracksPerCylinder) * (disk.disk_info.SectorsPerTrack) * (disk.disk_info.BytesPerSector);
	cout << driverLetter << "�����ڴ����ܹ���" << (long)sector << "������" << endl;
	cout << "���̴�Ϊ:" << DiskSize / (1024 * 1024) << "MB " << endl;
	delete temp;
	return disk;
}