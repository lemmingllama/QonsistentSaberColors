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

#include "VRUIControls/VRLaserPointer.hpp"
#include "VRUIControls/VRPointer.hpp"

GlobalNamespace::VRController* leftController;
GlobalNamespace::VRController* rightController;

UnityEngine::Color leftSaberColor;
UnityEngine::Color rightSaberColor;

VRUIControls::VRLaserPointer* laserPointer;

GlobalNamespace::ColorSchemesSettings* colorSchemesSettings;

const UnityEngine::Color defaultLeftColor {0.784314, 0.078431, 0.078431, 1.000000}; 
const UnityEngine::Color defaultRightColor{0.156863, 0.556863, 0.823529, 1.000000};
const UnityEngine::Color defaultLaserColor{0.125490f, 0.752941f, 1.000000f, 0.501961f};

namespace QonsistentSaberColors
{
    void UpdateColors()
    {
        getLogger().info("Updating colorScheme");
        if(colorSchemesSettings->overrideDefaultColors && getModConfig().Enabled.GetValue())
        {
            auto scheme = colorSchemesSettings->GetSelectedColorScheme();
            leftSaberColor = scheme->saberAColor;
            rightSaberColor = scheme->saberBColor;
        }
        else
        {
            leftSaberColor = defaultLeftColor;
            rightSaberColor = defaultRightColor;
        }
    }

    void UpdatePointers()
    {
        getLogger().info("Updating pointers");
        auto controllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
        leftController = controllers[1];
        rightController = controllers[0];

        colorSchemesSettings = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::PlayerDataModel*>()[0]->playerData->colorSchemesSettings;
    }


    void SetControllerColors(UnityEngine::Color leftColor, UnityEngine::Color rightColor)
    {
        if(!leftController->m_CachedPtr.m_value || !leftController || !rightController->m_CachedPtr.m_value || !rightController)
        {
            getLogger().info("One of the controllers was null, updating pointers");
            UpdatePointers();
        }

        auto leftGlowColors = leftController->GetComponentsInChildren<GlobalNamespace::SetSaberGlowColor*>();
        for(auto leftGlow : leftGlowColors)
        {
            auto glowColor = leftGlow ->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberGlowColor*>();
            glowColor->colorManager->colorScheme->saberAColor = leftColor;
            glowColor->colorManager->colorScheme->saberBColor = rightColor;
            glowColor->SetColors();
        }

        auto rightGlowColors = rightController->GetComponentsInChildren<GlobalNamespace::SetSaberGlowColor*>();
        for(auto rightGlow : rightGlowColors)
        {
            auto glowColor = rightGlow ->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberGlowColor*>();
            glowColor->colorManager->colorScheme->saberAColor = leftColor;
            glowColor->colorManager->colorScheme->saberBColor = rightColor;
            glowColor->SetColors();
        }


        auto leftFakeGlowColors = leftController->GetComponentsInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
        for(auto leftFake : leftFakeGlowColors)
        {
            auto glowColor = leftFake->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
            glowColor->colorManager->colorScheme->saberAColor = leftColor;
            glowColor->colorManager->colorScheme->saberBColor = rightColor;
            glowColor->SetColors();
        }

        auto rightFakeGlowColors = rightController->GetComponentsInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
        for(auto rightFake : rightFakeGlowColors)
        {
            auto glowColor = rightFake->get_gameObject()->GetComponentInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();
            glowColor->colorManager->colorScheme->saberAColor = leftColor;
            glowColor->colorManager->colorScheme->saberBColor = rightColor;
            glowColor->SetColors();
        }
    }

    void SetLaserColor(VRUIControls::VRLaserPointer* pointer)
    {
        if(!laserPointer || !laserPointer->m_CachedPtr.m_value)
        {
            getLogger().info("laserPointer was null, not setting color");
            return;
        }
        UnityEngine::Color color;
        if(getModConfig().ColoredLasers.GetValue() && getModConfig().Enabled.GetValue())
        {
            auto parent = pointer->get_transform()->get_parent()->get_name();
            parent == "ControllerLeft" ? color = leftSaberColor : color = rightSaberColor;
        }
        else
            color = defaultLaserColor;
        color.a = 0;
        
        auto renderer = laserPointer->GetComponentInChildren<UnityEngine::MeshRenderer*>();
        auto matArray = renderer->GetMaterialArray();
        for(auto mat: matArray)
        {
            mat->set_color(color);
        }
    }

    void SetLaser(VRUIControls::VRLaserPointer* laser)
    {
        laserPointer = laser;
    }

    void UpdateControllerColors()
    {
        UpdateColors();
        SetControllerColors(leftSaberColor, rightSaberColor);
    }

    void UpdateLaserColor()
    {
        UpdateColors();
        SetLaserColor(laserPointer);
    }
}