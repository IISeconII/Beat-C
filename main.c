/* main.c */

#include "main.h"


int main() {

	keysetting_main();

	title_main();

	char* mapName;
	while (1) {
		mapName = selectmap_main();
		if (mapName == NULL) {
			system("cls");
			gotoxy(1, 1);
			puts("맵 리스트를 불러오는 데 실패하였습니다.");
			Sleep(1000);
			break;
		}

		gameplay_main(mapName);
	}
}
