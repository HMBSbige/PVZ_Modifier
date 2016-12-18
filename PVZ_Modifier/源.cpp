#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "MemoryModify.h"
using namespace std;
////////////////////////
void print() {
	printf(R"(------------------------------------------------------------------------------------------
欢迎使用PVZ：GOTY修改器（只适用于steam版本）
输入:1.修改阳光数	2.修改智慧树高度	3.清屏	0.退出
------------------------------------------------------------------------------------------
)");
	return;
}
int main()
{
	DWORD value;
	int i;
	MemoryModify PVZ;
	string ProcessName = "popcapgame1.exe";
	vector<DWORD> ads_Sun = { 0x00731C50,0x868,0x5578 };
	vector<DWORD> ads_Tree = { 0x00731C50,0x94C,0x120 };
	print();
	while (cin >> i) {
		switch (i) {
		case 1:
			if (!PVZ.init(ProcessName, ads_Sun)) {
				goto END1;
			}
			cout << "进程名:" << ProcessName << "\nPID:" << PVZ.getPID();
			cout << "\n请输入阳光数(0~2147483647):";
			if (cin >> value) {
				if (PVZ.Modify(value))
					cout << "修改成功！" << endl;
				else
					cout << "修改失败！" << endl;
			}else{ 
				cout << "阳光数输入错误！" << endl;
				goto END1; 
			}
			END1:
			break;
		case 2:
			if (!PVZ.init(ProcessName, ads_Tree)) {
				goto END2;
			}
			cout << "进程名:" << ProcessName << "\nPID:" << PVZ.getPID();
			cout << "\n请输入智慧树高度(0~2147483647):";
			if (cin >> value) {
				if (PVZ.Modify(value))
					cout << "修改成功！" << endl;
				else
					cout << "修改失败！" << endl;
			}
			else {
				cout << "智慧树高度数错误！" << endl;
				goto END2;
			}
		END2:
			break;
		case 3:
			system("cls");
			break;
		case 0:
			goto END;
		default:
			cout << "输入错误！"<< endl;
			break;
		}
		print();
	}
END:
	cout << endl;
	system("pause");
	return 0;
}