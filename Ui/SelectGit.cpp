//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectGit.h"
#include "EZGit.h"

#include "ui_SelectGit.h"

#include "UiFwd.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
namespace NUi
{
    CSelectGit::CSelectGit( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectGit )
    {
        fImpl->setupUi( this );

        registerField( QString( GIT_EXEC_FIELD ) + "*", fImpl->git );
        registerField( QString( SHOW_INIT_PAGES_FIELD ), fImpl->showNextTme );

        connect( fImpl->git, &QLineEdit::textChanged, this, &CSelectGit::completeChanged );

        connect(
            fImpl->selectGIT, &QToolButton::clicked,
            [ = ]()
            {
                QString defaultDir = fImpl->git->text();
                if ( defaultDir.isEmpty() )
                {
                    auto programDirs = QStandardPaths::standardLocations( QStandardPaths::StandardLocation::ApplicationsLocation );
                    if ( !programDirs.isEmpty() )
                        defaultDir = programDirs.front();
                }

                auto gitExec = QFileDialog::getOpenFileName( this, tr( "Select GIT Executable" ), defaultDir, tr( "Executable Files (*.exe)" ) );
                if ( gitExec.isEmpty() )
                    return;

                auto fi = QFileInfo( gitExec );
                if ( fi.exists() && fi.isFile() && fi.isExecutable() )
                    fImpl->git->setText( gitExec );
                else
                    QMessageBox::critical( this, tr( "Invalid Executable" ), tr( "'%1' is not an executable with the correct permissions" ).arg( gitExec ) );
            } );
    }

    CSelectGit::~CSelectGit()
    {
    }

    bool CSelectGit::isComplete() const
    {
        auto exec = field( GIT_EXEC_FIELD ).toString();
        if ( exec.isEmpty() )
            return false;

        auto fi = QFileInfo( exec );
        return fi.exists() && fi.isFile() && fi.isExecutable();
    }

    int CSelectGit::nextId() const
    {
        if ( ezGit()->showSelectRemote() )
            return toInt( EPageID::eSelectRemote );
        else if ( ezGit()->showSetCredentials() )
            return toInt( EPageID::eSetCredentials );
        return toInt( EPageID::eSelectGoal );
    }
    
    bool CSelectGit::validatePage()
    {
        return ezGit()->confirmCredentialManager();
    }


}
