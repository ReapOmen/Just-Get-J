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

    initWithPhysics();
    _board = unique_ptr<Board>(new Board());
    _scaled = vector<Button*>(0);
    _selected = false;

    setupLayout();
    createGridOfButtons();

    // set up the content size of the layout
    auto buttonSize = _buttons[0][0]->getContentSize();
    auto layoutSize = Size(buttonSize.width * _buttons.size(),
                           buttonSize.height * _buttons.size());
    _layout->setContentSize(layoutSize);

    // set up an edge at the bottom of the layout
    // to stop buttons from falling
    auto layoutPB = PhysicsBody::createEdgeSegment(
        Vec2(-layoutSize.width / 2.0f, -layoutSize.height / 2.0f),
        Vec2(layoutSize.width / 2.0f, -layoutSize.height / 2.0f)
    );
    layoutPB->setDynamic(false);
    _layout->setPhysicsBody(layoutPB);

    addChild(_layout);

    return true;
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
            auto btnSize = button->getContentSize();
            button->setPhysicsBody(PhysicsBody::createBox(btnSize));
            _layout->addChild(button);
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
    _buttons[i].push_back(button);
    return button;
}

void GridScene::gridItemOnClick(Ref* pSender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        // we only bind this method to Buttons so this cast is always safe
        Button* b = dynamic_cast<Button*>(pSender);
        if (!_selected) {
            selectAndScale(b);
        } else {
            auto result = _board->click(b->getName()[0] - '0',
                                        b->getName()[1] - '0');
            for (auto it = _scaled.begin(); it != _scaled.end();) {
                (*it)->setScale(1.0f);
                it = _scaled.erase(it);
            }
            if (result) {
                refreshGrid();
                _selected = false;
            } else {
                selectAndScale(b);
            }
        }
    }
}

void GridScene::selectAndScale(Button* b) {
    auto result = _board->select(b->getName()[0] - '0',
                                 b->getName()[1] - '0');
    if (result.size() > 1) {
        _selected = true;
        for (const auto& pair : result) {
            auto button = _buttons[pair.first][pair.second];
            _scaled.push_back(button);
            button->setScale(1.1f);
        }
    }
}

void GridScene::refreshGrid() {
    auto grid = _board->getGrid();
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid.size(); ++j) {
            _buttons[i][j]->loadTextureNormal("button_" +
                                              string(1, grid[i][j]) + ".png");
        }
    }
}
