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
	pauseTimer = 0;
	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);
	
	system("cls");
}

// 노트 맵 파일을 읽어서 note에 저장한다.
int readNoteMapFile() {
	FILE *f;

	// 맵 폴더 위치 설정 (maps/mapName/)
	const int mapDirSize = (int)strlen(mapFolder) + 1 + (int)strlen(mapName) + 1 + 1;
	mapDir = malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s/%s/", mapFolder, mapName);

	// info.json 경로 설정 (maps/mapName/info.json)
	const char infoStr[] = "info.json";
	const int infoPathSize = mapDirSize + (int)strlen(infoStr);
	char* infoPath = malloc(infoPathSize);
	sprintf_s(infoPath, infoPathSize, "%s%s", mapDir, infoStr);

	// info.json 객체 생성
	JSON_Value *jsonValue = json_parse_file(infoPath);
	if (jsonValue == NULL) return -1;
	mapInfo = json_value_get_object(jsonValue);

	// 맵 파일 경로 설정 (maps/mapName/mapName.txt)
	const char* noteFile = json_object_get_string(mapInfo, "mapFile");
	const int notePathSize = mapDirSize + (int)strlen(noteFile);
	char* notePath = malloc(notePathSize);
	if (notePath == NULL) return -1;
	sprintf_s(notePath, notePathSize, "%s%s", mapDir, noteFile);

	// 맵 정보 읽기
	mapLength = (int)json_object_get_number(mapInfo, "mapLength");
	FALLSPEED = (int)json_object_get_number(mapInfo, "fallSpeed");

	if (FALLSPEED == 0) { // FALLSPEED를 명시하지 않았을 경우 BPM으로부터 추출 (16비트 기준)
		const double bpm = (int)json_object_get_number(mapInfo, "bpm");
		FALLSPEED = (int)round(60 / bpm / 4 * 1000);
	}

	// 노트 읽어서 map에 넣기
	map = malloc(mapLength * sizeof(char*));
	if (map == NULL) return -1;
	fopen_s(&f, notePath, "r");
	if (f == NULL) return -1;

	// 한 줄씩 읽기
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

	// 정리
	fclose(f);
	free(mapDir); free(infoPath); free(notePath);
	return 0;
}


// 화면 맵을 그린다.
void drawScreen() {

	// 박스
	for (int i = 0; i <= HEI; i++) {
		gotoxy(glp - 1, gtp + i);

		_putch('|');

		for (int j = 0; j < LINE * NOTETHK; j++) {
			if (i == HEI-2) {
				wprintf(L"□"); j++; // 판정선
			}
			else if (i == HEI) {
				_putch('^'); // 데드라인
			}
			else {
				_putch(' '); // 노트 있는 빈칸
			}
		}

		_putch('|');
	}

	// 키
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK + 1, gtp + HEI + 2);
		wprintf(L"%s", keyName[i]);
	}

	// 곡 제목
	gotoxy(glp - 6 - (int)strlen(mapName), gtp + 1);
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

	// 키
	gotoxy(glp + 1, gtp + HEI + 2);
	for (int i = 0; i < LINE * NOTETHK; i++) {
		_putch(' ');
	}
}


// 화면에 있는 모든 노트를 한 칸씩 아래로 이동시킨다. (스레드)
// 노트를 만들고, 화면 밖으로 나가면 없앤다.
void fallingNote() {
	static clock_t timer = 0;
	if (timer == 0) timer = clock();
	static clock_t runtime = 0;
	if (runtime == 0) runtime = FALLSPEED;

	// 모든 노트가 만들어지고 좀 있다가 게임을 종료시키기 위한 타이머
	static clock_t endTimer = 0;
	static BOOL end = FALSE;

	if (clock() - (timer + pauseTimer) >= runtime) {

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

		runtime += FALLSPEED;
	}

	// 맵의 마지막 노트를 만들면 좀 있다 게임 종료
	if (end) {
		if (clock() - (endTimer + pauseTimer) >= FALLSPEED * HEI + 1000) {
			timer = 0;
			runtime = 0;
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
			for (int k = 0; k < NOTETHK / 2; k++) {
				wprintf(L"%ws", note[i][j] == N ? L"■" : L"  ");
			}
		}
	}

	// 판정선
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
		if (note[HEI - 2][i] == x) {
			for (int j = 0; j < NOTETHK / 2; j++) {
				if (isPressed[i])
					wprintf(L"▣");
				else
					wprintf(L"□");
			}
				
		}
	}
}

// 키보드 입력을 감지한다.
void keyInput() {

	if (_kbhit()) {
		for (int i = 0; i < LINE; i++) {
			if (GetAsyncKeyState(key[i])) {
				if (!isPressed[i]) { // 누름
					press(i);

					gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
					for (int j = 0; j < NOTETHK / 2; j++)
						wprintf(L"▣");

					isPressed[i] = TRUE;
				}
			}
			else { // 뗌
				gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
				for (int j = 0; j < NOTETHK / 2; j++)
					wprintf(L"□");

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
			case ESC: pause(); break; // ESC -> 일시정지
		}
	}*/
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


// 일시정지
void pause() {
	// 정지
	if (!paused) {
		paused = TRUE;
		clock_t pauseStart = clock();

		// 창 클리어
		drawScreen();
		gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI/2-1); puts("Paused");

		// ESC를 누를 때까지 대기
		int k = 0;
		while (k != ESC) {
			k = _getch();
			if (k == 0xE0 || k == 0)
				k = _getch();
		}
		
		paused = FALSE;
		gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI/2-1); puts("      ");
		countdown();
		pauseTimer += clock() - pauseStart; // 일시정지 중이었던 시간을 빼줌
	}
}


// 노래(BGM)를 재생한다.
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
	/*gotoxy(glp + LINE * NOTETHK + 2, gtp + 6);
	printf("%d", noteCount);*/
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

	// 점수 저장
	JSON_Value *rootValue = json_parse_file(hsfPath);
	JSON_Object *highScore = json_value_get_object(rootValue);
	if (json_object_get_number(highScore, mapName) < score) { // 하이 스코어를 넘으면 갱신
		json_object_set_number(highScore, mapName, score);
		json_serialize_to_file_pretty(rootValue, hsfPath);
	}
}
