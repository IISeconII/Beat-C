/* gameplay.c */

#include "gameplay.h"


// ������ �÷����ϴ� ��
void gameplay_main(char* _mapName) {
	mapName = _mapName;

	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);
	score = 0;
	mapIndex = 0;
	songPlayed = FALSE;
	gameEnd = FALSE;

	system("cls");

	if (readNoteMapFile() == -1) {
		system("cls");
		gotoxy(1, 1);
		puts("�� ������ �������� �ʰų� ���� �� �� �����ϴ�.");
		Sleep(1000);
		return;
	}

	drawScreen();

	countdown();

	while (!gameEnd) {
		fallingNote();
		keyInput();
		removingJudgeTxt();
	}

	showScore();
	while (!_kbhit()) removingJudgeTxt();


	free(mapDir);
	for (int i = 0; i < mapLength; i++) free(map[i]); free(map);
}


// ��Ʈ �� ������ �о note�� �����Ѵ�.
int readNoteMapFile() {
	FILE *f;

	// �� ���� ��� ���� (maps/mapName/)
	int mapDirSize = strlen(mapPath) + strlen(mapName) + 1 + 1;
	mapDir = (char*)malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s%s/", mapPath, mapName);

	// �� ���� ��� ���� (maps/mapName/mapName.txt)
	const char* extension = ".txt";
	int pathSize = mapDirSize + strlen(mapName) + strlen(extension);
	char* path = (char*)malloc(pathSize);
	if (path == NULL) return -1;
	sprintf_s(path, pathSize, "%s%s%s", mapDir, mapName, extension);


	// �� ���� ����
	mapLength = 0;
	fopen_s(&f, path, "r");
	if (f == NULL) return -1;

	while (!feof(f)) {
		if (fgetc(f) == '\n')
			mapLength++;
	} mapLength++;

	fseek(f, 0, SEEK_SET);

	
	// �� �о map�� �ֱ�
	map = (char**)malloc(mapLength * sizeof(char*));
	if (map == NULL) return -1;
	char* line;

	for (int i = 0; i < mapLength; i++) {

		line = (char*)malloc(LINE * sizeof(char));
		if (line == NULL) return -1;

		for (int j = 0; j < LINE; j++) {
			line[j] = fgetc(f);
		}

		map[i] = line;

		fgetc(f); // '\n'
	}

	fclose(f);
	free(path);
	return 0;
}

// ȭ�� ���� �׸���.
void drawScreen() {
	system("cls");

	// �翷 �ڽ�
	for (int i = 0; i < HEI; i++) {
		gotoxy(LINE * NOTETHK, i); _putch('|');
	}

	// ������
	gotoxy(0, HEI - 2);
	for (int j = 0; j <= LINE * NOTETHK; j++) {
		_putch('-');
	}

	// �������
	gotoxy(0, HEI);
	for (int j = 0; j <= LINE * NOTETHK; j++) {
		_putch('^');
	}
}

// ī��Ʈ�ٿ�
void countdown() {
	for (int i = 3; i >= 1; i--) {
		gotoxy(LINE*NOTETHK/2, HEI/2-1); // �� �߾�
		printf("%d", i);
		Sleep(500);
	}

	gotoxy(LINE*NOTETHK/2-3, HEI/2-1);
	puts("Start!");
	Sleep(500);
}

// �뷡(BGM)�� ����Ѵ�.
void playSong() {
	char extension[] = ".wav";
	const int pathSize = strlen(mapDir) + strlen(mapName) + sizeof(extension);
	char* songPath = (char*)malloc(pathSize);
	if (songPath == NULL) return;
	sprintf_s(songPath, pathSize, "%s%s%s", mapDir, mapName, extension);

	PlaySound(TEXT("maps/map1/map1.wav"), 0, SND_ASYNC);
	gotoxy(30, 10); puts("play"); // debug

	free(songPath);
}


