/* title.c */

#include "title.h"


// 타이틀 화면 씬
void main_title() {

	system("cls");

	setColor(VIOLET);
	for (int i = 0; i < titleHei; i++) {
		gotoxy(SCREEN_WIDTH/2 - titleWid/2, 4+i);
		puts(beatc[i]);
	}

	const wchar_t title[] = L"Ｂｅａｔ－Ｃ";
	gotoxy((SCREEN_WIDTH - (int)wcslen(title)*2) / 2, 4+titleHei+1);
	wprintf(L"%s", title);

	setColor(GRAY);
	const char anykey[] = "시작하려면 아무 키나 누르세요";
	gotoxy(SCREEN_WIDTH/2 - (int)strlen(anykey)/2, 4+titleHei+4);
	puts(anykey);

	gotoxy(SCREEN_WIDTH/2, 4+titleHei+5);
	setCursor(UNDERBAR);
	clearBuffer();

	int _ = _getch();
}
