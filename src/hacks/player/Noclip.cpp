#include "Config.hpp"
#include "Geode/binding/GameStatsManager.hpp"
#include "Geode/binding/PlayLayer.hpp"
#include "ui/UIManager.hpp"
#include "ui/widgets/ToggleWidget.hpp"
#include <hacks/Hack.hpp>
#include <LoadManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

namespace summit::hack::global {
  class Noclip : Hack {
    public:
      std::string getTab() override { return "player"; }
      std::string getId() override { return "player.noclip"; }
      void init() override {
        geode::log::info("Registering noclip: {}", ui::UIManager::registerWidget("Player", ui::widgets::ToggleWidget::create(getId(), "Noclip", Config::get<bool>("player.noclip", false))->setDescription("Prevents you from dying.")->setCallback([](bool toggled) {
          Config::set<bool>("player.noclip", toggled);
        })));
      }
  };

  $execute {
    LoadManager::onLoad([](){
      (new Noclip)->init();
    },1,LoadTime::Early);
  }
}
