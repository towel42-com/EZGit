#ifndef __UI_SELECTBRANCHORTAG_H
#define __UI_SELECTBRANCHORTAG_H

#include "BasePage.h"
#include <memory>

class QTreeWidgetItem;
class QTreeWidget;
namespace NUi
{
    namespace Ui
    {
        class CSelectBranchOrTag;
    }

    class CSelectBranchOrTag : public CBasePage
    {
        Q_OBJECT

    public:
        explicit CSelectBranchOrTag( QWidget *parent = nullptr );
        ~CSelectBranchOrTag();

        virtual bool isComplete() const override;
        virtual void initializePage() override;
        virtual int nextId() const override;

    private Q_SLOTS:

    private:
        struct SBranchOrTag
        {
            QString fOID;
            QString fName;
            bool fIsBranch{ false };
            bool fIsDefaultBranch{ false };
            QTreeWidgetItem *createItem( QTreeWidget *parent );
        };
        std::list< SBranchOrTag > loadBranchesAndTags();

    private:
        QString getDefaultOID();
        std::unordered_map< QString, QString > getAllBranchesAndTags();

        QTreeWidgetItem *createItem( const QString &name, bool isBranch );

        std::unique_ptr< Ui::CSelectBranchOrTag > fImpl;
    };
}
#endif
