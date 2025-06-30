#ifndef __UI_SELECTGOAL_H
#define __UI_SELECTGOAL_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectGoal;
    }

    class CSelectGoal : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectGoal( QWidget *parent = nullptr );
        ~CSelectGoal();

        virtual int nextId() const override;
        virtual bool isComplete() const override;

    private Q_SLOTS:
    private:

    private:
        std::unique_ptr< Ui::CSelectGoal > fImpl;
    };
}
#endif
