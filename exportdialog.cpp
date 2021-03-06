#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "goopal.h"
#include "commondialog.h"

#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QFile>

#include "datamgr.h"

ExportDialog::ExportDialog( QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog),
    accountName(name)
{
    ui->setupUi(this);

    setParent(Goopal::getInstance()->mainFrame);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->widget->setObjectName("widget");
    ui->widget->setStyleSheet("#widget {background-color:rgba(10, 10, 10,100);}");
    ui->containerWidget->setObjectName("containerwidget");
    ui->containerWidget->setStyleSheet("#containerwidget{background-color: rgb(246, 246, 246);border:1px groove rgb(180,180,180);}");
    
#ifdef WIN32
    exportPath = QDir::currentPath().replace("/", "\\");
    ui->pathLineEdit->setText(exportPath + "\\" + accountName + ".gkey");
#else
    exportPath = QDir::homePath() + "/Documents/AchainWalletLite";
    ui->pathLineEdit->setText(exportPath + "/" + accountName + ".gkey");
#endif // WIN32 //zxlrun

    ui->exportBtn->setStyleSheet("QToolButton{background-color:#4dc8cf;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
                                 "QToolButton:hover{background-color:#7dd9df;}");
    ui->cancelBtn->setStyleSheet("QToolButton{background-color:#ffffff;color:#282828;border:1px solid #7dd9df;border-radius:3px;}"
                                 "QToolButton:hover{color:#7dd9df;}");

    ui->pathLineEdit->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
    ui->pathLineEdit->setTextMargins(8,0,0,0);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::pop()
{
    move(0,0);
    exec();
}

void ExportDialog::hiddenCancelBtn()
{
	ui->exportBtn->setGeometry(QRect(211, 223, 120, 36));
	ui->cancelBtn->setHidden(true);
}

void ExportDialog::on_pathBtn_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this,tr( "Select path"));
    if( !file.isEmpty())
    {
#ifdef WIN32
        file.replace("/","\\");
        ui->pathLineEdit->setText( file + "\\" + accountName + ".gkey");
#else
        ui->pathLineEdit->setText( file + "/" + accountName + ".gkey");
#endif // WIN32 //zxlrun
    }
}

void ExportDialog::on_cancelBtn_clicked()
{
    close();
}

void ExportDialog::on_exportBtn_clicked()
{
    if( !ui->pathLineEdit->text().endsWith(".gkey"))
    {
        CommonDialog commonDialog(CommonDialog::OkOnly);
        commonDialog.setText(tr("Wrong file format!"));
        commonDialog.pop();
        return;
    }

    dumpAccountPrivateKey();
}

void ExportDialog::dumpAccountPrivateKey()
{
    privateKey = DataMgr::getInstance()->walletExportPrivateKey(accountName);

    if(privateKey == "")
        return;

    QByteArray ba = privateKey.toUtf8();

    QDir exportDir(exportPath);
    if (!exportDir.exists())
        exportDir.mkdir(exportPath);

    QFile file(ui->pathLineEdit->text());

    QString fName = file.fileName();
#ifdef WIN332
    fName.replace("/","\\");
    fName = fName.mid( fName.lastIndexOf("\\") + 1);
#else
    fName = fName.mid( fName.lastIndexOf("/") + 1);
#endif

    if( file.exists())
    {
        CommonDialog tipDialog(CommonDialog::OkAndCancel);
        tipDialog.setText( fName + tr( " already exists!\nCover it or not?") );
        if ( !tipDialog.pop())  return;
    }

    if( !file.open(QIODevice::WriteOnly))
    {
        qDebug() << "privatekey file open " + fName + " ERROR";

        CommonDialog tipDialog(CommonDialog::OkOnly);
        tipDialog.setText( tr( "Wrong path!") );
        tipDialog.pop();
        return;
    }

    file.resize(0);
    QTextStream ts( &file);
    ts << ba.toBase64();
    file.close();
    close();

    CommonDialog tipDialog(CommonDialog::OkOnly);
    tipDialog.setText( tr("Export succeeded!     ") );
    tipDialog.pop();
}
