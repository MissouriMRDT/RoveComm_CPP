/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2026-06-18
 *
 * @copyright Copyright Mars Rover Design Team 2026 - All Rights Reserved
 ******************************************************************************/

#ifndef MANIFEST_H
#define MANIFEST_H

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

namespace manifest
{
    /******************************************************************************
     * @brief Enumeration of Data Types to be used in RoveComm
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    enum DataTypes
    {
        INT8_T,
        UINT8_T,
        INT16_T,
        UINT16_T,
        INT32_T,
        UINT32_T,
        FLOAT_T,
        DOUBLE_T,
        CHAR
    };

    /******************************************************************************
     * @brief Allows constraining templates to only RoveComm supported types
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    template<typename T>
    concept RoveCommType = std::same_as<T, int8_t> || std::same_as<T, uint8_t> || std::same_as<T, int16_t> || std::same_as<T, uint16_t> || std::same_as<T, int32_t> ||
                           std::same_as<T, uint32_t> || std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, char>;

    /******************************************************************************
     * @brief IP Address Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    struct AddressEntry
    {
        public:
            int FIRST_OCTET;
            int SECOND_OCTET;
            int THIRD_OCTET;
            int FOURTH_OCTET;

            constexpr AddressEntry(int first, int second, int third, int fourth) noexcept :
                FIRST_OCTET(first), SECOND_OCTET(second), THIRD_OCTET(third), FOURTH_OCTET(fourth)
            {}

            AddressEntry(std::string_view ip)
            {
                size_t pos1  = ip.find('.');
                size_t pos2  = ip.find('.', pos1 + 1);
                size_t pos3  = ip.find('.', pos2 + 1);
                FIRST_OCTET  = std::stoi(std::string(ip.substr(0, pos1)));
                SECOND_OCTET = std::stoi(std::string(ip.substr(pos1 + 1, pos2 - pos1 - 1)));
                THIRD_OCTET  = std::stoi(std::string(ip.substr(pos2 + 1, pos3 - pos2 - 1)));
                FOURTH_OCTET = std::stoi(std::string(ip.substr(pos3 + 1)));
            }

            std::string IP_STR() const
            {
                return std::to_string(FIRST_OCTET) + "." + std::to_string(SECOND_OCTET) + "." + std::to_string(THIRD_OCTET) + "." + std::to_string(FOURTH_OCTET);
            }
    };

    /******************************************************************************
     * @brief Manifest Entry Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    struct ManifestEntry
    {
        public:
            int DATA_ID;
            int DATA_COUNT;
            DataTypes DATA_TYPE;
    };

    /******************************************************************************
     * @brief Board Entry Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    struct BoardEntry
    {
        public:
            AddressEntry ADDRESS;
            std::vector<ManifestEntry> COMMANDS, TELEMETRY, ERRORS;
    };

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Core
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 2, 110};

        // Commands
        namespace Commands
        {
            // [LeftSpeed, RightSpeed] (-1 - 1) -> (-100% - 100%)
            constexpr ManifestEntry DRIVELEFTRIGHT{3000, 2, DataTypes::FLOAT_T};
            // [LF, LM, LR, RF, RM, RR] (-1 - 1) -> (-100% - 100%)
            constexpr ManifestEntry DRIVEINDIVIDUAL{3001, 6, DataTypes::FLOAT_T};
            // [Enabled]
            constexpr ManifestEntry WATCHDOGOVERRIDE{3002, 1, DataTypes::UINT8_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry LEFTGIMBAL{3003, 2, DataTypes::INT16_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry RIGHTGIMBAL{3004, 2, DataTypes::INT16_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry BACKGIMBAL{3005, 2, DataTypes::INT16_T};
            // [R, G, B] (brightness 0 - 255)
            constexpr ManifestEntry LEDRGB{3006, 3, DataTypes::UINT8_T};
            // [Color] (RGBA)
            constexpr ManifestEntry BACKIMAGE{3007, 256, DataTypes::UINT32_T};
            // [R, G, B] (brightness 0 - 255)
            constexpr ManifestEntry INTERNALRGB{3008, 3, DataTypes::UINT8_T};
            // [Color] (RGBA)
            constexpr ManifestEntry INTERNALIMAGE{3009, 256, DataTypes::UINT32_T};
            // [State] (DisplayState)
            constexpr ManifestEntry STATEDISPLAY{3010, 1, DataTypes::UINT8_T};
            // [Brightness] (0 - 255)
            constexpr ManifestEntry BRIGHTNESS{3011, 1, DataTypes::UINT8_T};
            // [Mode] (0: Teleop 1: Autonomy)
            constexpr ManifestEntry SETWATCHDOGMODE{3012, 1, DataTypes::UINT8_T};
            // [Message] (null terminated string)
            constexpr ManifestEntry LEDTEXT{3013, 256, DataTypes::CHAR};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [FL, ML, BL, FR, MR, BR] (-1 - 1) -> (-100% - 100%)
            constexpr ManifestEntry MOTORSPEEDS{3100, 6, DataTypes::FLOAT_T};
            // [FL, ML, BL, FR, MR, BR] (A)
            constexpr ManifestEntry MOTORCURRENTS{3101, 6, DataTypes::FLOAT_T};
            // [FL, ML, BL, FR, MR, BR] (A battery side)
            constexpr ManifestEntry VESCCURRENTS{3102, 6, DataTypes::FLOAT_T};
            // [Roll, Pitch] (deg)
            constexpr ManifestEntry IMUDATA{3103, 2, DataTypes::FLOAT_T};
            // [X, Y, Z] (m/s2)
            constexpr ManifestEntry ACCELEROMETERDATA{3104, 3, DataTypes::FLOAT_T};
            // [FanSpeed, BoardTemperature, OtherTemperature] (rpm, C, C)
            constexpr ManifestEntry THERMAL{3105, 3, DataTypes::FLOAT_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {
            // [MotorID, FaultCode]
            constexpr ManifestEntry VESCFAULT{3200, 2, DataTypes::UINT8_T};
        }    // namespace Errors

        // Enums
        enum class MOTORS
        {
            FRONT_LEFT   = 0,
            MIDDLE_LEFT  = 1,
            BACK_LEFT    = 2,
            FRONT_RIGHT  = 3,
            MIDDLE_RIGHT = 4,
            BACK_RIGHT   = 5
        };

        enum class DISPLAYSTATE
        {
            TELEOP       = 0,
            AUTONOMY     = 1,
            REACHED_GOAL = 2
        };

        enum class VESCFAULTCODE
        {
            NONE                               = 0,
            OVER_VOLTAGE                       = 1,
            UNDER_VOLTAGE                      = 2,
            DRV                                = 3,
            ABS_OVER_CURRENT                   = 4,
            OVER_TEMP_FET                      = 5,
            OVER_TEMP_MOTOR                    = 6,
            GATE_DRIVER_OVER_VOLTAGE           = 7,
            GATE_DRIVER_UNDER_VOLTAGE          = 8,
            MCU_UNDER_VOLTAGE                  = 9,
            BOOTING_FROM_WATCHDOG_RESET        = 10,
            ENCODER_SPI                        = 11,
            ENCODER_SINCOS_BELOW_MIN_AMPLITUDE = 12,
            ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE = 13,
            FLASH_CORRUPTION                   = 14,
            HIGH_OFFSET_CURRENT_SENSOR_1       = 15,
            HIGH_OFFSET_CURRENT_SENSOR_2       = 16,
            HIGH_OFFSET_CURRENT_SENSOR_3       = 17,
            UNBALANCED_CURRENTS                = 18,
            BRK                                = 19,
            RESOLVER_LOT                       = 20,
            RESOLVER_DOS                       = 21,
            RESOLVER_LOS                       = 22,
            FLASH_CORRUPTION_APP_CFG           = 23,
            FLASH_CORRUPTION_MC_CFG            = 24,
            ENCODER_NO_MAGNET                  = 25,
            ENCODER_MAGNET_TOO_STRONG          = 26,
            PHASE_FILTER                       = 27
        };

    }    // namespace Core

    /******************************************************************************
     * @brief PMS Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace PMS
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 2, 102};

        // Commands
        namespace Commands
        {
            // Power off all systems except network (PMS will stay on)
            constexpr ManifestEntry ESTOP{4000, 0, DataTypes::UINT8_T};
            // Power off all systems including network, cannot recover without physical reboot (PMS will stay on)
            constexpr ManifestEntry SUICIDE{4001, 0, DataTypes::UINT8_T};
            // Cycle all systems including network off and back on (PMS will stay on)
            constexpr ManifestEntry REBOOT{4002, 0, DataTypes::UINT8_T};
            // [Motor, Core, Aux] (bitmask enable)
            constexpr ManifestEntry ENABLEBUS{4003, 1, DataTypes::UINT8_T};
            // [Motor, Core, Aux] (bitmask disable)
            constexpr ManifestEntry DISABLEBUS{4004, 1, DataTypes::UINT8_T};
            // [Motor, Core, Aux] (bitmask enabled)
            constexpr ManifestEntry SETBUS{4005, 1, DataTypes::UINT8_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [PackCurrent, AuxCurrent, LowCurrent, NetworkCurrent, RadioM2Current, RadioM9Current, Cell1Voltage, Cell2Voltage, Cell3Voltage, Cell4Voltage, Cell5Voltage,
            // Cell6Voltage] (A, A, A, A, A, A, V, V, V, V, V, V)
            constexpr ManifestEntry CURRENTANDVOLTAGE{4100, 12, DataTypes::FLOAT_T};
            // [Motor, Core, Aux, RadioM2, RadioM9, Network] (bitmask enabled)
            constexpr ManifestEntry BUSSTATUS{4101, 1, DataTypes::UINT8_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {
            // Higher current draw than the battery can support. Rover will Reboot automatically
            constexpr ManifestEntry PACKOVERCURRENT{4200, 0, DataTypes::UINT8_T};
            // [C1, C2, C3, C4, C5, C6] (bitmask undervolt) Rover will EStop automatically
            constexpr ManifestEntry CELLUNDERVOLTAGE{4201, 1, DataTypes::UINT8_T};
            // [C1, C2, C3, C4, C5, C6] (bitmask critical) Rover will Suicide automatically
            constexpr ManifestEntry CELLCRITICAL{4202, 1, DataTypes::UINT8_T};
            // Aux system current draw too high. Rover will disable Aux bus automatically
            constexpr ManifestEntry AUXOVERCURRENT{4203, 0, DataTypes::UINT8_T};
        }    // namespace Errors
    }    // namespace PMS

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 2, 104};

        // Commands
        namespace Commands
        {}

        // Telemetry
        namespace Telemetry
        {
            // [Lat, Lon, Alt, HorizontalAccuracy, VerticalAccuracy, HeadingAccuracy, FixType, IsDifferential] (deg, deg, m, m, m, deg, ublox_navpvt fix type
            // http://docs.ros.org/en/noetic/api/ublox_msgs/html/msg/NavPVT.html, bool)
            constexpr ManifestEntry GPSLATLONALT{6100, 8, DataTypes::DOUBLE_T};
            // [Heading] (0 - 360)
            constexpr ManifestEntry COMPASSDATA{6102, 1, DataTypes::FLOAT_T};
            // [Satellites]
            constexpr ManifestEntry SATELLITECOUNTDATA{6103, 1, DataTypes::UINT8_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {
            //
            constexpr ManifestEntry GPSLOCKERROR{6200, 0, DataTypes::UINT8_T};
        }    // namespace Errors
    }    // namespace Nav

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace SignalStack
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 100, 101};

        // Commands
        namespace Commands
        {
            // [Speed] (-1000 - 1000) -> (-100% - 100%)
            constexpr ManifestEntry OPENLOOP{7000, 1, DataTypes::INT16_T};
            // [Heading] (0 - 360)
            constexpr ManifestEntry SETANGLETARGET{7001, 1, DataTypes::FLOAT_T};
            // [Rover Lat, Rover Lon, Basestation Lat, Basestation Lon] (-90 - 90, -180 - 180, -90 - 90, -180 - 180)
            constexpr ManifestEntry SETGPSTARGET{7002, 4, DataTypes::DOUBLE_T};
            // [Enabled]
            constexpr ManifestEntry WATCHDOGOVERRIDE{7003, 1, DataTypes::UINT8_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [Heading] (0 - 360)
            constexpr ManifestEntry COMPASSANGLE{7100, 1, DataTypes::FLOAT_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace SignalStack

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 2, 107};

        // Commands
        namespace Commands
        {
            // [X, J2, J3, J4, J5, J6] (-32768 - 32767) -> (-100% - 100%)
            constexpr ManifestEntry OPENLOOP{8000, 6, DataTypes::INT16_T};
            // [X, J2, J3, J4, J5, J6] (in, deg, deg, deg, deg, deg)
            constexpr ManifestEntry TARGETANGLE{8001, 6, DataTypes::FLOAT_T};
            // [X, J2, J3, J4, J5, J6] (in, deg, deg, deg, deg, deg)
            constexpr ManifestEntry TARGETANGLEINCREMENT{8002, 6, DataTypes::FLOAT_T};
            // [Gripper] (-32768 - 32767) -> (-100% - 100%)
            constexpr ManifestEntry GRIPPEROPENLOOP{8003, 1, DataTypes::INT16_T};
            // [X, Y, Z, J4, J5, J6] (in, in, in, deg, deg, deg)
            constexpr ManifestEntry IKPOSITION{8004, 6, DataTypes::FLOAT_T};
            // [X, Y, Z, J4, J5, J6] (in, in, in, deg, deg, deg)
            constexpr ManifestEntry IKPOSITIONINCREMENT{8005, 6, DataTypes::FLOAT_T};
            // [TX, TY, TZ, RX, RY, RZ] (in, in, in, deg, deg, deg)
            constexpr ManifestEntry IKPOSEINCREMENT{8006, 6, DataTypes::FLOAT_T};
            // [Enabled]
            constexpr ManifestEntry LASER{8007, 1, DataTypes::UINT8_T};
            // [Position] (0 - 180)
            constexpr ManifestEntry LINEARSERVO{8008, 1, DataTypes::UINT8_T};
            // [Position] (0 - 180)
            constexpr ManifestEntry CACHE{8009, 1, DataTypes::UINT8_T};
            // [Enabled]
            constexpr ManifestEntry WATCHDOGOVERRIDE{8010, 1, DataTypes::UINT8_T};
            // [X+, X-, J2+, J2-, J3+, J3-, J4+, J4-, J5+, J5-] (bitmask override enabled)
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{8011, 1, DataTypes::UINT16_T};
            // [X, J2, J3, J4, J5, J6] (bitmask override enabled)
            constexpr ManifestEntry CLOSEDLOOPOVERRIDE{8012, 1, DataTypes::UINT8_T};
            // [X, Roll] (bitmask start calibration)
            constexpr ManifestEntry CALIBRATEENCODER{8013, 1, DataTypes::UINT8_T};
            // [X+, X-, J2+, J2-, J3+, J3-, J4+, J4-, J5+, J5-] (bitmask override enabled)
            constexpr ManifestEntry SOFTLIMITOVERRIDE{8014, 1, DataTypes::UINT16_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry ARMGIMBAL1{8015, 2, DataTypes::INT16_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry ARMGIMBAL2{8016, 2, DataTypes::INT16_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [X, J2, J3, J4, J5, J6, GX, GY, GZ] (in, deg, deg, deg, deg, deg, in, in, in)
            constexpr ManifestEntry POSITION{8100, 9, DataTypes::FLOAT_T};
            // [X+, X-, J2+, J2-, J3+, J3-, J4+, J4-, J5+, J5-] (bitmask depressed)
            constexpr ManifestEntry LIMITSWITCH{8101, 1, DataTypes::UINT16_T};
            // [X+, X-, J2+, J2-, J3+, J3-, J4+, J4-, J5+, J5-] (bitmask triggered)
            constexpr ManifestEntry SOFTLIMIT{8102, 1, DataTypes::UINT16_T};
            // [X, J2, J3, J4, J5, J6, G] (ping time ms)
            constexpr ManifestEntry SMOCOPING{8103, 7, DataTypes::UINT16_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace Arm

    /******************************************************************************
     * @brief Auger Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Auger
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 2, 108};

        // Commands
        namespace Commands
        {
            // [Speed] (-32768 - 32767) -> (-100% - 100%)
            constexpr ManifestEntry AUGERAXIS{9000, 1, DataTypes::INT16_T};
            // [AugerAxis+, AugerAxis-] (bitmask override enabled)
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{9001, 1, DataTypes::UINT8_T};
            // Request calibration of the AugerAxis encoder
            constexpr ManifestEntry CALIBRATEENCODER{9002, 0, DataTypes::UINT8_T};
            // [Speed] (-1000 - 1000) -> (-100% - 100%)
            constexpr ManifestEntry AUGER{9003, 1, DataTypes::INT16_T};
            // [Enabled]
            constexpr ManifestEntry WATCHDOGOVERRIDE{9004, 1, DataTypes::UINT8_T};
            // [White, 365, 405, 500] (brightness 0 - 255)
            constexpr ManifestEntry LED{9005, 4, DataTypes::UINT8_T};
            // [AFFilters, SoilTrapdoor] (0 - 180)
            constexpr ManifestEntry AUGERSERVO{9006, 2, DataTypes::INT16_T};
            // [Pan, Tilt] (0 - 180)
            constexpr ManifestEntry AUGERGIMBAL{9007, 2, DataTypes::INT16_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [AugerAxis] (in)
            constexpr ManifestEntry POSITION{9100, 1, DataTypes::FLOAT_T};
            // [AugerSpeed] (rpm)
            constexpr ManifestEntry AUGERSPEED{9101, 1, DataTypes::FLOAT_T};
            // [AugerAxis+, AugerAxis-] (bitmask depressed)
            constexpr ManifestEntry LIMITSWITCH{9102, 1, DataTypes::UINT8_T};
            // [Temperature, Humidity] (C, relative %)
            constexpr ManifestEntry ENVIRONMENTAL{9103, 2, DataTypes::FLOAT_T};
            // [AugerCurrent] (A)
            constexpr ManifestEntry AUGERCURRENT{9104, 1, DataTypes::FLOAT_T};
            // [AugerAxis] (ping time ms)
            constexpr ManifestEntry SMOCOPING{9105, 1, DataTypes::UINT16_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace Auger

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 3, 100};

        // Commands
        namespace Commands
        {
            // Start Autonomy_Software
            constexpr ManifestEntry STARTAUTONOMY{11000, 1, DataTypes::UINT8_T};
            // Return Autonomy_Software to Idle state
            constexpr ManifestEntry DISABLEAUTONOMY{11001, 1, DataTypes::UINT8_T};
            // [Lat, Lon, AUTONOMYWAYPOINTTYPES]
            constexpr ManifestEntry ADDPOSITIONLEG{11002, 3, DataTypes::DOUBLE_T};
            // [Lat, Lon, AUTONOMYWAYPOINTTYPES, MarkerRadius (meters)]
            constexpr ManifestEntry ADDMARKERLEG{11003, 4, DataTypes::DOUBLE_T};
            // [Lat, Lon, AUTONOMYWAYPOINTTYPES, ObjectRadius (meters)]
            constexpr ManifestEntry ADDOBJECTLEG{11004, 4, DataTypes::DOUBLE_T};
            // Clear queued positions, markers, and objects waypoints.
            constexpr ManifestEntry CLEARWAYPOINTS{11005, 1, DataTypes::UINT8_T};
            // A multiplier from 0.0 to 1.0 that will scale the max power effort of Autonomy.
            constexpr ManifestEntry SETMAXSPEED{11006, 1, DataTypes::FLOAT_T};
            // A multiplier from 0.0 to 1.0 that will filter points from the traversability map. Higher values will result in more conservative pathing.
            constexpr ManifestEntry SETMINTRAVSCORE{11007, 1, DataTypes::FLOAT_T};
            // A multiplier from 0.0 to 1.0 that will bias the pathing algorithm towards shorter paths (lower values) or safer paths (higher values).
            constexpr ManifestEntry SETBETABIAS{11008, 1, DataTypes::FLOAT_T};
            // [Enum (AUTONOMYLOG), Enum (AUTONOMYLOG), Enum (AUTONOMYLOG)] {Console, File, RoveComm}
            constexpr ManifestEntry SETLOGGINGLEVELS{11009, 3, DataTypes::UINT8_T};
            // [Lat, Lon, ObstacleRadius (meters)]
            constexpr ManifestEntry ADDOBSTACLE{11010, 3, DataTypes::DOUBLE_T};
            // Clear queued permanent obstacles.
            constexpr ManifestEntry CLEAROBSTACLES{11011, 1, DataTypes::UINT8_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [State] (AUTONOMYSTATE)
            constexpr ManifestEntry CURRENTSTATE{11100, 1, DataTypes::UINT8_T};
            // [State] (0: Teleop 1: Autonomy 2: Reached Goal)
            constexpr ManifestEntry STATEDISPLAY{11101, 1, DataTypes::UINT8_T};
            // [Thread, FPS] (AUTONOMYTHREADS, fps)
            constexpr ManifestEntry THREADFPS{11103, 2, DataTypes::UINT32_T};
            // [Lat, Lon, Lat, Lon, ...] (deg, deg, deg, deg, ...)
            constexpr ManifestEntry PATHWAYPOINTS{11104, 1000, DataTypes::DOUBLE_T};
            // [EstimatedTimeToGoal] (s)
            constexpr ManifestEntry TIMEREMAINING{11105, 1, DataTypes::DOUBLE_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}

        // Enums
        enum class AUTONOMYSTATE
        {
            IDLE              = 0,
            NAVIGATING        = 1,
            SEARCHPATTERN     = 2,
            APPROACHINGMARKER = 3,
            APPROACHINGOBJECT = 4,
            VERIFYINGGPS      = 5,
            VERIFYINGMARKER   = 6,
            VERIFYINGOBJECT   = 7,
            REVERSING         = 8,
            STUCK             = 9
        };

        enum class AUTONOMYLOG
        {
            TRACEL3  = 0,
            TRACEL2  = 1,
            TRACEL1  = 2,
            DEBUG    = 3,
            INFO     = 4,
            NOTICE   = 5,
            WARNING  = 6,
            ERROR    = 7,
            CRITICAL = 8
        };

        enum class AUTONOMYTHREADS
        {
            NOTSET         = 0,
            MAINPROCESS    = 1,
            MAINCAM        = 2,
            REARCAM        = 3,
            TAGDETECTOR    = 4,
            OBJECTDETECTOR = 5,
            STATEMACHINE   = 6,
            ROVECOMMUDP    = 7,
            ROVECOMMTCP    = 8
        };

        enum class AUTONOMYWAYPOINTTYPES
        {
            CONTINUOUSNAVIGATE = -99,
            ROCKPICK           = -4,
            WATERBOTTLE        = -3,
            MALLET             = -2,
            ANY                = -1,
            TAG0               = 0,
            TAG1               = 1,
            TAG2               = 2,
            TAG3               = 3
        };

    }    // namespace Autonomy

    /******************************************************************************
     * @brief Camera1 Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 4, 100};

        // Commands
        namespace Commands
        {
            // [Camera, Restart]
            constexpr ManifestEntry TAKEPICTURE{12000, 2, DataTypes::UINT8_T};
            // [Camera, Restart]
            constexpr ManifestEntry TOGGLESTREAM{12001, 2, DataTypes::UINT8_T};
            // [Arguments] (0x1f delimited, 0x04 terminated list with maximum length of 16384 characters for RPi-Camera/config.toml/ffmpeg_arguments. Accepts the
            // following substitutions: $index: camera index, $input: input device file, $ip: output ip, $port: output port, $brightness, $contrast)
            constexpr ManifestEntry SETFFMPEGARGUMENTS{12002, 16384, DataTypes::CHAR};
            // [Arguments] (0x1f delimited, 0x04 terminated list with maximum length of 16384 characters for RPi-Camera/config.toml/picture_arguments. Accepts the
            // following substitutions: $index: camera index, $input: input device file, $output: output file without extension, $brightness, $contrast)
            constexpr ManifestEntry SETPICTUREARGUMENTS{12003, 16384, DataTypes::CHAR};
            // [Camera0, Camera1, Camera2, Camera3] (-1.0 - 1.0)
            constexpr ManifestEntry SETBRIGHTNESS{12004, 4, DataTypes::FLOAT_T};
            // [Camera0, Camera1, Camera2, Camera3] (-1.0 - 2.0)
            constexpr ManifestEntry SETCONTRAST{12005, 4, DataTypes::FLOAT_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [AvailableCameras]
            constexpr ManifestEntry AVAILABLECAMERAS{12100, 1, DataTypes::UINT8_T};
            // [StreamingCameras]
            constexpr ManifestEntry STREAMINGCAMERAS{12101, 1, DataTypes::UINT8_T};
            // Picture has been taken.
            constexpr ManifestEntry PICTURETAKEN{12102, 0, DataTypes::UINT8_T};
            // [cpu0, cpu1, cpu2, cpu3, mem, storage] (% usage)
            constexpr ManifestEntry UTILIZATION{12103, 6, DataTypes::UINT8_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Camera2
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 4, 101};

        // Commands
        namespace Commands
        {
            // [Camera, Restart]
            constexpr ManifestEntry TAKEPICTURE{13000, 2, DataTypes::UINT8_T};
            // [Camera, Restart]
            constexpr ManifestEntry TOGGLESTREAM{13001, 2, DataTypes::UINT8_T};
            // [Arguments] (0x1f delimited, 0x04 terminated list with maximum length of 16384 characters for RPi-Camera/config.toml/ffmpeg_arguments. Accepts the
            // following substitutions: $index: camera index, $input: input device file, $ip: output ip, $port: output port, $brightness, $contrast)
            constexpr ManifestEntry SETFFMPEGARGUMENTS{13002, 16384, DataTypes::CHAR};
            // [Arguments] (0x1f delimited, 0x04 terminated list with maximum length of 16384 characters for RPi-Camera/config.toml/picture_arguments. Accepts the
            // following substitutions: $index: camera index, $input: input device file, $output: output file without extension, $brightness, $contrast)
            constexpr ManifestEntry SETPICTUREARGUMENTS{13003, 16384, DataTypes::CHAR};
            // [Camera0, Camera1, Camera2, Camera3] (-1.0 - 1.0)
            constexpr ManifestEntry SETBRIGHTNESS{13004, 4, DataTypes::FLOAT_T};
            // [Camera0, Camera1, Camera2, Camera3] (-1.0 - 2.0)
            constexpr ManifestEntry SETCONTRAST{13005, 4, DataTypes::FLOAT_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [AvailableCameras]
            constexpr ManifestEntry AVAILABLECAMERAS{13100, 1, DataTypes::UINT8_T};
            // [StreamingCameras]
            constexpr ManifestEntry STREAMINGCAMERAS{13101, 1, DataTypes::UINT8_T};
            // Picture has been taken.
            constexpr ManifestEntry PICTURETAKEN{13102, 0, DataTypes::UINT8_T};
            // [cpu0, cpu1, cpu2, cpu3, mem, storage] (% usage)
            constexpr ManifestEntry UTILIZATION{13103, 6, DataTypes::UINT8_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace Camera2

    /******************************************************************************
     * @brief CameraServer Board IP Address, Commands, Telemetry, and Error
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace CameraServer
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 4, 102};

        // Commands
        namespace Commands
        {
            // [Camera]
            constexpr ManifestEntry TAKEPHOTO{14000, 1, DataTypes::UINT8_T};
            // [Camera, Action] (id, 0: Shutdown 1: Startup 2: Restart)
            constexpr ManifestEntry TOGGLESTREAM{14001, 2, DataTypes::UINT8_T};
            // [Camera, Brightness] (id, 0 - 255)
            constexpr ManifestEntry ADJUSTBRIGHTNESS{14002, 2, DataTypes::UINT8_T};
            // [Camera, Contrast] (id, 0 - 255)
            constexpr ManifestEntry ADJUSTCONTRAST{14003, 2, DataTypes::UINT8_T};
            // [Camera, Saturation] (id, 0 - 255)
            constexpr ManifestEntry ADJUSTSATURATION{14004, 2, DataTypes::UINT8_T};
            // [Camera, Hue] (id, 0 - 255)
            constexpr ManifestEntry ADJUSTHUE{14005, 2, DataTypes::UINT8_T};
            // [Camera, Temperature]
            constexpr ManifestEntry SETWHITEBALANCE{14008, 2, DataTypes::UINT8_T};
            // [Camera, BacklightContrast]
            constexpr ManifestEntry ADJUSTBACKLIGHTCONTRAST{14009, 2, DataTypes::UINT8_T};
            // [Camera, Exposure]
            constexpr ManifestEntry SETEXPOSURE{14010, 2, DataTypes::INT32_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [Camera0, Camera1, Camera2, Camera3, Camera4, Camera5, Camera6, Camera7] (bitmask able to stream)
            constexpr ManifestEntry AVAILABLECAMERAS{14100, 1, DataTypes::UINT8_T};
            // [Port0, Port1, Port2, Port3] (currently streaming on each port)
            constexpr ManifestEntry STREAMINGCAMERAS{14101, 4, DataTypes::UINT8_T};
            // Picture has been taken
            constexpr ManifestEntry PICTURETAKEN{14102, 0, DataTypes::UINT8_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {
            // [Camera] (id) Camera has errored and stopped streaming
            constexpr ManifestEntry CAMERAUNAVAILABLE{14200, 1, DataTypes::UINT8_T};
        }    // namespace Errors
    }    // namespace CameraServer

    /******************************************************************************
     * @brief Raman Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Raman
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{192, 168, 3, 105};

        // Commands
        namespace Commands
        {
            // [Speed] (-32768 - 32767) -> (-100% - 100%)
            constexpr ManifestEntry INSTRUMENTSAXIS{16000, 1, DataTypes::INT16_T};
            // [InstrumentsAxis+, InstrumentsAxis-] (bitmask override enabled)
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{16001, 1, DataTypes::UINT8_T};
            // Request calibration of the InstrumentsAxis encoder
            constexpr ManifestEntry CALIBRATEENCODER{16002, 0, DataTypes::UINT8_T};
            // [Enabled]
            constexpr ManifestEntry WATCHDOGOVERRIDE{16003, 1, DataTypes::UINT8_T};
            // [Enabled]
            constexpr ManifestEntry LASER{16004, 1, DataTypes::UINT8_T};
            // [Integration Time] (ms)
            constexpr ManifestEntry REQUESTRAMANREADING{16005, 1, DataTypes::UINT32_T};
        }    // namespace Commands

        // Telemetry
        namespace Telemetry
        {
            // [InstrumentsAxis, TOF] (mm, mm)
            constexpr ManifestEntry POSITION{16100, 2, DataTypes::FLOAT_T};
            // [InstrumentsAxis+, InstrumentsAxis-] (bitmask depressed)
            constexpr ManifestEntry LIMITSWITCH{16101, 1, DataTypes::UINT8_T};
            // Raman CCD elements 1-512
            constexpr ManifestEntry RAMANREADING_PART1{16102, 512, DataTypes::UINT16_T};
            // Raman CCD elements 513-1024
            constexpr ManifestEntry RAMANREADING_PART2{16103, 512, DataTypes::UINT16_T};
            // Raman CCD elements 1025-1536
            constexpr ManifestEntry RAMANREADING_PART3{16104, 512, DataTypes::UINT16_T};
            // Raman CCD elements 1537-2048
            constexpr ManifestEntry RAMANREADING_PART4{16105, 512, DataTypes::UINT16_T};
            // [InstrumentsAxis] (ping time ms)
            constexpr ManifestEntry SMOCOPING{16106, 1, DataTypes::UINT16_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace Raman

    /******************************************************************************
     * @brief RoveSoSimulator Board IP Address, Commands, Telemetry, and Error
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace RoveSoSimulator
    {
        // IP Address
        constexpr AddressEntry IP_ADDRESS{127, 0, 0, 1};

        // Commands
        namespace Commands
        {}

        // Telemetry
        namespace Telemetry
        {
            // [Accel X, Accel Y, Accel Z, Gyro X, Gyro Y, Gyro Z, Quat X, Quat Y, Quat Z, Quat W]
            constexpr ManifestEntry IMU{99100, 10, DataTypes::DOUBLE_T};
        }    // namespace Telemetry

        // Error
        namespace Errors
        {}
    }    // namespace RoveSoSimulator

    /******************************************************************************
     * @brief RoveComm General Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace General
    {
        const int UPDATE_RATE            = 100;
        const int ETHERNET_UDP_PORT      = 11000;
        const int ETHERNET_TCP_PORT      = 12000;
        const int SUBNET_MAC_FIRST_BYTE  = 222;
        const int SUBNET_MAC_SECOND_BYTE = 173;
    }    // namespace General

    /******************************************************************************
     * @brief RoveComm System Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace System
    {
        const int PING_DATA_ID            = 1;
        const int PING_REPLY_DATA_ID      = 2;
        const int SUBSCRIBE_DATA_ID       = 3;
        const int UNSUBSCRIBE_DATA_ID     = 4;
        const int INVALID_VERSION_DATA_ID = 5;
        const int NO_DATA_DATA_ID         = 6;
    }    // namespace System

    /******************************************************************************
     * @brief BoardEntry Map for Runtime Lookups
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    const std::map<std::string, BoardEntry> BOARDS{
        {
            "Core",
            {
                manifest::Core::IP_ADDRESS,
                {
                    manifest::Core::Commands::DRIVELEFTRIGHT,
                    manifest::Core::Commands::DRIVEINDIVIDUAL,
                    manifest::Core::Commands::WATCHDOGOVERRIDE,
                    manifest::Core::Commands::LEFTGIMBAL,
                    manifest::Core::Commands::RIGHTGIMBAL,
                    manifest::Core::Commands::BACKGIMBAL,
                    manifest::Core::Commands::LEDRGB,
                    manifest::Core::Commands::BACKIMAGE,
                    manifest::Core::Commands::INTERNALRGB,
                    manifest::Core::Commands::INTERNALIMAGE,
                    manifest::Core::Commands::STATEDISPLAY,
                    manifest::Core::Commands::BRIGHTNESS,
                    manifest::Core::Commands::SETWATCHDOGMODE,
                    manifest::Core::Commands::LEDTEXT,
                },
                {
                    manifest::Core::Telemetry::MOTORSPEEDS,
                    manifest::Core::Telemetry::MOTORCURRENTS,
                    manifest::Core::Telemetry::VESCCURRENTS,
                    manifest::Core::Telemetry::IMUDATA,
                    manifest::Core::Telemetry::ACCELEROMETERDATA,
                    manifest::Core::Telemetry::THERMAL,
                },
                {
                    manifest::Core::Errors::VESCFAULT,
                },
            },
        },
        {
            "PMS",
            {
                manifest::PMS::IP_ADDRESS,
                {
                    manifest::PMS::Commands::ESTOP,
                    manifest::PMS::Commands::SUICIDE,
                    manifest::PMS::Commands::REBOOT,
                    manifest::PMS::Commands::ENABLEBUS,
                    manifest::PMS::Commands::DISABLEBUS,
                    manifest::PMS::Commands::SETBUS,
                },
                {
                    manifest::PMS::Telemetry::CURRENTANDVOLTAGE,
                    manifest::PMS::Telemetry::BUSSTATUS,
                },
                {
                    manifest::PMS::Errors::PACKOVERCURRENT,
                    manifest::PMS::Errors::CELLUNDERVOLTAGE,
                    manifest::PMS::Errors::CELLCRITICAL,
                    manifest::PMS::Errors::AUXOVERCURRENT,
                },
            },
        },
        {
            "Nav",
            {
                manifest::Nav::IP_ADDRESS,
                {},
                {
                    manifest::Nav::Telemetry::GPSLATLONALT,
                    manifest::Nav::Telemetry::COMPASSDATA,
                    manifest::Nav::Telemetry::SATELLITECOUNTDATA,
                },
                {
                    manifest::Nav::Errors::GPSLOCKERROR,
                },
            },
        },
        {
            "SignalStack",
            {
                manifest::SignalStack::IP_ADDRESS,
                {
                    manifest::SignalStack::Commands::OPENLOOP,
                    manifest::SignalStack::Commands::SETANGLETARGET,
                    manifest::SignalStack::Commands::SETGPSTARGET,
                    manifest::SignalStack::Commands::WATCHDOGOVERRIDE,
                },
                {
                    manifest::SignalStack::Telemetry::COMPASSANGLE,
                },
                {},
            },
        },
        {
            "Arm",
            {
                manifest::Arm::IP_ADDRESS,
                {
                    manifest::Arm::Commands::OPENLOOP,
                    manifest::Arm::Commands::TARGETANGLE,
                    manifest::Arm::Commands::TARGETANGLEINCREMENT,
                    manifest::Arm::Commands::GRIPPEROPENLOOP,
                    manifest::Arm::Commands::IKPOSITION,
                    manifest::Arm::Commands::IKPOSITIONINCREMENT,
                    manifest::Arm::Commands::IKPOSEINCREMENT,
                    manifest::Arm::Commands::LASER,
                    manifest::Arm::Commands::LINEARSERVO,
                    manifest::Arm::Commands::CACHE,
                    manifest::Arm::Commands::WATCHDOGOVERRIDE,
                    manifest::Arm::Commands::LIMITSWITCHOVERRIDE,
                    manifest::Arm::Commands::CLOSEDLOOPOVERRIDE,
                    manifest::Arm::Commands::CALIBRATEENCODER,
                    manifest::Arm::Commands::SOFTLIMITOVERRIDE,
                    manifest::Arm::Commands::ARMGIMBAL1,
                    manifest::Arm::Commands::ARMGIMBAL2,
                },
                {
                    manifest::Arm::Telemetry::POSITION,
                    manifest::Arm::Telemetry::LIMITSWITCH,
                    manifest::Arm::Telemetry::SOFTLIMIT,
                    manifest::Arm::Telemetry::SMOCOPING,
                },
                {},
            },
        },
        {
            "Auger",
            {
                manifest::Auger::IP_ADDRESS,
                {
                    manifest::Auger::Commands::AUGERAXIS,
                    manifest::Auger::Commands::LIMITSWITCHOVERRIDE,
                    manifest::Auger::Commands::CALIBRATEENCODER,
                    manifest::Auger::Commands::AUGER,
                    manifest::Auger::Commands::WATCHDOGOVERRIDE,
                    manifest::Auger::Commands::LED,
                    manifest::Auger::Commands::AUGERSERVO,
                    manifest::Auger::Commands::AUGERGIMBAL,
                },
                {
                    manifest::Auger::Telemetry::POSITION,
                    manifest::Auger::Telemetry::AUGERSPEED,
                    manifest::Auger::Telemetry::LIMITSWITCH,
                    manifest::Auger::Telemetry::ENVIRONMENTAL,
                    manifest::Auger::Telemetry::AUGERCURRENT,
                    manifest::Auger::Telemetry::SMOCOPING,
                },
                {},
            },
        },
        {
            "Autonomy",
            {
                manifest::Autonomy::IP_ADDRESS,
                {
                    manifest::Autonomy::Commands::STARTAUTONOMY,
                    manifest::Autonomy::Commands::DISABLEAUTONOMY,
                    manifest::Autonomy::Commands::ADDPOSITIONLEG,
                    manifest::Autonomy::Commands::ADDMARKERLEG,
                    manifest::Autonomy::Commands::ADDOBJECTLEG,
                    manifest::Autonomy::Commands::CLEARWAYPOINTS,
                    manifest::Autonomy::Commands::SETMAXSPEED,
                    manifest::Autonomy::Commands::SETMINTRAVSCORE,
                    manifest::Autonomy::Commands::SETBETABIAS,
                    manifest::Autonomy::Commands::SETLOGGINGLEVELS,
                    manifest::Autonomy::Commands::ADDOBSTACLE,
                    manifest::Autonomy::Commands::CLEAROBSTACLES,
                },
                {
                    manifest::Autonomy::Telemetry::CURRENTSTATE,
                    manifest::Autonomy::Telemetry::STATEDISPLAY,
                    manifest::Autonomy::Telemetry::THREADFPS,
                    manifest::Autonomy::Telemetry::PATHWAYPOINTS,
                    manifest::Autonomy::Telemetry::TIMEREMAINING,
                },
                {},
            },
        },
        {
            "Camera1",
            {
                manifest::Camera1::IP_ADDRESS,
                {
                    manifest::Camera1::Commands::TAKEPICTURE,
                    manifest::Camera1::Commands::TOGGLESTREAM,
                    manifest::Camera1::Commands::SETFFMPEGARGUMENTS,
                    manifest::Camera1::Commands::SETPICTUREARGUMENTS,
                    manifest::Camera1::Commands::SETBRIGHTNESS,
                    manifest::Camera1::Commands::SETCONTRAST,
                },
                {
                    manifest::Camera1::Telemetry::AVAILABLECAMERAS,
                    manifest::Camera1::Telemetry::STREAMINGCAMERAS,
                    manifest::Camera1::Telemetry::PICTURETAKEN,
                    manifest::Camera1::Telemetry::UTILIZATION,
                },
                {},
            },
        },
        {
            "Camera2",
            {
                manifest::Camera2::IP_ADDRESS,
                {
                    manifest::Camera2::Commands::TAKEPICTURE,
                    manifest::Camera2::Commands::TOGGLESTREAM,
                    manifest::Camera2::Commands::SETFFMPEGARGUMENTS,
                    manifest::Camera2::Commands::SETPICTUREARGUMENTS,
                    manifest::Camera2::Commands::SETBRIGHTNESS,
                    manifest::Camera2::Commands::SETCONTRAST,
                },
                {
                    manifest::Camera2::Telemetry::AVAILABLECAMERAS,
                    manifest::Camera2::Telemetry::STREAMINGCAMERAS,
                    manifest::Camera2::Telemetry::PICTURETAKEN,
                    manifest::Camera2::Telemetry::UTILIZATION,
                },
                {},
            },
        },
        {
            "CameraServer",
            {
                manifest::CameraServer::IP_ADDRESS,
                {
                    manifest::CameraServer::Commands::TAKEPHOTO,
                    manifest::CameraServer::Commands::TOGGLESTREAM,
                    manifest::CameraServer::Commands::ADJUSTBRIGHTNESS,
                    manifest::CameraServer::Commands::ADJUSTCONTRAST,
                    manifest::CameraServer::Commands::ADJUSTSATURATION,
                    manifest::CameraServer::Commands::ADJUSTHUE,
                    manifest::CameraServer::Commands::SETWHITEBALANCE,
                    manifest::CameraServer::Commands::ADJUSTBACKLIGHTCONTRAST,
                    manifest::CameraServer::Commands::SETEXPOSURE,
                },
                {
                    manifest::CameraServer::Telemetry::AVAILABLECAMERAS,
                    manifest::CameraServer::Telemetry::STREAMINGCAMERAS,
                    manifest::CameraServer::Telemetry::PICTURETAKEN,
                },
                {
                    manifest::CameraServer::Errors::CAMERAUNAVAILABLE,
                },
            },
        },
        {
            "Raman",
            {
                manifest::Raman::IP_ADDRESS,
                {
                    manifest::Raman::Commands::INSTRUMENTSAXIS,
                    manifest::Raman::Commands::LIMITSWITCHOVERRIDE,
                    manifest::Raman::Commands::CALIBRATEENCODER,
                    manifest::Raman::Commands::WATCHDOGOVERRIDE,
                    manifest::Raman::Commands::LASER,
                    manifest::Raman::Commands::REQUESTRAMANREADING,
                },
                {
                    manifest::Raman::Telemetry::POSITION,
                    manifest::Raman::Telemetry::LIMITSWITCH,
                    manifest::Raman::Telemetry::RAMANREADING_PART1,
                    manifest::Raman::Telemetry::RAMANREADING_PART2,
                    manifest::Raman::Telemetry::RAMANREADING_PART3,
                    manifest::Raman::Telemetry::RAMANREADING_PART4,
                    manifest::Raman::Telemetry::SMOCOPING,
                },
                {},
            },
        },
        {
            "RoveSoSimulator",
            {
                manifest::RoveSoSimulator::IP_ADDRESS,
                {},
                {
                    manifest::RoveSoSimulator::Telemetry::IMU,
                },
                {},
            },
        },
    };

    /******************************************************************************
     * @brief RoveComm Helper Functions
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-18
     ******************************************************************************/
    namespace Helpers
    {
        inline std::optional<BoardEntry> FindBoardById(uint16_t unDataId)
        {
            switch (unDataId / 1000)
            {
                case 3: return BOARDS.at("Core");
                case 4: return BOARDS.at("PMS");
                case 6: return BOARDS.at("Nav");
                case 7: return BOARDS.at("SignalStack");
                case 8: return BOARDS.at("Arm");
                case 9: return BOARDS.at("Auger");
                case 11: return BOARDS.at("Autonomy");
                case 12: return BOARDS.at("Camera1");
                case 13: return BOARDS.at("Camera2");
                case 14: return BOARDS.at("CameraServer");
                case 16: return BOARDS.at("Raman");
                case 99: return BOARDS.at("RoveSoSimulator");
            }
            return {};
        }

