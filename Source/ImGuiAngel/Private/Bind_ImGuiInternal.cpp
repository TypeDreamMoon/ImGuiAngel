#include "CoreMinimal.h"
#include "AngelscriptBinds.h"
#include "AngelscriptManager.h"

// 引入 ImGui
#include <imgui.h>
#include <imgui_internal.h>

// 定义不透明句柄，用于在 AngelScript 安全传递 ImGuiWindow* 指针
struct FImGuiWindowHandle
{
	ImGuiWindow* Ptr;
};

// 辅助函数封装
namespace ImGuiInternalWrappers
{
	// =======================================================================
	// 1. Docking Builder (布局构建 - 核心)
	// =======================================================================

	// 将指定窗口停靠到某个 Node ID
	static void DockBuilderDockWindow(FString WindowName, uint32 NodeId)
	{
		ImGui::DockBuilderDockWindow(TCHAR_TO_UTF8(*WindowName), NodeId);
	}

	// 创建一个新的 Dock Node
	static uint32 DockBuilderAddNode(uint32 NodeId, int Flags)
	{
		return ImGui::DockBuilderAddNode(NodeId, Flags);
	}

	// 移除 Node
	static void DockBuilderRemoveNode(uint32 NodeId)
	{
		ImGui::DockBuilderRemoveNode(NodeId);
	}

	// 切割 Node (用于创建分栏布局)
	// Dir: 0=Left, 1=Right, 2=Up, 3=Down
	static uint32 DockBuilderSplitNode(uint32 NodeId, int Dir, float SizeRatio, uint32& OutIdAtDir, uint32& OutIdAtOppositeDir)
	{
		return ImGui::DockBuilderSplitNode(NodeId, static_cast<ImGuiDir>(Dir), SizeRatio, &OutIdAtDir, &OutIdAtOppositeDir);
	}

	// 完成构建
	static void DockBuilderFinish(uint32 NodeId)
	{
		ImGui::DockBuilderFinish(NodeId);
	}

	// =======================================================================
	// 2. Item Flags (高级交互控制)
	// =======================================================================

	// 模拟 PushDisabled / PopDisabled
	static void PushItemFlag(int Option, bool bEnabled)
	{
		// 允许你在脚本里写 PushItemFlag(ImGuiItemFlags.Disabled, true)
		ImGui::PushItemFlag((ImGuiItemFlags)Option, bEnabled);
	}

	static void PopItemFlag()
	{
		ImGui::PopItemFlag();
	}

	// =======================================================================
	// 3. Window & ID Utilities (窗口与ID)
	// =======================================================================

	// 获取 ID (计算 DockNode ID 时很有用)
	static uint32 GetID(FString StrID)
	{
		return ImGui::GetID(TCHAR_TO_UTF8(*StrID));
	}

	// 获取当前窗口句柄
	// 修复：不使用 inline 的 ImGui::GetCurrentWindow()，因为它依赖无法链接的 GImGui 符号
	static FImGuiWindowHandle GetCurrentWindow()
	{
		ImGuiContext* Context = ImGui::GetCurrentContext();
		if (Context && Context->CurrentWindow)
		{
			// 模仿 ImGui::GetCurrentWindow() 的行为
			Context->CurrentWindow->WriteAccessed = true;
			return {Context->CurrentWindow};
		}
		return {nullptr};
	}

	// 强制聚焦某个窗口
	static void FocusWindow(FImGuiWindowHandle Window)
	{
		if (Window.Ptr) ImGui::FocusWindow(Window.Ptr);
	}

	// 强制设置窗口位置
	static void SetWindowPos(FImGuiWindowHandle Window, FVector2D Pos)
	{
		if (Window.Ptr) ImGui::SetWindowPos(Window.Ptr, ImVec2(Pos.X, Pos.Y), 0);
	}
}

