#include "main.hpp"
#include "SettingsViewController.hpp"
#include "ColorManager.hpp"
#include "ModConfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(QonsistentSaberColors, SettingsViewController)

void QonsistentSaberColors::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(!firstActivation)
        return;

    auto transform = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform())->get_transform();

    QuestUI::BeatSaberUI::CreateToggle(transform, "Enabled", getModConfig().Enabled.GetValue(), [](bool value)
    {
        getModConfig().Enabled.SetValue(value);
        QonsistentSaberColors::UpdateControllerColors();
        QonsistentSaberColors::UpdateLaserColor();
    });

    QuestUI::BeatSaberUI::CreateToggle(transform, "Colored lasers", getModConfig().ColoredLasers.GetValue(), [](bool value)
    {
        getModConfig().ColoredLasers.SetValue(value);
        QonsistentSaberColors::UpdateLaserColor();
    });
}