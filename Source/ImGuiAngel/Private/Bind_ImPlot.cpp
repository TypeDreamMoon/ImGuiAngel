#include "CoreMinimal.h"
#include "AngelscriptBinds.h"
#include "AngelscriptManager.h"

// 引入 ImPlot
#include "implot.h"

namespace ImPlotWrappers
{
    // =========================================================================================
    // 辅助转换函数
    // =========================================================================================

    static inline ImVec2 ToImVec2(const FVector2D& V) { return ImVec2(V.X, V.Y); }
    static inline FVector2D ToFVector2D(const ImVec2& V) { return FVector2D(V.x, V.y); }
    // 新增：处理 ImPlotPoint (double精度) 到 FVector2D 的转换
    static inline FVector2D ToFVector2D(const ImPlotPoint& V) { return FVector2D(V.x, V.y); }
    static inline ImVec4 ToImVec4(const FLinearColor& V) { return ImVec4(V.R, V.G, V.B, V.A); }

    // =========================================================================================
    // Wrappers
    // =========================================================================================

    // --- Context ---
    static ImPlotContext* CreateContext() { return ImPlot::CreateContext(); }
    static void DestroyContext(ImPlotContext* Context) { ImPlot::DestroyContext(Context); }
    static ImPlotContext* GetCurrentContext() { return ImPlot::GetCurrentContext(); }
    static void SetCurrentContext(ImPlotContext* Context) { ImPlot::SetCurrentContext(Context); }

    // --- Demo ---
    static void ShowDemoWindow(bool& bIsOpen) { ImPlot::ShowDemoWindow(&bIsOpen); }

    // --- Begin/End Plot ---
    static bool BeginPlot(FString Title, FVector2D Size, int Flags) 
    { 
        return ImPlot::BeginPlot(TCHAR_TO_UTF8(*Title), ToImVec2(Size), Flags); 
    }
    static void EndPlot() { ImPlot::EndPlot(); }

    // --- Begin/End Subplots ---
    static bool BeginSubplots(FString Title, int Rows, int Cols, FVector2D Size, int Flags)
    {
        return ImPlot::BeginSubplots(TCHAR_TO_UTF8(*Title), Rows, Cols, ToImVec2(Size), Flags);
    }
    static void EndSubplots() { ImPlot::EndSubplots(); }

