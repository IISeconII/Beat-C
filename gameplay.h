/* gameplay.h */
#pragma once
#pragma comment(lib,"winmm.lib")

#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include <wchar.h>
#include <math.h>
#include "main.h"


#define NOTETHK 6 // note thickness; 노트 하나의 두께 (가로 길이)
#define HEI 20 // height; 창 높이 (밑에서 두 번째 줄이 퍼펙트 판정선)
#define FALLSPEED 150 // 노트가 떨어지는 속도(ms), FALLSPEED초에 한 칸씩 떨어짐

#define N '@' // 노트
#define x ' ' // 노트 없음


const int glp = (SCREEN_WIDTH/2) - (LINE/2 * NOTETHK); // gameplay left padding
const int gtp = 0; // gameplay top padding

char* mapDir; // 맵 폴더 경로
char* mapName; // 맵 이름
char** map; // 맵 (노트가 나오는 패턴)
int mapLength; // 패턴의 길이 (map의 행 개수)
int mapIndex; // 현재 맵의 몇 번째 줄을 읽고 있는가?

char note[HEI][LINE]; // 화면에 보이는 모든 노트 리스트 (= 화면)

int score; // 점수
int combo; // 콤보
double accuracy; // 정확도
int noteCount; // 노트가 내려온 횟수 (판정 계산에 사용)

BOOL songPlayed; // BGM이 재생됐는가?
BOOL gameEnd; // 게임이 끝났는가?

// 판정 텍스트를 지워야 하는가? (removingJudgeTxt() 참조)
// 0: false, 1: 제거 신호, 2: 제거 타이머 작동 중
int shouldRemove[LINE];


void main_gameplay(char* _mapName);
void init();
int readNoteMapFile();
void drawScreen();
void countdown();
void playSong();

void fallingNote();
void showNotes();

void keyInput();
void press(int line);
void hitNote(int line, int judgement);


void removingJudgeTxt();

void updateUI(int comboPlus);

void showStats();
