//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/ErrorDiffusionDithering.hpp"

void ErrorDiffusionDithering::TransformImage(QImage &image) {
    image = image.mirrored(true, false);
}
