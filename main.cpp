#include <QtGui/QApplication>
#include "vqbform.h"

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>
#include <KDebug>

int main(int argc, char *argv[])
{
//  QApplication a(argc, argv);
    KAboutData about( "vqb",
                  0,
                  ki18n("VQB"),
                  "0.1",
                  ki18n("Visual Query Builder for SPARQL based on the local Nepomuk storage"),
                  KAboutData::License_GPL,
                  ki18n("(C) 2009 Oszkar Ambrus"),
                  KLocalizedString(),
                  0,
                  "aoszkar@gmail.com");
    about.addAuthor( ki18n("Oszkar Ambrus"), KLocalizedString(), "aoszkar@gmail.com" );
    KCmdLineOptions options;
    KCmdLineArgs::init( argc, argv, &about);
    //options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;
    VqbForm w;
    w.show();
    return app.exec();
}
