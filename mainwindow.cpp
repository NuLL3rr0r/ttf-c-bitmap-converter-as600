#include <boost/filesystem/path.hpp>
#include <Magick++.h>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#define         BASE_IMAGE_SIZE                 96.0
#define         PREVIEW_FILE_NAME               L"preview.png"

using namespace std;
using namespace boost;
using namespace Magick;

std::vector<std::wstring> MainWindow::s_glyphs {
    L"۰",
    L"۱",
    L"۲",
    L"۳",
    L"۴",
    L"۵",
    L"۶",
    L"۷",
    L"۸",
    L"۹"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if (m_tempDir.isValid()) {
        m_previewFilePath = QString::fromStdWString((boost::filesystem::path(m_tempDir.path().toStdWString())
                                                     / boost::filesystem::path(PREVIEW_FILE_NAME)).wstring());
    }

    ui->setupUi(this);
    this->setFixedSize(483, 261);

    for (std::vector<std::wstring>::const_iterator it = s_glyphs.begin();
         it != s_glyphs.end(); ++it) {
        ui->previewComboBox->addItem(QString::fromStdWString(*it));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFontPushButton_clicked()
{
    QFileInfo fi(ui->fontPathLineEdit->text());
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Pick a File"),
                                                ui->fontPathLineEdit->text() == "" ? QDir::homePath() + "/Desktop/" : fi.absolutePath(),
                                                tr("True type fonts (*.ttf);;All Files (*.*)"));

    if (!file.isNull()) {
        ui->fontPathLineEdit->setText(file);
        ui->previewComboBox->setEnabled(true);
        ui->lcdOutputPushButton->setEnabled(true);
        ui->printerOutputPushButton->setEnabled(true);

        Preview();
    }
}

void MainWindow::Preview()
{
    list<Drawable> drawList;

    double ratio = (BASE_IMAGE_SIZE / (double)ui->bitmapCharSizeWSpinBox->value())
            * ((double)ui->bitmapCharSizeWSpinBox->value() / (double)ui->bitmapCharSizeHSpinBox->value());

    Image image(Geometry(ui->bitmapCharSizeWSpinBox->value() * ratio,
                         ui->bitmapCharSizeHSpinBox->value() * ratio), Color("white"));

    drawList.push_back(DrawableTextAntialias(true));
    drawList.push_back(DrawableFont(ui->fontPathLineEdit->text().toStdString()));
    drawList.push_back(DrawablePointSize(BASE_IMAGE_SIZE));
    drawList.push_back(DrawableFillColor(Color(255, 255, 255, MaxRGB)));
    drawList.push_back(DrawableGravity(CenterGravity));
    drawList.push_back(DrawableText(0, 0, ui->previewComboBox->currentText().toStdString()));

    image.draw(drawList);

    image.write(m_previewFilePath.toStdString());
}

