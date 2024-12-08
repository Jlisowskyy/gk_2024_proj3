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

/* Octree */
static constexpr size_t NUM_CHILD = 8;

class OctTree final {
    // ------------------------------
    // Internal types
    // ------------------------------

    struct node {
        node() = delete;

        explicit node(const QRgb color) : m_color(color) {
        }

        ~node() {
            CleanUp();
        }

        void InsertColor(const QRgb color, const uint32_t range) {
            if (range == 0) {
                Q_ASSERT(m_color == color);
                ++m_count;
                return;
            }

            const uint32_t idx = GetColorIndex(color);

            if (!m_children[idx]) {
                m_children[idx] = new node(GetRangeColor(m_color, color, range));
            }

            m_children[idx]->InsertColor(color, range / 2);
        }

        [[nodiscard]] uint32_t GetColorIndex(const QRgb color) const {
            uint32_t idx{};

            idx |= (qRed(color) > qRed(m_color)) << 0;
            idx |= (qGreen(color) > qGreen(m_color)) << 1;
            idx |= (qBlue(color) > qBlue(m_color)) << 2;

            return idx;
        }

        [[nodiscard]] static QRgb GetRangeColor(const QRgb parentColor, const QRgb color, const uint32_t parentRange) {
            uint32_t idx{};

            /* We go level lower */
            const int childRange = static_cast<int>(parentRange / 2);

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

        QRgb m_color{};
        uint64_t m_count{};
        std::array<node *, NUM_CHILD> m_children{};
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:
    OctTree() = default;

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

    void InsertColor(QColor const &color) {
        if (m_root == nullptr) {
            m_root = new node(qRgb(127, 127, 127));
        }

        m_root->InsertColor(color.rgb(), 128);
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
            tree.InsertColor(QColor(pixelColor));
        }
    }
    tree.DumpToDotFile("octree_full.dot");

    /* reduce the colors */

    tree.DumpToDotFile("octree_reduced.dot");
}
