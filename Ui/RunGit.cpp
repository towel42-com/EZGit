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

namespace NUi
{
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
                QApplication::restoreOverrideCursor();
                QTimer::singleShot( 0, this, &CRunGit::slotRunNextCmd );
            } );
    }

    CRunGit::~CRunGit()
    {
    }

    std::pair< QString, bool > CRunGit::runGit( const QString &gitExec, const QStringList &args, const QString &pwd )
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

        auto retVal = runGit( &process, gitExec, args, pwd, true );
        if ( !retVal )
        {
            outputText += "\nERROR: " + process.errorString();
        }
        return std::make_pair( outputText, retVal );
    }

    bool CRunGit::runGit( QProcess *process, const QString &gitExec, const QStringList &args, const QString &pwd, bool waitForFinished )
    {
        auto fi = QFileInfo( gitExec );
        Q_ASSERT( fi.exists() && fi.isFile() && fi.isExecutable() );

        QString outputText;
        process->setWorkingDirectory( pwd );
        process->setProcessChannelMode( QProcess::MergedChannels );

        process->start( fi.absoluteFilePath(), args );
        QApplication::setOverrideCursor( Qt::WaitCursor );
        if ( waitForFinished )
        {
            if ( !process->waitForFinished( -1 ) || ( process->error() == QProcess::FailedToStart ) )
            {
                QApplication::restoreOverrideCursor();
                return false;
            }
            QApplication::restoreOverrideCursor();
        }

        return true;
    }

    void CRunGit::runGit( const QStringList &args, const QString &pwd, bool clearFirst )
    {
        auto gitExec = field( GIT_EXEC_FIELD ).toString();

        if ( clearFirst )
            fImpl->output->clear();

        QString cmd;
        if ( clearFirst )
            cmd = "======================================================\n";
        cmd += "PWD: " + pwd + "\n";
        cmd += createCmdLine( gitExec, args ) + "\n";
        cmd += "======================================================\n";

        fImpl->output->appendPlainText( cmd );

        if ( !runGit( fProcess, gitExec, args, pwd, false ) )
        {
        }
    }

    void CRunGit::slotRunNextCmd()
    {
        if ( fGitCmds.empty() )
            return;
        auto currCmd = fGitCmds.front();
        fGitCmds.pop_front();
        runGit( std::get< 0 >( currCmd ), std::get< 1 >( currCmd ), std::get< 2 >( currCmd ) );
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
        auto branch = field( BRANCH_FIELD ).toString();
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

        addGitCmd( { "clone", "--branch", branch, "--recurse-submodules", repoUrl, repoDir }, parentDirPath, clearFirst );
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

    void CRunGit::addGitCmd( const QStringList &args, const QString &pwd, bool clearFirst )
    {
        fGitCmds.emplace_back( args, pwd, clearFirst );
        if ( fGitCmds.size() == 1 )
            QTimer::singleShot( 0, this, &CRunGit::slotRunNextCmd );
    }
}