/*
	Copyright (c) 2018 Jonathan Khoo

	This software is provided "AS IS".  No warranty is expressed or implied.
	Permission to copy without fee all or part of this material is granted
	provided that the copies are not made or distributed for direct commercial
	advantage, this copyright notice is included, and notice that copying is
	by permission of the author of this software.  To copy otherwise, or to
	republish, requires a fee and/or specific written permission from the author.

*/

#define VC_EXTRALEAN
#include <windows.h>
#include <fstream>
#include <iomanip>
#include "LC3Extension.h"

std::ofstream fo;
BITS ir = 0;
bool firstInstruction = true;
bool halted = false;

// The standard DLL entry point.  This function is not necessary.  It called by
// the Win32 framework.  One time initialization and cleanup should occur here.
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	// On start up, open the output file.
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		fo.open("log.txt");
		fo << std::internal << std::setfill('0');
	}
    return TRUE;
}

// FireInterrupt3 is a required method for an LC3 extension.  This implementation 
// prints the current state of the simulator to the output file after every instruction
// until a HALT instruction is reached, then the output file is closed.
LC_API int FireInterrupt3(GetValueMethod GetValue) {
	BITS r[8], pc, psr;

	if (halted || ir == 0xf025) {
		halted = true;
		fo.close();
		return NO_INTERRUPT;
	}
	if (firstInstruction) {
		firstInstruction = false;
		if (GetValue(LCEXT_PC, pc) != LCEXT_SUCCESS) {
			return NO_INTERRUPT;
		}
		GetValue(pc, ir);
		return NO_INTERRUPT;
	}
	for (int i = 0; i < 8; i++) {
		if (GetValue(LCEXT_R0 - i, r[i]) == LCEXT_SUCCESS) {
			fo << "R" << i << "\t0x" << std::hex << std::setw(4) << r[i] << std::endl;
		}
	}
	if (GetValue(LCEXT_PC, pc) == LCEXT_SUCCESS) {
		fo << "PC\t0x" << std::hex << std::setw(4) << pc << std::endl;
	}
	fo << "IR\t0x" << std::hex << std::setw(4) << ir << std::endl;
	GetValue(pc, ir);
	if (GetValue(LCEXT_PSR, psr) == LCEXT_SUCCESS) {
		fo << "CC\t";
		if (psr & (1 << 2)) fo << "N";
		if (psr & (1 << 1)) fo << "Z";
		if (psr & (1 << 0)) fo << "P";
		fo << std::endl;
	}
	fo << "==================" << std::endl;
	fo.flush();
	return NO_INTERRUPT;
}

// Unused method.
LC_API void ExecuteReservedOpcode3(GetValueMethod GetValue, SetValueMethod SetValue) {
	return;
}
