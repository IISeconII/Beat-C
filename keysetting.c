/* keysetting.c */
#include "keysetting.h"


// 키를 설정하는 씬
void main_keysetting() {
	setCursor(BLOCK);
	gotoxy(1, 1);
	puts("키 설정");

	for (int i = 0; i < LINE; i++) {
		gotoxy(1+i*2, 3); _putch('v');
	}
	for (int i = 0; i < LINE; i++) {
		gotoxy(1+i*2, 7); _putch('^');
		gotoxy(1+i*2, 5); key[i] = _getche();
		gotoxy(1+i*2, 7); _putch(' ');
	}
}
