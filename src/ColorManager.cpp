#include "main.hpp"
#include "ModConfig.hpp"
#include "ColorManager.hpp"

#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorSchemesSettings.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/ColorManager.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"

#include "VRUIControls/VRInputModule.hpp"
#include "VRUIControls/VRLaserPointer.hpp"
#include "VRUIControls/VRPointer.hpp"
#include "UnityEngine/XR/XRNode.hpp"

VRUIControls::VRInputModule* inputModule;

GlobalNamespace::ColorSchemesSettings* colorSchemesSettings;

const UnityEngine::Color defaultLeftColor {0.784314, 0.078431, 0.078431, 1.000000}; 
const UnityEngine::Color defaultRightColor{0.156863, 0.556863, 0.823529, 1.000000};
const UnityEngine::Color defaultLaserColor{0.125490f, 0.752941f, 1.000000f, 0.501961f};

namespace QonsistentSaberColors {
    
    UnityEngine::Color GetLeftColor()
    {
        auto scheme = colorSchemesSettings->GetSelectedColorScheme();
        return (colorSchemesSettings->overrideDefaultColors && getModConfig().Enabled.GetValue()) ? colorSchemesSettings->GetSelectedColorScheme()->saberAColor : defaultLeftColor;
    }

    UnityEngine::Color GetRightColor()
    {
        auto scheme = colorSchemesSettings->GetSelectedColorScheme();
        return (colorSchemesSettings->overrideDefaultColors && getModConfig().Enabled.GetValue()) ? colorSchemesSettings->GetSelectedColorScheme()->saberBColor : defaultRightColor;
    }

    void UpdatePointers()
    {
        getLogger().info("Updating pointers");

        inputModule = UnityEngine::Resources::FindObjectsOfTypeAll<VRUIControls::VRInputModule*>()[0];

        colorSchemesSettings = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::PlayerDataModel*>()[0]->playerData->colorSchemesSettings;
    }

    void SetControllerColor(GlobalNamespace::VRController* controller, UnityEngine::Color color)
    {
        auto glowColors = controller->GetComponentsInChildren<GlobalNamespace::SetSaberGlowColor*>();
        for(auto glow : glowColors)
        {
            auto glowColor = glow ->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberGlowColor*>();
            auto scheme = glow->colorManager->colorScheme;
            controller->node == UnityEngine::XR::XRNode::LeftHand ? scheme->saberAColor = color : scheme->saberBColor = color; 
            glowColor->SetColors();
        }

        auto fakeGlowColors = controller->GetComponentsInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
        for(auto fake : fakeGlowColors)
        {
            auto glowColor = fake->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
            auto scheme = glowColor->colorManager->colorScheme;
            controller->node == UnityEngine::XR::XRNode::LeftHand ? scheme->saberAColor = color : scheme->saberBColor = color;
            glowColor->SetColors();
        }
    }

    void SetLaserColor(VRUIControls::VRLaserPointer* pointer, UnityEngine::Color color)
    {
        auto renderer = pointer->GetComponentInChildren<UnityEngine::MeshRenderer*>();
        auto matArray = renderer->GetMaterialArray();
        for(auto mat: matArray)
        {
            mat->set_color(color);
        }
    }

    void UpdateControllerColors()
    {
        if(!inputModule || !inputModule->m_CachedPtr.m_value)
            return;
        SetControllerColor(inputModule->vrPointer->leftVRController, GetLeftColor());
        SetControllerColor(inputModule->vrPointer->rightVRController, GetRightColor());
    }

    void UpdateLaserColor()
    {
        if(!inputModule || !inputModule->m_CachedPtr.m_value)
            return;

        UnityEngine::Color color;
        if(getModConfig().ColoredLasers.GetValue() && getModConfig().Enabled.GetValue())
        {
            auto parent = inputModule->vrPointer->laserPointer->get_transform()->get_parent()->get_name();
            color = parent == "ControllerLeft" ? GetLeftColor() : GetRightColor();
        }
        else
            color = defaultLaserColor;

        color.a = 0;
        SetLaserColor(inputModule->vrPointer->laserPointer, color);
    }
}