/* selectmap.h */
#pragma once

#include <io.h>
#include "main.h"
#include "parson.h"


int mapCount; // ���� ����
char** mapList; // �� ���� â���� ��� �� ���

const int slp = 32; // selectmap left padding
const int stp = 2; // selectmap top padding

static char mapGuideDocs[][82] = { // �� ���۹� �ؽ�Ʈ
	":( ���� ���׿�..",
	"",
	"�� ���۹�:",
	"- ���α׷��� �ִ� ��ο� maps ������ ����ϴ�. ���� �⺻ ����Դϴ�.",
	"- ���� �ȿ� �� �̸����� �� ������ ����ϴ�. �̰��� ���� ���ϴ�.",
	"- �� �ȿ� ���̸�.txt ������ ����ϴ�. ���⿡ ��Ʈ �����Ͱ� ���ϴ�.",
	"",
	"��Ʈ �����ʹ� ������ ���� �����Դϴ�.",
	"",
	"@... @�� ��Ʈ�̰�, .�� ���� ����(' ')�Դϴ�.",
	".... (��('.')�� �����ֱ� ���ؼ� ���� ��� �� �̴ϴ�)",
	".@.. ������ ���۵Ǹ� �� �� �ٺ��� ��Ʈ ���ڸ� �о ���� ȭ�鿡 ��Ʈ�� ���ϴ�.",
	".... // �ۼ� ��",
	"..@.",
	"....",
	"...@",
};

char* statsPath; // stats.dat ���� ��� (JSON) ("maps/stats.dat")


char* main_selectmap();
int loadMaps();
int selecting(int mapCount);
void blink(int pointer);
