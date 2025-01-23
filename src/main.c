#include <stdio.h>
#include <SDL2/SDL.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>
#include <cimplot.h>
#include "GL/glew.h"

#include "gtfs.h"

#define SDL_MAIN_HANDLED

SDL_Window *window;
SDL_GLContext *glContext;

int c_windowWidth;
int c_windowHeight;

ImGuiIO* ioptr;

float pointsx[] = {-37.416861f, -37.70335908f, -37.72919081f, -37.77726729f, -37.57914973f, -37.7830262f, -37.60227314f, -37.81838925f, -36.08426161f, -37.28220485f, -36.89364778f, -37.68782809f, -37.82877565f, -37.60429951f, -37.55865829f, -37.42787497f, -36.54455031f, -36.76560312f, -38.32880781f, -37.20731649f, -38.09910771f, -38.22890091f, -37.0628859f, -36.15563663f, -37.48355944f, -38.34337792f, -38.07283126f, -36.36923291f, -37.5420263f, -38.13637147f, -36.71852539f, -36.131043f, -36.49483741f, -36.7491306f, -38.09103062f, -38.14424211f, -37.45900212f, -37.37166409f, -36.79483286f, -35.73312162f, -37.29365111f, -37.25836427f, -38.02242764f, -37.96292909f, -38.11099211f, -37.42366057f, -37.18977634f, -38.198549f, -38.17690856f, -36.39912828f, -38.23671876f, -36.61314794f, -36.7854641f, -38.08150256f, -38.12283523f, -38.09837714f, -36.05311274f, -37.46490085f, -36.36229401f, -38.15640367f, -38.10310272f, -37.02439853f, -36.38381106f, -38.15865863f, -36.1858934f, -37.96704739f, -35.34111357f, -37.0923313f, -38.23621199f, -38.2072423f, -38.19854647f, -38.08488125f, -36.63881606f, -37.35524821f, -36.35510089f, -38.16478926f, -38.38501396f, -38.24013506f, -36.1058271f, -37.35929126f, -38.20315754f, -37.81830513f, -37.80755595f, -37.80157779f, -37.78833946f, -37.70097389f, -37.82407446f, -37.87722507f, -37.92456781f, -37.98996792f, -38.0395774f, -38.08061383f, -37.75600088f, -37.68295879f, -38.38639224f, -37.53973228f, -37.42460539f, -37.30273815f, -37.05090849f, -37.17184115f, -38.21595887f, -36.70675355f, -37.87288647f, -37.83216813f, -37.71252223f, -36.53420922f, -36.66639019f, -36.61480046f, -37.76609003f};
float pointsy[] = {145.0053723f,144.5722158f,144.6507131f,144.7708312f,144.7280304f,144.801542f,144.9431265f,144.9520576f,146.9245154f,142.9369137f,145.229515f,144.4375023f,147.6273983f,144.225448f,143.8595309f,143.3822337f,145.9839151f,144.2828545f,143.7836252f,145.0429373f,145.7207575f,143.1509278f,144.2139434f,146.6113748f,144.7453029f,143.5866531f,144.3797684f,144.2311203f,144.9701244f,145.8560167f,144.2483799f,144.7533347f,144.6076404f,145.5680886f,145.6742247f,144.3549891f,144.5989872f,145.0279465f,144.2489566f,143.9244251f,144.9835204f,144.4508016f,144.4144043f,144.4984849f,145.7668603f,144.5612584f,144.3753219f,144.3550567f,146.2605716f,145.3582148f,146.396753f,145.2406356f,145.1603567f,145.5717587f,144.3522719f,144.3653214f,144.1131327f,144.6799507f,144.698505f,146.7869664f,147.0548051f,145.1385123f,145.4059731f,144.358987f,146.470417f,147.0814694f,143.5623432f,145.1027948f,142.9114722f,146.1547728f,146.5389007f,145.6284189f,145.7159234f,145.026615f,146.3170385f,145.932829f,142.475545f,143.9841339f,146.8712657f,144.5261788f,146.0630628f,144.9669643f,144.9421684f,144.9021403f,144.8324638f,144.7740944f,144.9901643f,145.0425196f,145.1206852f,145.2097254f,145.3449585f,145.4863677f,144.9160957f,144.9195931f,142.5388712f,143.8199989f,143.8881236f,143.7820111f,143.7424023f,143.706148f,144.3064372f,144.3209245f,144.6087319f,144.6947144f,144.6038477f,144.2014149f,144.3682582f,144.5045598f,144.7357156f};

int c_uiActive = 1;

int main(int argc, char *argv[])
{

    char *source = NULL;
    FILE *fp = fopen("data/shapes.txt", "r");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
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

    return 0;
}
