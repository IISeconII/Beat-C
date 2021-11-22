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


// selectmap.h
extern int mapCount;
extern char** mapList;


void main_keysetting();
void main_title();
char* main_selectmap();
void main_gameplay(char* _mapName);
