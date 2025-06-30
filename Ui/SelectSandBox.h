#ifndef __UI_SELECTSANDBOX_H
#define __UI_SELECTSANDBOX_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectSandBox;
    }

    class CSelectSandBox : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectSandBox( QWidget *parent = nullptr );
        ~CSelectSandBox();

        virtual int nextId() const override;
        virtual void initializePage() override;

    private Q_SLOTS:
        void slotUpdateRepoDir();

    private:
    private:
        std::unique_ptr< Ui::CSelectSandBox > fImpl;
    };
}
#endif
