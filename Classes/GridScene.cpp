#include "GridScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using std::string;
using std::to_string;
using std::vector;
using std::unique_ptr;
using ui::Button;
using ui::Layout;

Scene* GridScene::createScene()
{
    return GridScene::create();
}

bool GridScene::init() {
    if ( !Scene::init() ) {
        return false;
    }

    _board = unique_ptr<Board>(new Board());
    _scaled = vector<Button*>(0);
    _selected = false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _layout = Layout::create();
    _layout->setLayoutType(Layout::Type::RELATIVE);

    auto grid = _board->getGrid();
    auto gridSize = grid.size();
    _buttons = vector<vector<Button*>>(gridSize);
    for (size_t i = 0; i < gridSize; ++i) {
        _buttons[i] = vector<Button*>();
        for (size_t j = 0; j < gridSize; ++j) {
            auto button = createButton(i, j, grid[i][j]);
            _layout->addChild(button);
            button->setLayoutParameter(getLayoutParam(i, j));
        }
    }

    auto childSize = _buttons[0][0]->getContentSize();
    _layout->setContentSize(Size(childSize.width * grid.size(),
                                 childSize.height * grid.size()));
    _layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layout->setPosition(Vec2 (visibleSize.width / 2, visibleSize.height / 2));
    addChild(_layout);

    return true;
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

ui::LayoutParameter* GridScene::getLayoutParam(int i, int j) {
    ui::RelativeLayoutParameter* rlp = ui::RelativeLayoutParameter::create();
    rlp->setRelativeName(to_string(i) + to_string(j));
    // the top left character of the grid will be placed in the top left
    // corner of the layout
    if (i == 0 && j == 0) {
        rlp->setAlign(
            ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT
        );
        // first element of any line except line 0
    } else if (j == 0) {
        // we set it relative to the one above
        // (so element (1, 0) is relative to (0, 0))
        rlp->setRelativeToWidgetName(to_string(i - 1) +
                                     to_string(0));
        // we want it aligned below the one above
        rlp->setAlign(
            ui::RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_CENTER
        );
        // any element that is not the top left corner
        // and not on the first column
    } else {
        rlp->setRelativeToWidgetName(to_string(i) +
                                     to_string(j - 1));
        rlp->setAlign(
            ui::RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_CENTER
        );
    }
    return rlp;
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
