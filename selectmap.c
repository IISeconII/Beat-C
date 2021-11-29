/* selectmap.c */

#include "selectmap.h"


// 맵을 선택하는 씬
char* main_selectmap() {

	system("cls");

	setCursor(HIDE);

	gotoxy(slp, stp);
	puts("곡을 선택하세요.");
	
	mapCount = 0;
	if (loadMaps() == -1) return NULL;
	
	if (mapCount == 0) {
		system("cls");
		for (int i = 0; i < sizeof(mapGuideDocs) / sizeof(mapGuideDocs[0]); i++) {
			gotoxy(slp, stp+i); puts(mapGuideDocs[i]);
		}

		while (1);
	}

	int pointer = selecting(mapCount);
	return mapList[pointer];
}


// 맵 리스트에서 맵 선택하기
int selecting(int mapCount)
{
	int pointer = 0;
	gotoxy(slp, stp+2+pointer);
	_putch('>');

	int key;
	BOOL selected = FALSE;
	while (!selected) {

		key = _getch();
		if (key == 0xE0 || key == 0)
			key = _getch();

		switch (key) {

			case 72: // ↑ up
				gotoxy(slp, stp+2+pointer); _putch(' ');
				pointer--;
				if (pointer < 0)
					pointer = mapCount - 1;
				break;

			case 80: // ↓ down
				gotoxy(slp, stp + 2 + pointer); _putch(' ');
				pointer++;
				if (pointer >= mapCount)
					pointer = 0;
				break;

			case 13: // enter
			case 32: // space
				selected = TRUE;
				break;
		}

		gotoxy(slp, stp+2+pointer); _putch('>');
	}

	return pointer;
}


// 맵 리스트 불러와서 maps에 넣기
int loadMaps() {

	// 맵 경로 제작 ("maps/*")
	static char* dir = NULL;
	if (dir == NULL) {
		dir = malloc(strlen(mapPath)+1+1);
		if (dir == NULL) return -1;
		sprintf_s(dir, strlen(dir)+1+1, "%s*", mapPath);
	}
	

	// finddata 세팅
	struct _finddata_t fd;
	intptr_t handle = _findfirst(dir, &fd);
	if (handle == -1) {
		mapCount = 0;
		return 0;
	}

	// 맵 개수 구하기
	while (_findnext(handle, &fd) != -1) {
		if (!strcmp(fd.name, ".."))
			continue;
		mapCount++;
	}

	// 맵 리스트 제작
	mapList = malloc(mapCount * sizeof(char*));
	if (mapList == NULL) return -1;

	handle = _findfirst(dir, &fd);
	for (int i = 0; i < mapCount; i++) {
		if (_findnext(handle, &fd) == -1)
			break;
		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")) {
			i--;
			continue;
		}
		
		mapList[i] = malloc(strlen(fd.name)+1);
		if (mapList[i] == NULL) return -1;
		strcpy_s(mapList[i], strlen(fd.name)+1, fd.name);
	}
	
	_findclose(handle);


	// 리스트 화면에 띄우기
	for (int i = 0; i < mapCount; i++) {
		gotoxy(slp+2, stp+2+i);
		puts(mapList[i]);
	}

	return 0;
}


