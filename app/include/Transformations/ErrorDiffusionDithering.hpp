//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef ERRORDIFFUSIONDITHERING_HPP
#define ERRORDIFFUSIONDITHERING_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"

/* external includes */
#include <QImage>

/* Forward declarations */
class ToolBar;

class ErrorDiffusionDithering final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    ErrorDiffusionDithering() = delete;

    explicit ErrorDiffusionDithering(ToolBar *) {
    }

    ~ErrorDiffusionDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;
};

#endif //ERRORDIFFUSIONDITHERING_HPP
