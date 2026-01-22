#include "EZGit.h"
#include "RunGit.h"
#include "SelectGit.h"
#include "SelectRemote.h"
#include "SelectGoal.h"
#include "SelectBranchOrTag.h"
#include "SelectCloneDir.h"
#include "SelectLocalDir.h"
#include "SetCredentials.h"
#include "Summary.h"
#include "EnterComment.h"
#include "Version.h"

#include "UiFwd.h"

#include <QDir>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QCloseEvent>
#include <QTimer>
#include <QAbstractButton>
#include <ShlObj_core.h>
#include <QApplication>
#include <QFontMetrics>

namespace NUi
{
    CEZGit::CEZGit( QWidget *parent ) :
        QWizard( parent )
    {
        auto title = NVersion::getWindowTitle( true, false );
        setWindowTitle( title );

        setDefaultProperty( QTreeWidget::staticMetaObject.className(), "currentItem", SIGNAL( currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ) );
        setDefaultProperty( QPlainTextEdit::staticMetaObject.className(), "plainText", SIGNAL( textChanged() ) );
        setOptions( options() & ~QWizard::WizardOption::IndependentPages );
        setOptions( options() | QWizard::NoCancelButton );

        setPage( toInt( EPageID::eSelectGit ), new CSelectGit );
        setPage( toInt( EPageID::eSelectRemote ), new CSelectRemote );
        setPage( toInt( EPageID::eSetCredentials ), new CSetCredentials );

        setPage( toInt( EPageID::eSelectGoal ), new CSelectGoal );
        setPage( toInt( EPageID::eSelectBranch ), new CSelectBranchOrTag );
        setPage( toInt( EPageID::eSelectCloneDir ), new CSelectCloneDir );

        setPage( toInt( EPageID::eSelectLocalDir ), new CSelectLocalDir );
        setPage( toInt( EPageID::eEnterComment ), new CEnterComment );
        setPage( toInt( EPageID::eSummary ), new CSummary );
        setPage( toInt( EPageID::eRunGit ), new CRunGit );

        loadFields();

        auto firstPage = getFirstPage();
        if ( firstPage != EPageID::eSelectGit )
        {
            confirmCredentialManager();
        }
        setStartId( toInt( firstPage ) );
        setButtonText( QWizard::CommitButton, tr( "Execute" ) );
    }

    EPageID CEZGit::getFirstPage() const
    {
        auto firstPage = EPageID::eSelectGoal;
        if ( showSelectGit() )
            firstPage = EPageID::eSelectGit;
        else if ( showSelectRemote() )
            firstPage = EPageID::eSelectRemote;
        else if ( showSetCredentials() )
            firstPage = EPageID::eSetCredentials;

        return firstPage;
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
        setField( EMAIL_FIELD, getGitEmail() );
        setField( USERNAME_FIELD, getGitUserName() );
        setField( SANDBOX_FIELD, settings.value( SANDBOX_SETTING ) );
        setField( REPO_DIR_FIELD, settings.value( REPO_DIR_SETTING ) );
    }

    void CEZGit::saveFields()
    {
        QSettings settings;
        settings.setValue( SHOW_INIT_PAGES_SETTING, field( SHOW_INIT_PAGES_FIELD ).toBool() || field( SHOW_INIT_PAGES_FIELD2 ).toBool() );

        settings.setValue( GIT_EXEC_SETTING, field( GIT_EXEC_FIELD ) );
        settings.setValue( REMOTE_URL_SETTING, field( REMOTE_URL_FIELD ) );
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
        auto page = this->page( toInt( pageID ) );
        Q_ASSERT( page );
        if ( !page )
            return true;
        page->initializePage();
        return !page->isComplete();
    }

    QString CEZGit::getGitEmail()
    {
        return getConfigValue( "user.email" );
    }

    QString CEZGit::getGitUserName()
    {
        return getConfigValue( "user.name" );
    }

    QString CEZGit::getGitLoggedInName()
    {
        auto users = runGit( { "credential-manager", "github", "list" } );
        if ( !users.second )
        {
            users.first.clear();
        }
        return users.first;
    }

    QString CEZGit::getConfigValue( const QString &key )
    {
        auto retVal = runGit( { "config", key } );
        if ( !retVal.second )
            return {};

        return retVal.first.trimmed();
    }

    std::pair< QString, bool > CEZGit::runGit( const QStringList &args )
    {
        auto gitExec = getGitExec();
        if ( gitExec.isEmpty() )
            return {};

        return CRunGit::runGit( this, gitExec, args, {} );
    }

    bool CEZGit::confirmCredentialManager()
    {
        auto credHelper = getConfigValue( "credential.helper" );
        if ( credHelper == "manager" )
            return true;

        auto status = runGit( { "config", "--global", "credential.helper", "manager" } );
        if ( !status.second )
        {
            QMessageBox::critical( this, tr( "Could not properly set credentials" ), tr( "You must use the windows credential manager, which could not be set.\nERROR: %1" ).arg( status.first ) );
            QTimer::singleShot( 0, this, &CEZGit::reject );
            return false;
        }

        credHelper = getConfigValue( "credential.helper" );
        if ( credHelper != "manager" )
        {
            QMessageBox::critical( this, tr( "Could not properly set credentials" ), tr( "You must use the windows credential manager, which could not be set." ) );
            QTimer::singleShot( 0, this, &CEZGit::reject );
            return false;
        }

        return true;
    }

    void CEZGit::setRunningCmd( bool running )
    {
        if ( running )
        {
            bool first = fDisableCnt == 0;
            fDisableCnt++;
            if ( !first )
                return;
            QApplication::setOverrideCursor( Qt::WaitCursor );
        }
        else
        {
            if ( fDisableCnt == 0 )
                return;
            fDisableCnt--;
            if ( fDisableCnt != 0 )
                return;
            QApplication::restoreOverrideCursor();
        }
        auto currentPage = this->currentPage();
        setButtonEnabled( QWizard::WizardButton::FinishButton, !running );
        setButtonEnabled( QWizard::WizardButton::CommitButton, !running );
        setButtonEnabled( QWizard::WizardButton::NextButton, !running );
    }

    void CEZGit::setButtonEnabled( QWizard::WizardButton which, bool enabled )
    {
        if ( button( which ) )
            button( which )->setEnabled( enabled );
    }

}