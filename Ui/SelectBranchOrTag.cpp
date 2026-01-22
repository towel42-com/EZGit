//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "SelectBranchOrTag.h"
#include "EZGit.h"

#include "ui_SelectBranchOrTag.h"

#include <QMessageBox>

#include "UiFwd.h"

namespace NUi
{
    CSelectBranchOrTag::CSelectBranchOrTag( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CSelectBranchOrTag )
    {
        fImpl->setupUi( this );

        registerField( QString( BRANCH_OR_TAG_FIELD ) + "*", fImpl->selectedBranchOrTag );
        registerField( QString( ISBRANCH_FIELD ) + "*", fImpl->isBranch );
        connect(
            fImpl->branchOrTags, &QTreeWidget::currentItemChanged,
            [ = ]()
            {
                fImpl->selectedBranchOrTag->setText( {} );
                auto item = fImpl->branchOrTags->currentItem();
                if ( !item )
                {
                    fImpl->isBranch->setCheckState( Qt::CheckState::PartiallyChecked );
                    emit completeChanged();
                    return;
                }
                auto name = item->text( 0 );
                auto checkState = ( item->text( 2 ) == "Branch" ) ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;

                fImpl->selectedBranchOrTag->setText( name );
                fImpl->isBranch->setCheckState( checkState );
                emit completeChanged();
            } );

        fImpl->selectedBranchOrTag->setVisible( false );
        fImpl->isBranch->setVisible( false );
        fImpl->isBranch->setCheckState( Qt::CheckState::PartiallyChecked );
    }

    CSelectBranchOrTag::~CSelectBranchOrTag()
    {
    }

    bool CSelectBranchOrTag::isComplete() const
    {
        bool complete = !field( BRANCH_OR_TAG_FIELD ).toString().isEmpty();
        complete = complete && ( fImpl->isBranch->checkState() != Qt::CheckState::PartiallyChecked );

        return complete;
    }

    void CSelectBranchOrTag::initializePage()
    {
        fImpl->branchOrTags->clear();
        auto branches = loadBranchesAndTags();

        for ( auto &&branch : branches )
        {
            branch.createItem( fImpl->branchOrTags );
        }
    }

    std::list< CSelectBranchOrTag::SBranchOrTag > CSelectBranchOrTag::loadBranchesAndTags()
    {
        auto defaultOID = getDefaultOID();
        auto oidToNameMap = getAllBranchesAndTags();

        std::list< SBranchOrTag > retVal;
        SBranchOrTag defaultBranch;
        for ( auto &&ii : oidToNameMap )
        {
            bool isBranch = ii.second.startsWith( "refs/heads/" );
            bool isDefaultBranch = false;
            if ( isBranch )
            {
                isDefaultBranch = ( ii.first == defaultOID );
                ii.second = ii.second.mid( 11 ).trimmed();
            }
            else if ( ii.second.startsWith( "refs/tags/" ) )
                ii.second = ii.second.mid( 10 ).trimmed();

            retVal.emplace_back( ii.first, ii.second, isBranch, isDefaultBranch );
        }

        retVal.sort(   //
            []( const SBranchOrTag &lhs, const SBranchOrTag &rhs )
            {
                if ( lhs.fIsBranch != rhs.fIsBranch )
                {
                    return lhs.fIsBranch;   // branches show first;
                }

                if ( lhs.fIsDefaultBranch != rhs.fIsDefaultBranch )
                {
                    return lhs.fIsDefaultBranch;
                }

                return lhs.fName.compare( rhs.fName, Qt::CaseInsensitive ) < 0;
            } );

        return retVal;
    }

    int CSelectBranchOrTag::nextId() const
    {
        return toInt( EPageID::eSelectCloneDir );
    }

    QString CSelectBranchOrTag::getDefaultOID()
    {
        auto results = ezGit()->runGit( { "ls-remote", "--symref", field( REMOTE_URL_FIELD ).toString(), "HEAD" } );
        if ( !results.second )
        {
            QMessageBox::critical( this, tr( "Error calling git" ), results.first );
            return {};
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
        return defaultOID;
    }

    std::unordered_map< QString, QString > CSelectBranchOrTag::getAllBranchesAndTags()
    {
        auto results = ezGit()->runGit( { "ls-remote", "--tags", "--heads", "--refs", field( REMOTE_URL_FIELD ).toString() } );
        if ( !results.second )
        {
            QMessageBox::critical( this, tr( "Error calling git" ), results.first );
            return {};
        }

        std::unordered_map< QString, QString > oidToNameMap;
        auto lines = results.first.split( '\n' );
        for ( auto &&line : lines )
        {
            auto pos = line.indexOf( '\t' );
            if ( pos == -1 )
                continue;

            auto oid = line.mid( 0, pos ).trimmed();
            auto branch = line.mid( pos ).trimmed();

            if ( branch.startsWith( "refs/original" ) || branch.startsWith( "refs/remotes" ) )
                continue;

            auto &&jj = oidToNameMap.find( oid );
            if ( jj != oidToNameMap.end() )
            {
                if ( ( *jj ).second.startsWith( "refs" ) )
                    continue;
                ( *jj ).second = branch;
            }
            else
                oidToNameMap[ oid ] = branch;
        }
        return oidToNameMap;
    }

    QTreeWidgetItem *CSelectBranchOrTag::SBranchOrTag::createItem( QTreeWidget *parent )
    {
        auto item = new QTreeWidgetItem( parent );
        item->setText( 0, fName );
        if ( fIsBranch && fIsDefaultBranch )
            item->setText( 1, "Yes" );
        item->setText( 2, fIsBranch ? QStringLiteral( "Branch" ) : QStringLiteral( "Tag" ) );
        item->setSelected( true );
        return item;
    }

}
