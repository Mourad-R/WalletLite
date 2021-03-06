#ifndef TRANSFERPAGE_H
#define TRANSFERPAGE_H

#include <QWidget>

namespace Ui {
class TransferPage;
}

class WaitingPage;

#define MODULE_TRAHSFER_PAGE "TRAHSFER_PAGE"

class TransferPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransferPage(QString name,QWidget *parent = 0);
    ~TransferPage();

    QString getCurrentAccount();
    void setAddress(QString);
	void setbalanceLabel(QString balance, QString token);
	void showBalance();

private:
    void paintEvent(QPaintEvent*);
    void setAssertType();
    void showWaitingPage();
    void hideWaitingPage();
	void checkAmountValid(double amount);

signals:
    void accountChanged(QString);
    void showShadowWidget();
    void hideShadowWidget();
    void showBillPage();
	void recreateTransferPage();
	void back();

public slots:
	void tokenTransferTo(QString result);
	void updateBalance();

private slots:
    void on_accountComboBox_currentIndexChanged(const QString &arg1);
    void on_sendBtn_clicked();
    void on_amountLineEdit_textChanged(const QString &arg1);
    void on_sendtoLineEdit_textChanged(const QString &arg1);
    void on_copyBtn_clicked();

    void walletTransferToAddress(bool resp_success, QString id, QString msg);
    void walletCheckAddress(QString address);

    void on_assetComboBox_currentIndexChanged(int index);

private:
    Ui::TransferPage *ui;
    WaitingPage* waitingPage;
    QString accountName;
    QStringList contactsList;
    bool inited;
    int transfer_time;
};

#endif // TRANSFERPAGE_H
