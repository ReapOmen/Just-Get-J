#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <map>

using ints = std::pair<int, int>;

class pairComp {
public:

    bool operator()(const ints& p1, const ints& p2) {
        return p1.first > p2.first && p1.second > p2.second;
    }
};

class Board {
public:

    static const int GRID_SIZE = 5;

    Board();

    ~Board() = default;

    const std::vector<std::pair<int, int>>& select(int i, int j);

    bool click(int i, int j);

    const std::vector<std::vector<char>>& getGrid() const { return _grid; }

    const std::vector<std::pair<ints, ints>>& getNewPositions()
        const { return _newPos; }

    const std::vector<std::pair<int, int>>& getNewValues()
        const { return _newValues; }

    void print() const;

private:
    std::vector<std::vector<char>> _grid;
    std::vector<std::vector<bool>> _visited;
    std::vector<std::pair<int, int>> _selected;
    std::vector<std::pair<ints, ints>> _newPos;
    std::vector<std::pair<int, int>> _newValues;

    char getRandomChar();
    void fill(int i, int j, char c);
    bool inRange(int i, int j) const;
    void resetVisited();
    void dropLetters();
    void moveUntilBoundary(int i, int j);
    void fillWithRandom();
};

#endif // __BOARD_H__
