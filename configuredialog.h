#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureDialog;
}

class ConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDialog(const QString& name, const QString& mail, QWidget *parent = 0);
    ~ConfigureDialog();

    void retrieveData(QString &displayName, QString &emailAddress);
private:
    Ui::ConfigureDialog *ui;

    QString name_;
    QString mail_;
    void initialize();
};

#endif // CONFIGUREDIALOG_H
