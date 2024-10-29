// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
// #include <Windows.h>
// #include <iostream>
// #include <string> // c++ strings, that can be returned without going out of scope
#include <psapi.h> // For EnumProcessModules = getting process location
#include <vector> // c++ 'Object' for collections

const byte NOP = 0x90;

std::vector<byte> CreateNOPBytesSize(long size) {
	std::vector<byte> nop_bytes(size, NOP);
	return nop_bytes;
}

HANDLE lifecraft_process_handle = GetCurrentProcess();

DWORD_PTR GetProcessBaseAddress()
{
	DWORD out_number_of_bytes_to_store_all_module_handles;
	bool got_bytes_needed_for_all_handles = EnumProcessModules(lifecraft_process_handle, NULL, 0, &out_number_of_bytes_to_store_all_module_handles);
	
	bool fail = !got_bytes_needed_for_all_handles || !out_number_of_bytes_to_store_all_module_handles;
	if (fail) return 0;

	LPBYTE allocated_module_handles_storage_space_as_bytes = (LPBYTE)LocalAlloc(LPTR, out_number_of_bytes_to_store_all_module_handles);
	
	bool fail_to_alloc = !allocated_module_handles_storage_space_as_bytes;
	if (fail_to_alloc) return 0;

	HMODULE* out_module_handles_array = (HMODULE*)allocated_module_handles_storage_space_as_bytes;
	bool safe_get_all_modules_success = EnumProcessModules(lifecraft_process_handle, out_module_handles_array, out_number_of_bytes_to_store_all_module_handles, &out_number_of_bytes_to_store_all_module_handles);
	
	fail = !safe_get_all_modules_success;
	if (fail)
	{
		LocalFree(allocated_module_handles_storage_space_as_bytes);
		return 0;
	}


	DWORD_PTR baseAddress = (DWORD_PTR)out_module_handles_array[0];

	LocalFree(allocated_module_handles_storage_space_as_bytes);
	return baseAddress;
}

DWORD_PTR ConvertOffsetToAbsoluteAddress(long offset)
{
	return GetProcessBaseAddress() + offset;
}

std::string memory_to_hex_string(std::vector<byte> mem) {
	int size = mem.size();

	std::string outStr;
	char bufferStr[4] = "   ";

	for (int i = 0; i < size; i++) {
		sprintf(bufferStr, "%02X ", mem[i]);
		outStr += bufferStr;
	}

	return outStr;
}

void shout_bytes(std::vector<byte> bytes, const char title[] = "Bytes")
{
	MessageBoxA(NULL, memory_to_hex_string(bytes).c_str(), title, MB_OK);
}

bool write_memory_to_address(std::vector<byte> mem, DWORD_PTR absolute_location)
{
	return WriteProcessMemory(lifecraft_process_handle, (void*)absolute_location, &mem[0], mem.size(), 0);
}

bool is_memory_as_expected(DWORD_PTR loc, std::vector<byte> expected_memory_bytes) {
	int mem_size = expected_memory_bytes.size();

	std::vector<byte> out_read_memory_bytes(mem_size, 0);
	bool read_success = ReadProcessMemory(lifecraft_process_handle, (void*)loc, &out_read_memory_bytes[0], mem_size, 0);
	if (!read_success) return false;

	// shout_bytes(read_vect, "read");

	return expected_memory_bytes == out_read_memory_bytes;
}

// Convert CheatEngine lifecraft.exe+00ABCDEF in to a usable address
// Double check the original memory is as expected. If game has updated, don't break it with mod in wrong place.
bool patch_relative_if_expected_memory(long offset, std::vector<byte> mem_orig, std::vector<byte> mem_new) {
	DWORD_PTR abs_loc = ConvertOffsetToAbsoluteAddress(offset);

	if (!is_memory_as_expected(abs_loc, mem_orig)) return false;

	return write_memory_to_address(mem_new, abs_loc);
}

bool nop_relative_if_expected_memory(long offset, std::vector<byte> mem_orig) {
	return patch_relative_if_expected_memory(offset, mem_orig, CreateNOPBytesSize(mem_orig.size()));
}

void mod_code() {
	const long patch_offset = 0x00265E62; // +265E62 = 2514530
	std::vector<byte> expected = { 0xC7, 0x47, 0x4C, 0xFF, 0xFF, 0xFF, 0xFF }; // C7 47 4C FFFFFFFF

	nop_relative_if_expected_memory(patch_offset, expected);
}

BOOL WINAPI // APIENTRY 
DllMain( HMODULE hModule,
         DWORD  ul_reason_for_call,
         LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		// MessageBoxA(NULL, "Process Attach", "Dll Inject State", MB_OK);

		mod_code();

		break;
    case DLL_THREAD_ATTACH:
		// MessageBoxA(NULL, "Thread Attach", "Dll Inject State", MB_OK);
		break;
    case DLL_THREAD_DETACH:
		// MessageBoxA(NULL, "Thread Detach", "Dll Inject State", MB_OK);
		break;
    case DLL_PROCESS_DETACH:
		CloseHandle(lifecraft_process_handle);
		// MessageBoxA(NULL, "Process Detach", "Dll Inject State", MB_OK);
        break;
    }

    return TRUE;
}

