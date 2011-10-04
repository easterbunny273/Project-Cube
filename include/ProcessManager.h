/* this class is copied from "Game Coding Complete" and should be used later to organize the subsystems as processes,
   as well as specific types of events (in general, not in the meaning of the coded Events+EventManager)*/

#ifndef __PROCESS_MANAGER_HEADER
#define __PROCESS_MANAGER_HEADER

#include <memory>
#include <list>

class IProcess
{
    friend class ProcessManager;
protected:
    int	m_iType;
    bool m_bKill;
    bool m_bActive;
    bool m_bPaused;
    bool m_bInitialUpdate;
    std::shared_ptr<IProcess> m_spNext;

private:
    unsigned int m_nProcessFlags;
public:
    IProcess(int iType, unsigned int nOrder=0);
    virtual ~IProcess();

    bool IsDead() const { return m_bKill; }
    int GetType() const { return m_iType; }
    void SetType(const int iType) { m_iType = iType; }
    bool IsActive() const { return m_bActive; }
    void SetActive(const bool bActive) { m_bActive = bActive; }
    bool IsAttached() const;
    void SetAttached(const bool bWantAttached);
    bool IsPaused() const { return m_bPaused; }
    bool IsInitialized() const { return !m_bInitialUpdate; }
    std::shared_ptr<IProcess> const GetNext() const { return m_spNext; }
    void SetNext(std::shared_ptr<IProcess> spNext) { m_spNext = spNext; }

    virtual void VOnUpdate(const int iDeltaMilliseconds)
    {
	if (m_bInitialUpdate)
	{
	    VOnInitialize();
	    m_bInitialUpdate = false;
	}
    }

    virtual void VOnInitialize() {}
    virtual void VKill();
    virtual void VTogglePause() { m_bPaused = !m_bPaused; }
};

class ProcessManager
{
public:
    void Attach(std::shared_ptr<IProcess> spProcess);
    bool bHasProcesses();
    bool IsProcessActive(int iType);
    void UpdateProcesses(int iDeltaMilliseconds);
    ~ProcessManager();

private:
    std::list<std::shared_ptr<IProcess> > m_lProcessList;

    void Detach(std::shared_ptr<IProcess> spProcess);
};

#endif
