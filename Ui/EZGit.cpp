#include "EZGit.h"
#include "RunGit.h"
#include "SelectGit.h"
#include "SelectRepo.h"
#include "SelectGoal.h"
#include "SelectBranch.h"
#include "SelectCloneDir.h"
#include "SelectRepoDir.h"
#include "Summary.h"
#include "RunGit.h"
#include "EnterComment.h"

#include "UiFwd.h"

#include <QDir>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QCloseEvent>

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
        addPage( new CSelectRepo );
        addPage( new CSelectGoal );

        addPage( new CSelectBranch );
        addPage( new CSelectCloneDir );

        addPage( new CSelectRepoDir );
        addPage( new CEnterComment );
        addPage( new CSummary );
        addPage( new CRunGit );

        loadFields();

        auto firstPage = EPageID::eSelectGit;
        if ( !field( SHOW_SELECT_GIT_FIELD ).toBool() )
        {
            if ( field( SHOW_SELECT_REPO_FIELD ).toBool() )
                firstPage = EPageID::eSelectRepo;
            else
                firstPage = EPageID::eSelectGoal;
        }

        setStartId( static_cast< int >( firstPage ) );
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
        setField( SHOW_SELECT_REPO_FIELD, settings.value( SHOW_SELECT_REPO_SETTING, true ) );
        setField( REPO_URL_FIELD, settings.value( REPO_URL_SETTING, "https://github.com/pacificvolt/Firmware" ) );
        setField( SANDBOX_FIELD, settings.value( SANDBOX_SETTING ) );
        setField( REPO_DIR_FIELD, settings.value( REPO_DIR_SETTING ) );
    }

    void CEZGit::saveFields()
    {
        QSettings settings;
        settings.setValue( GIT_EXEC_SETTING, field( GIT_EXEC_FIELD ) );
        settings.setValue( SHOW_SELECT_GIT_SETTING, field( SHOW_SELECT_GIT_FIELD ) );
        settings.setValue( SHOW_SELECT_REPO_SETTING, field( SHOW_SELECT_REPO_FIELD ) );
        settings.setValue( REPO_URL_SETTING, field( REPO_URL_FIELD ) );
        settings.setValue( SANDBOX_SETTING, field( SANDBOX_FIELD ) );
        settings.setValue( REPO_DIR_SETTING, field( REPO_DIR_FIELD ) );
    }
}