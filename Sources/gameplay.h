/* gameplay.h */
#pragma once
#pragma comment(lib,"winmm.lib")

#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include <wchar.h>
#include <math.h>
#include "parson.h"
#include "main.h"


#define NOTETHK 6 // note thickness; ��Ʈ �ϳ��� �β� (���� ����)
#define HEI 20 // height; â ���� (�ؿ��� �� ��° ���� ����Ʈ ������)

#define N '@' // ��Ʈ
#define x ' ' // ��Ʈ ����


const int glp = (SCREEN_WIDTH/2) - (LINE/2 * NOTETHK); // gameplay left padding
const int gtp = 0; // gameplay top padding

char* mapName; // �� �̸�
char* mapDir; // �� ���� ���
JSON_Object* mapInfo; // info.json ������
char** map; // �� (��Ʈ�� ������ ����)
int mapLength; // ������ ���� (map�� �� ����)
int mapIndex; // ���� ���� �� ��° ���� �а� �ִ°�?

char note[HEI][LINE]; // ȭ�鿡 ���̴� ��� ��Ʈ ����Ʈ (= ȭ��)
int FALLSPEED = 150; // ��Ʈ�� �������� �ӵ�(ms), FALLSPEED�ʿ� �� ĭ�� ������

int score; // ����
int combo; // �޺�
double accuracy; // ��Ȯ��
int noteCount; // ��Ʈ�� ������ Ƚ�� (���� ��꿡 ���)

BOOL songPlayed; // BGM�� ����ƴ°�?
BOOL gameEnd; // ������ �����°�?

// ���� �ؽ�Ʈ�� ������ �ϴ°�? (removingJudgeTxt() ����)
// 0: false, 1: ���� ��ȣ, 2: ���� Ÿ�̸� �۵� ��
int shouldRemove[LINE];

// gameplay.h
extern char* hsfPath;


void main_gameplay(char* _mapName);

void init();
int readNoteMapFile();

void drawScreen();
void countdown();

void fallingNote();
void showNotes();
void keyInput();
void press(int line);
void hitNote(int line, int judgement);
void removingJudgeTxt();

void playSong();
void updateUI(int comboPlus);

void showStats();
