#ifndef __UI_SETCREDENTIALS_H
#define __UI_SETCREDENTIALS_H

#include "BasePage.h"
#include <memory>

class QLineEdit;
class QToolButton;

namespace NUi
{
    namespace Ui
    {
        class CSetCredentials;
    }

    class CSetCredentials : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSetCredentials( QWidget *parent = nullptr );
        ~CSetCredentials();

        virtual int nextId() const override;
        virtual bool isComplete() const override;
        virtual void initializePage() override;
       
    private Q_SLOTS:

    private:
        void loadLoggedInUser();
        bool fTextChanged{ false };
        std::unique_ptr< Ui::CSetCredentials > fImpl;
    };
}
#endif
