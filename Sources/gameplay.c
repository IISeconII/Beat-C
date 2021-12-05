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
	pauseTimer = 0;
	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);
	
	system("cls");
}

// ��Ʈ �� ������ �о note�� �����Ѵ�.
int readNoteMapFile() {
	FILE *f;

	// �� ���� ��ġ ���� (maps/mapName/)
	const int mapDirSize = (int)strlen(mapFolder) + 1 + (int)strlen(mapName) + 1 + 1;
	mapDir = malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s/%s/", mapFolder, mapName);

	// info.json ��� ���� (maps/mapName/info.json)
	const char infoStr[] = "info.json";
	const int infoPathSize = mapDirSize + (int)strlen(infoStr);
	char* infoPath = malloc(infoPathSize);
	sprintf_s(infoPath, infoPathSize, "%s%s", mapDir, infoStr);

	// info.json ��ü ����
	JSON_Value *jsonValue = json_parse_file(infoPath);
	if (jsonValue == NULL) return -1;
	mapInfo = json_value_get_object(jsonValue);

	// �� ���� ��� ���� (maps/mapName/mapName.txt)
	const char* noteFile = json_object_get_string(mapInfo, "mapFile");
	const int notePathSize = mapDirSize + (int)strlen(noteFile);
	char* notePath = malloc(notePathSize);
	if (notePath == NULL) return -1;
	sprintf_s(notePath, notePathSize, "%s%s", mapDir, noteFile);

	// �� ���� �б�
	mapLength = (int)json_object_get_number(mapInfo, "mapLength");
	FALLSPEED = (int)json_object_get_number(mapInfo, "fallSpeed");

	if (FALLSPEED == 0) { // FALLSPEED�� ������� �ʾ��� ��� BPM���κ��� ���� (16��Ʈ ����)
		const double bpm = (int)json_object_get_number(mapInfo, "bpm");
		FALLSPEED = (int)round(60 / bpm / 4 * 1000);
	}

	// ��Ʈ �о map�� �ֱ�
	map = malloc(mapLength * sizeof(char*));
	if (map == NULL) return -1;
	fopen_s(&f, notePath, "r");
	if (f == NULL) return -1;

	// �� �پ� �б�
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

	// ����
	fclose(f);
	free(mapDir); free(infoPath); free(notePath);
	return 0;
}


// ȭ�� ���� �׸���.
void drawScreen() {

	// �ڽ�
	for (int i = 0; i <= HEI; i++) {
		gotoxy(glp - 1, gtp + i);

		_putch('|');

		for (int j = 0; j < LINE * NOTETHK; j++) {
			if (i == HEI-2) {
				wprintf(L"��"); j++; // ������
			}
			else if (i == HEI) {
				_putch('^'); // �������
			}
			else {
				_putch(' '); // ��Ʈ �ִ� ��ĭ
			}
		}

		_putch('|');
	}

	// Ű
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK + 1, gtp + HEI + 2);
		wprintf(L"%s", keyName[i]);
	}

	// �� ����
	gotoxy(glp - 6 - (int)strlen(mapName), gtp + 1);
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

	// Ű
	gotoxy(glp + 1, gtp + HEI + 2);
	for (int i = 0; i < LINE * NOTETHK; i++) {
		_putch(' ');
	}
}


// ȭ�鿡 �ִ� ��� ��Ʈ�� �� ĭ�� �Ʒ��� �̵���Ų��. (������)
// ��Ʈ�� �����, ȭ�� ������ ������ ���ش�.
void fallingNote() {
	static clock_t timer = 0;
	if (timer == 0) timer = clock();
	static clock_t runtime = 0;
	if (runtime == 0) runtime = FALLSPEED;

	// ��� ��Ʈ�� ��������� �� �ִٰ� ������ �����Ű�� ���� Ÿ�̸�
	static clock_t endTimer = 0;
	static BOOL end = FALSE;

	if (clock() - (timer + pauseTimer) >= runtime) {

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

		runtime += FALLSPEED;
	}

	// ���� ������ ��Ʈ�� ����� �� �ִ� ���� ����
	if (end) {
		if (clock() - (endTimer + pauseTimer) >= FALLSPEED * HEI + 1000) {
			timer = 0;
			runtime = 0;
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
			for (int k = 0; k < NOTETHK / 2; k++) {
				wprintf(L"%ws", note[i][j] == N ? L"��" : L"  ");
			}
		}
	}

	// ������
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
		if (note[HEI - 2][i] == x) {
			for (int j = 0; j < NOTETHK / 2; j++) {
				if (isPressed[i])
					wprintf(L"��");
				else
					wprintf(L"��");
			}
				
		}
	}
}

// Ű���� �Է��� �����Ѵ�.
void keyInput() {

	if (_kbhit()) {
		for (int i = 0; i < LINE; i++) {
			if (GetAsyncKeyState(key[i])) {
				if (!isPressed[i]) { // ����
					press(i);

					gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
					for (int j = 0; j < NOTETHK / 2; j++)
						wprintf(L"��");

					isPressed[i] = TRUE;
				}
			}
			else { // ��
				gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
				for (int j = 0; j < NOTETHK / 2; j++)
					wprintf(L"��");

				isPressed[i] = FALSE;
			}
		}
	}

	/*static int k;
	if (_kbhit()) {
		k = _getch();
		if (k == 0xE0 || k == 0)
			k = _getch();

		switch (k) {
			case ESC: pause(); break; // ESC -> �Ͻ�����
		}
	}*/
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


// �Ͻ�����
void pause() {
	// ����
	if (!paused) {
		paused = TRUE;
		clock_t pauseStart = clock();

		// â Ŭ����
		drawScreen();
		gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI/2-1); puts("Paused");

		// ESC�� ���� ������ ���
		int k = 0;
		while (k != ESC) {
			k = _getch();
			if (k == 0xE0 || k == 0)
				k = _getch();
		}
		
		paused = FALSE;
		gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI/2-1); puts("      ");
		countdown();
		pauseTimer += clock() - pauseStart; // �Ͻ����� ���̾��� �ð��� ����
	}
}


// �뷡(BGM)�� ����Ѵ�.
void playSong() {
	/*const char* songName = json_object_get_string(mapInfo, "songFile");
	const int pathSize = (int)strlen(mapDir) + (int)strlen(songName) + 1;
	char* songPath = malloc(pathSize);
	if (songPath == NULL) return;
	sprintf_s(songPath, pathSize, "%s%s", mapDir, songName);

	PlaySound(songPath, 0, SND_ASYNC);
	gotoxy(30, 10); puts("play"); // debug

	free(songPath);*/
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
	/*gotoxy(glp + LINE * NOTETHK + 2, gtp + 6);
	printf("%d", noteCount);*/
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

	// ���� ����
	JSON_Value *rootValue = json_parse_file(hsfPath);
	JSON_Object *highScore = json_value_get_object(rootValue);
	if (json_object_get_number(highScore, mapName) < score) { // ���� ���ھ ������ ����
		json_object_set_number(highScore, mapName, score);
		json_serialize_to_file_pretty(rootValue, hsfPath);
	}
}
