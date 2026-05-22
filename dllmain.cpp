#include "pch.h"
#include "dllmain.h"
#include <windows.h>;

void* PMANEUVERCALL = (char*)0x140E91C3D;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if (!CheckMemory())
		return TRUE;

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

bool CheckMemory() {
	// Check that the game is the one loading the ASI,
	// and not crashpad_handler.exe which is in the same folder.
	MEMORY_BASIC_INFORMATION* info = new MEMORY_BASIC_INFORMATION;
	PMEMORY_BASIC_INFORMATION pInfo = info;
	size_t querySz = VirtualQuery(PMANEUVERCALL, pInfo, sizeof(*info));
	return (info->State == MEM_COMMIT);
}

bool PatchManeuver() {
	char nops[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	char expectedBytes[5] = { 0xE8, 0xEE, 0xD1, 0xFF, 0xFF};

	if (memcmp(PMANEUVERCALL, expectedBytes, 5) != 0)
		return false;

	DWORD oldprotect;
	VirtualProtect(PMANEUVERCALL, 5, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(PMANEUVERCALL, nops, 5);
	VirtualProtect(PMANEUVERCALL, 5, oldprotect, &oldprotect);

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