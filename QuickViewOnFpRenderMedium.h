#ifndef QUICK_VIEW_ON_FP_RENDER_MEDIUM_H
#define QUICK_VIEW_ON_FP_RENDER_MEDIUM_H

#include "fboquickview.h"
#include <QString>

namespace fp
{
    class IRenderMedium;
}

struct QuickViewOnFpRenderMedium
{
    QuickViewOnFpRenderMedium(fp::IRenderMedium* pRenderMedium);
    void setSource(const QString& source);
    FboQuickView fboQuickView;
};

#endif