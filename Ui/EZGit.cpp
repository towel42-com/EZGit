#include "EZGit.h"
#include "SelectGit.h"
#include "SelectRepo.h"
#include "SelectGoal.h"
#include "SelectBranch.h"
#include "SelectSandBox.h"
#include "Summary.h"

#include "UiFwd.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QSettings>
#include <QProcess>
#include <QApplication>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>

namespace NUi
{
    CEZGit::CEZGit( QWidget *parent ) :
        QWizard( parent )
    {
        setDefaultProperty( QTreeWidget::staticMetaObject.className(), "currentItem", SIGNAL( currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ) );
        setOptions( options() & ~QWizard::WizardOption::IndependentPages );
        addPage( new CSelectGit );
        addPage( new CSelectRepo );
        addPage( new CSelectGoal );
        addPage( new CSelectBranch );
        addPage( new CSelectSandBox );
        addPage( new CSummary );

        loadFields();

        if ( !field( SHOW_SELECT_GIT_FIELD ).toBool() )
        {
            setStartId( static_cast< int >( EPageID::eSelectRepo ) );
        }
    }

    std::pair< QString, bool > CEZGit::runGit( const QStringList &args )
    {
        auto gitExec = field( GIT_EXEC_FIELD ).toString();
        Q_ASSERT( !gitExec.isEmpty() );
        if ( gitExec.isEmpty() )
            return { tr( "Git Executable not set" ), false };

        auto fi = QFileInfo( gitExec );
        Q_ASSERT( fi.exists() && fi.isFile() && fi.isExecutable() );

        QProcess process;
        process.start( fi.absoluteFilePath(), args );
        QApplication::setOverrideCursor( Qt::WaitCursor );
        if ( !process.waitForFinished( -1 ) || process.error() == QProcess::FailedToStart )
        {
            QApplication::restoreOverrideCursor();
            return { tr( "ERROR: %1" ).arg( process.errorString() ), false };
        }
        QApplication::restoreOverrideCursor();

        auto retVal = process.readAll();

        return { retVal, true };
    }

    void CEZGit::clone()
    {
        auto repoUrl = field( REPO_URL_FIELD ).toString();
        auto branch = field( BRANCH_FIELD ).toString();
        auto repoDir = field( REPO_DIR_FIELD ).toString();

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
        auto results = runGit( { "clone", "--branch", branch, "--recurse-submodules", repoUrl, repoDir } );
        if ( !results.second )
        {
            QMessageBox::critical( this, tr( "Error cloning" ), results.first );
            return;
        }
    }

    void CEZGit::accept()
    {
        saveFields();
        if ( field( CLONE_GOAL_FIELD ).toBool() )
        {
            clone();
        }

        QWizard::accept();
    }

    void CEZGit::loadFields()
    {
        QSettings settings;

        if ( settings.contains( GIT_EXEC_SETTING ) )
            setField( GIT_EXEC_FIELD, settings.value( GIT_EXEC_SETTING ) );
        else
        {
            auto gitExec = QStandardPaths::findExecutable( "git.exe" );
            if ( QFileInfo( gitExec ).exists() )
                setField( GIT_EXEC_FIELD, gitExec );
            else
            {
                auto programDirs = QStandardPaths::standardLocations( QStandardPaths::StandardLocation::ApplicationsLocation );
                if ( !programDirs.isEmpty() )
                    setField( GIT_EXEC_FIELD, settings.value( GIT_EXEC_SETTING, QDir( programDirs.front() ).absoluteFilePath( "git.exe" ) ) );
            }
        }

        setField( SHOW_SELECT_GIT_FIELD, settings.value( SHOW_SELECT_GIT_SETTING, true ) );
        setField( REPO_URL_FIELD, settings.value( REPO_URL_SETTING, "https://github.com/pacificvolt/Firmware" ) );
        setField( SANDBOX_FIELD, settings.value( SANDBOX_SETTING ) );
    }

    void CEZGit::saveFields()
    {
        QSettings settings;
        settings.setValue( GIT_EXEC_SETTING, field( GIT_EXEC_FIELD ) );
        settings.setValue( SHOW_SELECT_GIT_SETTING, field( SHOW_SELECT_GIT_FIELD ) );
        settings.setValue( REPO_URL_SETTING, field( REPO_URL_FIELD ) );
        settings.setValue( SANDBOX_SETTING, field( SANDBOX_FIELD ) );
    }

}