/*
	freak was here
*/

#include <Windows.h>
#include <Shlobj.h>

BOOL VersionProxyInit();
static HINSTANCE hVersion = 0;

typedef BOOL(WINAPI *LPGetFileVersionInfoA)(LPCTSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef BOOL(WINAPI *LPGetFileVersionInfoByHandle)(DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef BOOL(WINAPI *LPGetFileVersionInfoExA)(DWORD dwFlags, LPCTSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef BOOL(WINAPI *LPGetFileVersionInfoExW)(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef DWORD(WINAPI *LPGetFileVersionInfoSizeA)(LPCTSTR lptstrFilename, LPDWORD lpdwHandle);
typedef DWORD(WINAPI *LPGetFileVersionInfoSizeExA)(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle);
typedef DWORD(WINAPI *LPGetFileVersionInfoSizeExW)(DWORD dwFlags, LPCWSTR lptstrFilename, LPDWORD lpdwHandle);
typedef DWORD(WINAPI *LPGetFileVersionInfoSizeW)(LPCWSTR lptstrFilename, LPDWORD lpdwHandle);
typedef DWORD(WINAPI *LPGetFileVersionInfoW)(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef DWORD(WINAPI *LPVerFindFileA)(DWORD dwFlags, LPCTSTR szFileName, LPCTSTR szWinDir, LPCTSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPTSTR szDestDir, PUINT lpuDestDirLen);
typedef DWORD(WINAPI *LPVerFindFileW)(DWORD dwFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen);
typedef DWORD(WINAPI *LPVerInstallFileA)(DWORD uFlags, LPCTSTR szSrcFileName, LPCTSTR szDestFileName, LPCTSTR szSrcDir, LPCTSTR szDestDir, LPCTSTR szCurDir, LPTSTR szTmpFile, PUINT lpuTmpFileLen);
typedef DWORD(WINAPI *LPVerInstallFileW)(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen);
typedef DWORD(WINAPI *LPVerLanguageNameA)(DWORD wLang, LPTSTR szLang, DWORD cchLang);
typedef DWORD(WINAPI *LPVerLanguageNameW)(DWORD wLang, LPWSTR szLang, DWORD cchLang);
typedef BOOL(WINAPI *LPVerQueryValueA)(LPCVOID pBlock, LPCTSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);
typedef BOOL(WINAPI *LPVerQueryValueW)(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);

static FARPROC lpGetFileVersionInfoA = NULL;
static FARPROC lpGetFileVersionInfoByHandle = NULL;
static FARPROC lpGetFileVersionInfoExA = NULL;
static FARPROC lpGetFileVersionInfoExW = NULL;
static FARPROC lpGetFileVersionInfoSizeA = NULL;
static FARPROC lpGetFileVersionInfoSizeExA = NULL;
static FARPROC lpGetFileVersionInfoSizeExW = NULL;
static FARPROC lpGetFileVersionInfoSizeW = NULL;
static FARPROC lpGetFileVersionInfoW = NULL;
static FARPROC lpVerFindFileA = NULL;
static FARPROC lpVerFindFileW = NULL;
static FARPROC lpVerInstallFileA = NULL;
static FARPROC lpVerInstallFileW = NULL;
static FARPROC lpVerLanguageNameA = NULL;
static FARPROC lpVerLanguageNameW = NULL;
static FARPROC lpVerQueryValueA = NULL;
static FARPROC lpVerQueryValueW = NULL;
