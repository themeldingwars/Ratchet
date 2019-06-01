/*
	freak was here
*/

#ifndef _MAIN_H
#define _MAIN_H

#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <Shlwapi.h>
#include <thread>
#pragma comment(lib, "shlwapi.lib")

using namespace std;
#define TITLE "Ratchet"

BOOL ProcessAttach(HMODULE);
BOOL ProcessDetach(HMODULE);
BOOL IsValidTarget();

void Start();
void AfterLoadInit();
void ThreadRun();

extern std::thread InjectedThread;
extern bool AfterLoadInitHasRun;
extern std::string BaseDIR;

#endif	// _MAIN_H