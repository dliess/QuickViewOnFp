#include "QuickViewOnFpDisplay.h"
#include "fboquickview.h"
#include "FpRenderIf.h"
#include "FpVector2D.h"
#include "FpWidgetOut_Spec.h"
#include "FpColorRGB.h"
#include "FpGfxTypes.h"
#include "FpDisplayHandler.h"
#include "DisaplayRenderCbIf.h"

//#include <QQmlContext>
#include <iostream>
using namespace fpquickview;

class DisplayRenderer : public DisaplayRenderCbIf
{
public:
    DisplayRenderer(FboQuickView& rFboQuickView);
    void revealed(IRender& r);
    void renderDisplays(IRender& r);
    void setRenderingRequested();
private:
    FboQuickView& m_rFboQuickView;
    bool          m_quickViewHasRendered;
};

DisplayRenderer::DisplayRenderer(FboQuickView& rFboQuickView) : 
    m_rFboQuickView(rFboQuickView),
    m_quickViewHasRendered(false)
    {}

void DisplayRenderer::revealed(IRender& r)
{
    r.setFrameBufRendering(true);
} 

void DisplayRenderer::renderDisplays(IRender& r)
{
    if(!m_quickViewHasRendered)
    {
        return;
    }
    m_quickViewHasRendered = false;
    QImage image = m_rFboQuickView.grab();
    const gfx::Rectangle rect( gfx::Coord(0, 0),
                               gfx::Size2D(image.width(), image.height()) );
    r.streamToSubWindow(rect, reinterpret_cast<ColorRGB*>(image.bits()));
}

void DisplayRenderer::setRenderingRequested()
{
    m_quickViewHasRendered = true;
}

class QuickViewOnFpDisplayPrivate : public QObject
{
    Q_OBJECT
public:
    explicit QuickViewOnFpDisplayPrivate(FpDisplayHandler& rFpDisplayHandler,
                                         fp::Display::Id displayID);
    ~QuickViewOnFpDisplayPrivate();
    FboQuickView*    m_pFboQuickView;
    DisplayRenderer* m_pDisplayRenderer;
public slots:
    void onQuickViewStateChange(FboQuickView::Status status);
};

QuickViewOnFpDisplayPrivate::QuickViewOnFpDisplayPrivate(FpDisplayHandler& rFpDisplayHandler,
                                                         fp::Display::Id displayID) :
    QObject(Q_NULLPTR),
    m_pFboQuickView(new FboQuickView),
    m_pDisplayRenderer(new DisplayRenderer(*m_pFboQuickView))
{
    rFpDisplayHandler.registerDisplayCb(*m_pDisplayRenderer, DisplayWidget(displayID));

    QObject::connect(m_pFboQuickView, &FboQuickView::sceneRendered, [this](){
        m_pDisplayRenderer->setRenderingRequested();
    });
    QObject::connect(m_pFboQuickView, &FboQuickView::statusChanged,
                        this,         &QuickViewOnFpDisplayPrivate::onQuickViewStateChange);

    const auto displayRes = fp::Display::getResolution(displayID);
    m_pFboQuickView->setGeometry(QRect(QPoint(0, 0), QSize(displayRes.x, displayRes.y)));
    //quickView.rootContext()->setContextProperty("applicationPath", "file://"+qApp->applicationDirPath()+ "/");
}

QuickViewOnFpDisplayPrivate::~QuickViewOnFpDisplayPrivate()
{
    delete m_pDisplayRenderer;
    delete m_pFboQuickView;
}


void QuickViewOnFpDisplayPrivate::onQuickViewStateChange(FboQuickView::Status status)
{
    switch(status)
    {
    case FboQuickView::Status::Null:
        std::cout << "FboQuickView status changed to 'Null'" << std::endl;
        break;
    case FboQuickView::Status::Ready:
        std::cout << "FboQuickView status changed to 'Ready'" << std::endl;
        break;
    case FboQuickView::Status::Loading:
        std::cout << "FboQuickView status changed to 'Loading'" << std::endl;
        break;
    case FboQuickView::Status::Error:
    {
        QList<QQmlError>::const_iterator i;
        for (i = m_pFboQuickView->errors().constBegin(); i != m_pFboQuickView->errors().constEnd(); ++i)
        {
            std::cerr << "FboQuickView Error occurred: " << i->description().toStdString() << std::endl;
        }
        std::cerr << "Error occured, exiting" << std::endl;
        exit(-1);
    }
    default:
        std::cerr << "FboQuickView Status changed to 'unknown state'" << std::endl;
        break;
    }
}

QuickViewOnFpDisplay::QuickViewOnFpDisplay(FpDisplayHandler& rFpDisplayHandler,
                                           fp::Display::Id displayID) :
    d_ptr(new QuickViewOnFpDisplayPrivate(rFpDisplayHandler, displayID))
{
}

QuickViewOnFpDisplay::~QuickViewOnFpDisplay()
{
    delete d_ptr;
}

void QuickViewOnFpDisplay::setSource(const QString& filename)
{
    Q_D(QuickViewOnFpDisplay);

    d->m_pFboQuickView->setSource(QUrl::fromLocalFile(filename));
}

FboQuickView& QuickViewOnFpDisplay::quickView()
{
    Q_D(QuickViewOnFpDisplay);

    return *d->m_pFboQuickView;
}



#include "QuickViewOnFpDisplay.moc"
