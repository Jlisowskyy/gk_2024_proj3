//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/PopularityDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

PopularityDithering::PopularityDithering(const ToolBar *toolBar) : m_K(toolBar->m_kSpin->value()) {
}

void PopularityDithering::TransformImage(QImage &image) {
    image = image.mirrored(true, false);
}
