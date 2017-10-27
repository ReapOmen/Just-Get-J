#include "GridScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using std::string;
using std::to_string;
using std::vector;
using std::unique_ptr;
using ui::Button;
using ui::Layout;

Scene* GridScene::createScene() {
    return GridScene::create();
}

bool GridScene::init() {
    if ( !Scene::init() ) {
        return false;
    }

    _board = unique_ptr<Board>(new Board());
    _board->addListener(this);
    _highlighted = vector<Button*>(0);
    _selected = false;

    setupLayout();
    createGridOfButtons();

    // set up the content size of the layout
    auto buttonSize = _buttons[0][0]->getContentSize();
    auto layoutSize = Size(buttonSize.width * _buttons.size(),
                           buttonSize.height * _buttons.size());
    _layout->setContentSize(layoutSize);

    addChild(_layout);

    return true;
}

void GridScene::notify(const std::string& message) {
    if (message == Board::M_GAME_OVER) {
        onGameOver();
    }
}

void GridScene::setupLayout() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _layout = Layout::create();
    _layout->setLayoutType(Layout::Type::ABSOLUTE);
    _layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layout->setPosition(Vec2(visibleSize.width / 2,
                              visibleSize.height / 2));
    _layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _layout->setBackGroundColor(Color3B::GRAY);
}

void GridScene::createGridOfButtons() {
    auto grid = _board->getGrid();
    auto gridSize = grid.size();
    _buttons = vector<vector<Button*>>(gridSize);
    for (size_t i = 0; i < gridSize; ++i) {
        _buttons[i] = vector<Button*>();
        for (size_t j = 0; j < gridSize; ++j) {
            auto button = createButton(i, j, grid[i][j]);
            _buttons[i].push_back(button);
            auto btnSize = button->getContentSize();
            float x = j * btnSize.width + btnSize.width / 2.0f;
            float y = (gridSize - i - 1) * btnSize.height + btnSize.height / 2.0f;
            button->setPosition(Vec2(x, y));
        }
    }
}

Button* GridScene::createButton(int i, int j, char c) {
    auto button = Button::create("button_" + string(1, c) + ".png");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setName(to_string(i) + to_string(j));
    button->setTouchEnabled(true);
    button->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType te) {
            gridItemOnClick(ref, te);
        });
    _layout->addChild(button);
    return button;
}

void GridScene::gridItemOnClick(Ref* pSender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        // we only bind this method to Buttons so this cast is always safe
        Button* b = dynamic_cast<Button*>(pSender);
        if (!_selected) {
            selectAndHighlight(b);
        } else {
            auto result = _board->click(b->getName()[0] - '0',
                                        b->getName()[1] - '0');
            for (auto it = _highlighted.begin(); it != _highlighted.end();) {
                (*it)->setHighlighted(false);
                if (result && (*it)->getName() != b->getName()) {
                    _layout->removeChildByName((*it)->getName());
                }
                it = _highlighted.erase(it);
            }
            if (result) {
                refreshGrid();
                _selected = false;
            } else {
                selectAndHighlight(b);
            }
        }
    }
}

void GridScene::selectAndHighlight(Button* b) {
    auto result = _board->select(b->getName()[0] - '0',
                                 b->getName()[1] - '0');
    if (result.size() > 1) {
        _selected = true;
        for (const auto& pair : result) {
            auto button = _buttons[pair.first][pair.second];
            _highlighted.push_back(button);
            button->setHighlighted(true);
        }
    }
}

void GridScene::refreshGrid() {
    auto grid = _board->getGrid();
    auto newPositions = _board->getNewPositions();
    for (auto entry : newPositions) {
        int i = entry.first.first;
        int j = entry.first.second;
        int i2 = entry.second.first;
        int j2 = entry.second.second;
        _buttons[i2][j2] = _buttons[i][j];
        _buttons[i2][j2]->setName(to_string(i2) + to_string(j2));
        auto btnSize = _buttons[i2][j2]->getContentSize();
        float x = j2 * btnSize.width + btnSize.width / 2.0f;
        float y = (grid.size() - i2 - 1) * btnSize.height + btnSize.height / 2.0f;
        _buttons[i2][j2]->runAction(MoveTo::create(0.15f, Vec2(x, y)));
    }
    auto newValues = _board->getNewValues();
    vector<int> vals(grid.size());
    for (auto pair : newValues) {
        ++vals[pair.second];
    }
    for (auto pair : newValues) {
        int i = pair.first;
        int j = pair.second;
        auto button = createButton(i, j, grid[i][j]);
        _buttons[i][j] = button;
        auto btnSize = button->getContentSize();
        float x = j * btnSize.width + btnSize.width / 2.0f;
        float y = _layout->getContentSize().height +
            btnSize.height * (vals[j] - i) + btnSize.height / 2;
        button->setPosition(Vec2(x, y));
        auto act = MoveTo::create(0.15f,
                                  Vec2(x, (grid.size() - i - 1) *
                                          btnSize.height +
                                          btnSize.height / 2.0f));
        button->runAction(act);
    }
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid.size(); ++j) {
            _buttons[i][j]->loadTextureNormal("button_" +
                                              string(1, grid[i][j]) + ".png");
        }
    }
}

void GridScene::onGameOver() {
    auto layer = LayerColor::create(Color4B(0, 0, 0, 180));
    for (int i = 0; i < _buttons.size(); ++i) {
        for (int j = 0; j < _buttons.size(); ++j) {
            _buttons[i][j]->setEnabled(false);
        }
    }
    addChild(layer);
    auto retryButton = Button::create("button_retry.png");
    retryButton->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType te) {
            Director::getInstance()->replaceScene(GridScene::createScene());
        });
    retryButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(retryButton);
    retryButton->setPosition(_layout->getPosition());

}
