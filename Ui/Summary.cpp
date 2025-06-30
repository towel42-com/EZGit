//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "Summary.h"
#include "UrlValidator.h"

#include "ui_Summary.h"
#include "UiFwd.h"

namespace NUi
{
    CSummary::CSummary( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSummary )
    {
        fImpl->setupUi( this );
    }

    CSummary::~CSummary()
    {
    }

    void CSummary::initializePage()
    {
        QString summary;
        if ( field( CLONE_GOAL_FIELD ).toBool() )
        {
            auto repoUrl = field( REPO_URL_FIELD ).toString();
            auto branch = field( BRANCH_FIELD ).toString();
            auto repoDir = field( REPO_DIR_FIELD ).toString();
            summary += tr( "Cloning repository '<b>%1</b>'<br><ul><li>Branch: '<b>%2</b>'</li><br>to<br><li> Directory: '<b>%3</b>'</li></ul>" )   //
                           .arg( repoUrl )   //
                           .arg( branch )   //
                           .arg( repoDir );
        }

        fImpl->summary->clear();
        fImpl->summary->appendHtml( summary );
    }
}
