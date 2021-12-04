/* gameplay.c */

#include "gameplay.h"


// ������ �÷����ϴ� ��
void main_gameplay(char* _mapName) {
	mapName = _mapName;

	init();

	if (readNoteMapFile() == -1) {
		system("cls");
		gotoxy(1, 1);
		puts("�� ������ �������� �ʰų� ���� �� �� �����ϴ�.");
		Sleep(1000);
		return;
	}

	drawScreen();
	countdown();
	updateUI(0);

	while (!gameEnd) {
		fallingNote();
		keyInput();
		removingJudgeTxt();
	}

	clearBuffer();
	showStats();
	while (!_kbhit()) removingJudgeTxt();


	free(mapDir);
	for (int i = 0; i < mapLength; i++) free(map[i]); free(map);
}


// ���� �ʱ�ȭ + cls
void init()
{
	mapIndex = 0;
	songPlayed = FALSE;
	gameEnd = FALSE;
	score = 0;
	combo = 0;
	accuracy = 100;
	noteCount = 0;
	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);

	system("cls");
}

// ��Ʈ �� ������ �о note�� �����Ѵ�.
int readNoteMapFile() {
	FILE *f;

	// �� ���� ��� ���� (maps/mapName/)
	const int mapDirSize = (int)strlen(mapPath) + (int)strlen(mapName) + 1 + 1;
	mapDir = malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s%s/", mapPath, mapName);

	// �� ���� ��� ���� (maps/mapName/mapName.txt)
	const char* extension = ".txt";
	const int pathSize = mapDirSize + (int)strlen(mapName) + (int)strlen(extension);
	char* path = malloc(pathSize);
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
	map = malloc(mapLength * sizeof(char*));
	if (map == NULL) return -1;
	char* line;

	for (int i = 0; i < mapLength; i++) {

		line = malloc(LINE * sizeof(char));
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

	// �翷 �ڽ�
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp - 1, gtp + i); _putch('|');
		gotoxy(glp + LINE*NOTETHK, gtp + i); _putch('|');
	}

	// ������
	gotoxy(glp - 1, gtp + HEI-2);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('-');
	}

	// �������
	gotoxy(glp - 1, gtp + HEI);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('^');
	}

	// �� ����
	gotoxy(glp-6-(int)strlen(mapName), gtp+1);
	printf("< %s >", mapName);
}

// ī��Ʈ�ٿ�
void countdown() {
	for (int i = 3; i >= 1; i--) {
		gotoxy(glp + LINE*NOTETHK/2, gtp + HEI/2-1); // �� �߾�
		printf("%d", i);
		Sleep(500);
	}

	gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI / 2 - 1);
	puts("Start!");
	Sleep(500);
}

// �뷡(BGM)�� ����Ѵ�.
void playSong() {
	const char extension[] = ".wav";
	const int pathSize = (int)strlen(mapDir) + (int)strlen(mapName) + sizeof(extension);
	char* songPath = malloc(pathSize);
	if (songPath == NULL) return;
	sprintf_s(songPath, pathSize, "%s%s%s", mapDir, mapName, extension);

	PlaySound((wchar_t*)songPath, 0, SND_ASYNC);
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

		// miss ��Ʈ �˻�
		for (int i = 0; i < LINE; i++) {
			if (note[HEI-1][i] == N) {
				hitNote(i, -1);
			}
		}

		// ������ �� ĭ�� ������
		for (int i = 1; i < HEI; i++) {
			for (int j = 0; j < LINE; j++) {
				note[HEI-i][j] = note[HEI-(i+1)][j];

				// ��Ʈ�� ó�� �������� ����� �� BGM ���
				if (!songPlayed && i == 2 && note[HEI-i][j] == N) {
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

				gotoxy(30, 10); puts("end"); // debug

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

			gotoxy(30, 11); puts("gameEnd"); // debug

			gameEnd = TRUE;
			end = FALSE;
		}
	}
}

// ��Ʈ + ���� �ܼ� â�� ����Ѵ�.
void showNotes() {
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp, gtp + i);
		for (int j = 0; j < LINE; j++) {
			for (int k = 0; k < NOTETHK/2; k++) {
				wprintf(L"%ws", note[i][j] == N ? L"��" : L"  ");
			}
		}
	}

	// ������
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
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
	for (int i = 1; i <= 4; i++) {
		if (note[HEI - i][line] == N) {
			hitNote(line, i);
			return;
		}
	}
}

// �ش� line�� ��Ʈ�� ģ��.
// judgement :
//   1 -> LATE
//   2 -> GOOD
//   3 -> FAST
//   4 -> miss(TOOFAST)
//  -1 -> miss(TOOLATE)
void hitNote(int line, int judgement) {

	// �ش� ��Ʈ ����
	if (judgement != -1) note[HEI-judgement][line] = x;
	noteCount++;


	// ����, ��Ȯ��
	switch (judgement) {
		case 1: // LATE
		case 3: // FAST (200���� 66%)
			score += 200 + (int)round(200 * combo / 100.0);
			accuracy = (accuracy * (noteCount-1) + 200/3.0) / noteCount;
			updateUI(+1);
			break;
		case 2: // GOOD (300���� 100%)
			score += 300 + (int)round(300 * combo / 100.0);
			accuracy = (accuracy * (noteCount-1) + 100) / noteCount;
			updateUI(+1);
			break;
		case 4:
		case -1: // miss (0���� 0%)
			score += 0;
			accuracy = (accuracy * (noteCount - 1) + 0) / noteCount;
			updateUI(-1);
			break;
	}


	// ���� �ؽ�Ʈ
	gotoxy(glp + line*NOTETHK+1, gtp + HEI+1);
	switch (judgement) {
		case 1: puts("LATE"); break;
		case 2: puts("GOOD"); break;
		case 3: puts("FAST"); break;
		case 4: case -1: puts("miss"); break;
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
			gotoxy(glp + i*NOTETHK+1, gtp + HEI+1);
			puts("    ");
			shouldRemove[i] = 0;
		}
	}
	
}


// ���� & �޺� UI ������Ʈ
// 1: �޺� ����, -1: �޺� �ʱ�ȭ
void updateUI(int comboPlus) {

	// ����
	gotoxy(glp + LINE * NOTETHK + 2, gtp);
	printf("Score: %d", score);

	// �޺�
	if (comboPlus == 1) {
		combo++;
	} else if (comboPlus == -1) {
		combo = 0;
	}
	gotoxy(glp + LINE*NOTETHK + 2, gtp + 2);
	printf("Combo: %-4d", combo);

	// ��Ȯ��
	gotoxy(glp + LINE * NOTETHK + 2, gtp + 4);
	printf("Accuracy: %-3.3f%%", accuracy);

	// [debug] noteCount
	gotoxy(glp + LINE * NOTETHK + 2, gtp + 6);
	printf("%d", noteCount);
}


// ��� ����
void showStats() {
	gotoxy(glp + 1, gtp + HEI/2-3);
	printf("%s �÷��� ���", mapName);
	gotoxy(glp + 1, gtp + HEI/2-1);
	printf("����: %d��", score);
	gotoxy(glp + 1, gtp + HEI/2);
	printf("��Ȯ��: %.3f%%", accuracy);
	gotoxy(glp + 1, gtp + HEI/2+2);
	printf("���� ȭ������ ���ư�����");
	gotoxy(glp + 1, gtp + HEI/2+3);
	printf("�ƹ� Ű�� ��������");
}
