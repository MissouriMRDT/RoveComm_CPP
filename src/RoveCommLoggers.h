/******************************************************************************
 * @brief Defines functions and objects used at the global scope for when
 *        testing the functionality of RoveComm C++.
 *
 *        Note: This is not compiled or packaged into the library that is
 *              distributed into other projects.
 *
 *
 * @file RoveCommLoggers.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#if !defined(__ROVECOMM_LIBRARY_MODE__) || (__ROVECOMM_LIBRARY_MODE__ == 0)

/// \cond
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include "quill/backend/PatternFormatter.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/Filesystem.h"

#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/RotatingFileSink.h"

/// \endcond

#ifndef ROVECOMM_LOGGER_H
#define ROVECOMM_LOGGER_H

/******************************************************************************
 * @brief Logging Levels:
 *
 *        Priority > Level     > Description
 *        Level 1  > TRACE_L3  > Unused
 *        Level 2  > TRACE_L2  > Unused
 *        Level 3  > TRACE_L1  > Unused
 *        Level 4  > DEBUG     > Details that would only be useful in a debug environment
 *        Level 5  > INFO      > State Changes, RoveComm Updates GPS/IMU/Autonomy, etc
 *        Level 6  > NOTICE    > Noteworthy events that don't indicate a problem.
 *        Level 7  > WARNING   > Something unexpected happened - application could potentially error soon.
 *        Level 8  > ERROR     > Something went wrong - application could potentially have critical error soon.
 *        Level 9  > CRITICAL  > Something went very wrong - application will exit after logging is sent.
 *
 *        Note: At testing sessions we will have "DEBUG" Logging set as the level that is being outputted.
 *              However, at competition and when using "RELEASE" code we will be using "INFO" Logging. When
 *              a logging level is set, we only receive logging messages that are that level or higher
 *              priority.
 *
 *        Example: When INFO is set, we only receive: INFO, NOTICE, WARNING, ERROR, CRITICAL
 *                 When DEBUG is set, we only receive: DEBUG, INFO, NOTICE, WARNING, ERROR, CRITICAL
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-08-20
 ******************************************************************************/
namespace logging
{
    //////////////////////////////////////////
    // Declare namespace external variables and objects.
    /////////////////////////////////////////

    extern quill::Logger* g_qFileLogger;
    extern quill::Logger* g_qConsoleLogger;
    extern quill::Logger* g_qSharedLogger;

    extern quill::LogLevel g_eConsoleLogLevel;
    extern quill::LogLevel g_eFileLogLevel;

    extern std::string g_szProgramStartTimeString;

    // Logging constants.
    extern const std::string LOGGING_OUTPUT_PATH_ABSOLUTE;
    extern const quill::LogLevel CONSOLE_MIN_LEVEL;
    extern const quill::LogLevel FILE_MIN_LEVEL;
    extern const quill::LogLevel CONSOLE_DEFAULT_LEVEL;
    extern const quill::LogLevel FILE_DEFAULT_LEVEL;

    // Logging color constants.
    extern const std::string szTraceL3Color;
    extern const std::string szTraceL2Color;
    extern const std::string szTraceL1Color;
    extern const std::string szDebugColor;
    extern const std::string szInfoColor;
    extern const std::string szNoticeColor;
    extern const std::string szWarningColor;
    extern const std::string szErrorColor;
    extern const std::string szCriticalColor;
    extern const std::string szBacktraceColor;

    /////////////////////////////////////////
    // Declare namespace methods.
    /////////////////////////////////////////

    void InitializeLoggers(std::string szLoggingOutputPath);

    /////////////////////////////////////////
    // Define namespace file filters.
    /////////////////////////////////////////

    /******************************************************************************
     * @brief This class serves as a container class for handling log filtering of
     *    loggers. This must be used if you want each handler to have a different
     *    logging level since adding multiple handlers to the same logger will apply the
     *    loggers logging level to each handler.
     *
     *
     * @author clayjay3 (claytonraycowen@gmail.com)
     * @date 2024-03-16
     ******************************************************************************/
    class LoggingFilter : public quill::Filter
    {
        private:
            // Declare private member variables.
            quill::LogLevel m_eMinLogLevel;

