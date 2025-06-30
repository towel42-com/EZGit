#ifndef __UI_SELECTREPO_H
#define __UI_SELECTREPO_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectRepo;
    }

    class CSelectRepo : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectRepo( QWidget *parent = nullptr );
        ~CSelectRepo();

    private Q_SLOTS:
    private:

    private:
        std::unique_ptr< Ui::CSelectRepo > fImpl;
    };
}
#endif
