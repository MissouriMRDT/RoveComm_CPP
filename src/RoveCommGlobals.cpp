// Forward Declarations
quill::Logger* g_qFileLogger;
quill::Logger* g_qConsoleLogger;
quill::Logger* g_qSharedLogger;

/******************************************************************************
 * @brief Set up project wide logger.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-25
 ******************************************************************************/
void InitializeLoggers()
{
    // Retrieve the current time for the log file name
    time_t tCurrentTime   = time(nullptr);
    struct tm sTimeStruct = *localtime(&tCurrentTime);
    char cCurrentTime[80];

    // Format the current time in a format that can be used as a file name
    std::strftime(cCurrentTime, sizeof(cCurrentTime), "%Y%m%d-%H%M%S", &sTimeStruct);

    // Turn the current time into a file name
    std::string szFilenameWithExtension;
    szFilenameWithExtension = cCurrentTime;
    szFilenameWithExtension += ".log";

    // Create Handlers
    std::shared_ptr<quill::Handler> qFileHandler    = quill::rotating_file_handler(szFilenameWithExtension, "w", quill::FilenameAppend::None, 1024, 5);
    std::shared_ptr<quill::Handler> qConsoleHandler = quill::stdout_handler();

    // Configure Patterns
    qFileHandler->set_pattern("%(ascii_time) %(level_name) [%(process)] [%(thread)] %(message)",       // format
                              "%Y-%m-%d %H:%M:%S.%Qms",                                                // timestamp format
                              quill::Timezone::GmtTime);                                               // timestamp's timezone

    qConsoleHandler->set_pattern("%(ascii_time) %(level_name) [%(process)] [%(thread)] %(message)",    // format
                                 "%Y-%m-%d %H:%M:%S.%Qms",                                             // timestamp format
                                 quill::Timezone::GmtTime);                                            // timestamp's timezone

    // Enable Color Console
    static_cast<quill::ConsoleHandler*>(qConsoleHandler.get())->enable_console_colours();

    // Configure Quill
    quill::Config qConfig;
    qConfig.enable_console_colours = true;
    qConfig.default_handlers.emplace_back(qConsoleHandler);
    quill::configure(qConfig);

    // Start Quill
    quill::start();

    // Create Loggers
    g_qFileLogger    = quill::create_logger("FILE_LOGGER", {qFileHandler});
    g_qConsoleLogger = quill::create_logger("CONSOLE_LOGGER", {qConsoleHandler});
    g_qSharedLogger  = quill::create_logger("SHARED_LOGGER", {qFileHandler, qConsoleHandler});

    // Set Logging Levels
    g_qFileLogger->set_log_level(quill::LogLevel::TraceL3);
    g_qConsoleLogger->set_log_level(quill::LogLevel::TraceL3);
    g_qSharedLogger->set_log_level(quill::LogLevel::TraceL3);

    // // Enable Backtrace
    g_qFileLogger->init_backtrace(2, quill::LogLevel::Critical);
    g_qConsoleLogger->init_backtrace(2, quill::LogLevel::Critical);
    g_qSharedLogger->init_backtrace(2, quill::LogLevel::Critical);
}