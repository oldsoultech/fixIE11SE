// fixIE11SE_GUI.c
// A basic Windows GUI app to try and stop IE11 to Edge redirection

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void FixIE11Redirection() {
    char systemPath[MAX_PATH];
    GetSystemDirectoryA(systemPath, MAX_PATH);

    char originalDLL[MAX_PATH], renamedDLL[MAX_PATH];
    sprintf(originalDLL, "%s\\ie_to_edge_bho.dll", systemPath);
    sprintf(renamedDLL, "%s\\ie_to_edge_bho.bak", systemPath);

    if (PathFileExistsA(originalDLL)) {
        MoveFileExA(originalDLL, renamedDLL, MOVEFILE_REPLACE_EXISTING);
    }

    HKEY hKey;
    const char* regPath = "Software\\Microsoft\\Internet Explorer\\Main";
    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = 0;
        RegSetValueExA(hKey, "HideInternetExplorerRedirectUXForIncompatibleSites", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
    }

    system("schtasks /Change /TN \"Microsoft\\EdgeUpdate\\EdgeUpdateTaskMachineCore\" /Disable >nul 2>&1");
    system("schtasks /Change /TN \"Microsoft\\EdgeUpdate\\EdgeUpdateTaskMachineUA\" /Disable >nul 2>&1");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MessageBoxA(NULL, "Running fixIE11SE...\nAttempting redirection bypass.", "fixIE11SE", MB_OK | MB_ICONINFORMATION);
    FixIE11Redirection();
    MessageBoxA(NULL, "Done. Please reboot your system.", "fixIE11SE", MB_OK | MB_ICONINFORMATION);
    return 0;
}
