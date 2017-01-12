#if defined __linux__

#include <Orange/logging/Logging.hpp>
#include <Orange/priv/linux/LinuxWindowImpl.hpp>
#include <Orange/priv/linux/LinuxInputImpl.hpp>

#include <glm/glm.hpp>

#include <cstring>

// https://github.com/depp/keycode/blob/master/keytable_evdev.c
/* This file is automatically generated by keycode.py.  */
const unsigned char EVDEV_HID_TO_NATIVE[256] = {
255,255,255,255, 38, 56, 54, 40, 26, 41, 42, 43, 31, 44, 45, 46,
 58, 57, 32, 33, 24, 27, 39, 28, 30, 55, 25, 53, 29, 52, 10, 11,
 12, 13, 14, 15, 16, 17, 18, 19, 36,  9, 22, 23, 65, 20, 21, 34,
 35, 51,255, 47, 48, 49, 59, 60, 61, 66, 67, 68, 69, 70, 71, 72,
 73, 74, 75, 76, 95, 96,107, 78,127,118,110,112,119,115,117,114,
113,116,111, 77,106, 63, 82, 86,104, 87, 88, 89, 83, 84, 85, 79,
 80, 81, 90, 91, 94,255,255,125,191,192,193,194,195,196,197,198,
199,200,201,202,255,255,135,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
 37, 50, 64,133,105, 62,108,134,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};
