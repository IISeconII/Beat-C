/* keysetting.c */

#include "keysetting.h"


// 키를 설정하는 씬
void keysetting_main() {
	setCursor(BLOCK);
	gotoxy(1, 1);
	puts("키 설정");

	gotoxy(1, 2);
	for (int i = 0; i < LINE; i++) {
		key[i] = _getche();
	}
}
