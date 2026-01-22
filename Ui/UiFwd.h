#ifndef __UIFWD_H
#define __UIFWD_H

#include <type_traits>
#include <QString>
namespace NUi
{
    enum class EPageID
    {
        eSelectGit,
        eSelectRemote,
        eSetCredentials,
        eSelectGoal,
        eSelectBranch,
        eSelectCloneDir,
        eSelectLocalDir,
        eEnterComment,
        eSummary,
        eRunGit
    };
    template< typename E = EPageID >
    constexpr int toInt( E e ) noexcept
    {
        return static_cast< std::underlying_type_t< E > >( e );
    }

    extern QString SHOW_INIT_PAGES_FIELD;
    extern QString SHOW_INIT_PAGES_FIELD2;
    extern QString SHOW_INIT_PAGES_SETTING;

    extern QString GIT_EXEC_FIELD;
    extern QString GIT_EXEC_SETTING;

    extern QString REMOTE_URL_FIELD;
    extern QString REMOTE_URL_SETTING;

    extern QString EMAIL_FIELD;
    extern QString USERNAME_FIELD;
    extern QString CREDENTIALS_CHANGED_FIELD;
    extern QString LOGGED_IN_USER_FIELD;
    
    extern QString CLONE_GOAL_FIELD;
    extern QString PULL_GOAL_FIELD;
    extern QString PUSH_GOAL_FIELD;

    extern QString BRANCH_OR_TAG_FIELD;
    extern QString ISBRANCH_FIELD;

    extern QString SANDBOX_FIELD;
    extern QString SANDBOX_SETTING;

   
    extern QString REPO_DIR_FIELD;
    extern QString SANDBOX_REPO_DIR_FIELD;

    extern QString REPO_DIR_SETTING;

    extern QString COMMENT_FIELD;
}

#endif