        inline std::optional<ManifestEntry> FindEntryById(uint16_t unDataId)
        {
            std::optional<BoardEntry> board = FindBoardById(unDataId);
            if (!board)
            {
                return {};
            }
            if (auto it = std::find_if(board->COMMANDS.begin(), board->COMMANDS.end(), [unDataId](const auto& entry) { return entry.DATA_ID == unDataId; });
                it != board->COMMANDS.end())
            {
                return *it;
            }
            if (auto it = std::find_if(board->TELEMETRY.begin(), board->TELEMETRY.end(), [unDataId](const auto& entry) { return entry.DATA_ID == unDataId; });
                it != board->TELEMETRY.end())
            {
                return *it;
            }
            if (auto it = std::find_if(board->ERRORS.begin(), board->ERRORS.end(), [unDataId](const auto& entry) { return entry.DATA_ID == unDataId; });
                it != board->ERRORS.end())
            {
                return *it;
            }
            return {};
        }

        constexpr size_t DataTypeSize(manifest::DataTypes eDataType)
        {
            switch (eDataType)
            {
                case manifest::DataTypes::INT8_T: return 1;
                case manifest::DataTypes::UINT8_T: return 1;
                case manifest::DataTypes::INT16_T: return 2;
                case manifest::DataTypes::UINT16_T: return 2;
                case manifest::DataTypes::INT32_T: return 4;
                case manifest::DataTypes::UINT32_T: return 4;
                case manifest::DataTypes::FLOAT_T: return 4;
                case manifest::DataTypes::DOUBLE_T: return 8;
                case manifest::DataTypes::CHAR: return 1;
                default: return 1;
            }
        }

