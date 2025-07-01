#include "Ui/EZGit.h"

#include <QApplication>
#include <QIcon>

#include "Version.h"

int main( int argc, char *argv[] )
{
    Q_INIT_RESOURCE( resources );
    QApplication appl( argc, argv );

    NVersion::setupApplication( appl, true );

    QIcon icon( ":application.png" );
    appl.setWindowIcon( icon );


    NUi::CEZGit ezGit;
    ezGit.show();

    return appl.exec();
}
