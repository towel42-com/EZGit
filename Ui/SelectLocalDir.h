#ifndef __UI_SELECTLOCALDIR_H
#define __UI_SELECTLOCALDIR_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectLocalDir;
    }

    class CSelectLocalDir : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectLocalDir( QWidget *parent = nullptr );
        ~CSelectLocalDir();

        virtual int nextId() const override;
        virtual bool isComplete() const override;

    private Q_SLOTS:
    private:
    private:
        bool isGitDir( const QString &dir, bool showMessage ) const;

        std::unique_ptr< Ui::CSelectLocalDir > fImpl;
    };
}
#endif
