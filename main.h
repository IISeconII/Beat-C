/* main.h */
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include "fun.h"

#define SCREEN_WIDTH 120 // �ܼ� â ���� ����
#define SCREEN_HEIGHT 30 // �ܼ� â ���� ����

#define LINE 4 // ��Ʈ ������ �� = ������ Ű�� ����


int key[LINE]; // �� ������ ������ Ű ���

const char* mapPath; // ���� ����� ���� ���


void keysetting_main();
void title_main();
char* selectmap_main();
void gameplay_main(char* _mapName);
