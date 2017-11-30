#include "graphics.h"

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
    m_board->SetModel(glm::scale(m_board->GetModel(), glm::vec3(2, 2, 2)));

    m_chessPiece = new Object("../objects/fountain_small.obj");
    m_chessPiece->SetModel(glm::translate(m_chessPiece->GetModel(),
                                          glm::vec3(0.0, 0.0, 2.0)));

    // Set up the shaders
    m_shader = new Shader();
    if (!m_shader->initialize()) {
        printf("Shader Failed to Initialize\n");
        return false;
    }

    // Add the vertex shader
    if (!m_shader->addShaderFromFile("../shaders/lighting.vert", GL_VERTEX_SHADER)) {
        printf("Vertex Shader failed to Initialize\n");
        return false;
    }

    // Add the fragment shader
    if (!m_shader->addShaderFromFile("../shaders/lighting.frag", GL_FRAGMENT_SHADER)) {
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

    //load textures
    if (!TextureManager::getInstance()->initHandlers((*m_shader))) {
        std::cout << "Unable to get texture handlers from shader." << std::endl;
        return false;
    }

    std::string boardTexture = "../textures/green_grass.jpg";
    if (!TextureManager::getInstance()->addTexture("floor", boardTexture)) {
        std::cout << "Unable to load texture " << boardTexture << std::endl;
        return false;
    }

    std::string boardNormalMap = "../textures/grass_ground_normal.jpg";
    if (!TextureManager::getInstance()->addTexture("floorNormal", boardNormalMap)) {
        std::cout << "Unable to load texture " << boardNormalMap << std::endl;
        return false;
    }

    std::string fountainTextures = "../textures/fountain_texture.jpg";
    if (!TextureManager::getInstance()->addTexture("fountain", fountainTextures)) {
        std::cout << "Unable to load texture " << fountainTextures << std::endl;
        return false;
    }

    std::string fountainNormal = "../textures/fountain_normal.png";
    if (!TextureManager::getInstance()->addTexture("fountainNormal", fountainNormal)) {
        std::cout << "Unable to load texture " << fountainNormal << std::endl;
        return false;
    }

    m_board->setColorTextureId("floor");
    m_board->setNormalMapTextureId("floorNormal");

    m_chessPiece->setColorTextureId("fountain");
    m_chessPiece->setNormalMapTextureId("fountainNormal");

    billboardRenderer = new Billboard();
    if (!billboardRenderer->initialize("../textures/tree_spring.jpg")) {
        std::cout << "Billboard renderer failed to initialize" << std::endl;
        return false;
    }

    skyBox = new SkyBox();
    bool skyBoxInit = skyBox->init("../textures/skyboxes/Sunset/",
                                   "SunsetLeft.jpg",
                                   "SunsetRight.jpg",
                                   "SunsetUp.jpg",
                                   "SunsetDown.jpg",
                                   "SunsetBack.jpg",
                                   "SunsetFront.jpg");

    if (!skyBoxInit) {
        std::cout << "Unable to initialize sky box" << std::endl;
        return false;
    }

    lightingModel = new LightingModel();
    if (!lightingModel->initialize((*m_shader))) {
        std::cout << "lighting model failed to initialize" << std::endl;
        return false;
    }

    particleSystem = new ParticleSystem();

    if (!particleSystem->initialize("../textures/rainTexture.jpg")) {
        std::cout << "Unable to initialize particle system" << std::endl;
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
    m_chessPiece->Update(dt);
}

void Graphics::Render(unsigned int dt) {
    renderPass(dt);
    skyBoxPass();
}

void Graphics::skyBoxPass() {
    glm::mat4 projectionView = m_camera->GetProjection() * m_camera->GetView();
    skyBox->render(projectionView);
}

void Graphics::renderPass(unsigned int dt) {
    //clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render billboard
    glm::mat4 projectionView = m_camera->GetProjection() * m_camera->GetView();
    billboardRenderer->render(projectionView, m_camera->getPosition());

    // Render shader program
    m_shader->enable();

    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the board
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_board->GetModel()));
    m_board->Render();

    //Add lighting
    lightingModel->renderLighting();

    // Note~ uncomment for particle system
    glm::mat4 chessModel = m_board->GetModel() * m_chessPiece->GetModel();
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(chessModel));
    m_chessPiece->Render();

    //particle system
    particleSystem->render(dt, m_camera);



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
        case SDLK_q:
            m_camera->updatePosition(VERTICAL_POSITIVE);
            break;
        case SDLK_e:
            m_camera->updatePosition(VERTICAL_NEGATIVE);
            break;
        case SDLK_z:
            m_camera->updatePosition(HORIZONTAL_POSITIVE);
            break;
        case SDLK_c:
            m_camera->updatePosition(HORIZONTAL_NEGATIVE);
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

