#include <android_native_app_glue.h>
#include "revApp.h"
#include "AndroidContext.h"

android_app* appContext = nullptr;

void handle_cmd(android_app* app, int32_t cmd) {
    Window window(app->window);
    switch (cmd) {
      case APP_CMD_INIT_WINDOW:
            revApp::Get().StartUp(&window);
            break;
        case APP_CMD_TERM_WINDOW:
            revApp::Get().ShutDown();
            break;
        default:
            break;
    }
}


void android_main(struct android_app* state)
{
    state->onAppCmd = handle_cmd;
    appContext = state;

    revApp& app = revApp::Get();
    int events;
    android_poll_source* source;
    do {
        if (ALooper_pollAll(app.isInitialized() ? 1 : 0, nullptr, &events,
                            (void**)&source) >= 0) {
            if (source != NULL) source->process(state, source);
        }
        if(app.isInitialized()) {
            app.Run();
        }
    } while (state->destroyRequested == 0);
}
