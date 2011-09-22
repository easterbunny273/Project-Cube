#ifndef __PROJECT_CUBE_EVENTS_HEADER
#define __PROJECT_CUBE_EVENTS_HEADER

#include "EventManager.h"

class InputKeyEvent : public EventManager::IEvent
{

public:
    enum TKey
    {
	KEY_UNKNOWN = 0,

	// key modifiers
	KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL, KEY_LALT, KEY_RALT,

	// F1..F12
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

	// cursor keys
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

	// special keys like SPACE and ESC
	KEY_ESC, KEY_SPACE, KEY_TAB, KEY_ENTER, KEY_BACKSPACE,

	// numbers 0..9 (mapped to ascii-conform values 48..57)
	KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

	// chars A..Z (mapped to ascii-conform values 65..90
	KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E,
	KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
	KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
	KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
	KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

	// numbers on numpad
	KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4,
	KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9

	// blabla. not finished yet, but you got the idea :)
    };

    enum TEvent
    {
	EVENT_UP,
	EVENT_DOWN
    };

    virtual std::string GetName() const { return std::string("input.key"); }

    static std::shared_ptr<IEvent> GetPrototype();

    static std::shared_ptr<IEvent> Create(TKey eKey, TEvent eEvent);
    static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString);

    static void RegisterLua();

    TKey GetKey() const { return m_eKey; }
    TEvent GetEvent() const { return m_eEvent; }

private:
    InputKeyEvent() {}

    TKey    m_eKey;
    TEvent  m_eEvent;

};

class InputMouseButtonEvent : public EventManager::IEvent
{
public:
    enum TMouseButton
    {
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT
    };

    enum TEvent
    {
	EVENT_UP,
	EVENT_DOWN
    };

    virtual std::string GetName() const { return std::string("input.mouse.button"); }

    static std::shared_ptr<IEvent> GetPrototype();

    static std::shared_ptr<IEvent> Create(TMouseButton eButton, TEvent eEvent);
    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString);

    static void RegisterLua() {};

    TEvent GetEvent() const { return m_eEvent; }
    TMouseButton GetMouseButton() const { return m_eButton; }
private:
    InputMouseButtonEvent() {}

    TEvent	    m_eEvent;
    TMouseButton    m_eButton;
};

class InputMouseMoveEvent : public EventManager::IEvent
{
public:

    virtual std::string GetName() const { return std::string("input.mouse.move"); }

    static std::shared_ptr<IEvent> GetPrototype();

    static std::shared_ptr<IEvent> Create(int iX, int iY);
    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString);

    static void RegisterLua() {};

    int GetX() const { return m_iX; }
    int GetY() const { return m_iY; }
private:
    InputMouseMoveEvent() {}

    int m_iX;
    int m_iY;
};

#endif
