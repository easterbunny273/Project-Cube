#include "ProcessManager.h"

#include <memory>

void ProcessManager::UpdateProcesses(int iDeltaMilliseconds)
{
    auto iter = m_lProcessList.begin();

    std::shared_ptr<IProcess> spNext;

    while (iter != m_lProcessList.end())
    {
	std::shared_ptr<IProcess> p(*iter);

	if (p->IsDead())
	{
	    // check for a child process and add if exists
	    spNext = p->GetNext();

	    if (spNext)
	    {
		// set next of p
		p->SetNext(std::shared_ptr<IProcess>((IProcess*) NULL));

		// attach spNext
		Attach(spNext);
	    }

	    Detach(p);
	}
	else if (p->IsActive() && !p->IsPaused())
	{
	    p->VOnUpdate(iDeltaMilliseconds);
	}

	iter++;
    }
}

void ProcessManager::Attach(std::shared_ptr<IProcess> spProcess)
{
    m_lProcessList.push_back(spProcess);
}

void ProcessManager::Detach(std::shared_ptr<IProcess> spProcess)
{
    m_lProcessList.remove(spProcess);
}
