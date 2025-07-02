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
        registerField( QString( CREDENTIALS_CHANGED_FIELD ) + "*", fImpl->credentialsChanged );
        fImpl->credentialsChanged->setVisible( false );

        connect( fImpl->email, &QLineEdit::textChanged, [ this ]() { fTextChanged = true; } );
        connect( fImpl->userName, &QLineEdit::textChanged, [ this ]() { fTextChanged = true; } );

        connect(
            fImpl->logIntoGithub, &QPushButton::clicked,
            [ this ]()   //
            {   //
                ezGit()->runGit( { "credential-manager", "github", "login" } );
                emit completeChanged();
            } );
    }

    CSetCredentials::~CSetCredentials()
    {
    }

    bool CSetCredentials::credentialsOK() const
    {
        auto users = ezGit()->runGit( { "credential-manager", "github", "list" } );
        return users.second && !users.first.isEmpty();
    }

    int CSetCredentials::nextId() const
    {
        return toInt( EPageID::eSelectGoal );
    }

    bool CSetCredentials::isComplete() const
    {
        bool complete = !field( USERNAME_FIELD ).toString().isEmpty();
        complete = complete && !field( EMAIL_FIELD ).toString().isEmpty();
        complete = complete && credentialsOK();
        return complete;
    }
}
