#include "main.hpp"
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

GlobalNamespace::VRController* leftController;
GlobalNamespace::VRController* rightController;

UnityEngine::Color leftSaberColor;
UnityEngine::Color rightSaberColor;

const UnityEngine::Color defaultLeftColor {0.784314, 0.078431, 0.078431, 1.000000}; 
const UnityEngine::Color defaultRightColor{0.156863, 0.556863, 0.823529, 1.000000};

namespace QonsistentSaberColors
{
    void GetColorScheme()
    {
        auto colorSchemeSettings = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::PlayerDataModel*>().First()->playerData->colorSchemesSettings;
        if(colorSchemeSettings->overrideDefaultColors)
        {
            auto scheme = colorSchemeSettings->GetSelectedColorScheme();
            leftSaberColor = scheme->saberAColor;
            rightSaberColor = scheme->saberBColor;
        }
        else
        {
            leftSaberColor = defaultLeftColor;
            rightSaberColor = defaultRightColor;
        }
    }

    void GetControllers()
    {
        getLogger().info("Finding VRControllers via resources");
        auto controllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
        leftController = controllers[1];
        rightController = controllers[0];
    }

    void SetColors()
    {
        GetColorScheme();
        SetColors(leftSaberColor, rightSaberColor);
    }

    void SetColors(UnityEngine::Color leftColor, UnityEngine::Color rightColor)
    {
        getLogger().info("Setting controller colors");

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
        getLogger().info("Done setting colors");
    }

    void ResetColors()
    {
        SetColors(defaultLeftColor, defaultRightColor);
    }
}