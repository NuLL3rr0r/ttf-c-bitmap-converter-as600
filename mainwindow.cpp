#include <algorithm>
#include <bitset>
#include <exception>
#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility.hpp>
#include <Magick++.h>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#define         SPACE_GLYPH                     0x0020
#define         GLYPH_IS_NOT_DEFINED            0x0000

#define         BASE_IMAGE_SIZE                 96.0
#define         PREVIEW_FILE_NAME               L"preview.png"
#define         GLYPH_FILE_NAME                 L"glyph%1.png"

#define         ERROR_TITLE                     "Error"
#define         UNKNOWN_ERROR                   "Unknown Error!"

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
    0xFEFB, // ﻻ
    0xFEFC, // ﻼ
};

const wchar_t MainWindow::s_alphanumGlyphs[] = {
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
    0x0622, // آ
    0x0627, // ا
    0x0628, // ب
    0x067E, // پ
    0x062A, // ت
    0x062B, // ث
    0x062C, // ج
    0x0686, // چ
    0x062D, // ح
    0x062E, // خ
    0x062F, // د
    0x0630, // ذ
    0x0631, // ر
    0x0632, // ز
    0x0698, // ژ
    0x0633, // س
    0x0634, // ش
    0x0635, // ص
    0x0636, // ض
    0x0637, // ط
    0x0638, // ظ
    0x0639, // ع
    0x063A, // غ
    0x0641, // ف
    0x0642, // ق
    0x06A9, // ک
    0x06AF, // گ
    0x0644, // ل
    0x0645, // م
    0x0646, // ن
    0x0648, // و
    0x0647, // ه
    0x06CC, // ی
    0x0621, // ء
    0x0626, // ئ
};

const wchar_t MainWindow::s_detachedGlyphs[] = {
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
};

const wchar_t MainWindow::s_initialGlyphs[] = {
    GLYPH_IS_NOT_DEFINED, // ۰
    GLYPH_IS_NOT_DEFINED, // ۱
    GLYPH_IS_NOT_DEFINED, // ۲
    GLYPH_IS_NOT_DEFINED, // ۳
    GLYPH_IS_NOT_DEFINED, // ۴
    GLYPH_IS_NOT_DEFINED, // ۵
    GLYPH_IS_NOT_DEFINED, // ۶
    GLYPH_IS_NOT_DEFINED, // ۷
    GLYPH_IS_NOT_DEFINED, // ۸
    GLYPH_IS_NOT_DEFINED, // ۹
    GLYPH_IS_NOT_DEFINED, // ﺁ
    GLYPH_IS_NOT_DEFINED, // ﺍ
    0xFE91, // ﺑ
    0xFB58, // ﭘ
    0xFE97, // ﺗ
    0xFE9B, // ﺛ
    0xFE9F, // ﺟ
    0xFB7C, // ﭼ
    0xFEA3, // ﺣ
    0xFEA7, // ﺧ
    GLYPH_IS_NOT_DEFINED, // ﺩ
    GLYPH_IS_NOT_DEFINED, // ﺫ
    GLYPH_IS_NOT_DEFINED, // ﺭ
    GLYPH_IS_NOT_DEFINED, // ﺯ
    GLYPH_IS_NOT_DEFINED, // ﮊ
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
    GLYPH_IS_NOT_DEFINED, // ﻭ
    0xFEEB, // ﻫ
    0xFBFE, // ﯾ
    0xFE80, // ﺀ
    0xFE8B, // ﺋ
};

