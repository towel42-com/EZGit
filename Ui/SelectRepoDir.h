#ifndef __UI_SELECTREPODIR_H
#define __UI_SELECTREPODIR_H

#include "BasePage.h"
#include <memory>

namespace NUi
{
    namespace Ui
    {
        class CSelectRepoDir;
    }

    class CSelectRepoDir : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectRepoDir( QWidget *parent = nullptr );
        ~CSelectRepoDir();

        virtual int nextId() const override;
        virtual bool isComplete() const override;

    private Q_SLOTS:
    private:
    private:
        bool isGitDir( const QString &dir, bool showMessage ) const;

        std::unique_ptr< Ui::CSelectRepoDir > fImpl;
    };
}
#endif
