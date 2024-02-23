#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

CRITICAL_SECTION cs;


DWORD WINAPI f(LPVOID arg){
    printf("Hello from thread!\n");
    ExitThread(0);
}

int main(){
    DWORD thr_id = 0;
    DWORD arg = 5;
    HANDLE hThread = CreateThread(
        NULL,
        0,
        f,
        &arg,
        0,
        &thr_id
    );
    if (hThread == NULL){
        printf("Error %d\n", GetLastError());
        return 1;
    }
    


    return 0;
}