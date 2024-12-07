//
// Created by Jlisowskyy on 07/12/24.
//

#include "../include/UiObjects/DisplaySpace.hpp"

/* internal includes */
#include "../include/Management/StateMgr.hpp"

/* external includes */
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

void DisplaySpace::setup(StateMgr *stateMgr) {
    Q_ASSERT(stateMgr != nullptr);
    Q_ASSERT(m_stateMgr == nullptr);

    m_stateMgr = stateMgr;

    /* left layout */
    auto *pOriginalLayout = new QVBoxLayout();
    auto *pOriginalImageLabel = new QLabel();
    auto *pOriginalCaptionLabel = new QLabel("Original Image");
    pOriginalCaptionLabel->setAlignment(Qt::AlignCenter);
    pOriginalLayout->addWidget(pOriginalImageLabel);
    pOriginalLayout->addWidget(pOriginalCaptionLabel);
    m_originalImageLabel = pOriginalImageLabel;

    /* right layout */
    auto *pTransformedLayout = new QVBoxLayout();
    auto *pTransformedImageLabel = new QLabel();
    auto *pTransformedCaptionLabel = new QLabel("Transformed Image");
    pTransformedCaptionLabel->setAlignment(Qt::AlignCenter);
    pTransformedLayout->addWidget(pTransformedImageLabel);
    pTransformedLayout->addWidget(pTransformedCaptionLabel);
    m_transformedImageLabel = pTransformedImageLabel;

    /* main layout */
    auto *pMainLayout = new QHBoxLayout(this);
    pMainLayout->addLayout(pOriginalLayout);
    pMainLayout->addLayout(pTransformedLayout);

    // Connect signals from StateMgr
    connect(stateMgr, &StateMgr::onOriginalImageChanged, this, &DisplaySpace::onOriginalImageChanged);
    connect(stateMgr, &StateMgr::onTransformedImageChanged, this, &DisplaySpace::onTransformedImageChanged);
}

void DisplaySpace::onOriginalImageChanged(QImage *image) {
    if (image) {
        const QPixmap pixmap = QPixmap::fromImage(*image).scaled(
            m_originalImageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        m_originalImageLabel->setPixmap(pixmap);
        m_originalImageLabel->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "Original image is null";
    }
}

void DisplaySpace::onTransformedImageChanged(QImage *image) {
    if (image) {
        const QPixmap pixmap = QPixmap::fromImage(*image).scaled(
            m_transformedImageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        m_transformedImageLabel->setPixmap(pixmap);
        m_transformedImageLabel->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "Original image is null";
    }
}
