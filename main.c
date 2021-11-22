/* main.c */

#include "main.h"


int main() {

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
