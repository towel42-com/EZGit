//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectRepoDir.h"

#include "ui_SelectRepoDir.h"

#include "UiFwd.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace NUi
{
    CSelectRepoDir::CSelectRepoDir( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectRepoDir )
    {
        fImpl->setupUi( this );
        registerField( QString( REPO_DIR_FIELD ) + "*", fImpl->repoDir );

        connect( fImpl->repoDir, &QLineEdit::textChanged, this, &CSelectRepoDir::completeChanged );

        connect(
            fImpl->selectRepository, &QToolButton::clicked,   //
            [ = ]()
            {
                auto curr = fImpl->repoDir->text();
                if ( curr.isEmpty() )
                    curr = field( SANDBOX_FIELD ).toString();

                auto repo = QFileDialog::getExistingDirectory( this, tr( "Select Sandbox:" ), curr );
                if ( repo.isEmpty() )
                    return;

                if ( isGitDir( repo, true ) )
                    fImpl->repoDir->setText( repo );
            } );
    }

    CSelectRepoDir::~CSelectRepoDir()
    {
    }

    bool CSelectRepoDir::isGitDir( const QString &dir, bool showMessage ) const
    {
        auto fi = QFileInfo( QDir( dir ).absoluteFilePath( ".git" ) );
        if ( fi.exists() && fi.isDir() )
            return true;
        if ( showMessage )
        {
            QMessageBox::warning( const_cast< CSelectRepoDir * >( this ), tr( "Not a GIT directory" ), tr( "'%1' is not a GIT directory" ).arg( dir ) );
        }
        return false;
    }

    bool CSelectRepoDir::isComplete() const
    {
        return isGitDir( field( REPO_DIR_FIELD ).toString(), false );
    }

    int CSelectRepoDir::nextId() const
    {
        auto nextPage = EPageID::eSummary;
        if ( field( PUSH_GOAL_FIELD ).toBool() )
        {
            nextPage = EPageID::eEnterComment;
        }

        return static_cast< int >( nextPage );
    }
}

 