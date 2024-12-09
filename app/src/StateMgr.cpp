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
    connect(toolBar->m_save, &QAction::triggered, this, &StateMgr::onSaveImageClicked);
    connect(toolBar->m_generate, &QAction::triggered, this, &StateMgr::onGenerateButtonClicked);
}

void StateMgr::loadDefaults() {
    _loadImage(_loadTextureFromFile(UI_CONSTANTS::DEFAULT_IMAGE_PATH));
}

void StateMgr::onAlgorithmChanged(const ALGORITHMS::TYPE type) {
    qDebug() << "changing algorithm in state mgr";
    Q_ASSERT(type < ALGORITHMS::TYPE::LAST);

    m_currentAlgorithm = type;
}

void StateMgr::onRefreshButtonClicked() {
    qDebug() << "refresh button clicked";

    ITransformation *const newTransformation = TRANSFORMATIONS[m_currentAlgorithm](m_toolBar);

    delete m_transformedImage;
    m_transformedImage = new QImage(*m_image);

    newTransformation->TransformImage(*m_transformedImage);

    emit onTransformedImageChanged(m_transformedImage);
}

void StateMgr::onLoadButtonClicked() {
    qDebug() << "load button clicked";

    _loadImage(_loadTextureFromFile(_loadImageFileName()));
}

void StateMgr::onSaveImageClicked() {
    const QString fileName = QFileDialog::getSaveFileName(m_toolBar->m_toolBar, tr("Save Image File"),
                                                          QString(),
                                                          tr("Images (*.png)"));
    if (!fileName.isEmpty()) {
        qDebug() << "save image to " << fileName;
        const bool result = m_transformedImage->save(fileName);
        if (!result) {
            qDebug() << "save image failed";
        }
    } else {
        qDebug() << "save image failed";
    }
}

void StateMgr::onGenerateButtonClicked() {
    QImage *oldImage = m_image;
    m_image = _generateImage();

    delete m_transformedImage;
    delete oldImage;

    m_transformedImage = new QImage(*m_image);

    emit onOriginalImageChanged(m_image);
    emit onTransformedImageChanged(m_transformedImage);
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

        emit onOriginalImageChanged(m_image);
        emit onTransformedImageChanged(m_transformedImage);
    }
}

QImage *StateMgr::_generateImage() {
    QImage *image = new QImage(*m_image);

    /* paint the zebra */
    double coef = 0.3;
    int col = 0;

    const int zebraWidth = image->width() / 2;
    for (int x = 0; x < zebraWidth; x += int(coef * double(zebraWidth))) {
        const int size = int(coef * double(zebraWidth));
        const int range = x + size;
        const QColor color = col == 0 ? QColor(Qt::black) : QColor(Qt::white);

        for (int y = 0; y < m_image->height(); y++) {
            for (int xx = x; xx < range; xx++) {
                image->setPixelColor(xx, y, color);
            }
        }

        col ^= 1;
        coef = std::max(coef * 0.8, 0.02);
    }

    static constexpr double brightnessValue = 1.0;
    /* hsv model drawing */
    for (int y = 0; y < image->height(); y++) {
        for (int x = zebraWidth; x < image->width(); x++) {
            const double HDegrees = (double(x - zebraWidth) / double(zebraWidth)) * 360.0;
            const double SCoef = 1.0 - double(y) / double(image->height());

            image->setPixelColor(x, y, _convertHsvToRgb(HDegrees, SCoef, brightnessValue));
        }
    }


    return image;
}

QColor StateMgr::_convertHsvToRgb(double hue, double sat, double val) {
    hue = std::fmod(hue, 360.0);
    if (hue < 0) hue += 360.0;

    sat = std::clamp(sat, 0.0, 1.0);
    val = std::clamp(val, 0.0, 1.0);

    if (sat == 0.0) {
        const int grayValue = static_cast<int>(val * 255);
        return {grayValue, grayValue, grayValue};
    }

    const double sector = hue / 60.0;
    const int sectorIndex = static_cast<int>(sector);
    const double fracPart = sector - sectorIndex;

    const double p = val * (1.0 - sat);
    const double q = val * (1.0 - sat * fracPart);
    const double t = val * (1.0 - sat * (1.0 - fracPart));

    switch (sectorIndex) {
        case 0: return {
                static_cast<int>(val * 255),
                static_cast<int>(t * 255),
                static_cast<int>(p * 255)
            };
        case 1: return {
                static_cast<int>(q * 255),
                static_cast<int>(val * 255),
                static_cast<int>(p * 255)
            };
        case 2: return {
                static_cast<int>(p * 255),
                static_cast<int>(val * 255),
                static_cast<int>(t * 255)
            };
        case 3: return {
                static_cast<int>(p * 255),
                static_cast<int>(q * 255),
                static_cast<int>(val * 255)
            };
        case 4: return {
                static_cast<int>(t * 255),
                static_cast<int>(p * 255),
                static_cast<int>(val * 255)
            };
        default: return {
                static_cast<int>(val * 255),
                static_cast<int>(p * 255),
                static_cast<int>(q * 255)
            };
    }
}
