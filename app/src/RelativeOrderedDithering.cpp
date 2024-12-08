//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/RelativeOrderedDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

/* external includes */
#include <cmath>

RelativeOrderedDithering::RelativeOrderedDithering(const ToolBar *tool_bar) : m_Kr(tool_bar->m_krSpin->value()),
                                                                              m_Kg(tool_bar->m_kgSpin->value()),
                                                                              m_Kb(tool_bar->m_kbSpin->value()) {
}

void RelativeOrderedDithering::TransformImage(QImage &image) {
    for (int y = 0; y < image.height(); ++y) {
        const auto rowData = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            const QRgb pixelColor = rowData[x];

            const int r = qRed(pixelColor);
            const int g = qGreen(pixelColor);
            const int b = qBlue(pixelColor);

            int colors[] = {r, g, b};
            const double weights[] = {m_Kr, m_Kg, m_Kb};

            for (size_t idx = 0; idx < 3; ++idx) {
                const int color = colors[idx];
                const double weight = weights[idx];

                const auto [ditherSize, ditherIdx] = FindNearestSize(256 / (weight - 1));

                const size_t mapStartIndex = DITHERING_START_INDEXES[ditherIdx];

                const size_t matrixX = static_cast<size_t>(x) % ditherSize;
                const size_t matrixY = static_cast<size_t>(y) % ditherSize;

                const double mapValue =
                        DITHERING_MAPS[mapStartIndex + matrixY * ditherSize + matrixX];

                // Calculate scaling factors
                const double normalizedThreshold = mapValue / static_cast<double>((ditherSize * ditherSize));

                const double scaleFactor = weight - 1.0;
                const double normalizedColor = color / 255.0;

                // Apply dithering with threshold
                const double ditherValue = normalizedThreshold / scaleFactor;
                double newColorValue = std::floor(normalizedColor * scaleFactor + ditherValue) / scaleFactor;

                // Clamp and convert back to integer
                colors[idx] = std::clamp(static_cast<int>(std::round(newColorValue * 255.0)), 0, 255);
            }

            rowData[x] = qRgb(colors[0], colors[1], colors[2]);
        }
    }
}
