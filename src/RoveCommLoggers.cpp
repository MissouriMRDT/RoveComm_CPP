/******************************************************************************
 * @brief Defines functions and objects used at the global scope for when
 *        testing the functionality of RoveComm C++.
 *
 *        Note: This is not compiled or packaged into the library that is
 *              distributed into other projects.
 *
 *
 * @file RoveCommLoggers.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#if !defined(__ROVECOMM_LIBRARY_MODE__) || (__ROVECOMM_LIBRARY_MODE__ == 0)

#include "RoveCommLoggers.h"

/// \cond
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>

/// \endcond

/******************************************************************************
 * @brief Namespace containing all global type/structs that will be used project wide
 *      for logging.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-08-20
 ******************************************************************************/
namespace logging
{
    /////////////////////////////////////////
    // Forward declarations for namespace variables and objects.
    /////////////////////////////////////////
    quill::Logger* g_qFileLogger;
    quill::Logger* g_qConsoleLogger;
    quill::Logger* g_qSharedLogger;

    quill::LogLevel g_eConsoleLogLevel;
    quill::LogLevel g_eFileLogLevel;

    std::string g_szProgramStartTimeString;

    // Logging constants.
    const std::string LOGGING_OUTPUT_PATH_ABSOLUTE = "../logs/";                  // The absolute to write output logging and video files to.
    const quill::LogLevel CONSOLE_MIN_LEVEL        = quill::LogLevel::TraceL3;    // The minimum logging level that is allowed to send to the console log stream.
    const quill::LogLevel FILE_MIN_LEVEL           = quill::LogLevel::TraceL3;    // The minimum logging level that is allowed to send to the file log streams.
    const quill::LogLevel CONSOLE_DEFAULT_LEVEL    = quill::LogLevel::Info;       // The default logging level for console stream.
    const quill::LogLevel FILE_DEFAULT_LEVEL       = quill::LogLevel::TraceL3;    // The default logging level for file streams.

    // Logging color constants.
    const std::string szTraceL3Color   = "\033[30m";           // Standard Grey
    const std::string szTraceL2Color   = "\033[30m";           // Standard Grey
    const std::string szTraceL1Color   = "\033[30m";           // Standard Grey
    const std::string szDebugColor     = "\033[36m";           // Standard Cyan
    const std::string szInfoColor      = "\033[32m";           // Standard Green
    const std::string szNoticeColor    = "\033[97m\033[1m";    // Bright Bold White
    const std::string szWarningColor   = "\033[93m\033[1m";    // Bright Bold Yellow
    const std::string szErrorColor     = "\033[91m\033[1m";    // Bright Bold Red
    const std::string szCriticalColor  = "\033[95m\033[1m";    // Bright Bold Magenta
    const std::string szBacktraceColor = "\033[34m";           // Standard Blue

