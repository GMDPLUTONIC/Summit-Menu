#include "UIManager.hpp"
#include "Geode/loader/Log.hpp"
#include <utils/LoadManager.hpp>

namespace summit::ui {
  UIManager *UIManager::m_instance = nullptr;

  UIManager *UIManager::get() {
    if (!m_instance) m_instance = new UIManager;
    return m_instance;
  }

  void UIManager::registerTab(Tab *tab) {
    if (!tab) geode::log::error("(UIManager) No tab provided.");
    auto id = tab->getId();
    if (m_tabs.contains(id)) return geode::log::error("(UIManager) Tab {} already registered.", id);
    m_tabs[id] = tab;
    m_order.push_back(id);
  }
  
  Tab *UIManager::getTab(std::string id) {
    return m_tabs[id];
  }
  
  std::map<std::string, Tab *> UIManager::getTabs() {
    return m_tabs;
  }

  std::list<std::string> UIManager::getOrder() {
    return m_order;
  }

  // Tab

  void Tab::init(std::string id) {
    this->m_id = id;
  }

  std::string Tab::getId() {
    return m_id;
  }

  Tab *Tab::create(std::string id) {
    auto ret = new Tab;
    ret->init(id);
    return ret;
  }

  std::map<std::string, Widget *> Tab::getWidgets() {
    return m_widgets;
  }

  Widget *Tab::getWidget(std::string id) {
    return m_widgets[id];
  }

  void Tab::addWidget(Widget *widget) {
    if (!widget) geode::log::error("(Tab {}) No widget provided.", m_id);
    auto id = widget->getId();
    if (m_widgets.contains(id)) return geode::log::error("(Tab {}) Widget {} already registered.", m_id, id);
    m_order.push_back(id);
    m_widgets[id] = widget;
  }

  void Tab::removeWidget(std::string id) {
    if (!m_widgets.contains(id)) return geode::log::error("(Tab {}) Widget {} not registered.", m_id, id);
    m_widgets.erase(id);
    m_order.remove(id);
  }

  std::list<std::string> Tab::getOrder() {
    return m_order;
  }

  // Widget

  void Widget::init(std::string id, std::string label) {
    this->m_id = id;
    this->m_label = label;
  }

  std::string Widget::getId() {
    return m_id;
  }

  std::string Widget::getLabel  () {
    return m_label;
  }
  
  Widget *Widget::setLabel(std::string label) {
    this->m_label = label;
    return this;
  }

  Widget *Widget::create(std::string id, std::string label) {
    auto ret = new Widget;
    ret->init(id, label);
    return ret;
  }


  std::string Widget::getDescription() {
    return m_desc;
  }

  Widget *Widget::setDescription(std::string desc) {
    m_desc = desc;
    return this;
  }

  // Widget *Widget::addSubWidget(Widget *widget) {
  //   if (!widget) geode::log::error("(Widget {}) No subwidget provided.", m_id);
  //   auto id = widget->getId();
  //   if (m_subWidgets.contains(id)) {
  //     geode::log::error("(Widget {}) Subwidget {} already added.", m_id, id);
  //     return this;
  //   }
  //   m_subOrder.push_back(id);
  //   m_subWidgets[id] = widget;
  //   return this;
  // }

  // Widget *Widget::getSubWidget(std::string id) {
  //   return m_subWidgets[id];
  // }

  // std::map<std::string, Widget *> Widget::getSubWidgets() {
  //   return m_subWidgets;
  // }

  // std::vector<std::string> Widget::getSubOrder() {
  //   return m_subOrder;
  // }


  // Toggle Widget

  void ToggleWidget::init(std::string id, std::string label, bool toggled, std::function<void (bool toggled)> callback) {
    Widget::init(id, label);
    this->m_toggled = toggled;
    this->m_callback = callback;
  }

  ToggleWidget *ToggleWidget::setLabel(std::string label) {
    m_label = label;
    return this;
  }
  bool ToggleWidget::isToggled() {
    return m_toggled;
  }
  ToggleWidget *ToggleWidget::setToggled(bool value) {
    this->m_toggled = value;
    return this;
  }

  ToggleWidget *ToggleWidget::create(std::string id, std::string label, bool toggled, std::function<void (bool)> callback) {
    auto ret = new ToggleWidget;
    ret->init(id, label, toggled, callback);
    return ret;
  }

  std::function<void (bool)> ToggleWidget::getCallback() {
    return m_callback;
  }

  ToggleWidget *ToggleWidget::setCallback(std::function<void (bool)> callback) {
    this->m_callback = callback;
    return this;
  }

  ToggleWidget *ToggleWidget::setDescription(std::string desc) {
    m_desc = desc;
    return this;
  }

  // ToggleWidget *ToggleWidget::addSubWidget(Widget *widget) {
  //   if (!widget) geode::log::error("(ToggleWidget {}) No subwidget provided.", m_id);
  //   auto id = widget->getId();
  //   if (m_subWidgets.contains(id)) {
  //     geode::log::error("(ToggleWidget {}) Subwidget {} already added.", m_id, id);
  //     return this;
  //   }
  //   m_subOrder.push_back(id);
  //   m_subWidgets[id] = widget;
  //   return this;
  // }


  $onLoad(Initial, 0)  {
    UIManager::get()->registerTab(Tab::create("Global"));
    UIManager::get()->registerTab(Tab::create("Player"));
    UIManager::get()->registerTab(Tab::create("Config"));
  }
}