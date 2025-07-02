#ifndef __EZGIT_H
#define __EZGIT_H

#include "UiFwd.h"

#include <QWizard>
#include <QPointer>

class QPlainTextEdit;

namespace NUi
{
    class CRunGit;
    class CEZGit : public QWizard
    {
        Q_OBJECT;

    public:
        CEZGit( QWidget *parent = nullptr );

        bool showSelectGit() const;
        bool showSelectRemote() const;
        bool showSetCredentials() const;

        bool showInitPage( EPageID pageID ) const;
        bool isPageComplete( EPageID pageID ) const;

        QString getConfigValue( const QString &key ) const;

    public:
        std::pair< QString, bool > runGit( const QStringList &args ) const;

        QString getGitEmail() const;
        QString getGitUserName() const;
        bool confirmCredentialManager();
        void saveFields();

    private:
        EPageID getFirstPage() const;
        void loadFields();

    private:

    private:
        static QString getProgramFiles( bool sixtyFourBit );
        QString getGitExec() const;
    };
}

#endif
