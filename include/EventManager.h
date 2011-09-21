#ifndef __EVENT_MANAGER_HEADER
#define __EVENT_MANAGER_HEADER

#include <memory>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <map>

#include "HashedString.h"

class IEventData;

class IEventListener
{
public:
    IEventListener() {};
    virtual ~IEventListener() {};

    /// returns ascii-test name for this listener
    virtual char const * GetName() = 0;

    virtual bool HandleEvent (IEventData const &rEvent) = 0;
};

typedef HashedString EventType;

class IEventData
{
public:
    virtual const EventType &VGetEventType() const = 0;
    virtual float VGetTimeStamp() const = 0;
   // virtual void VSerialize(std::ostrstream &out) = 0;
};

typedef std::shared_ptr<IEventData> IEventDataPtr;

class BaseEventData : public IEventData
{
public:
    explicit BaseEventData(const float fTimeStamp = 0.0f) : m_fTimeStamp(fTimeStamp) {}
    virtual ~BaseEventData() {}

    virtual const EventType &VGetEventType() const = 0;
    float VGetTimeStamp() const { m_fTimeStamp; }
  //  virtual void VSerialize(std::ostrstream &out) const {}

protected:
    const float m_fTimeStamp;
};

typedef std::shared_ptr<IEventListener> EventListenerPtr;

class IEventManager
{
public:
    enum eConstants { kINFINITE = 0xffffffff };
    IEventManager(char const * const szName, bool bSetAsGlobal) {};
    virtual ~IEventManager() {};

    virtual bool VAddListener(EventListenerPtr const &inHandler, EventType const &inType) = 0;

    virtual bool VDelListener(EventListenerPtr const &inHandler, EventType const &inType) = 0;

    virtual bool VTrigger(IEventData const &inEvent) = 0;

    virtual bool VQueueEvent(IEventDataPtr const &inEvent) = 0;

    virtual bool VAbortEvent(EventType const &inType, bool bAllOfType = false) = 0;

    virtual bool VTick (unsigned long nMaxMillis = kINFINITE) = 0;

    virtual bool VValidateType(EventType const &inType) const = 0;

private:
    static IEventManager *Get();
};

typedef std::vector<EventListenerPtr> EventListenerList;
typedef std::vector<EventType> EventTypeList;

class EventManager : public IEventManager
{
public:
    explicit EventManager(char const * const szName, bool bSetAsGlobal);
    virtual ~EventManager();

    bool VAddListener(EventListenerPtr const &inListener, EventType const &inType);
    bool VDelListener(EventListenerPtr const &inListener, EventType const &inType);
    bool VTrigger(IEventData const &inEvent) const;
    bool VQueueEvent(IEventDataPtr const &inEvent);
    bool VAbortEvent(EventType const &inType, bool bAllOfType);
    bool VTick(unsigned long nMaxMillis);
    bool VValidateType(const EventType &inType) const;

    EventListenerList GetListenerList(EventType const &eventType) const;

    EventTypeList GetTypeList() const;

private:
    typedef std::set<EventType> EventTypeSet;

    typedef std::pair<EventTypeSet::iterator, bool> EventTypeSetIRes;

    typedef std::list<EventListenerPtr> EventListenerTable;

    typedef std::map<unsigned int, EventListenerTable> EventListenerMap;

    typedef std::pair<unsigned int, EventListenerTable> EventListenerMapEnt;

    typedef std::pair <EventListenerMap::iterator, bool> EventListenerMapIRes;

    typedef std::list <IEventDataPtr> EventQueue;

    enum eConstants { kNumQueues = 2 };

    EventTypeSet m_typeList;

    EventListenerMap m_registry;

    EventQueue m_queues[kNumQueues];

    int m_activeQueue;

};

#endif
