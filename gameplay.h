/* gameplay.h */
#pragma once
#pragma comment(lib,"winmm.lib")

#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include "main.h"


#define NOTETHK 6 // note thickness; ��Ʈ �ϳ��� �β� (���� ����)
#define HEI 20 // height; â ���� (�ؿ��� �� ��° ���� ����Ʈ ������)
#define FALLSPEED 150 // ��Ʈ�� �������� �ӵ�(ms), FALLSPEED�ʿ� �� ĭ�� ������

#define N '@' // ��Ʈ
#define x ' ' // ��Ʈ ����


char* mapDir; // �� ���� ���
char* mapName; // �� �̸�
char** map; // �� (��Ʈ�� ������ ����)
int mapLength; // ������ ���� (map�� �� ����)
int mapIndex; // ���� ���� �� ��° ���� �а� �ִ°�?

char note[HEI][LINE]; // ȭ�鿡 ���̴� ��� ��Ʈ ����Ʈ (= ȭ��)
int score; // ����

BOOL songPlayed; // BGM�� ����ƴ°�?
BOOL gameEnd; // ������ �����°�?

// ���� �ؽ�Ʈ�� ������ �ϴ°�? (removingJudgeTxt() ����)
// 0: false, 1: ���� ��ȣ, 2: ���� Ÿ�̸� �۵� ��
int shouldRemove[LINE];


void gameplay_main(char* _mapName);
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

void showScore();
