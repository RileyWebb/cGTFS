#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>
#include <cimplot.h>

ImGuiTextBuffer buffer;
ImGuiTextFilter filter;
ImVector_int lineOffsets;
bool autoScroll;

bool ui_logEnabled = 0;

void UI_DrawLog()
{
    //REF: ImGui Examples
    if (!igBegin("Logs", &ui_logEnabled, 0))
    {
        igEnd();
        return;
    }

    if (igBeginPopup("Options", 0))
    {
        igCheckbox("Auto-scroll", &autoScroll);
        igEndPopup();
    }

    if (igButton("Options", (ImVec2){20,10}))
        igOpenPopup_Str("Options", 0);
    igSameLine(1,1);
    bool clear = igButton("Clear", (ImVec2){20,10});
    igSameLine(1,1);
    bool copy = igButton("Copy", (ImVec2){20,10});
    igSameLine(1,1);
    ImGuiTextFilter_Draw(&filter, "Filter", -100.0f);

    igSeparator();



    igEnd();
}
