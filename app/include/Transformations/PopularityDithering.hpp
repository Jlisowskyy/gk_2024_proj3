//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef POPULARITYDITHERING_HPP
#define POPULARITYDITHERING_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"

/* external includes */
#include <QImage>

/* Forward declaration */
class ToolBar;

class PopularityDithering final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    PopularityDithering() = delete;

    explicit PopularityDithering(const ToolBar* toolBar);

    ~PopularityDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;

    // ------------------------------
    // Class fields
    // ------------------------------
private:
    const double m_K;
};


#endif //POPULARITYDITHERING_HPP
