# Unreal ImGui Angel

一个ImGui的Angel脚本绑定插件

需要支持AngelScript的UnrealEngine

目前测试着 AS的热更非常不错 可以在Runtime时写这个ImGui的脚本

## 引擎

- [Unreal Engine Angelscript](https://angelscript.hazelight.se/)

- [Moon Engine](https://github.com/TypeDreamMoon/Moon-Engine)

## 需求的Unreal ImGui

- [UnrealImGui - Type Dream Moon](https://github.com/TypeDreamMoon/UnrealImGui/tree/docking)

## 写法

Demo

```angelscript
    UFUNCTION(BlueprintOverride)
    void Tick(float DeltaSeconds)
    {
        DrawImGui();
    }

    void DrawImGui()
    {
        if (ImGui::Begin("Hello IMGUI", bShowUI))
        {
            ImGui::Text("This is an AngelScript Actor!");
            if (ImGui::Button("Press Me"))
            {
                Print("Button Pressed!", 2.0f);
            }

            if (id != -1)
            {
                ImGui::Image(id, FVector2D(size, size));
            }

            ImGui::ColorEdit4("Debug Color", DebugColor);

            ImGui::Spacing();

            ImGui::Checkbox("Show Test Window", bShowUI);

            ImGui::Text("Move Speed:");
            float32 a = 15;
            float32 b = 500;
            ImGui::SliderFloat("##MoveSpeed", MoveSpeed, a, b);
            ImGui::SliderFloat("Size", size, 0.0f, 500.0f);

            for (int i = 0; i < 10; i++)
            {
                ImGui::Text(f"Test{i}");
                ImGui::Text("AS牛笔");
                ImGui::Separator();
            }

            ImGui::End();
        }

        if (ImGui::Begin("Test", bShowUI))
        {
            ImGui::ColorEdit4("Debug Color", DebugColor);
            ImGui::Separator();
            ImGui::InputText("Actor Label", ActorLabel);
            ImGui::End();
        }
    }
```

## UTexture支持

鉴于UnrealImGui支持UTexture的绘制 我也封装了UTexture的绘制

### 使用方法

#### 初始化

```angelscript
    // BeginPlay时或其他时候 定义一个全局ID 作为Texture的UniqueID
    UPROPERTY()
    UTexture2D Texture;
        
    int id = -1;
    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        Print("AngelScript Actor Started!", 2.0f);
        id = ImGui::RegisterTexture(Texture, FName("MyTexture"));
    }
```

#### 使用

```angelscript
    ImGui::Image(id, FVector2D(128, 128));
```

#### 释放纹理

```angelscript
    UFUNCTION(BlueprintOverride)
    void EndPlay(EEndPlayReason EndPlayReason)
    {
        ImGui::ReleaseTexture(id);
    }
```