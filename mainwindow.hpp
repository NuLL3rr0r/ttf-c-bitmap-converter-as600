#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <vector>
#include <unordered_map>
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
    template <typename _T>
    struct Hasher
    {
        std::size_t operator()(const _T &t) const
        {
            return std::hash<unsigned char>()(static_cast<unsigned char>(t));
        }
    };

    enum class Glyph : unsigned char {
        None = 255,
        Detached = 0,
        Initial = 1,
        Medial = 2,
        Final = 3,
        Combined = 4
    };

    typedef std::unordered_map<wchar_t, std::vector<wchar_t>, Hasher<wchar_t>> GlyphsMap_t;

private:
    static const wchar_t s_glyphs[];
    static const wchar_t s_alphanumGlyphs[];
    static const wchar_t s_detachedGlyphs[];
    static const wchar_t s_initialGlyphs[];
    static const wchar_t s_medialGlyphs[];
    static const wchar_t s_finalGlyphs[];

private:
    Ui::MainWindow *ui;
    std::vector<QString> m_glyphs;
    GlyphsMap_t m_glyphsMap;
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
    void on_charScalingDoubleSpinBox_valueChanged(double arg1);
    void on_charRotationDoubleSpinBox_valueChanged(double arg1);
    void on_charSkewXDoubleSpinBox_valueChanged(double arg1);
    void on_charSkewYDoubleSpinBox_valueChanged(double arg1);
    void on_previewComboBox_currentIndexChanged(int index);
    void on_lcdOutputPushButton_clicked();
    void on_printerOutputPushButton_clicked();
    void on_convertButton_clicked();

private:
    void Preview();
    void ConvertToDisplayUnicode(const std::wstring &text, std::wstring &out_displayText,
                                 const bool bEncode = false);
};

#endif // MAINWINDOW_HPP
