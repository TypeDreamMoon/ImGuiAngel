#include "CoreMinimal.h"
#include "AngelscriptBinds.h"
#include "AngelscriptManager.h"

// 引入 ImGui
#include <imgui.h>

namespace ImGuiWrappers
{
    // =========================================================================================
    // 辅助转换函数：处理 FString -> const char* 和 引用类型
    // =========================================================================================

    // --- Windows ---
    static bool Begin(FString Name, bool& bOpen, int Flags) { return ImGui::Begin(TCHAR_TO_UTF8(*Name), &bOpen, Flags); }
    static bool Begin_NoClose(FString Name, int Flags) { return ImGui::Begin(TCHAR_TO_UTF8(*Name), nullptr, Flags); }
    static void End() { ImGui::End(); }

    static bool BeginChild(FString StrID, FVector2D Size, bool Border, int Flags) { return ImGui::BeginChild(TCHAR_TO_UTF8(*StrID), ImVec2(Size.X, Size.Y), Border, Flags); }
    static void EndChild() { ImGui::EndChild(); }

    // --- Window Utilities ---
    static bool IsWindowAppearing() { return ImGui::IsWindowAppearing(); }
    static bool IsWindowCollapsed() { return ImGui::IsWindowCollapsed(); }
    static bool IsWindowFocused(int Flags) { return ImGui::IsWindowFocused(Flags); }
    static bool IsWindowHovered(int Flags) { return ImGui::IsWindowHovered(Flags); }
    static FVector2D GetWindowPos() { ImVec2 V = ImGui::GetWindowPos(); return FVector2D(V.x, V.y); }
    static FVector2D GetWindowSize() { ImVec2 V = ImGui::GetWindowSize(); return FVector2D(V.x, V.y); }
    static float GetWindowWidth() { return ImGui::GetWindowWidth(); }
    static float GetWindowHeight() { return ImGui::GetWindowHeight(); }

    // --- Layout & Cursor ---
    static void Separator() { ImGui::Separator(); }
    static void SameLine(float OffsetFromStartX, float Spacing) { ImGui::SameLine(OffsetFromStartX, Spacing); }
    static void NewLine() { ImGui::NewLine(); }
    static void Spacing() { ImGui::Spacing(); }
    static void Dummy(FVector2D Size) { ImGui::Dummy(ImVec2(Size.X, Size.Y)); }
    static void Indent(float IndentW) { ImGui::Indent(IndentW); }
    static void Unindent(float IndentW) { ImGui::Unindent(IndentW); }
    static void BeginGroup() { ImGui::BeginGroup(); }
    static void EndGroup() { ImGui::EndGroup(); }
    static FVector2D GetCursorPos() { ImVec2 V = ImGui::GetCursorPos(); return FVector2D(V.x, V.y); }
    static float GetCursorPosX() { return ImGui::GetCursorPosX(); }
    static float GetCursorPosY() { return ImGui::GetCursorPosY(); }
    static void SetCursorPos(FVector2D LocalPos) { ImGui::SetCursorPos(ImVec2(LocalPos.X, LocalPos.Y)); }
    static void SetCursorPosX(float LocalX) { ImGui::SetCursorPosX(LocalX); }
    static void SetCursorPosY(float LocalY) { ImGui::SetCursorPosY(LocalY); }
    static float GetFrameHeight() { return ImGui::GetFrameHeight(); }
    static float GetFrameHeightWithSpacing() { return ImGui::GetFrameHeightWithSpacing(); }

    // --- Text ---
    static void Text(FString Text) { ImGui::Text("%s", TCHAR_TO_UTF8(*Text)); }
    static void TextColored(FLinearColor Color, FString Text) { ImGui::TextColored(ImVec4(Color.R, Color.G, Color.B, Color.A), "%s", TCHAR_TO_UTF8(*Text)); }
    static void TextDisabled(FString Text) { ImGui::TextDisabled("%s", TCHAR_TO_UTF8(*Text)); }
    static void TextWrapped(FString Text) { ImGui::TextWrapped("%s", TCHAR_TO_UTF8(*Text)); }
    static void LabelText(FString Label, FString Text) { ImGui::LabelText(TCHAR_TO_UTF8(*Label), "%s", TCHAR_TO_UTF8(*Text)); }
    static void BulletText(FString Text) { ImGui::BulletText("%s", TCHAR_TO_UTF8(*Text)); }

    // --- Main Widgets ---
    static bool Button(FString Label, FVector2D Size) { return ImGui::Button(TCHAR_TO_UTF8(*Label), ImVec2(Size.X, Size.Y)); }
    static bool SmallButton(FString Label) { return ImGui::SmallButton(TCHAR_TO_UTF8(*Label)); }
    static bool InvisibleButton(FString StrID, FVector2D Size, int Flags) { return ImGui::InvisibleButton(TCHAR_TO_UTF8(*StrID), ImVec2(Size.X, Size.Y), Flags); }
    static bool Checkbox(FString Label, bool& bValue) { return ImGui::Checkbox(TCHAR_TO_UTF8(*Label), &bValue); }
    
    // --- Radio & Combo ---
    static bool RadioButton(FString Label, bool bActive) { return ImGui::RadioButton(TCHAR_TO_UTF8(*Label), bActive); }
    static bool RadioButtonInt(FString Label, int32& V, int32 V_Button) { return ImGui::RadioButton(TCHAR_TO_UTF8(*Label), &V, V_Button); }
    
    // Combo (简化版，只支持简单字符串列表，复杂版需要自定义 Getter)
    static bool BeginCombo(FString Label, FString PreviewValue, int Flags) { return ImGui::BeginCombo(TCHAR_TO_UTF8(*Label), TCHAR_TO_UTF8(*PreviewValue), Flags); }
    static void EndCombo() { ImGui::EndCombo(); }
    static bool Selectable(FString Label, bool bSelected, int Flags, FVector2D Size) { return ImGui::Selectable(TCHAR_TO_UTF8(*Label), bSelected, Flags, ImVec2(Size.X, Size.Y)); }
    static bool SelectableRef(FString Label, bool& bSelected, int Flags, FVector2D Size) { return ImGui::Selectable(TCHAR_TO_UTF8(*Label), &bSelected, Flags, ImVec2(Size.X, Size.Y)); }

