#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <qfiledialog.h>
#include <qtextedit.h>
#include <QTextStream>
#include <string.h>
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
};

#endif // MAINWINDOW_H
