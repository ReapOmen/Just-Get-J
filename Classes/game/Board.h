#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>

class Board {
public:

    static const int GRID_SIZE = 5;

    Board();

    ~Board() = default;

    const std::vector<std::pair<int, int>>& select(int i, int j);

    bool click(int i, int j);

    const std::vector<std::vector<char>>& getGrid() const { return _grid; }

    void print() const;

private:
    std::vector<std::vector<char>> _grid;
    std::vector<std::vector<bool>> _visited;
    std::vector<std::pair<int, int>> _selected;

    void fill(int i, int j, char c);
    bool inRange(int i, int j) const;
    void resetVisited();
};

#endif // __BOARD_H__
