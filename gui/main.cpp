#include "Ui/EZGit.h"

#include <QApplication>
#include <QIcon>
#include <cstring>

#include "Version.h"

int main( int argc, char *argv[] )
{
    Q_INIT_RESOURCE( resources );
    QApplication appl( argc, argv );

    NVersion::setupApplication( appl, true );
    //auto img = NUi::NUtils::getSVG( ":/resources/application.svg", { 48, 38 } );
    //Q_ASSERT( img.has_value() );

    //QIcon icon( img.value() );
    //appl.setWindowIcon( icon );

    NUi::CEZGit ezGit;
    ezGit.show();

    return appl.exec();
}
