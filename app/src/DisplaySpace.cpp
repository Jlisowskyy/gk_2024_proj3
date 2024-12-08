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

DisplaySpace::DisplaySpace(QWidget *parent) : QWidget(parent) {
    auto *pMainLayout = new QHBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->setSpacing(10);

    /* left layout */
    auto *pOriginalLayout = new QVBoxLayout();
    pOriginalLayout->setContentsMargins(0, 0, 0, 0);
    auto *pOriginalImageLabel = new QLabel();
    pOriginalImageLabel->setScaledContents(true);
    pOriginalImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *pOriginalCaptionLabel = new QLabel("Original Image");
    pOriginalCaptionLabel->setAlignment(Qt::AlignCenter);
    pOriginalLayout->addWidget(pOriginalImageLabel, 1);
    pOriginalLayout->addWidget(pOriginalCaptionLabel);

    /* right layout */
    auto *pTransformedLayout = new QVBoxLayout();
    pTransformedLayout->setContentsMargins(0, 0, 0, 0);
    auto *pTransformedImageLabel = new QLabel();
    pTransformedImageLabel->setScaledContents(true);
    pTransformedImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *pTransformedCaptionLabel = new QLabel("Transformed Image");
    pTransformedCaptionLabel->setAlignment(Qt::AlignCenter);
    pTransformedLayout->addWidget(pTransformedImageLabel, 1);
    pTransformedLayout->addWidget(pTransformedCaptionLabel);

    /* main layout */
    pMainLayout->addLayout(pOriginalLayout, 1);
    pMainLayout->addLayout(pTransformedLayout, 1);

    m_originalImageLabel = pOriginalImageLabel;
    m_transformedImageLabel = pTransformedImageLabel;
}

void DisplaySpace::setup(StateMgr *stateMgr) {
    Q_ASSERT(stateMgr != nullptr);
    Q_ASSERT(m_stateMgr == nullptr);

    m_stateMgr = stateMgr;

    // Connect signals from StateMgr
    connect(stateMgr, &StateMgr::onOriginalImageChanged, this, &DisplaySpace::onOriginalImageChanged);
    connect(stateMgr, &StateMgr::onTransformedImageChanged, this, &DisplaySpace::onTransformedImageChanged);
}

void DisplaySpace::onOriginalImageChanged(QImage *image) {
    if (image) {
        const QPixmap pixmap = QPixmap::fromImage(*image).scaled(
            m_originalImageLabel->size(),
            Qt::IgnoreAspectRatio,
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
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        );
        m_transformedImageLabel->setPixmap(pixmap);
        m_transformedImageLabel->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "Original image is null";
    }
}
