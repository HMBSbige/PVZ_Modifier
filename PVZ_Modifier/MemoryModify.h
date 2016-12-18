#pragma once
#include <iostream>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <cstring>
#include <vector>
#include <new>
using namespace std;
///////////////////////////
class MemoryModify {
private:
	string ProcessName;
	vector<DWORD> address;
	DWORD pid;

	inline wchar_t* c2w(const char *str)
	{
		int length = strlen(str) + 1;
		//wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
		wchar_t *t = new wchar_t[length];
		memset(t, 0, length * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
		return t;
	}
	BOOL _stdcall IBRP(DWORD add);
	DWORD GLE();
	DWORD getpid(string name);
	bool ReadMemory_4Bytes(HANDLE handle, vector<DWORD> add, DWORD *value, DWORD *address);
public:
	bool init(string name, vector<DWORD> ads) {
		ProcessName = name;
		pid = getpid(ProcessName);
		address = ads;
		if (pid == 0) {
			printf("找不到该进程!\n");
			return false;
		}
		return true;
	}
	DWORD getPID() { return pid; }
	bool Modify(DWORD value);
};