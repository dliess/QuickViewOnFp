#include "QuickViewOnFpRenderMedium.h"
#include "FpIRenderMedium.h"
#include <iostream>

QuickViewOnFpRenderMedium::QuickViewOnFpRenderMedium(fp::IRenderMedium* pRenderMedium)
{
    if(nullptr == pRenderMedium)
    {
        std::cerr << "pRenderMedium is 'Null'" << std::endl;
        return;
    }
    QObject::connect(&fboQuickView, &FboQuickView::sceneRendered, [this, pRenderMedium](){
        QImage image = fboQuickView.grab();
        const fp::gfx::Rectangle rect( fp::gfx::Coord(0, 0),
                                       fp::gfx::Size2D(image.width(), image.height()) );
        pRenderMedium->streamToSubWindow(rect, reinterpret_cast<fp::ColorRGB*>(image.bits()));
        pRenderMedium->flushFrameBuffer();
    });

    QObject::connect(&fboQuickView, &FboQuickView::statusChanged,[this](FboQuickView::Status status){
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
            for (i = fboQuickView.errors().constBegin(); i != fboQuickView.errors().constEnd(); ++i)
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
    });
    const auto resolution = pRenderMedium->getResolution();
    fboQuickView.setGeometry(QRect(QPoint(0, 0), QSize(resolution.x, resolution.y)));
}

void QuickViewOnFpRenderMedium::setSource(const QString& source)
{
    fboQuickView.setSource(QUrl::fromLocalFile(source));
}