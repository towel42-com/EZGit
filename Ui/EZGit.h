#ifndef __EZGIT_H
#define __EZGIT_H

#include <QWizard>

namespace NUi
{
    class CEZGit : public QWizard
    {
        Q_OBJECT;

    public:
        CEZGit( QWidget *parent = nullptr );

    public:
        std::pair< QString, bool > runGit( const QStringList &args );

        virtual void accept() override;

    private:
        void loadFields();
        void saveFields();
        void clone();
    };
}

#endif
