#include <windows.h>
#include <iostream>
using namespace std;
#include <winioctl.h>
#include <string.h>

struct Disk //关于 Disk 结构的定义
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
	cout << "请输入磁盘号：a/c" << endl;
	cin >> DriverLetter;//选择要查看的磁盘
	disk = physicDisk(DriverLetter);
}

Disk physicDisk(char driverLetter) //
{
	flag = true;
	DISK_GEOMETRY* temp = new DISK_GEOMETRY;
	char device[9] = "\\\\.\\c:";
	device[4] = driverLetter;
	Floppy = CreateFile(device,                             //将要打开的驱动器名
		GENERIC_READ,                       //存取的权限
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享的权限
		NULL,                               //默认属性位
		OPEN_EXISTING,                      //创建驱动器的方式
		0,                                  //所创建的驱动器的属性
		NULL);                           //指向模板文件的句柄
	if (GetLastError() == ERROR_ALREADY_EXISTS)    //如打开失败，返回错误代码
	{
		cout << "不能打开磁盘" << endl;
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
	if (!Result) //如果失败，返回错误代码
	{
		cout << "打开失败" << endl;
		cout << "错误代码为：" << GetLastError() << endl;
		flag = false;
		return disk;
	}

	disk.disk_info = *temp;//输出整个物理磁盘的信息
	cout << driverLetter << "盘有: " << endl;
	cout << "柱面数为：" << (unsigned long)disk.disk_info.Cylinders.QuadPart << endl;
	cout << "每柱面的磁道数为：" << disk.disk_info.TracksPerCylinder << endl;
	cout << "每磁道的扇区数为：" << disk.disk_info.SectorsPerTrack << endl;
	cout << "每扇区的字节数为：" << disk.disk_info.BytesPerSector << endl;
	sector = disk.disk_info.Cylinders.QuadPart * (disk.disk_info.TracksPerCylinder) * (disk.disk_info.SectorsPerTrack);
	double DiskSize = (double)disk.disk_info.Cylinders.QuadPart * (disk.disk_info.TracksPerCylinder) * (disk.disk_info.SectorsPerTrack) * (disk.disk_info.BytesPerSector);
	cout << driverLetter << "盘所在磁盘总共有" << (long)sector << "个扇区" << endl;
	cout << "磁盘大为:" << DiskSize / (1024 * 1024) << "MB " << endl;
	delete temp;
	return disk;
}