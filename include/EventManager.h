/*
 * header file for EventManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 09/2011, Project Cube
 */

#ifndef __EVENT_MANAGER2_HEADER
#define __EVENT_MANAGER2_HEADER

#include <string>
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
    class IEvent
    {
    public:
	virtual std::string GetName() const = 0;

	virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString) = 0;
    };

    class IEventListener
    {
    public:
	virtual bool OnEvent(std::shared_ptr<EventManager::IEvent> spEvent) = 0;
    };



    EventManager();
    std::shared_ptr<IEvent> CreateEvent(const char * szCreateString);
    void CreateEvent2(const char * szCreateString);



    void RegisterEventListener(IEventListener *pListener, std::vector<std::string> vsEventNames);
    void RegisterEventListener(IEventListener *pListener, std::string sEventName);

    void TriggerEvent(std::shared_ptr<IEvent> spEvent);

    void QueueEvent(std::shared_ptr<IEvent> spEvent);

    void ProcessEvents();

    void Test();


private:

    template<class T> void RegisterEvent();
    void ItlRegisterEventPrototype(std::shared_ptr<IEvent> spEventPrototype);

    bool ItlCheckIfEventIsRegistered(std::shared_ptr<IEvent> spEvent);

    std::list<std::shared_ptr<IEvent> > m_lPrototypes;

    std::map<std::string, std::list<IEventListener *> > m_mEventListener;

    std::list<std::shared_ptr<IEvent> > m_lEventQueue[2];

    unsigned int m_nActiveQueue;
};

template<class T> void EventManager::RegisterEvent()
{
    // register the event type
    // if the compiler fails at this line, it means that
    // the specific Event does not have the necessary static method GetPrototype()
    ItlRegisterEventPrototype(T::GetPrototype());

    // register the event in LUA
    // if the compiler fails at this line, it means that
    // the specific Event does not have the necessary static method RegisterLua()
    T::RegisterLua();
}

#endif
