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


// 변수+a 초기화
void init()
{
	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);
	score = 0;
	mapIndex = 0;
	songPlayed = FALSE;
	gameEnd = FALSE;

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
	system("cls");

	// 양옆 박스
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp - 1, i + gtp); _putch('|');
		gotoxy(LINE*NOTETHK + glp, i + gtp); _putch('|');
	}

	// 판정선
	gotoxy(glp - 1, HEI-2 + gtp);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('-');
	}

	// 데드라인
	gotoxy(glp - 1, HEI + gtp);
	for (int j = 0; j <= LINE * NOTETHK + 1; j++) {
		_putch('^');
	}
}

// 카운트다운
void countdown() {
	for (int i = 3; i >= 1; i--) {
		gotoxy(LINE*NOTETHK/2 + glp, HEI/2-1 + gtp); // 맵 중앙
		printf("%d", i);
		Sleep(500);
	}

	gotoxy(LINE*NOTETHK/2-3 + glp, HEI/2-1 + gtp);
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

		// MISS 노트 검사
		for (int i = 0; i < LINE; i++) {
			if (note[HEI-1][i] == N) {
				gotoxy(i*NOTETHK+1 + glp, HEI+1 + gtp);
				puts("miss");
				shouldRemove[i] = 1;
			}
		}

		// 밑으로 한 칸씩 내리기
		for (int i = 1; i < HEI; i++) {
			for (int j = 0; j < LINE; j++) {
				note[HEI-i][j] = note[HEI-(i+1)][j];

				// 노트가 처음 판정선에 닿았을 때 BGM 재생
				if (!songPlayed && i == 2 && note[HEI - i][j] == N) {
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
			gameEnd = TRUE;
			end = FALSE;
		}
	}
}

// 노트 + 맵을 콘솔 창에 출력한다.
void showNotes() {
	for (int i = 0; i < HEI; i++) {
		gotoxy(glp, i + gtp);
		for (int j = 0; j < LINE; j++) {
			for (int k = 0; k < NOTETHK; k++) {
				_putch(note[i][j]);
			}
		}
	}

	// 판정선
	for (int i = 0; i < LINE; i++) {
		gotoxy(i*NOTETHK + glp, HEI-2 + gtp);
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
	for (int i = 1; i <= 3; i++) {
		if (note[HEI - i][line] == N) {
			hitNote(line, i);
			return;
		}
	}
}

// 해당 line의 노트를 친다.
// judgement: 1 -> LATE, 2 -> GOOD, 3 -> FAST
void hitNote(int line, int judgement) {

	// 해당 노트 삭제
	note[HEI-judgement][line] = x;


	// 점수
	switch (judgement) {
		case 1: score += 200; break; // LATE
		case 2: score += 300; break; // GOOD
		case 3: score += 200; break; // FAST
	}
	gotoxy(LINE*NOTETHK+2 + glp, gtp);
	printf("%-d", score);


	// 판정 텍스트
	gotoxy(line*NOTETHK+1 + glp, HEI+1 + gtp);
	switch (judgement) {
		case 1: puts("LATE"); break;
		case 2: puts("GOOD"); break;
		case 3: puts("FAST"); break;
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
			gotoxy(i*NOTETHK+1 + glp, HEI+1 + gtp);
			puts("    ");
			shouldRemove[i] = 0;
		}
	}
	
}


// 플레이 종료 & 점수 띄우기
void showScore() {
	gotoxy(2 + glp, HEI/2-1 + gtp);
	printf("점수: %d점", score);
	gotoxy(2 + glp, HEI/2 + gtp);
	printf("메인 화면으로 돌아가려면 아무 키나 누르세요.");
}
