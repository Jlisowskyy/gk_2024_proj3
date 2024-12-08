//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/ErrorDiffusionDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

/* external includes */
#include <cmath>

#include "../include/Transformations/ErrorDiffussionFilters.hpp"

ErrorDiffusionDithering::ErrorDiffusionDithering(const ToolBar *toolBar) : m_Kr(toolBar->m_krSpin->value()),
                                                                           m_Kg(toolBar->m_kgSpin->value()),
                                                                           m_Kb(toolBar->m_kbSpin->value()) {
}

void ErrorDiffusionDithering::TransformImage(QImage &image) {
    const auto approx = [](const int color, const double numColors) -> int {
        const double step = 255.0 / (numColors - 1.0);

        return static_cast<int>(std::round(static_cast<double>(color) / step) * step);
    };

    auto copy = image.copy();

    for (int y = 0; y < image.height(); ++y) {
        const auto rowDataSource = reinterpret_cast<const QRgb *>(image.scanLine(y));
        const auto rowDataDest = reinterpret_cast<QRgb *>(copy.scanLine(y));

        for (int x = 0; x < image.width(); ++x) {
            const QRgb pixelColor = rowDataSource[x];

            const int r = qRed(pixelColor);
            const int g = qGreen(pixelColor);
            const int b = qBlue(pixelColor);

            const int approxRed = approx(r, m_Kr);
            const int approxGreen = approx(g, m_Kg);
            const int approxBlue = approx(b, m_Kb);

            rowDataDest[x] = qRgb(approxRed, approxGreen, approxBlue);

            const double errorRed = r - approxRed;
            const double errorGreen = g - approxGreen;
            const double errorBlue = b - approxBlue;

            for (int dx = -DEFAULT_FILTER_DX; dx <= DEFAULT_FILTER_DX; ++dx) {
                for (int dy = -DEFAULT_FILTER_DY; dy <= DEFAULT_FILTER_DY; ++dy) {
                    const int nx = x + dx;
                    const int ny = y + dy;

                    if (nx >= 0 && nx < image.width() && ny >= 0 && ny <= image.height()) {
                        const auto nRowDataSource = reinterpret_cast<QRgb *>(image.scanLine(ny));
                        const double filterValue = DEFAULT_FILTER[
                            dy + DEFAULT_FILTER_DY * DEFAULT_FILTER_SIZE + dx + DEFAULT_FILTER_DX];

                        const int filterRed = static_cast<int>(
                            static_cast<double>(qRed(nRowDataSource[nx])) + errorRed * filterValue);

                        const int filterGreen = static_cast<int>(
                            static_cast<double>(qGreen(nRowDataSource[nx])) + errorGreen * filterValue);

                        const int filterBlue = static_cast<int>(
                            static_cast<double>(qBlue(nRowDataSource[nx])) + errorBlue * filterValue);

                        nRowDataSource[nx] = qRgb(std::clamp(filterRed, 0, 255),
                                                  std::clamp(filterGreen, 0, 255),
                                                  std::clamp(filterBlue, 0, 255));
                    }
                }
            }
        }
    }

    image = copy;
}
