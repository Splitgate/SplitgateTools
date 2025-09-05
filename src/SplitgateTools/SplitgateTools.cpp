#include "SplitgateTools.h"
#include "Games/BaseGame.h"
#include "Globals.h"

void SplitgateTools::Main()
{
	SetupLogger();

	UE_LOG(LogInit, Display, "Starting up Splitgate Tools! Created by Adam & Ruby Rain");

	Game = BaseGame::Create();
	Game->Init_PreEngine();
}

void SplitgateTools::SetupLogger()
{
	AllocConsole();
	FILE* File;
	freopen_s(&File, "CONOUT$", "w", stdout);

	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_pattern("%^[%H:%M:%S] %v%$");

	spdlog::logger logger("multi_sink", { console_sink }); // TODO: imgui sink
	logger.set_level(spdlog::level::level_enum::debug);

	spdlog::register_logger(std::make_shared<spdlog::logger>(logger));
	spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
	spdlog::flush_every(std::chrono::seconds(1));
}