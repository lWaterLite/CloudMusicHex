#ifndef QT_TEST1_MAINWINDOW_H
#define QT_TEST1_MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include "header.h"
#include "Mp3.h"
#include "initializeFile.h"
#include "ID3BitsWrite.h"
#include "jsonParse.h"

using namespace std;

/**\brief Custom button\n
 * Style:\n
 * size: 350, 45;
 * font-size: 14px
 */
class getCachePathPushButton: public QPushButton {
private:
public:
    explicit getCachePathPushButton(const QString & btnName = nullptr, QWidget * parent = nullptr);
};

/**\brief Custom Label\n
 * Style:\n
 * font-family: Arial;
 * font-size: 14px
 */
class CustomLabel: public QLabel {
private:
public:
    explicit CustomLabel(const QString & labText = nullptr, QWidget * parent = nullptr);
};

/**\brief Main Window\n
 * <h3>Style:\n</h3>
 * size: minimum 590, fixed 300;
 * font: Arial, 14px;\n
 * <h3>Private variable:\n</h3>
 * cachePathLabel: CustomLabel *; Use to initialize the label with the text of absolute cache path.
 * processBtn: getCachePathPushButton *; Use to initialize the button that get cache path.
 * cachePathString: string *; Use to store the absolute cache path.
 * cachePathGetResult: bool; Use to store the result of path initializing function.\n
 * <h3>Signals:\n</h3>
 * mainProcessExe(): void; This slot function is used to execute all process of mp3 file creation, it's kind like a Main
 * Function encapsulation.
 */
class MainWindow: public QMainWindow {
private:
    CustomLabel * cachePathLabel;
    getCachePathPushButton * processBtn;

    string * cachePathString;
    bool cachePathGetResult;
    vector<string>fileAbsolutePath;
public:
    explicit MainWindow();
    ~MainWindow() override;
    slots
    void mainProcessExe();
};

#endif //QT_TEST1_MAINWINDOW_H
