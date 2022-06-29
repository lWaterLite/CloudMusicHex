#ifndef CLOUDMUSICHEX_JSONPARSE_H
#define CLOUDMUSICHEX_JSONPARSE_H

#include "header.h"

// string GBK_2_UTF8(const string& strGBK);
string UTF8_2_GBK(const string& strUTF8);
size_t jsonInfoGet(vector<string>& totalContent, vector<size_t>& contentSize); // content seq: name, artists, albumName, no
bool jsonPicGet(string & picName);
void jsonLyricGet(string & lyrics);

#endif //CLOUDMUSICHEX_JSONPARSE_H
