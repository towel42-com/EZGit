//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectRepo.h"
#include "UrlValidator.h"

#include "ui_SelectRepo.h"
#include "UiFwd.h"

namespace NUi
{
    CSelectRepo::CSelectRepo( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectRepo )
    {
        fImpl->setupUi( this );
        fImpl->repoURL->setValidator( new CUrlValidator );

        registerField( QString( REPO_URL_FIELD ) + "*", fImpl->repoURL );
    }

    CSelectRepo::~CSelectRepo()
    {
    }

}
