/* keysetting.c */
#include "keysetting.h"


// Ű�� �����ϴ� ��
void main_keysetting() {
	setCursor(BLOCK);
	gotoxy(1, 1);
	puts("Ű ����");

	for (int i = 0; i < LINE; i++) {
		gotoxy(1+i*2, 3); _putch('v');
	}
	for (int i = 0; i < LINE; i++) {
		gotoxy(1+i*2, 7); _putch('^');
		gotoxy(1+i*2, 5); key[i] = _getche();
		gotoxy(1+i*2, 7); _putch(' ');
	}
}
