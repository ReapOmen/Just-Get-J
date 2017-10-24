#ifndef __GRID_SCENE_H__
#define __GRID_SCENE_H__

#define CC_USE_PHYSICS 1

#include <memory>

#include "game/Board.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GridScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(GridScene);

private:
    std::unique_ptr<Board> _board;
    cocos2d::ui::Layout* _layout;
    std::vector<std::vector<cocos2d::ui::Button*>> _buttons;
    std::vector<cocos2d::ui::Button*> _highlighted;
    bool _selected;

    void setupLayout();
    void createGridOfButtons();
    cocos2d::ui::Button* createButton(int i, int j, char c);
    void gridItemOnClick(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void selectAndHighlight(cocos2d::ui::Button* button);
    void refreshGrid();
    bool onContactBegin(const cocos2d::PhysicsContact& contact);
};

#endif // __GRID_SCENE_H__
