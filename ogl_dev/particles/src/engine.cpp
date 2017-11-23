
#include "engine.h"

Engine::Engine(string name, int width, int height) {
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;
    m_FULLSCREEN = false;
}

Engine::Engine(string name) {
    m_WINDOW_NAME = name;
    m_WINDOW_HEIGHT = 0;
    m_WINDOW_WIDTH = 0;
    m_FULLSCREEN = true;
}

Engine::~Engine() {
    delete m_window;
    delete m_graphics;
    m_window = nullptr;
    m_graphics = nullptr;
}

bool Engine::Initialize() {
    // Start a window
    m_window = new Window();
    if (!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)) {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT)) {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    // Set the time
    m_currentTimeMillis = GetCurrentTimeMillis();

    // No errors
    return true;
}

void Engine::Run() {
    m_running = true;

    while (m_running) {
        // Update the DT
        m_DT = getDT();

        // Check the keyboard input
        while (SDL_PollEvent(&m_event) != 0) {
            keyboard();
        }

        // Update and render the graphics
        m_graphics->Update(m_DT);
        m_graphics->Render(m_DT);

        // Swap to the Window
        m_window->Swap();
    }
}

void Engine::keyboard() {
    if (m_event.type == SDL_QUIT) {
        m_running = false;
    } else if (m_event.type == SDL_KEYDOWN) {
        // handle key down events here
        if (m_event.key.keysym.sym == SDLK_ESCAPE) {
            m_running = false;
            return;
        }

        switch (m_event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
            case SDLK_LEFT:
            case SDLK_RIGHT:
            case SDLK_w:
            case SDLK_s:
            case SDLK_d:
            case SDLK_a:
            case SDLK_q:
            case SDLK_e:
            case SDLK_z:
            case SDLK_c:
                //camera movement
                m_graphics->cameraInput(m_event.key.keysym.sym);
                break;
            default:
                cout << m_event.key.keysym.sym << " KEY NOT USED " << endl;
                break;
        }


    }
}

unsigned int Engine::getDT() {
    long long TimeNowMillis = GetCurrentTimeMillis();
    assert(TimeNowMillis >= m_currentTimeMillis);
    auto DeltaTimeMillis = (unsigned int) (TimeNowMillis - m_currentTimeMillis);
    m_currentTimeMillis = TimeNowMillis;
    return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis() {
    timeval t;
    gettimeofday(&t, nullptr);
    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}
