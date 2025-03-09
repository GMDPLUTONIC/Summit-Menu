#include "MenuBall.hpp"
#include "ui/Style.hpp"
#include <Summit.hpp>
#include <Geode/Geode.hpp>

namespace summit::ui {


  MenuBall *MenuBall::m_instance = nullptr;

  bool MenuBall::init() {
    if (!CCMenu::init())
      return false;
    // scheduleUpdate();
    m_sprite = cocos2d::CCSprite::createWithSpriteFrameName("summitBtn.png"_spr);
    m_sprite->setScale(.4f);
    m_sprite->setID("sprite");
    addChild(m_sprite);
    setContentSize(m_sprite->getScaledContentSize());
    m_sprite->setPosition(getContentSize() / 2);

    cocos2d::CCScene::get()->addChild(this);
    geode::SceneManager::get()->keepAcrossScenes(this);

    // 107 k
    // 109 m
    // 55  7
    setZOrder(10710955);
    auto x = summit::Config::get<float>("config.ball-x", 50.f);
    auto y = summit::Config::get<float>("config.ball-y", 50.f);
    m_scale = std::clamp(summit::Config::get<float>("config.ball-scale", 1.f), 0.25f, 3.f);
    x = std::clamp(x, -getContentWidth() / 2,
                  cocos2d::CCDirector::get()->getWinSize().width -
                      getContentWidth() / 2);
    y = std::clamp(y, -getContentHeight() / 2,
                  cocos2d::CCDirector::get()->getWinSize().height -
                      getContentHeight() / 2);
    setPosition({x, y});

    summit::Config::set<float>("config.ball-x", x);
    summit::Config::set<float>("config.ball-y", y);
    summit::Config::set<float>("config.ball-scale", m_scale);

    setID("summit-button"_spr);

    return true;
  };

  MenuBall *MenuBall::get(bool createIfNone) {
    if (m_instance)
      return m_instance;
    if (!createIfNone) return nullptr;
    m_instance = new MenuBall();
    if (m_instance && m_instance->init()) {
      m_instance->autorelease();
      return m_instance;
    } else {
      delete m_instance;
      return nullptr;
    }
  }

  void MenuBall::registerWithTouchDispatcher() {
    cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -10710955, true);
  }

  bool MenuBall::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *evt) {
    if (!m_handleTouch || !m_render)
      return false;
    if (getScaledContentSize().width / 2 <
        ccpDistance(m_sprite->getPosition(),
                    convertToNodeSpace(touch->getLocation()))) {
      return false;
    }

    m_diff = getPosition() - touch->getLocation();;
    m_startPos = new cocos2d::CCPoint(touch->getLocation());

    m_moving = false;

    stopAllActions();
    #ifdef GEODE_IS_IOS
    runAction(cocos2d::CCEaseOut::create(
        cocos2d::CCScaleTo::create(0.3f, m_scale * m_multiplier), 1.6f));
    #else
    runAction(cocos2d::CCEaseSineOut::create(
        cocos2d::CCScaleTo::create(0.3f, m_scale * m_multiplier)));
    #endif
    return true;
  }

  void MenuBall::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *evt) {
    stopAllActions();
    #ifdef GEODE_IS_IOS
    runAction(cocos2d::CCEaseOut::create(
        cocos2d::CCScaleTo::create(0.3f, m_scale), 1.6f));
    #else
    runAction(cocos2d::CCEaseSineOut::create(
        cocos2d::CCScaleTo::create(0.3f, m_scale)));
    #endif
    if (m_moving) {
      summit::Config::set<float>("config.ball-x", getPositionX());
      summit::Config::set<float>("config.ball-y", getPositionY());
      summit::Config::save();
      return;
    }
    if (!m_callback) {
      geode::log::error("Missing callback for menu ball!");
      return;
    }
    m_callback();
  }

  void MenuBall::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *evt) {
    if (!m_moving)
      if (ccpDistance(*m_startPos, touch->getLocation()) > 3)
        m_moving = true;
    if (m_moving) {
      auto pos = touch->getLocation() + m_diff;
      pos.x = std::clamp(pos.x, -getContentWidth() / 2,
                        cocos2d::CCDirector::get()->getWinSize().width -
                            getContentWidth() / 2);
      pos.y = std::clamp(pos.y, -getContentHeight() / 2,
                        cocos2d::CCDirector::get()->getWinSize().height -
                            getContentHeight() / 2);
      setPosition(pos);
    }
  }

  // void MenuBall::update(float dt) {
    // setVisible(isRendered());
  // }

  std::function<void()> MenuBall::getCallback() { return m_callback; }
  void MenuBall::setCallback(std::function<void()> callback) {
    m_callback = callback;
  }

  bool MenuBall::isRendered() {
    // if (!summit::Config::get<bool>("config.showball", false)) return false;
    return m_render;
  }

  void MenuBall::setRendered(bool render) {
    m_render = render;
    setVisible(render);
  }

  bool MenuBall::isHandlingTouch() {
    // if (!summit::Config::get<bool>("config.showball", false)) return false;
    return m_render && m_handleTouch;
  }

  void MenuBall::setHandlingTouch(bool handle) { m_handleTouch = handle; }
}

$onLoad(MenuLayer, 0) {
  auto ball = summit::ui::MenuBall::get(true);
  if(!ball) return;
  ball->setCallback([]() {
    if (auto style = summit::ui::StyleManager::get()->getCurrentStyle())
      style->toggle();
  });

  #ifndef GEODE_IS_IOS
  auto bind = summit::keybinds::Keybind::create("openmenu", "Open Menu", [ball](){
      ball->getCallback()();
      return true;
    }, summit::keybinds::Keys::Tab);
    summit::keybinds::KeybindManager::get()->registerKeybind(bind);
  #endif
}

#include <Geode/modify/CCScene.hpp>

class $modify(cocos2d::CCScene) {
  int getHighestChildZ() {
    int btnZ;
    auto btn = summit::ui::MenuBall::get();
    if (btn) {
      btnZ = btn->getZOrder();
      btn->setZOrder(-1);
    }
    auto highest = CCScene::getHighestChildZ();
    if (btn)
      btn->setZOrder(btnZ);
    return highest;
  }
};
