#pragma once

#define MAPSZ 3999

extern unsigned char g_map[];
extern const char g_dirs[];
extern unsigned int g_mapw, g_maph;

bool inMapBounds(int x, int y);
bool tileBlocks(int t);
bool tileBlocksVision(int t);
void loadMapArr(unsigned char *m);

