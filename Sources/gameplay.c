/* gameplay.c */

#include "gameplay.h"


// 게임을 플레이하는 씬
void main_gameplay(char* _mapName) {
	mapName = _mapName;

	init();

	if (readNoteMapFile() == -1) {
		system("cls");
		gotoxy(1, 1);
		puts("맵 파일이 존재하지 않거나 맵을 열 수 없습니다.");
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


// 변수 초기화 + cls
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

// 노트 맵 파일을 읽어서 note에 저장한다.
int readNoteMapFile() {
	FILE *f;

	// 맵 폴더 경로 설정 (maps/mapName/)
	const int mapDirSize = (int)strlen(mapPath) + (int)strlen(mapName) + 1 + 1;
	mapDir = malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s%s/", mapPath, mapName);

	// 맵 파일 경로 설정 (maps/mapName/mapName.txt)
	const char* extension = ".txt";
	const int pathSize = mapDirSize + (int)strlen(mapName) + (int)strlen(extension);
	char* path = malloc(pathSize);
	if (path == NULL) return -1;
	sprintf_s(path, pathSize, "%s%s%s", mapDir, mapName, extension);


	// 줄 개수 세기
	mapLength = 0;
	fopen_s(&f, path, "r");
	if (f == NULL) return -1;

	while (!feof(f)) {
		if (fgetc(f) == '\n')
			mapLength++;
	} mapLength++;

	fseek(f, 0, SEEK_SET);

	
	// 맵 읽어서 map에 넣기
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

// 화면 맵을 그린다.
void drawScreen() {

	// 양옆 박스
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp - 1, gtp + i); _putch('|');
		gotoxy(glp + LINE*NOTETHK, gtp + i); _putch('|');
	}

	// 판정선
	gotoxy(glp - 1, gtp + HEI-2);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('-');
	}

	// 데드라인
	gotoxy(glp - 1, gtp + HEI);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('^');
	}

	// 곡 제목
	gotoxy(glp-6-(int)strlen(mapName), gtp+1);
	printf("< %s >", mapName);
}

// 카운트다운
void countdown() {
	for (int i = 3; i >= 1; i--) {
		gotoxy(glp + LINE*NOTETHK/2, gtp + HEI/2-1); // 맵 중앙
		printf("%d", i);
		Sleep(500);
	}

	gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI / 2 - 1);
	puts("Start!");
	Sleep(500);
}

// 노래(BGM)를 재생한다.
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


// 화면에 있는 모든 노트를 한 칸씩 아래로 이동시킨다. (스레드)
// 노트를 만들고, 화면 밖으로 나가면 없앤다.
void fallingNote() {
	static clock_t timer = 0;
	if (timer == 0)
		timer = clock();

	// 모든 노트가 만들어지고 좀 있다가 게임을 종료시키기 위한 타이머
	static clock_t endTimer = 0;
	static BOOL end = FALSE;

	if (clock() - timer >= FALLSPEED) {

		// miss 노트 검사
		for (int i = 0; i < LINE; i++) {
			if (note[HEI-1][i] == N) {
				hitNote(i, -1);
			}
		}

		// 밑으로 한 칸씩 내리기
		for (int i = 1; i < HEI; i++) {
			for (int j = 0; j < LINE; j++) {
				note[HEI-i][j] = note[HEI-(i+1)][j];

				// 노트가 처음 판정선에 닿았을 때 BGM 재생
				if (!songPlayed && i == 2 && note[HEI-i][j] == N) {
					playSong();
					songPlayed = TRUE;
				}
			}
		}

		// 맨 윗 줄에 새로 노트 추가
		if (mapIndex < mapLength) {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = map[mapIndex][i];
			}
			mapIndex++;

			// 맵의 마지막 노트를 만들면 좀 있다 게임 종료
			if (mapIndex == mapLength) {

				gotoxy(30, 10); puts("end"); // debug

				end = TRUE;
				endTimer = clock();

			}
		}

		// 추가할 노트가 없으면 빈칸
		else {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = x;
			}
		}

		// 화면 업데이트
		showNotes();

		timer = clock();
	}

	// 맵의 마지막 노트를 만들면 좀 있다 게임 종료
	if (end) {
		if (clock() - endTimer >= FALLSPEED * HEI + 1000) {

			gotoxy(30, 11); puts("gameEnd"); // debug

			gameEnd = TRUE;
			end = FALSE;
		}
	}
}

