/* keysetting.c */

#include "keysetting.h"


// Ű�� �����ϴ� ��
void keysetting_main() {
	setCursor(BLOCK);
	gotoxy(1, 1);
	puts("Ű ����");

	gotoxy(1, 2);
	for (int i = 0; i < LINE; i++) {
		key[i] = _getche();
	}
}
