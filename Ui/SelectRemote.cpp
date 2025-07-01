//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectRemote.h"
#include "UrlValidator.h"
#include "EZGit.h"

#include "ui_SelectRemote.h"
#include "UiFwd.h"

namespace NUi
{
    CSelectRemote::CSelectRemote( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectRemote )
    {
        fImpl->setupUi( this );
        fImpl->repoURL->setValidator( new CUrlValidator );

        registerField( QString( REMOTE_URL_FIELD ) + "*", fImpl->repoURL );
    }

    CSelectRemote::~CSelectRemote()
    {
    }

    int CSelectRemote::nextId() const
    {
        if ( ezGit()->showSetCredentials() )
            return toInt( EPageID::eSetCredentials );
        return toInt( EPageID::eSelectGoal );
    }

}
