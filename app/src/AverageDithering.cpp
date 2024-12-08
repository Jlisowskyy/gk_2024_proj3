//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/AveragDithering.hpp"

void AverageDithering::TransformImage(QImage &image) {
    image = image.mirrored(true, false);
}