const unsigned char EVDEV_NATIVE_TO_HID[256] = {
255,255,255,255,255,255,255,255,255, 41, 30, 31, 32, 33, 34, 35,
 36, 37, 38, 39, 45, 46, 42, 43, 20, 26,  8, 21, 23, 28, 24, 12,
 18, 19, 47, 48, 40,224,  4, 22,  7,  9, 10, 11, 13, 14, 15, 51,
 52, 53,225, 49, 29, 27,  6, 25,  5, 17, 16, 54, 55, 56,229, 85,
226, 44, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 83, 71, 95,
 96, 97, 86, 92, 93, 94, 87, 89, 90, 91, 98, 99,255,255,100, 68,
 69,255,255,255,255,255,255,255, 88,228, 84, 70,230,255, 74, 82,
 75, 80, 79, 77, 81, 78, 73, 76,255,255,255,255,255,103,255, 72,
255,255,255,255,255,227,231,118,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,104,
105,106,107,108,109,110,111,112,113,114,115,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

namespace orange {
  namespace priv {
    namespace linux {

      void LinuxInputImpl::UpdateDevices() {
        // Get the correct locations for data within raw input messages...
        // Only managed to figure this out because of https://www.baseplex.de/docs/opentk/html/_x_i2_mouse_keyboard_8cs_source.html
        mousemapping.clear();

        int deviceCount = 0;
        XIDeviceInfo* deviceInfo = XIQueryDevice(display, XIAllMasterDevices, &deviceCount);
        for (int i = 0; i < deviceCount; ++i) {
          switch(deviceInfo[i].use) {
            case XIMasterKeyboard: {

              break;
            }

            case XIMasterPointer: {
              mousemapping[deviceInfo[i].deviceid] = MouseDevice();
              for (int j = 0; j < deviceInfo[i].num_classes; ++j) {
                if (!deviceInfo[i].classes)
                  continue;

                XIAnyClassInfo* classInfo = deviceInfo[i].classes[j];
                if (!classInfo)
                  continue;

                switch (classInfo->type) {
                  case XIButtonClass: {
                    XIButtonClassInfo* button = (XIButtonClassInfo*)classInfo;
                    LOG(Log::DEFAULT) << "Buttons: " << button->num_buttons;
                    break;
                  }

                  case XIScrollClass: {
                    XIScrollClassInfo* scroll = (XIScrollClassInfo*)classInfo;
                    switch (scroll->scroll_type) {
                      case XIScrollTypeVertical:
                        mousemapping[deviceInfo[i].deviceid].ScrollY = *scroll;
                        break;

                      case XIScrollTypeHorizontal:
                        mousemapping[deviceInfo[i].deviceid].ScrollX = *scroll;
                        break;

                      default:
                        break;
                    }
                    break;
                  }

                  case XIValuatorClass: {
                    XIValuatorClassInfo* valuator = (XIValuatorClassInfo*)classInfo;
                    if (valuator->label == XInternAtom(display, "Rel X", false))
                      mousemapping[deviceInfo[i].deviceid].MotionX = *valuator;
                    else if (valuator->label == XInternAtom(display, "Rel Y", false))
                      mousemapping[deviceInfo[i].deviceid].MotionY = *valuator;
                    else if (valuator->label == XInternAtom(display, "Abs X", false))
                      mousemapping[deviceInfo[i].deviceid].AbsoluteX = *valuator;
                    else if (valuator->label == XInternAtom(display, "Abs Y", false))
                      mousemapping[deviceInfo[i].deviceid].AbsoluteY = *valuator;

                    break;
                  }

                }
              }
              break;
            }


          }
        }

        XIFreeDeviceInfo(deviceInfo);
      }

      LinuxInputImpl::LinuxInputImpl(WindowImpl* _impl) {
        // Store the window impl we are associated with.
        window = _impl;

        // Use our own connection so we don't have to do any synchronization.
        display = XOpenDisplay(nullptr);

        // Makes sure we have xinput extension available.
        int opcode, event, error;
        if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &error)) {
           LOG(Log::CRITICAL) << "X Input extension not available.";
        }
        xinputOpcode = opcode;

        // Sure we support at least version 2.0
        int major = 2, minor = 0;
        if (XIQueryVersion(display, &major, &minor) == BadRequest) {
          LOG(Log::CRITICAL) << "XI2 not available. Server supports " << major << "." << minor;
        }

        // Need to use a root window to get raw input for buttons and keyboard.
        dummyWindow = DefaultRootWindow(display);

        // Select for the events we want.
        XIEventMask dummyEventMask;
        dummyEventMask.deviceid = XIAllDevices;
        dummyEventMask.mask_len = XIMaskLen(XI_RawMotion);
        dummyEventMask.mask = (unsigned char*)calloc(dummyEventMask.mask_len, sizeof(char));

        XISetMask(dummyEventMask.mask, XI_RawMotion);
        XISetMask(dummyEventMask.mask, XI_RawKeyPress);
        XISetMask(dummyEventMask.mask, XI_RawKeyRelease);
        XISetMask(dummyEventMask.mask, XI_RawButtonPress);
        XISetMask(dummyEventMask.mask, XI_RawButtonRelease);
        XISetMask(dummyEventMask.mask, XI_DeviceChanged);

        XISelectEvents(display, dummyWindow, &dummyEventMask, 1);

        free(dummyEventMask.mask);

        // Need this for absolute mouse positions.
        XIEventMask eventMask;
        eventMask.deviceid = XIAllDevices;
        eventMask.mask_len = XIMaskLen(XI_Motion);
        eventMask.mask = (unsigned char*)calloc(eventMask.mask_len, sizeof(char));

        XISetMask(eventMask.mask, XI_Motion);

        XISelectEvents(display, (::Window)window->GetWindowHandle(), &eventMask, 1);

        free(eventMask.mask);

        // Make sure we have the correct device id mappings to begin with.
        UpdateDevices();
      }
      LinuxInputImpl::~LinuxInputImpl() {
        // Destroy the window.
        XDestroyWindow(display, dummyWindow);

        // Close the display we created.
        if (display)
          XCloseDisplay(display);
      }

      // Keyboard
      bool LinuxInputImpl::IsKeyDown(InputCode::Key::Key _key) {
        return KeyboardKeys[_key];
      }
      bool LinuxInputImpl::IsKeyPressed(InputCode::Key::Key _key) {
        return false;
      }
      bool LinuxInputImpl::IsKeyReleased(InputCode::Key::Key _key) {
        return false;
      }

      // Mouse
      glm::vec2 LinuxInputImpl::GetMousePos() {
        return glm::vec2(MouseAbsoluteX, MouseAbsoluteY);
      }
      glm::vec2 LinuxInputImpl::GetRelativeMouseMove() {
        return glm::vec2(MouseRelX, MouseRelY);
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
        return (int)MouseRelWheelY;
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

      static double ReadValue(const XIRawEvent* event, int bit, bool raw=true) {
        double value = 0;
        if (XIMaskIsSet(event->valuators.mask, bit)) {
          double* raw_valuator = event->raw_values;
          double* valuator = event->valuators.values;

          for (int i = 0; i < bit; ++i) {
            if (XIMaskIsSet(event->valuators.mask, i)) {
              valuator++;
              raw_valuator++;
            }
          }

          if (raw)
            value = *raw_valuator;
          else
            value = *valuator;
        }
        return value;
      }

      static bool TranslateKey(int keycode, InputCode::Key::Key& key) {
        if (keycode < 0 || keycode >= sizeof(EVDEV_NATIVE_TO_HID))
          return false;

        key = (InputCode::Key::Key)EVDEV_NATIVE_TO_HID[keycode];
        return true;
      }

      // Update
      void LinuxInputImpl::Update() {
        // Copy over the current values for keyboard and mouse buttons into their old arrays.
        memcpy(OldKeyboardKeys, KeyboardKeys, sizeof(KeyboardKeys));
        memcpy(OldMouseButtons, MouseButtons, sizeof(MouseButtons));

        // Reset relative movement
        MouseRelX = 0;
        MouseRelY = 0;
        MouseRelWheelX = 0;
        MouseRelWheelY = 0;

        // Get all of the events that are currently queued.
        XSync(display, false);
        while (XEventsQueued(display, QueuedAlready) > 0) {
          // Get the event.
          XEvent event;
          XNextEvent(display, &event);
          if (event.xcookie.type == GenericEvent &&
              event.xcookie.extension == xinputOpcode &&
              XGetEventData(display, &event.xcookie)) {

            switch (event.xcookie.evtype) {
              case XI_DeviceChanged: {
                UpdateDevices();
                break;
              }

              case XI_RawButtonPress: {
                break;
              }

              case XI_RawButtonRelease: {
                break;
              }

              case XI_RawMotion: {
                XIRawEvent* rawEvent = (XIRawEvent*)event.xcookie.data;
                auto mousedevice = mousemapping.find(rawEvent->deviceid);
                if (mousedevice != mousemapping.end()) {
                  if (mousedevice->second.MotionX.number != -1)
                    MouseRelX += ReadValue(rawEvent, mousedevice->second.MotionX.number);
                  if (mousedevice->second.MotionY.number != -1)
                    MouseRelY += ReadValue(rawEvent, mousedevice->second.MotionY.number);

                  if (mousedevice->second.ScrollX.number != -1)
                    MouseRelWheelX += ReadValue(rawEvent, mousedevice->second.ScrollX.number) / mousedevice->second.ScrollX.increment;
                  if (mousedevice->second.ScrollY.number != -1)
                    MouseRelWheelY += ReadValue(rawEvent, mousedevice->second.ScrollY.number) / mousedevice->second.ScrollY.increment;
                }

                LOG(Log::DEFAULT) << "Mouse Move, X: " << MouseRelX << ", Y:" << MouseRelY;
                LOG(Log::DEFAULT) << "Mouse Scroll, X: " << MouseRelWheelX << ", Y:" << MouseRelWheelY;

                break;
              }

              case XI_Motion: {
                XIDeviceEvent* deviceEvent = (XIDeviceEvent*)event.xcookie.data;
                MouseAbsoluteX = deviceEvent->event_x;
                MouseAbsoluteY = deviceEvent->event_y;

                LOG(Log::DEFAULT) << "Mouse Absolute, X: " << MouseAbsoluteX << ", Y:" << MouseAbsoluteY;

                break;
              }

              case XI_RawKeyPress:
              case XI_RawKeyRelease: {
                XIRawEvent* rawEvent = (XIRawEvent*)event.xcookie.data;

                if (event.xcookie.evtype == XI_RawKeyPress)
                  LOG(Log::DEFAULT) << "Pressed Key: " << rawEvent->detail;
                else
                  LOG(Log::DEFAULT) << "Released Key: " << rawEvent->detail;

                InputCode::Key::Key key;
                if (TranslateKey(rawEvent->detail, key))
                  KeyboardKeys[(int)key] = event.xcookie.evtype == XI_RawKeyPress;
                break;
              }

            }

          }
        }




      }

    }
  }
}

#endif
