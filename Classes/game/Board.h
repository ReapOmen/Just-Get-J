
#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <map>

using Coord = std::pair<int, int>;

class Board {
public:

    static const int GRID_SIZE = 5;

    Board();

    ~Board() = default;

    const std::vector<Coord>& select(int i, int j);

    bool click(int i, int j);

    const std::vector<std::vector<char>>& getGrid() const { return _grid; }

    const std::vector<std::pair<Coord, Coord>>& getNewPositions()
        const { return _newPos; }

    const std::vector<Coord>& getNewValues()
        const { return _newValues; }

    void print() const;

private:
    std::vector<std::vector<char>> _grid;
    std::vector<std::vector<bool>> _visited;
    std::vector<Coord> _selected;
    std::vector<std::pair<Coord, Coord>> _newPos;
    std::vector<Coord> _newValues;

    char getRandomChar();
    void fill(int i, int j, char c);
    bool inRange(int i, int j) const;
    void resetVisited();
    void dropLetters();
    void moveUntilBoundary(int i, int j);
    void fillWithRandom();
};

#endif // __BOARD_H__
