#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ImportDialog;
}

class ShadowWidget;

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ~ImportDialog();

    bool pop();

private slots:
    void on_cancelBtn_clicked();
    void on_pathBtn_clicked();
    void on_importBtn_clicked();
    void on_privateKeyLineEdit_textChanged(const QString &arg1);
    void on_privateKeyLineEdit_returnPressed();

    void walletImportPrivateKey(bool result);

private:
	Ui::ImportDialog *ui;
	bool yesOrNO = false;

    bool accountNameAlreadyExisted(QString name);
};

#endif // IMPORTDIALOG_H
