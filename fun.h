/* fun.h */
#pragma once

#include <Windows.h>


// HIDE: Ŀ�� �����, UNDERBAR: _ Ŀ�� ���̱�, BLOCK: ��� Ŀ�� ���̱�
typedef enum CURSOR_TYPE { HIDE, UNDERBAR, BLOCK } CURSOR_TYPE;


void gotoxy(int x, int y);
void setCursor(CURSOR_TYPE curType);
