#pragma once

#include <spdlog/spdlog.h>

using spdlogger = std::shared_ptr<spdlog::logger>;

const std::vector<spdlog::sink_ptr> CreateLogSinks();

spdlogger NewLogger(const std::string& name);

static auto logger = NewLogger("global");