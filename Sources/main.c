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
			puts("�� ����Ʈ�� �ҷ����� �� �����Ͽ����ϴ�.");
			Sleep(2000);
			break;
		}

		main_gameplay(mapName);
		for (int i = 0; i < mapCount; i++) free(mapList[i]); free(mapList);
	}
}

// stdin ���� Ŭ����
void clearBuffer() {
	while (_kbhit()) {
		int _ = _getch();
	}
}
