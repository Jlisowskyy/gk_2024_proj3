//
// Created by Jlisowskyy on 07/12/24.
//

#include "../include/Management/StateMgr.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

/* external includes */
#include <QFileDialog>
#include <QDebug>

StateMgr::StateMgr(QObject *parent) : QObject(parent) {
}

void StateMgr::setup(ToolBar *toolBar) {
    Q_ASSERT(toolBar != nullptr);
    Q_ASSERT(m_toolBar == nullptr);

    m_toolBar = toolBar;

    connect(toolBar->m_load, &QAction::triggered, this, &StateMgr::onLoadButtonClicked);
    connect(toolBar->m_refresh, &QAction::triggered, this, &StateMgr::onRefreshButtonClicked);
    connect(toolBar, &ToolBar::AlgorithmChanged, this, &StateMgr::onAlgorithmChanged);
}

void StateMgr::loadDefaults() {
    _loadImage(_loadTextureFromFile(UI_CONSTANTS::DEFAULT_IMAGE_PATH));
}

void StateMgr::onAlgorithmChanged(const ALGORITHMS::TYPE type) {
    qDebug() << "changing algorithm in state mgr";
    Q_ASSERT(type < ALGORITHMS::TYPE::LAST);

    const auto func = TRANSFORMATIONS[type];

    if (!func) {
        qDebug() << "No transformation for algorithm:" << ALGORITHMS::DESCRIPTIONS[type];
        return;
    }

    delete m_transformation;
    m_transformation = func(m_toolBar);

    delete m_transformedImage;
    m_transformedImage = new QImage(*m_image);

    m_transformation->TransformImage(*m_transformedImage);

    emit onTransformedImageChanged(m_transformedImage);
}

void StateMgr::onRefreshButtonClicked() {
    qDebug() << "refresh button clicked";

    if (m_transformation) {
        delete m_transformedImage;
        m_transformedImage = new QImage(*m_image);

        m_transformation->TransformImage(*m_transformedImage);

        emit onTransformedImageChanged(m_transformedImage);
    }
}

void StateMgr::onLoadButtonClicked() {
    qDebug() << "load button clicked";

    _loadImage(_loadTextureFromFile(_loadImageFileName()));
}

QImage *StateMgr::_loadTextureFromFile(const QString &path) {
    const auto pImage = new QImage(path);

    if (pImage->isNull()) {
        qWarning() << "Failed to load image from path:" << path;
        return nullptr;
    }

    return pImage;
}

QString StateMgr::_loadImageFileName() {
    const QString initialPath = m_previousDirectory.isEmpty() ? QDir::currentPath() : m_previousDirectory;
    const QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "Open File",
        initialPath,
        "Images (*.png *.jpg *.bmp);;All Files (*)"
    );

    if (!filePath.isEmpty()) {
        m_previousDirectory = QFileInfo(filePath).absolutePath();
        qDebug() << "File selected:" << filePath;
    } else {
        qDebug() << "No file selected.";
    }

    return filePath;
}

void StateMgr::_loadImage(QImage *const originalImage) {
    if (originalImage) {
        delete m_transformedImage;
        delete m_image;

        m_image = originalImage;
        m_transformedImage = new QImage(*originalImage);

        emit onOriginalImageChanged(originalImage);
        emit onTransformedImageChanged(m_transformedImage);
    }
}
