#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <time.h>

extern "C" void shell_header();

int main() {
	std::vector<char> opcode;
	char* code;
	int len;

	char shell_end[] = "\x48\xb8\xf0\xde\xbc\x9a\x78\x56\x34\x12";
	bool is_end = false;
	ULONG key = 0;

	//读取shellocode文件
	std::fstream shellcode("./input.bin", std::ios::binary | std::ios::in);
	if (shellcode.fail()) {
		std::cout << "打开文件失败！" << std::endl;
		return -1;
	}
	shellcode.seekg(0, std::ios::end);
	len = shellcode.tellg();
	shellcode.seekg(0, std::ios::beg);
	code = new char[len];
	shellcode.read(code, len);
	shellcode.close();
	//写入壳头
	for (int i = 0; ;) {
		is_end = true;
		for (int j = 0; j < 10; j++) {
			if (*((char*)shell_header + i + j) != shell_end[j]) {
				is_end = false;

				if ((*(ULONG64*)((char*)shell_header + i)) == 0xffffffffffffffff) {
					//手动修补长度
					for (int z = 0; z < 8; z++) {
						opcode.push_back((len >> (z * 8)) & 0xff);
					}
					i += 8;
				}
				else if ((*(ULONG64*)((char*)shell_header + i)) == 0xeeeeeeeeeeeeeeee){
					srand((unsigned int)time(NULL));
					key = rand() % 0xf0 + 1;
					for (int z = 0; z < 8; z++) {
						opcode.push_back((key >> (z * 8)) & 0xff);
					}
					i += 8;
				}
				else {
					opcode.push_back(*((char*)shell_header + i));
					i++;
				}
				
				break;
			}
		}
		if (is_end) {
			break;
		}
		
	}
	//对shellcode进行加密
	opcode.push_back(code[0]); //汇编解密bug
	for (int i = 1; i < len; i++) {
		opcode.push_back(code[i] ^ key);
	}
	std::cout << "shellcode长度：0x" << std::hex  << len << std::endl;
	std::cout << "密钥：0x" << std::hex << key << std::endl;
	std::fstream output("./abc.bin", std::ios::binary | std::ios::out);
	output.write(opcode.data(), opcode.size());
	output.close();

	delete[] code;
	system("pause");
}
