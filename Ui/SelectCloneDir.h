#ifndef __UI_SELECTCLONEDIR_H
#define __UI_SELECTCLONEDIR_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectCloneDir;
    }

    class CSelectCloneDir : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectCloneDir( QWidget *parent = nullptr );
        ~CSelectCloneDir();

        virtual int nextId() const override;
        virtual void initializePage() override;

    private Q_SLOTS:
        void slotUpdateRepoDir();

    private:
    private:
        std::unique_ptr< Ui::CSelectCloneDir > fImpl;
    };
}
#endif
