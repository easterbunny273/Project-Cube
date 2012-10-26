#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T21:18:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT		 += opengl

TARGET = Project-Cube-Qt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += BambooEngine/include/
INCLUDEPATH += BambooLib/include/
INCLUDEPATH += include/
INCLUDEPATH += include/tinyxml

LIBS += -lGLEW -lIL -lglfw -lassimp

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += BambooEngine/src/TextureManager.cpp \
    BambooEngine/src/ShaderManager.cpp \
    BambooEngine/src/RenderNode.cpp \
    BambooEngine/src/Graphic.cpp \
    BambooEngine/src/Graphic-QtWidgetWrapper.cpp \
    BambooEngine/src/Graphic-GlfwWindow.cpp \
    BambooEngine/src/GeometryData.cpp \
    BambooEngine/src/Camera.cpp \
    BambooEngine/src/AssimpWrapper.cpp \
    BambooEngine/src/SemanticSceneNodes/LoadedModel_SemSceneNode.cpp \
    BambooEngine/src/SemanticSceneNodes/Light_SemSceneNode.cpp \
    BambooEngine/src/SemanticSceneNodes/Cube_SemSceneNode.cpp \
    BambooEngine/src/SemanticSceneNodes/Camera_SemSceneNode.cpp \
    BambooEngine/src/RenderNodes/RenderNode_SpotLight_Model.cpp \
    BambooEngine/src/RenderNodes/RenderNode_SpotLight.cpp \
    BambooEngine/src/RenderNodes/RenderNode_RenderPass.cpp \
    BambooEngine/src/RenderNodes/RenderNode_PostEffect.cpp \
    BambooEngine/src/RenderNodes/RenderNode_Generic.cpp \
    BambooEngine/src/RenderNodes/RenderNode_FBO.cpp \
    BambooEngine/src/RenderNodes/RenderNode_Deferred.cpp \
    BambooEngine/src/RenderNodes/RenderNode_CubeMap.cpp \
    BambooEngine/src/RenderNodes/RenderNode_Camera.cpp \
    BambooEngine/src/RenderNodes/RenderNode_BoundingBox.cpp \
    BambooEngine/src/DeferredNodeTranslator/LoadedModel_RuleObject.cpp \
    BambooEngine/src/DeferredNodeTranslator/Light_RuleObject.cpp \
    BambooEngine/src/DeferredNodeTranslator/DeferredNodeTranslator.cpp \
    BambooEngine/src/DeferredNodeTranslator/Cube_RuleObject.cpp \
    BambooEngine/src/DeferredNodeTranslator/Camera_RuleObject.cpp \
    BambooLib/src/Logger.cpp \
    BambooLib/src/CoreSystem.cpp \
    BambooLib/src/BinarySerializer.cpp \
    src/MainApp.cpp \
    src/Main.cpp \
    src/Events.cpp \
    src/EventManager.cpp \
    src/Settings.cpp \
    src/ProcessManager.cpp \
    src/Gamelogic/Level.cpp \
    src/Gamelogic/Grid.cpp \
    src/Gamelogic/Cube.cpp \
    src/Gamelogic/Objects/Object.cpp \
    src/Gamelogic/Objects/LightObject.cpp \
    src/DummyGame/SampleObject.cpp \
    src/DummyGame/Game.cpp \
    include/tinyxml/tinyxmlparser.cpp \
    include/tinyxml/tinyxmlerror.cpp \
    include/tinyxml/tinyxml.cpp \
    include/tinyxml/tinystr.cpp \
    src/gui/MainWindow.cpp

HEADERS += \
    BambooEngine/include/TextureManager.h \
    BambooEngine/include/ShaderManager.h \
    BambooEngine/include/prevent_old_opengl.h \
    BambooEngine/include/INodeTranslator.h \
    BambooEngine/include/Graphic.h \
    BambooEngine/include/Graphic-QtWidgetWrapper.h \
    BambooEngine/include/Graphic-GlfwWindow.h \
    BambooEngine/include/GeometryData.h \
    BambooEngine/include/common_gl.h \
    BambooEngine/include/Camera.h \
	BambooEngine/include/KeyIdentifiers.h \
    BambooEngine/include/AssimpWrapper.h \
    BambooEngine/include/SemanticSceneNodes/LoadedModel_SemSceneNode.h \
    BambooEngine/include/SemanticSceneNodes/Light_SemSceneNode.h \
    BambooEngine/include/SemanticSceneNodes/ISemanticSceneNode.h \
    BambooEngine/include/SemanticSceneNodes/Cube_SemSceneNode.h \
    BambooEngine/include/SemanticSceneNodes/Camera_SemSceneNode.h \
    BambooEngine/include/RenderNodes/RenderNode_SpotLight.h \
    BambooEngine/include/RenderNodes/RenderNode_Camera.h \
    BambooEngine/include/RenderNodes/RenderNode_BoundingBox.h \
    BambooEngine/include/RenderNodes/IRenderNode.h \
    BambooEngine/include/RenderNodes/RenderNode_SpotLight_Model.h \
    BambooEngine/include/RenderNodes/RenderNode_RenderPass.h \
    BambooEngine/include/RenderNodes/RenderNode_PostEffect.h \
    BambooEngine/include/RenderNodes/RenderNode_Generic.h \
    BambooEngine/include/RenderNodes/RenderNode_FBO.h \
    BambooEngine/include/RenderNodes/RenderNode_Deferred.h \
    BambooEngine/include/RenderNodes/RenderNode_CubeMap.h \
    BambooEngine/include/DeferredNodeTranslator/LoadedModel_RuleObject.h \
    BambooEngine/include/DeferredNodeTranslator/Light_RuleObject.h \
    BambooEngine/include/DeferredNodeTranslator/DeferredNodeTranslator.h \
    BambooEngine/include/DeferredNodeTranslator/Cube_RuleObject.h \
    BambooEngine/include/DeferredNodeTranslator/Camera_RuleObject.h \
    BambooLib/include/Logger.h \
    BambooLib/include/IStreamable.h \
    BambooLib/include/ISerializer.h \
    BambooLib/include/IIdentifyable.h \
    BambooLib/include/GeneralDefinitions.h \
    BambooLib/include/CoreSystem.h \
    BambooLib/include/BinarySerializer.h \
    include/MainApp.h \
    include/IXMLSerializeable.h \
    include/Events.h \
    include/EventManager.h \
    include/Error.h \
    include/Settings.h \
    include/ProcessManager.h \
    include/Gamelogic/Level.h \
    include/Gamelogic/IObject.h \
    include/Gamelogic/ILevel.h \
    include/Gamelogic/IGameState.h \
    include/Gamelogic/ICube.h \
    include/Gamelogic/Grid.h \
    include/Gamelogic/GameLogic.h \
    include/Gamelogic/Cube.h \
    include/Gamelogic/Objects/Object.h \
    include/Gamelogic/Objects/LightObject.h \
    include/DummyGame/SampleObject.h \
    include/DummyGame/Game.h \
    include/tinyxml/tinyxml.h \
    include/tinyxml/tinystr.h \
    include/gui/MainWindow.h \
    BambooEngine/include/KeyIdentifiers.h

FORMS += \
    include/gui/MainWindow.ui
