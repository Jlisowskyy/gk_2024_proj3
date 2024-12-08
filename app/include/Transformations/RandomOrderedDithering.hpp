//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef RANDOMORDEREDDITHERING_HPP
#define RANDOMORDEREDDITHERING_HPP

/* internal includes */
#include "../Intf.hpp"
#include "../Interfaces/ITransformation.h"

/* external includes */
#include <QImage>
#include <random>

/* Forward declarations */
class ToolBar;

class RandomOrderedDithering final : public ITransformation {
public:
    // ------------------------------
    // Interface specification
    // ------------------------------

    RandomOrderedDithering() = delete;

    explicit RandomOrderedDithering(const ToolBar *toolBar);

    ~RandomOrderedDithering() override = default;

    // ------------------------------
    // Interface methods
    // ------------------------------

    void TransformImage(QImage &image) override;

    // ------------------------------
    // Class fields
    // ------------------------------
private:
    std::mt19937_64 m_gen{std::random_device{}()};
    const double m_Kr;
    const double m_Kg;
    const double m_Kb;
};


#endif //RANDOMORDEREDDITHERING_HPP
