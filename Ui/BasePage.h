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

    private Q_SLOTS:
    private:
    private:
    };
}
#endif
