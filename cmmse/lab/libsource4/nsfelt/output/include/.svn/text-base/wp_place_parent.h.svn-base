/*
 * 地名上下级从属关系模块定义
 */

#ifndef PLACE_PARENT_H_
#define PLACE_PARENT_H_

#include <ul_dict.h>

#define PLACE_MAP_FILE "PlaceMap"

//升级点: 现在是5857, 如果将来地名增多, 需要扩大这个SIZE
const unsigned int PARENTS_BUFFER_SIZE = 7000;	//缓冲区的大小


Sdict_search* loadPlaceMap(const char* mapPath, const char* mapFile, char* parentZone);

void freePlaceMap(Sdict_search* pDictSearch);

bool isPlaceParent(const char* firstPlace, char* secondPlace, 
		Sdict_search* pDictSearch, const char* parentZone);

#endif //PLACE_PARENT_H_
