#include "mainWindow.h"

getCachePathPushButton::getCachePathPushButton(const QString &btnName, QWidget *parent) {
    setParent(parent);
    resize(350, 45);
    setStyleSheet("QPushButton{font-size: 14px}");
    setText(btnName);
}

MainWindow::MainWindow() {
    // MainWindow Format Set
    setWindowTitle("CloudMusicHex");
    setFixedHeight(300);
    setMinimumWidth(590);

    // menubar set
    auto * menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    // help menu set
    auto * helpMenu = new QMenu("帮助", menuBar);
    menuBar->setFont(QFont("Microsoft YaHei UI", 10));
    menuBar->addMenu(helpMenu);
    // help action and about action set
    auto * helpAction = new QAction("说明", helpMenu);
    helpMenu->addAction(helpAction);
    auto * aboutAction = new QAction("关于", helpMenu);
    helpMenu->addAction(aboutAction);
    // about message box set
    connect(aboutAction, &QAction::triggered, [=](){
        auto * aboutMessageBox = new QMessageBox(this);
        aboutMessageBox->setWindowTitle("关于");
        aboutMessageBox->setIcon(QMessageBox::Information);
        aboutMessageBox->setFont(QFont("Arial", 12));
        aboutMessageBox->setText("Powered by lWaterLite.\nLast update on 6.21.2022");
        aboutMessageBox->exec();
    });
    // help message box set
    connect(helpAction, &QAction::triggered, [=](){
        auto * helpMessageBox = new QMessageBox(this);
        helpMessageBox->setWindowTitle("帮助");
        helpMessageBox->setIcon(QMessageBox::Information);
        setStyleSheet("QPushButton{font-size: 14px}");
        helpMessageBox->setText("使用前请先检查在电脑上已经安装网易云音乐\n如未更改缓存路径请使用默认缓存路径\n如果更改请自行选择缓存路径，定位到存放缓存文件的文件夹目录下\n选择完成后点击确定开始执行");
        helpMessageBox->exec();
    });

    // Cache Path button set
    auto defaultCacheBtn = new getCachePathPushButton("1.获取默认缓存路径", this);
    defaultCacheBtn->move(120, 50);
    auto customCacheBtn = new getCachePathPushButton("2.选择自定缓存路径", this);
    customCacheBtn->move(120, 120);

    // Cache info label set
    auto * lab = new CustomLabel("缓存路径:", this);
    lab->move(120, 190);
    cachePathLabel = new CustomLabel(R"(C:\work\Program_space\Clion\Cpp)", this);
    cachePathLabel->move(120, 210);

    // process button set
    processBtn = new getCachePathPushButton("确定", this);
    processBtn->setEnabled(false);
    processBtn->resize(120, 40);
    processBtn->move(230, 240);
    // button signal and lambda connection
    cachePathString = new string;
    cachePathGetResult = false;
    connect(defaultCacheBtn, &QPushButton::clicked, [&]() mutable {
        cachePathGetResult = initializeCachePath(*cachePathString);
        if (!cachePathGetResult) {
            *cachePathString = "错误！路径不存在";
            cachePathLabel->setText(cachePathString->c_str());
            cachePathLabel->adjustSize();
            if (processBtn->isEnabled()) processBtn->setEnabled(false);
        }
        else {
            cachePathLabel->setText(cachePathString->c_str());
            cachePathLabel->adjustSize();
            if (!processBtn->isEnabled()) processBtn->setEnabled(true);
        }
    });
    connect(customCacheBtn, &QPushButton::clicked, [&]() mutable  {
            *cachePathString = QFileDialog::getExistingDirectory(this, "选择文件夹").toUtf8().data();
            *cachePathString = *cachePathString + "/";
            cachePathLabel->setText(cachePathString->c_str());
            cachePathLabel->adjustSize();
            if (!processBtn->isEnabled()) processBtn->setEnabled(true);
    });

    // process button and lambda connection
    connect(processBtn, &QPushButton::clicked, [=](){
        if ("确定" == processBtn->text()) {
            processBtn->setEnabled(false);
            fileNameGet(*cachePathString, fileAbsolutePath, ".uc");
            if (fileAbsolutePath.empty()) {
                cachePathLabel->setText("选择的缓存路径不含任何缓存文件");
                cachePathLabel->adjustSize();
            }
            else {
                processBtn->setText("关闭");
                mainProcessExe();
                processBtn->setEnabled(true);
            }
        }
        else {
            close();
            }
        });
}

MainWindow::~MainWindow() {
    delete cachePathString;
}

/**\brief Main Function of the whole execution.\n
 * This function is a encapsulation of the console Main Function,
 * which is used to create the GUI in Qt. The detailed descriptions are as followed:\n
 * First, function initialize a const string array to store the ID3 FrameID, then it starts to
 * traverse the file in the target directory, and pick those contained the suffix ".uc", adding them to the vector.
 * After that, function starts to detect the "mp3" and "temp" folder, and create them when not existing.
 * Then the function begin to traverse the file in the vector to process mp3. If the network is fine, the mp3 will contain
 * ID3 framework, otherwise it will only contain the MPEG frame. While processing, the console will print the details of procedures.
 * Check the function that contained in this to see more details.
 */
void MainWindow::mainProcessExe() {
    const string TYPENAME[4] = {"TIT2", "TPE1", "TALB", "TRCK"};
    createPath("./mp3/");
    createPath("./temp/");
    // processing mp3 file
    for (auto & fileName: fileAbsolutePath) {
        string songID = getSongID(fileName, cachePathString->length());
        string infoURL = "http://music.163.com/api/song/detail/?ids=[" + songID + "]";
        string lyricURL = "http://music.163.com/api/song/lyric?id=" + songID + "&lv=1&kv=1&tv=-1";

        if (downloadResult(songID)) {
            auto * mp3File = new Mp3;
            mp3File->mp3FileCreate(TYPENAME, fileName);
            delete mp3File;
        }
        else {
            ofstream fo;
            fo.open("./mp3/" + songID + ".mp3", ios::binary);
            cout << "+ " << songID << ".mp3" << endl;
            xorPureMp3BitsWrite(fo, fileName);
            fo.close();
        }
    }
    fileSweeping();
}

CustomLabel::CustomLabel(const QString &labText, QWidget *parent) {
    setFont(QFont("Arial", 12));
    setParent(parent);
    setText(labText);
    setStyleSheet("QLabel{font-size:14px}");
    adjustSize();
}
