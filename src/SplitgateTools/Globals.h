#pragma once
#include "Memory.h"
#include <string>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

//------ Compile time defines
#define D3D12_ALLOWED false		// We dont want to use dx12 until we support it directly for the gui

// Add custom log defines here, these contain nothing as they are used directly for the macro name and so intellisense picks them up
#define LogInit
#define LogRHI

// Defines to shorten the typing rather than inputting the namespace and enum, hijack the debug level to make it a success since we wont use debug really
#define Display spdlog::level::info
#define Warning spdlog::level::warn
#define Error spdlog::level::err
#define Success spdlog::level::debug

#define SPDLOG_LOGGER(Logger, Category, Level, ...) \
{ \
	if (Level == spdlog::level::warn) \
	{ \
		SPDLOG_LOGGER_CALL(Logger, Level, ##Category": " "Warning: " __VA_ARGS__); \
	} \
	else if (Level == spdlog::level::err) \
	{ \
		SPDLOG_LOGGER_CALL(Logger, Level, ##Category": " "Error: " __VA_ARGS__); \
	} \
	else if (Level == spdlog::level::critical) \
	{ \
		SPDLOG_LOGGER_CALL(Logger, Level, ##Category": " "Fatal: " __VA_ARGS__); \
	} \
	else \
	{ \
		SPDLOG_LOGGER_CALL(Logger, Level, ##Category": " __VA_ARGS__); \
	} \
}

#define SPDLOG(Category, Level, ...) SPDLOG_LOGGER(spdlog::default_logger_raw(), Category, Level, __VA_ARGS__)
#define UE_LOG(Category, Level, Msg, ...) SPDLOG(#Category, Level, ##Msg, __VA_ARGS__)

// TODO: maybe rename these
#define LOG_ADDRESS(Addr, DisplayName) \
if (Addr) UE_LOG(LogInit, Display, "Found {} [{}]", DisplayName, Memory::Address(Addr).AsRVA<void*>()) \
else      UE_LOG(LogInit, Warning, "Failed to find {}", DisplayName)

#define LOG_OFFSET(Off, DisplayName) \
if (Off != -1) UE_LOG(LogInit, Display, "Found {} [{}]", DisplayName, Memory::Address(Off).As<void*>()) \
else           UE_LOG(LogInit, Warning, "Failed to find {}", DisplayName)

// return if value == 0
#define check(Value) if (!(Value)) { return; }
// return 0 if value == 0
#define check0(Value) if (!(Value)) { return 0; }
// return 1 if value == 0
#define check1(Value) if (!(Value)) { return 1; }
// continue if value == 0
#define checkC(Value) if (!(Value)) { continue; }
// break if value == 0
#define checkB(Value) if (!(Value)) { break; }

// Use very sparingly

#define WAIT_FOR(a) UE_LOG(LogInit, Display, "Awaiting {}...", #a); while (!(a)) { Sleep(1000); }; if (a) { UE_LOG(LogInit, Display, "{} ready!", #a); }
#define WAIT_FORNAMED(a, b) UE_LOG(LogInit, Display, "Awaiting {}...", a); while (!(b)) { Sleep(1000); }; if (b) { UE_LOG(LogInit, Display, "{} ready!", a); }