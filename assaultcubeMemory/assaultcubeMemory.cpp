#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <list>

using namespace std;

uintptr_t GetModuleBaseAddress(DWORD dwProcID, char* szModuleName)
{
    uintptr_t ModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 ModuleEntry32;
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &ModuleEntry32))
        {
            do
            {
                if (ModuleEntry32.szModule == (WCHAR *)szModuleName)
                {
                    
                }
                else
                {
                    ModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }
        CloseHandle(hSnapshot);
    }
    return ModuleBaseAddress;
}

int main()
{
    int healthValue = 100;

    

    HWND hwnd = FindWindowA(NULL, "AssaultCube");

    if (hwnd == NULL)
    {
        cout << "Window wasn't found." << endl;
        Sleep(3000);
        exit(-1);
    }
    else
    {
        

        DWORD procID;
        GetWindowThreadProcessId(hwnd, &procID);
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

        if (procID == NULL)
        {
            cout << "Process ID wasn't found." << endl;
            Sleep(3000);
            exit(-1);
        }
        else
        {
            list<string> players;

            DWORD baseAddress = GetModuleBaseAddress(procID, (char*)"ac_client.exe");
            DWORD pEntities = baseAddress + 0x10F4F8;
            DWORD aEntities;
            DWORD aOffset;
            DWORD someShit;
            
            

            for (int i = 1; i < 12; i++)
            {
                aOffset = 0x0;
                ReadProcessMemory(handle, (LPVOID)pEntities, &aEntities, sizeof(aEntities), NULL);
                aEntities += i * 4;


                ReadProcessMemory(handle, (LPVOID)aEntities, &someShit, sizeof(someShit), NULL);

                aOffset = someShit + 0x0225;

                char value[25] = { 0 };
                
                ReadProcessMemory(handle, (LPVOID)aOffset, &value, sizeof(value), NULL);

                cout << value << endl;

                players.push_front(value);
            }

            

         

            Sleep(100000);

            /*while (true) 
            {
                if (GetKeyState(VK_MENU) & 0x8000)
                {
                    DWORD entityPointer = baseAddress + 0x10F4F4;
                    DWORD entityAddress;
                    ReadProcessMemory(handle, (LPVOID)entityPointer, &entityAddress, sizeof(entityAddress), NULL);
                    DWORD entityHealth = entityAddress + 0xF8;
                    WriteProcessMemory(handle, (LPVOID)entityHealth, &healthValue, sizeof(healthValue), 0);
                }
            }*/


        }
    }

    return 0;
}
