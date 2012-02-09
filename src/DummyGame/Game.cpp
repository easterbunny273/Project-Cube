#include "DummyGame/Game.h"
#include "DummyGame/SampleObject.h"

#include "MainApp.h"
#include "PC_Logger.h"
#include "EventManager.h"
#include "Events.h"

std::list<IObject *> DummyGame::GetObjectsInCube()
{
    std::list<IObject *> lTestList;

    lTestList.push_back(new SampleObject());

    return lTestList;
}

std::shared_ptr<ICube *> DummyGame::GetCurrentCube()
{
    return std::shared_ptr<ICube *>();
}

glm::vec3 DummyGame::GetPlayerPositionInCurrentCube()
{
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

void DummyGame::ItlOnKeyDown(InputKeyEvent::TKey eKey)
{
    MainApp *pApp = MainApp::GetInstance();
    assert (pApp != NULL);

    EventManager *pEventManager = pApp->GetEventManager();
    assert(pEventManager != NULL);

    if (eKey == 'W')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Z, 3.0f));
    else if (eKey == 'S')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Z, -3.0f));
    else if (eKey == 'A')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_X, -3.0f));
    else if (eKey == 'D')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_X, 3.0f));
    else if (eKey == InputKeyEvent::KEY_LCTRL)
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Y, -3.0f));
    else if (eKey == InputKeyEvent::KEY_SPACE)
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Y, 3.0f));
    else if (eKey == InputKeyEvent::KEY_ESC)
	m_bStop = true;

}

void DummyGame::ItlOnKeyUp(InputKeyEvent::TKey eKey)
{
    MainApp *pApp = MainApp::GetInstance();
    assert (pApp != NULL);

    EventManager *pEventManager = pApp->GetEventManager();
    assert(pEventManager != NULL);

    if (eKey == 'W')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Z, -3.0f));
    else if (eKey == 'S')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Z, 3.0f));
    else if (eKey == 'A')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_X, 3.0f));
    else if (eKey == 'D')
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_X, -3.0f));
    else if (eKey == InputKeyEvent::KEY_LCTRL)
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Y, 3.0f));
    else if (eKey == InputKeyEvent::KEY_SPACE)
    pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_MOVE_Y, -3.0f));
    else if (eKey == InputKeyEvent::KEY_ESC)
	m_bStop = true;
}

void DummyGame::OnMouseMove(int iX, int iY)
{
    if (m_bLeftMouseButtonDown)
    {
	MainApp *pApp = MainApp::GetInstance();
	assert (pApp != NULL);

	EventManager *pEventManager = pApp->GetEventManager();
	assert(pEventManager != NULL);

	pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_ROTATE_X, iX / 10.0f));
	pEventManager->QueueEvent(CameraMovementEvent::Create(CameraMovementEvent::CAMERA_ROTATE_Y, iY / 10.0f));
    }
}

void DummyGame::OnMouseButtonPressed(InputMouseButtonEvent::TMouseButton eButton)
{
    if (eButton == InputMouseButtonEvent::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = true;

	// hide mouse and lock it to window center
	// input events will now report the relative position to the window center
        //MainApp::GetInstance()->GetGraphic()->HideAndLockMouseToWindowCenter();
    }
}

void DummyGame::OnMouseButtonReleased(InputMouseButtonEvent::TMouseButton eButton)
{
    if (eButton == InputMouseButtonEvent::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = false;

        //MainApp::GetInstance()->GetGraphic()->UnHideAndUnLockMouse();
    }
}

DummyGame::DummyGame()
{
    m_bStop = false;
    m_bLeftMouseButtonDown = false;

    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, InputKeyEvent::EventType());
    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, InputMouseButtonEvent::EventType());
    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, InputMouseMoveEvent::EventType());
}

bool DummyGame::OnEvent(std::shared_ptr<EventManager::IEvent> spEvent)
{
    if (spEvent->IsEventType(InputKeyEvent::EventType()))
    {
	std::shared_ptr<InputKeyEvent> spInputKeyEvent = InputKeyEvent::Cast(spEvent);
	assert (spInputKeyEvent);

	InputKeyEvent::TKey eKey = spInputKeyEvent->GetKey();
	InputKeyEvent::TEvent eEvent = spInputKeyEvent->GetEvent();

	if (eEvent == InputKeyEvent::EVENT_UP)
	    ItlOnKeyUp(eKey);
	else if (eEvent == InputKeyEvent::EVENT_DOWN)
	    ItlOnKeyDown(eKey);
	else
	    assert (!"no if condition fired");
    }
    else if (spEvent->IsEventType(InputMouseButtonEvent::EventType()))
    {
	std::shared_ptr<InputMouseButtonEvent> spInputMouseButtonEvent = InputMouseButtonEvent::Cast(spEvent);
	assert (spInputMouseButtonEvent);

	InputMouseButtonEvent::TMouseButton eButton = spInputMouseButtonEvent->GetMouseButton();
	InputMouseButtonEvent::TEvent eEvent = spInputMouseButtonEvent->GetEvent();

	if (eEvent == InputMouseButtonEvent::EVENT_UP)
	    OnMouseButtonReleased(eButton);
	else
	    OnMouseButtonPressed(eButton);
    }
    else if (spEvent->IsEventType(InputMouseMoveEvent::EventType()))
    {
	std::shared_ptr<InputMouseMoveEvent> spInputMouseMoveEvent = InputMouseMoveEvent::Cast(spEvent);
	assert (spInputMouseMoveEvent);

	int iRelX = spInputMouseMoveEvent->GetX();
	int iRelY = spInputMouseMoveEvent->GetY();

	OnMouseMove(iRelX, iRelY);
    }
	return true;
}
