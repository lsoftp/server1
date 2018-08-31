#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <qfiledialog.h>
#include <qtextedit.h>
#include <QTextStream>
#include <string.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}
using namespace std;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int rownb;
    explicit MainWindow(QWidget *parent = 0);
    void Table2ExcelByTxt(QTableWidget *table);
    void closeEvent(QCloseEvent *e);
    ~MainWindow();
    Ui::MainWindow *ui;
public slots:
    void addrow(string s1, string s2,string s3);
private slots:
    void on_action_2_triggered();
    void on_pushButton_clicked();
    void on_action_4_triggered();
    void on_pushButton_2_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_action14544916843_triggered();

public:
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;

public:
    void createActions();
    void createMenu();

private slots:
    void on_hide_clicked();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_showMainAction();
    void on_exitAppAction();
};

#endif // MAINWINDOW_H
