/* title.c */

#include "title.h"


// Ÿ��Ʋ ȭ�� ��
void title_main() {

	system("cls");

	char title[7][75] = {
		":::::::::  ::::::::::     :::     :::::::::::                :::::::: ",
		":+:    :+: :+:          :+: :+:       :+:                   :+:    :+:",
		"+:+    +:+ +:+         +:+   +:+      +:+                   +:+       ",
		"+#++:++#+  +#++:++#   +#++:++#++:     +#+     +#++:++#++:+  +#+       ",
		"+#+    +#+ +#+        +#+     +#+     +#+                   +#+       ",
		"#+#    #+# #+#        #+#     #+#     #+#                   #+#    #+#",
		"#########  ########## ###     ###     ###                    ######## ",
	};
	const int titleWid = strlen(title[0]); // 70
	const int titleHei = sizeof(title) / sizeof(title[0]); // 7

	for (int i = 0; i < titleHei; i++) {
		gotoxy(SCREEN_WIDTH/2 - titleWid/2, 4+i);
		puts(title[i]);
	}

	char anykey[] = "�����Ϸ��� �ƹ� Ű�� ��������";
	gotoxy(SCREEN_WIDTH/2 - strlen(anykey)/2, 4+titleHei+2);
	puts(anykey);

	gotoxy(SCREEN_WIDTH/2, 4+titleHei+3);
	setCursor(UNDERBAR);
	int _ = _getch();

	mapPath = "maps/"; // �̸� �Ҵ�
}
