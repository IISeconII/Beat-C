/* title.c */

#include "title.h"


// Ÿ��Ʋ ȭ�� ��
void main_title() {

	system("cls");

	for (int i = 0; i < titleHei; i++) {
		gotoxy(SCREEN_WIDTH/2 - titleWid/2, 4+i);
		puts(beatc[i]);
	}

	const char title[] = "Beat-C";
	gotoxy(SCREEN_WIDTH/2 - (int)strlen(title)/2, 4+titleHei+2);
	puts(title);
	const char anykey[] = "�����Ϸ��� �ƹ� Ű�� ��������";
	gotoxy(SCREEN_WIDTH/2 - (int)strlen(anykey)/2, 4+titleHei+4);
	puts(anykey);

	gotoxy(SCREEN_WIDTH/2, 4+titleHei+5);
	setCursor(UNDERBAR);
	clearBuffer();

	int _ = _getch();
}