// ȭ�鿡 �ִ� ��� ��Ʈ�� �� ĭ�� �Ʒ��� �̵���Ų��. (������)
// ��Ʈ�� �����, ȭ�� ������ ������ ���ش�.
void fallingNote() {
	static clock_t timer = 0;
	if (timer == 0)
		timer = clock();

	// ��� ��Ʈ�� ��������� �� �ִٰ� ������ �����Ű�� ���� Ÿ�̸�
	static clock_t endTimer = 0;
	static BOOL end = FALSE;

	if (clock() - timer >= FALLSPEED) {

		// MISS ��Ʈ �˻�
		for (int i = 0; i < LINE; i++) {
			if (note[HEI-1][i] == N) {
				gotoxy(i*NOTETHK+1, HEI+1);
				puts("miss");
				shouldRemove[i] = 1;
			}
		}

		// ������ �� ĭ�� ������
		for (int i = 1; i < HEI; i++) {
			for (int j = 0; j < LINE; j++) {
				note[HEI-i][j] = note[HEI-(i+1)][j];

				// ��Ʈ�� ó�� �������� ����� �� BGM ���
				if (!songPlayed && i == 2 && note[HEI - i][j] == N) {
					playSong();
					songPlayed = TRUE;
				}
			}
		}

		// �� �� �ٿ� ���� ��Ʈ �߰�
		if (mapIndex < mapLength) {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = map[mapIndex][i];
			}
			mapIndex++;

			// ���� ������ ��Ʈ�� ����� �� �ִ� ���� ����
			if (mapIndex == mapLength) {
				end = TRUE;
				endTimer = clock();
			}
		}

		// �߰��� ��Ʈ�� ������ ��ĭ
		else {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = x;
			}
		}

		// ȭ�� ������Ʈ
		showNotes();

		timer = clock();
	}

	// ���� ������ ��Ʈ�� ����� �� �ִ� ���� ����
	if (end) {
		if (clock() - endTimer >= FALLSPEED * HEI + 1000) {
			gameEnd = TRUE;
			end = FALSE;
		}
	}
}

// ��Ʈ + ���� �ܼ� â�� ����Ѵ�.
void showNotes() {
	gotoxy(0, 0);
	for (int i = 0; i < HEI; i++) {
		for (int j = 0; j < LINE; j++) {
			for (int k = 0; k < NOTETHK; k++) {
				_putch(note[i][j]);
			}
		}
		_putch('\n');
	}

	// ������
	for (int i = 0; i < LINE; i++) {
		gotoxy(i * NOTETHK, HEI - 2);
		if (note[HEI - 2][i] == x) {
			for (int j = 0; j < NOTETHK; j++)
				_putch('-');
		}
	}
}

// Ű���� �Է��� �����Ѵ�.
void keyInput() {
	static int k;

	if (_kbhit()) {
		k = _getch();
		if (k == 0xE0 || k == 0)
			k = _getch();

		for (int i = 0; i < LINE; i++) {
			if (k == key[i]) {
				press(i);
			}
		}
	}
}

// key�� ������ �� ȣ��ȴ�.
void press(int line) {
	for (int i = 1; i <= 3; i++) {
		if (note[HEI - i][line] == N) {
			hitNote(line, i);
			return;
		}
	}
}

// �ش� line�� ��Ʈ�� ģ��.
// judgement: 1 -> LATE, 2 -> GOOD, 3 -> FAST
void hitNote(int line, int judgement) {

	// �ش� ��Ʈ ����
	note[HEI-judgement][line] = x;


	// ����
	switch (judgement) {
		case 1: score += 200; break; // LATE
		case 2: score += 300; break; // GOOD
		case 3: score += 200; break; // FAST
	}
	gotoxy(LINE * NOTETHK + 2, 0);
	printf("%-d", score);


	// ���� �ؽ�Ʈ
	gotoxy(line * NOTETHK + 1, HEI + 1);
	switch (judgement) {
		case 1: puts("LATE"); break;
		case 2: puts("GOOD"); break;
		case 3: puts("FAST"); break;
	}
	shouldRemove[line] = 1;


	// ȭ�� ������Ʈ
	showNotes();
}


// ��Ʈ�� ġ�� 1�� ���Ŀ� ���� �ؽ�Ʈ ���� (������)
void removingJudgeTxt() {
	static clock_t timer[LINE] = {0,};

	for (int i = 0; i < LINE; i++) {
		if (shouldRemove[i] == 1) {
			timer[i] = clock();
			shouldRemove[i] = 2;
		}

		if (shouldRemove[i] == 2 && clock() - timer[i] >= 1000) {
			gotoxy(i * NOTETHK + 1, HEI + 1);
			puts("    ");
			shouldRemove[i] = 0;
		}
	}
	
}


// �÷��� ���� & ���� ����
void showScore() {
	gotoxy(2, HEI/2-1);
	printf("����: %d��", score);
	gotoxy(2, HEI/2);
	printf("���� ȭ������ ���ư����� �ƹ� Ű�� ��������.");
}