    /******************************************************************************
     * @brief Logger Initializer - Sets Up all the logging handlers required for
     *        having the above loggers.
     *
     * @param szLoggingOutputPath - A string containing the filepath to output log files to.
     *                      Must be properly formatted.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2023-08-22
     ******************************************************************************/
    void InitializeLoggers(std::string szLoggingOutputPath)
    {
        // Retrieve the current time for the log file name
        std::chrono::time_point<std::chrono::system_clock> tmCurrentTime = std::chrono::system_clock::now();
        std::time_t tCurrentTime                                         = std::chrono::system_clock::to_time_t(tmCurrentTime);

        // Convert time to local time
        std::tm* tLocalTime = std::localtime(&tCurrentTime);

        // Format the current time in a format that can be used as a file name
        char cCurrentTime[80];
        std::strftime(cCurrentTime, sizeof(cCurrentTime), "%Y%m%d-%H%M%S", tLocalTime);

        // Store start time string in member variable.
        g_szProgramStartTimeString = cCurrentTime;

        // Assemble filepath string.
        std::filesystem::path szFilePath;
        std::filesystem::path szFilename;
        szFilePath = szLoggingOutputPath + "/";            // Main location for all recordings.
        szFilePath += g_szProgramStartTimeString + "/";    // Folder for each program run.
        szFilename = "console_output";                     // Base file name.

        // Check if directory exists.
        if (!std::filesystem::exists(szFilePath))
        {
            // Create directory.
            if (!std::filesystem::create_directories(szFilePath))
            {
                // Submit logger message.
                std::cerr << "Unable to create the logging output directory: " << szFilePath.string() << " for console output file." << std::endl;
            }
        }
        else
        {
            // Submit logger message.
            std::cerr << "Unable to create logging output directory " << szFilePath.string() << ": it already exists." << std::endl;
        }

        // Construct the full output path.
        std::filesystem::path szFullOutputPath = szFilePath / szFilename;

        // Set Console Color Profile
        quill::ConsoleColours qColors;
        qColors.set_default_colours();
        qColors.set_colour(quill::LogLevel::TraceL3, szTraceL3Color);
        qColors.set_colour(quill::LogLevel::TraceL2, szTraceL2Color);
        qColors.set_colour(quill::LogLevel::TraceL1, szTraceL1Color);
        qColors.set_colour(quill::LogLevel::Debug, szDebugColor);
        qColors.set_colour(quill::LogLevel::Info, szInfoColor);
        qColors.set_colour(quill::LogLevel::Warning, szWarningColor);
        qColors.set_colour(quill::LogLevel::Error, szErrorColor);
        qColors.set_colour(quill::LogLevel::Critical, szCriticalColor);
        qColors.set_colour(quill::LogLevel::Backtrace, szBacktraceColor);

        // Create Patterns
        std::string szLogFilePattern   = "%(time) %(log_level) [%(thread_id)] [%(file_name):%(line_number)] %(message)";
        std::string szCSVFilePattern   = "%(time),\t%(log_level),\t[%(thread_id)],\t[%(file_name):%(line_number)],\t\"%(message)\"";
        std::string szConsolePattern   = "%(time) %(log_level:8) [%(thread_id)] [%(file_name):%(line_number)] %(message)";
        std::string szTimestampPattern = "%Y-%m-%d %H:%M:%S.%Qms";

        // Create Sinks
        std::shared_ptr<quill::Sink> qLogFileSink = quill::Frontend::create_or_get_sink<MRDTRotatingFileSink>(
            szFullOutputPath.replace_extension(".log"),    // Log Output Path
            []()
            {
                return quill::RotatingFileSinkConfig();    // Rotating File Sink Configs
            }(),
            szLogFilePattern,                              // Log Output Pattern
            szTimestampPattern,                            // Log Timestamp Pattern
            quill::Timezone::LocalTime                     // Log Timezone
        );

        std::shared_ptr<quill::Sink> qCSVFileSink = quill::Frontend::create_or_get_sink<MRDTRotatingFileSink>(
            szFullOutputPath.replace_extension(".csv"),    // Log Output Path
            []()
            {
                return quill::RotatingFileSinkConfig();    // Rotating File Sink Configs
            }(),
            szCSVFilePattern,                              // Log Output Pattern
            szTimestampPattern,                            // Log Timestamp Pattern
            quill::Timezone::LocalTime                     // Log Timezone
        );

        std::shared_ptr<quill::Sink> qConsoleSink = quill::Frontend::create_or_get_sink<MRDTConsoleSink>("ConsoleSink",        // Log Name
                                                                                                         qColors,              // Log Custom Colors
                                                                                                         szConsolePattern,     // Log Output Pattern
                                                                                                         szTimestampPattern    // Log Timestamp Pattern
        );

        // Configure Quill
        quill::BackendOptions qBackendConfig;

        // Start Quill
        quill::Backend::start(qBackendConfig);

        // Set Handler Filters
        qLogFileSink->add_filter(std::make_unique<LoggingFilter>("LogFileFilter", quill::LogLevel::TraceL3));
        qCSVFileSink->add_filter(std::make_unique<LoggingFilter>("CSVFileFilter", quill::LogLevel::TraceL3));
        qConsoleSink->add_filter(std::make_unique<LoggingFilter>("ConsoleFilter", quill::LogLevel::TraceL3));

        // Create Loggers
        g_qFileLogger    = quill::Frontend::create_or_get_logger("FILE_LOGGER", {qLogFileSink, qCSVFileSink});
        g_qConsoleLogger = quill::Frontend::create_or_get_logger("CONSOLE_LOGGER", {qConsoleSink});
        g_qSharedLogger  = quill::Frontend::create_or_get_logger("SHARED_LOGGER", {qLogFileSink, qCSVFileSink, qConsoleSink});

        // Set Internal Logging Level Limiters
        g_eConsoleLogLevel = CONSOLE_DEFAULT_LEVEL;
        g_eFileLogLevel    = FILE_DEFAULT_LEVEL;

        // Set Base Logging Levels
        g_qFileLogger->set_log_level(quill::LogLevel::TraceL3);
        g_qConsoleLogger->set_log_level(quill::LogLevel::TraceL3);
        g_qSharedLogger->set_log_level(quill::LogLevel::TraceL3);

        // Enable Backtrace
        g_qFileLogger->init_backtrace(2, quill::LogLevel::Critical);
        g_qConsoleLogger->init_backtrace(2, quill::LogLevel::Critical);
        g_qSharedLogger->init_backtrace(2, quill::LogLevel::Critical);
    }

