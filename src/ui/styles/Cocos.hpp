#pragma once

#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/ui/Popup.hpp"
#include "Geode/ui/ScrollLayer.hpp"
#include <ui/UIManager.hpp>
#include "ui/Style.hpp"
#include <map>

namespace summit::ui::styles {

  class CocosUIStyle : public Style {
    public:
      virtual void init() override;
      virtual void deinit() override;

      virtual std::string getId() override;
      virtual StyleSystem getSystem() override;

      virtual void show() override;
      virtual void hide() override;

      void hideFromPopup();

      void update(float dt) override;
  };

  class CocosUI : public geode::Popup<CocosUIStyle *> {
    protected:
      bool m_closeHandled = false;
      bool setup(CocosUIStyle *style) override;
      CocosUIStyle *m_style = nullptr;
      std::string m_currentTab = "Global";
      cocos2d::CCMenu *m_tabMenu;
      geode::ScrollLayer *m_hackScroll;
      std::map<std::string, cocos2d::CCMenu *> m_hackMenus = {};
    public:
      void onTab(CCObject *sender);
      static CocosUI* create(CocosUIStyle *style);
      void onClose(CCObject *) override;
  };

  namespace CUI {
    class LabelNode : public cocos2d::CCNode {
      protected:
        cocos2d::CCMenu *m_buttonMenu = nullptr;
        cocos2d::CCLabelBMFont *m_label = nullptr;
        Widget *m_widget = nullptr;
        WidgetSize m_size;
        virtual bool init(Widget *widget);
      public:
        static LabelNode *create(Widget *widget);
        WidgetSize getWidgetSize();

        void onDesc(CCObject *sender);
        // void onSubWidgets(CCObject *sender);
    };
    class ToggleNode : public LabelNode {
      protected:
        CCMenuItemToggler *m_toggle = nullptr;
        ToggleWidget *m_widget = nullptr;
        bool init(Widget *widget) override;
      public:
        static ToggleNode *create(Widget *widget);

        void onToggle(cocos2d::CCObject *sender);
        void onDesc(CCObject *sender);
        // void onSubWidgets(CCObject *sender);
    };
  }
}