#include <Geode/modify/CCDrawNode.hpp>
#include <Summit.hpp>

namespace summit::hacks::player {
  class SolidWaveTrail : public Hack {
    public:
      std::string getId() override { return "player.solidwavetrail"; }
      void init() override {
        auto t = ui::ToggleWidget::create(getId(), "Solid Wave Trail", Config::get("player.solidwavetrail", false), [](bool toggled) {
          Config::set("player.solidwavtrail", toggled);
        })->setDescription("Removes blending on the wave trail.");
        ui::UIManager::get()->getTab("Player")->addWidget(t);
      }
  };

  RegisterHack(SolidWaveTrail)

    class $modify(SWTCCDrawNode, cocos2d::CCDrawNode) {
        bool drawPolygon(cocos2d::CCPoint* p0, unsigned int p1, const cocos2d::ccColor4F& p2, float p3, const cocos2d::ccColor4F& p4) {
          if (summit::Config::get<bool>("player.solidwavetrail", false)) {
            if (p2.r == 1.F && p2.g == 1.F && p2.b == 1.F && p2.a != 1.F) return true;
            this->setBlendFunc(cocos2d::CCSprite::create()->getBlendFunc());
            this->setZOrder(-1);
            return CCDrawNode::drawPolygon(p0, p1, p2, p3, p4);
          }
        }
    };
}
