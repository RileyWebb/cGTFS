#include <SDL2/SDL.h>
#include <GL/glew.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>
#include <cimplot.h>

#include "ui.h"
#include "gtfs.h"

SDL_Window *window;
SDL_GLContext *glContext;

ImGuiIO* ioptr;

gtfs_schedule_t schedule;

void UI_Init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "cGTFS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480,
            //| SDL_WINDOW_VULKAN
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);//| SDL_WINDOW_HIDDEN);


    //SDL_GL_GetDrawableSize(window, &c_windowWidth, &c_windowHeight);

    //SDL_SetWindowFullscreen(window, VAR_GetInt("fullscreen") ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    //SDL_GL_SetSwapInterval(VAR_GetInt("vsync"));

    //SDL_SetWindowGrab(window, 1);

    //SDL_RaiseWindow(window);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetWindowMinimumSize(window, 200, 200);
    SDL_GL_SetSwapInterval(1);

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

    //debug_registercallback();
    ui_logEnabled = 0;

    {
        FILE *fp = _wfopen(L"data/shapes.txt", L"r, ccs=UTF-8");
        if (!fp)
        {
            wprintf(L"Error: Unable to open file\n");
        }

        // Get file size
        fseek(fp, 0, SEEK_END);
        size_t file_size = ftell(fp);// / sizeof(wchar_t);  // Size in wide characters
        rewind(fp);

        // Allocate memory for the file content + null terminator
        wchar_t *content = malloc((file_size + 1) * sizeof(wchar_t));
        if (!content)
        {
            wprintf(L"Error: Memory allocation failed\n");
            fclose(fp);
        }

        // Read file into buffer
        fread(content, sizeof(wchar_t), file_size, fp);
        content[file_size] = L'\0';  // Null-terminate the string

        fclose(fp);

        //schedule = calloc(1, sizeof(gtfs_schedule_t));

        gtfs_parse_shapes(&schedule, content);

        free(content);
    }
    {
        FILE *fp = _wfopen(L"data/stops.txt", L"r, ccs=UTF-8");
        if (!fp)
        {
            wprintf(L"Error: Unable to open file\n");
        }

        // Get file size
        fseek(fp, 0, SEEK_END);
        size_t file_size = ftell(fp);// / sizeof(wchar_t);  // Size in wide characters
        rewind(fp);

        // Allocate memory for the file content + null terminator
        wchar_t *content = malloc((file_size + 1) * sizeof(wchar_t));
        if (!content)
        {
            wprintf(L"Error: Memory allocation failed\n");
            fclose(fp);
        }

        // Read file into buffer
        fread(content, sizeof(wchar_t), file_size, fp);
        content[file_size] = L'\0';  // Null-terminate the string

        fclose(fp);

        //schedule = calloc(1, sizeof(gtfs_schedule_t));

        gtfs_parse_stops(&schedule, content);

        free(content);
    }
}

bool ui_running;

void UI_Draw()
{
    ImGuiViewport* viewport = igGetMainViewport();
    igSetNextWindowPos(viewport->Pos, ImGuiCond_Always, (ImVec2){0,0});
    igSetNextWindowSize(viewport->Size, ImGuiCond_Always);
    igSetNextWindowViewport(viewport->ID);

    igBegin("##main", &ui_running, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                   ImGuiWindowFlags_NoResize);
    igEnd();
}

void UI_DrawGTFS()
{
    igBegin("GTFS Viewer", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);
    ImVec2 size;
    igGetWindowSize(&size);
    //ImGuiID dock = igGetID_Str("##main");
    igDockBuilderDockWindow("##main",igGetActiveID() );
    ImPlot_BeginPlot("Test", size, ImPlotFlags_NoTitle);//ImPlotFlags_CanvasOnly);

    ImPlot_SetupAxes(NULL,NULL, 0,0);//ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);//ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
    ImPlot_SetupAxesLimits(-37,-39,143,145, ImPlotCond_Once);

    for (int i = 0; i < schedule.num_shapes; ++i)
    {
        double xpoints[schedule.shapes[i].num_points];
        double ypoints[schedule.shapes[i].num_points];

        for (int j = 0; j < schedule.shapes[i].num_points; ++j)
        {
            xpoints[j] = schedule.shapes[i].points[j].lat;
            ypoints[j] = schedule.shapes[i].points[j].lon;
        }
        ImVec4 col;
        ImPlot_GetAutoColor(&col, i);
        ImPlot_SetNextMarkerStyle(1, 1, (ImVec4){1,0,0,1}, 4, (ImVec4){1,1,1,1});
        ImPlot_SetNextLineStyle(col,1);
        ImPlot_PlotLine_doublePtrdoublePtr("##Filled", &xpoints, &ypoints, schedule.shapes[i].num_points, 0, 0, sizeof(double));
        //ImPlot_PlotLine_FloatPtrFloatPtr("##Filled", &schedule.shapes[0].points[0].lat, &schedule.shapes[0].points[0].lon,schedule.shapes->num_points, 0, 0, );//sizeof(gtfs_point_t));
    }

    for (int i = 0; i < schedule.num_stops; ++i)
    {
        char name[wcslen(schedule.stops[i].name) + 1];
        wcstombs(&name, schedule.stops[i].name, wcslen(schedule.stops[i].name));
        ImPlot_PlotText(name, schedule.stops[i].lat, schedule.stops[i].lon, (ImVec2){0,0}, ImPlotTextFlags_None);
    }
    //igPushID_Int(i);

    //igPopID();



    ImPlot_EndPlot();
    igEnd();
}

void UI_Update()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
            case SDL_QUIT:
                //exit(0);
                //G_Quit();
                break;
            case SDL_WINDOWEVENT:
                switch (e.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        //SDL_GL_GetDrawableSize(window, &c_windowWidth, &c_windowHeight);
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

    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();

    //igShowDemoWindow((bool *) &ui_running);

    UI_Draw();
    UI_DrawGTFS();
    UI_DrawLog();

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        igUpdatePlatformWindows();
        igRenderPlatformWindowsDefault(NULL,NULL);
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(window);
}

void UI_Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}