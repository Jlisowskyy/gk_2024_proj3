//
// Jakub Lisowski 2024
//

#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

#include <QColor>
#include <array>

namespace ALGORITHMS {
    enum TYPE : uint32_t {
        AVERAGE,
        ERROR_DIFFUSION,
        RANDOM_ORDERED,
        RELATIVE_ORDERED,
        POPULARITY,
        LAST,
    };

    static constexpr std::array<const char *, TYPE::LAST> DESCRIPTIONS{
        "Average dithering",
        "Error diffusion dithering",
        "Random ordered dithering",
        "Relative ordered dithering",
        "Popularity algorithm",
    };

    static constexpr uint32_t DEFAULT_SELECTION = AVERAGE;
}

namespace UI_CONSTANTS {
    static constexpr uint64_t DEFAULT_ICON_SIZE = 32;
    static constexpr const char *DEFAULT_BUTTON_PAYLOAD = "payload";

    static constexpr const char *DEFAULT_IMAGE_PATH = ":/images/img1.png";
}

#endif /* APP_CONSTANTS_H */
