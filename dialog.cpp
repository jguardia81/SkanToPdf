#include "dialog.h"
#include "ui_dialog.h"
#include <KF5/KSane/ksanewidget.h>
#include <QMap>
#include <QDebug>
#include <QPrinter>
#include <QPainter>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    prepare();

    connect (ui->scanWidget, &KSaneIface::KSaneWidget::imageReady, this,  &Dialog::onImageReady );
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
}

//QPrinter printer;
//    printer.setOutputFormat(QPrinter::PdfFormat);
//    printer.setOutputFileName("/home/jean/visu.pdf");
//    QPrinter::Margins margins;
//    margins.bottom = 0.0;
//    margins.top =0.0;
//    margins.left = 0.0;
//    margins.right =0.0;
//    printer.setMargins(margins);
//    printer.setFullPage(true);
//    QPageLayout layout = printer.pageLayout();
//    layout.setMode(QPageLayout::FullPageMode);
//    printer.setPageLayout(layout);

//    QPainter painter;

//    painter.begin(&printer);

//    painter.drawText(10,10,"Mon texte imprimé");
//    painter.drawImage(0,0,qimage);
//    painter.end();


//}
