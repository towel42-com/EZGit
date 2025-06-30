#ifndef __UI_URLVALIDATOR
#define __UI_URLVALIDATOR

#include <QValidator>
#include <memory>

namespace NUi
{
    class CUrlValidator : public QValidator
    {
        Q_OBJECT

    public:
        explicit CUrlValidator( QObject *parent = nullptr );
        ~CUrlValidator();

        virtual QValidator::State validate( QString &input, int &pos ) const override;

    private Q_SLOTS:
    private:
    private:
    };
}
#endif
