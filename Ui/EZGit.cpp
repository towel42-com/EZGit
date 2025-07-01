#include "EZGit.h"
#include "RunGit.h"
#include "SelectGit.h"
#include "SelectRemote.h"
#include "SelectGoal.h"
#include "SelectBranch.h"
#include "SelectCloneDir.h"
#include "SelectLocalDir.h"
#include "SetCredentials.h"
#include "Summary.h"
#include "EnterComment.h"

#include "UiFwd.h"

#include <QDir>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QCloseEvent>
#include <ShlObj_core.h>

namespace NUi
{
    CEZGit::CEZGit( QWidget *parent ) :
        QWizard( parent )
    {
        setDefaultProperty( QTreeWidget::staticMetaObject.className(), "currentItem", SIGNAL( currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ) );
        setDefaultProperty( QPlainTextEdit::staticMetaObject.className(), "plainText", SIGNAL( textChanged() ) );
        setOptions( options() & ~QWizard::WizardOption::IndependentPages );
        setOptions( options() | QWizard::NoCancelButton );

        addPage( new CSelectGit );
        addPage( new CSelectRemote );
        addPage( new CSetCredentials );

        addPage( new CSelectGoal );
        addPage( new CSelectBranch );
        addPage( new CSelectCloneDir );

        addPage( new CSelectLocalDir );
        addPage( new CEnterComment );
        addPage( new CSummary );
        addPage( new CRunGit );

        loadFields();

        setStartId( getFirstPage() );
    }

    int CEZGit::getFirstPage() const
    {
        auto firstPage = EPageID::eSelectGit;
        if ( showSelectGit() )
            firstPage = EPageID::eSelectGit;
        else if ( showSelectRemote() )
            firstPage = EPageID::eSelectRemote;
        else if ( showSetCredentials() )
            firstPage = EPageID::eSetCredentials;

        return toInt( firstPage );
    }

    bool CEZGit::showSelectGit() const
    {
        return showInitPage( EPageID::eSelectGit );
    }

    bool CEZGit::showSelectRemote() const
    {
        return showInitPage( EPageID::eSelectRemote );
    }

    bool CEZGit::showSetCredentials() const
    {
        return showInitPage( EPageID::eSetCredentials );
    }

    QString CEZGit::getProgramFiles( bool sixtyFourBit )
    {
        QString result;
        LPWSTR path;
        auto clsid = sixtyFourBit ? FOLDERID_ProgramFilesX64 : FOLDERID_ProgramFilesX86;
        if ( Q_LIKELY( SUCCEEDED( SHGetKnownFolderPath( clsid, KF_FLAG_DONT_VERIFY, 0, &path ) ) ) )
        {
            result = QString::fromWCharArray( path );
            CoTaskMemFree( path );
        }
        return result;
    }

    QString CEZGit::getGitExec() const
    {
        QStringList execs;

        QSettings settings;
        if ( settings.contains( GIT_EXEC_SETTING ) )
            execs << settings.value( GIT_EXEC_SETTING ).toString();

        execs << QStandardPaths::findExecutable( "git.exe" )   //
              << QDir( getProgramFiles( true ) ).absoluteFilePath( "Git/bin/git.exe" )   //
              << QDir( getProgramFiles( false ) ).absoluteFilePath( "Git/bin/git.exe" )   //
            ;

        for ( auto &&ii : execs )
        {
            auto fi = QFileInfo( ii );
            if ( fi.exists() && fi.isExecutable() && fi.isFile() )
                return fi.absoluteFilePath();
        }
        return {};
    }

    void CEZGit::loadFields()
    {
        QSettings settings;

        setField( SHOW_INIT_PAGES_FIELD, settings.value( SHOW_INIT_PAGES_SETTING, true ) );

        setField( GIT_EXEC_FIELD, getGitExec() );

        setField( REMOTE_URL_FIELD, settings.value( REMOTE_URL_SETTING, "https://github.com/pacificvolt/Firmware" ) );
        setField( EMAIL_FIELD, settings.value( EMAIL_SETTING, getGitEmail() ) );
        setField( USERNAME_FIELD, settings.value( USERNAME_SETTING, getGitUserName() ) );
        setField( SANDBOX_FIELD, settings.value( SANDBOX_SETTING ) );
        setField( REPO_DIR_FIELD, settings.value( REPO_DIR_SETTING ) );
    }

    void CEZGit::saveFields()
    {
        QSettings settings;
        settings.setValue( SHOW_INIT_PAGES_SETTING, field( SHOW_INIT_PAGES_FIELD ) );

        settings.setValue( GIT_EXEC_SETTING, field( GIT_EXEC_FIELD ) );
        settings.setValue( REMOTE_URL_SETTING, field( REMOTE_URL_FIELD ) );
        settings.setValue( EMAIL_SETTING, field( EMAIL_FIELD ) );
        settings.setValue( USERNAME_SETTING, field( USERNAME_FIELD ) );

        settings.setValue( SANDBOX_SETTING, field( SANDBOX_FIELD ) );
        settings.setValue( REPO_DIR_SETTING, field( REPO_DIR_FIELD ) );
    }

    bool CEZGit::showInitPage( EPageID pageID ) const
    {
        if ( field( SHOW_INIT_PAGES_FIELD ).toBool() )
            return true;

        return isPageComplete( pageID );
    }

    bool CEZGit::isPageComplete( EPageID pageID ) const
    {
        auto page = dynamic_cast< CSelectRemote * >( this->page( toInt( pageID ) ) );
        Q_ASSERT( page );
        if ( !page )
            return true;
        return !page->isComplete();
    }

    QString CEZGit::getGitEmail() const
    {
        return getConfigValue( "user.email" );
    }

    QString CEZGit::getGitUserName() const
    {
        return getConfigValue( "user.name" );
    }

    QString CEZGit::getConfigValue( const QString &key ) const
    {
        auto retVal = runGit( { "config", key }  );
        if ( !retVal.second )
            return {};

        return retVal.first.trimmed();
    }

    std::pair< QString, bool > CEZGit::runGit( const QStringList &args, bool wait ) const
    {
        auto gitExec = getGitExec();
        if ( gitExec.isEmpty() )
            return {};

        return CRunGit::runGit( gitExec, args, wait, {} );
    }

}