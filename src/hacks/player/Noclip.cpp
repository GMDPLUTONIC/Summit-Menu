#include "Geode/binding/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Summit.hpp>

namespace summit::hacks::player {
  class Noclip : public Hack {
    public:
      std::string getId() override { return "player.noclip"; }
      void init() override {
        auto t = ui::ToggleWidget::create(getId(), "Noclip", Config::get("player.noclip", false), [](bool toggled) {
          Config::set("player.noclip", toggled);
        })->setDescription("Prevents dying in levels.");
        // auto p1 = ui::ToggleWidget::create("player.noclip.p1", "Player 1 Noclip", Config::get("player.noclip.p1", true), [](bool toggled) {
        //   Config::set("player.noclip.p1", toggled);
        // })->setDescription("If player 1 should be noclipping.");
        // t->addSubWidget(p1);
        // auto p2 = ui::ToggleWidget::create("player.noclip.p2", "Player 2 Noclip", Config::get("player.noclip.p2", true), [](bool toggled) {
        //   Config::set("player.noclip.p2", toggled);
        // })->setDescription("If player 2 should be noclipping.");
        // t->addSubWidget(p2);
        ui::UIManager::get()->getTab("Player")->addWidget(t);
      }
  };

  RegisterHack(Noclip)

  // class AutoSafeMode : Hack {
  //   public:
  //     std::string getTab() override { return "global"; }
  //     std::string getId() override { return "global.autosafemode"; }
  //     void init() override {
  //       ui::UIManager::registerWidget(getId(), ui::widgets::ToggleWidget::create(getId(), "Auto Safe Mode", false)->setDescription("Prevents you from beating any levels if you have any cheats enabled."));
  //     }
  // };

  class $modify(NCPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject *p, GameObject *g) {
      if (summit::Config::get<bool>("player.noclip", false) && g != m_anticheatSpike) {
        if (p == m_player1 && summit::Config::get<bool>("player.noclip.p1", true)) {
          return;
        }
        if (p == m_player1 && summit::Config::get<bool>("player.noclip.p2", true)) {
          return;
        }
      }
      PlayLayer::destroyPlayer(p,g);
    }
  };
}