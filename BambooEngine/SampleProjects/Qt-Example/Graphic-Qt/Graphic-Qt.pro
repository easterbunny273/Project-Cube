#-------------------------------------------------
#
# Project created by QtCreator 2011-11-01T09:36:12
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Graphic-Qt
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_LFLAGS += -lglfw -lGLEW -lIL -lassimp

SOURCES += main.cpp\
        MainWindow.cpp \
    src/Graphics/TextureManager.cpp \
    src/Graphics/ShaderManager.cpp \
    src/Graphics/Shader.cpp \
    src/Graphics/RenderNode.cpp \
    src/Graphics/Graphic.cpp \
    src/Graphics/Graphic-QtWidgetWrapper.cpp \
    src/Graphics/SceneObjects/LoadedModel.cpp \
    src/Graphics/SceneObjects/Cube.cpp \
    src/Graphics/RenderNodes/RenderNode_RenderPass.cpp \
    src/Graphics/RenderNodes/RenderNode_PostEffect.cpp \
    src/Graphics/RenderNodes/RenderNode_FBO.cpp \
    src/Graphics/RenderNodes/RenderNode_Cube.cpp \
    src/Graphics/RenderNodes/RenderNode_Camera.cpp \
    src/Graphics/RenderNodes/RenderNode_BoundingBox.cpp \
    src/Graphics/RenderNodes/RenderNode_AssimpImport.cpp \
    Logger.cpp \
    src/Graphics/Graphic-GlfwWindow.cpp

INCLUDEPATH += include/

HEADERS  += MainWindow.h \
    include/Graphics/TextureManager.h \
    include/Graphics/ShaderManager.h \
    include/Graphics/Shader.h \
    include/Graphics/prevent_old_opengl.h \
    include/Graphics/Graphic.h \
    include/Graphics/Graphic-QtWidgetWrapper.h \
    include/Graphics/Graphic-GlfwWindow.h \
    include/Graphics/common_gl.h \
    include/Graphics/RenderNodes/RenderNode_RenderPass.h \
    include/Graphics/RenderNodes/RenderNode_PostEffect.h \
    include/Graphics/RenderNodes/RenderNode_FBO.h \
    include/Graphics/RenderNodes/RenderNode_Cube.h \
    include/Graphics/RenderNodes/RenderNode_Camera.h \
    include/Graphics/RenderNodes/RenderNode_BoundingBox.h \
    include/Graphics/RenderNodes/RenderNode_AssimpImport.h \
    include/Graphics/RenderNodes/IRenderNode.h \
    include/Graphics/SceneObjects/LoadedModel.h \
    include/Graphics/SceneObjects/ISceneObject.h \
    include/Graphics/SceneObjects/Cube.h \
    Logger.h

FORMS    += MainWindow.ui









unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += glew IL

unix:!macx:!symbian: LIBS += -lassimp

