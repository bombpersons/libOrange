#if defined __linux__

#include <Orange/priv/linux/LinuxInputImpl.hpp>

#include <glm/glm.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      LinuxInputImpl::LinuxInputImpl(WindowImpl* _impl) {

      }
      LinuxInputImpl::~LinuxInputImpl() {

      }

      // Keyboard
      bool LinuxInputImpl::IsKeyDown(InputCode::Key::Key _key) {
        return false;
      }
      bool LinuxInputImpl::IsKeyPressed(InputCode::Key::Key _key) {
        return false;
      }
      bool LinuxInputImpl::IsKeyReleased(InputCode::Key::Key _key) {
        return false;
      }

      // Mouse
      glm::vec2 LinuxInputImpl::GetMousePos() {
        return glm::vec2();
      }
      glm::vec2 LinuxInputImpl::GetRelativeMouseMove() {
        return glm::vec2();
      }
      bool LinuxInputImpl::IsMouseButtonDown(InputCode::MouseButton::MouseButton _button) {
        return false;
      }
      bool LinuxInputImpl::IsMouseButtonPressed(InputCode::MouseButton::MouseButton _button) {
        return false;
      }
      bool LinuxInputImpl::IsMouseButtonReleased(InputCode::MouseButton::MouseButton _button) {
        return false;
      }
      int LinuxInputImpl::GetScrollWheelDelta() {
        return 0;
      }

      // Joysticks / Gamepads
      bool LinuxInputImpl::IsGamePadButtonDown() {
        return false;
      }
      bool LinuxInputImpl::IsGamePadButtonPressed() {
        return false;
      }
      bool LinuxInputImpl::IsGamePadButtonReleased() {
        return false;
      }
      glm::vec2 LinuxInputImpl::GetGamePadAxis() {
        return glm::vec2();
      }

      // Graphics tablets
      glm::vec2 LinuxInputImpl::GetTabletPos() {
        return glm::vec2();
      }
      bool LinuxInputImpl::IsTabletButtonDown() {
        return false;
      }
      bool LinuxInputImpl::IsTabletButtonPressed() {
        return false;
      }
      bool LinuxInputImpl::IsTabletButtonReleased() {
        return false;
      }

      // Update
      void LinuxInputImpl::Update() {

      }

    }
  }
}

#endif
