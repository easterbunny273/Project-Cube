#include "DummyGame/Game.h"
#include "DummyGame/SampleObject.h"

#include "MainApp.h"
#include "Logger.h"
#include "EventManager.h"
#include "Events.h"
#include "Graphics/Graphic.h"

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

    Graphic *pGraphic = pApp->GetGraphic();

    assert (pGraphic != NULL);

    Graphic::Camera *pCamera = pGraphic->GetCamera();

    assert (pCamera != NULL);

    if (eKey == 'W')
    {
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, 1.0));
    }
    else if (eKey == 'S')
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, -1.0));
    else if (eKey == 'A')
	pCamera->AddToMoveVector(glm::vec3(-1.0, 0.0, 0.0));
    else if (eKey == 'D')
	pCamera->AddToMoveVector(glm::vec3(1.0, 0.0, 0.0));
    else if (eKey == InputKeyEvent::KEY_LCTRL)
	pCamera->AddToMoveVector(glm::vec3(0.0, -1.0, 0.0));
    else if (eKey == InputKeyEvent::KEY_SPACE)
	pCamera->AddToMoveVector(glm::vec3(0.0, 1.0, 0.0));

}

void DummyGame::ItlOnKeyUp(InputKeyEvent::TKey eKey)
{
    MainApp *pApp = MainApp::GetInstance();

    assert (pApp != NULL);

    Graphic *pGraphic = pApp->GetGraphic();

    assert (pGraphic != NULL);

    Graphic::Camera *pCamera = pGraphic->GetCamera();

    assert (pCamera != NULL);

    if (eKey == 'W')
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, -1.0));
    else if (eKey == 'S')
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, 1.0));
    else if (eKey == 'A')
	pCamera->AddToMoveVector(glm::vec3(1.0, 0.0, 0.0));
    else if (eKey == 'D')
	pCamera->AddToMoveVector(glm::vec3(-1.0, 0.0, 0.0));
    else if (eKey == InputKeyEvent::KEY_LCTRL)
	pCamera->AddToMoveVector(glm::vec3(0.0, 1.0, 0.0));
    else if (eKey == InputKeyEvent::KEY_SPACE)
	pCamera->AddToMoveVector(glm::vec3(0.0, -1.0, 0.0));
    else if (eKey == InputKeyEvent::KEY_ESC)
	m_bStop = true;
}

void DummyGame::OnMouseMove(int iX, int iY)
{
    if (m_bLeftMouseButtonDown)
    {
	MainApp *pApp = MainApp::GetInstance();
	assert (pApp != NULL);

	Graphic *pGraphic = pApp->GetGraphic();
	assert (pGraphic != NULL);

	Graphic::Camera *pCamera = pGraphic->GetCamera();
	assert (pCamera != NULL);

	//Logger::debug() << "input: " << iX << ":" << iY << Logger::endl;
	// update camera
	pCamera->RotateHorizontal(iX / 10.0f);
	pCamera->RotateVertical(iY / 10.0f);
    }
}

void DummyGame::OnMouseButtonPressed(InputMouseButtonEvent::TMouseButton eButton)
{
    if (eButton == InputMouseButtonEvent::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = true;

	// hide mouse and lock it to window center
	// input events will now report the relative position to the window center
	MainApp::GetInstance()->GetGraphic()->HideAndLockMouseToWindowCenter();
    }
}

void DummyGame::OnMouseButtonReleased(InputMouseButtonEvent::TMouseButton eButton)
{
    if (eButton == InputMouseButtonEvent::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = false;

	MainApp::GetInstance()->GetGraphic()->UnHideAndUnLockMouse();
    }
}

DummyGame::DummyGame()
{
    m_bStop = false;
    m_bLeftMouseButtonDown = false;

    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, "input.key");
    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, "input.mouse.button");
    MainApp::GetInstance()->GetEventManager()->RegisterEventListener(this, "input.mouse.move");
}

bool DummyGame::OnEvent(std::shared_ptr<EventManager::IEvent> spEvent)
{
    if (spEvent->GetName() == "input.key")
    {
	InputKeyEvent *pInputKeyEvent = dynamic_cast<InputKeyEvent *>(&(*spEvent));

	assert (pInputKeyEvent != NULL);

	InputKeyEvent::TKey eKey = pInputKeyEvent->GetKey();
	InputKeyEvent::TEvent eEvent = pInputKeyEvent->GetEvent();

	if (eEvent == InputKeyEvent::EVENT_UP)
	    ItlOnKeyUp(eKey);
	else if (eEvent == InputKeyEvent::EVENT_DOWN)
	    ItlOnKeyDown(eKey);
	else
	    assert (!"no if condition fired");
    }
    else if (spEvent->GetName() == "input.mouse.button")
    {
	InputMouseButtonEvent *pInputMouseButtonEvent = dynamic_cast<InputMouseButtonEvent *>(&(*spEvent));

	assert (pInputMouseButtonEvent != NULL);

	InputMouseButtonEvent::TMouseButton eButton = pInputMouseButtonEvent->GetMouseButton();
	InputMouseButtonEvent::TEvent eEvent = pInputMouseButtonEvent->GetEvent();

	if (eEvent == InputMouseButtonEvent::EVENT_UP)
	    OnMouseButtonReleased(eButton);
	else
	    OnMouseButtonPressed(eButton);
    }
    else if (spEvent->GetName() == "input.mouse.move")
    {
	InputMouseMoveEvent *pInputMouseMoveEvent = dynamic_cast<InputMouseMoveEvent *>(&(*spEvent));

	assert (pInputMouseMoveEvent != NULL);

	int iRelX = pInputMouseMoveEvent->GetX();
	int iRelY = pInputMouseMoveEvent->GetY();

	OnMouseMove(iRelX, iRelY);
    }
}