const wchar_t MainWindow::s_medialGlyphs[] = {
    GLYPH_IS_NOT_DEFINED, // ۰
    GLYPH_IS_NOT_DEFINED, // ۱
    GLYPH_IS_NOT_DEFINED, // ۲
    GLYPH_IS_NOT_DEFINED, // ۳
    GLYPH_IS_NOT_DEFINED, // ۴
    GLYPH_IS_NOT_DEFINED, // ۵
    GLYPH_IS_NOT_DEFINED, // ۶
    GLYPH_IS_NOT_DEFINED, // ۷
    GLYPH_IS_NOT_DEFINED, // ۸
    GLYPH_IS_NOT_DEFINED, // ۹
    GLYPH_IS_NOT_DEFINED, // ﺁ
    GLYPH_IS_NOT_DEFINED, // ﺎ
    0xFE92, // ﺒ
    0xFB59, // ﭙ
    0xFE98, // ﺘ
    0xFE9C, // ﺜ
    0xFEA0, // ﺠ
    0xFB7D, // ﭽ
    0xFEA4, // ﺤ
    0xFEA8, // ﺨ
    GLYPH_IS_NOT_DEFINED, // ﺪ
    GLYPH_IS_NOT_DEFINED, // ﺬ
    GLYPH_IS_NOT_DEFINED, // ﺮ
    GLYPH_IS_NOT_DEFINED, // ﺰ
    GLYPH_IS_NOT_DEFINED, // ﮋ
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
    GLYPH_IS_NOT_DEFINED, // ﻮ
    0xFEEC, // ﻬ
    0xFBFF, // ﯿ
    0xFE80, // ﺀ
    0xFE8C, // ﺌ
};

const wchar_t MainWindow::s_finalGlyphs[] = {
    GLYPH_IS_NOT_DEFINED, // ۰
    GLYPH_IS_NOT_DEFINED, // ۱
    GLYPH_IS_NOT_DEFINED, // ۲
    GLYPH_IS_NOT_DEFINED, // ۳
    GLYPH_IS_NOT_DEFINED, // ۴
    GLYPH_IS_NOT_DEFINED, // ۵
    GLYPH_IS_NOT_DEFINED, // ۶
    GLYPH_IS_NOT_DEFINED, // ۷
    GLYPH_IS_NOT_DEFINED, // ۸
    GLYPH_IS_NOT_DEFINED, // ۹
    GLYPH_IS_NOT_DEFINED, // ﺁ
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

        for (size_t i = 32; i < 127; ++i) {
            m_glyphs.push_back(QString::fromStdString(string(1, (char)i)));
        }
    }

    {
        size_t len = sizeof(s_alphanumGlyphs) / sizeof(wchar_t);
        for (size_t i = 0; i < len; ++i) {
            std::vector<wchar_t> vec;
            vec.push_back(s_detachedGlyphs[i]);
            vec.push_back(s_initialGlyphs[i]);
            vec.push_back(s_medialGlyphs[i]);
            vec.push_back(s_finalGlyphs[i]);
            m_glyphsMap[s_alphanumGlyphs[i]] = vec;
        }
    }

    if (m_tempDir.isValid()) {
        m_previewFilePath = QString::fromStdWString((boost::filesystem::path(m_tempDir.path().toStdWString())
                                                     / boost::filesystem::path(PREVIEW_FILE_NAME)).wstring());
        m_glyphFilePath = QString::fromStdWString((boost::filesystem::path(m_tempDir.path().toStdWString())
                                                   / boost::filesystem::path(GLYPH_FILE_NAME)).wstring());
    }

    ui->setupUi(this);
    this->setFixedSize(830, 461);

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

