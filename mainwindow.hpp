#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <vector>
#include <QtCore/QString>
#include <QtCore/QTemporaryDir>
#include <QtWidgets/QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static std::vector<std::wstring> s_glyphs;

private:
    Ui::MainWindow *ui;
    QTemporaryDir m_tempDir;
    QString m_previewFilePath;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectFontPushButton_clicked();

private:
    void Preview();
};

#endif // MAINWINDOW_HPP
