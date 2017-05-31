#include "configuredialog.h"
#include "ui_configuredialog.h"

ConfigureDialog::ConfigureDialog(const QString &name, const QString &mail, QWidget *parent) :
    ui(new Ui::ConfigureDialog),
    name_(name),
    mail_(mail),
    QDialog(parent)

{
    ui->setupUi(this);
    initialize();
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::initialize() {
    ui->txtDisplayName->setText(name_);
    ui->txtEmailAddress->setText(mail_);
}

void ConfigureDialog::retrieveData( QString &displayName, QString& emailAddress) {
    displayName = ui->txtDisplayName->text();
    emailAddress = ui->txtEmailAddress->text();
}
