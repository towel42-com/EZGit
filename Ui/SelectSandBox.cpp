//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectSandBox.h"

#include "ui_SelectSandBox.h"

#include "UiFwd.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace NUi
{
    CSelectSandBox::CSelectSandBox( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectSandBox )
    {
        fImpl->setupUi( this );
        registerField( QString( SANDBOX_FIELD ), fImpl->sandBox );
        registerField( QString( REPO_DIR_FIELD ) + "*", fImpl->repoDir );

        connect(
            fImpl->selectSandBox, &QToolButton::clicked,   //
            [ = ]()
            {
                auto sandBox = QFileDialog::getExistingDirectory( this, tr( "Select Sandbox:" ), fImpl->sandBox->text() );
                if ( sandBox.isEmpty() )
                    return;

                fImpl->sandBox->setText( sandBox );
            } );
        connect( fImpl->sandBox, &QLineEdit::textChanged, this, &CSelectSandBox::slotUpdateRepoDir );
    }

    CSelectSandBox::~CSelectSandBox()
    {
    }

    void CSelectSandBox::slotUpdateRepoDir()
    {
        fImpl->repoDir->clear();
        if ( fImpl->sandBox->text().isEmpty() )
            return;
        auto dir = QDir( fImpl->sandBox->text() );

        auto url = QUrl::fromUserInput( field( REPO_URL_FIELD ).toString() );
        auto sub = url.path();
        auto pos = sub.lastIndexOf( '/' );
        if ( pos != -1 )
            sub = sub.mid( pos + 1 );
        auto branch = field( BRANCH_FIELD ).toString();

        auto path = dir.absoluteFilePath( sub + "." + branch );
        fImpl->repoDir->setText( path );
    }

    void CSelectSandBox::initializePage()
    {
        slotUpdateRepoDir();
    }

    int CSelectSandBox::nextId() const
    {
        return static_cast< int >( EPageID::eSummary );
    }
}