    // --- Drags ---
    static bool DragFloat(FString Label, float& V, float Speed, float Min, float Max, FString Format, int Flags) { return ImGui::DragFloat(TCHAR_TO_UTF8(*Label), &V, Speed, Min, Max, TCHAR_TO_UTF8(*Format), Flags); }
    static bool DragFloat_Simple(FString Label, float& V, float Speed, float Min, float Max) { return ImGui::DragFloat(TCHAR_TO_UTF8(*Label), &V, Speed, Min, Max, "%.3f", 0); }

    static bool DragFloat2(FString Label, FVector2D& V, float Speed, float Min, float Max, FString Format, int Flags) 
    { 
        float f[2] = { (float)V.X, (float)V.Y };
        bool res = ImGui::DragFloat2(TCHAR_TO_UTF8(*Label), f, Speed, Min, Max, TCHAR_TO_UTF8(*Format), Flags);
        if(res) { V.X = f[0]; V.Y = f[1]; }
        return res;
    }
    static bool DragFloat2_Simple(FString Label, FVector2D& V, float Speed, float Min, float Max) 
    { 
        float f[2] = { (float)V.X, (float)V.Y };
        bool res = ImGui::DragFloat2(TCHAR_TO_UTF8(*Label), f, Speed, Min, Max, "%.3f", 0);
        if(res) { V.X = f[0]; V.Y = f[1]; }
        return res;
    }

    static bool DragFloat3(FString Label, FVector& V, float Speed, float Min, float Max, FString Format, int Flags) 
    { 
        float f[3] = { (float)V.X, (float)V.Y, (float)V.Z };
        bool res = ImGui::DragFloat3(TCHAR_TO_UTF8(*Label), f, Speed, Min, Max, TCHAR_TO_UTF8(*Format), Flags);
        if(res) { V.X = f[0]; V.Y = f[1]; V.Z = f[2]; }
        return res;
    }
    static bool DragFloat3_Simple(FString Label, FVector& V, float Speed, float Min, float Max) 
    { 
        float f[3] = { (float)V.X, (float)V.Y, (float)V.Z };
        bool res = ImGui::DragFloat3(TCHAR_TO_UTF8(*Label), f, Speed, Min, Max, "%.3f", 0);
        if(res) { V.X = f[0]; V.Y = f[1]; V.Z = f[2]; }
        return res;
    }

    static bool DragInt(FString Label, int32& V, float Speed, int Min, int Max, FString Format, int Flags) { return ImGui::DragInt(TCHAR_TO_UTF8(*Label), &V, Speed, Min, Max, TCHAR_TO_UTF8(*Format), Flags); }
    static bool DragInt_Simple(FString Label, int32& V, float Speed, int Min, int Max) { return ImGui::DragInt(TCHAR_TO_UTF8(*Label), &V, Speed, Min, Max, "%d", 0); }

    // --- Sliders ---
    static bool SliderFloat(FString Label, float& V, float Min, float Max, FString Format, int Flags) { return ImGui::SliderFloat(TCHAR_TO_UTF8(*Label), &V, Min, Max, TCHAR_TO_UTF8(*Format), Flags); }
    static bool SliderFloat_Simple(FString Label, float& V, float Min, float Max) { return ImGui::SliderFloat(TCHAR_TO_UTF8(*Label), &V, Min, Max, "%.3f", 0); }

    static bool SliderFloat2(FString Label, FVector2D& V, float Min, float Max, FString Format, int Flags) 
    { 
        float f[2] = { (float)V.X, (float)V.Y };
        bool res = ImGui::SliderFloat2(TCHAR_TO_UTF8(*Label), f, Min, Max, TCHAR_TO_UTF8(*Format), Flags);
        if(res) { V.X = f[0]; V.Y = f[1]; }
        return res;
    }
    static bool SliderFloat2_Simple(FString Label, FVector2D& V, float Min, float Max) 
    { 
        float f[2] = { (float)V.X, (float)V.Y };
        bool res = ImGui::SliderFloat2(TCHAR_TO_UTF8(*Label), f, Min, Max, "%.3f", 0);
        if(res) { V.X = f[0]; V.Y = f[1]; }
        return res;
    }

    static bool SliderFloat3(FString Label, FVector& V, float Min, float Max, FString Format, int Flags) 
    { 
        float f[3] = { (float)V.X, (float)V.Y, (float)V.Z };
        bool res = ImGui::SliderFloat3(TCHAR_TO_UTF8(*Label), f, Min, Max, TCHAR_TO_UTF8(*Format), Flags);
        if(res) { V.X = f[0]; V.Y = f[1]; V.Z = f[2]; }
        return res;
    }
    static bool SliderFloat3_Simple(FString Label, FVector& V, float Min, float Max) 
    { 
        float f[3] = { (float)V.X, (float)V.Y, (float)V.Z };
        bool res = ImGui::SliderFloat3(TCHAR_TO_UTF8(*Label), f, Min, Max, "%.3f", 0);
        if(res) { V.X = f[0]; V.Y = f[1]; V.Z = f[2]; }
        return res;
    }

    static bool SliderInt(FString Label, int32& V, int Min, int Max, FString Format, int Flags) { return ImGui::SliderInt(TCHAR_TO_UTF8(*Label), &V, Min, Max, TCHAR_TO_UTF8(*Format), Flags); }
    static bool SliderInt_Simple(FString Label, int32& V, int Min, int Max) { return ImGui::SliderInt(TCHAR_TO_UTF8(*Label), &V, Min, Max, "%d", 0); }

