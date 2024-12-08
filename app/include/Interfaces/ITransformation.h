//
// Created by Jlisowskyy on 07/12/24.
//

#ifndef ITRANSFORMATION_H
#define ITRANSFORMATION_H

/* internal includes */
#include "../Intf.hpp"

/* external includes */
#include <QImage>

class ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    ITransformation() = default;

    virtual ~ITransformation() = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    virtual void TransformImage(QImage &image) = 0;
};

#endif //ITRANSFORMATION_H
