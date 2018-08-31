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
    on_hide_clicked();
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

void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();
    this->hide();
        mSysTrayIcon->show();

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

void MainWindow::on_hide_clicked()
{
    //隐藏主窗口


    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/808.ico");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("测试系统托盘图标"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单
    createActions();
    createMenu();
    //在系统托盘显示此对象

}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        mSysTrayIcon->showMessage(QObject::trUtf8("Message Title"),
                                  QObject::trUtf8("欢迎使用此程序"),
                                  QSystemTrayIcon::Information,
                                  1000);
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::createActions()
{
    mShowMainAction = new QAction(QObject::trUtf8("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(QObject::trUtf8("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

}

void MainWindow::createMenu()
{
    mMenu = new QMenu(this);
    mMenu->addAction(mShowMainAction);

    mMenu->addSeparator();

    mMenu->addAction(mExitAppAction);

    mSysTrayIcon->setContextMenu(mMenu);
}

void MainWindow::on_showMainAction()
{
    this->show();
}

void MainWindow::on_exitAppAction()
{
    exit(0);
}
