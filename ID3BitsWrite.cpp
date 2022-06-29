#include "ID3BitsWrite.h"

Frame::Frame() {
    size = new char[4]{0};
}

Frame::~Frame() {
    delete []size;
}

/** This method is used to convert a size_t to 4-size char array in little endian.
 *
 * @param i_size size_t, size prepared to convert.
 */
void Frame::bitsSize_tToCharArray(size_t i_size) {
    size[0] = char (i_size / 0x200000);
    i_size -= size[0] * 0x200000;
    size[1] = char (i_size / 0x4000);
    i_size -= size[1] * 0x4000;
    size[2] = char (i_size / 0x80);
    i_size -= size[2] * 0x80;
    size[3] = char (i_size);
}

HeadFrame::HeadFrame() {
    header = new char[3]{0x49, 0x44, 0x33};
    ver = 0x04;
    revision = 0x00;
    headFlag = 0x00;
}

HeadFrame::~HeadFrame() {
    delete []header;
}

void HeadFrame::ID3FrameBitsWrite(ofstream &o, const size_t & i_size) {
    bitsSize_tToCharArray(i_size);
    o.write(header, 3);
    o.write(&ver, 1);
    o.write(&revision, 1);
    o.write(&headFlag, 1);
    o.write(size, 4);
}

SignFrame::SignFrame() {
    signType = new char[4];
    flag = 0x00;
    encode = 0x03;
}

void SignFrame::ID3FrameBitsWrite(ofstream & o,const size_t & i_size) {
    bitsSize_tToCharArray(i_size);
    o.write(signType, 4);
    o.write(size, 4);
    o.write(&flag, 1);
    o.write(&flag, 1);
    o.write(&encode, 1);
}

SignFrame::~SignFrame() {
    delete []signType;
}

void SignFrame::setSignFrameType(const string &type) {
    for (int i=0;i<4;++i) {
        signType[i] = type[i];
    }
}

void ID3signFrameRegularBitsWrite(ofstream & o,const string & type ,const string & content, const size_t & contentSize) {
    auto * signFrame = new SignFrame;
    signFrame->setSignFrameType(type);
    signFrame->ID3FrameBitsWrite(o, contentSize);
    o << content;
    o << char(0x00);
    delete signFrame;
}

/**\brief This function is used to judge cover file's type
 *
 * @param picFilePath const string &, cover file relative path.
 * @return bool, true if cover is jpeg, false for png.
 */
bool coverType(const string &picFilePath) {
    if (picFilePath.ends_with(".jpg")) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t coverBitsCount(const string & picFilePath) {

    uint32_t size = filesystem::file_size(picFilePath.c_str());
    if (coverType(picFilePath)) size += 21;
    else size += 20;
    return size;
}

void ID3signFrameCoverBitsWrite(ofstream & o, const string & picFilePath, const size_t &coverSize) {
    auto * signFrame = new SignFrame;
    signFrame->setSignFrameType("APIC");
    signFrame->ID3FrameBitsWrite(o, coverSize);
    delete signFrame;
    if (coverType(picFilePath)) o << "image/jpeg";
    else o << "image/png";
    char buffer;
    buffer = 0x00;
    o.write(&buffer, 1);
    buffer = 0x03;
    o.write(&buffer, 1);
    o << "cover";
    buffer = 0x00;
    o.write(&buffer, 1);
    ifstream i;
    i.open(picFilePath, ios::binary);
    while (i.read(&buffer, 1)) o.write(&buffer, 1);
    o << char(0x00);
    i.close();
}

void ID3signFrameLyricBitsWrite(ofstream & o, const string * lyrics) {
    auto * signFrame = new SignFrame;
    signFrame->setSignFrameType("USLT");
    signFrame->ID3FrameBitsWrite(o, lyrics->length()+6);
    delete signFrame;
    char * buffer = new char(0x58);
    for (int i=0;i<3;++i) {
        o.write(buffer, 1);
    }
    *buffer = 0x00;
    o.write(buffer, 1);
    delete buffer;
    o << *lyrics;
    o << char(0x00);
}

void ID3paddingBitsWrite(ofstream & o) {
    char buffer = 0x00;
    for (int i=0;i<100;++i) {
        o.write(&buffer, 1);
    }
}
