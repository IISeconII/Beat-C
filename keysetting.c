/* keysetting.c */
#include "keysetting.h"


// Ű�� �����ϴ� ��
void main_keysetting() {
	setCursor(BLOCK);
	gotoxy(klp, ktp);
	puts("Ű ����");

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
