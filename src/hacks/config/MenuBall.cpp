#include "../ui/nodes/MenuBall.hpp"
#include <Summit.hpp>

#ifdef GEODE_IS_DESKTOP
namespace summit::hacks::config {
  class ShowBall : public Hack {
    public:
      std::string getId() override { return "config.showball"; }
      void init() override {
        auto toggle = ui::ToggleWidget::create(getId(), "Show Menu Ball", Config::get("config.showball", false), [](bool toggled) {
          Config::set("config.showball", toggled);
          auto ball = ui::MenuBall::get();
          if (toggled) {
            ball->setRendered(true);
          } else {
            ball->setRendered(false);
          }
        });
        ui::UIManager::get()->getTab("Config")->addWidget(toggle);
      }
  };

  RegisterHack(ShowBall)

  $onLoad(MenuLayer, 1) {
    auto ball = ui::MenuBall::get();
    if (Config::get("config.showball", false)) {
      ball->setRendered(true);
    } else {
      ball->setRendered(false);
    }
  }
}
#endif