#include <iostream>
#include <windows.h>
#include <fstream>

using namespace std;

HANDLE t1 = NULL;
HANDLE t2 = NULL;

DWORD WINAPI Func1(LPVOID p) {
    ofstream f("out1.txt");
    while (true) {
        f << "Thread 1 is working..." << endl;
        Sleep(600);
    }
    return 0;
}

DWORD WINAPI Func2(LPVOID p) {
    ofstream f("out2.txt");
    while (true) {
        f << "Thread 2 is working..." << endl;
        Sleep(600);
    }
    return 0;
}

void ShowTime(HANDLE t) {
    FILETIME t_create, t_exit, t_kern, t_user;
    SYSTEMTIME u_time, l_time;

    if (GetThreadTimes(t, &t_create, &t_exit, &t_kern, &t_user)) {
        FileTimeToSystemTime(&t_create, &u_time);
        SystemTimeToTzSpecificLocalTime(NULL, &u_time, &l_time);

        printf("Time: %02d/%02d/%d %02d:%02d:%02d\n",
            l_time.wDay, l_time.wMonth, l_time.wYear,
            l_time.wHour, l_time.wMinute, l_time.wSecond);
    }
    else {
        cout << "Error time." << endl;
    }
}

int main() {
    int c;

    while (true) {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Start Threads" << endl;
        cout << "2. Pause 1" << endl;
        cout << "3. Resume 1" << endl;
        cout << "4. Priority UP" << endl;
        cout << "5. Context" << endl;
        cout << "6. Time" << endl;
        cout << "0. Exit" << endl;
        cout << "Select: ";
        cin >> c;

        if (c == 1) {
            if (t1 == NULL && t2 == NULL) {
                t1 = CreateThread(NULL, 0, Func1, NULL, 0, NULL);
                t2 = CreateThread(NULL, 0, Func2, NULL, 0, NULL);
                cout << "OK. Started." << endl;
            }
            else {
                cout << "Already started." << endl;
            }
        }
        else if (c == 2) {
            if (t1 != NULL) {
                SuspendThread(t1);
                cout << "Paused." << endl;
            }
        }
        else if (c == 3) {
            if (t1 != NULL) {
                ResumeThread(t1);
                cout << "Resumed." << endl;
            }
        }
        else if (c == 4) {
            if (t1 != NULL) {
                SetThreadPriority(t1, THREAD_PRIORITY_HIGHEST);
                cout << "Priority = HIGH." << endl;
            }
        }
        else if (c == 5) {
            if (t1 != NULL) {
                CONTEXT reg;
                reg.ContextFlags = CONTEXT_CONTROL;
                if (GetThreadContext(t1, &reg)) {
                    cout << "IP: 0x" << hex << reg.Rip << endl;
                    cout << "SP: 0x" << hex << reg.Rsp << dec << endl;
                }
            }
        }
        else if (c == 6) {
            if (t1 != NULL) {
                ShowTime(t1);
            }
            else {
                cout << "No thread yet." << endl;
            }
        }
        else if (c == 0) {
            if (t1 != NULL) {
                TerminateThread(t1, 0);
                CloseHandle(t1);
            }
            if (t2 != NULL) {
                TerminateThread(t2, 0);
                CloseHandle(t2);
            }
            cout << "Bye." << endl;
            break;
        }
    }
    return 0;
}