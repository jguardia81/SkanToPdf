#ifndef SCANNEDIMAGEDISPLAY_H
#define SCANNEDIMAGEDISPLAY_H

#include <QDialog>
#include <QImage>
namespace Ui {
class ScannedImageDisplay;
}

class ScannedImageDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit ScannedImageDisplay(QWidget *parent = 0);
    ~ScannedImageDisplay();
    void displayImaqe(const QImage* const imageToDisplay );

private:
    Ui::ScannedImageDisplay *ui;
};

#endif // SCANNEDIMAGEDISPLAY_H
