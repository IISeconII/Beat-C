/* keysetting.c */

#include "keysetting.h"


// Ű�� �����ϴ� ��
void main_keysetting() {
	setCursor(BLOCK);
	gotoxy(1, 1);
	puts("Ű ����");

	gotoxy(1, 2);
	for (int i = 0; i < LINE; i++) {
		key[i] = _getche();
	}
}