    static bool SliderAngle(FString Label, float& V_Rad, float MinDeg, float MaxDeg, FString Format, int Flags) { return ImGui::SliderAngle(TCHAR_TO_UTF8(*Label), &V_Rad, MinDeg, MaxDeg, TCHAR_TO_UTF8(*Format), Flags); }
    static bool SliderAngle_Simple(FString Label, float& V_Rad, float MinDeg, float MaxDeg) { return ImGui::SliderAngle(TCHAR_TO_UTF8(*Label), &V_Rad, MinDeg, MaxDeg, "%.0f deg", 0); }

    // --- Inputs ---
    static bool InputText(FString Label, FString& Text, int BufferSize, int Flags)
    {
        if (BufferSize <= 0) BufferSize = 256;
        TArray<char> Buffer; Buffer.SetNumZeroed(BufferSize);
        FTCHARToUTF8 Converter(*Text);
        int32 CopyLen = FMath::Min(Converter.Length(), BufferSize - 1);
        FMemory::Memcpy(Buffer.GetData(), Converter.Get(), CopyLen);
        bool bChanged = ImGui::InputText(TCHAR_TO_UTF8(*Label), Buffer.GetData(), BufferSize, Flags);
        if (bChanged) Text = FString(UTF8_TO_TCHAR(Buffer.GetData()));
        return bChanged;
    }
    static bool InputTextMultiline(FString Label, FString& Text, FVector2D Size, int BufferSize, int Flags)
    {
        if (BufferSize <= 0) BufferSize = 1024;
        TArray<char> Buffer; Buffer.SetNumZeroed(BufferSize);
        FTCHARToUTF8 Converter(*Text);
        int32 CopyLen = FMath::Min(Converter.Length(), BufferSize - 1);
        FMemory::Memcpy(Buffer.GetData(), Converter.Get(), CopyLen);
        bool bChanged = ImGui::InputTextMultiline(TCHAR_TO_UTF8(*Label), Buffer.GetData(), BufferSize, ImVec2(Size.X, Size.Y), Flags);
        if (bChanged) Text = FString(UTF8_TO_TCHAR(Buffer.GetData()));
        return bChanged;
    }
    static bool InputFloat(FString Label, float& V, float Step, float StepFast, FString Format, int Flags) { return ImGui::InputFloat(TCHAR_TO_UTF8(*Label), &V, Step, StepFast, TCHAR_TO_UTF8(*Format), Flags); }
    static bool InputFloat_Simple(FString Label, float& V, float Step, float StepFast) { return ImGui::InputFloat(TCHAR_TO_UTF8(*Label), &V, Step, StepFast, "%.3f", 0); }

    static bool InputInt(FString Label, int32& V, int Step, int StepFast, int Flags) { return ImGui::InputInt(TCHAR_TO_UTF8(*Label), &V, Step, StepFast, Flags); }

    // --- Colors ---
    static bool ColorEdit3(FString Label, FLinearColor& Col, int Flags) { return ImGui::ColorEdit3(TCHAR_TO_UTF8(*Label), (float*)&Col, Flags); }
    static bool ColorEdit4(FString Label, FLinearColor& Col, int Flags) { return ImGui::ColorEdit4(TCHAR_TO_UTF8(*Label), (float*)&Col, Flags); }
    static bool ColorPicker3(FString Label, FLinearColor& Col, int Flags) { return ImGui::ColorPicker3(TCHAR_TO_UTF8(*Label), (float*)&Col, Flags); }
    static bool ColorPicker4(FString Label, FLinearColor& Col, int Flags) { return ImGui::ColorPicker4(TCHAR_TO_UTF8(*Label), (float*)&Col, Flags); }
    static bool ColorButton(FString DescId, FLinearColor Col, int Flags, FVector2D Size) { return ImGui::ColorButton(TCHAR_TO_UTF8(*DescId), ImVec4(Col.R, Col.G, Col.B, Col.A), Flags, ImVec2(Size.X, Size.Y)); }

    // --- Tree & Tabs ---
    static bool TreeNode(FString Label) { return ImGui::TreeNode(TCHAR_TO_UTF8(*Label)); }
    static bool TreeNodeEx(FString Label, int Flags) { return ImGui::TreeNodeEx(TCHAR_TO_UTF8(*Label), Flags); }
    static void TreePush(FString StrID) { ImGui::TreePush(TCHAR_TO_UTF8(*StrID)); }
    static void TreePop() { ImGui::TreePop(); }
    static float GetTreeNodeToLabelSpacing() { return ImGui::GetTreeNodeToLabelSpacing(); }
    static bool CollapsingHeader(FString Label, int Flags) { return ImGui::CollapsingHeader(TCHAR_TO_UTF8(*Label), Flags); }
    static bool CollapsingHeaderWithClose(FString Label, bool& bOpen, int Flags) { return ImGui::CollapsingHeader(TCHAR_TO_UTF8(*Label), &bOpen, Flags); }
    static void SetNextItemOpen(bool bIsOpen, int Cond) { ImGui::SetNextItemOpen(bIsOpen, Cond); }

    static bool BeginTabBar(FString StrID, int Flags) { return ImGui::BeginTabBar(TCHAR_TO_UTF8(*StrID), Flags); }
    static void EndTabBar() { ImGui::EndTabBar(); }
    static bool BeginTabItem(FString Label, bool& bOpen, int Flags) { return ImGui::BeginTabItem(TCHAR_TO_UTF8(*Label), &bOpen, Flags); }
    static bool BeginTabItem_Simple(FString Label, int Flags) { return ImGui::BeginTabItem(TCHAR_TO_UTF8(*Label), nullptr, Flags); }
    static void EndTabItem() { ImGui::EndTabItem(); }

    // --- Menus ---
    static bool BeginMenuBar() { return ImGui::BeginMenuBar(); }
    static void EndMenuBar() { ImGui::EndMenuBar(); }
    static bool BeginMainMenuBar() { return ImGui::BeginMainMenuBar(); }
    static void EndMainMenuBar() { ImGui::EndMainMenuBar(); }
    static bool BeginMenu(FString Label, bool bEnabled) { return ImGui::BeginMenu(TCHAR_TO_UTF8(*Label), bEnabled); }
    static void EndMenu() { ImGui::EndMenu(); }
    static bool MenuItem(FString Label, FString Shortcut, bool bSelected, bool bEnabled) { return ImGui::MenuItem(TCHAR_TO_UTF8(*Label), TCHAR_TO_UTF8(*Shortcut), bSelected, bEnabled); }
    static bool MenuItemToggle(FString Label, FString Shortcut, bool& bSelected, bool bEnabled) { return ImGui::MenuItem(TCHAR_TO_UTF8(*Label), TCHAR_TO_UTF8(*Shortcut), &bSelected, bEnabled); }

