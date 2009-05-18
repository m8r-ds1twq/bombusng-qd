#pragma once

#include <windows.h>

class Notify {
private:
    Notify() {};
public:
    static void PlayNotify(int soundst);

    static BOOL vibraOn;
};