#include "scannedimagedisplay.h"
#include "ui_scannedimagedisplay.h"

ScannedImageDisplay::ScannedImageDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScannedImageDisplay)
{
    ui->setupUi(this);
}

ScannedImageDisplay::~ScannedImageDisplay()
{
    delete ui;
}

void ScannedImageDisplay::displayImaqe(const QImage * const imageToDisplay)
{
    QImage *newImage = new QImage(*imageToDisplay);
    this->hide();
    ui->imageLabel->setPixmap(QPixmap::fromImage(*newImage));
    this->show();
}
