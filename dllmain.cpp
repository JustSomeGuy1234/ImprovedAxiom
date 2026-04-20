#include "pch.h"
#include "dllmain.h"
#include <windows.h>;
#include <fstream>;
#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH: {
		HMODULE hExecutable = GetModuleHandle(NULL);
		bool failed = false;
		try {
			failed = failed || !PatchManeuver();
			/*failed = failed || !PatchAxiomRange();
			log << "AxiomRange patch: " << (failed ? "Failed" : "Success") << std::endl;
			failed = failed || !PatchAxiomSpeed();
			log << "AxiomSpeed patch: " << (failed ? "Failed" : "Success") << std::endl;*/
		} catch(...) {
			failed = true;
		}
		if (failed) {
			MessageBox(NULL, L"ImprovedAxiom Maneuver failed to patch. Mod is likely out of date.", L"Patch Status", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		//log.flush();
		break;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

bool PatchManeuver() {
	char* pCall = (char*)0x140e25e5d;
	char nops[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	char expectedBytes[5] = { 0xe8, 0x6e, 0xce, 0xff, 0xff };

	if (memcmp(pCall, expectedBytes, 5) != 0)
		return false;

	DWORD oldprotect;
	VirtualProtect(pCall, 1024, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(pCall, nops, 5);
	VirtualProtect(pCall, 1024, oldprotect, &oldprotect);

	return true;
}

/*bool PatchAxiomRange() {
	float* pRange = (float*)0x146e22038;
	if (*(UINT32*)pRange != 1101004800)
		return false;

	DWORD oldprotect;
	VirtualProtect(pRange, 1024, PAGE_EXECUTE_READWRITE, &oldprotect);
	*pRange = 100.0f;
	VirtualProtect(pRange, 1024, oldprotect, &oldprotect);
	
	return true;
}

bool PatchAxiomSpeed() {
	char* pReelDuration = (char*)0x14042074A;
	char* pPropellDuration = (char*)0x1404209AB;
	char zeroDurationInstr[9] = { 0xc6, 0x84, 0x3e, 0xe7, 0x09, 0x00, 0x00, 0x00, 0x90};

	char expected[9] = { 0xF3, 0x0F, 0x11, 0xBC, 0x3E, 0xE4, 0x09, 0x00, 0x00 };
	if (memcmp(pReelDuration, expected, 9) != 0)
		return false;

	DWORD oldprotect;
	VirtualProtect(pReelDuration, 1024, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(pReelDuration, zeroDurationInstr, 9);
	memcpy(pPropellDuration, zeroDurationInstr, 9);
	VirtualProtect(pReelDuration, 1024, oldprotect, &oldprotect);

	return true;
}*/