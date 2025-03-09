#include "KeybindManager.hpp"
#include "Geode/loader/Log.hpp"
#include <Geode/modify/CCKeyboardDispatcher.hpp>

#ifndef GEODE_IS_IOS

namespace summit::keybinds {
  void Keybind::init(std::string id, std::string name, std::function<bool ()> callback, Keys key, KeyStates state, std::list<Modifiers> modifiers) {
    m_id = id;
    m_name = name;
    m_callback = callback;
    m_key = key;
    m_state = state;
    m_modifiers = modifiers;
  }

  Keybind *Keybind::create(std::string id, std::string name, std::function<bool ()> callback, Keys key, KeyStates state, std::list<Modifiers> modifiers) {
    auto ret = new Keybind();
    ret->init(id, name, callback, key, state, modifiers);
    return ret;
  }


  KeybindManager *KeybindManager::m_instance = nullptr;

  KeybindManager *KeybindManager::get() {
    if (!m_instance) m_instance = new KeybindManager;
    return m_instance;
  }
      
  void KeybindManager::registerKeybind(Keybind *bind) {
    if (m_keybinds.contains(bind->m_id)) return geode::log::error("(KeybindManager) {} is already registered!", bind->m_id);
    m_keybinds[bind->m_id] = bind;
  }
  void KeybindManager::removeKeybind(std::string id) {
    if (!m_keybinds.contains(id)) return geode::log::error("(KeybindManager) {} is not registered!", id);
    m_keybinds.erase(id);
  }
  Keybind *KeybindManager::getKeybind(std::string id) {
    return m_keybinds[id];
  }
  std::map<std::string, Keybind *> KeybindManager::getKeybinds() {
    return m_keybinds;
  }



  bool KeybindManager::checkBinds(Keys key, KeyStates state, int modifiers) {
    for (auto& bind : KeybindManager::get()->getKeybinds()) {
      int bindModifiers = 0;
      for (auto& mod : bind.second->m_modifiers) {
        bindModifiers += (int) mod;
      }
      if (bind.second->m_key == key && bind.second->m_state == state && bindModifiers == modifiers) {
        if (bind.second->m_callback()) return true;
      }
    }
    return false;
  }

