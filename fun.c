/* fun.c */

#include "fun.h"


// (x, y) ��ǥ�� Ŀ���� �̵���Ų��.
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Ŀ�� ����� �����Ѵ�.
void setCursor(CURSOR_TYPE curType) {
	CONSOLE_CURSOR_INFO CurInfo = {0,};

	switch (curType) {
		case HIDE:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = FALSE;
			break;
		case UNDERBAR:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = TRUE;
			break;
		case BLOCK:
			CurInfo.dwSize = 100;
			CurInfo.bVisible = TRUE;
			break;
	}

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