// 노트 + 맵을 콘솔 창에 출력한다.
void showNotes() {
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp, gtp + i);
		for (int j = 0; j < LINE; j++) {
			for (int k = 0; k < NOTETHK/2; k++) {
				wprintf(L"%ws", note[i][j] == N ? L"■" : L"  ");
			}
		}
	}

	// 판정선
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
		if (note[HEI - 2][i] == x) {
			for (int j = 0; j < NOTETHK; j++)
				_putch('-');
		}
	}
}

// 키보드 입력을 감지한다.
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

// key가 눌렸을 때 호출된다.
void press(int line) {
	for (int i = 1; i <= 4; i++) {
		if (note[HEI - i][line] == N) {
			hitNote(line, i);
			return;
		}
	}
}

// 해당 line의 노트를 친다.
// judgement :
//   1 -> LATE
//   2 -> GOOD
//   3 -> FAST
//   4 -> miss(TOOFAST)
//  -1 -> miss(TOOLATE)
void hitNote(int line, int judgement) {

	// 해당 노트 삭제
	if (judgement != -1) note[HEI-judgement][line] = x;
	noteCount++;


	// 점수, 정확도
	switch (judgement) {
		case 1: // LATE
		case 3: // FAST (200점에 66%)
			score += 200 + (int)round(200 * combo / 100.0);
			accuracy = (accuracy * (noteCount-1) + 200/3.0) / noteCount;
			updateUI(+1);
			break;
		case 2: // GOOD (300점에 100%)
			score += 300 + (int)round(300 * combo / 100.0);
			accuracy = (accuracy * (noteCount-1) + 100) / noteCount;
			updateUI(+1);
			break;
		case 4:
		case -1: // miss (0점에 0%)
			score += 0;
			accuracy = (accuracy * (noteCount - 1) + 0) / noteCount;
			updateUI(-1);
			break;
	}


	// 판정 텍스트
	gotoxy(glp + line*NOTETHK+1, gtp + HEI+1);
	switch (judgement) {
		case 1: puts("LATE"); break;
		case 2: puts("GOOD"); break;
		case 3: puts("FAST"); break;
		case 4: case -1: puts("miss"); break;
	}
	shouldRemove[line] = 1;


	// 화면 업데이트
	showNotes();
}

// 노트를 치고 1초 이후에 판정 텍스트 제거 (스레드)
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


// 점수 & 콤보 UI 업데이트
// 1: 콤보 증가, -1: 콤보 초기화
void updateUI(int comboPlus) {

	// 점수
	gotoxy(glp + LINE * NOTETHK + 2, gtp);
	printf("Score: %d", score);

	// 콤보
	if (comboPlus == 1) {
		combo++;
	} else if (comboPlus == -1) {
		combo = 0;
	}
	gotoxy(glp + LINE*NOTETHK + 2, gtp + 2);
	printf("Combo: %-4d", combo);

	// 정확도
	gotoxy(glp + LINE * NOTETHK + 2, gtp + 4);
	printf("Accuracy: %-3.3f%%", accuracy);

	// [debug] noteCount
	gotoxy(glp + LINE * NOTETHK + 2, gtp + 6);
	printf("%d", noteCount);
}


// 기록 띄우기
void showStats() {
	gotoxy(glp + 1, gtp + HEI/2-3);
	printf("%s 플레이 결과", mapName);
	gotoxy(glp + 1, gtp + HEI/2-1);
	printf("점수: %d점", score);
	gotoxy(glp + 1, gtp + HEI/2);
	printf("정확도: %.3f%%", accuracy);
	gotoxy(glp + 1, gtp + HEI/2+2);
	printf("메인 화면으로 돌아가려면");
	gotoxy(glp + 1, gtp + HEI/2+3);
	printf("아무 키나 누르세요");
}
