//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "BasePage.h"
#include "EZGit.h"

namespace NUi
{
    CBasePage::CBasePage( QWidget *parent ) :
        QWizardPage( parent )
    {
    }

    CBasePage::~CBasePage()
    {
    }


    CEZGit * CBasePage::ezGit() const
    {
        auto wizard = dynamic_cast< CEZGit * >( this->wizard() );
        Q_ASSERT( wizard );
        return wizard;
    }
}
