//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectLocalDir.h"

#include "ui_SelectLocalDir.h"

#include "UiFwd.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace NUi
{
    CSelectLocalDir::CSelectLocalDir( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectLocalDir )
    {
        fImpl->setupUi( this );
        registerField( QString( REPO_DIR_FIELD ) + "*", fImpl->localDir );

        connect( fImpl->localDir, &QLineEdit::textChanged, this, &CSelectLocalDir::completeChanged );

        connect(
            fImpl->selectLocalDir, &QToolButton::clicked,   //
            [ = ]()
            {
                auto curr = fImpl->localDir->text();
                if ( curr.isEmpty() )
                    curr = field( SANDBOX_FIELD ).toString();

                auto repo = QFileDialog::getExistingDirectory( this, tr( "Select Sandbox:" ), curr );
                if ( repo.isEmpty() )
                    return;

                if ( isGitDir( repo, true ) )
                    fImpl->localDir->setText( repo );
            } );
    }

    CSelectLocalDir::~CSelectLocalDir()
    {
    }

    bool CSelectLocalDir::isGitDir( const QString &dir, bool showMessage ) const
    {
        auto fi = QFileInfo( QDir( dir ).absoluteFilePath( ".git" ) );
        if ( fi.exists() && fi.isDir() )
            return true;
        if ( showMessage )
        {
            QMessageBox::warning( const_cast< CSelectLocalDir * >( this ), tr( "Not a GIT directory" ), tr( "'%1' is not a GIT directory" ).arg( dir ) );
        }
        return false;
    }

    bool CSelectLocalDir::isComplete() const
    {
        return isGitDir( field( REPO_DIR_FIELD ).toString(), false );
    }

    int CSelectLocalDir::nextId() const
    {
        auto nextPage = EPageID::eSummary;
        if ( field( PUSH_GOAL_FIELD ).toBool() )
        {
            nextPage = EPageID::eEnterComment;
        }

        return toInt( nextPage );
    }
}

 