#include "UrlValidator.h"

#include <QUrl>

namespace NUi
{

    CUrlValidator::CUrlValidator( QObject *parent /*= nullptr */ ) :
        QValidator( parent )
    {
    }

    CUrlValidator::~CUrlValidator()
    {
    }

    QValidator::State CUrlValidator::validate( QString &input, int & /*pos*/ ) const
    {
        auto url = QUrl::fromUserInput( input );
        if ( url.isLocalFile() )
            return QValidator::State::Invalid;
        if ( url.isValid() )
            return QValidator::State::Acceptable;

        return QValidator::State::Invalid;
    }
}