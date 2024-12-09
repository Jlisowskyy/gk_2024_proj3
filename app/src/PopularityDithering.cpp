//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/PopularityDithering.hpp"

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"

/* External includes */
#include <array>
#include <functional>
#include <fstream>
#include <iomanip>
#include <QDebug>
#include <limits>

/* Octree */
static constexpr size_t NUM_CHILD = 8;

class OctTree final {
    // ------------------------------
    // Internal types
    // ------------------------------

    struct node {
        node() = delete;

        explicit node(const QRgb color, const bool isLeaf = false) : m_color(color), m_isLeaf(isLeaf) {
        }

        ~node() {
            CleanUp();
        }

        uint64_t Reduce() {
            Q_ASSERT(m_minCount != std::numeric_limits<uint64_t>::max());
            Q_ASSERT(m_minChildIdx != std::numeric_limits<uint64_t>::max());

            if (m_children[m_minChildIdx]->m_isLeaf) {
                const uint64_t count = m_children[m_minChildIdx]->m_count;

                --m_childCount;
                delete m_children[m_minChildIdx];
                m_children[m_minChildIdx] = nullptr;

                _setMin();
                return count;
            }

            const uint64_t count = m_children[m_minChildIdx]->Reduce();

            if (m_children[m_minChildIdx]->m_childCount == 0) {
                --m_childCount;
                delete m_children[m_minChildIdx];
                m_children[m_minChildIdx] = nullptr;
            }

            m_count -= count;
            _setMin();
            return count;
        }

        bool InsertColor(const QRgb color, const uint32_t range) {
            ++m_count;

            if (range == 0) {
                Q_ASSERT(m_color == color);
                m_minCount = m_count;
                return m_count == 1;
            }

            const uint32_t idx = GetColorIndex(color);

            if (!m_children[idx]) {
                m_children[idx] = new node(GetRangeColor(m_color, color, range), range == 1);
                ++m_childCount;
            }

            const bool result = m_children[idx]->InsertColor(color, range / 2);
            _setMin();
            Q_ASSERT(m_minCount != std::numeric_limits<uint64_t>::max());
            return result;
        }

        [[nodiscard]] uint32_t GetColorIndex(const QRgb color) const {
            uint32_t idx{};

            idx |= (qRed(color) > qRed(m_color)) << 0;
            idx |= (qGreen(color) > qGreen(m_color)) << 1;
            idx |= (qBlue(color) > qBlue(m_color)) << 2;

            return idx;
        }

        [[nodiscard]] static QRgb GetRangeColor(const QRgb parentColor, const QRgb color, const uint32_t parentRange) {
            /* We go level lower */
            const int childRange = static_cast<int>(parentRange / 2);

            if (childRange == 0) {
                return color;
            }

            const int red = qRed(parentColor) + (qRed(color) > qRed(parentColor) ? childRange : -childRange);
            const int green = qGreen(parentColor) + (qGreen(color) > qGreen(parentColor) ? childRange : -childRange);
            const int blue = qBlue(parentColor) + (qBlue(color) > qBlue(parentColor) ? childRange : -childRange);

            return qRgb(red, green, blue);
        }

        void CleanUp() {
            for (auto &child: m_children) {
                delete child;
                child = nullptr;
            }
        }

        [[nodiscard]] QRgb FindClosestColor(const QRgb color) const {
            if (m_childCount == 0) {
                Q_ASSERT(m_isLeaf);
                return m_color;
            }

            const uint32_t idx = GetColorIndex(color);
            if (m_children[idx]) {
                return m_children[idx]->FindClosestColor(color);
            }

            return m_children[_pickClosestColorIdx(color)]->FindClosestColor(color);
        }

    private:
        void _setMin() {
            uint64_t minCount = std::numeric_limits<uint64_t>::max();
            uint64_t minChildIdx = std::numeric_limits<uint64_t>::max();

            for (size_t i = 0; i < m_children.size(); ++i) {
                if (m_children[i]) {
                    if (minCount > m_children[i]->m_minCount) {
                        minCount = m_children[i]->m_minCount;
                        minChildIdx = i;
                    }
                }
            }

            m_minCount = minCount;
            m_minChildIdx = minChildIdx;
        }