  #if !defined(GEODE_IS_WINDOWS) && !defined(GEODE_IS_MACOS)
    // this is ugly and boring so macro
    #define ckrk(cocos, mine) case cocos2d::enumKeyCodes::cocos: return Keys::mine;
    Keys cocosToKey(cocos2d::enumKeyCodes key) {
      switch(key) {
        default: 
          return Keys::Unknown;
        ckrk(KEY_Space, Space)
        // ckrk(KEY_Apostrophe, Apostrophe)
        ckrk(KEY_OEMComma, Comma)
        ckrk(KEY_OEMMinus, Minus)
        ckrk(KEY_OEMPeriod, Period)
        // ckrk(KEY_Slash, Slash)
        ckrk(KEY_Zero, Num0)
        ckrk(KEY_One, Num1)
        ckrk(KEY_Two, Num2)
        ckrk(KEY_Three, Num3)
        ckrk(KEY_Four, Num4)
        ckrk(KEY_Five, Num5)
        ckrk(KEY_Six, Num6)
        ckrk(KEY_Seven, Num7)
        ckrk(KEY_Eight, Num8)
        ckrk(KEY_Nine, Num9)
        // ckrk(KEY_Semicolon, Semicolon)
        // ckrk(KEY_OEMEqual, Equal)
        ckrk(KEY_A, A)
        ckrk(KEY_B, B)
        ckrk(KEY_C, C)
        ckrk(KEY_D, D)
        ckrk(KEY_E, E)
        ckrk(KEY_F, F)
        ckrk(KEY_G, G)
        ckrk(KEY_H, H)
        ckrk(KEY_I, I)
        ckrk(KEY_J, J)
        ckrk(KEY_K, K)
        ckrk(KEY_L, L)
        ckrk(KEY_M, M)
        ckrk(KEY_N, N)
        ckrk(KEY_O, O)
        ckrk(KEY_P, P)
        ckrk(KEY_Q, Q)
        ckrk(KEY_R, R)
        ckrk(KEY_S, S)
        ckrk(KEY_T, T)
        ckrk(KEY_U, U)
        ckrk(KEY_V, V)
        ckrk(KEY_W, W)
        ckrk(KEY_X, X)
        ckrk(KEY_Y, Y)
        ckrk(KEY_Z, Z)
        // ckrk(KEY_LeftBracket, LeftBracket)
        // ckrk(KEY_Backslash, Backslash)
        // ckrk(KEY_RightBracket, RightBracket)
        // ckrk(KEY_Grave, GraveAccent)
        // ckrk(KEY_World1, World1) // What even is World1, I copied from the GL thing
        // ckrk(KEY_World2, World2)
        ckrk(KEY_Escape, Escape)
        ckrk(KEY_Enter, Enter)
        ckrk(KEY_Tab, Tab)
        ckrk(KEY_Backspace, Backspace)
        ckrk(KEY_Insert, Insert)
        ckrk(KEY_Delete, Delete)
        ckrk(KEY_Right, Right)
        ckrk(KEY_Left, Left)
        ckrk(KEY_Down, Down)
        ckrk(KEY_Up, Up)
        ckrk(KEY_PageUp, PageUp)
        ckrk(KEY_PageDown, PageDown)
        ckrk(KEY_Home, Home)
        ckrk(KEY_End, End)
        ckrk(KEY_CapsLock, CapsLock)
        ckrk(KEY_ScrollLock, ScrollLock)
        ckrk(KEY_Numlock, NumLock)
        ckrk(KEY_PrintScreen, PrintScreen)
        ckrk(KEY_Pause, Pause)
        ckrk(KEY_F1, F1)
        ckrk(KEY_F2, F2)
        ckrk(KEY_F3, F3)
        ckrk(KEY_F4, F4)
        ckrk(KEY_F5, F5)
        ckrk(KEY_F6, F6)
        ckrk(KEY_F7, F7)
        ckrk(KEY_F8, F8)
        ckrk(KEY_F9, F9)
        ckrk(KEY_F10, F10)
        ckrk(KEY_F11, F11)
        ckrk(KEY_F12, F12)
        ckrk(KEY_F13, F13)
        ckrk(KEY_F14, F14)
        ckrk(KEY_F15, F15)
        ckrk(KEY_F16, F16)
        ckrk(KEY_F17, F17)
        ckrk(KEY_F18, F18)
        ckrk(KEY_F19, F19)
        ckrk(KEY_F20, F20)
        ckrk(KEY_F21, F21)
        ckrk(KEY_F22, F22)
        ckrk(KEY_F23, F23)
        ckrk(KEY_F24, F24)
        // ckrk(KEY_F25, F25) // sorry f25 nerds
        ckrk(KEY_NumPad0, Num0)
        ckrk(KEY_NumPad1, Num1)
        ckrk(KEY_NumPad2, Num2)
        ckrk(KEY_NumPad3, Num3)
        ckrk(KEY_NumPad4, Num4)
        ckrk(KEY_NumPad5, Num5)
        ckrk(KEY_NumPad6, Num6)
        ckrk(KEY_NumPad7, Num7)
        ckrk(KEY_NumPad8, Num8)
        ckrk(KEY_NumPad9, Num9)
        ckrk(KEY_Decimal, Decimal)
        ckrk(KEY_Divide, Divide)
        ckrk(KEY_Multiply, Multiply)
        ckrk(KEY_Subtract, Subtract)
        ckrk(KEY_Add, Add)
        // ckrk(KEY_NumEnter, NumEnter)
        // ckrk(KEY_Equal, NumEqual)
        ckrk(KEY_LeftShift, LeftShift)
        ckrk(KEY_LeftControl, LeftControl)
        // ckrk(KEY_LeftAlt, LeftAlt)
        ckrk(KEY_LeftWindowsKey, LeftSuper)
        ckrk(KEY_RightShift, RightShift)
        ckrk(KEY_RightContol, RightControl)
        // ckrk(KEY_RightAlt, RightAlt)
        ckrk(KEY_RightWindowsKey, RightSuper)
        ckrk(KEY_LeftMenu, Menu)
        ckrk(KEY_RightMenu, Menu)
      }
    }
    class $modify (KeybindCCKBD, cocos2d::CCKeyboardDispatcher) {
      bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
        int mods = 
          (m_bCommandPressed ? 8 : 0)
        + (m_bAltPressed ? 4 : 0)
        + (m_bControlPressed ? 2 : 0)
        + (m_bCommandPressed ? 1 : 0);
        if(KeybindManager::get()->checkBinds(cocosToKey(key), repeat ? KeyStates::Hold : down ? KeyStates::Press : KeyStates::Release, mods)) return true;
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
      }
    };
  #endif

  std::string nameForKey(Keys key) {
    switch(key) {
      default: return "Unknown";
      case Keys::Space: return "Space";
      case Keys::Apostrophe: return "Apostrophe";
      case Keys::Comma: return "Comma";
      case Keys::Minus: return "Minus";
      case Keys::Period: return "Period";
      case Keys::Slash: return "Slash";
      case Keys::Key0: return "Num0";
      case Keys::Key1: return "Num1";
      case Keys::Key2: return "Num2";
      case Keys::Key3: return "Num3";
      case Keys::Key4: return "Num4";
      case Keys::Key5: return "Num5";
      case Keys::Key6: return "Num6";
      case Keys::Key7: return "Num7";
      case Keys::Key8: return "Num8";
      case Keys::Key9: return "Num9";
      case Keys::Semicolon: return "Semicolon";
      case Keys::Equal: return "Equal";
      case Keys::A: return "A";
      case Keys::B: return "B";
      case Keys::C: return "C";
      case Keys::D: return "D";
      case Keys::E: return "E";
      case Keys::F: return "F";
      case Keys::G: return "G";
      case Keys::H: return "H";
      case Keys::I: return "I";
      case Keys::J: return "J";
      case Keys::K: return "K";
      case Keys::L: return "L";
      case Keys::M: return "M";
      case Keys::N: return "N";
      case Keys::O: return "O";
      case Keys::P: return "P";
      case Keys::Q: return "Q";
      case Keys::R: return "R";
      case Keys::S: return "S";
      case Keys::T: return "T";
      case Keys::U: return "U";
      case Keys::V: return "V";
      case Keys::W: return "W";
      case Keys::X: return "X";
      case Keys::Y: return "Y";
      case Keys::Z: return "Z";
      case Keys::LeftBracket: return "LeftBracket";
      case Keys::Backslash: return "Backslash";
      case Keys::RightBracket: return "RightBracket";
      case Keys::GraveAccent: return "GraveAccent";
      case Keys::World1: return "World1";
      case Keys::World2: return "World2";
      case Keys::Escape: return "Escape";
      case Keys::Enter: return "Enter";
      case Keys::Tab: return "Tab";
      case Keys::Backspace: return "Backspace";
      case Keys::Insert: return "Insert";
      case Keys::Delete: return "Delete";
      case Keys::Right: return "Right";
      case Keys::Left: return "Left";
      case Keys::Down: return "Down";
      case Keys::Up: return "Up";
      case Keys::PageUp: return "PageUp";
      case Keys::PageDown: return "PageDown";
      case Keys::Home: return "Home";
      case Keys::End: return "End";
      case Keys::CapsLock: return "CapsLock";
      case Keys::ScrollLock: return "ScrollLock";
      case Keys::NumLock: return "NumLock";
      case Keys::PrintScreen: return "PrintScreen";
      case Keys::Pause: return "Pause";
      case Keys::F1: return "F1";
      case Keys::F2: return "F2";
      case Keys::F3: return "F3";
      case Keys::F4: return "F4";
      case Keys::F5: return "F5";
      case Keys::F6: return "F6";
      case Keys::F7: return "F7";
      case Keys::F8: return "F8";
      case Keys::F9: return "F9";
      case Keys::F10: return "F10";
      case Keys::F11: return "F11";
      case Keys::F12: return "F12";
      case Keys::F13: return "F13";
      case Keys::F14: return "F14";
      case Keys::F15: return "F15";
      case Keys::F16: return "F16";
      case Keys::F17: return "F17";
      case Keys::F18: return "F18";
      case Keys::F19: return "F19";
      case Keys::F20: return "F20";
      case Keys::F21: return "F21";
      case Keys::F22: return "F22";
      case Keys::F23: return "F23";
      case Keys::F24: return "F24";
      case Keys::F25: return "F25";
      case Keys::Num0: return "Num0";
      case Keys::Num1: return "Num1";
      case Keys::Num2: return "Num2";
      case Keys::Num3: return "Num3";
      case Keys::Num4: return "Num4";
      case Keys::Num5: return "Num5";
      case Keys::Num6: return "Num6";
      case Keys::Num7: return "Num7";
      case Keys::Num8: return "Num8";
      case Keys::Num9: return "Num9";
      case Keys::Decimal: return "Decimal";
      case Keys::Divide: return "Divide";
      case Keys::Multiply: return "Multiply";
      case Keys::Subtract: return "Subtract";
      case Keys::Add: return "Add";
      case Keys::NumEnter: return "NumEnter";
      case Keys::NumEqual: return "NumEqual";
      case Keys::LeftShift: return "LeftShift";
      case Keys::LeftControl: return "LeftControl";
      case Keys::LeftAlt: return "LeftAlt";
      case Keys::LeftSuper: return "LeftSuper";
      case Keys::RightShift: return "RightShift";
      case Keys::RightControl: return "RightControl";
      case Keys::RightAlt: return "RightAlt";
      case Keys::RightSuper: return "RightSuper";
      case Keys::Menu: return "Menu";
    }
  }
}

#endif