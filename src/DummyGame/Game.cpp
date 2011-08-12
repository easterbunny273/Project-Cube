#include "DummyGame/Game.h"
#include "DummyGame/SampleObject.h"

#include "MainApp.h"
#include "Logger.h"

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

void DummyGame::OnKeyDown(IInputEventListener::TKey eKey)
{
    MainApp *pApp = MainApp::GetInstance();

    assert (pApp != NULL);

    Graphic *pGraphic = pApp->GetGraphic();

    assert (pGraphic != NULL);

    Graphic::Camera *pCamera = pGraphic->GetCamera();

    assert (pCamera != NULL);

    if (eKey == 'W')
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, 1.0));
    else if (eKey == 'S')
	pCamera->AddToMoveVector(glm::vec3(0.0, 0.0, -1.0));
    else if (eKey == 'A')
	pCamera->AddToMoveVector(glm::vec3(-1.0, 0.0, 0.0));
    else if (eKey == 'D')
	pCamera->AddToMoveVector(glm::vec3(1.0, 0.0, 0.0));
    else if (eKey == IInputEventListener::KEY_LCTRL)
	pCamera->AddToMoveVector(glm::vec3(0.0, -1.0, 0.0));
    else if (eKey == IInputEventListener::KEY_SPACE)
	pCamera->AddToMoveVector(glm::vec3(0.0, 1.0, 0.0));

}

void DummyGame::OnKeyUp(IInputEventListener::TKey eKey)
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
    else if (eKey == IInputEventListener::KEY_LCTRL)
	pCamera->AddToMoveVector(glm::vec3(0.0, 1.0, 0.0));
    else if (eKey == IInputEventListener::KEY_SPACE)
	pCamera->AddToMoveVector(glm::vec3(0.0, -1.0, 0.0));
    else if (eKey == IInputEventListener::KEY_ESC)
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

void DummyGame::OnMouseButtonPressed(IInputEventListener::TMouseButton eButton)
{
    if (eButton == IInputEventListener::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = true;

	// hide mouse and lock it to window center
	// input events will now report the relative position to the window center
	MainApp::GetInstance()->GetGraphic()->HideAndLockMouseToWindowCenter();
    }
}

void DummyGame::OnMouseButtonReleased(IInputEventListener::TMouseButton eButton)
{
    if (eButton == IInputEventListener::BUTTON_LEFT)
    {
	m_bLeftMouseButtonDown = false;

	MainApp::GetInstance()->GetGraphic()->UnHideAndUnLockMouse();
    }
}

DummyGame::DummyGame()
{
    m_bStop = false;
}
