/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2026-02-16
 *
 * @copyright Copyright Mars Rover Design Team 2026 - All Rights Reserved
 ******************************************************************************/

#ifndef MANIFEST_H
#define MANIFEST_H

#include <map>
#include <stdint.h>
#include <string>

namespace manifest
{
    /******************************************************************************
     * @brief Enumeration of Data Types to be used in RoveComm
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
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
     * @brief IP Address Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    struct AddressEntry
    {
        public:
            int FIRST_OCTET;
            int SECOND_OCTET;
            int THIRD_OCTET;
            int FOURTH_OCTET;
            std::string IP_STR = std::to_string(FIRST_OCTET) + "." + std::to_string(SECOND_OCTET) + "." + std::to_string(THIRD_OCTET) + "." + std::to_string(FOURTH_OCTET);
    };

    /******************************************************************************
     * @brief Manifest Entry Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    struct ManifestEntry
    {
        public:
            int DATA_ID;
            int DATA_COUNT;
            DataTypes DATA_TYPE;
    };

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Core
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 110};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"DRIVELEFTRIGHT", ManifestEntry{3000, 2, DataTypes::FLOAT_T}},
            {"DRIVEINDIVIDUAL", ManifestEntry{3001, 6, DataTypes::FLOAT_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{3002, 1, DataTypes::UINT8_T}},
            {"LEFTGIMBAL", ManifestEntry{3003, 2, DataTypes::INT16_T}},
            {"RIGHTGIMBAL", ManifestEntry{3004, 2, DataTypes::INT16_T}},
            {"BACKGIMBAL", ManifestEntry{3005, 2, DataTypes::INT16_T}},
            {"LEDRGB", ManifestEntry{3006, 3, DataTypes::UINT8_T}},
            {"BACKIMAGE", ManifestEntry{3007, 256, DataTypes::UINT32_T}},
            {"INTERNALRGB", ManifestEntry{3008, 3, DataTypes::UINT8_T}},
            {"INTERNALIMAGE", ManifestEntry{3009, 256, DataTypes::UINT32_T}},
            {"STATEDISPLAY", ManifestEntry{3010, 1, DataTypes::UINT8_T}},
            {"BRIGHTNESS", ManifestEntry{3011, 1, DataTypes::UINT8_T}},
            {"SETWATCHDOGMODE", ManifestEntry{3012, 1, DataTypes::UINT8_T}},
            {"LEDTEXT", ManifestEntry{3013, 256, DataTypes::CHAR}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"MOTORSPEEDS", ManifestEntry{3100, 6, DataTypes::FLOAT_T}},
            {"MOTORCURRENTS", ManifestEntry{3101, 6, DataTypes::FLOAT_T}},
            {"VESCCURRENTS", ManifestEntry{3102, 6, DataTypes::FLOAT_T}},
            {"IMUDATA", ManifestEntry{3103, 2, DataTypes::FLOAT_T}},
            {"ACCELEROMETERDATA", ManifestEntry{3104, 3, DataTypes::FLOAT_T}},
            {"THERMAL", ManifestEntry{3105, 3, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"VESCFAULT", ManifestEntry{3200, 2, DataTypes::UINT8_T}},
        };
        
        // Enums
        enum class MOTORS
        {
            FRONT_LEFT = 0,
            MIDDLE_LEFT = 1,
            BACK_LEFT = 2,
            FRONT_RIGHT = 3,
            MIDDLE_RIGHT = 4,
            BACK_RIGHT = 5
        }; 

        enum class DISPLAYSTATE
        {
            TELEOP = 0,
            AUTONOMY = 1,
            REACHED_GOAL = 2
        }; 

        enum class VESCFAULTCODE
        {
            NONE = 0,
            OVER_VOLTAGE = 1,
            UNDER_VOLTAGE = 2,
            DRV = 3,
            ABS_OVER_CURRENT = 4,
            OVER_TEMP_FET = 5,
            OVER_TEMP_MOTOR = 6,
            GATE_DRIVER_OVER_VOLTAGE = 7,
            GATE_DRIVER_UNDER_VOLTAGE = 8,
            MCU_UNDER_VOLTAGE = 9,
            BOOTING_FROM_WATCHDOG_RESET = 10,
            ENCODER_SPI = 11,
            ENCODER_SINCOS_BELOW_MIN_AMPLITUDE = 12,
            ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE = 13,
            FLASH_CORRUPTION = 14,
            HIGH_OFFSET_CURRENT_SENSOR_1 = 15,
            HIGH_OFFSET_CURRENT_SENSOR_2 = 16,
            HIGH_OFFSET_CURRENT_SENSOR_3 = 17,
            UNBALANCED_CURRENTS = 18,
            BRK = 19,
            RESOLVER_LOT = 20,
            RESOLVER_DOS = 21,
            RESOLVER_LOS = 22,
            FLASH_CORRUPTION_APP_CFG = 23,
            FLASH_CORRUPTION_MC_CFG = 24,
            ENCODER_NO_MAGNET = 25,
            ENCODER_MAGNET_TOO_STRONG = 26,
            PHASE_FILTER = 27
        }; 

    }    // namespace Core

    /******************************************************************************
     * @brief PMS Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace PMS
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 102};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ESTOP", ManifestEntry{4000, 0, DataTypes::UINT8_T}},
            {"SUICIDE", ManifestEntry{4001, 0, DataTypes::UINT8_T}},
            {"REBOOT", ManifestEntry{4002, 0, DataTypes::UINT8_T}},
            {"ENABLEBUS", ManifestEntry{4003, 1, DataTypes::UINT8_T}},
            {"DISABLEBUS", ManifestEntry{4004, 1, DataTypes::UINT8_T}},
            {"SETBUS", ManifestEntry{4005, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"CURRENTANDVOLTAGE", ManifestEntry{4100, 12, DataTypes::FLOAT_T}},
            {"BUSSTATUS", ManifestEntry{4101, 1, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"PACKOVERCURRENT", ManifestEntry{4200, 0, DataTypes::UINT8_T}},
            {"CELLUNDERVOLTAGE", ManifestEntry{4201, 1, DataTypes::UINT8_T}},
            {"CELLCRITICAL", ManifestEntry{4202, 1, DataTypes::UINT8_T}},
            {"AUXOVERCURRENT", ManifestEntry{4203, 0, DataTypes::UINT8_T}},
        };
    }    // namespace PMS

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 104};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"GPSLATLONALT", ManifestEntry{6100, 8, DataTypes::DOUBLE_T}},
            {"COMPASSDATA", ManifestEntry{6102, 1, DataTypes::FLOAT_T}},
            {"SATELLITECOUNTDATA", ManifestEntry{6103, 1, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"GPSLOCKERROR", ManifestEntry{6200, 0, DataTypes::UINT8_T}},
        };
    }    // namespace Nav

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace SignalStack
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 100, 101};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"OPENLOOP", ManifestEntry{7000, 1, DataTypes::INT16_T}},
            {"SETANGLETARGET", ManifestEntry{7001, 1, DataTypes::FLOAT_T}},
            {"SETGPSTARGET", ManifestEntry{7002, 4, DataTypes::DOUBLE_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{7003, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"COMPASSANGLE", ManifestEntry{7100, 1, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace SignalStack

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 107};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"OPENLOOP", ManifestEntry{8000, 6, DataTypes::INT16_T}},
            {"TARGETANGLE", ManifestEntry{8001, 6, DataTypes::FLOAT_T}},
            {"TARGETANGLEINCREMENT", ManifestEntry{8002, 6, DataTypes::FLOAT_T}},
            {"GRIPPEROPENLOOP", ManifestEntry{8003, 1, DataTypes::INT16_T}},
            {"IKPOSITION", ManifestEntry{8004, 6, DataTypes::FLOAT_T}},
            {"IKPOSITIONINCREMENT", ManifestEntry{8005, 6, DataTypes::FLOAT_T}},
            {"IKPOSEINCREMENT", ManifestEntry{8006, 6, DataTypes::FLOAT_T}},
            {"LASER", ManifestEntry{8007, 1, DataTypes::UINT8_T}},
            {"LINEARSERVO", ManifestEntry{8008, 1, DataTypes::UINT8_T}},
            {"CACHE", ManifestEntry{8009, 1, DataTypes::UINT8_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{8010, 1, DataTypes::UINT8_T}},
            {"LIMITSWITCHOVERRIDE", ManifestEntry{8011, 1, DataTypes::UINT16_T}},
            {"CLOSEDLOOPOVERRIDE", ManifestEntry{8012, 1, DataTypes::UINT8_T}},
            {"CALIBRATEENCODER", ManifestEntry{8013, 1, DataTypes::UINT8_T}},
            {"SOFTLIMITOVERRIDE", ManifestEntry{8014, 1, DataTypes::UINT16_T}},
            {"ARMGIMBAL1", ManifestEntry{8015, 2, DataTypes::INT16_T}},
            {"ARMGIMBAL2", ManifestEntry{8016, 2, DataTypes::INT16_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"POSITION", ManifestEntry{8100, 9, DataTypes::FLOAT_T}},
            {"LIMITSWITCH", ManifestEntry{8101, 1, DataTypes::UINT16_T}},
            {"SOFTLIMIT", ManifestEntry{8102, 1, DataTypes::UINT16_T}},
            {"SMOCOPING", ManifestEntry{8103, 7, DataTypes::UINT16_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Arm

    /******************************************************************************
     * @brief Auger Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Auger
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 108};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"AUGERAXIS", ManifestEntry{9000, 1, DataTypes::INT16_T}},
            {"LIMITSWITCHOVERRIDE", ManifestEntry{9001, 1, DataTypes::UINT8_T}},
            {"CALIBRATEENCODER", ManifestEntry{9002, 0, DataTypes::UINT8_T}},
            {"AUGER", ManifestEntry{9003, 1, DataTypes::INT16_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{9004, 1, DataTypes::UINT8_T}},
            {"LED", ManifestEntry{9005, 4, DataTypes::UINT8_T}},
            {"AUGERSERVO", ManifestEntry{9006, 2, DataTypes::INT16_T}},
            {"AUGERGIMBAL", ManifestEntry{9007, 2, DataTypes::INT16_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"POSITION", ManifestEntry{9100, 1, DataTypes::FLOAT_T}},
            {"AUGERSPEED", ManifestEntry{9101, 1, DataTypes::FLOAT_T}},
            {"LIMITSWITCH", ManifestEntry{9102, 1, DataTypes::UINT8_T}},
            {"ENVIRONMENTAL", ManifestEntry{9103, 2, DataTypes::FLOAT_T}},
            {"AUGERCURRENT", ManifestEntry{9104, 1, DataTypes::FLOAT_T}},
            {"SMOCOPING", ManifestEntry{9105, 1, DataTypes::UINT16_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Auger

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 100};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"STARTAUTONOMY", ManifestEntry{11000, 1, DataTypes::UINT8_T}},
            {"DISABLEAUTONOMY", ManifestEntry{11001, 1, DataTypes::UINT8_T}},
            {"ADDPOSITIONLEG", ManifestEntry{11002, 3, DataTypes::DOUBLE_T}},
            {"ADDMARKERLEG", ManifestEntry{11003, 4, DataTypes::DOUBLE_T}},
            {"ADDOBJECTLEG", ManifestEntry{11004, 4, DataTypes::DOUBLE_T}},
            {"CLEARWAYPOINTS", ManifestEntry{11005, 1, DataTypes::UINT8_T}},
            {"SETMAXSPEED", ManifestEntry{11006, 1, DataTypes::FLOAT_T}},
            {"SETMINTRAVSCORE", ManifestEntry{11007, 1, DataTypes::FLOAT_T}},
            {"SETBETABIAS", ManifestEntry{11008, 1, DataTypes::FLOAT_T}},
            {"SETLOGGINGLEVELS", ManifestEntry{11009, 3, DataTypes::UINT8_T}},
            {"ADDOBSTACLE", ManifestEntry{11010, 3, DataTypes::DOUBLE_T}},
            {"CLEAROBSTACLES", ManifestEntry{11011, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"CURRENTSTATE", ManifestEntry{11100, 1, DataTypes::UINT8_T}},
            {"STATEDISPLAY", ManifestEntry{11101, 1, DataTypes::UINT8_T}},
            {"THREADFPS", ManifestEntry{11103, 2, DataTypes::UINT32_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
        enum class AUTONOMYSTATE
        {
            IDLE = 0,
            NAVIGATING = 1,
            SEARCHPATTERN = 2,
            APPROACHINGMARKER = 3,
            APPROACHINGOBJECT = 4,
            VERIFYINGGPS = 5,
            VERIFYINGMARKER = 6,
            VERIFYINGOBJECT = 7,
            REVERSING = 8,
            STUCK = 9
        }; 

        enum class AUTONOMYLOG
        {
            TRACEL3 = 0,
            TRACEL2 = 1,
            TRACEL1 = 2,
            DEBUG = 3,
            INFO = 4,
            NOTICE = 5,
            WARNING = 6,
            ERROR = 7,
            CRITICAL = 8
        }; 

        enum class AUTONOMYTHREADS
        {
            NOTSET = 0,
            MAINPROCESS = 1,
            MAINCAM = 2,
            REARCAM = 3,
            TAGDETECTOR = 4,
            OBJECTDETECTOR = 5,
            STATEMACHINE = 6,
            ROVECOMMUDP = 7,
            ROVECOMMTCP = 8
        }; 

        enum class AUTONOMYWAYPOINTTYPES
        {
            CONTINUOUSNAVIGATE = -99,
            ROCKPICK = -4,
            WATERBOTTLE = -3,
            MALLET = -2,
            ANY = -1,
            TAG0 = 0,
            TAG1 = 1,
            TAG2 = 2,
            TAG3 = 3
        }; 

    }    // namespace Autonomy

    /******************************************************************************
     * @brief Camera1 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 100};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"TAKEPICTURE", ManifestEntry{12000, 2, DataTypes::UINT8_T}},
            {"TOGGLESTREAM", ManifestEntry{12001, 2, DataTypes::UINT8_T}},
            {"SETFFMPEGARGUMENTS", ManifestEntry{12002, 16384, DataTypes::CHAR}},
            {"SETPICTUREARGUMENTS", ManifestEntry{12003, 16384, DataTypes::CHAR}},
            {"SETBRIGHTNESS", ManifestEntry{12004, 4, DataTypes::FLOAT_T}},
            {"SETCONTRAST", ManifestEntry{12005, 4, DataTypes::FLOAT_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"AVAILABLECAMERAS", ManifestEntry{12100, 1, DataTypes::UINT8_T}},
            {"STREAMINGCAMERAS", ManifestEntry{12101, 1, DataTypes::UINT8_T}},
            {"PICTURETAKEN", ManifestEntry{12102, 0, DataTypes::UINT8_T}},
            {"UTILIZATION", ManifestEntry{12103, 6, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Camera2
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 101};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"TAKEPICTURE", ManifestEntry{13000, 2, DataTypes::UINT8_T}},
            {"TOGGLESTREAM", ManifestEntry{13001, 2, DataTypes::UINT8_T}},
            {"SETFFMPEGARGUMENTS", ManifestEntry{13002, 16384, DataTypes::CHAR}},
            {"SETPICTUREARGUMENTS", ManifestEntry{13003, 16384, DataTypes::CHAR}},
            {"SETBRIGHTNESS", ManifestEntry{13004, 4, DataTypes::FLOAT_T}},
            {"SETCONTRAST", ManifestEntry{13005, 4, DataTypes::FLOAT_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"AVAILABLECAMERAS", ManifestEntry{13100, 1, DataTypes::UINT8_T}},
            {"STREAMINGCAMERAS", ManifestEntry{13101, 1, DataTypes::UINT8_T}},
            {"PICTURETAKEN", ManifestEntry{13102, 0, DataTypes::UINT8_T}},
            {"UTILIZATION", ManifestEntry{13103, 6, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Camera2

    /******************************************************************************
     * @brief CameraServer Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace CameraServer
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 102};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"TAKEPHOTO", ManifestEntry{14000, 1, DataTypes::UINT8_T}},
            {"TOGGLESTREAM", ManifestEntry{14001, 2, DataTypes::UINT8_T}},
            {"ADJUSTBRIGHTNESS", ManifestEntry{14002, 2, DataTypes::UINT8_T}},
            {"ADJUSTCONTRAST", ManifestEntry{14003, 2, DataTypes::UINT8_T}},
            {"ADJUSTSATURATION", ManifestEntry{14004, 2, DataTypes::UINT8_T}},
            {"ADJUSTHUE", ManifestEntry{14005, 2, DataTypes::UINT8_T}},
            {"SETWHITEBALANCE", ManifestEntry{14008, 2, DataTypes::UINT8_T}},
            {"ADJUSTBACKLIGHTCONTRAST", ManifestEntry{14009, 2, DataTypes::UINT8_T}},
            {"SETEXPOSURE", ManifestEntry{14010, 2, DataTypes::INT32_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"AVAILABLECAMERAS", ManifestEntry{14100, 1, DataTypes::UINT8_T}},
            {"STREAMINGCAMERAS", ManifestEntry{14101, 4, DataTypes::UINT8_T}},
            {"PICTURETAKEN", ManifestEntry{14102, 0, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"CAMERAUNAVAILABLE", ManifestEntry{14200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace CameraServer

    /******************************************************************************
     * @brief Raman Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Raman
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 105};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"INSTRUMENTSAXIS", ManifestEntry{16000, 1, DataTypes::INT16_T}},
            {"LIMITSWITCHOVERRIDE", ManifestEntry{16001, 1, DataTypes::UINT8_T}},
            {"CALIBRATEENCODER", ManifestEntry{16002, 0, DataTypes::UINT8_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{16003, 1, DataTypes::UINT8_T}},
            {"LASER", ManifestEntry{16004, 1, DataTypes::UINT8_T}},
            {"REQUESTRAMANREADING", ManifestEntry{16005, 1, DataTypes::UINT32_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"POSITION", ManifestEntry{16100, 2, DataTypes::FLOAT_T}},
            {"LIMITSWITCH", ManifestEntry{16101, 1, DataTypes::UINT8_T}},
            {"RAMANREADING_PART1", ManifestEntry{16102, 512, DataTypes::UINT16_T}},
            {"RAMANREADING_PART2", ManifestEntry{16103, 512, DataTypes::UINT16_T}},
            {"RAMANREADING_PART3", ManifestEntry{16104, 512, DataTypes::UINT16_T}},
            {"RAMANREADING_PART4", ManifestEntry{16105, 512, DataTypes::UINT16_T}},
            {"SMOCOPING", ManifestEntry{16106, 1, DataTypes::UINT16_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Raman

    /******************************************************************************
     * @brief RoveSoSimulator Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace RoveSoSimulator
    {
        // IP Address
        const AddressEntry IP_ADDRESS{127, 0, 0, 1};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"IMU", ManifestEntry{99100, 10, DataTypes::DOUBLE_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace RoveSoSimulator

    /******************************************************************************
     * @brief RoveComm General Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
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
     * @date 2026-02-16
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
     * @brief RoveComm Helper Functions
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-02-16
     ******************************************************************************/
    namespace Helpers
    {
        inline DataTypes GetDataTypeFromMap(const std::map<std::string, ManifestEntry>& dataMap, uint16_t dataId)
        {
            for (const auto& entry : dataMap)
            {
                if (entry.second.DATA_ID == dataId)
                {
                    return entry.second.DATA_TYPE;
                }
            }
            return DataTypes::CHAR;    // Default return value if dataId not found
        }
        
