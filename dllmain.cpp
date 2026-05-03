#include "pch.h"
#include "dllmain.h"
#include <windows.h>;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH: {
		bool result = PatchManeuver();

		if (!result) {
			MessageBox(NULL, L"ImprovedAxiom Maneuver: Patch failed.", L"Patch Status", MB_OK | MB_ICONERROR);
		}
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
	char* pCall = (char*)0x140E69F8D;
	char nops[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	char expectedBytes[5] = { 0xE8 ,0xEE, 0xD1, 0xFF, 0xFF};

	if (memcmp(pCall, expectedBytes, 5) != 0)
		return false;

	DWORD oldprotect;
	VirtualProtect(pCall, 5, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(pCall, nops, 5);
	VirtualProtect(pCall, 5, oldprotect, &oldprotect);

	return true;
}

/*
bool PatchAxiomRange() {
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
	char* pReelDuration = (char*)0x140420705;
	char zeroReelDurationJmp[2] = { 0xeb, 0x43 };
	char reelExpected[2] = { 0x76, 0x43 };

	char* pPropellDuration = (char*)0x1404209a0;
	char zeroPropellJmp[2] = { 0xeb, 0x06};
	char propellExpected[2] = { 0x76, 0x06 };

	if (memcmp(pReelDuration, reelExpected, 2) != 0)
		return false;
	if (memcmp(pPropellDuration, propellExpected, 2) != 0)
		return false;


	DWORD oldprotect;
	VirtualProtect(pReelDuration, 1024, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(pReelDuration, zeroReelDurationJmp, 2);
	memcpy(pPropellDuration, zeroPropellJmp, 2);
	VirtualProtect(pReelDuration, 1024, oldprotect, &oldprotect);

	return true;
}
*/