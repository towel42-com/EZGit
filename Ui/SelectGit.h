#ifndef __UI_SELECTGIT_H
#define __UI_SELECTGIT_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectGit;
    }

    class CSelectGit : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectGit( QWidget *parent = nullptr );
        ~CSelectGit();

        virtual bool isComplete() const override;
        virtual int nextId() const override;
        virtual bool validatePage() override;
    
	private Q_SLOTS:
    private:

    private:
        std::unique_ptr< Ui::CSelectGit > fImpl;
    };
}
#endif
