#include "../nodes/MenuBall.hpp"
#include "../nodes/CFMenuItem.hpp"
#include <Summit.hpp>
#include "Cocos.hpp"
#include <Geode/Geode.hpp>

namespace summit::ui::styles {

  CUI::LabelNode *fromWidget(Widget *widget) {
    if (widget->getType() == "Toggle") {
      auto tw = static_cast<ToggleWidget *>(widget);
      auto node = CUI::ToggleNode::create(tw);
      return node;
    }
    auto node = CUI::LabelNode::create(widget);
    return node;
  }

  void CocosUIStyle::init() {
    
  }

  void CocosUIStyle::deinit() {

  }

  std::string CocosUIStyle::getId() {
    return "CocosUI";
  }

  StyleSystem CocosUIStyle::getSystem() {
    return StyleSystem::Cocos;
  }

  void CocosUIStyle::show() {
    m_visible = true;
    MenuBall::get()->setHandlingTouch(false);
    if (auto ui = CocosUI::create(this)) {
      ui->show();
      // the funny km7 again
      ui->setZOrder(10710955 + 1);
      geode::SceneManager::get()->keepAcrossScenes(ui);
    }
  }

  void CocosUIStyle::hide() {
    m_visible = false;
    MenuBall::get()->setHandlingTouch(true);
    Style::hide();
    if (auto node = cocos2d::CCScene::get()->getChildByID("km7dev.summit_menu/cocos-ui")) {
      if (auto ui = static_cast<CocosUI *>(node)) {
        ui->onClose(ui);
      }
    }
  }

  void CocosUIStyle::hideFromPopup() {
    m_visible = false;
    MenuBall::get()->setHandlingTouch(true);
    Style::hide();
  }

  // Called for whatever system is used
  // cocos uses CCNode::update, imgui uses... whatever it has, etc
  void CocosUIStyle::update(float dt) {
    
  }

  // Popup

