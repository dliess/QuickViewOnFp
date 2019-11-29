#ifndef QUICK_VIEW_ON_FP_DISPLAY
#define QUICK_VIEW_ON_FP_DISPLAY

#include "FpWidgetTypesOut.h"
#include <QObject>

class FpDisplayHandler;
class QString;
class FboQuickView;
class QuickViewOnFpDisplayPrivate;

namespace fpquickview
{

class QuickViewOnFpDisplay
{
public:
    QuickViewOnFpDisplay(FpDisplayHandler& rFpDisplayHandler,
                         fp::Display::Id displayID);
    ~QuickViewOnFpDisplay();
    void setSource(const QString& filename);
    FboQuickView& quickView();

private:
    QuickViewOnFpDisplayPrivate* d_ptr;
    Q_DECLARE_PRIVATE(QuickViewOnFpDisplay)
};

} // namespace fpquickview


#endif