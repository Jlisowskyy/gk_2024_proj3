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

    explicit AverageDithering(ToolBar *) {
    }

    ~AverageDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;
};

#endif //AVERAGDITHERING_HPP
