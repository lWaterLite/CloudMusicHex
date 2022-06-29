#ifndef CLOUDMUSICHEX_MP3_H
#define CLOUDMUSICHEX_MP3_H

#include "header.h"
#include "ID3BitsWrite.h"
#include "jsonParse.h"

/**\brief The encapsulation of <a href=https://id3.org/>ID3</a> framework and MPEG frame\n
 *
 * <h3>Private variable:\n</h3>
 * headFrame: HeadFrame *; Use to store the HeadFrame of ID3.
 * signFrame: SignFrame *; use to store the regular SignFrame of ID3, point to a 4-length SignFrame class array.
 * content: vector\<string\>; use to store the content of 4 regular ID3 SignFrame.
 * contentSize: vector\<size_t\>; use to store the size of the content of 4 regular ID3 SignFrame.
 * totalSize: uint32_t; use to store the total size of all ID3 SignFrame.
 * ofs: ofstream; use to open a ofstream object.
 * mp3FileName: string; use to store the filename of the new mp3 file.
 * lyrics: string *; point to a string stored the lyrics.
 * picName: string *; point to a string stored the cover picture filename.
 * getPicResult: bool; use to store the result of downloading cover picture.\n
 *
 * <h3>Private method:\n</h3>
 * mp3JsonInfoGet(): void; use to get ID3 info and cover url from json.
 * mp3FileNameGet(): void; use to get new mp3 filename.
 * mp3JsonLyricGet(): void; use to get lyrics from json.
 * mp3CoverGet(): void; use to get cover picture file online.
 * mp3OftreamCreate(): void; use to create a ofstream object.
 * mp3HeadFrameWrite(): void; use to write ID3 HeadFrame.
 * mp3SignFrameWrite(const string * TypeArray): void; use to write 4 ID3 regular SignFrame.
 * mp3LyricsFrameWrite(): void; use to write ID3 lyrics SignFrame.
 * mp3CoverFrameWrite(): void; use to write ID3 cover SignFrame.
 * mp3PaddingWrite(): void; use to write ID3 padding.
 * mp3MPEGBitsWrite(const string & fileName): void; use to write MPEG frames.\n
 *
 * <h3>Public function:\n</h3>
 * Mp3();
 * ~Mp3();
 * Mp3(const Mp3 &) = delete; The copy constructor is prohibited.
 * Mp3 & operator=(const Mp3 &) = delete; The assignment operator is prohibited.
 * mp3FileCreate(const string * TypeArray, const string & fileName): void; use to process the mp3 file.
 */
class Mp3 {
private:
    HeadFrame * headFrame;
    SignFrame * signFrame;
    vector<string>content;
    vector<size_t>contentSize;
    uint32_t totalSize;
    ofstream ofs;
    string mp3FileName;
    string * lyrics;
    string * picName;
    bool getPicResult;

    void mp3JsonInfoGet();
    void mp3FileNameGet();
    void mp3JsonLyricGet();
    void mp3CoverGet();
    void mp3OftreamCreate();
    void mp3HeadFrameWrite();
    void mp3SignFrameWrite(const string * TypeArray);
    void mp3LyricsFrameWrite();
    void mp3CoverFrameWrite();
    void mp3PaddingWrite();
    void mp3MPEGBitsWrite(const string & fileName);
public:
    Mp3();
    Mp3(const Mp3 &) = delete;
    Mp3 & operator=(const Mp3 &) = delete;
    ~Mp3();

    void mp3FileCreate(const string * TypeArray, const string & fileName);
};


#endif //CLOUDMUSICHEX_MP3_H
