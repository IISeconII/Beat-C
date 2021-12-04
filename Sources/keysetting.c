/* keysetting.c */

#include "keysetting.h"


// 키를 설정하는 씬
void main_keysetting() {

	const int klp = 8; // keysetting left padding
	const int ktp = 4; // keysetting top padding
	const int gap = 5; // 키 공간 사이의 간격 (+1)
	memset(key, 0, sizeof(key));

	setCursor(BLOCK);
	gotoxy(klp + (LINE + gap * 3 - 8) / 2, ktp);
	puts("키 설정");
	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i * gap, ktp + 2); wprintf(L"▽");
	}

	// 키 입력
	for (int i = 0; i < LINE; i++) {
		gotoxy(klp + i * gap, ktp + 6); wprintf(L"↑");
		
		int k = 0;
		for (;;) {
			gotoxy(klp + i * gap, ktp + 4);
			setCursor(BLOCK);
			k = _getch();

			BOOL able = TRUE;
			if (k == 0xE0 || k == 0) { k = _getch(); able = FALSE; } // 특수키(확장키) 불가능
			else if (k == 27) able = FALSE; // Esc 불가능
			for (int j = 0; j < i; j++) if (k == key[j]) { able = FALSE; break; } // 중복된 키 불가능

			if (able)
				break;
		}

		wchar_t keyName[5] = L"";
		switch (k) {
			case 8: // Backspace
				wcscpy_s(keyName, sizeof(keyName)/sizeof(wchar_t), L"Bksp"); break;
			case 13: // Enter
				wcscpy_s(keyName, sizeof(keyName)/sizeof(wchar_t), L"Bksp"); break;
			case 32: // Space
				wcscpy_s(keyName, sizeof(keyName)/sizeof(wchar_t), L"Bksp"); break;
			default:
				keyName[0] = k + 0xFEE0 - ('a' <= k && k <= 'z' ? 'a' - 'A' : 0); keyName[1] = '\0'; break;
		}
		gotoxy(klp + i * gap + (wcslen(keyName) == 4 ? -1 : 0), ktp + 4);
		wprintf(L"%s", keyName);
		
		key[i] = k;

		setCursor(HIDE);
		gotoxy(klp + i * gap, ktp + 2); wprintf(L"▼");
		gotoxy(klp + i * gap, ktp + 6); wprintf(L"　");
	}

	Sleep(180);
	for (int r = 0; r < 2; r++) {
		for (int i = 0; i < LINE; i++) {
			gotoxy(klp + i * gap, ktp + 4); wprintf(L"―");
		}
		Sleep(110);
		for (int i = 0; i < LINE; i++) {
			gotoxy(klp + i * gap, ktp + 4); wprintf(L"%c", key[i] + 0xFEE0 - ('a' <= key[i] && key[i] <= 'z' ? 'a' - 'A' : 0));
		}
		Sleep(110);
	}
	
	Sleep(400);

	// 클리어
	for (int i = 0; i < 6; i++) {
		gotoxy(klp, ktp + i);
		for (int j = 0; j < gap * 3 + LINE; j++) {
			_putch(' ');
		}
	}

}