    // --- Setup ---
    static void SetupAxis(int Axis, FString Label, int Flags)
    {
        const char* LabelPtr = Label.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Label);
        ImPlot::SetupAxis(Axis, LabelPtr, Flags);
    }
    static void SetupAxes(FString XLabel, FString YLabel, int XFlags, int YFlags)
    {
        ImPlot::SetupAxes(TCHAR_TO_UTF8(*XLabel), TCHAR_TO_UTF8(*YLabel), XFlags, YFlags);
    }
    static void SetupLegend(int Location, int Flags) { ImPlot::SetupLegend(Location, Flags); }
    static void SetupAxisLimits(int Axis, float Min, float Max, int Cond)
    {
        ImPlot::SetupAxisLimits(Axis, (double)Min, (double)Max, Cond);
    }
    static void SetupAxesLimits(float XMin, float XMax, float YMin, float YMax, int Cond)
    {
        ImPlot::SetupAxesLimits((double)XMin, (double)XMax, (double)YMin, (double)YMax, Cond);
    }
    static void SetupFinish() { ImPlot::SetupFinish(); }

    // --- SetNext ---
    static void SetNextAxisLimits(int Axis, float Min, float Max, int Cond)
    {
        ImPlot::SetNextAxisLimits(Axis, (double)Min, (double)Max, Cond);
    }
    static void SetNextAxisToFit(int Axis) { ImPlot::SetNextAxisToFit(Axis); }
    static void SetNextAxesToFit() { ImPlot::SetNextAxesToFit(); }

    // --- Plot Items (Array Wrappers) ---
    
    // Line
    static void PlotLine(FString Label, TArray<float>& Values, float XScale, float XStart, int Flags)
    {
        if (Values.Num() > 0)
            ImPlot::PlotLine(TCHAR_TO_UTF8(*Label), Values.GetData(), Values.Num(), (double)XScale, (double)XStart, Flags);
    }
    static void PlotLineXY(FString Label, TArray<float>& Xs, TArray<float>& Ys, int Flags)
    {
        if (Xs.Num() > 0 && Ys.Num() > 0)
        {
            int Count = FMath::Min(Xs.Num(), Ys.Num());
            ImPlot::PlotLine(TCHAR_TO_UTF8(*Label), Xs.GetData(), Ys.GetData(), Count, Flags);
        }
    }

    // Scatter
    static void PlotScatter(FString Label, TArray<float>& Values, float XScale, float XStart, int Flags)
    {
        if (Values.Num() > 0)
            ImPlot::PlotScatter(TCHAR_TO_UTF8(*Label), Values.GetData(), Values.Num(), (double)XScale, (double)XStart, Flags);
    }
    static void PlotScatterXY(FString Label, TArray<float>& Xs, TArray<float>& Ys, int Flags)
    {
        if (Xs.Num() > 0 && Ys.Num() > 0)
        {
            int Count = FMath::Min(Xs.Num(), Ys.Num());
            ImPlot::PlotScatter(TCHAR_TO_UTF8(*Label), Xs.GetData(), Ys.GetData(), Count, Flags);
        }
    }

    // Bars
    static void PlotBars(FString Label, TArray<float>& Values, float BarSize, float Shift, int Flags)
    {
        if (Values.Num() > 0)
            ImPlot::PlotBars(TCHAR_TO_UTF8(*Label), Values.GetData(), Values.Num(), (double)BarSize, (double)Shift, Flags);
    }
    static void PlotBarsXY(FString Label, TArray<float>& Xs, TArray<float>& Ys, float BarSize, int Flags)
    {
        if (Xs.Num() > 0 && Ys.Num() > 0)
        {
            int Count = FMath::Min(Xs.Num(), Ys.Num());
            ImPlot::PlotBars(TCHAR_TO_UTF8(*Label), Xs.GetData(), Ys.GetData(), Count, (double)BarSize, Flags);
        }
    }

    // Stairs
    static void PlotStairs(FString Label, TArray<float>& Values, float XScale, float XStart, int Flags)
    {
        if (Values.Num() > 0)
            ImPlot::PlotStairs(TCHAR_TO_UTF8(*Label), Values.GetData(), Values.Num(), (double)XScale, (double)XStart, Flags);
    }

    // Shaded
    static void PlotShaded(FString Label, TArray<float>& Xs, TArray<float>& Ys, float YRef, int Flags)
    {
        if (Xs.Num() > 0 && Ys.Num() > 0)
        {
            int Count = FMath::Min(Xs.Num(), Ys.Num());
            ImPlot::PlotShaded(TCHAR_TO_UTF8(*Label), Xs.GetData(), Ys.GetData(), Count, (double)YRef, Flags);
        }
    }

    // Text
    static void PlotText(FString Text, float X, float Y, FVector2D PixOffset, int Flags)
    {
        ImPlot::PlotText(TCHAR_TO_UTF8(*Text), (double)X, (double)Y, ToImVec2(PixOffset), Flags);
    }

    // --- Styling ---
    static void PushStyleColor(int Idx, FLinearColor Color) { ImPlot::PushStyleColor(Idx, ToImVec4(Color)); }
    static void PopStyleColor(int Count) { ImPlot::PopStyleColor(Count); }
    static void PushStyleVarFloat(int Idx, float Val) { ImPlot::PushStyleVar(Idx, Val); }
    static void PushStyleVarInt(int Idx, int Val) { ImPlot::PushStyleVar(Idx, Val); }
    static void PushStyleVarVec2(int Idx, FVector2D Val) { ImPlot::PushStyleVar(Idx, ToImVec2(Val)); }
    static void PopStyleVar(int Count) { ImPlot::PopStyleVar(Count); }

    // --- Utils ---
    static bool IsPlotHovered() { return ImPlot::IsPlotHovered(); }
    static FVector2D GetPlotMousePos() { return ToFVector2D(ImPlot::GetPlotMousePos()); }
    static FVector2D GetPlotSize() { return ToFVector2D(ImPlot::GetPlotSize()); }
}

