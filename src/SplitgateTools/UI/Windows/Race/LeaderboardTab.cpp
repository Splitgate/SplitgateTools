#include "HttpSystem/HttpSystem.h"
#include "LeaderboardTab.h"

#include <ImGuiNotify/ImGuiNotify.hpp>
#include <Globals.h>

static std::vector<std::string> Difficulties = {
    "Easy", "Medium", "Hard"
};

void LeaderboardTab::RenderContent()
{
    if (LoadState != ELoadState::Loaded)
    {
        const char* LoadingText = "Fetching Race Stats...";
        switch (LoadState)
        {
        case ELoadState::Failed:
            LoadingText = "Failed to load Race Stats.";
            break;

        default:
        case ELoadState::Loading:
            LoadingText = "Fetching Race Stats...";
            break;
        };

        ImGuiStyle& Style = ImGui::GetStyle();

        float SizeX = ImGui::CalcTextSize(LoadingText).x + Style.FramePadding.x * 2.0f;
        float SizeY = ImGui::CalcTextSize(LoadingText).y + Style.FramePadding.y * 2.0f;
        ImVec2 Avail = ImGui::GetContentRegionAvail();

        float OffsetX = (Avail.x - SizeX) * 0.5f;
        float OffsetY = (Avail.y - SizeY) * 0.5f;
        if (OffsetX > 0.0f && OffsetY > 0.0f)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + OffsetX);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + OffsetY);
        }

        ImGui::Text(LoadingText);

        return;
    }

    if (ImGui::BeginTabBar("##Difficulty"))
    {
        DrawLeaderboardData("Easy");
        DrawLeaderboardData("Medium");
        DrawLeaderboardData("Hard");

        ImGui::EndTabBar();
    }
}

void LeaderboardTab::FetchLeaderboard()
{
    LoadState = ELoadState::Loading;

    for (auto& Difficulty : Difficulties)
    {
        if (CachedLeaderboard.contains(Difficulty))
            continue;

        httplib::Request MapLeaderboardReq = httplib::Request();
        MapLeaderboardReq.path = std::format("/api/generate-leaderboard?map={}&difficulty={}&length=all", Name, Difficulty);
        MapLeaderboardReq.method = "GET";

        HttpSystem::SendRequest(RACEBASE_URL, MapLeaderboardReq, [&](httplib::Response Resp, httplib::Error Err)
        {
            if (Err == httplib::Error::Success)
            {
                nlohmann::json LeaderboardData = nlohmann::json::parse(Resp.body);
                std::vector<LeaderboardEntry> Entries = LeaderboardData.get<std::vector<LeaderboardEntry>>();

                CachedLeaderboard[Difficulty] = Entries;
                UE_LOG(LogHttp, Display, "Adding Cached Entry {} {}", Name, Difficulty);

                if (CachedLeaderboard.size() == 3) // All data, let it render now
                {
                    LoadState = ELoadState::Loaded;
                }
            }
            else
            {
                ImGui::InsertNotification({
                    ImGuiToastType::Error, 5000, "Failed to receive leaderboard data %s - %s",
                    httplib::to_string(Err).c_str(), Resp.body.c_str()
                });
            }
        });
    }

    if (CachedLeaderboard.size() == 3) // All data, let it render now
    {
        LoadState = ELoadState::Loaded;
    }
}

// Whether to use th,nd,rd or st
const char* Suffix(__int64 Value)
{
    switch (Value % 100)
    {
    case 11: case 12: case 13:
        return "th";

    default:
        switch (Value % 10)
        {
            default:
                return "th";
            case 1:
                return "st";
            case 2:
                return "nd";
            case 3:
                return "rd";
        }
    }
}

std::string FormatTime(int Milliseconds)
{
    std::chrono::system_clock::time_point TimePoint{ std::chrono::milliseconds{Milliseconds} };
    auto TimePointCast = std::chrono::time_point_cast<std::chrono::milliseconds>(TimePoint);

    std::time_t TimeT = std::chrono::system_clock::to_time_t(TimePointCast);

    tm Time{};
    localtime_s(&Time, &TimeT);

    if (Time.tm_hour != 0)
    {
        return std::format("{0:%H}:{0:%M}:{0:%S}", TimePointCast);
    };

    return std::format("{0:%M}:{0:%S}", TimePointCast);
}

void LeaderboardTab::DrawLeaderboardData(const char* Difficulty)
{
    //ImGui::Text(CachedLeaderboard.first.c_str());
    if (ImGui::BeginTabItem(Difficulty))
    {
        ImGui::BeginChild("LeaderboardScrollableData", { 0, 0, }, ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
        {
            std::string FormattedName = std::format("Leaderboard_{}_{}", Name, Difficulty);
            if (ImGui::BeginTable(FormattedName.c_str(), 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("Position");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Time");
                ImGui::TableHeadersRow();

                std::vector<LeaderboardEntry> EntryList = CachedLeaderboard[Difficulty];
                for (auto& Entry : EntryList)
                {
                    ImGui::TableNextColumn();

                    __int64 Position = (std::find(EntryList.begin(), EntryList.end(), Entry) - EntryList.begin()) + 1;

                    std::string PositionString = std::to_string(Position) + Suffix(Position); //std::format("{}{}", Position, Suffix(Position));
                    ImGui::Text(PositionString.c_str());
                    ImGui::TableNextColumn();

                    ImGui::Text(Entry.displayName.c_str());
                    ImGui::TableNextColumn();

                    ImGui::Text(FormatTime(Entry.value).c_str());
                    ImGui::TableNextColumn();

                    //ImGui::Text(Entry.compositeUserId.platform.c_str());
                    //ImGui::TableNextColumn();

                    // Ignore adding a new row if last entry
                    if (Position != EntryList.size())
                        ImGui::TableNextRow(ImGuiTableFlags_Borders, 20);
                }

                ImGui::EndTable();
            }

            ImGui::EndChild();
        }

        ImGui::EndTabItem();
    }
}

void LeaderboardTab::OnSelected()
{
    FetchLeaderboard();
}
