/* keysetting.c */
#include "keysetting.h"


// 키를 설정하는 씬
void main_keysetting() {
	setCursor(BLOCK);
	gotoxy(klp, ktp);
	puts("키 설정");

	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i*2, ktp + 2); _putch('v');
	}
	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i*2, ktp + 6); _putch('^');
		gotoxy(klp + i*2, ktp + 4); key[i] = _getche();
		gotoxy(klp + i*2, ktp + 6); _putch(' ');
	}

	for (int i = 0; i < 6; i++) {
		gotoxy(klp, ktp + i);
		puts("       ");
	}
	setCursor(HIDE);
}
