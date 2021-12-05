/* selectmap.c */

#include "selectmap.h"


// ���� �����ϴ� ��
char* main_selectmap() {
	system("cls");
	setCursor(HIDE);

	gotoxy(slp, stp);
	puts("���� �����ϼ���.");
	
	mapCount = 0;
	if (loadMaps() == -1) return NULL;
	
	if (mapCount == 0) {
		system("cls");
		for (int i = 0; i < sizeof(mapGuideDocs) / sizeof(mapGuideDocs[0]); i++) {
			gotoxy(slp, stp+i); puts(mapGuideDocs[i]);
		}

		while (1);
	}

	else {
		gotoxy(slp + 30, stp-1); puts("Ű�� �����Ϸ���");
		gotoxy(slp + 30, stp);   puts("SŰ�� ��������");
	}

	int pointer = selecting(mapCount);
	return mapList[pointer];
}


// �� ����Ʈ���� �� �����ϱ�
int selecting(int mapCount)
{
	int pointer = 0;
	gotoxy(slp, stp+2+pointer); _putch('>');

	Sleep(300);
	clearBuffer();

	int key;
	BOOL selected = FALSE;
	while (!selected) {

		key = _getch();
		if (key == 0xE0 || key == 0)
			key = _getch();

		switch (key) {

			case 's':
				gotoxy(slp, stp+2+pointer); _putch(' ');
				main_keysetting();
				gotoxy(slp, stp+2+pointer); _putch('>');
				break;

			case UP:
				gotoxy(slp, stp+2+pointer); _putch(' ');
				pointer--;
				if (pointer < 0)
					pointer = mapCount - 1;
				break;

			case DOWN:
				gotoxy(slp, stp + 2 + pointer); _putch(' ');
				pointer++;
				if (pointer >= mapCount)
					pointer = 0;
				break;

			case ENTER:
			case SPACE:
				selected = TRUE;
				break;
		}

		gotoxy(slp, stp+2+pointer); _putch('>');
	}

	return pointer;
}


// �� ����Ʈ �ҷ��ͼ� maps�� �ֱ�
int loadMaps() {

	mapFolder = "maps";

	// �� ��� ���� ("maps/*")
	static char* dir = NULL;
	if (dir == NULL) {
		dir = malloc(strlen(mapFolder)+2+1);
		if (dir == NULL) return -1;
		sprintf_s(dir, strlen(dir)+2+1, "%s/*", mapFolder);
	}
	
	// finddata ����
	struct _finddata_t fd;
	intptr_t handle = _findfirst(dir, &fd);
	if (handle == -1) {
		mapCount = 0;
		return 0;
	}

	// �� ���� ���ϱ�
	while (_findnext(handle, &fd) != -1) {
		if (!strcmp(fd.name, "..") || !(fd.attrib & _A_SUBDIR)) // .. �Ǵ� ������ �ƴϸ�
			continue;
		mapCount++;
	}

	// �� ����Ʈ ����
	mapList = malloc(mapCount * sizeof(char*));
	if (mapList == NULL) return -1;

	handle = _findfirst(dir, &fd);
	for (int i = 0; i <= mapCount; i++) {
		if (_findnext(handle, &fd) == -1)
			break;
		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..") || !(fd.attrib & _A_SUBDIR)) { // . �Ǵ� .. �Ǵ� ������ �ƴϸ�
			i--; continue;
		}
		
		mapList[i] = malloc(strlen(fd.name)+1);
		if (mapList[i] == NULL) return -1;
		strcpy_s(mapList[i], strlen(fd.name)+1, fd.name);
	}
	
	_findclose(handle);


	// ���̽��ھ� ������ ���� (������ ����)
	const char* highScoreFileName = "stats.json";
	const int hsfPathSize = (int)strlen(mapFolder) + 1 + (int)strlen(highScoreFileName) + 1;
	hsfPath = malloc(hsfPathSize);
	if (hsfPath == NULL) return -1;
	sprintf_s(hsfPath, hsfPathSize, "%s/%s", mapFolder, highScoreFileName);

	JSON_Value* rootValue = json_parse_file(hsfPath);
	if (rootValue == NULL) {
		rootValue = json_value_init_object();
		json_serialize_to_file_pretty(rootValue, hsfPath);
	}
	JSON_Object* highScore = json_value_get_object(rootValue);


	// ����Ʈ & ���� ȭ�鿡 ����
	for (int i = 0; i < mapCount; i++) {
		gotoxy(slp+2, stp+2+i); puts(mapList[i]);
		gotoxy(slp+2+30, stp+2+i); printf("%d", (int)json_object_get_number(highScore, mapList[i]));
	}

	return 0;
}