  bool CocosUI::setup(CocosUIStyle *style) {
    this->m_style = style;

    setID("cocos-ui"_spr);
    m_mainLayer->setID("main-layer");
    m_bgSprite->setID("popup-bg");

    auto ws = cocos2d::CCDirector::get()->getWinSize();
    m_buttonMenu->setContentSize(ws);
    m_buttonMenu->setPosition(ws / 2 - (m_mainLayer->getPosition() - m_mainLayer->getContentSize() / 2));
    m_buttonMenu->setID("button-menu");

    m_closeBtn->setPosition(m_closeBtn->getScaledContentWidth() / 2, ws.height - m_closeBtn->getScaledContentHeight() / 2);
    m_closeBtn->setID("close-button");

    // idk some hardcoded constant values ig
    float height = 240.f;
    cocos2d::CCSize tabBtnSize = {80, 20};
    cocos2d::CCSize fullHackSize = {370, 30};

    m_tabMenu = cocos2d::CCMenu::create();
    m_tabMenu->setID("tab-menu");
    m_tabMenu->setLayout(geode::AxisLayout::create(geode::Axis::Column)->setCrossAxisOverflow(false)->setAxisAlignment(geode::AxisAlignment::End)->setAxisReverse(true)->setAutoScale(false)->setGap(3.f));
    m_tabMenu->setContentSize({tabBtnSize.width, height});
    m_tabMenu->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(m_tabMenu, geode::Anchor::Left, {tabBtnSize.width / 2 + 10, 0});

    m_hackScroll = geode::ScrollLayer::create({fullHackSize.width, height});
    m_hackScroll->setID("hack-scroll");
    m_hackScroll->setContentSize({fullHackSize.width, height});
    m_hackScroll->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(m_hackScroll, geode::Anchor::Right, {-fullHackSize.width / 2 - 10, 0});

    auto btnBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    btnBg->setID("background");
    btnBg->setContentSize({fullHackSize.width, height});
    btnBg->setColor({0,0,0});
    btnBg->setOpacity(75);
    btnBg->setZOrder(-1);
    m_hackScroll->addChildAtPosition(btnBg, geode::Anchor::Center);

    // I refuse to change this name.
    auto holderBecauseScrollLayerIsSuperStupidAndIHateIt = cocos2d::CCNode::create();
    holderBecauseScrollLayerIsSuperStupidAndIHateIt->setID("dumb-holder");
    m_hackScroll->m_contentLayer->addChildAtPosition(holderBecauseScrollLayerIsSuperStupidAndIHateIt, geode::Anchor::Top);

    auto tabY = -5;

    auto uim = UIManager::get();
    for (auto id : uim->getOrder()) {
      auto tab = uim->getTab(id);
      auto btnBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
      btnBg->setID("btn-bg");
      btnBg->setScale(0.25f);
      btnBg->setContentSize(tabBtnSize * 4);
      btnBg->setColor({0,0,0});
      btnBg->setOpacity(75);
      tabY += tabBtnSize.height + 3.f;
  
      auto btnLab = cocos2d::CCLabelBMFont::create(id.c_str(), "bigFont.fnt");
      btnLab->limitLabelWidth(tabBtnSize.width - 5, .5f, .05f);
      btnLab->setID("btn-label");
      
      auto btn = CFMenuItem::create(
          btnBg, this, menu_selector(CocosUI::onTab)
      );
      btn->setID(id);
      btn->addChildAtPosition(btnLab, geode::Anchor::Center);
      btn->m_animationEnabled = false;
      btn->m_colorEnabled = true;
      btn->m_baseColor = id == m_currentTab ? cocos2d::ccColor3B({160,160,160}) : cocos2d::ccColor3B({0,0,0});
      if (auto img = static_cast<cocos2d::CCSprite *>(btn->getNormalImage())) img->setColor(btn->m_baseColor);
      btn->m_selectColor = {50,50,50};
      m_tabMenu->addChild(btn);

      cocos2d::CCMenu *menu = cocos2d::CCMenu::create();
      menu->setID(id);
      menu->setContentSize({fullHackSize.width, height});
      menu->ignoreAnchorPointForPosition(false);
      menu->setVisible(m_currentTab == id);
      m_hackMenus[id] = menu;
      
      auto btnY = 0.f;
      bool left = true;
      std::list<CUI::LabelNode *> cache = {};
      for (auto const& [wid, widget] : tab->getWidgets()) {
        auto node = fromWidget(widget);
        if (!node) continue;
        if (left) {
          if (!cache.empty()) {
            for (auto node2 : cache) {
              btnY += node2->getContentHeight();
              node2->setPosition({0, btnY});
              menu->addChild(node2);
            }
            cache.clear();
          }
          btnY += node->getContentHeight();
          node->setPosition({0, btnY});
          menu->addChild(node);
          if (node->getWidgetSize() == WidgetSize::Half) {
            left = false;
          }
        } else {
          if (node->getWidgetSize() == WidgetSize::Full) {
            cache.push_back(node);
          } else {
            node->setPosition({fullHackSize.width / 2, btnY});
            menu->addChild(node);
            left = true;
          }
        }
      }
      if (!cache.empty()) {
        for (auto node2 : cache) {
          btnY += node2->getContentHeight();
          node2->setPosition({0, btnY});
          menu->addChild(node2);
        }
        cache.clear();
      }

      auto h = std::max(height, btnY);
      menu->setContentSize({fullHackSize.width, h});
      for (auto node : geode::cocos::CCArrayExt<CUI::LabelNode *>(menu->getChildren())) {
        node->setPositionY(h - node->getPositionY());
      }

      if (m_currentTab == id) {
        m_hackScroll->m_contentLayer->setContentHeight(h);
      }
     
      holderBecauseScrollLayerIsSuperStupidAndIHateIt->addChildAtPosition(menu, geode::Anchor::Bottom, {0, -menu->getContentHeight() / 2});
      m_hackScroll->scrollToTop();
    }
    m_tabMenu->updateLayout();
    

    return true;
  }

  void CocosUI::onTab(cocos2d::CCObject *sender) {
    if (!sender) return;
    auto node = static_cast<CFMenuItem*>(sender);
    if (!node) return;
    auto old = static_cast<CFMenuItem*>(m_tabMenu->getChildByID(m_currentTab));
    if (old) {
      old->m_baseColor = {0, 0, 0};
      if (auto img = static_cast<cocos2d::CCSprite *>(old->getNormalImage())) img->setColor({0, 0, 0});
    }
    node->m_baseColor = {160, 160, 160};
    if (auto img = static_cast<cocos2d::CCSprite *>(node->getNormalImage())) img->setColor({160, 160, 160});
    if (auto menu = m_hackMenus[m_currentTab]) {
      menu->setVisible(false);
    }
    m_currentTab = node->getID();
    if (auto menu = m_hackMenus[m_currentTab]) {
      menu->setVisible(true);
      m_hackScroll->m_contentLayer->setContentHeight(menu->getContentSize().height);
    }
    m_hackScroll->scrollToTop();
  }

  void CocosUI::onClose(cocos2d::CCObject *sender) {
    if (sender != this) {
      m_style->hideFromPopup();
    }
    m_closeHandled = true;
    Popup::onClose(sender);
    geode::SceneManager::get()->forget(this);
  }
    
  CocosUI* CocosUI::create(CocosUIStyle *style) {
    if (cocos2d::CCScene::get()->getChildByID("km7dev.summit_menu/cocos-ui")) {
      geode::log::error("There's already a visible CocosUI!");
      return nullptr;
    }
    auto ret = new CocosUI();
    if (ret->initAnchored(475.f, 270.f, style, "GJ_square05.png")) {
      ret->autorelease();
      return ret;
    }

    delete ret;
    return nullptr;
  }

