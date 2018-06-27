﻿#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#ifdef WIN32
#include "Windows.h"
#endif //zxlwin
#ifdef Q_OS_MACOS || Q_OS_MAC32 || Q_OS_MAC64
#include <ApplicationServices/ApplicationServices.h>
#endif
#include "firstlogin.h"
#include "ui_firstlogin.h"
#include "goopal.h"
#include "macro.h"
#include "debug_log.h"
#include "commondialog.h"
#include "datamgr.h"
#include "database.h"

FirstLogin::FirstLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstLogin)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;
    ui->setupUi(this);
    setAutoFillBackground(true);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(DataMgr::getDataMgr()->getWorkPath() + "pic2/bg.png")));
    setPalette(palette);

    ui->pwdLineEdit->setStyleSheet("color:white;background:transparent;border-width:0;border-style:outset;");
    ui->pwdLineEdit->setFocus();
    ui->pwdLineEdit->installEventFilter(this);
    ui->pwdLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->pwdLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->pwdLineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->pwdLineEdit2->setStyleSheet("color:white;background:transparent;border-width:0;border-style:outset;");
    ui->pwdLineEdit2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->pwdLineEdit2->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->pwdLineEdit2->setAttribute(Qt::WA_MacShowFocusRect, false);

    QFont font = ui->pwdLabel->font();
    font.setPixelSize(12);
    ui->pwdLabel->setFont(font);
    ui->pwdLabel2->setFont(font);
    font = ui->tipLabel->font();
    font.setPixelSize(12);
    ui->tipLabel->setFont(font);
    ui->minBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/minimize2.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
                              "QToolButton:hover{background-image:url(%2pic2/minimize_hover.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(DataMgr::getDataMgr()->getWorkPath()).arg(DataMgr::getDataMgr()->getWorkPath()));
    ui->closeBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/close2.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
                                "QToolButton:hover{background-image:url(%2pic2/close_hover.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(DataMgr::getDataMgr()->getWorkPath()).arg(DataMgr::getDataMgr()->getWorkPath()));
    ui->correctLabel->setPixmap(QPixmap(DataMgr::getDataMgr()->getWorkPath() + "pic2/correct2.png"));
    ui->correctLabel2->setPixmap(QPixmap(DataMgr::getDataMgr()->getWorkPath() + "pic2/correct2.png"));
    ui->correctLabel->hide();
    ui->correctLabel2->hide();

	QString language = DataMgr::getInstance()->getLanguage();
    if (language.isEmpty() || language == "English") {
        ui->tipLabel2->setPixmap(QPixmap(DataMgr::getDataMgr()->getWorkPath() + "pic2/pwdTip_En.png"));
        ui->tipLabel2->setGeometry(602, 148, 316, 124);
    } else {
        ui->tipLabel2->setPixmap(QPixmap(DataMgr::getDataMgr()->getWorkPath() + "pic2/pwdTip.png"));
        ui->tipLabel2->setGeometry(602, 148, 319, 124);
    }
    
	ui->versionLabel->setText("Achain Wallet Lite " ACHAIN_WALLET_VERSION_STR);

#ifdef WIN32
    if (GetKeyState(VK_CAPITAL)) {  //zxlwin
        ui->tipLabel->setText(tr("Caps lock opened!"));
    }
#endif // WIN32

#ifdef Q_OS_MACOS || Q_OS_MAC32 || Q_OS_MAC64
    bool wasCapsLockDown = CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, 57);
    if (wasCapsLockDown)
        ui->tipLabel->setText(tr("Caps lock opened!"));
#endif

    connect(DataMgr::getInstance(), &DataMgr::onWalletUnlock, this, &FirstLogin::walletUnlockFirstLogin);
    DLOG_QT_WALLET_FUNCTION_END;
}

FirstLogin::~FirstLogin() {
    DLOG_QT_WALLET_FUNCTION_BEGIN;
    delete ui;
    DLOG_QT_WALLET_FUNCTION_END;
}