    /******************************************************************************
     * @brief Writes a log message to the MRDT console sink, formats the message
     * using the provided formatter, and then passes the formatted log message
     * along with the original data to the parent class (ConsoleSink) for handling.
     *
     * @param log_metadata - Metadata about the log statement (e.g., file, line number).
     * @param log_timestamp - The timestamp of the log statement.
     * @param thread_id - The ID of the thread that generated the log.
     * @param thread_name - The name of the thread that generated the log.
     * @param process_id - The ID of the process that generated the log.
     * @param logger_name - The name of the logger that generated the log.
     * @param log_level - The level/severity of the log statement.
     * @param log_level_description - A description of the log level.
     * @param log_level_short_code - A short code representing the log level.
     * @param named_args - Optional named arguments passed with the log statement.
     * @param log_message - The actual log message content.
     *
     * @note This method calls the base class's `write_log` function to actually
     * handle the log output, after formatting the message with custom formatting logic.
     *
     * @note This method should not be called directly. It is meant to be invoked
     * by the logging framework as part of the log handling process.
     *
     * @see quill::ConsoleSink
     *
     * @warning Ensure that the formatter is correctly configured before using this
     * method, as improper formatting may lead to incorrect log outputs.
     *
     * @attention This method overrides the base class's write_log function to
     * inject custom formatting logic while preserving the core file logging functionality.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-08-16
     ******************************************************************************/
    void MRDTConsoleSink::write_log(quill::MacroMetadata const* log_metadata,
                                    uint64_t log_timestamp,
                                    std::string_view thread_id,
                                    std::string_view thread_name,
                                    std::string const& process_id,
                                    std::string_view logger_name,
                                    quill::LogLevel log_level,
                                    std::string_view log_level_description,
                                    std::string_view log_level_short_code,
                                    std::vector<std::pair<std::string, std::string>> const* named_args,
                                    std::string_view log_message,
                                    std::string_view)
    {
        // Format the log message
        std::string_view szvFormattedLogMessage = _formatter.format(log_timestamp,            // Timestamp
                                                                    thread_id,                // Thread ID
                                                                    thread_name,              // Thread name
                                                                    process_id,               // Process ID
                                                                    logger_name,              // Logger name
                                                                    log_level_description,    // Log level description
                                                                    log_level_short_code,     // Log level short code
                                                                    *log_metadata,            // Log statement metadata
                                                                    named_args,               // Named arguments
                                                                    log_message               // Log message
        );

        // Check if logging level is permitted
        if (static_cast<int>(g_eConsoleLogLevel) <= static_cast<int>(log_level))
        {
            quill::ConsoleSink::write_log(log_metadata,             // Metadata
                                          log_timestamp,            // Timestamp
                                          thread_id,                // Thread ID
                                          thread_name,              // Thread Name
                                          process_id,               // Process ID
                                          logger_name,              // Logger name
                                          log_level,                // Log level
                                          log_level_description,    // Log level description
                                          log_level_short_code,     // Log level short code
                                          named_args,               // Named arguments
                                          log_message,              // Log Message
                                          szvFormattedLogMessage    // Formatted Log Message
            );
        }
    }

