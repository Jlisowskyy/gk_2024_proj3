//
// Created by Jlisowskyy on 07/12/24.
//

#ifndef STATEMGR_HPP
#define STATEMGR_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"
#include "../UiObjects/ToolBar.hpp"

/* external includes */
#include <QObject>
#include <QImage>
#include <array>

/* Forward declaration */

class StateMgr final : public QObject {
    Q_OBJECT

    inline static std::array<ITransformation* (*)(ToolBar *), ALGORITHMS::TYPE::LAST> TRANSFORMATIONS{
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
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

    // ------------------------------
    // Class slots
    // ------------------------------

public slots:
    void onAlgorithmChanged(ALGORITHMS::TYPE type);

    void onRefreshButtonClicked();

    void onLoadButtonClicked();

    // ------------------------------
    // Class private methods
    // ------------------------------
private:
    [[nodiscard]] static QImage *_loadTextureFromFile(const QString &path);

    QImage *_loadTextureFromFile();

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
