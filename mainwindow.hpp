#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <vector>
#include <QtCore/QString>
#include <QtCore/QTemporaryDir>
#include <QtWidgets/QMainWindow>

namespace Magick {
    class Image;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static const wchar_t s_glyphs[];

private:
    std::vector<QString> m_glyphs;
    Ui::MainWindow *ui;
    QTemporaryDir m_tempDir;
    QString m_previewFilePath;
    QString m_glyphFilePath;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectFontPushButton_clicked();
    void on_bitmapCharSizeWSpinBox_valueChanged(int arg1);
    void on_bitmapCharSizeHSpinBox_valueChanged(int arg1);
    void on_charPaddingSpinBox_valueChanged(int arg1);
    void on_previewComboBox_currentIndexChanged(int index);
    void on_lcdOutputPushButton_clicked();
    void on_printerOutputPushButton_clicked();

private:
    //void GetGlyphThumb(const QString &glyph, Magick::Image &image);
    void Preview();
};

#endif // MAINWINDOW_HPP
