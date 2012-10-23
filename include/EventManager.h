/*
 * header file for EventManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 09/2011, Project Cube
 */

#ifndef __PROJECT_CUBE_EVENT_MANAGER_HEADER
#define __PROJECT_CUBE_EVENT_MANAGER_HEADER

#include <memory>
#include <map>
#include <vector>
#include <list>

/*
 *  EventManager
 *
 *  This class is an event manager, which is designed to collect events
 *  and send it to all registered listeners.
 *
 *  To define a new event, a class derived from IEvent must be defined,
 *  and registered in the constructor of the EventManager() (see source,
 *  and do the same as for the other events)
 *
 *  To register the event sucessfully, it must provide some static methods:
 *
 *  static void RegisterLua();
 *  which should call the necessary lua methods to register the class in lua.
 *
 *  static std::shared_ptr<IEvent> GetPrototype();
 *  which creates a prototype of the event
 */

class EventManager
{
public:
    /*! \name Public interfaces */
    //@{
	class IEvent
	{
	public:
	    /*! \name Public types */
	    //@{
		typedef const char * TEventType;
	    //@}

	    /*  ***
	     *  *** The class should also have some static methods,
	     *  *** which cannot be defined through the interface (because they are static :-) ):
	    //@{
		/// returns the prototype of this event
		static std::shared_ptr<IEvent> GetPrototype();

		/// creates and returns a new instance of this event
		static std::shared_ptr<IEvent> Create(parameters);

		/// creates a new instance of this event and reads the parameter from the given string
		static std::shared_ptr<IEvent> CreateFromString(std::string sParameters);

		/// registers the event in the LUA environment
		static void RegisterLua();

		/// casts the given shared_ptr<IEvent> to a shared_ptr of this class, if possible
		static std::shared_ptr<DerivedClassOfIEvent> Cast(std::shared_ptr<IEvent> spEvent);

		/// returns the event type of this event
		static TEventType EventType() { return s_szEventType; }
	    //@}
	    */

	    /*! \name Public methods */
	    //@{
		/// returns the event type of the concrete instance
		virtual TEventType GetEventType() const = 0;

		/// returns true if this event has the given type
		bool IsEventType(TEventType tEventType) { return (GetEventType()==tEventType); }
	    //@}
	//protected:
	    /*! \name Construction / Destruction */
	    //@{
		/// constructor, counts the instances of IEvents
		IEvent() { s_iCount++; }

		/// destructor
		virtual ~IEvent() { s_iCount--; }
	    //@}

	public:
	    /*! \name Private members */
	    //@{
		static int s_iCount;
	    //@}
	};

	/*! \class IEventListener */
	//@{
	    /// this interface must be used to handle events
	    class IEventListener
	    {
	    public:
		virtual bool OnEvent(std::shared_ptr<EventManager::IEvent> spEvent) = 0;
	    };
	//@}
    //@}

    /*! \name Construction / Destruction */
    //@{
	/// constructor, registers all known event types
	EventManager();

	/// destructor, checks if all events were deleted
	~EventManager();
    //@}

    /*! \name Public methods */
    //@{
	/// triggers an event immediately
	void TriggerEvent(std::shared_ptr<IEvent> spEvent);

	/// queues an event and triggers it as soon as ProcessEvents() is called
	void QueueEvent(std::shared_ptr<IEvent> spEvent);

	/// processes all queued events.
	/// events which are created while processing the queue are
	/// processed the next time
	void ProcessEvents();

	/// registers an event listener for the given event types
	void RegisterEventListener(IEventListener *pListener, std::vector<IEvent::TEventType> vtEventNames);

	/// registers an event listener for the given event type
	void RegisterEventListener(IEventListener *pListener, IEvent::TEventType tEventName);

	/// register all known EventTypes in the EventManager and in the Lua-environment
	void Initialize();
    //@}

private:
    /*! \name Private helper methods */
    //@{
	/// registers an event T in the event manager (template method!)
	template<class T> void ItlRegisterEvent();

	/// registers the prototype of an event in the event manager
	void ItlRegisterEventPrototype(std::shared_ptr<IEvent> spEventPrototype);

	/// checks if the prototype of an given event type is known to the event manager
	bool ItlCheckIfEventIsRegistered(std::shared_ptr<IEvent> spEvent);
    //@}

    /*! \name Private members */
    //@{
	std::list<std::shared_ptr<IEvent> >			    m_lPrototypes;	    ///< a list of all known event types (prototypes), each registered event type is stored in this list
	std::map<IEvent::TEventType, std::list<IEventListener *> >  m_mEventListener;	    ///< a map which holds a list for each event type, and in this list are all listeners
	std::list<std::shared_ptr<IEvent> >			    m_lEventQueue[2];	    ///< doublebuffered list of queued events

	unsigned int m_nActiveQueue;						    ///< the currently active list in the doublebuffered list m_lEventQueue[]
    //@}
};

template<class T> void EventManager::ItlRegisterEvent()
{
    // register the event type
    // if the compiler fails at this line, it means that
    // the specific Event does not have the necessary static method GetPrototype()
    ItlRegisterEventPrototype(T::GetPrototype());
}

#endif // __PROJECT_CUBE_EVENT_MANAGER_HEADER
