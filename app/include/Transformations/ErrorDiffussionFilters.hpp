//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef ERRORDIFFUSSIONFILTERS_HPP
#define ERRORDIFFUSSIONFILTERS_HPP

/* external includes */
#include <array>

static constexpr std::array FLOYD_FILTER{
    0.0, 0.0, 0.0,
    0.0, 0.0, 7.0 / 16.0,
    3.0 / 16.0, 5.0 / 16.0, 1.0 / 16.0
};

static constexpr std::array BURKES_FILTER{
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 8.0 / 32.0, 4.0 / 32.0,
    2.0 / 32.0, 4.0 / 32.0, 8.0 / 32.0, 4.0 / 32.0, 2.0 / 32.0
};

static constexpr std::array STUCKY_FILTER{
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 8.0 / 42.0, 4.0 / 42.0,
    2.0 / 42.0, 4.0 / 42.0, 8.0 / 42.0, 4.0 / 42.0, 2.0 / 42.0,
    1.0 / 42.0, 2.0 / 42.0, 4.0 / 42.0, 2.0 / 42.0, 1.0 / 42.0
};

static constexpr std::array DEFAULT_FILTER = FLOYD_FILTER;
static constexpr size_t DEFAULT_FILTER_SIZE = 3;
static constexpr int DEFAULT_FILTER_DX = 1;
static constexpr int DEFAULT_FILTER_DY = 1;

#endif //ERRORDIFFUSSIONFILTERS_HPP