    // --- Tooltips ---
    static void BeginTooltip() { ImGui::BeginTooltip(); }
    static void EndTooltip() { ImGui::EndTooltip(); }
    static void SetTooltip(FString Text) { ImGui::SetTooltip("%s", TCHAR_TO_UTF8(*Text)); }

    // --- Popups ---
    static bool BeginPopup(FString StrID, int Flags) { return ImGui::BeginPopup(TCHAR_TO_UTF8(*StrID), Flags); }
    static bool BeginPopupModal(FString Name, bool& bOpen, int Flags) { return ImGui::BeginPopupModal(TCHAR_TO_UTF8(*Name), &bOpen, Flags); }
    static bool BeginPopupContextItem(FString StrID, int PopupFlags) { return ImGui::BeginPopupContextItem(TCHAR_TO_UTF8(*StrID), PopupFlags); }
    static void EndPopup() { ImGui::EndPopup(); }
    static void OpenPopup(FString StrID, int PopupFlags) { ImGui::OpenPopup(TCHAR_TO_UTF8(*StrID), PopupFlags); }
    static void CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }

    // --- Tables (New API) ---
    static bool BeginTable(FString StrID, int Columns, int Flags, FVector2D OuterSize, float InnerWidth) { return ImGui::BeginTable(TCHAR_TO_UTF8(*StrID), Columns, Flags, ImVec2(OuterSize.X, OuterSize.Y), InnerWidth); }
    static void EndTable() { ImGui::EndTable(); }
    static void TableNextRow(int RowFlags, float MinRowHeight) { ImGui::TableNextRow(RowFlags, MinRowHeight); }
    static bool TableNextColumn() { return ImGui::TableNextColumn(); }
    static bool TableSetColumnIndex(int ColumnN) { return ImGui::TableSetColumnIndex(ColumnN); }
    static void TableSetupColumn(FString Label, int Flags, float InitWidthOrWeight, int UserId) { ImGui::TableSetupColumn(TCHAR_TO_UTF8(*Label), Flags, InitWidthOrWeight, UserId); }
    static void TableHeadersRow() { ImGui::TableHeadersRow(); }

    // --- Utilities ---
    static bool IsItemHovered(int Flags) { return ImGui::IsItemHovered(Flags); }
    static bool IsItemActive() { return ImGui::IsItemActive(); }
    static bool IsItemFocused() { return ImGui::IsItemFocused(); }
    static bool IsItemClicked(int MouseButton) { return ImGui::IsItemClicked(MouseButton); }
    static bool IsItemVisible() { return ImGui::IsItemVisible(); }
    static bool IsAnyItemHovered() { return ImGui::IsAnyItemHovered(); }
    static bool IsAnyItemActive() { return ImGui::IsAnyItemActive(); }
    
    // --- Style & ID ---
    static void PushID(FString StrID) { ImGui::PushID(TCHAR_TO_UTF8(*StrID)); }
    static void PushIDInt(int IntID) { ImGui::PushID(IntID); }
    static void PopID() { ImGui::PopID(); }
    static void PushItemWidth(float ItemWidth) { ImGui::PushItemWidth(ItemWidth); }
    static void PopItemWidth() { ImGui::PopItemWidth(); }
    static void SetNextItemWidth(float ItemWidth) { ImGui::SetNextItemWidth(ItemWidth); }

    // Demo
    static void ShowDemoWindow(bool& bShow) { if (bShow) ImGui::ShowDemoWindow(&bShow); }
    static void ShowUserGuide() { ImGui::ShowUserGuide(); }
}

