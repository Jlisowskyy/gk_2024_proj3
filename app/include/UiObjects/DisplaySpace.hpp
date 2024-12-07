//
// Created by Jlisowskyy on 07/12/24.
//

#ifndef DISPLAYSPACE_HPP
#define DISPLAYSPACE_HPP

/* internal includes */
#include "../Intf.hpp"

/* external includes */
#include <QImage>
#include <QWidget>
#include <QLabel>

/* Forward declaration */
class StateMgr;


class DisplaySpace : public QWidget {
    Q_OBJECT

    // ------------------------------
    // Class creation
    // ------------------------------

public:
    explicit DisplaySpace(QWidget *parent);

    ~DisplaySpace() override = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void setup(StateMgr *stateMgr);

    // ------------------------------
    // Class slots
    // ------------------------------

public slots:
    void onOriginalImageChanged(QImage *image);

    void onTransformedImageChanged(QImage *image);

    // ------------------------------
    // Class protected methods
    // ------------------------------

protected:
    // ------------------------------
    // Class fields
    // ------------------------------

    QImage *m_originalImage{};
    QImage *m_transformedImage{};

    StateMgr *m_stateMgr{};

    QLabel *m_originalImageLabel{};
    QLabel *m_transformedImageLabel{};
};

#endif //DISPLAYSPACE_HPP
