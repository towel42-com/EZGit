#ifndef __EZGIT_H
#define __EZGIT_H

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

    public:
        void saveFields();

    private:
        void loadFields();
    };
}

#endif
