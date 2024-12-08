//
// Created by Jlisowskyy on 08/12/24.
//

#ifndef DITHERINGMAPS_HPP
#define DITHERINGMAPS_HPP

#include <array>
#include <cinttypes>
#include <cstddef>
#include <cassert>

#include <iostream>

template<class NumT, size_t SIZE>
class SquareMatrix {
    // ------------------------------
    // Internal types
    // ------------------------------

    struct SquareMatrixFetcher {
        SquareMatrixFetcher() = delete;

        constexpr ~SquareMatrixFetcher() = default;

        constexpr SquareMatrixFetcher(const size_t row, NumT *const data): m_row(row), m_data(data) {
        }

        constexpr NumT &operator[](const size_t col) {
            return m_data[m_row * SIZE + col];
        }

        constexpr const NumT &operator[](const size_t col) const {
            return m_data[m_row * SIZE + col];
        }

    protected:
        size_t m_row{};
        NumT *m_data;
    };

public:
    // ------------------------------
    // Class creation
    // ------------------------------

    SquareMatrix() = delete;

    SquareMatrix(const SquareMatrix &) = delete;

    SquareMatrix(SquareMatrix &&) = delete;

    constexpr ~SquareMatrix() = default;

    explicit constexpr SquareMatrix(NumT *data) : m_data(data) {
    }

    // ------------------------------
    // Class interaction
    // ------------------------------

    constexpr NumT *data() const {
        return m_data;
    }

    constexpr SquareMatrixFetcher operator[](const size_t row) {
        return SquareMatrixFetcher(row, m_data);
    }

    // ------------------------------
    // Class fields
    // ------------------------------
protected:
    NumT *m_data;
};

static constexpr size_t DITHERING_SIZES[]{
    2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256
};

static constexpr size_t DITHERING_SIZES_COUNT = std::size(DITHERING_SIZES);

static constexpr size_t TOTAL_SIZE = []() -> size_t {
    size_t total_size = 0;
    for (const auto &size: DITHERING_SIZES) {
        total_size += size * size;
    }
    return total_size;
}();

static constexpr std::array<size_t, DITHERING_SIZES_COUNT> DITHERING_START_INDEXES = [
        ]() -> std::array<size_t, DITHERING_SIZES_COUNT> {
            std::array<size_t, DITHERING_SIZES_COUNT> rv{};
            size_t index = 0;

            for (size_t i = 0; i < DITHERING_SIZES_COUNT; ++i) {
                rv[i] = index;
                index += DITHERING_SIZES[i] * DITHERING_SIZES[i];
            }

            return rv;
        }();

template<size_t IDX>
static constexpr void InitMap_(double *map, double* prevMap) {
    constexpr size_t SIZE = DITHERING_SIZES[IDX];
    SquareMatrix<double, SIZE> mat(map);
    SquareMatrix<double, SIZE / 2> prevMat(prevMap);
    constexpr size_t HALF_SIZE = SIZE / 2;

    for (size_t i = 0; i < HALF_SIZE; ++i) {
        for (size_t j = 0; j < HALF_SIZE; ++j) {
            mat[i][j] = 4 * prevMat[i][j];
        }
    }

    for (size_t i = 0; i < HALF_SIZE; ++i) {
        for (size_t j = HALF_SIZE; j < SIZE; ++j) {
            mat[i][j] = 4 * prevMat[i][j - HALF_SIZE] + 2;
        }
    }

    for (size_t i = HALF_SIZE; i < SIZE; ++i) {
        for (size_t j = 0; j < HALF_SIZE; ++j) {
            mat[i][j] = 4 * prevMat[i - HALF_SIZE][j] + 3;
        }
    }

    for (size_t i = HALF_SIZE; i < SIZE; ++i) {
        for (size_t j = HALF_SIZE; j < SIZE; ++j) {
            mat[i][j] = 4 * prevMat[i - HALF_SIZE][j - HALF_SIZE] + 1;
        }
    }
}

template<size_t IDX>
static constexpr void InitMapDispatch(double *map, double* prevMap, const size_t idx) {
    if constexpr (IDX == 0) {
        InitMap_<0>(map, prevMap);
        return;
    }

    if (idx == IDX) {
        InitMap_<IDX>(map, prevMap);
        return;
    }

    if constexpr (IDX != 0) {
        InitMapDispatch<IDX - 1>(map, prevMap, idx);
    }
}

static constexpr void InitMap(double *map, double *prevMap, const size_t idx) {
    assert(idx < DITHERING_SIZES_COUNT);

    InitMapDispatch<DITHERING_SIZES_COUNT - 1>(map, prevMap, idx);
}

static constexpr std::array<double, TOTAL_SIZE> DITHERING_MAPS = []() -> std::array<double, TOTAL_SIZE> {
    std::array<double, TOTAL_SIZE> dithering_maps{};

    double *map0 = dithering_maps.data() + DITHERING_START_INDEXES[0];
    SquareMatrix<double, 2> mat0(map0);
    mat0[0][0] = 0.0;
    mat0[0][1] = 2.0;
    mat0[1][0] = 3.0;
    mat0[1][1] = 1.0;

    double *map1 = dithering_maps.data() + DITHERING_START_INDEXES[1];
    SquareMatrix<double, 3> mat1(map1);
    mat1[0][0] = 6.0;
    mat1[0][1] = 8.0;
    mat1[0][2] = 4.0;
    mat1[1][0] = 1.0;
    mat1[1][1] = 0.0;
    mat1[1][2] = 3.0;
    mat1[2][0] = 5.0;
    mat1[2][1] = 2.0;
    mat1[2][2] = 7.0;

    for (size_t idx = 2; idx < DITHERING_SIZES_COUNT; ++idx) {
        InitMap(
            dithering_maps.data() + DITHERING_START_INDEXES[idx],
            dithering_maps.data() + DITHERING_START_INDEXES[idx - 2],
            idx);
    }

    return dithering_maps;
}();

[[nodiscard]] std::pair<size_t, size_t> FindNearestSize(double size);

template<size_t IDX>
void DebugDisplayMatrix() {
    constexpr size_t SIZE = DITHERING_SIZES[IDX];
    constexpr size_t START_IDX = DITHERING_START_INDEXES[IDX];

    for (size_t y = 0; y < SIZE; ++y) {
        for (size_t x = 0; x < SIZE; ++x) {
            std::cout << DITHERING_MAPS[START_IDX + y * SIZE + x] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

// template<class FuncT>
// void RunWithDitheringMap()


#endif //DITHERINGMAPS_HPP
