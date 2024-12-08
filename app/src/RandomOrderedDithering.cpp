//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/RandomOrderedDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

RandomOrderedDithering::RandomOrderedDithering(const ToolBar *toolBar) : m_Kr(toolBar->m_krSpin->value()),
                                                                         m_Kg(toolBar->m_kgSpin->value()),
                                                                         m_Kb(toolBar->m_kbSpin->value()) {
}

void RandomOrderedDithering::TransformImage(QImage &image) {
    image = image.mirrored(true, false);
}