void FirstLogin::on_createBtn_clicked() {
    DLOG_QT_WALLET_FUNCTION_BEGIN;
    
    if (ui->pwdLineEdit->text().isEmpty() || ui->pwdLineEdit2->text().isEmpty()) {
        ui->tipLabel->setText(tr("Empty!"));
        return;
    }
    
    if (ui->pwdLineEdit->text() == ui->pwdLineEdit2->text()) {
        if (ui->pwdLineEdit->text().size() < 8) {
            ui->tipLabel->setText(tr("Too short!"));
            return;
        }
        
        ui->pwdLineEdit->setEnabled(false);
        ui->pwdLineEdit2->setEnabled(false);

		bool ret = DataMgr::getInstance()->walletSavePassphrase(ui->pwdLineEdit->text());
		if (ret)
		{
			DataMgr::getInstance()->walletUnlock(ui->pwdLineEdit->text());
		}

    } else {
        ui->tipLabel->setText(tr("not consistent!"));
        return;
    }
    
    DLOG_QT_WALLET_FUNCTION_END;
}

void FirstLogin::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(255,255,255));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(QPoint(367, 217), QPoint(595, 217));
    painter.drawLine(QPoint(367, 289), QPoint(595, 289));
}

void FirstLogin::on_pwdLineEdit2_returnPressed() {
    on_createBtn_clicked();
}

void FirstLogin::on_minBtn_clicked() {
    if (false) {
        emit tray();
        
    } else {
        emit minimum();
    }
}

void FirstLogin::on_closeBtn_clicked() {
    CommonDialog commonDialog(CommonDialog::OkAndCancel);
    commonDialog.setText(tr("Close Achain Wallet Lite?"));
    bool choice = commonDialog.pop();
    
    if (choice) {
        emit closeGOP();
        
    } else {
        return;
    }
}

bool FirstLogin::eventFilter(QObject *watched, QEvent *e) {
    if (watched == ui->pwdLineEdit) {
        if (e->type() == QEvent::FocusIn) {
            ui->tipLabel2->show();
            
        } else if (e->type() == QEvent::FocusOut) {
            ui->tipLabel2->hide();
        }
    }
    
    return QWidget::eventFilter(watched, e);
}

void FirstLogin::on_pwdLineEdit_textChanged(const QString &arg1) {
    if (arg1.indexOf(' ') > -1) {
        ui->pwdLineEdit->setText(ui->pwdLineEdit->text().remove(' '));
        return;
    }
    
    ui->tipLabel->clear();
    
    if (arg1.size() > 7) {
        ui->correctLabel->show();
        
        if (ui->pwdLineEdit->text() == ui->pwdLineEdit2->text()) {
            ui->correctLabel2->show();
            
        } else {
            ui->correctLabel2->hide();
        }
        
    } else {
        ui->correctLabel->hide();
        ui->correctLabel2->hide();
    }
}

void FirstLogin::on_pwdLineEdit2_textChanged(const QString &arg1) {
    if (arg1.indexOf(' ') > -1) {
        ui->pwdLineEdit2->setText(ui->pwdLineEdit2->text().remove(' '));
        return;
    }
    
    ui->tipLabel->clear();
    
    if (arg1.size() > 7 && (ui->pwdLineEdit->text() == ui->pwdLineEdit2->text())) {
        ui->correctLabel2->show();
        
    } else {
        ui->correctLabel2->hide();
    }
}

void FirstLogin::walletUnlockFirstLogin(bool result)
{
    ui->pwdLineEdit->setEnabled(true);
    ui->pwdLineEdit2->setEnabled(true);
    
    if (result) {
        emit login();
        this->close();
        deleteLater();
    }
}

void FirstLogin::jsonDataUpdated(QString id) {
}

void FirstLogin::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);

    if(e->key() == Qt::Key_CapsLock)
    {
#ifdef WIN32
        if(GetKeyState(VK_CAPITAL) == -127)
        {
            ui->tipLabel->setText( tr("Caps lock opened!") );
        }
        else
        {
            ui->tipLabel->setText( tr("Caps lock closed!") );
        }
#endif // WIN32

#ifdef Q_OS_MACOS || Q_OS_MAC32 || Q_OS_MAC64
        ui->tipLabel->setText(tr("Caps lock opened!"));
#endif
    }
}

void FirstLogin::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);

    if(e->key() == Qt::Key_CapsLock)
    {
#ifdef Q_OS_MACOS || Q_OS_MAC32 || Q_OS_MAC64
        ui->tipLabel->setText(tr("Caps lock closed!"));
#endif
    }
}