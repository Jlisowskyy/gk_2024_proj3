//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef RELATIVEORDEREDDITHERING_HPP
#define RELATIVEORDEREDDITHERING_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"
#include "../Transformations/DitheringMaps.hpp"

/* external includes */
#include <QImage>

/* Forward declarations */
class ToolBar;

class RelativeOrderedDithering final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    RelativeOrderedDithering() = delete;

    explicit RelativeOrderedDithering(const ToolBar* tool_bar);

    ~RelativeOrderedDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;

    // ------------------------------
    // Class fields
    // ------------------------------
private:
    const double m_Kr{};
    const double m_Kg{};
    const double m_Kb{};
};


#endif //RELATIVEORDEREDDITHERING_HPP
