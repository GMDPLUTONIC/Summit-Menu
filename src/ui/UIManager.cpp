#include "UIManager.hpp"
#include "Geode/DefaultInclude.hpp"
#include <Loader.hpp>
#include "style/Style.hpp"
#include "MenuBall.hpp"
#include <imgui-cocos.hpp>

namespace summit::ui {

std::map<std::string, styles::Style *> styleMap;
styles::Style *currentStyle = nullptr;

bool addStyle(styles::Style *style) {
  if (styleMap.contains(style->getId()))
    return false;
  styleMap[style->getId()] = style;
  return true;
}

styles::Style *getStyle(std::string id) {
  if (!styleMap.contains(id))
    return nullptr;
  return styleMap.at(id);
}

styles::Style *getStyle() { return currentStyle; }

bool setStyle(std::string id) {
  if (!styleMap.contains(id))
    return false;
  if (currentStyle && currentStyle->getStyleType() == styles::StyleType::ImGui)
    ImGuiCocos::get().destroy();
  currentStyle = styleMap.at(id);
  if (currentStyle->getStyleType() == styles::StyleType::ImGui)
    ImGuiCocos::get().setup([] {
      currentStyle->init();
      ImGui::GetIO().IniFilename = NULL;
    }).draw([] () {
      currentStyle->update(ImGui::GetIO().DeltaTime);
    });
  summit::ui::MenuBall::get()->setCallback([] () {
    summit::ui::currentStyle->toggle();
  });
  return true;
}
}

$execute {
  summit::Loader::onLoad([](){
    // #ifdef GEODE_IS_MOBILE
    // summit::ui::setStyle("CocosUI");
    // #else
    summit::ui::setStyle("ImTabbed");
    // #endif
  }, 2);
}