void MainWindow::on_charScalingDoubleSpinBox_valueChanged(double arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_charRotationDoubleSpinBox_valueChanged(double arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_charSkewXDoubleSpinBox_valueChanged(double arg1)
{
    (void)arg1;
    Preview();
}

void MainWindow::on_charSkewYDoubleSpinBox_valueChanged(double arg1)
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
    try {
        ui->outputBitmapCharsTextEdit->clear();

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
            drawList.push_back(DrawablePointSize((BASE_IMAGE_SIZE
                                                  - (ui->charPaddingSpinBox->value()))
                                                 * ui->charScalingDoubleSpinBox->value()));
            drawList.push_back(DrawableFillColor(Color("black")));
            drawList.push_back(DrawableGravity(CenterGravity));
            drawList.push_back(DrawableRotation(ui->charRotationDoubleSpinBox->value()));
            drawList.push_back(DrawableSkewX(ui->charSkewXDoubleSpinBox->value()));
            drawList.push_back(DrawableSkewY(ui->charSkewYDoubleSpinBox->value()));
            drawList.push_back(DrawableText(0, 0, it->toStdString(), "UTF-8"));
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

        ui->outputBitmapCharsTextEdit->setText((format("/* %1%x%2% '%3% Byte(s)' AS600-mini LCD Font */\n")
                                     % COLS % ROWS % (ceil(ROWS / 8.0) * COLS)).str().c_str());
        for (vector<QString>::const_iterator it = glyphsCBitmap.begin();
             it != glyphsCBitmap.end(); ++it) {
            ui->outputBitmapCharsTextEdit->setText(ui->outputBitmapCharsTextEdit->toPlainText() + (*it) + "\n");
        }
    }

    catch(const Magick::ErrorDraw &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Error &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const std::exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch (...) {
        QMessageBox::critical(this, ERROR_TITLE, UNKNOWN_ERROR);
    }
}

void MainWindow::on_printerOutputPushButton_clicked()
{
    try {
        ui->outputBitmapCharsCodeTextEdit->clear();
        ui->outputBitmapCharsTextEdit->clear();

        {
            size_t i = 0;
            for (std::vector<QString>::const_iterator it = m_glyphs.begin();
                 it != m_glyphs.end(); ++it) {
                wstringstream code;
                code << hex << uppercase << i;
                ui->outputBitmapCharsCodeTextEdit->setText(
                            QString::fromStdWString((wformat(L"%3%0x80,0x%1%, // %2%\n")
                             % (code.str().size() == 2 ? code.str() : (L"0" + code.str()))
                             % it->toStdWString()
                             % ui->outputBitmapCharsCodeTextEdit->toPlainText().toStdWString()).str().c_str()));
                ++i;
            }
        }

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
            drawList.push_back(DrawablePointSize((BASE_IMAGE_SIZE
                                                  - (ui->charPaddingSpinBox->value()))
                                                 * ui->charScalingDoubleSpinBox->value()));
            drawList.push_back(DrawableFillColor(Color("black")));
            drawList.push_back(DrawableGravity(CenterGravity));
            drawList.push_back(DrawableRotation(ui->charRotationDoubleSpinBox->value()));
            drawList.push_back(DrawableSkewX(ui->charSkewXDoubleSpinBox->value()));
            drawList.push_back(DrawableSkewY(ui->charSkewYDoubleSpinBox->value()));
            drawList.push_back(DrawableText(0, 0, it->toStdString(), "UTF-8"));
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
                                    + " // " + m_glyphs[glyphIndex]);
            ++glyphIndex;
        }

        ui->outputBitmapCharsTextEdit->setText((format("/* %1%x%2% '%3% Byte(s)' AS600-mini Printer Font */\n")
                                     % ROWS % COLS % (ceil(COLS / 8.0) * ROWS)).str().c_str());
        for (vector<QString>::const_iterator it = glyphsCBitmap.begin();
             it != glyphsCBitmap.end(); ++it) {
            ui->outputBitmapCharsTextEdit->setText(ui->outputBitmapCharsTextEdit->toPlainText() + (*it) + "\n");
        }
    }

    catch(const Magick::ErrorDraw &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Error &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const std::exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch (...) {
        QMessageBox::critical(this, ERROR_TITLE, UNKNOWN_ERROR);
    }
}

void MainWindow::on_convertButton_clicked()
{
    wstring output;
    ConvertToDisplayUnicode(ui->sourceTextEdit->toPlainText().toStdWString(),
                            output, ui->encodeCheckBox->isChecked());
    ui->displayUnicodeTextTextEdit->setText(QString::fromStdWString(output));
}

void MainWindow::Preview()
{
    try {
        double ratio = (BASE_IMAGE_SIZE / (double)ui->bitmapCharSizeWSpinBox->value())
                * ((double)ui->bitmapCharSizeWSpinBox->value()
                   / (double)ui->bitmapCharSizeHSpinBox->value());

        Image image(Geometry(ui->bitmapCharSizeWSpinBox->value() * ratio,
                             ui->bitmapCharSizeHSpinBox->value() * ratio),
                    Color("white"));
        list<Drawable> drawList;
        drawList.push_back(DrawableTextAntialias(true));
        drawList.push_back(DrawableFont(ui->fontPathLineEdit->text().toStdString()));
        drawList.push_back(DrawablePointSize((BASE_IMAGE_SIZE
                                              - (ui->charPaddingSpinBox->value()))
                                             * ui->charScalingDoubleSpinBox->value()));
        drawList.push_back(DrawableFillColor(Color("black")));
        drawList.push_back(DrawableGravity(CenterGravity));
        drawList.push_back(DrawableRotation(ui->charRotationDoubleSpinBox->value()));
        drawList.push_back(DrawableSkewX(ui->charSkewXDoubleSpinBox->value()));
        drawList.push_back(DrawableSkewY(ui->charSkewYDoubleSpinBox->value()));
        drawList.push_back(DrawableText(0, 0,
                                        ui->previewComboBox->currentText().toStdString(),
                                        "UTF-8"));
        image.draw(drawList);
        image.write(m_previewFilePath.toStdString());

        QPixmap preview(m_previewFilePath);
        ui->previewLabel->setPixmap(preview);
    }

    catch(const Magick::ErrorDraw &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Error &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const Magick::Exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch(const std::exception &ex) {
        QMessageBox::critical(this, ERROR_TITLE, ex.what());
    }

    catch (...) {
        QMessageBox::critical(this, ERROR_TITLE, UNKNOWN_ERROR);
    }
}

void MainWindow::ConvertToDisplayUnicode(const std::wstring &text, std::wstring &out_displayText,
                                         const bool bEncode)
{
    out_displayText.clear();

    size_t len = text.length();
    Glyph lastCharType = Glyph::None;

    for (size_t i = 0; i < len; ++i) {
        const wchar_t ch = text[i];

        if (ch == SPACE_GLYPH) {
            lastCharType = Glyph::None;
            out_displayText += ch;
            continue;
        }

        GlyphsMap_t::const_iterator it = m_glyphsMap.find(ch);

        if (it != m_glyphsMap.end()) {
            std::vector<wchar_t> vec(it->second);

            Glyph type = Glyph::None;

            wchar_t pre;
            wchar_t post;

            bool isFirstChar = (i == 0);
            bool isLastChar = (i == len - 1);

            if (!isFirstChar && !isLastChar) {
                pre = text[i - 1];
                post = text[i + 1];

                bool hasPreSpace = (pre == SPACE_GLYPH)
                        || m_glyphsMap.find(pre) == m_glyphsMap.end();
                bool hasPostSpace = (post == SPACE_GLYPH)
                        || m_glyphsMap.find(post) == m_glyphsMap.end();

                if (ch == 0x0644 && post == 0x0627) { // ل & ا
                    type = Glyph::Combined;
                    if (m_glyphsMap.find(pre) == m_glyphsMap.end()) {
                        out_displayText += 0xFEFB; // ﻻ
                    } else {
                        out_displayText += 0xFEFC; // ﻼ
                    }
                    ++i;
                    continue;
                }

                if (hasPreSpace && hasPostSpace) {
                    type = Glyph::Detached;
                }
                else if (hasPreSpace && !hasPostSpace) {
                    type = Glyph::Initial;
                }
                else if (!hasPreSpace && !hasPostSpace) {
                    type = Glyph::Medial;
                }
                else if (!hasPreSpace && hasPostSpace) {
                    type = Glyph::Final;
                }
            }
            else {
                if (isFirstChar) {
                    post = text[i + 1];

                    if (ch == 0x0644 && post == 0x0627) { // ل & ا
                        type = Glyph::Combined;
                        out_displayText += 0xFEFB; // ﻻ
                        ++i;
                        continue;
                    }

                    if (post == SPACE_GLYPH
                            || m_glyphsMap.find(post) == m_glyphsMap.end()){
                        type = Glyph::Detached;
                    } else {
                        type = Glyph::Initial;
                    }
                }

                if (isLastChar) {
                    pre = text[i - 1];

                    if (pre == SPACE_GLYPH
                            || m_glyphsMap.find(pre) == m_glyphsMap.end()) {
                        type = Glyph::Detached;
                    } else {
                        type = Glyph::Final;
                    }
                }
            }

            if (type != Glyph::None) {
                if (vec[(wchar_t)type] == GLYPH_IS_NOT_DEFINED) {
                    switch (type) {
                    case Glyph::Initial:
                        type = Glyph::Detached;
                        break;
                    case Glyph::Medial:
                        type = Glyph::Final;
                        break;
                    default:
                        break;
                    }
                }

                if (lastCharType == Glyph::None
                        || lastCharType == Glyph::Detached
                        || lastCharType == Glyph::Final) {
                    switch (type) {
                    case Glyph::Medial:
                        type = Glyph::Initial;
                        break;
                    case Glyph::Final:
                        type = Glyph::Detached;
                        break;
                    default:
                        break;
                    }
                }

                out_displayText += vec[(wchar_t)type];
            }

            lastCharType = type;
            vec.clear();
            continue;
        }

        out_displayText += ch;
    }

    {
        wstring temp;
        wstring tempLtr;
        bool foundLtr = false;
        const wchar_t *end = s_glyphs + (sizeof(s_glyphs) / sizeof(wchar_t));
        for (int i = out_displayText.size() - 1; i >= 0; --i) {
            const wchar_t *glyph = std::find(s_glyphs,
                                             end,
                                             out_displayText[i]);
            if (glyph != end) {
                if (foundLtr) {
                    std::reverse(tempLtr.begin(), tempLtr.end());
                    temp += tempLtr;
                    tempLtr.clear();
                }
                foundLtr = false;
                temp += out_displayText[i];
            } else {
                foundLtr = true;
                tempLtr += out_displayText[i];
            }
        }

        out_displayText = temp;
    }

    {
        std::vector<std::wstring> lines;
        boost::split(lines, out_displayText, boost::is_any_of(L"\n"));

        out_displayText.clear();
        std::vector<std::wstring>::const_reverse_iterator lastLine = lines.rend();
        if (lines.size() > 0)
            --lastLine;
        for (std::vector<std::wstring>::const_reverse_iterator rit
             = lines.rbegin(); rit != lines.rend(); ++rit) {
            out_displayText += (*rit);
            if (rit != lines.rend() && rit != lastLine)
                out_displayText += L'\n';
        }
    }

    if (bEncode) {
        std::wstring temp;
        for (size_t i = 0; i < out_displayText.length(); ++i) {
            wstringstream code;
            size_t pos = 0;
            bool found = false;
            for (size_t c = 0; c < m_glyphs.size(); ++c) {
                if (m_glyphs[c].toStdWString()[0] == out_displayText[i]) {
                    found = true;
                    break;
                }
                ++pos;
            }
            if (found) {
                code << hex << uppercase << pos;
                temp += (wformat(L"%2%0x80,0x%1%, ")
                         % (code.str().size() == 2
                            ? code.str() : (L"0" + code.str()))
                         % ui->outputBitmapCharsCodeTextEdit->toPlainText()
                      .toStdWString()).str().c_str();
            } else {
                temp += (wformat(L"%2%'%1%', ")
                         % (out_displayText[i] != L'\n' ? wstring(1, out_displayText[i]) : L"\\n")
                         % ui->outputBitmapCharsCodeTextEdit->toPlainText()
                      .toStdWString()).str().c_str();
            }
        }

        out_displayText = temp;
    }
}

