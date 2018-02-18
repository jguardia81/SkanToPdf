#include "dialog.h"
#include "ui_dialog.h"
#include <KF5/KSane/ksanewidget.h>
#include <QMap>
#include <QDebug>
#include <QPrinter>
#include <QPainter>

#include <QFileDialog>
#include <QMessageBox>
#include <QWindow>
#include <QHBoxLayout>
#include "scannedimagedisplay.h"
#include "configuredialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    appSettings_ = new QSettings("home","SkanToPdf",this);
    prepare();

    model_ = new QStringListModel(this);

    ui->lswMail->setModel(model_);
    connect (ui->scanWidget, &KSaneIface::KSaneWidget::imageReady, this,  &Dialog::onImageReady );
    ui->lswMail->setSelectionMode(QAbstractItemView::MultiSelection);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::prepare() {
    QString selectedDevice = ui->scanWidget->selectDevice(this);
    QMap<QString,QString> options;
    if (!selectedDevice.trimmed().isEmpty()) {
        ui->scanWidget->openDevice(selectedDevice);
        ui->scanWidget->setOptionsCollapsed(true);
        ui->scanWidget->getOptVals(options);
        ui->scanWidget->setOptVal("mode","Color");
        ui->scanWidget->setOptVal("resolution","100 DPI");
        ui->scanWidget->enableAutoSelect(false);
        ui->scanWidget->setPreviewButtonText("Aperçu");
        ui->scanWidget->setScanButtonText("Ajouter au document");
    }

}

void Dialog::onImageReady(QByteArray &data, int width, int height, int bytes_per_line, int format)
{

    QImage qimage;
    qimage = ui->scanWidget->toQImage(data,width,height,bytes_per_line,(KSaneIface::KSaneWidget::ImageFormat) format);

    images_.append(qimage);
    //showImage(&qimage);
}


void Dialog::on_btnClose_clicked()
{
    QApplication::quit();
}

void Dialog::on_btnSearchDir_clicked()
{
    QFileDialog dirDialog(this);

    dirDialog.setFileMode(QFileDialog::Directory);
    int result = dirDialog.exec();

    if (result == QDialog::Accepted) {
        ui->txtPathToFolder->setText(dirDialog.selectedFiles().at(0));
    }
}

void Dialog::on_btnReset_clicked()
{
    int response = QMessageBox::question(this,"Remise à zéro", "Tout effacer ?",QMessageBox::Yes, QMessageBox::No);

    if (response == QMessageBox::Yes) {
        images_.clear();
        ui->txtFileName->setText("");
        model_->setStringList(QStringList());

    }
}

void Dialog::setupPrinter(QPrinter& printer, const QString& pathToPdfFile )
{
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pathToPdfFile);

    QPrinter::Margins margins;
    margins.bottom = 0.0;
    margins.top =0.0;
    margins.left = 0.0;
    margins.right =0.0;
    printer.setMargins(margins);
    printer.setFullPage(true);
    QPageLayout layout = printer.pageLayout();
    layout.setMode(QPageLayout::FullPageMode);
    printer.setPageLayout(layout);
}

void Dialog::printPages(QPrinter& printer, const QString &file, const QString &folder)
{
    QPainter painter;
    QRectF pageRect;
    painter.begin(&printer);

    printer.setPageSize(QPagedPaintDevice::A4);
    for (int index = 0; index < images_.count(); ++index) {
        if (index != 0) {
            printer.newPage();
        }
        painter.drawImage(printer.pageRect(),images_.at(index));
    }
    painter.end();
    QMessageBox::information(this,"Sauvegarde", file + " a été sauvegardé dans le dossier: " + folder );
}

QString Dialog::createFileUrl(const QString& file,  const QString& folder)
{
    QString pathToPdfFile = folder;
    if (!folder.endsWith("/")) {
        pathToPdfFile = pathToPdfFile.append("/");
    }
    pathToPdfFile.append(file);

    if (!pathToPdfFile.endsWith(".pdf")) {
        pathToPdfFile = pathToPdfFile.append(".pdf");
    }

    return pathToPdfFile;
}

void Dialog::on_btnSave_clicked()
{
    if (ui->txtFileName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Attention", "Il faut fournir le nom du fichier.");
        ui->txtFileName->setFocus();
        return;
    }

    if (ui->txtPathToFolder->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Attention", "Il faut fournir le dossier de stockage.");
        ui->txtPathToFolder->setFocus();
        return;
    }

    QString folder = ui->txtPathToFolder->text().trimmed();
    QString file = ui->txtFileName->text();

    QString pathToPdfFile = createFileUrl(file, folder);

    if (QFile::exists(pathToPdfFile)) {
        int result = QMessageBox::warning(this,"Fichier existant.", QString("Attention %1 existe dans déjà dans le dossier %2. Souhaitez-vous écrire par dessus ?").arg(file).arg(folder),QMessageBox::Yes, QMessageBox::No);
        if (result == QMessageBox::No) {
            QMessageBox::information(this,"Attention ", "Changez le nom ou le dossier. ");
            ui->txtFileName->setFocus();
            return;
        }
    }
    QPrinter printer;
    setupPrinter(printer,pathToPdfFile );

    printPages(printer, file, folder);
}

void Dialog::on_btnAddMail_clicked()
{
    model_->insertRow(model_->rowCount());
    QModelIndex modelIndex = model_->index(model_->rowCount()-1);

    ui->lswMail->edit(modelIndex);
}


void Dialog::on_btnRemoveMail_clicked()
{
    QModelIndexList indexlist = ui->lswMail->selectionModel()->selectedRows();
    QList<int> ints;
    for (QModelIndex index : indexlist) {
       ints.append(index.row());
    }


    std::sort(ints.begin(),ints.end(),[](int i,int j) {return i>j;});

    for (int val : ints) {
        model_->removeRow(val);
    }

}

void Dialog::on_btnResetMail_clicked()
{
     model_->setStringList(QStringList());
}

void Dialog::on_btnConfigure_clicked()
{
    QString name = "";
    QString mail = "";
    name = appSettings_->value("UserName", "").toString();
    mail = appSettings_->value("UserMail","").toString();

    ConfigureDialog confDialog(name,mail,this);

    int result = confDialog.exec();

    if (result == QDialog::Accepted) {
        confDialog.retrieveData(name,mail);

        appSettings_->setValue("UserName",name);
        appSettings_->setValue("UserMail",mail);
    }

}

void Dialog::showImage(const QImage * const qimage) {
   ScannedImageDisplay *scannedImagePreview = new ScannedImageDisplay(this);
   scannedImagePreview->displayImaqe(qimage);
   scannedImagePreview->show();
}
