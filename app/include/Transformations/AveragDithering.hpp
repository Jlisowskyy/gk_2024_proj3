//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef AVERAGDITHERING_HPP
#define AVERAGDITHERING_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"

/* external includes */
#include <QImage>

/* Forward declaration */
class ToolBar;

class AverageDithering final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    AverageDithering() = default;

    explicit AverageDithering(const ToolBar * toolBar);

    ~AverageDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;


    // ------------------------------
    // Class fields
    // ------------------------------
private:
    const double m_Kr;
    const double m_Kg;
    const double m_Kb;
};

#endif //AVERAGDITHERING_HPP
