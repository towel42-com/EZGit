//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "EnterComment.h"

#include "ui_EnterComment.h"
#include "UiFwd.h"

namespace NUi
{
    CEnterComment::CEnterComment( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CEnterComment )
    {
        fImpl->setupUi( this );

        registerField( QString( COMMENT_FIELD ) + "*", fImpl->comment );
    }

    CEnterComment::~CEnterComment()
    {
    }

    int CEnterComment::nextId() const
    {
        return toInt( EPageID::eSummary );
    }

}
