//
// Created by Jlisowskyy on 07/12/24.
//

#ifndef STATEMGR_HPP
#define STATEMGR_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"
#include "../UiObjects/ToolBar.hpp"

/* transformation includes */
#include "../Transformations/Identity.hpp"
#include "../Transformations/AveragDithering.hpp"
#include "../Transformations/PopularityDithering.hpp"
#include "../Transformations/ErrorDiffusionDithering.hpp"
#include "../Transformations/RandomOrderedDithering.hpp"
#include "../Transformations/RelativeOrderedDithering.hpp"

/* external includes */
#include <QObject>
#include <QImage>
#include <array>

/* Forward declaration */

class StateMgr final : public QObject {
    Q_OBJECT

    inline static std::array<ITransformation* (*)(ToolBar *), ALGORITHMS::TYPE::LAST> TRANSFORMATIONS{
        [](ToolBar *toolBar) -> ITransformation * { return new Identity(toolBar); },
        [](ToolBar *toolBar) -> ITransformation * { return new AverageDithering(toolBar); },
        [](ToolBar *toolBar) -> ITransformation * { return new ErrorDiffusionDithering(toolBar); },
        [](ToolBar *toolBar) -> ITransformation * { return new RandomOrderedDithering(toolBar); },
        [](ToolBar *toolBar) -> ITransformation * { return new RelativeOrderedDithering(toolBar); },
        [](ToolBar *toolBar) -> ITransformation * { return new PopularityDithering(toolBar); },
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:
    explicit StateMgr(QObject *parent);

    ~StateMgr() override = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void setup(ToolBar *toolBar);

    void loadDefaults();

    // ------------------------------
    // Class slots
    // ------------------------------

public slots:
    void onAlgorithmChanged(ALGORITHMS::TYPE type);

    void onRefreshButtonClicked();

    void onLoadButtonClicked();

    // ------------------------------
    // Class signals
    // ------------------------------

signals:
    void onTransformedImageChanged(QImage *image);

    void onOriginalImageChanged(QImage *image);

    // ------------------------------
    // Class private methods
    // ------------------------------
private:
    [[nodiscard]] static QImage *_loadTextureFromFile(const QString &path);

    QString _loadImageFileName();

    void _loadImage(QImage *originalImage);

    // ------------------------------
    // Class fields
    // ------------------------------

    ToolBar *m_toolBar{};

    QString m_previousDirectory{};
    ITransformation *m_transformation{};

    QImage *m_image{};
    QImage *m_transformedImage{};
};

#endif //STATEMGR_HPP
