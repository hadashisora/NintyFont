// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This just has the program entry point...
// nothing interesting
#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int32_t main(int32_t argc, char *argv[])
{
    QApplication a(argc, argv);
#if defined(_WIN32) || defined(_WIN64)
    a.setStyle(QStyleFactory::create("windowsvista")); //This is at least attempts to use the Aero theme on Windows if it's possible, so people don't have to poke either eyes out
#endif
    NintyFont::GUI::MainWindow w;
    w.show();
    return a.exec();
}
