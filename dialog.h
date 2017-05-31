#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include <QImage>
#include <QPrinter>
#include <QStringListModel>
#include <QSettings>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    void prepare();

private slots:
    void onImageReady( 	QByteArray &  	data,
                        int  	width,
                        int  	height,
                        int  	bytes_per_line,
                        int  	format
                        );

    void on_btnClose_clicked();

    void on_btnSearchDir_clicked();

    void on_btnReset_clicked();

    void on_btnSave_clicked();

    void on_btnAddMail_clicked();

    void on_btnRemoveMail_clicked();

    void on_btnResetMail_clicked();

    void on_btnConfigure_clicked();

private:
    QList<QImage> images_;

    void setupPrinter(QPrinter &printer, const QString &pathToPdfFile);
    void printPages(QPrinter &printer, const QString& file, const QString& folder);
    QString createFileUrl(const QString &file, const QString &folder);

    QStringListModel* model_ = nullptr;
    QSettings *appSettings_= nullptr;
};

#endif // DIALOG_H
