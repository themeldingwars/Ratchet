/*
	freak was here
*/

#include "VersionProxy.h"
#include "../Main/Main.h"

BOOL VersionProxyInit()
{
	CHAR pathBuffer[MAX_PATH];

	SHGetSpecialFolderPath(NULL, pathBuffer, CSIDL_WINDOWS, false);
	strcat_s(pathBuffer, MAX_PATH, "\\system32\\version.dll");

	hVersion = LoadLibrary(pathBuffer);
	if (!hVersion)
	{
		MessageBox(NULL, "The original version.dll could not be loaded or it could not be found.", "ERROR", MB_OK | MB_ICONERROR);
		return false;
	}

	lpGetFileVersionInfoA        = GetProcAddress(hVersion, "GetFileVersionInfoA");
	lpGetFileVersionInfoByHandle = GetProcAddress(hVersion, "GetFileVersionInfoByHandle");
	lpGetFileVersionInfoExA      = GetProcAddress(hVersion, "GetFileVersionInfoExA");
	lpGetFileVersionInfoExW      = GetProcAddress(hVersion, "GetFileVersionInfoExW");
	lpGetFileVersionInfoSizeA    = GetProcAddress(hVersion, "GetFileVersionInfoSizeA");
	lpGetFileVersionInfoSizeExA  = GetProcAddress(hVersion, "GetFileVersionInfoSizeExA");
	lpGetFileVersionInfoSizeExW  = GetProcAddress(hVersion, "GetFileVersionInfoSizeExW");
	lpGetFileVersionInfoSizeW    = GetProcAddress(hVersion, "GetFileVersionInfoSizeW");
	lpGetFileVersionInfoW        = GetProcAddress(hVersion, "GetFileVersionInfoW");
	lpVerFindFileA               = GetProcAddress(hVersion, "VerFindFileA");
	lpVerFindFileW               = GetProcAddress(hVersion, "VerFindFileW");
	lpVerInstallFileA            = GetProcAddress(hVersion, "VerInstallFileA");
	lpVerInstallFileW            = GetProcAddress(hVersion, "VerInstallFileW");
	lpVerLanguageNameA           = GetProcAddress(hVersion, "VerLanguageNameA");
	lpVerLanguageNameW           = GetProcAddress(hVersion, "VerLanguageNameW");
	lpVerQueryValueA             = GetProcAddress(hVersion, "VerQueryValueA");
	lpVerQueryValueW             = GetProcAddress(hVersion, "VerQueryValueW");

	return true;
}

#pragma region Proxy Functions

extern "C" BOOL WINAPI wrappedGetFileVersionInfoA(LPCTSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	AfterLoadInit();
	return ((LPGetFileVersionInfoA)(lpGetFileVersionInfoA))(lptstrFilename, dwHandle, dwLen, lpData);
}

extern "C" BOOL WINAPI wrappedGetFileVersionInfoByHandle(DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	return ((LPGetFileVersionInfoByHandle)(lpGetFileVersionInfoByHandle))(dwHandle, dwLen, lpData);
}

extern "C" BOOL WINAPI wrappedGetFileVersionInfoExA(DWORD dwFlags, LPCTSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	return ((LPGetFileVersionInfoExA)(lpGetFileVersionInfoExA))(dwFlags, lptstrFilename, dwHandle, dwLen, lpData);
}

extern "C" BOOL WINAPI wrappedGetFileVersionInfoExW(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	return ((LPGetFileVersionInfoExW)(lpGetFileVersionInfoExW))(dwFlags, lptstrFilename, dwHandle, dwLen, lpData);
}

extern "C" DWORD WINAPI wrappedGetFileVersionInfoSizeA(LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	return ((LPGetFileVersionInfoSizeA)(lpGetFileVersionInfoSizeA))(lptstrFilename, lpdwHandle);
}

extern "C" DWORD WINAPI wrappedGetFileVersionInfoSizeExA(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	return ((LPGetFileVersionInfoSizeExA)(lpGetFileVersionInfoSizeExA))(dwFlags, lptstrFilename, lpdwHandle);
}

extern "C" DWORD WINAPI wrappedGetFileVersionInfoSizeExW(DWORD dwFlags, LPCWSTR lptstrFilename, LPDWORD lpdwHandle)
{
	return ((LPGetFileVersionInfoSizeExW)(lpGetFileVersionInfoSizeExW))(dwFlags, lptstrFilename, lpdwHandle);
}

extern "C" DWORD WINAPI wrappedGetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle)
{
	return ((LPGetFileVersionInfoSizeW)(lpGetFileVersionInfoSizeW))(lptstrFilename, lpdwHandle);
}

extern "C" BOOL WINAPI wrappedGetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	return ((LPGetFileVersionInfoW)(lpGetFileVersionInfoW))(lptstrFilename, dwHandle, dwLen, lpData);
}

extern "C" DWORD WINAPI wrappedVerFindFileA(DWORD dwFlags, LPCTSTR szFileName, LPCTSTR szWinDir, LPCTSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPTSTR szDestDir, PUINT lpuDestDirLen)
{
	return ((LPVerFindFileA)(lpVerFindFileA))(dwFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
}

extern "C" DWORD WINAPI wrappedVerFindFileW(DWORD dwFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen)
{
	return ((LPVerFindFileW)(lpVerFindFileW))(dwFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
}

extern "C" DWORD WINAPI wrappedVerInstallFileA(DWORD uFlags, LPCTSTR szSrcFileName, LPCTSTR szDestFileName, LPCTSTR szSrcDir, LPCTSTR szDestDir, LPCTSTR szCurDir, LPTSTR szTmpFile, PUINT lpuTmpFileLen)
{
	return ((LPVerInstallFileA)(lpVerInstallFileA))(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
}

extern "C" DWORD WINAPI wrappedVerInstallFileW(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen)
{
	return ((LPVerInstallFileW)(lpVerInstallFileW))(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
}

extern "C" DWORD WINAPI wrappedVerLanguageNameA(DWORD wLang, LPTSTR szLang, DWORD cchLang)
{
	return ((LPVerLanguageNameA)(lpVerLanguageNameA))(wLang, szLang, cchLang);
}

extern "C" DWORD WINAPI wrappedVerLanguageNameW(DWORD wLang, LPWSTR szLang, DWORD cchLang)
{
	return ((LPVerLanguageNameW)(lpVerLanguageNameW))(wLang, szLang, cchLang);
}

extern "C" BOOL WINAPI wrappedVerQueryValueA(LPCVOID pBlock, LPCTSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
	return ((LPVerQueryValueA)(lpVerQueryValueA))(pBlock, lpSubBlock, lplpBuffer, puLen);
}

extern "C" BOOL WINAPI wrappedVerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
	return ((LPVerQueryValueW)(lpVerQueryValueW))(pBlock, lpSubBlock, lplpBuffer, puLen);
}

#pragma endregion
