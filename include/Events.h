#ifndef __PROJECT_CUBE_EVENTS_HEADER
#define __PROJECT_CUBE_EVENTS_HEADER

#include "EventManager.h"

class InputKeyEvent : public EventManager::IEvent
{
public:
    /*! \name Public types */
    //@{
	/// enumeration for different keys
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

	/// enumeration for events
	enum TEvent
	{
	    EVENT_UP,
	    EVENT_DOWN
	};
    //@}

    /*! \name Static methods*/
    //@{
	/// returns the prototype of this event
	static std::shared_ptr<IEvent> GetPrototype();

	/// creates and returns a new instance of this event
	static std::shared_ptr<IEvent> Create(TKey eKey, TEvent eEvent);

	/// creates a new instance of this event and reads the parameter from the given string
	static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

	/// registers the event in the LUA environment
	static void RegisterLua();

	/// casts the given shared_ptr<IEvent> to a shared_ptr of this class, if possible
	static std::shared_ptr<InputKeyEvent> Cast(std::shared_ptr<IEvent> spEvent);

	/// returns the event type of this event
	static TEventType EventType() { return s_szEventType; }
    //@}

    /*! \name Public methods */
    //@{
	/// returns the event type
	virtual TEventType GetEventType() const { return s_szEventType; }
    //@}

    /*! \name Public attributes */
    //@{
	TKey GetKey() const { return m_eKey; }
	TEvent GetEvent() const { return m_eEvent; }
    //@}

private:
    /*! \name Private methods */
    //@{
	/// constructor, a new instance of this class must be created through Create()
	InputKeyEvent() {}
    //@}

    /*! \name Private members */
    //@{
	TKey    m_eKey;
	TEvent  m_eEvent;
    //@}

    /*! \name Static members */
    //@{
	static TEventType s_szEventType;
    //@}

};

class InputMouseButtonEvent : public EventManager::IEvent
{
public:
    /*! \name Public types */
    //@{
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
    //@}

    /*! \name Static methods*/
    //@{
	/// returns the prototype of this event
	static std::shared_ptr<IEvent> GetPrototype();

	/// creates and returns a new instance of this event
	static std::shared_ptr<IEvent> Create(TMouseButton eButton, TEvent eEvent);

	/// creates a new instance of this event and reads the parameter from the given string
	static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

	/// registers the event in the LUA environment
	static void RegisterLua();

	/// casts the given shared_ptr<IEvent> to a shared_ptr of this class, if possible
	static std::shared_ptr<InputMouseButtonEvent> Cast(std::shared_ptr<IEvent> spEvent);

	/// returns the event type of this event
	static TEventType EventType() { return s_szEventType; }
    //@}

    /*! \name Public methods */
    //@{
	/// returns the event type
	virtual TEventType GetEventType() const { return s_szEventType; }
    //@}

    /*! \name Public attributes */
    //@{
	TEvent GetEvent() const { return m_eEvent; }
	TMouseButton GetMouseButton() const { return m_eButton; }
    //@}

private:
    /*! \name Private methods */
    //@{
	/// constructor, a new instance of this class must be created through Create()
	InputMouseButtonEvent() {}
    //@}

    /*! \name Private members */
    //@{
	TEvent		m_eEvent;
	TMouseButton    m_eButton;
    //@}

    /*! \name Static members */
    //@{
	static TEventType s_szEventType;
    //@}
};

class InputMouseMoveEvent : public EventManager::IEvent
{
public:
    /*! \name Static methods*/
    //@{
	/// returns the prototype of this event
	static std::shared_ptr<IEvent> GetPrototype();

	/// creates and returns a new instance of this event
	static std::shared_ptr<IEvent> Create(int iX, int iY);

	/// creates a new instance of this event and reads the parameter from the given string
	static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

	/// registers the event in the LUA environment
	static void RegisterLua();

	/// casts the given shared_ptr<IEvent> to a shared_ptr of this class, if possible
	static std::shared_ptr<InputMouseMoveEvent> Cast(std::shared_ptr<IEvent> spEvent);

	/// returns the event type of this event
	static TEventType EventType() { return s_szEventType; }
    //@}

    /*! \name Public methods */
    //@{
	/// returns the event type
	virtual TEventType GetEventType() const { return s_szEventType; }
    //@}

    /*! \name Public attributes */
    //@{
	int GetX() const { return m_iX; }
	int GetY() const { return m_iY; }
    //@}

private:
    /*! \name Private methods */
    //@{
	/// constructor, a new instance of this class must be created through Create()
	InputMouseMoveEvent() {}
    //@}

    /*! \name Private members */
    //@{
	int m_iX;
	int m_iY;
    //@}

    /*! \name Static members */
    //@{
	static TEventType s_szEventType;
    //@}
};

class CameraMovementEvent : public EventManager::IEvent
{
public:
    enum TMovementType
    {
	CAMERA_MOVE_X,
	CAMERA_MOVE_Y,
	CAMERA_MOVE_Z,
	CAMERA_ROTATE_X,
	CAMERA_ROTATE_Y
    };

    /*! \name Static methods*/
    //@{
	/// returns the prototype of this event
	static std::shared_ptr<IEvent> GetPrototype();

	/// creates and returns a new instance of this event
	static std::shared_ptr<IEvent> Create(TMovementType tMovementType, float fValue);

	/// creates a new instance of this event and reads the parameter from the given string
	static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

	/// registers the event in the LUA environment
	static void RegisterLua();

	/// casts the given shared_ptr<IEvent> to a shared_ptr of this class, if possible
	static std::shared_ptr<CameraMovementEvent> Cast(std::shared_ptr<IEvent> spEvent);

	/// returns the event type of this event
	static TEventType EventType() { return s_szEventType; }
    //@}

    /*! \name Public methods */
    //@{
	/// returns the event type
	virtual TEventType GetEventType() const { return s_szEventType; }
    //@}

    /*! \name Public attributes */
    //@{
	TMovementType GetMovementType() const { return m_eMovementType; }
	float GetValue() const { return m_fValue; }
    //@}

private:
    /*! \name Private methods */
    //@{
	/// constructor, a new instance of this class must be created through Create()
	CameraMovementEvent() {}
    //@}

    /*! \name Private members */
    //@{
	TMovementType	m_eMovementType;
	float		m_fValue;
    //@}

    /*! \name Static members */
    //@{
	static TEventType s_szEventType;
    //@}
};

#endif
