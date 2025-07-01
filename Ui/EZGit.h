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
        std::pair< QString, bool > runGit( const QStringList &args, bool wait=false ) const;

        QString getGitEmail() const;
        QString getGitUserName() const;

    public:
        void saveFields();

    private:
        int getFirstPage() const;
        void loadFields();

    private:
        static QString getProgramFiles( bool sixtyFourBit );
        QString getGitExec() const;
    };
}

#endif