        template<typename T>
        struct CToRoveCommType
        {};

        template<>
        struct CToRoveCommType<int8_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::INT8_T;
                static constexpr size_t SIZE              = 1;
        };

        template<>
        struct CToRoveCommType<uint8_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::UINT8_T;
                static constexpr size_t SIZE              = 1;
        };

        template<>
        struct CToRoveCommType<int16_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::INT16_T;
                static constexpr size_t SIZE              = 2;
        };

        template<>
        struct CToRoveCommType<uint16_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::UINT16_T;
                static constexpr size_t SIZE              = 2;
        };

        template<>
        struct CToRoveCommType<int32_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::INT32_T;
                static constexpr size_t SIZE              = 4;
        };

        template<>
        struct CToRoveCommType<uint32_t>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::UINT32_T;
                static constexpr size_t SIZE              = 4;
        };

        template<>
        struct CToRoveCommType<float>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::FLOAT_T;
                static constexpr size_t SIZE              = 4;
        };

        template<>
        struct CToRoveCommType<double>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::DOUBLE_T;
                static constexpr size_t SIZE              = 8;
        };

        template<>
        struct CToRoveCommType<char>
        {
                static constexpr manifest::DataTypes TYPE = manifest::DataTypes::CHAR;
                static constexpr size_t SIZE              = 1;
        };

        template<manifest::DataTypes>
        struct RoveCommToCType
        {};

        template<>
        struct RoveCommToCType<manifest::DataTypes::INT8_T>
        {
                using c_type                 = int8_t;
                static constexpr size_t SIZE = 1;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::UINT8_T>
        {
                using c_type                 = uint8_t;
                static constexpr size_t SIZE = 1;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::INT16_T>
        {
                using c_type                 = int16_t;
                static constexpr size_t SIZE = 2;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::UINT16_T>
        {
                using c_type                 = uint16_t;
                static constexpr size_t SIZE = 2;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::INT32_T>
        {
                using c_type                 = int32_t;
                static constexpr size_t SIZE = 4;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::UINT32_T>
        {
                using c_type                 = uint32_t;
                static constexpr size_t SIZE = 4;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::FLOAT_T>
        {
                using c_type                 = float;
                static constexpr size_t SIZE = 4;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::DOUBLE_T>
        {
                using c_type                 = double;
                static constexpr size_t SIZE = 8;
        };

        template<>
        struct RoveCommToCType<manifest::DataTypes::CHAR>
        {
                using c_type                 = char;
                static constexpr size_t SIZE = 1;
        };

    }    // namespace Helpers

}    // namespace manifest

#endif    // MANIFEST_H
