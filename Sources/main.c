/* main.c */

#include "main.h"


main() {

	setlocale(LC_ALL, "");

	main_keysetting();

	main_title();

	char* mapName;
	while (1) {

		mapName = main_selectmap();
		if (mapName == NULL) {
			system("cls");
			gotoxy(1, 1);
			puts("맵 리스트를 불러오는 데 실패하였습니다.");
			Sleep(2000);
			break;
		}

		main_gameplay(mapName);
		for (int i = 0; i < mapCount; i++) free(mapList[i]); free(mapList);
	}
}

// stdin 버퍼 클리어
void clearBuffer() {
	while (_kbhit()) {
		int _ = _getch();
	}
}
