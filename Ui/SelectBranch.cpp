//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectBranch.h"
#include "EZGit.h"
#include "RunGit.h"

#include "ui_SelectBranch.h"

#include <QMessageBox>

#include "UiFwd.h"

namespace NUi
{
    CSelectBranch::CSelectBranch( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectBranch )
    {
        fImpl->setupUi( this );

        registerField( QString( BRANCH_FIELD ) + "*", fImpl->selectedBranch );
        connect(
            fImpl->branches, &QTreeWidget::currentItemChanged,
            [ = ]()
            {
                fImpl->selectedBranch->setText( {} );
                auto item = fImpl->branches->currentItem();
                if ( !item )
                    return;
                fImpl->selectedBranch->setText( item->text( 0 ) );
            } );

        fImpl->selectedBranch->setVisible( false );
    }

    CSelectBranch::~CSelectBranch()
    {
    }

    void CSelectBranch::initializePage()
    {
        loadBranches();
    }

    void CSelectBranch::loadBranches()
    {
        auto gitExec = field( GIT_EXEC_FIELD ).toString();
        auto results = CRunGit::runGit( gitExec, { "ls-remote", "--symref", field( REPO_URL_FIELD ).toString(), "HEAD" } );
        if ( !results.second )
        {
            QMessageBox::critical( this, tr( "Error calling git" ), results.first );
            return;
        }
        auto lines = results.first.split( '\n' );
        QString defaultOID;
        for ( auto &&line : lines )
        {
            if ( line.startsWith( "ref:" ) )
                continue;

            auto pos1 = line.indexOf( '\t' );
            if ( pos1 == -1 )
                continue;
            defaultOID = line.mid( 0, pos1 ).trimmed();
        }

        results = CRunGit::runGit( gitExec, { "ls-remote", field( REPO_URL_FIELD ).toString() } );
        if ( !results.second )
        {
            QMessageBox::critical( this, tr( "Error calling git" ), results.first );
            return;
        }

        std::unordered_map< QString, QString > branchMap;
        lines = results.first.split( '\n' );
        for ( auto &&line : lines )
        {
            auto pos = line.indexOf( '\t' );
            if ( pos == -1 )
                continue;

            auto oid = line.mid( 0, pos ).trimmed();
            auto branch = line.mid( pos ).trimmed();

            if ( branch.startsWith( "refs/original" ) || branch.startsWith( "refs/remotes" ) )
                continue;

            auto &&jj = branchMap.find( oid );
            if ( jj != branchMap.end() )
            {
                if ( ( *jj ).second.startsWith( "refs" ) )
                    continue;
                ( *jj ).second = branch;
            }
            else
                branchMap[ oid ] = branch;
        }

        QStringList branches;
        QString defaultBranch;
        for ( auto &&ii : branchMap )
        {
            if ( ii.second.startsWith( "refs/heads/" ) )
                ii.second = ii.second.mid( 11 ).trimmed();

            if ( ii.first == defaultOID )
                defaultBranch = ii.second;
            else
                branches << ii.second;
        }

        if ( !defaultBranch.isEmpty() )
        {
            auto item = new QTreeWidgetItem( fImpl->branches );
            item->setText( 0, defaultBranch );
            item->setText( 1, "Yes" );
            item->setSelected( true );
        }
        branches.insert( 0, defaultBranch );

        branches.sort( Qt::CaseInsensitive );
        for ( auto &&branch : branches )
        {
            auto item = new QTreeWidgetItem( fImpl->branches );
            item->setText( 0, branch );
        }
    }

    int CSelectBranch::nextId() const
    {
        return static_cast< int >( EPageID::eSelectCloneDir );
    }

}
