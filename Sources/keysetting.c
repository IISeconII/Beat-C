/* keysetting.c */

#include "keysetting.h"


// Ű�� �����ϴ� ��
void main_keysetting() {

	const int klp = 8; // keysetting left padding
	const int ktp = 4; // keysetting top padding
	const int gap = 5; // Ű ���� ������ ���� (+1)
	memset(key, 0, sizeof(key));

	setCursor(BLOCK);
	gotoxy(klp + (LINE + gap * 3 - 8) / 2, ktp);
	puts("Ű ����");
	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i * gap, ktp + 2); _putch('v');
	}

	// Ű �Է�
	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i * gap, ktp + 6); _putch('^');
		
		int k = 0;
		for (;;) {
			gotoxy(klp + i * gap, ktp + 4);
			setCursor(BLOCK);
			k = _getch();

			if (k == 0xE0 || k == 0) { k = _getch(); continue; } // Ư��Ű(Ȯ��Ű) �Ұ���
			else if (k == 27) continue; // Esc �Ұ���
			for (int j = 0; j < i; j++) if (k == key[j]) continue; // �ߺ��� Ű �Ұ���

			break;
		}

		char keyName[4];
		switch (k) {
			case 8:
				strcpy_s(keyName, sizeof(keyName), "Bsp"); break;
			case 13:
				strcpy_s(keyName, sizeof(keyName), "Ent"); break;
			case 32:
				strcpy_s(keyName, sizeof(keyName), "Spc"); break;
			default:
				keyName[0] = k; keyName[1] = '\0'; break;
		}
		gotoxy(curX() + (strlen(keyName) == 3 ? -1 : 0), curY());
		printf("%s", keyName);
		key[i] = k;

		setCursor(HIDE);
		gotoxy(klp + i * gap, ktp + 6); _putch(' ');
	}

	Sleep(500);

	// Ŭ����
	for (int i = 0; i < 6; i++) {
		gotoxy(klp, ktp + i);
		for (int j = 0; j < gap * 3 + LINE; j++) {
			_putch(' ');
		}
	}

}
