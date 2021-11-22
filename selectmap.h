/* selectmap.h */
#pragma once

#include <conio.h>
#include <io.h>
#include "main.h"


char** mapList; // ¸Ê ¼±ÅÃ Ã¢¿¡¼­ ¶ç¿ï ¸Ê ¸ñ·Ï

const int lp = 30; // left padding
const int tp = 3; // top padding


char* selectmap_main();
int selecting(int mapCount);
int loadMaps(int* mapCount);
