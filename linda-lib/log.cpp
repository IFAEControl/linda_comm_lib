#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "log.hpp"

const std::vector<spdlog::sink_ptr> CreateLogSinks() {
    namespace spsinks = spdlog::sinks;

    std::vector<spdlog::sink_ptr> sinks;

    auto s = std::make_shared<spsinks::stdout_color_sink_mt>();
    sinks.emplace_back(s);
    sinks.back()->set_level(spdlog::level::info);

#ifdef _WIN32
    std::string tmp_dir = "C:\\Windows\\Temp\\";
#else
    std::string tmp_dir = "/tmp/";
#endif

    sinks.emplace_back(
        std::make_shared<spsinks::basic_file_sink_mt>(tmp_dir + "linda.log", false));
    sinks.back()->set_level(spdlog::level::trace);

    return sinks;
}

spdlogger NewLogger(const std::string& name) {
    {
        auto logger = spdlog::get(name);
        if(logger != nullptr) return logger;
    }

    auto sinks = CreateLogSinks();
    auto logger = std::make_shared<spdlog::logger>(name, begin(sinks), end(sinks));
    spdlog::register_logger(logger);
    logger->set_level(spdlog::level::trace);

    // XXX: Flush level should be warning but then the order
    // of messages is not preserved
    logger->flush_on(spdlog::level::trace);

    return logger;
}
