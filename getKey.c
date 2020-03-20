#include <windows.h>
#include "getKey.h"

int getKey(int keyCode) {
    short keyState = GetAsyncKeyState(keyCode & '_');
    if (keyState & 0x8000) {
        return 1;
    }
    return 0;
};