// 注册绑定
static void RegisterImGuiBinds()
{
    FAngelscriptBinds::FNamespace Namespace("ImGui");

    // ================== ENUMS (常用 Flags) ==================
    // WindowFlags
    auto WindowFlags = FAngelscriptBinds::Enum("ImGuiWindowFlags");
    WindowFlags["None"]             = 0;
    WindowFlags["NoTitleBar"]       = 1 << 0;
    WindowFlags["NoResize"]         = 1 << 1;
    WindowFlags["NoMove"]           = 1 << 2;
    WindowFlags["NoScrollbar"]      = 1 << 3;
    WindowFlags["NoScrollWithMouse"]= 1 << 4;
    WindowFlags["NoCollapse"]       = 1 << 5;
    WindowFlags["AlwaysAutoResize"] = 1 << 6;
    WindowFlags["NoBackground"]     = 1 << 7;
    WindowFlags["NoSavedSettings"]  = 1 << 8;
    WindowFlags["MenuBar"]          = 1 << 10;
    WindowFlags["HorizontalScrollbar"] = 1 << 11;
    WindowFlags["NoFocusOnAppearing"] = 1 << 12;
    WindowFlags["NoBringToFrontOnFocus"] = 1 << 13;

    // InputTextFlags
    auto InputTextFlags = FAngelscriptBinds::Enum("ImGuiInputTextFlags");
    InputTextFlags["None"]          = 0;
    InputTextFlags["CharsDecimal"]  = 1 << 0;
    InputTextFlags["CharsHexadecimal"] = 1 << 1;
    InputTextFlags["CharsUppercase"] = 1 << 2;
    InputTextFlags["CharsNoBlank"]  = 1 << 3;
    InputTextFlags["AutoSelectAll"] = 1 << 4;
    InputTextFlags["EnterReturnsTrue"] = 1 << 5;
    InputTextFlags["Password"]      = 1 << 7;
    InputTextFlags["ReadOnly"]      = 1 << 14;

    // TreeNodeFlags
    auto TreeNodeFlags = FAngelscriptBinds::Enum("ImGuiTreeNodeFlags");
    TreeNodeFlags["None"]           = 0;
    TreeNodeFlags["Selected"]       = 1 << 0;
    TreeNodeFlags["Framed"]         = 1 << 1;
    TreeNodeFlags["AllowItemOverlap"] = 1 << 2;
    TreeNodeFlags["NoTreePushOnOpen"] = 1 << 3;
    TreeNodeFlags["DefaultOpen"]    = 1 << 5;
    TreeNodeFlags["OpenOnDoubleClick"] = 1 << 6;
    TreeNodeFlags["OpenOnArrow"]    = 1 << 7;
    TreeNodeFlags["Leaf"]           = 1 << 8;
    TreeNodeFlags["Bullet"]         = 1 << 9;

    // TableFlags
    auto TableFlags = FAngelscriptBinds::Enum("ImGuiTableFlags");
    TableFlags["None"]              = 0;
    TableFlags["Resizable"]         = 1 << 0;
    TableFlags["Reorderable"]       = 1 << 1;
    TableFlags["Hideable"]          = 1 << 2;
    TableFlags["Sortable"]          = 1 << 3;
    TableFlags["NoSavedSettings"]   = 1 << 4;
    TableFlags["ContextMenuInBody"] = 1 << 5;
    TableFlags["RowBg"]             = 1 << 6;
    TableFlags["BordersInnerH"]     = 1 << 7;
    TableFlags["BordersOuterH"]     = 1 << 8;
    TableFlags["BordersInnerV"]     = 1 << 9;
    TableFlags["BordersOuterV"]     = 1 << 10;
    TableFlags["Borders"]           = 15 << 7; // InnerH | OuterH | InnerV | OuterV

    // Cond (SetNextItemOpen 用)
    auto Cond = FAngelscriptBinds::Enum("ImGuiCond");
    Cond["None"]          = 0;
    Cond["Always"]        = 1 << 0;
    Cond["Once"]          = 1 << 1;
    Cond["FirstUseEver"]  = 1 << 2;
    Cond["Appearing"]     = 1 << 3;

    // ================== FUNCTIONS ==================

    // Windows
    FAngelscriptBinds::BindGlobalFunction("bool Begin(FString Name, bool& bOpen, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::Begin));
    FAngelscriptBinds::BindGlobalFunction("bool Begin(FString Name, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::Begin_NoClose));
    FAngelscriptBinds::BindGlobalFunction("void End()", FUNC_TRIVIAL(ImGuiWrappers::End));
    FAngelscriptBinds::BindGlobalFunction("bool BeginChild(FString StrID, FVector2D Size = FVector2D(0,0), bool Border = false, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginChild));
    FAngelscriptBinds::BindGlobalFunction("void EndChild()", FUNC_TRIVIAL(ImGuiWrappers::EndChild));

    // Utilities
    FAngelscriptBinds::BindGlobalFunction("bool IsWindowAppearing()", FUNC_TRIVIAL(ImGuiWrappers::IsWindowAppearing));
    FAngelscriptBinds::BindGlobalFunction("bool IsWindowCollapsed()", FUNC_TRIVIAL(ImGuiWrappers::IsWindowCollapsed));
    FAngelscriptBinds::BindGlobalFunction("bool IsWindowFocused(int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::IsWindowFocused));
    FAngelscriptBinds::BindGlobalFunction("bool IsWindowHovered(int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::IsWindowHovered));
    FAngelscriptBinds::BindGlobalFunction("FVector2D GetWindowPos()", FUNC_TRIVIAL(ImGuiWrappers::GetWindowPos));
    FAngelscriptBinds::BindGlobalFunction("FVector2D GetWindowSize()", FUNC_TRIVIAL(ImGuiWrappers::GetWindowSize));
    FAngelscriptBinds::BindGlobalFunction("float32 GetWindowWidth()", FUNC_TRIVIAL(ImGuiWrappers::GetWindowWidth));
    FAngelscriptBinds::BindGlobalFunction("float32 GetWindowHeight()", FUNC_TRIVIAL(ImGuiWrappers::GetWindowHeight));

    // Layout
    FAngelscriptBinds::BindGlobalFunction("void Separator()", FUNC_TRIVIAL(ImGuiWrappers::Separator));
    FAngelscriptBinds::BindGlobalFunction("void SameLine(float OffsetFromStartX = 0.0f, float Spacing = -1.0f)", FUNC_TRIVIAL(ImGuiWrappers::SameLine));
    FAngelscriptBinds::BindGlobalFunction("void NewLine()", FUNC_TRIVIAL(ImGuiWrappers::NewLine));
    FAngelscriptBinds::BindGlobalFunction("void Spacing()", FUNC_TRIVIAL(ImGuiWrappers::Spacing));
    FAngelscriptBinds::BindGlobalFunction("void Dummy(FVector2D Size)", FUNC_TRIVIAL(ImGuiWrappers::Dummy));
    FAngelscriptBinds::BindGlobalFunction("void Indent(float IndentW = 0.0f)", FUNC_TRIVIAL(ImGuiWrappers::Indent));
    FAngelscriptBinds::BindGlobalFunction("void Unindent(float IndentW = 0.0f)", FUNC_TRIVIAL(ImGuiWrappers::Unindent));
    FAngelscriptBinds::BindGlobalFunction("void BeginGroup()", FUNC_TRIVIAL(ImGuiWrappers::BeginGroup));
    FAngelscriptBinds::BindGlobalFunction("void EndGroup()", FUNC_TRIVIAL(ImGuiWrappers::EndGroup));
    FAngelscriptBinds::BindGlobalFunction("FVector2D GetCursorPos()", FUNC_TRIVIAL(ImGuiWrappers::GetCursorPos));
    FAngelscriptBinds::BindGlobalFunction("float32 GetCursorPosX()", FUNC_TRIVIAL(ImGuiWrappers::GetCursorPosX));
    FAngelscriptBinds::BindGlobalFunction("float32 GetCursorPosY()", FUNC_TRIVIAL(ImGuiWrappers::GetCursorPosY));
    FAngelscriptBinds::BindGlobalFunction("void SetCursorPos(FVector2D LocalPos)", FUNC_TRIVIAL(ImGuiWrappers::SetCursorPos));
    FAngelscriptBinds::BindGlobalFunction("void SetCursorPosX(float LocalX)", FUNC_TRIVIAL(ImGuiWrappers::SetCursorPosX));
    FAngelscriptBinds::BindGlobalFunction("void SetCursorPosY(float LocalY)", FUNC_TRIVIAL(ImGuiWrappers::SetCursorPosY));
    FAngelscriptBinds::BindGlobalFunction("float32 GetFrameHeight()", FUNC_TRIVIAL(ImGuiWrappers::GetFrameHeight));
    FAngelscriptBinds::BindGlobalFunction("float32 GetFrameHeightWithSpacing()", FUNC_TRIVIAL(ImGuiWrappers::GetFrameHeightWithSpacing));

    // Text
    FAngelscriptBinds::BindGlobalFunction("void Text(FString Text)", FUNC_TRIVIAL(ImGuiWrappers::Text));
    FAngelscriptBinds::BindGlobalFunction("void TextColored(FLinearColor Color, FString Text)", FUNC_TRIVIAL(ImGuiWrappers::TextColored));
    FAngelscriptBinds::BindGlobalFunction("void TextDisabled(FString Text)", FUNC_TRIVIAL(ImGuiWrappers::TextDisabled));
    FAngelscriptBinds::BindGlobalFunction("void TextWrapped(FString Text)", FUNC_TRIVIAL(ImGuiWrappers::TextWrapped));
    FAngelscriptBinds::BindGlobalFunction("void LabelText(FString Label, FString Text)", FUNC_TRIVIAL(ImGuiWrappers::LabelText));
    FAngelscriptBinds::BindGlobalFunction("void BulletText(FString Text)", FUNC_TRIVIAL(ImGuiWrappers::BulletText));

    // Widgets
    FAngelscriptBinds::BindGlobalFunction("bool Button(FString Label, FVector2D Size = FVector2D(0,0))", FUNC_TRIVIAL(ImGuiWrappers::Button));
    FAngelscriptBinds::BindGlobalFunction("bool SmallButton(FString Label)", FUNC_TRIVIAL(ImGuiWrappers::SmallButton));
    FAngelscriptBinds::BindGlobalFunction("bool InvisibleButton(FString StrID, FVector2D Size, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::InvisibleButton));
    FAngelscriptBinds::BindGlobalFunction("bool Checkbox(FString Label, bool& bValue)", FUNC_TRIVIAL(ImGuiWrappers::Checkbox));
    FAngelscriptBinds::BindGlobalFunction("bool RadioButton(FString Label, bool bActive)", FUNC_TRIVIAL(ImGuiWrappers::RadioButton));
    FAngelscriptBinds::BindGlobalFunction("bool RadioButton(FString Label, int32& V, int32 V_Button)", FUNC_TRIVIAL(ImGuiWrappers::RadioButtonInt));

    // Combo & Selectable
    FAngelscriptBinds::BindGlobalFunction("bool BeginCombo(FString Label, FString PreviewValue, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginCombo));
    FAngelscriptBinds::BindGlobalFunction("void EndCombo()", FUNC_TRIVIAL(ImGuiWrappers::EndCombo));
    FAngelscriptBinds::BindGlobalFunction("bool Selectable(FString Label, bool bSelected = false, int Flags = 0, FVector2D Size = FVector2D(0,0))", FUNC_TRIVIAL(ImGuiWrappers::Selectable));
    FAngelscriptBinds::BindGlobalFunction("bool Selectable(FString Label, bool& bSelected, int Flags = 0, FVector2D Size = FVector2D(0,0))", FUNC_TRIVIAL(ImGuiWrappers::SelectableRef));

    // Drags
    FAngelscriptBinds::BindGlobalFunction("bool DragFloat(FString Label, float32& V, float Speed = 1.0f, float Min = 0.0f, float Max = 0.0f, FString Format = \"%.3f\", int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat));
    FAngelscriptBinds::BindGlobalFunction("bool DragFloat(FString Label, float32& V, float Speed, float Min, float Max)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool DragFloat2(FString Label, FVector2D& V, float Speed = 1.0f, float Min = 0.0f, float Max = 0.0f, FString Format = \"%.3f\", int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat2));
    FAngelscriptBinds::BindGlobalFunction("bool DragFloat2(FString Label, FVector2D& V, float Speed, float Min, float Max)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat2_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool DragFloat3(FString Label, FVector& V, float Speed = 1.0f, float Min = 0.0f, float Max = 0.0f, FString Format = \"%.3f\", int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat3));
    FAngelscriptBinds::BindGlobalFunction("bool DragFloat3(FString Label, FVector& V, float Speed, float Min, float Max)", FUNC_TRIVIAL(ImGuiWrappers::DragFloat3_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool DragInt(FString Label, int32& V, float Speed = 1.0f, int Min = 0, int Max = 0, FString Format = \"%d\", int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::DragInt));
    FAngelscriptBinds::BindGlobalFunction("bool DragInt(FString Label, int32& V, float Speed, int Min, int Max)", FUNC_TRIVIAL(ImGuiWrappers::DragInt_Simple));

    // Sliders
    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat(FString Label, float32& V, float32 Min, float32 Max, FString Format, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat));
    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat(FString Label, float32& V, float32 Min, float32 Max)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat_Simple));
    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat(FString Label, float& V, float32 Min, float32 Max)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat2(FString Label, FVector2D& V, float Min, float Max, FString Format , int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat2));
    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat2(FString Label, FVector2D& V, float Min, float Max)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat2_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat3(FString Label, FVector& V, float Min, float Max, FString Format, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat3));
    FAngelscriptBinds::BindGlobalFunction("bool SliderFloat3(FString Label, FVector& V, float Min, float Max)", FUNC_TRIVIAL(ImGuiWrappers::SliderFloat3_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool SliderInt(FString Label, int32& V, int Min, int Max, FString Format, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::SliderInt));
    FAngelscriptBinds::BindGlobalFunction("bool SliderInt(FString Label, int32& V, int Min, int Max)", FUNC_TRIVIAL(ImGuiWrappers::SliderInt_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool SliderAngle(FString Label, float& V_Rad, float MinDeg = -360.0f, float MaxDeg = 360.0f, FString Format, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::SliderAngle));
    FAngelscriptBinds::BindGlobalFunction("bool SliderAngle(FString Label, float& V_Rad, float MinDeg, float MaxDeg)", FUNC_TRIVIAL(ImGuiWrappers::SliderAngle_Simple));

    // Inputs
    FAngelscriptBinds::BindGlobalFunction("bool InputText(FString Label, FString& Text, int BufferSize = 256, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::InputText));
    FAngelscriptBinds::BindGlobalFunction("bool InputTextMultiline(FString Label, FString& Text, FVector2D Size = FVector2D(0,0), int BufferSize = 1024, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::InputTextMultiline));
    
    FAngelscriptBinds::BindGlobalFunction("bool InputFloat(FString Label, float32& V, float Step = 0.0f, float StepFast = 0.0f, FString Format = \"%.3f\", int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::InputFloat));
    FAngelscriptBinds::BindGlobalFunction("bool InputFloat(FString Label, float32& V, float Step, float StepFast)", FUNC_TRIVIAL(ImGuiWrappers::InputFloat_Simple));

    FAngelscriptBinds::BindGlobalFunction("bool InputInt(FString Label, int32& V, int Step = 1, int StepFast = 100, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::InputInt));

    // Colors
    FAngelscriptBinds::BindGlobalFunction("bool ColorEdit3(FString Label, FLinearColor& Col, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::ColorEdit3));
    FAngelscriptBinds::BindGlobalFunction("bool ColorEdit4(FString Label, FLinearColor& Col, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::ColorEdit4));
    FAngelscriptBinds::BindGlobalFunction("bool ColorPicker3(FString Label, FLinearColor& Col, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::ColorPicker3));
    FAngelscriptBinds::BindGlobalFunction("bool ColorPicker4(FString Label, FLinearColor& Col, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::ColorPicker4));
    FAngelscriptBinds::BindGlobalFunction("bool ColorButton(FString DescId, FLinearColor Col, int Flags = 0, FVector2D Size = FVector2D(0,0))", FUNC_TRIVIAL(ImGuiWrappers::ColorButton));

    // Trees
    FAngelscriptBinds::BindGlobalFunction("bool TreeNode(FString Label)", FUNC_TRIVIAL(ImGuiWrappers::TreeNode));
    FAngelscriptBinds::BindGlobalFunction("bool TreeNode(FString Label, int Flags)", FUNC_TRIVIAL(ImGuiWrappers::TreeNodeEx));
    FAngelscriptBinds::BindGlobalFunction("void TreePush(FString StrID)", FUNC_TRIVIAL(ImGuiWrappers::TreePush));
    FAngelscriptBinds::BindGlobalFunction("void TreePop()", FUNC_TRIVIAL(ImGuiWrappers::TreePop));
    FAngelscriptBinds::BindGlobalFunction("float32 GetTreeNodeToLabelSpacing()", FUNC_TRIVIAL(ImGuiWrappers::GetTreeNodeToLabelSpacing));
    FAngelscriptBinds::BindGlobalFunction("bool CollapsingHeader(FString Label, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::CollapsingHeader));
    FAngelscriptBinds::BindGlobalFunction("bool CollapsingHeader(FString Label, bool& bOpen, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::CollapsingHeaderWithClose));
    FAngelscriptBinds::BindGlobalFunction("void SetNextItemOpen(bool bIsOpen, int Cond = 0)", FUNC_TRIVIAL(ImGuiWrappers::SetNextItemOpen));

    // Tab Bar
    FAngelscriptBinds::BindGlobalFunction("bool BeginTabBar(FString StrID, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginTabBar));
    FAngelscriptBinds::BindGlobalFunction("void EndTabBar()", FUNC_TRIVIAL(ImGuiWrappers::EndTabBar));
    FAngelscriptBinds::BindGlobalFunction("bool BeginTabItem(FString Label, bool& bOpen, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginTabItem));
    FAngelscriptBinds::BindGlobalFunction("bool BeginTabItem(FString Label, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginTabItem_Simple));
    FAngelscriptBinds::BindGlobalFunction("void EndTabItem()", FUNC_TRIVIAL(ImGuiWrappers::EndTabItem));

    // Menu
    FAngelscriptBinds::BindGlobalFunction("bool BeginMenuBar()", FUNC_TRIVIAL(ImGuiWrappers::BeginMenuBar));
    FAngelscriptBinds::BindGlobalFunction("void EndMenuBar()", FUNC_TRIVIAL(ImGuiWrappers::EndMenuBar));
    FAngelscriptBinds::BindGlobalFunction("bool BeginMainMenuBar()", FUNC_TRIVIAL(ImGuiWrappers::BeginMainMenuBar));
    FAngelscriptBinds::BindGlobalFunction("void EndMainMenuBar()", FUNC_TRIVIAL(ImGuiWrappers::EndMainMenuBar));
    FAngelscriptBinds::BindGlobalFunction("bool BeginMenu(FString Label, bool bEnabled = true)", FUNC_TRIVIAL(ImGuiWrappers::BeginMenu));
    FAngelscriptBinds::BindGlobalFunction("void EndMenu()", FUNC_TRIVIAL(ImGuiWrappers::EndMenu));
    FAngelscriptBinds::BindGlobalFunction("bool MenuItem(FString Label, FString Shortcut = \"\", bool bSelected = false, bool bEnabled = true)", FUNC_TRIVIAL(ImGuiWrappers::MenuItem));
    FAngelscriptBinds::BindGlobalFunction("bool MenuItem(FString Label, FString Shortcut, bool& bSelected, bool bEnabled = true)", FUNC_TRIVIAL(ImGuiWrappers::MenuItemToggle));

    // Tooltips
    FAngelscriptBinds::BindGlobalFunction("void BeginTooltip()", FUNC_TRIVIAL(ImGuiWrappers::BeginTooltip));
    FAngelscriptBinds::BindGlobalFunction("void EndTooltip()", FUNC_TRIVIAL(ImGuiWrappers::EndTooltip));
    FAngelscriptBinds::BindGlobalFunction("void SetTooltip(FString Text)", FUNC_TRIVIAL(ImGuiWrappers::SetTooltip));

    // Popups
    FAngelscriptBinds::BindGlobalFunction("bool BeginPopup(FString StrID, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginPopup));
    FAngelscriptBinds::BindGlobalFunction("bool BeginPopupModal(FString Name, bool& bOpen, int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::BeginPopupModal));
    FAngelscriptBinds::BindGlobalFunction("bool BeginPopupContextItem(FString StrID = \"\", int PopupFlags = 1)", FUNC_TRIVIAL(ImGuiWrappers::BeginPopupContextItem)); // 1=MouseButtonRight
    FAngelscriptBinds::BindGlobalFunction("void EndPopup()", FUNC_TRIVIAL(ImGuiWrappers::EndPopup));
    FAngelscriptBinds::BindGlobalFunction("void OpenPopup(FString StrID, int PopupFlags = 0)", FUNC_TRIVIAL(ImGuiWrappers::OpenPopup));
    FAngelscriptBinds::BindGlobalFunction("void CloseCurrentPopup()", FUNC_TRIVIAL(ImGuiWrappers::CloseCurrentPopup));

    // Tables
    FAngelscriptBinds::BindGlobalFunction("bool BeginTable(FString StrID, int Columns, int Flags = 0, FVector2D OuterSize = FVector2D(0,0), float InnerWidth = 0.0f)", FUNC_TRIVIAL(ImGuiWrappers::BeginTable));
    FAngelscriptBinds::BindGlobalFunction("void EndTable()", FUNC_TRIVIAL(ImGuiWrappers::EndTable));
    FAngelscriptBinds::BindGlobalFunction("void TableNextRow(int RowFlags = 0, float MinRowHeight = 0.0f)", FUNC_TRIVIAL(ImGuiWrappers::TableNextRow));
    FAngelscriptBinds::BindGlobalFunction("bool TableNextColumn()", FUNC_TRIVIAL(ImGuiWrappers::TableNextColumn));
    FAngelscriptBinds::BindGlobalFunction("bool TableSetColumnIndex(int ColumnN)", FUNC_TRIVIAL(ImGuiWrappers::TableSetColumnIndex));
    FAngelscriptBinds::BindGlobalFunction("void TableSetupColumn(FString Label, int Flags = 0, float InitWidthOrWeight = 0.0f, int UserId = 0)", FUNC_TRIVIAL(ImGuiWrappers::TableSetupColumn));
    FAngelscriptBinds::BindGlobalFunction("void TableHeadersRow()", FUNC_TRIVIAL(ImGuiWrappers::TableHeadersRow));

    // Utilities / Item Query
    FAngelscriptBinds::BindGlobalFunction("bool IsItemHovered(int Flags = 0)", FUNC_TRIVIAL(ImGuiWrappers::IsItemHovered));
    FAngelscriptBinds::BindGlobalFunction("bool IsItemActive()", FUNC_TRIVIAL(ImGuiWrappers::IsItemActive));
    FAngelscriptBinds::BindGlobalFunction("bool IsItemFocused()", FUNC_TRIVIAL(ImGuiWrappers::IsItemFocused));
    FAngelscriptBinds::BindGlobalFunction("bool IsItemClicked(int MouseButton = 0)", FUNC_TRIVIAL(ImGuiWrappers::IsItemClicked));
    FAngelscriptBinds::BindGlobalFunction("bool IsItemVisible()", FUNC_TRIVIAL(ImGuiWrappers::IsItemVisible));
    FAngelscriptBinds::BindGlobalFunction("bool IsAnyItemHovered()", FUNC_TRIVIAL(ImGuiWrappers::IsAnyItemHovered));
    FAngelscriptBinds::BindGlobalFunction("bool IsAnyItemActive()", FUNC_TRIVIAL(ImGuiWrappers::IsAnyItemActive));

    // ID / Scope
    FAngelscriptBinds::BindGlobalFunction("void PushID(FString StrID)", FUNC_TRIVIAL(ImGuiWrappers::PushID));
    FAngelscriptBinds::BindGlobalFunction("void PushID(int IntID)", FUNC_TRIVIAL(ImGuiWrappers::PushIDInt));
    FAngelscriptBinds::BindGlobalFunction("void PopID()", FUNC_TRIVIAL(ImGuiWrappers::PopID));
    FAngelscriptBinds::BindGlobalFunction("void PushItemWidth(float ItemWidth)", FUNC_TRIVIAL(ImGuiWrappers::PushItemWidth));
    FAngelscriptBinds::BindGlobalFunction("void PopItemWidth()", FUNC_TRIVIAL(ImGuiWrappers::PopItemWidth));
    FAngelscriptBinds::BindGlobalFunction("void SetNextItemWidth(float ItemWidth)", FUNC_TRIVIAL(ImGuiWrappers::SetNextItemWidth));

    // Demo
    FAngelscriptBinds::BindGlobalFunction("void ShowDemoWindow(bool& bShow)", FUNC_TRIVIAL(ImGuiWrappers::ShowDemoWindow));
    FAngelscriptBinds::BindGlobalFunction("void ShowUserGuide()", FUNC_TRIVIAL(ImGuiWrappers::ShowUserGuide));
}

// 静态注册
AS_FORCE_LINK static FAngelscriptBinds::FBind Bind_ImGui_Static(FAngelscriptBinds::EOrder::Normal, &RegisterImGuiBinds);