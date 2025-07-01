#ifndef __UI_SELECTREMOTE_H
#define __UI_SELECTREMOTE_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectRemote;
    }

    class CSelectRemote : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectRemote( QWidget *parent = nullptr );
        ~CSelectRemote();

        virtual int nextId() const override;

    private Q_SLOTS:
    private:

    private:
        std::unique_ptr< Ui::CSelectRemote > fImpl;
    };
}
#endif
