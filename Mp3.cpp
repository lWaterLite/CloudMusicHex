#include "Mp3.h"
#include "initializeFile.h"

// Mp3 default constructor
Mp3::Mp3() {
    headFrame = new HeadFrame;
    signFrame = new SignFrame[4];
    totalSize = 0;
    getPicResult = false;
    lyrics = new string;
    lyrics->resize(10240);
    picName = new string;
}

// Mp3 default destructor
Mp3::~Mp3() {
    delete headFrame;
    delete []signFrame;
    delete lyrics;
    delete picName;
    content.clear();
    contentSize.clear();
    ofs.close();
}


void Mp3::mp3JsonInfoGet() {
    totalSize += jsonInfoGet(content, contentSize);
}

void Mp3::mp3FileNameGet() {
    mp3FileName = UTF8_2_GBK(content[0]) + " - " + UTF8_2_GBK(content[1]) + ".mp3";
}

void Mp3::mp3OftreamCreate() {
    ofs.open("./mp3/" + mp3FileName, ios::binary);
    cout << "+ " + mp3FileName << endl;
}

void Mp3::mp3JsonLyricGet() {
    jsonLyricGet(*lyrics);
    totalSize = totalSize + lyrics->length() + 16;
}

void Mp3::mp3CoverGet() {
    getPicResult = jsonPicGet(*picName);
    if (getPicResult) totalSize = totalSize + coverBitsCount(*picName) + 10;
    totalSize += 99;
}

void Mp3::mp3HeadFrameWrite() {
    headFrame->ID3FrameBitsWrite(ofs, totalSize);
}

void Mp3::mp3SignFrameWrite(const string * TypeArray) {
    for (int i=0;i<4;++i) {
        ID3signFrameRegularBitsWrite(ofs, TypeArray[i], content[i], contentSize[i]);
    }
}

void Mp3::mp3LyricsFrameWrite() {
    cout << "+ lyrics" << endl;
    ID3signFrameLyricBitsWrite(ofs, lyrics);
}

void Mp3::mp3CoverFrameWrite() {
    if (getPicResult) {
        cout << "+ cover" << endl;
        ID3signFrameCoverBitsWrite(ofs, *picName, coverBitsCount(*picName));
    }
}

void Mp3::mp3PaddingWrite() {
    ID3paddingBitsWrite(ofs);
}

void Mp3::mp3MPEGBitsWrite(const string & fileName) {
    xorPureMp3BitsWrite(ofs, fileName);
}

/**\brief Process the mp3 file\n
 *
 * This function is the only method of Mp3 class, which actually use the private function in specified sequence.
 *
 * @param TypeName: const string *, the string array of FrameID, length must be 4.
 * @param fileName: const string &, the filename of cache file, end with ".uc" suffix.
 */
void Mp3::mp3FileCreate(const string * TypeName, const string & fileName) {
    mp3JsonInfoGet();
    mp3FileNameGet();
    mp3JsonLyricGet();
    mp3CoverGet();
    mp3OftreamCreate();
    mp3HeadFrameWrite();
    mp3SignFrameWrite(TypeName);
    mp3LyricsFrameWrite();
    mp3CoverFrameWrite();
    mp3PaddingWrite();
    mp3MPEGBitsWrite(fileName);
}


