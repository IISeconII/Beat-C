/* fun.h */
#pragma once

#include <Windows.h>


// HIDE: 커서 숨기기, UNDERBAR: _ 커서 보이기, BLOCK: 블록 커서 보이기
typedef enum CURSOR_TYPE { HIDE, UNDERBAR, BLOCK } CURSOR_TYPE;


void gotoxy(int x, int y);
void setCursor(CURSOR_TYPE curType);
