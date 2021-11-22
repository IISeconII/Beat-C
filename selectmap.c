/* selectmap.c */

#include "selectmap.h"


// 맵을 선택하는 씬
char* selectmap_main() {

	system("cls");

	setCursor(HIDE);

	gotoxy(lp, tp);
	puts("곡을 선택하세요.");
	
	int mapCount = 0;
	if (loadMaps(&mapCount) == -1) return NULL;
	
	if (mapCount == 0) {
		gotoxy(lp, tp); puts("맵이 없네요..");
		gotoxy(lp, tp+2); puts("맵 제작법:");
		gotoxy(lp, tp+3); puts("알아서 하세요");
	}

	int pointer = selecting(mapCount);
	return mapList[pointer];
}


// 맵 리스트에서 맵 선택하기
int selecting(int mapCount)
{
	int pointer = 0;
	gotoxy(lp, tp+2+pointer);
	_putch('>');

	int key;
	BOOL selected = FALSE;
	while (!selected) {

		key = _getch();
		if (key == 0xE0 || key == 0)
			key = _getch();

		switch (key) {

			case 72: // ↑ up
				gotoxy(lp, tp+2+pointer); _putch(' ');
				pointer--;
				if (pointer < 0)
					pointer = mapCount - 1;
				break;

			case 80: // ↓ down
				gotoxy(lp, tp + 2 + pointer); _putch(' ');
				pointer++;
				if (pointer >= mapCount)
					pointer = 0;
				break;

			case 13: // enter
			case 32: // space
				selected = TRUE;
				break;
		}

		gotoxy(lp, tp+2+pointer); _putch('>');
	}


	for (int i = 0; i < mapCount; i++) free(mapList[i]); free(mapList);

	return pointer;
}


// 맵 리스트 불러와서 maps에 넣기
int loadMaps(int* mapCount) {

	// 맵 경로 제작 ("maps/*")
	static char* dir = NULL;
	if (dir == NULL) {
		dir = (char*)malloc(strlen(mapPath) + 1 + 1);
		if (dir == NULL) return -1;
		sprintf_s(dir, strlen(dir) + 1 + 1, "%s*", mapPath);
	}
	

	// finddata 세팅
	struct _finddata_t fd;
	long handle = _findfirst(dir, &fd);
	if (handle == -1) {
		*mapCount = 0;
		return 0;
	}

	// 맵 개수 구하기
	while (_findnext(handle, &fd) != -1) {
		if (!strcmp(fd.name, ".."))
			continue;
		(*mapCount)++;
	}

	// 맵 리스트 제작
	mapList = (char**)malloc(*mapCount * sizeof(char*));
	if (mapList == NULL) return -1;

	handle = _findfirst(dir, &fd);
	for (int i = 0; i < *mapCount; i++) {
		if (_findnext(handle, &fd) == -1)
			break;
		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")) {
			i--;
			continue;
		}

		mapList[i] = (char*)malloc(strlen(fd.name)+1);
		if (mapList[i] == NULL) return -1;
		strcpy_s(mapList[i], strlen(fd.name)+1, fd.name);
	}

	_findclose(handle);


	// 리스트 화면에 띄우기
	for (int i = 0; i < *mapCount; i++) {
		gotoxy(lp+2, tp+2+i);
		puts(mapList[i]);
	}

	return 0;
}
