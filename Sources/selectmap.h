/* selectmap.h */
#pragma once

#include <io.h>
#include "main.h"
#include "parson.h"


int mapCount; // 맵의 개수
char** mapList; // 맵 선택 창에서 띄울 맵 목록

const int slp = 34; // selectmap left padding
const int stp = 3; // selectmap top padding

static char mapGuideDocs[][82] = { // 맵 제작법 텍스트
	":( 맵이 없네요..",
	"",
	"맵 제작법:",
	"- 프로그램이 있는 경로에 maps 폴더를 만듭니다. 맵의 기본 경로입니다.",
	"- 폴더 안에 맵 이름으로 된 폴더를 만듭니다. 이곳에 맵이 담깁니다.",
	"- 그 안에 맵이름.txt 파일을 만듭니다. 여기에 노트 데이터가 들어갑니다.",
	"",
	"노트 데이터는 다음과 같은 형태입니다.",
	"",
	"@... @는 노트이고, .은 공백 문자(' ')입니다.",
	".... (점('.')은 보여주기 위해서 공백 대신 쓴 겁니다)",
	".@.. 게임이 시작되면 맨 윗 줄부터 노트 문자를 읽어서 게임 화면에 노트를 띄웁니다.",
	".... // 작성 중",
	"..@.",
	"....",
	"...@",
};

char* hsfPath; // stats.dat 파일 경로 (JSON) ("maps/stats.dat")


char* main_selectmap();
int selecting(int mapCount);
int loadMaps();
