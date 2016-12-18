#include "MemoryModify.h"

BOOL _stdcall MemoryModify::IBRP(DWORD add) {
	BOOL _stdcall code;
	code = IsBadReadPtr((void *)add, 4);
	if (code)
		cout << "无权限访问该内存" << endl;
	return code;
}
DWORD MemoryModify::GLE() {
	DWORD code = GetLastError();
	cout << "System Error Codes:" << code << endl;//https://msdn.microsoft.com/en-us/library/ms681381(v=vs.85).aspx
	return code;
}
DWORD MemoryModify::getpid(string name)
{
	HANDLE hprocessSnap = NULL;
	PROCESSENTRY32 pe32 = { 0 };
	hprocessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	wchar_t* temp = NULL;

	if (hprocessSnap == (HANDLE)-1) {
		printf("\nCreateToolhelp32Snapshot() failed:%d", GetLastError());
		if (temp != NULL)
			delete[] temp;
		return 1;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hprocessSnap, &pe32)) {
		do {
			//printf("%-30s%d\n", pe32.szExeFile, pe32.th32ProcessID);
			temp = c2w(name.c_str());
			if (!wcscmp(temp, pe32.szExeFile)) {
				if (temp != NULL)
					delete[] temp;
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hprocessSnap, &pe32));
	}
	else {
		printf("\nProcess32Firstt() failed:%d", GetLastError());
	}

	CloseHandle(hprocessSnap);
	if (temp != NULL)
		delete[] temp;
	return 0;
}
bool MemoryModify::ReadMemory_4Bytes(HANDLE handle, vector<DWORD> add, DWORD *value, DWORD *address) {
	DWORD error1, error2;
	size_t np = 1, i, size = add.size();

	*address = add[0];
	printf("%u级指针:0x%X\n", np++, *address);
	error1 = ReadProcessMemory(handle, (void *)*address, value, 4, &error2);
	if ((!error1) || (error2 != 4)) {
		goto END;
	}

	if (size>1) {
		printf("%u级指针:0x%X\t", np++, *value);

		for (i = 1; i <size - 1; ++i) {
			printf("偏移:+0x%X\t", add[i]);
			*address = add[i] + *value;
			printf("=0x%X\n", *address);

			error1 = ReadProcessMemory(handle, (void *)*address, value, 4, &error2);
			if ((!error1) || (error2 != 4)) {
				goto END;
			}
			printf("%u级指针:0x%X\t", np++, *value);
		}

		printf("偏移:+0x%X\t", add[i]);
		*address = add[i] + *value;
		printf("=0x%X\n", *address);

		error1 = ReadProcessMemory(handle, (void *)*address, value, 4, &error2);
		if ((!error1) || (error2 != 4)) {
			goto END;
		}
	}
	printf("原值:%u\t", *value);
	cout << endl;
	return true;
END:
	GLE();
	IBRP(*address);
	return false;
}
bool MemoryModify::Modify(DWORD value)
{
	DWORD proID = pid;
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, proID);
	if (hProc == NULL) {
		printf("打开进程失败!\n");
		GLE();
		return false;
	}

	DWORD ads_ps;
	DWORD d1, iS;

	if (!ReadMemory_4Bytes(hProc, address, &d1, &ads_ps)) {
		printf("读内存失败!\n");
		return false;
	}

	if ((!WriteProcessMemory(hProc, (void *)ads_ps, &value, 4, &iS)) || (iS != 4)) {
		printf("写内存失败!\n");
		GLE();
		IBRP(ads_ps);
		return false;
	}

	if (!CloseHandle(hProc)) {
		printf("关闭进程失败!\n");
		GLE();
		return false;
	}
	return true;
}