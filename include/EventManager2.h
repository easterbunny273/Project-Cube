#ifndef __EVENT_MANAGER2_HEADER
#define __EVENT_MANAGER2_HEADER

#include <string>
#include <memory>
#include <map>

class IEvent
{
public:

    virtual std::string GetName() const = 0;
    //virtual std::shared_ptr<IEvent> GetPrototype() = 0;

    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString) = 0;
};

class InputEvent : public IEvent
{
private:
    InputEvent() {}

    int m_iX, m_iY;
public:
    InputEvent(int x, int y);

    virtual std::string GetName() const { return std::string("input.key"); }

    static std::shared_ptr<IEvent> GetPrototype();

    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString);

};

class InputMouse : public IEvent
{
private:
    InputMouse() {}

    int m_iX;
public:
    InputMouse(int x);

    virtual std::string GetName() const { return std::string("input.mouse"); }

    static std::shared_ptr<IEvent> GetPrototype();

    virtual std::shared_ptr<IEvent> CreateNewEventFromString(std::string sCreateString);

};

class EventManager
{
public:
    std::shared_ptr<IEvent> CreateEvent(std::string sCreateString);

    void RegisterEventType(std::shared_ptr<IEvent> spEventPrototype);

    void Test();
private:
    std::map<std::string, std::shared_ptr<IEvent> > m_mPrototypes;
};

#endif