        [[nodiscard]] uint32_t _pickClosestColorIdx(const QRgb color) const {
            uint32_t bestPickIdx = 0;
            uint64_t minDistance = std::numeric_limits<uint64_t>::max();

            for (uint32_t idx = 0; idx < NUM_CHILD; ++idx) {
                if (m_children[idx]) {
                    const QRgb childColor = m_children[idx]->m_color;

                    const int64_t redDiff = qRed(color) - qRed(childColor);
                    const int64_t greenDiff = qGreen(color) - qGreen(childColor);
                    const int64_t blueDiff = qBlue(color) - qBlue(childColor);

                    const uint64_t distance =
                        redDiff * redDiff +
                        greenDiff * greenDiff +
                        blueDiff * blueDiff;

                    if (distance < minDistance) {
                        minDistance = distance;
                        bestPickIdx = idx;
                    }
                }
            }

            return bestPickIdx;
        }

    public:
        QRgb m_color{}; /* leaf - color of the pixel; non-leaf - color describing the cube */
        uint64_t m_count{};
        /* leaf - amount of times this color appears in the picture; non-leaf - sum of children appearances */
        bool m_isLeaf;

        uint64_t m_minCount{};
        uint64_t m_minChildIdx{};

        uint32_t m_childCount{};
        std::array<node *, NUM_CHILD> m_children{};
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:
    OctTree() : m_root(new node(qRgb(127, 127, 127))) {
    }

    ~OctTree() {
        delete m_root;
    }


    OctTree(OctTree const &) = delete;

    OctTree(OctTree &&) = delete;

    OctTree &operator=(OctTree const &) = delete;

    OctTree &operator=(OctTree &&) = delete;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void Reduce(const uint32_t numColors) {
        while (m_count > numColors) {
            m_root->Reduce();
            --m_count;
        }
    }

    void InsertColor(const QRgb color) {
        m_count += m_root->InsertColor(color, 128);
    }

    [[nodiscard]] QRgb FindClosestColor(const QRgb color) const {
        return m_root->FindClosestColor(color);
    }

    void DumpToDotFile(const std::string &filename) const {
        std::ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            qDebug() << "Could not open file for writing: " << QString::fromStdString(filename);
            return;
        }

        dotFile << "digraph OctTree {\n";
        dotFile << "    node [style=filled];\n";

        size_t nodeCounter = 0;

        std::function<void(const node *, size_t)> traverseNode =
                [&](const node *currentNode, const size_t parentId) {
            if (!currentNode) return;

            const size_t currentNodeId = nodeCounter++;

            char colorLabel[64];
            snprintf(colorLabel, sizeof(colorLabel),
                     "RGB(%d,%d,%d)\\nCount: %lu",
                     qRed(currentNode->m_color),
                     qGreen(currentNode->m_color),
                     qBlue(currentNode->m_color),
                     currentNode->m_count);

            dotFile << "    node" << currentNodeId << " [label=\"" << colorLabel
                    << "\", fillcolor=\"#"
                    << std::hex
                    << std::setw(2) << std::setfill('0') << qRed(currentNode->m_color)
                    << std::setw(2) << std::setfill('0') << qGreen(currentNode->m_color)
                    << std::setw(2) << std::setfill('0') << qBlue(currentNode->m_color)
                    << std::dec << "\"];\n";

            if (parentId != SIZE_MAX) {
                dotFile << "    node" << parentId << " -> node" << currentNodeId << ";\n";
            }

            for (const auto &child: currentNode->m_children) {
                if (child) {
                    traverseNode(child, currentNodeId);
                }
            }
        };

        if (m_root) {
            traverseNode(m_root, SIZE_MAX);
        }

        dotFile << "}\n";
        dotFile.close();
    }


    // ------------------------------
    // Class fields
    // ------------------------------
private:
    node *m_root{};
    uint64_t m_count{};
};


PopularityDithering::PopularityDithering(const ToolBar *toolBar) : m_K(toolBar->m_kSpin->value()) {
}

void PopularityDithering::TransformImage(QImage &image) {
    Q_ASSERT(!image.isNull());

    OctTree tree;

    /* Build the tree */
    for (int y = 0; y < image.height(); ++y) {
        const auto rowData = reinterpret_cast<const QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            const QRgb pixelColor = rowData[x];
            tree.InsertColor(pixelColor);
        }
    }
    tree.DumpToDotFile("octree_full.dot");

    /* reduce the colors */
    tree.Reduce(static_cast<uint64_t>(m_K));
    tree.DumpToDotFile("octree_reduced.dot");

    /* replace the colors */
    for (int y = 0; y < image.height(); ++y) {
        const auto rowData = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            rowData[x] = tree.FindClosestColor(rowData[x]);
        }
    }
}
