#include "graphics.h"

Graphics::Graphics() {

}

Graphics::~Graphics() {

}

bool Graphics::Initialize(int width, int height) {
    // Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK) {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
#endif

    // For OpenGL 3
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Init Camera
    m_camera = new Camera();
    if (!m_camera->Initialize(width, height)) {
        printf("Camera Failed to Initialize\n");
        return false;
    }

    // Create the object
    m_board = new Object("../objects/chessboard.obj");

    //load textures Note~ come back to this
//    TextureManager::getInstance()->loadTexture((string &) "chessboard", "../textures/chessboard-super-small-texture.png");

    // Set up the shaders
    m_shader = new Shader();
    if (!m_shader->initialize()) {
        printf("Shader Failed to Initialize\n");
        return false;
    }

    // Add the vertex shader
    if (!m_shader->addShaderFromFile("../shaders/lighting.vs", GL_VERTEX_SHADER)) {
        printf("Vertex Shader failed to Initialize\n");
        return false;
    }

    // Add the fragment shader
    if (!m_shader->addShaderFromFile("../shaders/lighting.fs", GL_FRAGMENT_SHADER)) {
        printf("Fragment Shader failed to Initialize\n");
        return false;
    }

    // Connect the program
    if (!m_shader->finalize()) {
        printf("Program to Finalize\n");
        return false;
    }

    // Locate the projection matrix in the shader
    m_projectionMatrix = m_shader->getUniformLocation("projectionMatrix");
    if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
        printf("m_projectionMatrix not found\n");
        return false;
    }

    // Locate the view matrix in the shader
    m_viewMatrix = m_shader->getUniformLocation("viewMatrix");
    if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
        printf("m_viewMatrix not found\n");
        return false;
    }

    // Locate the model matrix in the shader
    m_modelMatrix = m_shader->getUniformLocation("modelMatrix");
    if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
        printf("m_modelMatrix not found\n");
        return false;
    }

    lightingModel = new LightingModel();

    if (!lightingModel->initialize((*m_shader))) {
        printf("lighting model to Initialize\n");
        return false;
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

void Graphics::Update(unsigned int dt) {
    // Update the object
    m_board->Update(dt);
}

void Graphics::Render() {
    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the correct program
    m_shader->enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_board->GetModel()));

    //Add lighting
    lightingModel->renderLighting();

    //render models
    m_board->Render();

    // Get any errors from OpenGL
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        string val = ErrorString(error);
        std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
}

void Graphics::cameraInput(SDL_Keycode input) {
    switch (input) {
        case SDLK_UP:
            m_camera->updatePosition(UP);
            break;
        case SDLK_DOWN:
            m_camera->updatePosition(DOWN);
            break;
        case SDLK_LEFT:
            m_camera->updatePosition(LEFT);
            break;
        case SDLK_RIGHT:
            m_camera->updatePosition(RIGHT);
            break;
        case SDLK_w:
            m_camera->updateDirection(UP);
            break;
        case SDLK_s:
            m_camera->updateDirection(DOWN);
            break;
        case SDLK_d:
            m_camera->updateDirection(RIGHT);
            break;
        case SDLK_a:
            m_camera->updateDirection(LEFT);
            break;
        default:
            cout << input << " not mapped to camera update" << endl;
            break;
    }
}

std::string Graphics::ErrorString(GLenum error) {
    if (error == GL_INVALID_ENUM) {
        return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
    } else if (error == GL_INVALID_VALUE) {
        return "GL_INVALID_VALUE: A numeric argument is out of range.";
    } else if (error == GL_INVALID_OPERATION) {
        return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
    } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
        return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    } else if (error == GL_OUT_OF_MEMORY) {
        return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
    } else {
        return "None";
    }
}

