#ifndef CLOUDMUSICHEX_INITIALIZEFILE_H
#define CLOUDMUSICHEX_INITIALIZEFILE_H

#include "header.h"

// int initializeModeCode();
bool initializeCachePath(string & path);
void fileNameGet(const string &path, vector<string> &file, const string &suffix);
string getSongID(const string & fileName, size_t f_index);
void createPath(const string & path);
bool downloadResult(const string & ID);
void xorPureMp3BitsWrite(ofstream & o, const string & originFilePath);
void fileSweeping();

#endif //CLOUDMUSICHEX_INITIALIZEFILE_H
