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

    class CRunGit : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CRunGit( QWidget *parent = nullptr );
        ~CRunGit();

        static std::pair< QString, bool > runGit( const QString &gitExec, const QStringList &args, const QString &pwd );

        virtual int nextId() const override { return -1; }
        virtual void initializePage() override;

    public:
        QString createCmdLine( const QString &gitExec, const QStringList &args ) const;

    private Q_SLOTS:
        void slotRunNextCmd();

    private:
        void addGitCmd( const QStringList &args, const QString &pwd, bool clearFirst );
        static bool runGit( QProcess *process, const QString &gitExec, const QStringList &args, const QString &pwd, bool waitForFinished );
        void runGit( const QStringList &args, const QString &pwd, bool clearFirst );

        void clone();
        void pull();
        void push();
        std::unique_ptr< Ui::CRunGit > fImpl;
        QProcess *fProcess{ nullptr };

        std::list< std::tuple< QStringList, QString, bool > > fGitCmds;
    };
}
#endif
