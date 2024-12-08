//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/AveragDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

/* external includes */
#include <cmath>


AverageDithering::AverageDithering(const ToolBar *toolBar): m_Kr(toolBar->m_krSpin->value()),
                                                      m_Kg(toolBar->m_kgSpin->value()),
                                                      m_Kb(toolBar->m_kbSpin->value()) {
}

void AverageDithering::TransformImage(QImage &image) {
    const double redStep = 255.0 / (m_Kr - 1.0);
    const double greenStep = 255.0 / (m_Kg - 1.0);
    const double blueStep = 255.0 / (m_Kb - 1.0);

    for (int y = 0; y < image.height(); ++y) {
        const auto rowData = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            const QRgb pixelColor = rowData[x];

            const int red = static_cast<int>(std::round(static_cast<double>(qRed(pixelColor)) / redStep) * redStep);
            const int green = static_cast<int>(std::round(static_cast<double>(qGreen(pixelColor)) / greenStep) *
                                               greenStep);
            const int blue = static_cast<int>(std::round(static_cast<double>(qBlue(pixelColor)) / blueStep) * blueStep);

            rowData[x] = qRgb(red, green, blue);
        }
    }
}
