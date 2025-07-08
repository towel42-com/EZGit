//
// (c) Copyright 2025 -      Towel 42 Development, LLC
// All rights reserved.
//

#include "RunGit.h"
#include "EZGit.h"

#include "ui_RunGit.h"
#include "UiFwd.h"

#include <QFileInfo>
#include <QProcess>
#include <QFontDatabase>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QDirIterator>
namespace NUi
{
    void setReadOnly( const QString &path )
    {
        QNtfsPermissionCheckGuard guard;
        QFileInfo fi( path );
        if ( fi.isDir() )
        {
            qDebug() << fi.absoluteFilePath();
            QDirIterator it( path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );
            while ( it.hasNext() )
            {
                setReadOnly( it.next() );
            }
        }
        QFile file( path );
        auto permissions = file.permissions();
        permissions = permissions & ( ~QFileDevice::Permission::WriteGroup );
        permissions = permissions & ( ~QFileDevice::Permission::WriteOwner );
        permissions = permissions & ( ~QFileDevice::Permission::WriteOther );
        permissions = permissions & ( ~QFileDevice::Permission::WriteUser );
        file.setPermissions( permissions );
    }

    CRunGit::CRunGit( QWidget *parent ) :
        CBasePage( parent ),
        fImpl( new Ui::CRunGit ),
        fProcess( new QProcess( this ) )
    {
        fImpl->setupUi( this );

        const auto &&fixedPointFont = QFontDatabase::systemFont( QFontDatabase::FixedFont );
        fImpl->output->setFont( fixedPointFont );

        connect(
            fProcess, &QProcess::readyReadStandardOutput,   //
            [ this ]()
            {
                auto outText = fProcess->readAllStandardOutput();
                fImpl->output->appendPlainText( outText );
            } );

        connect(
            fProcess, &QProcess::finished,   //
            [ this ]()
            {
                if ( fCurrCmd && fCurrCmd->fPostRun )
                {
                    fCurrCmd->fPostRun();
                }
                ezGit()->setRunningCmd( false );
                QTimer::singleShot( 0, this, &CRunGit::slotRunNextCmd );
            } );
    }

    CRunGit::~CRunGit()
    {
    }

    std::pair< QString, bool > CRunGit::runGit( CEZGit *ezGit, const QString &gitExec, const QStringList &args, const QString &pwd )
    {
        auto gitCmd = std::make_shared< SGitCmd >();
        gitCmd->fArgs = args;
        gitCmd->fPWD = pwd;
        gitCmd->fClearFirst = true;
        gitCmd->fWaitForFinished = true;
        return runGit( ezGit, gitExec, gitCmd );
    }

    std::pair< QString, bool > CRunGit::runGit( CEZGit *ezGit, const QString &gitExec, std::shared_ptr< SGitCmd > gitCmd )
    {
        Q_ASSERT( !gitExec.isEmpty() );
        if ( gitExec.isEmpty() )
            return { tr( "ERROR: Git Executable not set" ), false };

        QProcess process;
        QString outputText;
        connect(
            &process, &QProcess::readyReadStandardOutput,   //
            [ &process, &outputText ]()
            {
                auto outText = process.readAllStandardOutput();
                outputText += outText;
            } );

        auto retVal = runGit( ezGit, &process, gitExec, gitCmd );
        if ( !retVal )
        {
            outputText += "\nERROR: " + process.errorString();
        }
        return std::make_pair( outputText, retVal );
    }

    bool CRunGit::runGit( CEZGit *ezGit, QProcess *process, const QString &gitExec, std::shared_ptr< SGitCmd > gitCmd )
    {
        auto fi = QFileInfo( gitExec );
        Q_ASSERT( fi.exists() && fi.isFile() && fi.isExecutable() );

        QString outputText;
        process->setWorkingDirectory( gitCmd->fPWD );
        process->setProcessChannelMode( QProcess::MergedChannels );

        process->start( fi.absoluteFilePath(), gitCmd->fArgs );
        ezGit->setRunningCmd( true );
        if ( gitCmd->fWaitForFinished )
        {
            if ( !process->waitForFinished( -1 ) || ( process->error() == QProcess::FailedToStart ) )
            {
                ezGit->setRunningCmd( false );
                return false;
            }
            ezGit->setRunningCmd( false );
            if ( gitCmd->fPostRun )
                gitCmd->fPostRun();
        }

        return true;
    }

    void CRunGit::runGit( std::shared_ptr< SGitCmd > gitCmd )
    {
        auto gitExec = field( GIT_EXEC_FIELD ).toString();

        if ( gitCmd->fClearFirst )
            fImpl->output->clear();

        QString cmd;
        if ( gitCmd->fClearFirst )
            cmd = "======================================================\n";
        cmd += "PWD: " + gitCmd->fPWD + "\n";
        cmd += createCmdLine( gitExec, gitCmd->fArgs ) + "\n";
        cmd += "======================================================\n";

        fImpl->output->appendPlainText( cmd );

        runGit( ezGit(), fProcess, gitExec, gitCmd );
    }

    void CRunGit::runGit( const QStringList &args, const QString &pwd, bool clearFirst )
    {
        auto gitCmd = std::make_shared< SGitCmd >();
        gitCmd->fArgs = args;
        gitCmd->fPWD = pwd;
        gitCmd->fClearFirst = clearFirst;
        runGit( gitCmd );
    }