        public:
            /******************************************************************************
             * @brief Construct a new Console Filter object.
             *
             * @param eMinLogLevel - The minimum acceptable log level for the console handler.
             *      All log levels above this will also be logged.
             *
             * @author clayjay3 (claytonraycowen@gmail.com)
             * @date 2024-03-16
             ******************************************************************************/
            LoggingFilter(const std::string szFilterBaseType, const quill::LogLevel eMinLogLevel) : quill::Filter(szFilterBaseType)
            {
                // Set member variables.
                m_eMinLogLevel = eMinLogLevel;
            };

            /******************************************************************************
             * @brief This method should never be called by this codebase, it is called internally
             *      by the quill library.
             *
             * @author clayjay3 (claytonraycowen@gmail.com)
             * @date 2024-03-16
             ******************************************************************************/
            QUILL_NODISCARD bool filter(quill::MacroMetadata const* log_metadata,
                                        uint64_t log_timestamp,
                                        std::string_view thread_id,
                                        std::string_view thread_name,
                                        std::string_view logger_name,
                                        quill::LogLevel log_level,
                                        std::string_view log_message,
                                        std::string_view log_statement) noexcept override
            {
                // Not using these.
                (void) log_metadata;
                (void) log_timestamp;
                (void) thread_id;
                (void) thread_name;
                (void) logger_name;
                (void) log_message;
                (void) log_statement;

                // Log only m_eMinLogLevel or higher to stdout.
                return log_level >= m_eMinLogLevel;
            }
    };

    /////////////////////////////////////////
    // Define namespace custom sinks
    /////////////////////////////////////////

    /******************************************************************************
     * @brief A custom console sink for logging messages with specific formatting
     *        and timestamping. This class extends `quill::ConsoleSink` and provides
     *        the capability to format log messages using a specified pattern and
     *        time format, allowing for customizable and colorized console outputs.
     *
     * This class is intended to be used in scenarios where real-time logging to the
     * console is required, such as during development, testing, and production. It
     * supports different formatting options, including log level descriptions,
     * thread information, timestamps, and log messages.
     *
     * ### Key Features:
     * - Customizable log message formats using a pattern.
     * - Colorized console output to highlight log levels.
     * - Supports different timestamp formats and timezones.
     * - Inherits from `quill::ConsoleSink` for seamless integration with Quill's
     *   logging framework.
     *
     * @note This class should be used for logging real-time information to the console.
     *       It is flexible enough to handle different environments, including testing
     *       and production, by adjusting the formatting and log levels.
     *
     * @see quill::ConsoleSink
     * @see quill::PatternFormatter
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-08-16
     ******************************************************************************/
    class MRDTConsoleSink : public quill::ConsoleSink
    {
        public:
            /******************************************************************************
             * @brief Constructs a new MRDTConsoleSink object with specified formatting and
             *        console colors. This constructor initializes the sink with a log
             *        message pattern, timestamp format, and optional timezone settings.
             *        The constructor also allows customization of the output stream.
             *
             * @param colours - The console colors configuration for highlighting log levels.
             * @param format_pattern - The pattern used to format the log message.
             * @param time_format - The format of the timestamp in the log message.
             * @param timestamp_timezone - The timezone used for the timestamp (default: LocalTime).
             * @param stream - The stream to output the logs to (default: "stdout").
             *
             * @note Ensure that the format pattern and time format are properly defined before
             *       using this constructor, as they directly affect the log output structure.
             *
             * @warning Incorrect configuration of format patterns or time formats may lead to
             *          malformed log outputs.
             *
             * @see quill::ConsoleSink
             * @see quill::PatternFormatter
             *
             * @author Eli Byrd (edbgkk@mst.edu)
             * @date 2024-08-16
             ******************************************************************************/
            MRDTConsoleSink(quill::ConsoleColours const& colours,                               // Custom Colors Import
                            std::string const& format_pattern,                                  // Custom Format Pattern
                            std::string const& time_format,                                     // Custom Time Format
                            quill::Timezone timestamp_timezone = quill::Timezone::LocalTime,    // Timezone
                            std::string const& stream          = "stdout"                       // Stream
                            ) :
                quill::ConsoleSink(colours, stream),                                            // Pass Parameters into quill::ConsoleSink
                _formatter(format_pattern, time_format, timestamp_timezone)                     // Pass Parameters into _formatter type
            {}

