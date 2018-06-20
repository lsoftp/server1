#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include "tcpserver.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList sl;
    
    sl << "手机号" << "MsgID" << "Time"<< "数据包";
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(100000);
    ui->tableWidget->setHorizontalHeaderLabels(sl);
    ui->tableWidget->setColumnWidth(3,1600);
    ui->checkBox_2->setCheckState(Qt::Checked);

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::addrow(string s1, string s2,string s3)
{
    static int t=0;
    ui->tableWidget->setItem(t,0,new QTableWidgetItem(s1.c_str()));
    ui->tableWidget->setItem(t,1,new QTableWidgetItem(s2.c_str()));
    ui->tableWidget->setItem(t,2,new QTableWidgetItem( QTime::currentTime().toString("hh:mm:ss.zzz")));

    ui->tableWidget->setItem(t,3,new QTableWidgetItem(s3.c_str()));
ui->tableWidget->setCurrentCell(t,0);
    rownb=t;
    t++;

}

void MainWindow::on_action_2_triggered()
{

}
void MainWindow::Table2ExcelByTxt(QTableWidget *table)
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
            QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.txt);;"));

    int row = table->rowCount();
    int col = table->columnCount();
    QList<QString> list;
    //添加列标题
    QString HeaderRow;
    for(int i=0;i<col;i++)
    {
        HeaderRow.append(table->horizontalHeaderItem(i)->text()+"\t");
    }
    list.push_back(HeaderRow);
    for(int i=0;i<rownb;i++)
    {
        QString rowStr = "";
        for(int j=0;j<col;j++){
            rowStr += table->item(i,j)->text() + "\t";

        }
        list.push_back(rowStr);
    }
    QTextEdit textEdit;
    for(int i=0;i<list.size();i++)
    {
        textEdit.append(list.at(i));
    }

    QFile file(filepath);
    if(file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts<<textEdit.document()->toPlainText();
        file.close();
    }
}
void MainWindow::on_pushButton_clicked()
{
    Table2ExcelByTxt(ui->tableWidget);
}

void MainWindow::on_action_4_triggered()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    static bool running = false;
    QString a;
    a=running?"start":"stop";
    running=!running;
    ui->pushButton_2->setText(a);
}

void MainWindow::on_checkBox_clicked()
{
    bool b=(ui->checkBox->checkState()==Qt::Checked);
    TcpClient::isconfig = b;

}

void MainWindow::on_checkBox_2_clicked()
{
    bool b=(ui->checkBox_2->checkState()==Qt::Checked);
    TcpClient::isdisplay = b;
}

void MainWindow::on_action14544916843_triggered()
{
    static MYWORD id=0;

}
