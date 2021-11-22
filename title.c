/* title.c */

#include "title.h"


// 타이틀 화면 씬
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

	char anykey[] = "시작하려면 아무 키나 누르세요";
	gotoxy(SCREEN_WIDTH/2 - strlen(anykey)/2, 4+titleHei+2);
	puts(anykey);

	gotoxy(SCREEN_WIDTH/2, 4+titleHei+3);
	setCursor(UNDERBAR);
	int _ = _getch();

	mapPath = "maps/"; // 미리 할당
}