static void RegisterImPlotBinds()
{
    FAngelscriptBinds::FNamespace Namespace("ImPlot");

    // ================== ENUMS ==================
    
    // ImPlotFlags
    auto PlotFlags = FAngelscriptBinds::Enum("ImPlotFlags");
    PlotFlags["None"]           = ImPlotFlags_None;
    PlotFlags["NoTitle"]        = ImPlotFlags_NoTitle;
    PlotFlags["NoLegend"]       = ImPlotFlags_NoLegend;
    PlotFlags["NoMouseText"]    = ImPlotFlags_NoMouseText;
    PlotFlags["NoInputs"]       = ImPlotFlags_NoInputs;
    PlotFlags["NoMenus"]        = ImPlotFlags_NoMenus;
    PlotFlags["NoBoxSelect"]    = ImPlotFlags_NoBoxSelect;
    PlotFlags["NoFrame"]        = ImPlotFlags_NoFrame;
    PlotFlags["Equal"]          = ImPlotFlags_Equal;
    PlotFlags["Crosshairs"]     = ImPlotFlags_Crosshairs;
    PlotFlags["CanvasOnly"]     = ImPlotFlags_CanvasOnly;

    // ImPlotAxisFlags
    auto AxisFlags = FAngelscriptBinds::Enum("ImPlotAxisFlags");
    AxisFlags["None"]           = ImPlotAxisFlags_None;
    AxisFlags["NoLabel"]        = ImPlotAxisFlags_NoLabel;
    AxisFlags["NoGridLines"]    = ImPlotAxisFlags_NoGridLines;
    AxisFlags["NoTickMarks"]    = ImPlotAxisFlags_NoTickMarks;
    AxisFlags["NoTickLabels"]   = ImPlotAxisFlags_NoTickLabels;
    AxisFlags["NoInitialFit"]   = ImPlotAxisFlags_NoInitialFit;
    AxisFlags["NoMenus"]        = ImPlotAxisFlags_NoMenus;
    AxisFlags["NoSideSwitch"]   = ImPlotAxisFlags_NoSideSwitch;
    AxisFlags["NoHighlight"]    = ImPlotAxisFlags_NoHighlight;
    AxisFlags["Opposite"]       = ImPlotAxisFlags_Opposite;
    AxisFlags["Foreground"]     = ImPlotAxisFlags_Foreground;
    AxisFlags["Invert"]         = ImPlotAxisFlags_Invert;
    AxisFlags["AutoFit"]        = ImPlotAxisFlags_AutoFit;
    AxisFlags["RangeFit"]       = ImPlotAxisFlags_RangeFit;
    AxisFlags["PanStretch"]     = ImPlotAxisFlags_PanStretch;
    AxisFlags["LockMin"]        = ImPlotAxisFlags_LockMin;
    AxisFlags["LockMax"]        = ImPlotAxisFlags_LockMax;
    AxisFlags["Lock"]           = ImPlotAxisFlags_Lock;
    AxisFlags["NoDecorations"]  = ImPlotAxisFlags_NoDecorations;
    AxisFlags["AuxDefault"]     = ImPlotAxisFlags_AuxDefault;

    // ImAxis
    auto Axis = FAngelscriptBinds::Enum("ImAxis");
    Axis["X1"] = ImAxis_X1;
    Axis["X2"] = ImAxis_X2;
    Axis["X3"] = ImAxis_X3;
    Axis["Y1"] = ImAxis_Y1;
    Axis["Y2"] = ImAxis_Y2;
    Axis["Y3"] = ImAxis_Y3;

    // ImPlotCol
    auto Col = FAngelscriptBinds::Enum("ImPlotCol");
    Col["Line"]          = ImPlotCol_Line;
    Col["Fill"]          = ImPlotCol_Fill;
    Col["MarkerOutline"] = ImPlotCol_MarkerOutline;
    Col["MarkerFill"]    = ImPlotCol_MarkerFill;
    Col["ErrorBar"]      = ImPlotCol_ErrorBar;
    Col["FrameBg"]       = ImPlotCol_FrameBg;
    Col["PlotBg"]        = ImPlotCol_PlotBg;
    Col["PlotBorder"]    = ImPlotCol_PlotBorder;
    Col["LegendBg"]      = ImPlotCol_LegendBg;
    Col["LegendBorder"]  = ImPlotCol_LegendBorder;
    Col["LegendText"]    = ImPlotCol_LegendText;
    Col["TitleText"]     = ImPlotCol_TitleText;
    Col["InlayText"]     = ImPlotCol_InlayText;
    Col["AxisText"]      = ImPlotCol_AxisText;
    Col["AxisGrid"]      = ImPlotCol_AxisGrid;
    Col["AxisTick"]      = ImPlotCol_AxisTick;
    Col["AxisBg"]        = ImPlotCol_AxisBg;
    Col["AxisBgHovered"] = ImPlotCol_AxisBgHovered;
    Col["AxisBgActive"]  = ImPlotCol_AxisBgActive;
    Col["Selection"]     = ImPlotCol_Selection;
    Col["Crosshairs"]    = ImPlotCol_Crosshairs;

    // ImPlotMarker
    auto Marker = FAngelscriptBinds::Enum("ImPlotMarker");
    Marker["None"]      = ImPlotMarker_None;
    Marker["Circle"]    = ImPlotMarker_Circle;
    Marker["Square"]    = ImPlotMarker_Square;
    Marker["Diamond"]   = ImPlotMarker_Diamond;
    Marker["Up"]        = ImPlotMarker_Up;
    Marker["Down"]      = ImPlotMarker_Down;
    Marker["Left"]      = ImPlotMarker_Left;
    Marker["Right"]     = ImPlotMarker_Right;
    Marker["Cross"]     = ImPlotMarker_Cross;
    Marker["Plus"]      = ImPlotMarker_Plus;
    Marker["Asterisk"]  = ImPlotMarker_Asterisk;

    // ImPlotLocation
    auto Location = FAngelscriptBinds::Enum("ImPlotLocation");
    Location["Center"]    = ImPlotLocation_Center;
    Location["North"]     = ImPlotLocation_North;
    Location["South"]     = ImPlotLocation_South;
    Location["West"]      = ImPlotLocation_West;
    Location["East"]      = ImPlotLocation_East;
    Location["NorthWest"] = ImPlotLocation_NorthWest;
    Location["NorthEast"] = ImPlotLocation_NorthEast;
    Location["SouthWest"] = ImPlotLocation_SouthWest;
    Location["SouthEast"] = ImPlotLocation_SouthEast;

    // ImPlotCond
    auto Cond = FAngelscriptBinds::Enum("ImPlotCond");
    Cond["None"]   = ImPlotCond_None;
    Cond["Always"] = ImPlotCond_Always;
    Cond["Once"]   = ImPlotCond_Once;

    // ImPlotStyleVar
    auto StyleVar = FAngelscriptBinds::Enum("ImPlotStyleVar");
    StyleVar["LineWeight"]          = ImPlotStyleVar_LineWeight;
    StyleVar["Marker"]              = ImPlotStyleVar_Marker;
    StyleVar["MarkerSize"]          = ImPlotStyleVar_MarkerSize;
    StyleVar["MarkerWeight"]        = ImPlotStyleVar_MarkerWeight;
    StyleVar["FillAlpha"]           = ImPlotStyleVar_FillAlpha;
    StyleVar["ErrorBarSize"]        = ImPlotStyleVar_ErrorBarSize;
    StyleVar["ErrorBarWeight"]      = ImPlotStyleVar_ErrorBarWeight;
    StyleVar["DigitalBitHeight"]    = ImPlotStyleVar_DigitalBitHeight;
    StyleVar["DigitalBitGap"]       = ImPlotStyleVar_DigitalBitGap;
    StyleVar["PlotBorderSize"]      = ImPlotStyleVar_PlotBorderSize;
    StyleVar["MinorAlpha"]          = ImPlotStyleVar_MinorAlpha;
    StyleVar["MajorTickLen"]        = ImPlotStyleVar_MajorTickLen;
    StyleVar["MinorTickLen"]        = ImPlotStyleVar_MinorTickLen;
    StyleVar["MajorTickSize"]       = ImPlotStyleVar_MajorTickSize;
    StyleVar["MinorTickSize"]       = ImPlotStyleVar_MinorTickSize;
    StyleVar["MajorGridSize"]       = ImPlotStyleVar_MajorGridSize;
    StyleVar["MinorGridSize"]       = ImPlotStyleVar_MinorGridSize;
    StyleVar["PlotPadding"]         = ImPlotStyleVar_PlotPadding;
    StyleVar["LabelPadding"]        = ImPlotStyleVar_LabelPadding;
    StyleVar["LegendPadding"]       = ImPlotStyleVar_LegendPadding;
    StyleVar["LegendInnerPadding"]  = ImPlotStyleVar_LegendInnerPadding;
    StyleVar["LegendSpacing"]       = ImPlotStyleVar_LegendSpacing;
    StyleVar["MousePosPadding"]     = ImPlotStyleVar_MousePosPadding;
    StyleVar["AnnotationPadding"]   = ImPlotStyleVar_AnnotationPadding;
    StyleVar["FitPadding"]          = ImPlotStyleVar_FitPadding;
    StyleVar["PlotDefaultSize"]     = ImPlotStyleVar_PlotDefaultSize;
    StyleVar["PlotMinSize"]         = ImPlotStyleVar_PlotMinSize;

    // ================== FUNCTIONS ==================

    // Context
    FAngelscriptBinds::BindGlobalFunction("ImPlotContext CreateContext()", FUNC_TRIVIAL(ImPlotWrappers::CreateContext));
    FAngelscriptBinds::BindGlobalFunction("void DestroyContext(ImPlotContext Context = nullptr)", FUNC_TRIVIAL(ImPlotWrappers::DestroyContext));
    FAngelscriptBinds::BindGlobalFunction("ImPlotContext GetCurrentContext()", FUNC_TRIVIAL(ImPlotWrappers::GetCurrentContext));
    FAngelscriptBinds::BindGlobalFunction("void SetCurrentContext(ImPlotContext Context)", FUNC_TRIVIAL(ImPlotWrappers::SetCurrentContext));

    // Demo
    FAngelscriptBinds::BindGlobalFunction("void ShowDemoWindow(bool& bIsOpen)", FUNC_TRIVIAL(ImPlotWrappers::ShowDemoWindow));

    // Begin/End Plot
    FAngelscriptBinds::BindGlobalFunction("bool BeginPlot(FString Title, FVector2D Size = FVector2D(-1,0), int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::BeginPlot));
    FAngelscriptBinds::BindGlobalFunction("void EndPlot()", FUNC_TRIVIAL(ImPlotWrappers::EndPlot));

    // Subplots
    FAngelscriptBinds::BindGlobalFunction("bool BeginSubplots(FString Title, int Rows, int Cols, FVector2D Size, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::BeginSubplots));
    FAngelscriptBinds::BindGlobalFunction("void EndSubplots()", FUNC_TRIVIAL(ImPlotWrappers::EndSubplots));

    // Setup
    FAngelscriptBinds::BindGlobalFunction("void SetupAxis(int Axis, FString Label = \"\", int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetupAxis));
    FAngelscriptBinds::BindGlobalFunction("void SetupAxes(FString XLabel, FString YLabel, int XFlags = 0, int YFlags = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetupAxes));
    FAngelscriptBinds::BindGlobalFunction("void SetupLegend(int Location, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetupLegend));
    FAngelscriptBinds::BindGlobalFunction("void SetupAxisLimits(int Axis, float Min, float Max, int Cond = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetupAxisLimits));
    FAngelscriptBinds::BindGlobalFunction("void SetupAxesLimits(float XMin, float XMax, float YMin, float YMax, int Cond = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetupAxesLimits));
    FAngelscriptBinds::BindGlobalFunction("void SetupFinish()", FUNC_TRIVIAL(ImPlotWrappers::SetupFinish));

    // SetNext
    FAngelscriptBinds::BindGlobalFunction("void SetNextAxisLimits(int Axis, float Min, float Max, int Cond = 0)", FUNC_TRIVIAL(ImPlotWrappers::SetNextAxisLimits));
    FAngelscriptBinds::BindGlobalFunction("void SetNextAxisToFit(int Axis)", FUNC_TRIVIAL(ImPlotWrappers::SetNextAxisToFit));
    FAngelscriptBinds::BindGlobalFunction("void SetNextAxesToFit()", FUNC_TRIVIAL(ImPlotWrappers::SetNextAxesToFit));

    // Plot Items
    FAngelscriptBinds::BindGlobalFunction("void PlotLine(FString Label, const TArray<float>& Values, float XScale = 1.0f, float XStart = 0.0f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotLine));
    FAngelscriptBinds::BindGlobalFunction("void PlotLine(FString Label, const TArray<float>& Xs, const TArray<float>& Ys, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotLineXY));
    
    FAngelscriptBinds::BindGlobalFunction("void PlotScatter(FString Label, const TArray<float>& Values, float XScale = 1.0f, float XStart = 0.0f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotScatter));
    FAngelscriptBinds::BindGlobalFunction("void PlotScatter(FString Label, const TArray<float>& Xs, const TArray<float>& Ys, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotScatterXY));

    FAngelscriptBinds::BindGlobalFunction("void PlotBars(FString Label, const TArray<float>& Values, float BarSize = 0.67f, float Shift = 0.0f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotBars));
    FAngelscriptBinds::BindGlobalFunction("void PlotBars(FString Label, const TArray<float>& Xs, const TArray<float>& Ys, float BarSize = 0.67f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotBarsXY));

    FAngelscriptBinds::BindGlobalFunction("void PlotStairs(FString Label, const TArray<float>& Values, float XScale = 1.0f, float XStart = 0.0f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotStairs));
    FAngelscriptBinds::BindGlobalFunction("void PlotShaded(FString Label, const TArray<float>& Xs, const TArray<float>& Ys, float YRef = 0.0f, int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotShaded));

    FAngelscriptBinds::BindGlobalFunction("void PlotText(FString Text, float X, float Y, FVector2D PixOffset = FVector2D(0,0), int Flags = 0)", FUNC_TRIVIAL(ImPlotWrappers::PlotText));

    // Styling
    FAngelscriptBinds::BindGlobalFunction("void PushStyleColor(int Idx, FLinearColor Color)", FUNC_TRIVIAL(ImPlotWrappers::PushStyleColor));
    FAngelscriptBinds::BindGlobalFunction("void PopStyleColor(int Count = 1)", FUNC_TRIVIAL(ImPlotWrappers::PopStyleColor));
    
    FAngelscriptBinds::BindGlobalFunction("void PushStyleVar(int Idx, float Val)", FUNC_TRIVIAL(ImPlotWrappers::PushStyleVarFloat));
    FAngelscriptBinds::BindGlobalFunction("void PushStyleVar(int Idx, int Val)", FUNC_TRIVIAL(ImPlotWrappers::PushStyleVarInt));
    FAngelscriptBinds::BindGlobalFunction("void PushStyleVar(int Idx, FVector2D Val)", FUNC_TRIVIAL(ImPlotWrappers::PushStyleVarVec2));
    FAngelscriptBinds::BindGlobalFunction("void PopStyleVar(int Count = 1)", FUNC_TRIVIAL(ImPlotWrappers::PopStyleVar));

    // Utils
    FAngelscriptBinds::BindGlobalFunction("bool IsPlotHovered()", FUNC_TRIVIAL(ImPlotWrappers::IsPlotHovered));
    FAngelscriptBinds::BindGlobalFunction("FVector2D GetPlotMousePos()", FUNC_TRIVIAL(ImPlotWrappers::GetPlotMousePos));
    FAngelscriptBinds::BindGlobalFunction("FVector2D GetPlotSize()", FUNC_TRIVIAL(ImPlotWrappers::GetPlotSize));
}

// 静态注册
AS_FORCE_LINK static FAngelscriptBinds::FBind Bind_ImPlot_Static(FAngelscriptBinds::EOrder::Normal, &RegisterImPlotBinds);