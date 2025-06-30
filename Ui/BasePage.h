#ifndef __UI_BASEPAGE_H
#define __UI_BASEPAGE_H

#include <QWizardPage>
#include <memory>

namespace NUi
{
    class CEZGit;
    class CBasePage : public QWizardPage
    {
        Q_OBJECT

    public:
        explicit CBasePage( QWidget *parent = nullptr );
        ~CBasePage();

        CEZGit *ezGit() const;
        virtual int nextId() const override = 0;

    private Q_SLOTS:
    private:
    private:
    };
}
#endif
