#ifndef __UI_RUNGIT_H
#define __UI_RUNGIT_H

#include "BasePage.h"
#include <memory>

class QProcess;

namespace NUi
{
    namespace Ui
    {
        class CRunGit;
    }

    struct SGitCmd
    {
        SGitCmd();
        SGitCmd( const QStringList &args, const QString &pwd, bool clearFirst );
        QStringList fArgs;
        QString fPWD;
        bool fClearFirst{ false };
        bool fWaitForFinished{ false };
        std::function< void() > fPostRun;
    };

    class CRunGit : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CRunGit( QWidget *parent = nullptr );
        ~CRunGit();

        static std::pair< QString, bool > runGit( CEZGit *ezGit, const QString &gitExec, const QStringList &args, const QString &pwd );
        static std::pair< QString, bool > runGit( CEZGit *ezGit, const QString &gitExec, std::shared_ptr< SGitCmd > );

        virtual int nextId() const override { return -1; }
        virtual void initializePage() override;

    public:
        QString createCmdLine( const QString &gitExec, const QStringList &args ) const;

    private Q_SLOTS:
        void slotRunNextCmd();

    private:
        void addGitCmd( std::shared_ptr< SGitCmd > cmd );
        void addGitCmd( const QStringList &args, const QString &pwd, bool clearFirst );
        static bool runGit( CEZGit *ezGit, QProcess *process, const QString &gitExec, std::shared_ptr< SGitCmd > );
        void runGit( std::shared_ptr< SGitCmd > gitCmd );
        void runGit( const QStringList &args, const QString &pwd, bool clearFirst );

        void clone( bool clearFirst );
        void pull( bool clearFirst );
        void push( bool clearFirst );
        std::unique_ptr< Ui::CRunGit > fImpl;
        QProcess *fProcess{ nullptr };

        std::shared_ptr< SGitCmd > fCurrCmd;
        std::list< std::shared_ptr< SGitCmd > > fGitCmds;
    };
}
#endif
