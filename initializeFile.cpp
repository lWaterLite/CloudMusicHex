#include "initializeFile.h"

/*
int initializeModeCode() {
    int code;
    cout << "0.use default cache path\n" << "1.use custom path\n"
         << "Please enter the number to continue process.\n";
    cin >> code;
    return code;
}
*/

/**\brief This function is used to get user's name, ONLY work on Windows.
 *
 * @return string, stores the user's name
 */
string getUserName() {
    char szBuffer[100];
    DWORD len = 100;
    GetUserNameA(szBuffer, &len);
    string s = szBuffer;
    return s;
}

/**\brief This function is used to initialize the default cache path.
 *
 * @param path string &, used to store the default cache path
 * @return bool, true if path exists, otherwise false.
 */
bool initializeCachePath(string & path) {
    path = "C:\\Users\\" + getUserName() + R"(\AppData\Local\Netease\CloudMusic\Cache\Cache\)";
    if (_access(path.c_str(), 0) == -1) return false;
    else return true;
}

/**\brief This function is used to get all cache file's absolute path.
 *
 * @param path const string &, pass the string that contains the cache path.
 * @param file vector<string> &, used to store all cache file absolute path.
 * @param suffix const string &, pass the string of file's suffix.
 */
void fileNameGet(const string &path, vector<string> &file, const string &suffix) {
    string fileName;
    for (auto & menu:filesystem::directory_iterator(path)) {
        fileName = menu.path().string();
        if (fileName.ends_with(suffix)) file.push_back(fileName);
    }
}

/**\brief This function is used to convert 4-size char array in little endian to int.
 *
 * @param size const char *, pointer to a 4-size char array.
 * @return uint32_t, int value of size.
 */
uint32_t charSizeToInt(const char * size) {
    uint32_t total_size;
    total_size = (size[0]&0x7F)*0x200000
                 +(size[1]&0x7F)*0x4000
                 +(size[2]&0x7F)*0x80
                 +(size[3]&0x7F);
    return total_size;
}

/**\brief This function is used to get song's ID in CloudMusic.
 *
 * @param fileName const string &, contain the cache file absolute path.
 * @param f_index size_t, the length of string that contains cache path.
 * @return string, song's ID as a string.
 */
string getSongID(const string & fileName, size_t f_index) {
    size_t i_index = fileName.find_first_of('-');
    return fileName.substr(f_index, i_index-f_index);
}

/**\brief This function is used to create a folder if which not existing.
 *
 * @param path const string &, the relative path of the folder.
 */
void createPath(const string & path) {
    if (_access(path.c_str(), 0) == -1)	{
        _mkdir(path.c_str());
        cout << "+ " << path << endl;
    }
}

/**\brief This function is used to download the file from a url.
 *
 * @param url const string &, the target URL.
 * @param jsonFileName const string &, filename of downloaded file.
 * @return HRESULT, the result of download event, S_OK if success.
 */
HRESULT downloadJsonFile(const string & url, const string & jsonFileName) {
    return URLDownloadToFileA(nullptr, url.c_str(), jsonFileName.c_str(), NULL, nullptr);
}

/**\brief This function is used to download the needed file when writing ID3.
 *
 * @param ID const string &, song's ID.
 * @return bool, true if download is successful, otherwise false.
 */
bool downloadResult(const string & ID) {
    bool r;
    auto * infoURL = new string{"http://music.163.com/api/song/detail/?ids=[" + ID + "]"};
    auto * lyricURL = new string{"http://music.163.com/api/song/lyric?id=" + ID + "&lv=1&kv=1&tv=-1"};
    if (downloadJsonFile(*infoURL, "./temp/info.json") == S_OK
    && downloadJsonFile(*lyricURL, "./temp/lyric.json") == S_OK) {
        r = true;
    }
    else r = false;
    delete infoURL;
    delete lyricURL;
    return r;
}

/**\brief This function is used to write MPEG frame into mp3 file.
 *
 * @param o oftream &, point to a on-processing mp3 file.
 * @param originFilePath const string &, the absolute path of cache file.
 */
void xorPureMp3BitsWrite(ofstream & o, const string & originFilePath) {
    char buffer;
    ifstream fi;
    fi.open(originFilePath, ios::binary);
    fi.seekg(6);
    char c_size[4];
    for (char &i: c_size) {
        fi.read(&buffer, 1);
        buffer = buffer ^ 0xA3; // NOLINT(cppcoreguidelines-narrowing-conversions)
        i = buffer;
    }
    uint32_t size = charSizeToInt(c_size);
    fi.seekg(size+10);
    while (fi.read(&buffer, 1)) {
        buffer = buffer ^ 0xA3; // NOLINT(cppcoreguidelines-narrowing-conversions)
        o << buffer;
    }
    fi.close();
}

/**\brief This function is used to do some finishing job.
 *
 * It delete all file in temp folder and folder itself.
 */
void fileSweeping() {
    cout << "Sweeping..." << endl;
    if (_access("./temp/", 0) == 0) {
        for (auto & directoryIterator: filesystem::directory_iterator("./temp/")) {
            remove(directoryIterator.path().string().c_str());
        }
    }
    RemoveDirectoryA("./temp");
    cout << "Done!";
}