        inline DataTypes GetDataTypeFromId(uint16_t dataId)
        {
            int boardId      = dataId / 1000;          // Determine board ID based on thousands place
            int dataTypeCode = (dataId / 100) % 10;    // Determine data type code based on hundreds place
            
            // Determine the board namespace based on boardId
            switch (boardId)
            {
                case 3:    // Core Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Core::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Core::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Core::ERROR, dataId);
                    }
                    break;
                case 4:    // PMS Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(PMS::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(PMS::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(PMS::ERROR, dataId);
                    }
                    break;
                case 6:    // Nav Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Nav::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Nav::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Nav::ERROR, dataId);
                    }
                    break;
                case 7:    // SignalStack Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(SignalStack::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(SignalStack::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(SignalStack::ERROR, dataId);
                    }
                    break;
                case 8:    // Arm Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Arm::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Arm::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Arm::ERROR, dataId);
                    }
                    break;
                case 9:    // Auger Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Auger::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Auger::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Auger::ERROR, dataId);
                    }
                    break;
                case 11:    // Autonomy Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Autonomy::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Autonomy::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Autonomy::ERROR, dataId);
                    }
                    break;
                case 12:    // Camera1 Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Camera1::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Camera1::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Camera1::ERROR, dataId);
                    }
                    break;
                case 13:    // Camera2 Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Camera2::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Camera2::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Camera2::ERROR, dataId);
                    }
                    break;
                case 14:    // CameraServer Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(CameraServer::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(CameraServer::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(CameraServer::ERROR, dataId);
                    }
                    break;
                case 16:    // Raman Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Raman::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Raman::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Raman::ERROR, dataId);
                    }
                    break;
                case 99:    // RoveSoSimulator Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(RoveSoSimulator::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(RoveSoSimulator::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(RoveSoSimulator::ERROR, dataId);
                    }
                    break;
                default:
                    // Invalid Board ID
                    break;
            }
            
            // If dataId is not found in any namespace, return a default type
            return DataTypes::CHAR;
        }
    }    // namespace Helpers

}    // namespace manifest

#endif    // MANIFEST_H