    /******************************************************************************
     * @brief Writes a log message to the MRDT rotating file sink. The log message
     * is first formatted using a custom formatter, and then the formatted message
     * along with the original log details are passed to the base class
     * (RotatingFileSink) for further handling (such as writing to a rotating log file).
     *
     * @param log_metadata - Metadata about the log statement (e.g., file, line number).
     * @param log_timestamp - The timestamp of the log statement.
     * @param thread_id - The ID of the thread that generated the log.
     * @param thread_name - The name of the thread that generated the log.
     * @param process_id - The ID of the process that generated the log.
     * @param logger_name - The name of the logger that generated the log.
     * @param log_level - The level/severity of the log statement.
     * @param log_level_description - A description of the log level.
     * @param log_level_short_code - A short code representing the log level.
     * @param named_args - Optional named arguments passed with the log statement.
     * @param log_message - The actual log message content.
     *
     * @note This method formats the log message using the provided formatter,
     * ensuring that the final output adheres to the defined format pattern. The
     * formatted message is then handled by the rotating file sink for writing to
     * a file that rotates based on file size or time interval.
     *
     * @note This method should not be called directly. It is meant to be invoked
     * by the logging framework as part of the log handling process.
     *
     * @see quill::RotatingFileSink
     *
     * @warning Ensure that the formatter is correctly configured and that the
     * rotating file sink is properly set up to avoid loss of log data.
     *
     * @attention This method overrides the base class's write_log function to
     * inject custom formatting logic while preserving the core file logging functionality.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-08-16
     ******************************************************************************/
    void MRDTRotatingFileSink::write_log(quill::MacroMetadata const* log_metadata,
                                         uint64_t log_timestamp,
                                         std::string_view thread_id,
                                         std::string_view thread_name,
                                         std::string const& process_id,
                                         std::string_view logger_name,
                                         quill::LogLevel log_level,
                                         std::string_view log_level_description,
                                         std::string_view log_level_short_code,
                                         std::vector<std::pair<std::string, std::string>> const* named_args,
                                         std::string_view log_message,
                                         std::string_view)
    {
        // Format the log message
        std::string_view szvFormattedLogMessage = _formatter.format(log_timestamp,            // Timestamp
                                                                    thread_id,                // Thread ID
                                                                    thread_name,              // Thread name
                                                                    process_id,               // Process ID
                                                                    logger_name,              // Logger name
                                                                    log_level_description,    // Log level description
                                                                    log_level_short_code,     // Log level short code
                                                                    *log_metadata,            // Log statement metadata
                                                                    named_args,               // Named arguments
                                                                    log_message               // Log message
        );

        // Check if logging level is permitted
        if (static_cast<int>(g_eFileLogLevel) <= static_cast<int>(log_level))
        {
            quill::RotatingFileSink::write_log(log_metadata,             // Metadata
                                               log_timestamp,            // Timestamp
                                               thread_id,                // Thread ID
                                               thread_name,              // Thread Name
                                               process_id,               // Process ID
                                               logger_name,              // Logger name
                                               log_level,                // Log level
                                               log_level_description,    // Log level description
                                               log_level_short_code,     // Log level short code
                                               named_args,               // Named arguments
                                               log_message,              // Log Message
                                               szvFormattedLogMessage    // Formatted Log Message
            );
        }
    }
}    // namespace logging

#endif    // __ROVECOMM_LIBRARY_MODE__
