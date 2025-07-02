#ifndef __UIFWD_H
#define __UIFWD_H

#include <type_traits>
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

    extern const char *SHOW_INIT_PAGES_FIELD;
    extern const char *SHOW_INIT_PAGES_SETTING;

    extern const char *GIT_EXEC_FIELD;
    extern const char *GIT_EXEC_SETTING;

    extern const char *REMOTE_URL_FIELD;
    extern const char *REMOTE_URL_SETTING;

    extern const char *EMAIL_FIELD;
    extern const char *USERNAME_FIELD;
    extern const char *CREDENTIALS_CHANGED_FIELD;
    extern const char *LOGGED_IN_USER_FIELD;
    
    extern const char *CLONE_GOAL_FIELD;
    extern const char *PULL_GOAL_FIELD;
    extern const char *PUSH_GOAL_FIELD;

    extern const char *BRANCH_FIELD;

    extern const char *SANDBOX_FIELD;
    extern const char *SANDBOX_SETTING;

    extern const char *REPO_DIR_FIELD;
    extern const char *SANDBOX_REPO_DIR_FIELD;

    extern const char *REPO_DIR_SETTING;

    extern const char *COMMENT_FIELD;
}

#endif
