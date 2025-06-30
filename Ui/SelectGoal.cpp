//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectGoal.h"
#include "UrlValidator.h"

#include "ui_SelectGoal.h"
#include "UiFwd.h"

namespace NUi
{
    CSelectGoal::CSelectGoal( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectGoal )
    {
        fImpl->setupUi( this );
        registerField( QString( CLONE_GOAL_FIELD ) + "*", fImpl->clone );
        registerField( QString( PULL_GOAL_FIELD ) + "*", fImpl->pull );
        registerField( QString( PUSH_GOAL_FIELD ) + "*", fImpl->push );

        connect( fImpl->clone, &QRadioButton::clicked, this, &CSelectGoal::completeChanged );
        connect( fImpl->push, &QRadioButton::clicked, this, &CSelectGoal::completeChanged );
        connect( fImpl->pull, &QRadioButton::clicked, this, &CSelectGoal::completeChanged );
    }

    CSelectGoal::~CSelectGoal()
    {
    }

    bool CSelectGoal::isComplete() const
    {
        return ( fImpl->clone->isChecked() || fImpl->pull->isChecked() || fImpl->push->isChecked() );
    }

    int CSelectGoal::nextId() const
    {
        EPageID nextPage = EPageID::eSelectGoal;
        if ( field( CLONE_GOAL_FIELD ).toBool() )
        {
            nextPage = EPageID::eSelectBranch;
        }
        else // if ( field( PULL_GOAL_FIELD ).toBool() || field( PUSH_GOAL_FIELD ).toBool() )
        {
            nextPage = EPageID::eSelectSandBox;
        }


        return static_cast< int >( nextPage );
    }
}
