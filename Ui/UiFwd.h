#ifndef __UIFWD_H
#define __UIFWD_H

namespace NUi
{
    enum class EPageID
    {
        eSelectGit,
        eSelectRepo,
        eSelectGoal,
        eSelectBranch,
        eSelectCloneDir,
        eSelectRepoDir,
        eEnterComment,
        eSummary,
        eRunGit
    };

    extern const char *SHOW_SELECT_GIT_FIELD;
    extern const char *SHOW_SELECT_GIT_SETTING;

    extern const char *SHOW_SELECT_REPO_FIELD;
    extern const char *SHOW_SELECT_REPO_SETTING;

    extern const char *GIT_EXEC_FIELD;
    extern const char *GIT_EXEC_SETTING;

    extern const char *REPO_URL_FIELD;
    extern const char *REPO_URL_SETTING;

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
