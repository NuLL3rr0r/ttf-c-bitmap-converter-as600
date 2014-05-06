#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <Magick++.h>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    /*! Change to executable path */
    boost::filesystem::path path(boost::filesystem::initial_path<boost::filesystem::path>());
    if (argc > 0 && argv[0] != NULL)
        path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));
    std::string appId(path.filename().string());
    std::string appPath(boost::algorithm::replace_last_copy(path.string(), appId, ""));
    boost::filesystem::current_path(appPath);

#if defined ( _WIN32 )
    /*! We shoule do this, or, we'll crash */

    const std::string MGK_TYPE_FILE(
                (boost::filesystem::path(
                     boost::filesystem::path(appPath)
                     / boost::filesystem::path("type."
                                           #if defined ( MAGICKPP_IS_GM )
                                               "mgk"
                                           #elif defined ( MAGICKPP_IS_IM )
                                               "xml"
                                           #endif /* defined ( MAGICKPP_IS_GM ) */
                                               )).string()));

    if (!boost::filesystem::exists(MGK_TYPE_FILE)) {
        std::ofstream outFile(MGK_TYPE_FILE);
        if (outFile.is_open()) {
            outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
                    << "<typemap>" << std::endl
                    << std::endl
                    << "</typemap>" << std::endl;
        } else {
            MessageBoxA(0, "Application executable path is read-only.", "Error",
                        MB_OK | MB_ICONERROR);
            return EXIT_FAILURE;
        }
    }
#endif /* defined ( _WIN32 ) */

    Magick::InitializeMagick(*argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

