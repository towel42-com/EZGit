#ifndef __UI_SELECTBRANCH_H
#define __UI_SELECTBRANCH_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectBranch;
    }

    class CSelectBranch : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectBranch( QWidget *parent = nullptr );
        ~CSelectBranch();

        virtual void initializePage() override;
        virtual int nextId() const override;

    public:
        void loadBranches();

        //virtual bool isComplete() const override;

    private Q_SLOTS:
    private:
    private:
        std::unique_ptr< Ui::CSelectBranch > fImpl;
    };
}
#endif
