#ifndef __UI_SUMMARY_H
#define __UI_SUMMARY_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSummary;
    }

    class CSummary : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSummary( QWidget *parent = nullptr );
        ~CSummary();

        virtual void initializePage() override;

    private Q_SLOTS:
    private:
    private:
        std::unique_ptr< Ui::CSummary > fImpl;
    };
}
#endif
