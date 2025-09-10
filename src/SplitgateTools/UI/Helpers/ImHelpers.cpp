#include "ImHelpers.h"
#include "Math/UnrealMathUtility.h"

bool ImGui::InputIntClamped(const char* label, int* v, int min, int max, int step, int step_fast, ImGuiInputTextFlags flags)
{
    bool ReturnValue = ImGui::InputInt(label, v, step, step_fast, flags);
    if (v && !(min != 0 && max != 0))
    {
        *v = FMath::Clamp(*v, min, max);
    }

    return ReturnValue;
}