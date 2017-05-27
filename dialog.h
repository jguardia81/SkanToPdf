#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include <QImage>
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

private:
    QList<QImage> images_;

};

#endif // DIALOG_H
