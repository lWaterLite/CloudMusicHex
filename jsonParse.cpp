#include "jsonParse.h"

/*
string GBK_2_UTF8(const string& strGBK) {
    string strOutUTF8;
    WCHAR * str1;
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, nullptr, 0);
    str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, nullptr, 0, nullptr, nullptr);
    char * str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, nullptr, nullptr);
    strOutUTF8 = str2;
    delete[]str1;
    str1 = nullptr;
    delete[]str2;
    str2 = nullptr;
    return strOutUTF8;
}
 */

/**\brief This function is used to convert a UTF-8 coded string to a GBK coded string so that the filename won't go wrong on Chinese OS.
 *
 * @param strUTF8 The string that coded with utf-8, the value of which won't change.
 * @return return a string that has the same text content of strUTF8 but coded with GBK.
 */
string UTF8_2_GBK(const string& strUTF8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, nullptr, 0);
    auto * wszGBK = new WCHAR [len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUTF8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, nullptr, 0, nullptr, nullptr);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, nullptr, nullptr);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}

/**\brief This function is used to parse a json file and get the some info that needed in ID3.
 *
 * content seq: name, artists, albumName, number
 *
 * @param totalContent pass a empty vector, which will have 4 string content after processing.
 * @param contentSize pass a empty vector, which will store the size of each content.
 * @return return a value that is the size of all content, and vector will change too.
 */
size_t jsonInfoGet(vector<string>& totalContent, vector<size_t>& contentSize) {
    ifstream i;
    i.open("./temp/info.json");
    Json::Reader reader;
    Json::Value root;
    string s_temp;
    size_t ut_temp;
    size_t total = 0;
    reader.parse(i, root, false);

    s_temp = root["songs"][0]["name"].asString();
    ut_temp = s_temp.length() + 2;
    totalContent.push_back(s_temp);
    contentSize.push_back(ut_temp);
    total += ut_temp;

    s_temp = root["songs"][0]["artists"][0]["name"].asString();
    ut_temp = s_temp.length() + 2;
    totalContent.push_back(s_temp);
    contentSize.push_back(ut_temp);
    total += ut_temp;

    s_temp = root["songs"][0]["album"]["name"].asString();
    ut_temp = s_temp.length() + 2;
    totalContent.push_back(s_temp);
    contentSize.push_back(ut_temp);
    total += ut_temp;

    s_temp = root["songs"][0]["no"].asString();
    ut_temp = s_temp.length() + 2;
    totalContent.push_back(s_temp);
    contentSize.push_back(ut_temp);
    total += ut_temp;

    total += 40;

    i.close();
    return total;
}

/**\brief This function is used to get cover url and download it online.
 *
 * @param picName pass a empty string, which will store the filename of cover file.
 * @return true for successful downloading, false for fail to get.
 */
bool jsonPicGet(string & picName) {
    ifstream i;
    i.open("./temp/info.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(i, root, false);
    string picURL = root["songs"][0]["album"]["picUrl"].asString();
    i.close();
    auto * r = new HRESULT;
    if (picURL.ends_with(".jpg") || picURL.ends_with(".jpeg")) {
        picName = "./temp/cover.jpg";
        *r = URLDownloadToFileA(nullptr, picURL.c_str(), picName.c_str(), NULL, nullptr);
    }
    else {
        picName = "./temp/cover.png";
        *r = URLDownloadToFileA(nullptr, picURL.c_str(), picName.c_str(), NULL, nullptr);
    }
    if (*r == S_OK) {delete r; return true;}
    else {delete r; return false;}
}

/**\brief This function is used to parse json to get the lyrics
 *
 * @param lyrics pass a string which will store the lyrics, PLEASE RESIZE IT TO AT LEAST 5096!
 */
void jsonLyricGet(string & lyrics) {
    ifstream i;
    i.open("./temp/lyric.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(i, root, false);

    lyrics = root["lrc"]["lyric"].asString();
}
