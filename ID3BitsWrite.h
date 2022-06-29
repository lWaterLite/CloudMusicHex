#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-override"
#ifndef CLOUDMUSICHEX_ID3BITSWRITE_H
#define CLOUDMUSICHEX_ID3BITSWRITE_H

#include "header.h"

/**\brief Base Frame class\n
 *
 * <h3>Private variable:\n</h3>
 * size: char *; point to a char array, length is 4.
 *
 * <h3>Public function:\n</h3>
 * Frame();
 * virtual ~Frame();
 * Frame(const Frame &) = delete; The copy constructor is prohibited.
 * Frame &operator=(const Frame &) = delete; The assignment operator is prohibited.
 * bitsSize_tToCharArray(size_t i_size): void; convert size to a 4-bits char array, using little endian.
 */
class Frame {
protected:
    char * size;
public:
    Frame();
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;
    virtual ~Frame();

    void bitsSize_tToCharArray(size_t i_size);
    virtual void ID3FrameBitsWrite(ofstream & o,const size_t & i_size) = 0;
};

class HeadFrame:public Frame {
private:
    char * header;
    char ver;
    char revision;
    char headFlag;
public:
    HeadFrame();
    HeadFrame(const HeadFrame &) = delete;
    HeadFrame &operator=(const HeadFrame &) = delete;
    virtual ~HeadFrame();

    virtual void ID3FrameBitsWrite(ofstream & o,const size_t & i_size);
};

// ID3 signFrame
class SignFrame:public Frame {
private:
    char * signType;
    char flag;
    char encode;
public:
    SignFrame();
    SignFrame(const SignFrame &) = delete;
    SignFrame &operator=(const SignFrame &) = delete;
    virtual ~SignFrame();

    virtual void ID3FrameBitsWrite(ofstream & o,const size_t & i_size);
    void setSignFrameType(const string & type);
};

void ID3signFrameRegularBitsWrite(ofstream & o,const string & type ,const string & content, const size_t & contentSize);
bool coverType(const string &picFilePath);  // jpg true, png false
uint32_t coverBitsCount(const string & picFilePath);
void ID3signFrameCoverBitsWrite(ofstream & o, const string & picFilePath, const size_t &coverSize);
void ID3signFrameLyricBitsWrite(ofstream & o, const string * lyrics);
void ID3paddingBitsWrite(ofstream & o);

#endif //CLOUDMUSICHEX_ID3BITSWRITE_H