            void write_log(quill::MacroMetadata const* log_metadata,
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
                           std::string_view) override;

        private:
            quill::PatternFormatter _formatter;
    };

    /******************************************************************************
     * @brief A custom rotating file sink that formats and logs messages to a file
     *        with automatic rotation based on file size or time interval. This class
     *        extends `quill::RotatingFileSink` and provides the ability to format
     *        log messages using a pattern and time format, ensuring that logs are
     *        written to a rotating file system.
     *
     * This class is ideal for scenarios where log files need to be managed based on
     * size or time constraints, such as in long-running applications. It formats the
     * log messages before writing them to files, making it easy to store logs with
     * consistent formatting while preventing file bloat through rotation.
     *
     * ### Key Features:
     * - Customizable log message formats using a pattern.
     * - Automatic file rotation based on configurable size or time intervals.
     * - Supports different timestamp formats and timezones for log entries.
     * - Inherits from `quill::RotatingFileSink` for easy integration with Quill's
     *   logging framework.
     *
     * @note This class is designed to handle file-based logging with automatic rotation,
     *       making it suitable for use in both development and production environments.
     *
     * @see quill::RotatingFileSink
     * @see quill::PatternFormatter
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-08-16
     ******************************************************************************/
    class MRDTRotatingFileSink : public quill::RotatingFileSink
    {
        public:
            /******************************************************************************
             * @brief Constructs a new MRDTRotatingFileSink object with specified formatting,
             *        file rotation settings, and an optional file event notifier. This constructor
             *        initializes the sink with a log message pattern, timestamp format, and
             *        configuration for rotating the log file based on size or time interval.
             *
             * @param filename - The path to the log file.
             * @param config - The configuration for rotating the log file (e.g., based on size or time).
             * @param format_pattern - The pattern used to format the log message.
             * @param time_format - The format of the timestamp in the log message.
             * @param timestamp_timezone - The timezone used for the timestamp (default: LocalTime).
             * @param file_event_notifier - Optional event notifier for file-related events (default: none).
             *
             * @note Ensure that the file rotation configuration (`config`) is correctly set up to avoid
             *       unexpected log file behavior. The format pattern and time format should also be defined
             *       correctly to ensure logs are written with the intended structure.
             *
             * @warning Misconfiguration of file rotation settings or format patterns may result in loss of log data or malformed log outputs.
             *
             * @see quill::RotatingFileSink
             * @see quill::PatternFormatter
             *
             * @author Eli Byrd (edbgkk@mst.edu)
             * @date 2024-08-16
             ******************************************************************************/
            MRDTRotatingFileSink(quill::fs::path const& filename,                                              // File Path
                                 quill::RotatingFileSinkConfig const& config,                                  // Rotating File Sink Config
                                 std::string const& format_pattern,                                            // Custom Format Pattern
                                 std::string const& time_format,                                               // Custom Time Format
                                 quill::Timezone timestamp_timezone           = quill::Timezone::LocalTime,    // Timezone
                                 quill::FileEventNotifier file_event_notifier = quill::FileEventNotifier{}     //
                                 ) :
                quill::RotatingFileSink(filename, config, file_event_notifier),                                // Pass Parameters into quill::RotatingFileSink
                _formatter(format_pattern, time_format, timestamp_timezone)                                    // Pass Parameters into _formatter type
            {}

            void write_log(quill::MacroMetadata const* log_metadata,
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
                           std::string_view) override;

        private:
            quill::PatternFormatter _formatter;
    };

}    // namespace logging

#endif    // ROVECOMM_LOGGER_H
#endif    // __ROVECOMM_LIBRARY_MODE__
