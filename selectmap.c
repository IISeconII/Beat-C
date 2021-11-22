/* selectmap.c */

#include "selectmap.h"


// ���� �����ϴ� ��
char* selectmap_main() {

	system("cls");

	setCursor(HIDE);

	gotoxy(lp, tp);
	puts("���� �����ϼ���.");
	
	int mapCount = 0;
	if (loadMaps(&mapCount) == -1) return NULL;
	
	if (mapCount == 0) {
		system("cls");

		static char mapGuideDocs[][82] = { // �� ���۹� �ؽ�Ʈ
			":( ���� ���׿�..",
			"",
			"�� ���۹�:",
			"- ���α׷��� �ִ� ��ο� maps ������ ����ϴ�. ���� �⺻ ����Դϴ�.",
			"- ���� �ȿ� �� �̸����� �� ������ ����ϴ�. �̰��� ���� ���ϴ�.",
			"- �� �ȿ� ���̸�.txt ������ ����ϴ�. ���⿡ ��Ʈ �����Ͱ� ���ϴ�.",
			"",
			"��Ʈ �����ʹ� ������ ���� �����Դϴ�.",
			"",
			"@... @�� ��Ʈ�̰�, .�� ���� ����(' ')�Դϴ�.",
			".... (��('.')�� �����ֱ� ���ؼ� ���� ��� �� �̴ϴ�)",
			".@.. ������ ���۵Ǹ� �� �� �ٺ��� ��Ʈ ���ڸ� �о ���� ȭ�鿡 ��Ʈ�� ���ϴ�.",
			".... // �ۼ� ��",
			"..@.",
			"....",
			"...@",
		};
		for (int i = 0; i < sizeof(mapGuideDocs) / sizeof(mapGuideDocs[0]); i++) {
			gotoxy(lp, tp+i); puts(mapGuideDocs[i]);
		}

		while (1);
	}

	int pointer = selecting(mapCount);
	return mapList[pointer];
}


// �� ����Ʈ���� �� �����ϱ�
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

			case 72: // �� up
				gotoxy(lp, tp+2+pointer); _putch(' ');
				pointer--;
				if (pointer < 0)
					pointer = mapCount - 1;
				break;

			case 80: // �� down
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


// �� ����Ʈ �ҷ��ͼ� maps�� �ֱ�
int loadMaps(int* mapCount) {

	// �� ��� ���� ("maps/*")
	static char* dir = NULL;
	if (dir == NULL) {
		dir = (char*)malloc(strlen(mapPath) + 1 + 1);
		if (dir == NULL) return -1;
		sprintf_s(dir, strlen(dir) + 1 + 1, "%s*", mapPath);
	}
	

	// finddata ����
	struct _finddata_t fd;
	intptr_t handle = _findfirst(dir, &fd);
	if (handle == -1) {
		*mapCount = 0;
		return 0;
	}

	// �� ���� ���ϱ�
	while (_findnext(handle, &fd) != -1) {
		if (!strcmp(fd.name, ".."))
			continue;
		(*mapCount)++;
	}

	// �� ����Ʈ ����
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


	// ����Ʈ ȭ�鿡 ����
	for (int i = 0; i < *mapCount; i++) {
		gotoxy(lp+2, tp+2+i);
		puts(mapList[i]);
	}

	return 0;
}
