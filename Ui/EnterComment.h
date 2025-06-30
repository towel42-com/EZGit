#ifndef __UI_ENTERCOMMENT_H
#define __UI_ENTERCOMMENT_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CEnterComment;
    }

    class CEnterComment : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CEnterComment( QWidget *parent = nullptr );
        ~CEnterComment();

        virtual int nextId() const override;

    private Q_SLOTS:
    private:

    private:
        std::unique_ptr< Ui::CEnterComment > fImpl;
    };
}
#endif