// 注册绑定
static void RegisterImGuiInternalBinds()
{
	FAngelscriptBinds::FNamespace Namespace("ImGuiInternal");

	// --- 注册句柄类型 ---
	// 作为一个简单的 POD (Plain Old Data) 传递，不涉及内存管理
	FAngelscriptBinds::ValueClass<FImGuiWindowHandle>("ImGuiWindowHandle", FBindFlags{.bPOD = true});

	// --- Enums ---

	// ImGuiDir (用于 SplitNode)
	auto Dir = FAngelscriptBinds::Enum("ImGuiDir");
	Dir["Left"] = 0;
	Dir["Right"] = 1;
	Dir["Up"] = 2;
	Dir["Down"] = 3;

	// ImGuiItemFlags (只选最有用的)
	auto ItemFlags = FAngelscriptBinds::Enum("ImGuiItemFlags");
	ItemFlags["None"] = 0;
	ItemFlags["Disabled"] = 1 << 2; // 最常用：禁用交互但保持显示
	ItemFlags["ReadOnly"] = 1 << 7; // 只读

	// ImGuiDockNodeFlags
	auto DockNodeFlags = FAngelscriptBinds::Enum("ImGuiDockNodeFlags");
	DockNodeFlags["None"] = 0;
	DockNodeFlags["NoCloseButton"] = 1 << 0; // 其实对应 KeepAliveOnly
	DockNodeFlags["NoResize"] = 1 << 5;

	// --- Functions ---

	// Docking
	FAngelscriptBinds::BindGlobalFunction("void DockBuilderDockWindow(FString WindowName, uint32 NodeId)", FUNC_TRIVIAL(ImGuiInternalWrappers::DockBuilderDockWindow));
	FAngelscriptBinds::BindGlobalFunction("uint32 DockBuilderAddNode(uint32 NodeId = 0, int Flags = 0)", FUNC_TRIVIAL(ImGuiInternalWrappers::DockBuilderAddNode));
	FAngelscriptBinds::BindGlobalFunction("void DockBuilderRemoveNode(uint32 NodeId)", FUNC_TRIVIAL(ImGuiInternalWrappers::DockBuilderRemoveNode));
	FAngelscriptBinds::BindGlobalFunction("uint32 DockBuilderSplitNode(uint32 NodeId, int Dir, float32 SizeRatio, uint32& OutIdAtDir, uint32& OutIdAtOppositeDir)", FUNC_TRIVIAL(ImGuiInternalWrappers::DockBuilderSplitNode));
	FAngelscriptBinds::BindGlobalFunction("void DockBuilderFinish(uint32 NodeId)", FUNC_TRIVIAL(ImGuiInternalWrappers::DockBuilderFinish));

	// Item Flags
	FAngelscriptBinds::BindGlobalFunction("void PushItemFlag(int Option, bool bEnabled = true)", FUNC_TRIVIAL(ImGuiInternalWrappers::PushItemFlag));
	FAngelscriptBinds::BindGlobalFunction("void PopItemFlag()", FUNC_TRIVIAL(ImGuiInternalWrappers::PopItemFlag));

	// Utilities
	FAngelscriptBinds::BindGlobalFunction("uint32 GetID(FString StrID)", FUNC_TRIVIAL(ImGuiInternalWrappers::GetID));

	// Window Manipulation
	FAngelscriptBinds::BindGlobalFunction("ImGuiWindowHandle GetCurrentWindow()", FUNC_TRIVIAL(ImGuiInternalWrappers::GetCurrentWindow));
	FAngelscriptBinds::BindGlobalFunction("void FocusWindow(ImGuiWindowHandle Window)", FUNC_TRIVIAL(ImGuiInternalWrappers::FocusWindow));
	FAngelscriptBinds::BindGlobalFunction("void SetWindowPos(ImGuiWindowHandle Window, FVector2D Pos)", FUNC_TRIVIAL(ImGuiInternalWrappers::SetWindowPos));
}

// 静态注册
AS_FORCE_LINK static FAngelscriptBinds::FBind Bind_ImGuiInternal_Static(FAngelscriptBinds::EOrder::Normal, &RegisterImGuiInternalBinds);