  namespace CUI {
    bool LabelNode::init(Widget *widget) {
      if (widget->m_overrides.m_size == WidgetSize::DontChange)
        m_size = WidgetSize::Half;
      else
        m_size = widget->m_overrides.m_size;

      if (!CCNode::init()) return false;
      this->m_widget = widget;
      setID(fmt::format("label-{}", widget->getId()));
      auto width = m_size == WidgetSize::Half ? 370.f / 2 : 370.f;
      setContentSize({width, 30});

      m_label = cocos2d::CCLabelBMFont::create(widget->getLabel().c_str(), "chatFont.fnt");
      m_label->limitLabelWidth(getContentWidth() - 10.f, 1.f, .05f);
      m_label->setID("label");
      addChildAtPosition(m_label, geode::Anchor::Center);

      return true;
    }

    WidgetSize LabelNode::getWidgetSize() {
      return m_size;
    }
    
    LabelNode *LabelNode::create(Widget *widget) {
      auto ret = new LabelNode();
      if (ret->init(widget)) {
        ret->autorelease();
        return ret;
      }
  
      delete ret;
      return nullptr;
    }

    void LabelNode::onDesc(cocos2d::CCObject *sender) {
      if (!m_widget) return geode::log::error("Failed to show description - no such widget.");
      FLAlertLayer::create(m_widget->getLabel().c_str(), m_widget->getDescription(), "OK")->show();
    }

    
    bool ToggleNode::init(Widget *widget) {
      if (widget->m_overrides.m_size == WidgetSize::DontChange)
        m_size = WidgetSize::Half;
      else
        m_size = widget->m_overrides.m_size;
      
      auto tw = static_cast<ToggleWidget *>(widget);
      if (!tw) return false;
      if (!CCNode::init()) return false;
      this->m_widget = tw;
      setID(fmt::format("toggle-{}", widget->getId()));
      auto width = m_size == WidgetSize::Half ? 370.f / 2 : 370.f;
      setContentSize({width, 30});

      m_buttonMenu = cocos2d::CCMenu::create();
      m_buttonMenu->setContentSize(getContentSize());
      m_buttonMenu->setID("button-menu");
      addChildAtPosition(m_buttonMenu, geode::Anchor::Center, {0, 0});

      float offset = 0.f;

      if (!widget->getDescription().empty()) {
        auto descSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        descSpr->setScale(.65f);
        auto descBtn = CCMenuItemSpriteExtra::create(
            descSpr, this, menu_selector(ToggleNode::onDesc)
        );
        descBtn->setID("desc-btn");
        m_buttonMenu->addChildAtPosition(descBtn, geode::Anchor::Right, {-12.625f - offset, 0});
        offset += 18.f;
      }


      // if (!widget->getSubWidgets().empty()) {
      //   auto swSpr = cocos2d::CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
      //   swSpr->setScale(.45f);
      //   auto swBtn = CCMenuItemSpriteExtra::create(
      //       swSpr, this, menu_selector(ToggleNode::onDesc)
      //   );
      //   swBtn->setID("sub-btn");
      //   m_buttonMenu->addChildAtPosition(swBtn, geode::Anchor::Right, {-12.625f - offset, 0});
      //   offset += 18.f;
      // }

      m_toggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ToggleNode::onToggle), 1.f);
      m_toggle->setID("toggle");
      m_toggle->toggle(tw->isToggled());
      // Built in scale in create doesnt scale hitbox 💔
      m_toggle->setScale(.75f);
      m_buttonMenu->addChildAtPosition(m_toggle, geode::Anchor::Right, {-17 - offset, 0});

      m_label = cocos2d::CCLabelBMFont::create(widget->getLabel().c_str(), "chatFont.fnt");
      m_label->limitLabelWidth(getContentWidth() - 35.f - offset, 1.f, .05f);
      m_label->setID("label");
      m_label->setAnchorPoint({0.f, .5f});
      addChildAtPosition(m_label, geode::Anchor::Left, {3.f, 0.f});

      return true;
    }

    void ToggleNode::onToggle(cocos2d::CCObject *sender) {
      bool toggled = !m_widget->isToggled();
      m_widget->setToggled(toggled);
      if (auto cb = m_widget->getCallback()) {
        cb(toggled);
      }
    }

    void ToggleNode::onDesc(cocos2d::CCObject *sender) {
      if (!m_widget) return geode::log::error("Failed to show description - no such widget.");
      FLAlertLayer::create(m_widget->getLabel().c_str(), m_widget->getDescription(), "OK")->show();
    }
    
    ToggleNode *ToggleNode::create(Widget *widget) {
      auto ret = new ToggleNode();
      if (ret->init(widget)) {
        ret->autorelease();
        return ret;
      }
  
      delete ret;
      return nullptr;
    }
  }

  RegisterStyle(CocosUIStyle)
}