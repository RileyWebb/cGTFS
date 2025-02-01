#include <stdio.h>

#include <SDL2/SDL.h>
#define SDL_MAIN_HANDLED
#include "gtfs.h"

#include "ui.h"
#include "debug.h"



int main(int argc, char *argv[])
{
    FILE *log_fp = fopen("log.txt", "w");
    debug_register(log_fp);
/*
    FILE *fp = _wfopen(L"data/shapes.txt", L"r, ccs=UTF-8");
    if (!fp) {
        wprintf(L"Error: Unable to open file\n");
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);// / sizeof(wchar_t);  // Size in wide characters
    rewind(fp);

    // Allocate memory for the file content + null terminator
    wchar_t *content = malloc((file_size + 1) * sizeof(wchar_t));
    if (!content) {
        wprintf(L"Error: Memory allocation failed\n");
        fclose(fp);
    }

    // Read file into buffer
    fread(content, sizeof(wchar_t), file_size, fp);
    content[file_size] = L'\0';  // Null-terminate the string

    fclose(fp);

    gtfs_schedule_t *schedule = calloc(1, sizeof(gtfs_schedule_t));
    DEBUG_INFO("START");
    gtfs_parse_shapes(schedule, content);
    DEBUG_INFO("END");

    free(content);
*/
    //gtfs_load_schedule(L"file:///data/providers/vicroads/google_transit.zip");

    UI_Init();
    ui_running = 1;
    while (ui_running)
    {
        UI_Update();
    }
    //UI_Exit();

    debug_flush();
    /*
    char *source = NULL;
    FILE *fp = fopen("data/shapes.txt", "r");
    if (fp != NULL) {

        if (fseek(fp, 0L, SEEK_END) == 0) {
            long bufsize = ftell(fp);
            if (bufsize == -1) {  }

            source = malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0) {}

            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0';
            }
        }
        fclose(fp);
    }

    gtfs_schedule_t schedule = {0};
    gtfs_parse_shapes(&schedule, source);

    free(source);

    printf("%zu", schedule.num_shapes);


    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480,
            //| SDL_WINDOW_VULKAN
            SDL_WINDOW_OPENGL );//| SDL_WINDOW_HIDDEN);


    //SDL_GL_GetDrawableSize(window, &c_windowWidth, &c_windowHeight);

    //SDL_SetWindowFullscreen(window, VAR_GetInt("fullscreen") ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    //SDL_GL_SetSwapInterval(VAR_GetInt("vsync"));

    //SDL_SetWindowGrab(window, 1);

    //SDL_RaiseWindow(window);

    glContext = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {}
        //DEBUG_FATAL("Unable to initialize OpenGL");

    igCreateContext(NULL);
    ImPlot_CreateContext();

    ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    igStyleColorsDark(NULL);

    while (1)
    {

        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
                case SDL_QUIT:
                    exit(0);
                    //G_Quit();
                    break;
                case SDL_WINDOWEVENT:
                    switch (e.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            SDL_GL_GetDrawableSize(window, &c_windowWidth, &c_windowHeight);
                            //R_ResizeViewport(c_windowWidth, c_windowHeight);
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    //C_InputProcessEvent(&e);
                    break;
            }
            ImGui_ImplSDL2_ProcessEvent(&e);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        igNewFrame();


        //igShowDemoWindow(&c_uiActive);

        igSetNextWindowSize((ImVec2 ){1000, 600}, ImGuiCond_FirstUseEver);
        igBegin("Test", &c_uiActive, 0);//ImGuiWindowFlags_DockNodeHost | ImGuiWindowFlags_AlwaysAutoResize);
        //ImDrawList_AddPolyline()
        //igText("asdasdadadasd");

        ImPlot_BeginPlot("Test", (ImVec2 ){800, 480}, ImPlotFlags_CanvasOnly);

        ImPlot_SetupAxes(NULL,NULL, 0,0);//ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);//ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
        ImPlot_SetupAxesLimits(-37,-39,143,145, ImPlotCond_Once);


        for (int i = 0; i < schedule.num_shapes; ++i)
        {
            float xpoints[schedule.shapes[i].num_points];
            float ypoints[schedule.shapes[i].num_points];

            for (int j = 0; j < schedule.shapes[i].num_points; ++j)
            {
                xpoints[j] = schedule.shapes[i].points[j].lat;
                ypoints[j] = schedule.shapes[i].points[j].lon;
            }

            ImPlot_SetNextMarkerStyle(1, 1, (ImVec4){1,0,0,1}, 4, (ImVec4){1,1,1,1});
            ImPlot_SetNextLineStyle((ImVec4){1,1,1,1},1);
            //ImPlot_PlotLine_FloatPtrFloatPtr("##Filled", &schedule.shapes[0].points[0].lat, &schedule.shapes[0].points[0].lon,schedule.shapes->num_points, 0, 0, );//sizeof(gtfs_point_t));
            ImPlot_PlotLine_FloatPtrFloatPtr("##Filled", &xpoints, &ypoints, schedule.shapes[i].num_points, 0, 0, sizeof(float));//sizeof(gtfs_point_t));
        }
            //igPushID_Int(i);

            //igPopID();



        ImPlot_EndPlot();
        igEnd();
        //D_UIDraw();

        igRender();
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

#ifdef IMGUI_HAS_DOCK
        if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            igUpdatePlatformWindows();
            igRenderPlatformWindowsDefault(NULL,NULL);
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
#endif

        SDL_GL_SwapWindow(window);
    }
    */

    return 0;
}
