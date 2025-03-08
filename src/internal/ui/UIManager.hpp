#pragma once

#include <functional>
#include <list>
#include <map>
#include <string>
namespace summit::ui {

  class Widget;
  class Tab;

  class UIManager {
    protected:
      static UIManager *m_instance;

      std::map<std::string, Tab *> m_tabs = {};
      std::list<std::string> m_order = {};

    public:
      static UIManager *get();

      void registerTab(Tab *tab);
      Tab *getTab(std::string id);
      std::list<std::string> getOrder();
      std::map<std::string, Tab *> getTabs();
  };

  class Tab {
    protected:
      Tab(){};
      std::list<std::string> m_order = {};
      std::map<std::string, Widget *> m_widgets = {};
      std::string m_id;
      void init(std::string id);
    public:
      std::string getId();
      std::list<std::string> getOrder();
      std::map<std::string, Widget *> getWidgets();
      Widget *getWidget(std::string id);
      void addWidget(Widget *widget);
      void removeWidget(std::string id);

      static Tab *create(std::string id);
      
  };
  enum class WidgetSize {
    DontChange,
    Half,
    Full,
  };
  class Widget {
    public:
      struct Overrides {
        WidgetSize m_size = WidgetSize::DontChange;
      };
    protected:
      Tab *m_tab = nullptr;
      std::string m_id = "";
      std::string m_label = "";
      std::string m_desc = "";
      std::map<std::string, Widget *> m_subWidgets = {};

      virtual void init(std::string id, std::string label);
    public:
      Overrides m_overrides = Overrides();
      virtual std::string getType() {return "Label";} // it's just easier to make the base a label  
      std::string getId();
      std::string getLabel();
      std::string getDescription();
      Widget *setDescription(std::string desc);
      Widget *addSubWidget(Widget *widget);
      Widget *getSubWidget(std::string id);
      std::map<std::string, Widget *> getSubWidgets();
      Widget *setLabel(std::string label);

      static Widget *create(std::string id, std::string label);
  };

  class ToggleWidget : public Widget{
    protected:
      bool m_toggled = false;
      std::function<void (bool toggled)> m_callback = nullptr;
      virtual void init(std::string id, std::string label, bool toggled, std::function<void (bool toggled)> callback);

    public:
      virtual std::string getType() {return "Toggle";}
      ToggleWidget *setLabel(std::string label);
      bool isToggled();
      ToggleWidget *setToggled(bool value);
      std::function<void (bool toggled)> getCallback();
      ToggleWidget *setCallback(std::function<void (bool toggled)> callback);

      static ToggleWidget *create(std::string id, std::string label, bool toggled, std::function<void (bool toggled)> callback);
  };
}