/* main.h */
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include "fun.h"

#define SCREEN_WIDTH 120 // 콘솔 창 가로 길이
#define SCREEN_HEIGHT 30 // 콘솔 창 세로 길이

#define LINE 4 // 노트 라인의 수 = 누르는 키의 개수


int key[LINE]; // 각 라인을 누르는 키 목록
const char* mapPath; // 맵이 저장된 폴더 경로


// selectmap.h
extern int mapCount;
extern char** mapList;


void main_keysetting();
void main_title();
char* main_selectmap();
void main_gameplay(char* _mapName);
