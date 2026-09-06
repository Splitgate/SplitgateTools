#include "ForgeMyMapsTab.h"
#include "ForgeUtility.h"

#include "Globals.h"
#include "Globals/FForgeMap.h"

#include "Misc/FGuid.h"

#include "Engine/UWorld.h"
#include "Engine/UGameInstance.h"
#include "GameFramework/APlayerController.h"
#include "Save/UPortalWarsSaveGame.h"
#include "Kismet/UPortalWarsGameplayStatics.h"
#include "Kismet/UBlueprintMapLibrary.h"

#include "nlohmann/json.hpp"
#include "Strings/Strings.h"

#include <fstream>

void ForgeMyMapsTab::RenderContent()
{
    if (!GWorld) return;

    // 
    // TEMP Hack --- Disallow in lobby as it causes issues when the game modifies or reads this tmap
    if (GWorld->Name == "Lobby")
    {
        const char* DisallowText = "Disallowed in lobby to prevent crashes.";

        ImGuiStyle& Style = ImGui::GetStyle();

        float SizeX = ImGui::CalcTextSize(DisallowText).x + Style.FramePadding.x * 2.0f;
        float SizeY = ImGui::CalcTextSize(DisallowText).y + Style.FramePadding.y * 2.0f;
        ImVec2 Avail = ImGui::GetContentRegionAvail();

        float OffsetX = (Avail.x - SizeX) * 0.5f;
        float OffsetY = (Avail.y - SizeY) * 0.5f;
        if (OffsetX > 0.0f && OffsetY > 0.0f)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + OffsetX);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + OffsetY);
        }

        ImGui::Text(DisallowText);

        return;
    }

    UGameInstance* GameInstance = GWorld->OwningGameInstance();
    if (!GameInstance) return;

    UPortalWarsSaveGame* SaveGame = UPortalWarsGameplayStatics::GetSaveGameForLocalPlayer(GameInstance->LocalPlayers()[0]);
    if (!SaveGame) return;

    TMap<FString, FForgeMap> ForgeMaps = SaveGame->ForgeMaps();
    if (!ForgeMaps.IsValid()) return;

    ImGui::BeginChild("MyForgeScrollableData", { 0, 0, }, ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
    {
        ImVec2 Avail = ImGui::GetContentRegionAvail();

        if (ImGui::BeginTable("MyMapList", 5, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY, 
            ImVec2(Avail.x, Avail.y)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);

            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Base Map");
            ImGui::TableSetupColumn("Object Count");
            ImGui::TableSetupColumn("Sharing Code");
            ImGui::TableSetupColumn("Options");

            ImGui::TableHeadersRow();

            int MapCounter = 0;
            for (auto& ForgeSave : ForgeMaps)
            {
                FForgeMap& Map = ForgeSave.Value();
                ++MapCounter;
                
                ImGui::TableNextColumn();
                
                ImGui::Text(Map.DisplayName.ToString().c_str());
                ImGui::TableNextColumn();
                
                ImGui::Text(Map.MapName.ToString().c_str());
                ImGui::TableNextColumn();
                
                ImGui::Text("%d Interactables", Map.InteractablesSaveData.Num());
                ImGui::TableNextColumn();
                
                ImGui::Text(Map.SharingCode.ToString().c_str());
                ImGui::TableNextColumn();
                
                char Buf[512];
                sprintf(Buf, "Manage###%d", MapCounter);
                if (ImGui::BeginMenu(Buf))
                {
                    //if (ImGui::MenuItem("Publish **STUB**"))
                    //{
                    //}
                    
                    if (ImGui::MenuItem("Duplicate"))
                    {
                        FGuid NewGuid = FGuid::NewGuid();
                        FString Key = Strings::StringToWide(NewGuid.ToString()).c_str();
                
                        static void* ForgeMaps_P = SaveGame->Class->FindProperty("ForgeMaps");
                        UBlueprintMapLibrary::GenericMap_Add(&SaveGame->ForgeMaps(), ForgeMaps_P, &Key, &Map);
                    }
                
                    if (ImGui::MenuItem("Delete"))
                    {
                        static void* ForgeMaps_P = SaveGame->Class->FindProperty("ForgeMaps");
                        UBlueprintMapLibrary::GenericMap_Remove(&SaveGame->ForgeMaps(), ForgeMaps_P, &ForgeSave.Key());
                    }
                
                    ImGui::EndMenu();
                }
            
                if (MapCounter > ForgeMaps.Num())
                    ImGui::TableNextRow(ImGuiTableFlags_Borders);
            }

            ImGui::EndTable();
        }

        ImGui::EndChild();
    }
}
