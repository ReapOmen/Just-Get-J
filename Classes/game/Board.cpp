#include "Board.h"
#include "cocos2d.h"
#include <iostream>
#include <algorithm>

USING_NS_CC;
using std::vector;
using std::pair;
using std::cout;
using std::endl;

Board::Board()
    : _grid(GRID_SIZE),
      _visited(GRID_SIZE),
      _selected(0) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        _grid[i] = vector<char>(GRID_SIZE);
        _visited[i] = vector<bool>(GRID_SIZE);
        for (int j = 0; j < GRID_SIZE; ++j) {
            _grid[i][j] = getRandomChar();
        }
    }
}

const vector<pair<int, int>>& Board::select(int i, int j) {
    _selected = vector<pair<int, int>>(0);
    if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
        fill(i, j, _grid[i][j]);
        resetVisited();
    }
    return _selected;
}

bool Board::click(int i, int j) {
    pair<int, int> pair = std::make_pair(i, j);
    if (_selected.size() > 1) {
        auto selection = std::find(_selected.begin(),
                                   _selected.end(),
                                   pair);
        if (selection != _selected.end()) {
            for (auto pair = _selected.begin(); pair != _selected.end();) {
                if (pair->first == i && pair->second == j) {
                    _grid[i][j] += 1;
                } else {
                    _grid[pair->first][pair->second] = '\0';
                }
                _selected.erase(pair);
            }
            dropLetters();
            return true;
        }
    }
    return false;
}

void Board::print() const {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cout << _grid[i][j] << " ";
        }
        cout << endl;
    }
}

char Board::getRandomChar() {
    return RandomHelper::random_int(0, 2) + 'a';
}

void Board::fill(int i, int j, char c) {
    if (inRange(i, j) && _grid[i][j] == c && !_visited[i][j]) {
        _visited[i][j] = true;
        _selected.push_back(std::make_pair(i, j));
        fill(i + 1, j, c);
        fill(i - 1, j, c);
        fill(i, j + 1, c);
        fill(i, j - 1, c);
    }
}

bool Board::inRange(int i, int j) const {
    return i >= 0 && j >= 0 && i < GRID_SIZE && j < GRID_SIZE;
}

void Board::resetVisited() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            _visited[i][j] = false;
        }
    }
}

void Board::dropLetters() {
    for (int j = 0; j < GRID_SIZE; ++j) {
        for (int i = GRID_SIZE - 1; i >= 0; --i) {
            if (_grid[i][j] != '\0') {
                moveUntilBoundary(i, j);
            }
        }
    }
    fillWithRandom();
}

void Board::moveUntilBoundary(int i, int j) {
    while (i + 1 < GRID_SIZE) {
        if (_grid[i+1][j] == '\0') {
            _grid[i+1][j] = _grid[i][j];
            _grid[i][j] = '\0';
            ++i;
        } else {
            return;
        }
    }
}

void Board::fillWithRandom() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (_grid[i][j] == '\0') {
                _grid[i][j] = getRandomChar();
            }
        }
    }
}
