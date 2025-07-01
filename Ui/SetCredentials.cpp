//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SetCredentials.h"
#include "EZGit.h"

#include "ui_SetCredentials.h"
#include "UiFwd.h"

#include <QMessageBox>
#include <QTimer>

namespace NUi
{
    CSetCredentials::CSetCredentials( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSetCredentials )
    {
        fImpl->setupUi( this );

        registerField( QString( EMAIL_FIELD ) + "*", fImpl->email );
        registerField( QString( USERNAME_FIELD ) + "*", fImpl->userName );
        registerField( QString( PASSWORD_FIELD ) + "*", fImpl->password );

        connect( fImpl->email, &QLineEdit::textChanged, [ this ]() { fTextChanged = true; } );
        connect( fImpl->password, &QLineEdit::textChanged, [ this ]() { fTextChanged = true; } );

        connect( fImpl->showPassword, &QToolButton::clicked, [ this ]() { showPassword( fImpl->password, fImpl->showPassword ); } );
        connect(
            fImpl->logIntoGithub, &QPushButton::clicked,
            [ this ]()   //
            {   //
                ezGit()->runGit( { "credential-manager", "github", "login" } );
                updateCredentialsOK();
            } );
    }

    CSetCredentials::~CSetCredentials()
    {
    }

    void CSetCredentials::initializePage()
    {
        auto credHelper = ezGit()->getConfigValue( "credential.helper" );
        fUsesManager = ( credHelper == "manager" );
        fImpl->password->setVisible( !fUsesManager );
        fImpl->showPassword->setVisible( !fUsesManager );
        fImpl->passwordLabel->setVisible( !fUsesManager );
        fImpl->logIntoGithub->setVisible( fUsesManager );
        //git credential-manager github list
        if ( fUsesManager )
        {
            updateCredentialsOK();
        }
    }

    void CSetCredentials::updateCredentialsOK()
    {
        auto users = ezGit()->runGit( { "credential-manager", "github", "list" } );
        fManagerCredOK = users.second && !users.first.isEmpty();
        emit completeChanged();
    }

    int CSetCredentials::nextId() const
    {
        return toInt( EPageID::eSelectGoal );
    }

    bool CSetCredentials::isComplete() const
    {
        bool complete = !field( USERNAME_FIELD ).toString().isEmpty();
        complete = complete && !field( EMAIL_FIELD ).toString().isEmpty();
        complete = complete && ( fUsesManager || !field( PASSWORD_FIELD ).toString().isEmpty() );
        if ( fUsesManager )
            complete = complete && fManagerCredOK;
        return complete;
    }

    void CSetCredentials::showPassword( QLineEdit *password, QToolButton *showButton )
    {
        password->setEchoMode( showButton->isChecked() ? QLineEdit::EchoMode::Normal : QLineEdit::EchoMode::Password );
        if ( showButton->isChecked() )
        {
            QTimer::singleShot( 5000, showButton, &QToolButton::animateClick );
        }
    }

    bool CSetCredentials::validatePage()
    {
        if ( !fTextChanged )
            return true;
        if ( fTextChanged )
        {
            ezGit()->runGit( { "config", "--global", "user.email", fImpl->email->text() } );
            ezGit()->runGit( { "config", "--global", "user.name", fImpl->userName->text() } );
        }
    }

}
