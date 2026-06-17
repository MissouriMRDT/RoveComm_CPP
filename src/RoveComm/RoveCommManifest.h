/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2026-06-17
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
     * @date 2026-06-17
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
     * @date 2026-06-17
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
     * @date 2026-06-17
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
     * @date 2026-06-17
     ******************************************************************************/
    namespace Core
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 110};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry DRIVELEFTRIGHT{3000, 2, DataTypes::FLOAT_T};
            constexpr ManifestEntry DRIVEINDIVIDUAL{3001, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry WATCHDOGOVERRIDE{3002, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LEFTGIMBAL{3003, 2, DataTypes::INT16_T};
            constexpr ManifestEntry RIGHTGIMBAL{3004, 2, DataTypes::INT16_T};
            constexpr ManifestEntry BACKGIMBAL{3005, 2, DataTypes::INT16_T};
            constexpr ManifestEntry LEDRGB{3006, 3, DataTypes::UINT8_T};
            constexpr ManifestEntry BACKIMAGE{3007, 256, DataTypes::UINT32_T};
            constexpr ManifestEntry INTERNALRGB{3008, 3, DataTypes::UINT8_T};
            constexpr ManifestEntry INTERNALIMAGE{3009, 256, DataTypes::UINT32_T};
            constexpr ManifestEntry STATEDISPLAY{3010, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry BRIGHTNESS{3011, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry SETWATCHDOGMODE{3012, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LEDTEXT{3013, 256, DataTypes::CHAR};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry MOTORSPEEDS{3100, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry MOTORCURRENTS{3101, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry VESCCURRENTS{3102, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry IMUDATA{3103, 2, DataTypes::FLOAT_T};
            constexpr ManifestEntry ACCELEROMETERDATA{3104, 3, DataTypes::FLOAT_T};
            constexpr ManifestEntry THERMAL{3105, 3, DataTypes::FLOAT_T};
        }

        // Error
        namespace Errors
        {
            constexpr ManifestEntry VESCFAULT{3200, 2, DataTypes::UINT8_T};
        }
        
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
     * @date 2026-06-17
     ******************************************************************************/
    namespace PMS
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 102};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry ESTOP{4000, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry SUICIDE{4001, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry REBOOT{4002, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry ENABLEBUS{4003, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry DISABLEBUS{4004, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry SETBUS{4005, 1, DataTypes::UINT8_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry CURRENTANDVOLTAGE{4100, 12, DataTypes::FLOAT_T};
            constexpr ManifestEntry BUSSTATUS{4101, 1, DataTypes::UINT8_T};
        }

        // Error
        namespace Errors
        {
            constexpr ManifestEntry PACKOVERCURRENT{4200, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry CELLUNDERVOLTAGE{4201, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry CELLCRITICAL{4202, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry AUXOVERCURRENT{4203, 0, DataTypes::UINT8_T};
        }
    }    // namespace PMS

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 104};

        // Commands
        namespace Commands
        {}
        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry GPSLATLONALT{6100, 8, DataTypes::DOUBLE_T};
            constexpr ManifestEntry COMPASSDATA{6102, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SATELLITECOUNTDATA{6103, 1, DataTypes::UINT8_T};
        }

        // Error
        namespace Errors
        {
            constexpr ManifestEntry GPSLOCKERROR{6200, 0, DataTypes::UINT8_T};
        }
    }    // namespace Nav

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace SignalStack
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 100, 101};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry OPENLOOP{7000, 1, DataTypes::INT16_T};
            constexpr ManifestEntry SETANGLETARGET{7001, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETGPSTARGET{7002, 4, DataTypes::DOUBLE_T};
            constexpr ManifestEntry WATCHDOGOVERRIDE{7003, 1, DataTypes::UINT8_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry COMPASSANGLE{7100, 1, DataTypes::FLOAT_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace SignalStack

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 107};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry OPENLOOP{8000, 6, DataTypes::INT16_T};
            constexpr ManifestEntry TARGETANGLE{8001, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry TARGETANGLEINCREMENT{8002, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry GRIPPEROPENLOOP{8003, 1, DataTypes::INT16_T};
            constexpr ManifestEntry IKPOSITION{8004, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry IKPOSITIONINCREMENT{8005, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry IKPOSEINCREMENT{8006, 6, DataTypes::FLOAT_T};
            constexpr ManifestEntry LASER{8007, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LINEARSERVO{8008, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry CACHE{8009, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry WATCHDOGOVERRIDE{8010, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{8011, 1, DataTypes::UINT16_T};
            constexpr ManifestEntry CLOSEDLOOPOVERRIDE{8012, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry CALIBRATEENCODER{8013, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry SOFTLIMITOVERRIDE{8014, 1, DataTypes::UINT16_T};
            constexpr ManifestEntry ARMGIMBAL1{8015, 2, DataTypes::INT16_T};
            constexpr ManifestEntry ARMGIMBAL2{8016, 2, DataTypes::INT16_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry POSITION{8100, 9, DataTypes::FLOAT_T};
            constexpr ManifestEntry LIMITSWITCH{8101, 1, DataTypes::UINT16_T};
            constexpr ManifestEntry SOFTLIMIT{8102, 1, DataTypes::UINT16_T};
            constexpr ManifestEntry SMOCOPING{8103, 7, DataTypes::UINT16_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace Arm

    /******************************************************************************
     * @brief Auger Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Auger
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 108};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry AUGERAXIS{9000, 1, DataTypes::INT16_T};
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{9001, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry CALIBRATEENCODER{9002, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry AUGER{9003, 1, DataTypes::INT16_T};
            constexpr ManifestEntry WATCHDOGOVERRIDE{9004, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LED{9005, 4, DataTypes::UINT8_T};
            constexpr ManifestEntry AUGERSERVO{9006, 2, DataTypes::INT16_T};
            constexpr ManifestEntry AUGERGIMBAL{9007, 2, DataTypes::INT16_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry POSITION{9100, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry AUGERSPEED{9101, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry LIMITSWITCH{9102, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry ENVIRONMENTAL{9103, 2, DataTypes::FLOAT_T};
            constexpr ManifestEntry AUGERCURRENT{9104, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SMOCOPING{9105, 1, DataTypes::UINT16_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace Auger

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 100};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry STARTAUTONOMY{11000, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry DISABLEAUTONOMY{11001, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry ADDPOSITIONLEG{11002, 3, DataTypes::DOUBLE_T};
            constexpr ManifestEntry ADDMARKERLEG{11003, 4, DataTypes::DOUBLE_T};
            constexpr ManifestEntry ADDOBJECTLEG{11004, 4, DataTypes::DOUBLE_T};
            constexpr ManifestEntry CLEARWAYPOINTS{11005, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry SETMAXSPEED{11006, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETMINTRAVSCORE{11007, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETBETABIAS{11008, 1, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETLOGGINGLEVELS{11009, 3, DataTypes::UINT8_T};
            constexpr ManifestEntry ADDOBSTACLE{11010, 3, DataTypes::DOUBLE_T};
            constexpr ManifestEntry CLEAROBSTACLES{11011, 1, DataTypes::UINT8_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry CURRENTSTATE{11100, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry STATEDISPLAY{11101, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry THREADFPS{11103, 2, DataTypes::UINT32_T};
            constexpr ManifestEntry PATHWAYPOINTS{11104, 1000, DataTypes::DOUBLE_T};
            constexpr ManifestEntry TIMEREMAINING{11105, 1, DataTypes::DOUBLE_T};
        }

        // Error
        namespace Errors
        {}
        
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
     * @date 2026-06-17
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 100};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry TAKEPICTURE{12000, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry TOGGLESTREAM{12001, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry SETFFMPEGARGUMENTS{12002, 16384, DataTypes::CHAR};
            constexpr ManifestEntry SETPICTUREARGUMENTS{12003, 16384, DataTypes::CHAR};
            constexpr ManifestEntry SETBRIGHTNESS{12004, 4, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETCONTRAST{12005, 4, DataTypes::FLOAT_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry AVAILABLECAMERAS{12100, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry STREAMINGCAMERAS{12101, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry PICTURETAKEN{12102, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry UTILIZATION{12103, 6, DataTypes::UINT8_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Camera2
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 101};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry TAKEPICTURE{13000, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry TOGGLESTREAM{13001, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry SETFFMPEGARGUMENTS{13002, 16384, DataTypes::CHAR};
            constexpr ManifestEntry SETPICTUREARGUMENTS{13003, 16384, DataTypes::CHAR};
            constexpr ManifestEntry SETBRIGHTNESS{13004, 4, DataTypes::FLOAT_T};
            constexpr ManifestEntry SETCONTRAST{13005, 4, DataTypes::FLOAT_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry AVAILABLECAMERAS{13100, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry STREAMINGCAMERAS{13101, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry PICTURETAKEN{13102, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry UTILIZATION{13103, 6, DataTypes::UINT8_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace Camera2

    /******************************************************************************
     * @brief CameraServer Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace CameraServer
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 102};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry TAKEPHOTO{14000, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry TOGGLESTREAM{14001, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry ADJUSTBRIGHTNESS{14002, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry ADJUSTCONTRAST{14003, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry ADJUSTSATURATION{14004, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry ADJUSTHUE{14005, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry SETWHITEBALANCE{14008, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry ADJUSTBACKLIGHTCONTRAST{14009, 2, DataTypes::UINT8_T};
            constexpr ManifestEntry SETEXPOSURE{14010, 2, DataTypes::INT32_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry AVAILABLECAMERAS{14100, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry STREAMINGCAMERAS{14101, 4, DataTypes::UINT8_T};
            constexpr ManifestEntry PICTURETAKEN{14102, 0, DataTypes::UINT8_T};
        }

        // Error
        namespace Errors
        {
            constexpr ManifestEntry CAMERAUNAVAILABLE{14200, 1, DataTypes::UINT8_T};
        }
    }    // namespace CameraServer

    /******************************************************************************
     * @brief Raman Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace Raman
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 105};

        // Commands
        namespace Commands
        {
            constexpr ManifestEntry INSTRUMENTSAXIS{16000, 1, DataTypes::INT16_T};
            constexpr ManifestEntry LIMITSWITCHOVERRIDE{16001, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry CALIBRATEENCODER{16002, 0, DataTypes::UINT8_T};
            constexpr ManifestEntry WATCHDOGOVERRIDE{16003, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry LASER{16004, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry REQUESTRAMANREADING{16005, 1, DataTypes::UINT32_T};
        }

        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry POSITION{16100, 2, DataTypes::FLOAT_T};
            constexpr ManifestEntry LIMITSWITCH{16101, 1, DataTypes::UINT8_T};
            constexpr ManifestEntry RAMANREADING_PART1{16102, 512, DataTypes::UINT16_T};
            constexpr ManifestEntry RAMANREADING_PART2{16103, 512, DataTypes::UINT16_T};
            constexpr ManifestEntry RAMANREADING_PART3{16104, 512, DataTypes::UINT16_T};
            constexpr ManifestEntry RAMANREADING_PART4{16105, 512, DataTypes::UINT16_T};
            constexpr ManifestEntry SMOCOPING{16106, 1, DataTypes::UINT16_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace Raman

    /******************************************************************************
     * @brief RoveSoSimulator Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
     ******************************************************************************/
    namespace RoveSoSimulator
    {
        // IP Address
        const AddressEntry IP_ADDRESS{127, 0, 0, 1};

        // Commands
        namespace Commands
        {}
        // Telemetry
        namespace Telemetry
        {
            constexpr ManifestEntry IMU{99100, 10, DataTypes::DOUBLE_T};
        }

        // Error
        namespace Errors
        {}
    }    // namespace RoveSoSimulator

    /******************************************************************************
     * @brief RoveComm General Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2026-06-17
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
     * @date 2026-06-17
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
     * @date 2026-06-17
     ******************************************************************************/
    namespace Helpers
    {
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