    void CRunGit::slotRunNextCmd()
    {
        fCurrCmd.reset();
        if ( fGitCmds.empty() )
        {
            QString action;
            if ( field( CLONE_GOAL_FIELD ).toBool() )
            {
                action = tr( "Finished cloning local repository." );
            }
            else if ( field( PULL_GOAL_FIELD ).toBool() )
            {
                action = tr( "Finished pulling to local repository." );
            }
            else if ( field( PUSH_GOAL_FIELD ).toBool() )
            {
                action = tr( "Finished pushing to remote repository." );
            }
            QMessageBox::information( this, tr( "Finished" ), action );
            return;
        }
        fCurrCmd = fGitCmds.front();
        fGitCmds.pop_front();
        runGit( fCurrCmd );
    }

    QString CRunGit::createCmdLine( const QString &gitExec, const QStringList &args ) const
    {
        auto retVal = QString( R"("%1" )" ).arg( gitExec );
        for ( auto &&ii : args )
        {
            if ( ii.indexOf( ' ' ) != -1 )
                retVal += QString( R"("%1" )" ).arg( ii );
            else
                retVal += ii + " ";
        }
        return retVal;
    }

    void CRunGit::initializePage()
    {
        auto ezGit = this->ezGit();
        if ( !ezGit )
            return;

        ezGit->saveFields();
        bool firstCmd = true;
        if ( field( CREDENTIALS_CHANGED_FIELD ).toBool() )
        {
            firstCmd = false;
            runGit( { "config", "--global", "user.email", field( EMAIL_FIELD ).toString() }, {}, true );
            runGit( { "config", "--global", "user.name", field( USERNAME_FIELD ).toString() }, {}, false );
        }

        if ( field( CLONE_GOAL_FIELD ).toBool() )
        {
            clone( firstCmd );
        }
        else if ( field( PULL_GOAL_FIELD ).toBool() )
        {
            pull( firstCmd );
        }
        else if ( field( PUSH_GOAL_FIELD ).toBool() )
        {
            push( firstCmd );
        }
    }

    void CRunGit::clone( bool clearFirst )
    {
        auto repoUrl = field( REMOTE_URL_FIELD ).toString();
        auto branch = field( BRANCH_OR_TAG_FIELD ).toString();
        auto repoDir = field( SANDBOX_REPO_DIR_FIELD ).toString();

        auto pos = repoDir.lastIndexOf( '/' );
        if ( pos == -1 )
            pos = repoDir.lastIndexOf( '\\' );
        auto parentDirPath = repoDir.mid( 0, pos );
        if ( !parentDirPath.isEmpty() )
        {
            auto parentDir = QDir( parentDirPath );
            if ( !parentDir.exists() )
            {
                if ( !parentDir.mkpath( "." ) )
                {
                    QMessageBox::critical( this, tr( "Could not create directory" ), tr( "Directory: '%1' could not be created" ).arg( parentDir.absolutePath() ) );
                    return;
                }
            }
        }

        auto gitCmd = std::make_shared< SGitCmd >();
        gitCmd->fArgs = { "clone", "--branch", branch, "--recurse-submodules", repoUrl, repoDir };
        gitCmd->fPWD = parentDirPath;
        gitCmd->fClearFirst = clearFirst;

        if ( !field( ISBRANCH_FIELD ).toBool() )
        {
            gitCmd->fPostRun = [ repoDir ]()   //
            {
                setReadOnly( repoDir );
            };
        }
        addGitCmd( gitCmd );
    }

    void CRunGit::pull( bool clearFirst )
    {
        auto repoDir = field( REPO_DIR_FIELD ).toString();

        addGitCmd( { "add", "." }, repoDir, clearFirst );
        addGitCmd( { "stash" }, repoDir, false );
        addGitCmd( { "pull", "--recurse-submodules" }, repoDir, false );
        addGitCmd( { "stash", "pop" }, repoDir, false );
    }

    void CRunGit::push( bool clearFirst )
    {
        auto repoDir = field( REPO_DIR_FIELD ).toString();
        auto comment = field( COMMENT_FIELD ).toString();

        addGitCmd( { "add", "." }, repoDir, clearFirst );
        addGitCmd( { "commit", "-a", "-m", comment }, repoDir, false );
        addGitCmd( { "push" }, repoDir, false );
    }

    void CRunGit::addGitCmd( std::shared_ptr< SGitCmd > gitCmd )
    {
        fGitCmds.emplace_back( gitCmd );
        if ( fGitCmds.size() == 1 )
            QTimer::singleShot( 0, this, &CRunGit::slotRunNextCmd );
    }

    void CRunGit::addGitCmd( const QStringList &args, const QString &pwd, bool clearFirst )
    {
        auto gitCmd = std::make_shared< SGitCmd >( args, pwd, clearFirst );
        return addGitCmd( gitCmd );
    }

    SGitCmd::SGitCmd()
    {
    }

    SGitCmd::SGitCmd( const QStringList &args, const QString &pwd, bool clearFirst ) :
        fArgs( args ),
        fPWD( pwd ),
        fClearFirst( clearFirst )
    {
    }
}