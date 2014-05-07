#include <bitset>
#include <sstream>
#include <cmath>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <Magick++.h>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#define         BASE_IMAGE_SIZE                 96.0
#define         PREVIEW_FILE_NAME               L"preview.png"
#define         GLYPH_FILE_NAME                 L"glyph%1.png"

using namespace std;
using namespace boost;
using namespace Magick;

const wchar_t MainWindow::s_glyphs[] = {
    0x06F0, // ۰
    0x06F1, // ۱
    0x06F2, // ۲
    0x06F3, // ۳
    0x06F4, // ۴
    0x06F5, // ۵
    0x06F6, // ۶
    0x06F7, // ۷
    0x06F8, // ۸
    0x06F9, // ۹
    0xFE81, // ﺁ
    0xFE8D, // ﺍ
    0xFE8F, // ﺏ
    0xFB56, // ﭖ
    0xFE95, // ﺕ
    0xFE99, // ﺙ
    0xFE9D, // ﺝ
    0xFB7A, // ﭺ
    0xFEA1, // ﺡ
    0xFEA5, // ﺥ
    0xFEA9, // ﺩ
    0xFEAB, // ﺫ
    0xFEAD, // ﺭ
    0xFEAF, // ﺯ
    0xFB8A, // ﮊ
    0xFEB1, // ﺱ
    0xFEB5, // ﺵ
    0xFEB9, // ﺹ
    0xFEBD, // ﺽ
    0xFEC1, // ﻁ
    0xFEC5, // ﻅ
    0xFEC9, // ﻉ
    0xFECD, // ﻍ
    0xFED1, // ﻑ
    0xFED5, // ﻕ
    0xFB8E, // ﮎ
    0xFB92, // ﮒ
    0xFEDD, // ﻝ
    0xFEE1, // ﻡ
    0xFEE5, // ﻥ
    0xFEED, // ﻭ
    0xFEE9, // ﻩ
    0xFBFC, // ﯼ
    0xFE80, // ﺀ
    0xFE89, // ﺉ
    0xFEFB, // ﻻ
    0xFE91, // ﺑ
    0xFB58, // ﭘ
    0xFE97, // ﺗ
    0xFE9B, // ﺛ
    0xFE9F, // ﺟ
    0xFB7C, // ﭼ
    0xFEA3, // ﺣ
    0xFEA7, // ﺧ
    0xFEB3, // ﺳ
    0xFEB7, // ﺷ
    0xFEBB, // ﺻ
    0xFEBF, // ﺿ
    0xFEC3, // ﻃ
    0xFEC7, // ﻇ
    0xFECB, // ﻋ
    0xFECF, // ﻏ
    0xFED3, // ﻓ
    0xFED7, // ﻗ
    0xFB90, // ﮐ
    0xFB94, // ﮔ
    0xFEDF, // ﻟ
    0xFEE3, // ﻣ
    0xFEE7, // ﻧ
    0xFEEB, // ﻫ
    0xFBFE, // ﯾ
    0xFE80, // ﺀ
    0xFE8B, // ﺋ
    0xFE92, // ﺒ
    0xFB59, // ﭙ
    0xFE98, // ﺘ
    0xFE9C, // ﺜ
    0xFEA0, // ﺠ
    0xFB7D, // ﭽ
    0xFEA4, // ﺤ
    0xFEA8, // ﺨ
    0xFEB4, // ﺴ
    0xFEB8, // ﺸ
    0xFEBC, // ﺼ
    0xFEC0, // ﻀ
    0xFEC4, // ﻄ
    0xFEC8, // ﻈ
    0xFECC, // ﻌ
    0xFED0, // ﻐ
    0xFED4, // ﻔ
    0xFED8, // ﻘ
    0xFB91, // ﮑ
    0xFB95, // ﮕ
    0xFEE0, // ﻠ
    0xFEE4, // ﻤ
    0xFEE8, // ﻨ
    0xFEEC, // ﻬ
    0xFBFF, // ﯿ
    0xFE80, // ﺀ
    0xFE8C, // ﺌ
    0xFE8E, // ﺎ
    0xFE90, // ﺐ
    0xFB57, // ﭗ
    0xFE96, // ﺖ
    0xFE9A, // ﺚ
    0xFE9E, // ﺞ
    0xFB7B, // ﭻ
    0xFEA2, // ﺢ
    0xFEA6, // ﺦ
    0xFEAA, // ﺪ
    0xFEAC, // ﺬ
    0xFEAE, // ﺮ
    0xFEB0, // ﺰ
    0xFB8B, // ﮋ
    0xFEB2, // ﺲ
    0xFEB6, // ﺶ
    0xFEBA, // ﺺ
    0xFEBE, // ﺾ
    0xFEC2, // ﻂ
    0xFEC6, // ﻆ
    0xFECA, // ﻊ
    0xFECE, // ﻎ
    0xFED2, // ﻒ
    0xFED6, // ﻖ
    0xFB8F, // ﮏ
    0xFB93, // ﮓ
    0xFEDE, // ﻞ
    0xFEE2, // ﻢ
    0xFEE6, // ﻦ
    0xFEEE, // ﻮ
    0xFEEA, // ﻪ
    0xFBFD, // ﯽ
    0xFE80, // ﺀ
    0xFE8A, // ﺊ
    0xFEFC, // ﻼ
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    {
        size_t len = sizeof(s_glyphs) / sizeof(wchar_t);
        for (size_t i = 0; i < len; ++i) {
            m_glyphs.push_back(QString::fromStdWString(wstring(1, s_glyphs[i])));
        }
    }

    if (m_tempDir.isValid()) {
        m_previewFilePath = QString::fromStdWString((boost::filesystem::path(m_tempDir.path().toStdWString())
                                                     / boost::filesystem::path(PREVIEW_FILE_NAME)).wstring());
        m_glyphFilePath = QString::fromStdWString((boost::filesystem::path(m_tempDir.path().toStdWString())
                                                   / boost::filesystem::path(GLYPH_FILE_NAME)).wstring());
    }

    ui->setupUi(this);
    this->setFixedSize(483, 261);

    for (std::vector<QString>::const_iterator it = m_glyphs.begin();
         it != m_glyphs.end(); ++it) {
        ui->previewComboBox->addItem(*it);
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
    double ratio = (BASE_IMAGE_SIZE / (double)ui->bitmapCharSizeWSpinBox->value())
            * ((double)ui->bitmapCharSizeWSpinBox->value() / (double)ui->bitmapCharSizeHSpinBox->value());

    Image image(Geometry(ui->bitmapCharSizeWSpinBox->value() * ratio,
                         ui->bitmapCharSizeHSpinBox->value() * ratio),
                Color("white"));
    list<Drawable> drawList;
    drawList.push_back(DrawableTextAntialias(true));
    drawList.push_back(DrawableFont(ui->fontPathLineEdit->text().toStdString()));
    drawList.push_back(DrawablePointSize(BASE_IMAGE_SIZE - ui->charPaddingSpinBox->value()));
    drawList.push_back(DrawableFillColor(Color("black")));
    drawList.push_back(DrawableGravity(CenterGravity));
    drawList.push_back(DrawableText(0, 0, ui->previewComboBox->currentText().toStdString()));
    image.draw(drawList);
    image.write(m_previewFilePath.toStdString());

    QPixmap preview(m_previewFilePath);
    ui->previewLabel->setPixmap(preview);
}

void MainWindow::on_bitmapCharSizeWSpinBox_valueChanged(int arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_bitmapCharSizeHSpinBox_valueChanged(int arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_charPaddingSpinBox_valueChanged(int arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_previewComboBox_currentIndexChanged(int index)
{
    (void)index;
    Preview();
}

void MainWindow::on_lcdOutputPushButton_clicked()
{
    ui->outputTextEdit->clear();

    const int COLS= ui->bitmapCharSizeWSpinBox->value();
    const int ROWS = ui->bitmapCharSizeHSpinBox->value();
    const double RATIO = (BASE_IMAGE_SIZE / COLS)
            * ((double)COLS / (double)ROWS);

    size_t glyphsImagesCount = 0;
    for (std::vector<QString>::const_iterator it = m_glyphs.begin();
             it != m_glyphs.end(); ++it) {
        Image image(Geometry(COLS * RATIO, ROWS * RATIO),
                    Color("white"));
        list<Drawable> drawList;
        drawList.push_back(DrawableTextAntialias(true));
        drawList.push_back(DrawableFont(ui->fontPathLineEdit->text().toStdString()));
        drawList.push_back(DrawablePointSize(BASE_IMAGE_SIZE - ui->charPaddingSpinBox->value()));
        drawList.push_back(DrawableFillColor(Color("black")));
        drawList.push_back(DrawableGravity(CenterGravity));
        drawList.push_back(DrawableText(0, 0, it->toStdString()));
        image.draw(drawList);
        Geometry g(COLS, ROWS);
        image.scale(g);
        image.write(m_glyphFilePath.arg(glyphsImagesCount).toStdString());
        ++glyphsImagesCount;
    }

    double tolerance = 1.0 - ui->colorToleranceDoubleSpinBox->value();
    vector<vector<vector<std::string>>> glyphsBitmap;
    for (size_t i = 0; i < glyphsImagesCount; ++i) {
        vector<vector<std::string>> glyphBitmap;
        Image image;
        image.read(m_glyphFilePath.arg(i).toStdString());
        size_t columns = image.columns();
        size_t rows = image.rows();
        PixelPacket *pixels = image.getPixels(0, 0, columns, rows);
        for (size_t row = 0; row < rows; ++row) {
            vector<std::string> rowBitmap;
            for (size_t column = 0; column < columns; ++column) {
                ColorRGB color(pixels[columns * row + column]);
                if (color.red() <= tolerance && color.green() <= tolerance && color.blue() <= tolerance) {
                    rowBitmap.push_back("1");
                } else {
                    rowBitmap.push_back("0");
                }
            }
            glyphBitmap.push_back(rowBitmap);
        }
        glyphsBitmap.push_back(glyphBitmap);
    }

    vector<QString> glyphsCBitmap;
    size_t glyphIndex = 0;
    for (vector<vector<vector<std::string>>>::const_iterator it = glyphsBitmap.begin();
         it != glyphsBitmap.end(); ++it) {

        std::string glyphCBitmap;
        int verticalBytes = ceil(ROWS / 8.0);
        for (int c = 0; c < COLS; ++c) {
            for (int b = 0; b < verticalBytes; ++b) {
                std::string cBitmap;
                for (int r = (((b + 1) * 8) - 1); r >= (b * 8); --r) {
                    if (r < ROWS) {
                        cBitmap += (*it)[r][c];
                    } else {
                        cBitmap += "0";
                    }
                }

                bitset<8> set(cBitmap);
                stringstream res;
                res << hex << uppercase << set.to_ulong();
                std::string hexCBitmap(res.str());
                if (hexCBitmap.size() != 2)
                    hexCBitmap = "0" + hexCBitmap;
                hexCBitmap = "0x" + hexCBitmap;

                glyphCBitmap += hexCBitmap + ", ";
            }
        }

        glyphsCBitmap.push_back(QString::fromStdString(glyphCBitmap)
                                + " // " + QString::fromStdWString(wstring(1, s_glyphs[glyphIndex])));
        ++glyphIndex;
    }

    ui->outputTextEdit->setText((format("/* %1%x%2% '%3% Byte(s)' AS600-mini LCD Font */\n")
                                 % COLS % ROWS % (ceil(ROWS / 8.0) * COLS)).str().c_str());
    for (vector<QString>::const_iterator it = glyphsCBitmap.begin();
         it != glyphsCBitmap.end(); ++it) {
        ui->outputTextEdit->setText(ui->outputTextEdit->toPlainText() + (*it) + "\n");
    }
}

void MainWindow::on_printerOutputPushButton_clicked()
{
    ui->outputTextEdit->clear();

    const int COLS= ui->bitmapCharSizeWSpinBox->value();
    const int ROWS = ui->bitmapCharSizeHSpinBox->value();
    const double RATIO = (BASE_IMAGE_SIZE / COLS)
            * ((double)COLS / (double)ROWS);

    size_t glyphsImagesCount = 0;
    for (std::vector<QString>::const_iterator it = m_glyphs.begin();
             it != m_glyphs.end(); ++it) {
        Image image(Geometry(COLS * RATIO, ROWS * RATIO),
                    Color("white"));
        list<Drawable> drawList;
        drawList.push_back(DrawableTextAntialias(true));
        drawList.push_back(DrawableFont(ui->fontPathLineEdit->text().toStdString()));
        drawList.push_back(DrawablePointSize(BASE_IMAGE_SIZE - ui->charPaddingSpinBox->value()));
        drawList.push_back(DrawableFillColor(Color("black")));
        drawList.push_back(DrawableGravity(CenterGravity));
        drawList.push_back(DrawableText(0, 0, it->toStdString()));
        image.draw(drawList);
        Geometry g(COLS, ROWS);
        image.scale(g);
        image.write(m_glyphFilePath.arg(glyphsImagesCount).toStdString());
        ++glyphsImagesCount;
    }

    double tolerance = 1.0 - ui->colorToleranceDoubleSpinBox->value();
    vector<vector<vector<std::string>>> glyphsBitmap;
    for (size_t i = 0; i < glyphsImagesCount; ++i) {
        vector<vector<std::string>> glyphBitmap;
        Image image;
        image.read(m_glyphFilePath.arg(i).toStdString());
        size_t columns = image.columns();
        size_t rows = image.rows();
        PixelPacket *pixels = image.getPixels(0, 0, columns, rows);
        for (size_t row = 0; row < rows; ++row) {
            vector<std::string> rowBitmap;
            for (size_t column = 0; column < columns; ++column) {
                ColorRGB color(pixels[columns * row + column]);
                if (color.red() <= tolerance && color.green() <= tolerance && color.blue() <= tolerance) {
                    rowBitmap.push_back("1");
                } else {
                    rowBitmap.push_back("0");
                }
            }
            glyphBitmap.push_back(rowBitmap);
        }
        glyphsBitmap.push_back(glyphBitmap);
    }

    vector<QString> glyphsCBitmap;
    size_t glyphIndex = 0;
    for (vector<vector<vector<std::string>>>::const_iterator it = glyphsBitmap.begin();
         it != glyphsBitmap.end(); ++it) {

        std::string glyphCBitmap;
        int horizontalBytes = ceil(COLS / 8.0);
        for (int r = 0; r < ROWS; ++r) {
            for (int b = 0; b < horizontalBytes; ++b) {
                std::string cBitmap;
                for (int c = (b * 8); c < ((b + 1) * 8); ++c) {
                    if (c < COLS) {
                        cBitmap += (*it)[r][c];
                    } else {
                        cBitmap += "0";
                    }
                }

                bitset<8> set(cBitmap);
                stringstream res;
                res << hex << uppercase << set.to_ulong();
                std::string hexCBitmap(res.str());
                if (hexCBitmap.size() != 2)
                    hexCBitmap = "0" + hexCBitmap;
                hexCBitmap = "0x" + hexCBitmap;

                glyphCBitmap += hexCBitmap + ", ";
            }
        }

        glyphsCBitmap.push_back(QString::fromStdString(glyphCBitmap)
                                + " // " + QString::fromStdWString(wstring(1, s_glyphs[glyphIndex])));
        ++glyphIndex;
    }

    ui->outputTextEdit->setText((format("/* %1%x%2% '%3% Byte(s)' AS600-mini Printer Font */\n")
                                 % ROWS % COLS % (ceil(COLS / 8.0) * ROWS)).str().c_str());
    for (vector<QString>::const_iterator it = glyphsCBitmap.begin();
         it != glyphsCBitmap.end(); ++it) {
        ui->outputTextEdit->setText(ui->outputTextEdit->toPlainText() + (*it) + "\n");
    }
}
