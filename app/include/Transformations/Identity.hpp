//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef IDENTITY_HPP
#define IDENTITY_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"

/* external includes */
#include <QImage>

/* forward declarations */
class ToolBar;

class Identity final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    Identity() = default;

    explicit Identity(ToolBar *) {
    };

    ~Identity() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage([[maybe_unused]] QImage &image) override {
        /* do nothing */
    }
};


#endif //IDENTITY_HPP
