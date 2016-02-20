#include "v_utility.h"
#include <windows.h>
#include <iostream>

using namespace std;
void fildFiles()
{
	/*
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
	//HANDLE handle = FindFirstFile("c:\\*", &search_data);
	LPCTSTR           lpFileName;
	LPWIN32_FIND_DATA lpFindFileData;
	//HANDLE handle = FindFirstFile(_In_  LPCTSTR           lpFileName, _Out_ LPWIN32_FIND_DATA lpFindFileData);
	HANDLE handle = FindFirstFile(lpFileName,lpFindFileData);
	while (handle != INVALID_HANDLE_VALUE)
	{
		std::cout << "\n" << search_data.cFileName;
		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}
	//Close the handle after use or memory/resource leak
	FindClose(handle);
	*/
}