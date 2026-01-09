#include "CoreMinimal.h"
#include "AngelscriptBinds.h"
#include "AngelscriptManager.h"
#include "Modules/ModuleManager.h"

// UnrealImGui Plugin Headers
#include "ImGuiModule.h"
#include "ImGuiTextureHandle.h"
#include <imgui.h>

namespace ImGuiImageWrappers
{
    // 缓存映射：TextureID -> TextureName
    // 用于在 ReleaseTexture 时通过 ID 找回注册时的 Name
    static TMap<int32, FName> GImGuiTextureNameCache;

    /**
     * 将 Unreal 的 UTexture 注册到 ImGui。
     */
    static int32 RegisterTexture(UTexture* Texture, FName Name)
    {
        if (!Texture)
        {
            return -1;
        }

        // 获取 ImGui 模块实例
        FImGuiModule& Module = FImGuiModule::Get();
        
        // 使用公开接口 RegisterTexture，它返回一个 Handle
        FImGuiTextureHandle Handle = Module.RegisterTexture(Name, Texture, false);
        
        // 获取 ImTextureID (通常是 void*) 并转换为 int32
        int32 TextureID = (int32)(intptr_t)Handle.GetTextureId();

        // 记录 ID 到 Name 的映射，以便后续释放
        GImGuiTextureNameCache.Add(TextureID, Name);

        return TextureID;
    }

    /**
     * 释放之前注册的纹理资源。
     */
    static void ReleaseTexture(int32 TextureID)
    {
        if (TextureID < 0)
        {
            return;
        }

        // 检查是否有该 ID 的记录
        FName Name;
        if (GImGuiTextureNameCache.RemoveAndCopyValue(TextureID, Name))
        {
            // 确保模块仍然加载
            if (FModuleManager::Get().IsModuleLoaded("ImGui"))
            {
                FImGuiModule& Module = FImGuiModule::Get();
                
                // 通过 Name 查找 Handle，然后释放
                FImGuiTextureHandle Handle = Module.FindTextureHandle(Name);
                if (Handle.IsValid())
                {
                    Module.ReleaseTexture(Handle);
                }
            }
        }
    }

    /**
     * 绘制纹理到 ImGui 窗口中。
     */
    static void Image(int32 TextureID, FVector2D Size, FVector2D UV0, FVector2D UV1, FLinearColor TintCol, FLinearColor BorderCol)
    {
        if (TextureID < 0)
        {
            return;
        }

        // 将 int32 索引强转回 ImTextureID (void*)
        ImTextureID ImTexID = (ImTextureID)(intptr_t)TextureID;

        ImGui::Image(ImTexID, 
            ImVec2(Size.X, Size.Y), 
            ImVec2(UV0.X, UV0.Y), 
            ImVec2(UV1.X, UV1.Y),
            ImVec4(TintCol.R, TintCol.G, TintCol.B, TintCol.A),
            ImVec4(BorderCol.R, BorderCol.G, BorderCol.B, BorderCol.A)
        );
    }
}

static void RegisterImGuiImageBinds()
{
    FAngelscriptBinds::FNamespace Namespace("ImGui");

    // ================== Image / Texture Functions ==================

    // 注册/更新纹理
    FAngelscriptBinds::BindGlobalFunction("int32 RegisterTexture(UTexture Texture, FName Name)", FUNC_TRIVIAL(ImGuiImageWrappers::RegisterTexture));

    // 释放纹理
    FAngelscriptBinds::BindGlobalFunction("void ReleaseTexture(int32 TextureID)", FUNC_TRIVIAL(ImGuiImageWrappers::ReleaseTexture));

    // 绘制图片 (完整版)
    FAngelscriptBinds::BindGlobalFunction(
        "void Image(int32 TextureID, FVector2D Size, FVector2D UV0 = FVector2D(0,0), FVector2D UV1 = FVector2D(1,1), FLinearColor TintCol = FLinearColor::White, FLinearColor BorderCol = FLinearColor::Transparent)", 
        FUNC_TRIVIAL(ImGuiImageWrappers::Image)
    );
}

// 静态注册
AS_FORCE_LINK static FAngelscriptBinds::FBind Bind_ImGuiImg_Static(FAngelscriptBinds::EOrder::Normal, &RegisterImGuiImageBinds);