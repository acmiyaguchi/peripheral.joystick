/*
 *      Copyright (C) 2014-2015 Garrett Brown
 *      Copyright (C) 2014-2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "JoystickXInput.h"
#include "JoystickInterfaceXInput.h"
#include "XInputDLL.h"
#include "log/Log.h"

#include <Xinput.h>

using namespace JOYSTICK;

#define XINPUT_ALIAS  "Xbox 360-compatible controller"
#define BUTTON_COUNT  15
#define HAT_COUNT     0 // hats are treated as buttons
#define AXIS_COUNT    6
#define MAX_AXIS      32768
#define MAX_TRIGGER   255

CJoystickXInput::CJoystickXInput(unsigned int controllerID, CJoystickInterfaceXInput* api)
 : CJoystick(api),
   m_controllerID(controllerID),
   m_dwPacketNumber(0)
{
  SetName(XINPUT_ALIAS);
  SetRequestedPort(m_controllerID);
  SetButtonCount(BUTTON_COUNT);
  SetHatCount(HAT_COUNT);
  SetAxisCount(AXIS_COUNT);
}

bool CJoystickXInput::Equals(const CJoystick* rhs) const
{
  if (rhs == nullptr)
    return false;

  const CJoystickXInput* rhsXInput = dynamic_cast<const CJoystickXInput*>(rhs);
  if (rhsXInput == nullptr)
    return false;

  return m_controllerID == rhsXInput->m_controllerID;
}

bool CJoystickXInput::ScanEvents(void)
{
  XINPUT_STATE_EX controllerState;

  if (!CXInputDLL::Get().GetState(m_controllerID, controllerState))
    return false;

  m_dwPacketNumber = controllerState.dwPacketNumber;

  SetButtonValue(0,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A)              ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(1,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B)              ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(2,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X)              ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(3,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)              ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(4,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(5,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(6,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)           ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(7,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START)          ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(8,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(9,  (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(10, (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)        ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(11, (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(12, (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(13, (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);
  SetButtonValue(14, (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_GUIDE)          ? JOYSTICK_STATE_BUTTON_PRESSED : JOYSTICK_STATE_BUTTON_UNPRESSED);

  SetAxisValue(0, (long)controllerState.Gamepad.sThumbLX, MAX_AXIS);
  SetAxisValue(1, (long)controllerState.Gamepad.sThumbLY, MAX_AXIS);
  SetAxisValue(2, (long)controllerState.Gamepad.sThumbRX, MAX_AXIS);
  SetAxisValue(3, (long)controllerState.Gamepad.sThumbRY, MAX_AXIS);
  SetAxisValue(4, (long)controllerState.Gamepad.bLeftTrigger, MAX_TRIGGER);
  SetAxisValue(5, (long)controllerState.Gamepad.bRightTrigger, MAX_TRIGGER);

  return true;
}
