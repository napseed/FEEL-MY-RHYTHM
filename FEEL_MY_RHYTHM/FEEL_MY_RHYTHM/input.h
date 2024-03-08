#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>



/// <summary>
/// Å° ÄÚµå
/// </summary>
extern const int ESCAPE;
extern const int LEFT;
extern const int RIGHT;
extern const int UP;
extern const int DOWN;
extern const int SPACE;



void SetKeyTable(const int keyIndex, bool isOn);

bool GetKeyTable(const int keyIndex);

void UpdateInput();
void UpdateMenuInput();
