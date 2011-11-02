#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Graphics/Graphic.h"
#include "Graphics/Graphic-QtWidgetWrapper.h"
#include "Graphics/SceneObjects/LoadedModel.h"
#include "Graphics/SceneObjects/Cube.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pBamboo = new Bamboo();

    m_spGLWidget = Bamboo::QtWidgetWrapper::Create(ui->widget);
    m_spGLWidget2 = Bamboo::QtWidgetWrapper::Create(ui->widget_2);


    // test

    std::shared_ptr<Bamboo::Camera> spCamera1(new Bamboo::Camera());
    // set perspective projection
    spCamera1->SetPerspectiveProjection(45.0f, 1.33f, 0.01f, 100.0f);

    std::shared_ptr<Bamboo::Camera> spCamera2(new Bamboo::Camera());
    // set perspective projection
    spCamera2->SetPerspectiveProjection(120.0f, 1.33f, 0.01f, 100.0f);

    // create scene
    std::shared_ptr<Bamboo::Scene> spScene1 = Bamboo::Scene::Create(spCamera1);
    std::shared_ptr<Bamboo::Scene> spScene2 = Bamboo::Scene::Create(spCamera2);

    // workaround - triggers initializing of graphic engine
    // todo: fix this.
    m_pBamboo = new Bamboo();

    // create objects
    std::shared_ptr<Bamboo::ISceneObject> spCube = Bamboo::SO_Cube::Create();
    std::shared_ptr<Bamboo::ISceneObject> spTable = Bamboo::SO_LoadedModel::Create("models/freepool-ng-table.3ds");

    std::shared_ptr<Bamboo::ISceneObject> spCube2 = Bamboo::SO_Cube::Create();
    std::shared_ptr<Bamboo::ISceneObject> spTable2 = Bamboo::SO_LoadedModel::Create("models/freepool-ng-table.3ds");
    //spTable->SetTransformMatrix(glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01)));

    // add objects to scene
    spScene1->AttachObject(spCube);
    spScene1->AttachObject(spTable);

    spScene2->AttachObject(spCube2);
    spScene2->AttachObject(spTable2);

    // add render loop
    m_pBamboo->AddRenderLoop(m_spGLWidget, spCamera1, spScene1);
   // m_pBamboo->AddRenderLoop(m_spGLWidget2, spCamera2, spScene2);

    QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
     timer->start(20);
}

MainWindow::~MainWindow()
{
    delete m_pBamboo;
    delete ui;
}

void MainWindow::updateGL()
{
    m_pBamboo->Render();
}
