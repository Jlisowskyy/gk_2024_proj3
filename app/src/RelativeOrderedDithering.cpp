//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/RelativeOrderedDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

RelativeOrderedDithering::RelativeOrderedDithering(const ToolBar *tool_bar) : m_Kr(tool_bar->m_krSpin->value()),
                                                                              m_Kg(tool_bar->m_kgSpin->value()),
                                                                              m_Kb(tool_bar->m_kbSpin->value()) {
}

void RelativeOrderedDithering::TransformImage(QImage &image) {
    image = image.mirrored(true, false);
}
