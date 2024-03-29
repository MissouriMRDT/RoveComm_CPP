/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2024-03-25
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
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
     * @date 2024-03-25
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
     * @date 2024-03-25
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
     * @date 2024-03-25
     ******************************************************************************/
    struct ManifestEntry
    {
        public:
            int DATA_ID;
            int DATA_COUNT;
            DataTypes DATA_TYPE;
    };

    /******************************************************************************
     * @brief BMS Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace BMS
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 100};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ESTOP", ManifestEntry{1000, 1, DataTypes::UINT8_T}},
            {"SUICIDE", ManifestEntry{1001, 1, DataTypes::UINT8_T}},
            {"REBOOT", ManifestEntry{1002, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"PACKCURRENT", ManifestEntry{1100, 1, DataTypes::FLOAT_T}},
            {"PACKVOLTAGE", ManifestEntry{1101, 1, DataTypes::FLOAT_T}},
            {"CELLVOLTAGE", ManifestEntry{1102, 8, DataTypes::FLOAT_T}},
            {"PACKTEMP", ManifestEntry{1103, 1, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"OVERCURRENT", ManifestEntry{1200, 1, DataTypes::UINT8_T}},
            {"CELLUNDERVOLTAGE", ManifestEntry{1201, 1, DataTypes::UINT8_T}},
            {"CELLCRITICAL", ManifestEntry{1202, 1, DataTypes::UINT8_T}},
            {"PACKOVERHEAT", ManifestEntry{1203, 1, DataTypes::UINT8_T}},
        };
    }    // namespace BMS

    /******************************************************************************
     * @brief Power Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Power
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 101};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ENABLEBUS", ManifestEntry{2000, 1, DataTypes::UINT16_T}},
            {"DISABLEBUS", ManifestEntry{2001, 1, DataTypes::UINT16_T}},
            {"SETBUS", ManifestEntry{2002, 1, DataTypes::UINT16_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"BUSSTATUS", ManifestEntry{2100, 1, DataTypes::UINT16_T}},
            {"BUSCURRENT", ManifestEntry{2101, 18, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"BUSOVERCURRENT", ManifestEntry{2200, 1, DataTypes::UINT32_T}},
        };
    }    // namespace Power

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
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
            {"LEFTDRIVEGIMBALINCREMENT", ManifestEntry{3003, 1, DataTypes::INT16_T}},
            {"RIGHTDRIVEGIMBALINCREMENT", ManifestEntry{3004, 1, DataTypes::INT16_T}},
            {"LEFTMAINGIMBALINCREMENT", ManifestEntry{3005, 2, DataTypes::INT16_T}},
            {"RIGHTMAINGIMBALINCREMENT", ManifestEntry{3006, 2, DataTypes::INT16_T}},
            {"BACKDRIVEGIMBALINCREMENT", ManifestEntry{3007, 1, DataTypes::INT16_T}},
            {"LEDRGB", ManifestEntry{3008, 3, DataTypes::UINT8_T}},
            {"LEDPATTERNS", ManifestEntry{3009, 1, DataTypes::UINT8_T}},
            {"STATEDISPLAY", ManifestEntry{3010, 1, DataTypes::UINT8_T}},
            {"BRIGHTNESS", ManifestEntry{3011, 1, DataTypes::UINT8_T}},
            {"SETWATCHDOGMODE", ManifestEntry{3012, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"DRIVESPEEDS", ManifestEntry{3100, 6, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
        enum class DISPLAYSTATE
        {
            TELEOP,
            AUTONOMY,
            REACHED_GOAL
        }; 

        inline uint8_t EnumToInteger(DISPLAYSTATE state)
        {
            return static_cast<uint8_t>(state);
        } 

        enum class PATTERNS
        {
            MRDT,
            BELGIUM,
            MERICA,
            DIRT,
            DOTA,
            MCD,
            WINDOWS
        }; 

        inline uint8_t EnumToInteger(PATTERNS state)
        {
            return static_cast<uint8_t>(state);
        } 

    }    // namespace Core

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 104};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"GPSLATLONALT", ManifestEntry{6100, 3, DataTypes::DOUBLE_T}},
            {"IMUDATA", ManifestEntry{6101, 3, DataTypes::FLOAT_T}},
            {"COMPASSDATA", ManifestEntry{6102, 1, DataTypes::FLOAT_T}},
            {"SATELLITECOUNTDATA", ManifestEntry{6103, 1, DataTypes::UINT8_T}},
            {"ACCELEROMETERDATA", ManifestEntry{6104, 3, DataTypes::FLOAT_T}},
            {"ACCURACYDATA", ManifestEntry{6105, 3, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"GPSLOCKERROR", ManifestEntry{6200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace Nav

    /******************************************************************************
     * @brief BaseStationNav Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace BaseStationNav
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 100, 112};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace BaseStationNav

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
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
        const std::map<std::string, ManifestEntry> ERROR = {
            {"WATCHDOGSTATUS", ManifestEntry{7200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace SignalStack

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 107};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"OPENLOOP", ManifestEntry{8000, 7, DataTypes::INT16_T}},
            {"SETPOSITION", ManifestEntry{8001, 6, DataTypes::FLOAT_T}},
            {"INCREMENTPOSITION", ManifestEntry{8002, 6, DataTypes::FLOAT_T}},
            {"SETIK", ManifestEntry{8003, 5, DataTypes::FLOAT_T}},
            {"INCREMENTIK_ROVERRELATIVE", ManifestEntry{8004, 5, DataTypes::FLOAT_T}},
            {"INCREMENTIK_WRISTRELATIVE", ManifestEntry{8005, 5, DataTypes::FLOAT_T}},
            {"LASER", ManifestEntry{8006, 1, DataTypes::UINT8_T}},
            {"SOLENOID", ManifestEntry{8007, 1, DataTypes::UINT8_T}},
            {"GRIPPER", ManifestEntry{8008, 1, DataTypes::INT16_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{8009, 1, DataTypes::UINT8_T}},
            {"LIMITSWITCHOVERRIDE", ManifestEntry{8010, 1, DataTypes::UINT8_T}},
            {"CALIBRATEENCODER", ManifestEntry{8011, 1, DataTypes::UINT8_T}},
            {"SELECTGRIPPER", ManifestEntry{8012, 1, DataTypes::UINT8_T}},
            {"CAMERATILT", ManifestEntry{8013, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"POSITIONS", ManifestEntry{8100, 7, DataTypes::FLOAT_T}},
            {"COORDINATES", ManifestEntry{8101, 5, DataTypes::FLOAT_T}},
            {"LIMITSWITCHTRIGGERED", ManifestEntry{8102, 1, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"WATCHDOGSTATUS", ManifestEntry{8200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace Arm

    /******************************************************************************
     * @brief ScienceActuation Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace ScienceActuation
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 2, 108};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"SCOOPAXIS_OPENLOOP", ManifestEntry{9000, 1, DataTypes::INT16_T}},
            {"SENSORAXIS_OPENLOOP", ManifestEntry{9001, 1, DataTypes::INT16_T}},
            {"SCOOPAXIS_SETPOSITION", ManifestEntry{9002, 1, DataTypes::FLOAT_T}},
            {"SENSORAXIS_SETPOSITION", ManifestEntry{9003, 1, DataTypes::FLOAT_T}},
            {"SCOOPAXIS_INCREMENTPOSITION", ManifestEntry{9004, 1, DataTypes::FLOAT_T}},
            {"SENSORAXIS_INCREMENTPOSITION", ManifestEntry{9005, 1, DataTypes::FLOAT_T}},
            {"LIMITSWITCHOVERRIDE", ManifestEntry{9006, 1, DataTypes::UINT8_T}},
            {"AUGER", ManifestEntry{9007, 1, DataTypes::INT16_T}},
            {"MICROSCOPE", ManifestEntry{9008, 1, DataTypes::UINT8_T}},
            {"PROBOSCIS", ManifestEntry{9009, 1, DataTypes::INT16_T}},
            {"WATCHDOGOVERRIDE", ManifestEntry{9010, 1, DataTypes::UINT8_T}},
            {"CALIBRATEENCODER", ManifestEntry{9011, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"POSITIONS", ManifestEntry{9100, 3, DataTypes::FLOAT_T}},
            {"LIMITSWITCHTRIGGERED", ManifestEntry{9101, 1, DataTypes::UINT8_T}},
            {"ENVIRONMENTALDATA", ManifestEntry{9102, 2, DataTypes::FLOAT_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"WATCHDOGSTATUS", ManifestEntry{9200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace ScienceActuation

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 100};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"STARTAUTONOMY", ManifestEntry{11000, 1, DataTypes::UINT8_T}},
            {"DISABLEAUTONOMY", ManifestEntry{11001, 1, DataTypes::UINT8_T}},
            {"ADDPOSITIONLEG", ManifestEntry{11002, 2, DataTypes::DOUBLE_T}},
            {"ADDMARKERLEG", ManifestEntry{11003, 2, DataTypes::DOUBLE_T}},
            {"ADDOBJECTLEG", ManifestEntry{11004, 2, DataTypes::DOUBLE_T}},
            {"CLEARWAYPOINTS", ManifestEntry{11005, 1, DataTypes::UINT8_T}},
            {"SETMAXSPEED", ManifestEntry{11006, 1, DataTypes::FLOAT_T}},
            {"SETLOGGINGLEVELS", ManifestEntry{11007, 3, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"CURRENTSTATE", ManifestEntry{11100, 1, DataTypes::UINT8_T}},
            {"REACHEDMARKER", ManifestEntry{11101, 1, DataTypes::UINT8_T}},
            {"CURRENTLOG", ManifestEntry{11102, 255, DataTypes::CHAR}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
        enum class AUTONOMYSTATE
        {
            IDLE,
            NAVIGATING,
            SEARCHPATTERN,
            APPROACHINGMARKER,
            APPROACHINGOBJECT,
            VERIFYINGMARKER,
            VERIFYINGOBJECT,
            AVOIDANCE,
            REVERSING,
            STUCK
        }; 

        inline uint8_t EnumToInteger(AUTONOMYSTATE state)
        {
            return static_cast<uint8_t>(state);
        } 

        enum class AUTONOMYLOG
        {
            TRACEL3,
            TRACEL2,
            TRACEL1,
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL
        }; 

        inline uint8_t EnumToInteger(AUTONOMYLOG state)
        {
            return static_cast<uint8_t>(state);
        } 

    }    // namespace Autonomy

    /******************************************************************************
     * @brief Camera1 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 100};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"CHANGECAMERAS", ManifestEntry{12000, 2, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"AVAILABLECAMERAS", ManifestEntry{12100, 1, DataTypes::UINT8_T}},
            {"STREAMINGCAMERAS", ManifestEntry{12101, 4, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"CAMERAUNAVAILABLE", ManifestEntry{12200, 1, DataTypes::UINT8_T}},
        };
    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace Camera2
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 4, 101};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Camera2

    /******************************************************************************
     * @brief RamanSpectrometer Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace RamanSpectrometer
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 102};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ENABLELEDS", ManifestEntry{13000, 1, DataTypes::UINT8_T}},
            {"REQUESTREADING", ManifestEntry{13001, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"CCDREADING_PART1", ManifestEntry{13100, 500, DataTypes::UINT16_T}},
            {"CCDREADING_PART2", ManifestEntry{13101, 500, DataTypes::UINT16_T}},
            {"CCDREADING_PART3", ManifestEntry{13102, 500, DataTypes::UINT16_T}},
            {"CCDREADING_PART4", ManifestEntry{13103, 500, DataTypes::UINT16_T}},
            {"CCDREADING_PART5", ManifestEntry{13104, 48, DataTypes::UINT16_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace RamanSpectrometer

    /******************************************************************************
     * @brief ReflectanceSpectrometer Board IP Address, Commands, Telemetry, and 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace ReflectanceSpectrometer
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 103};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ENABLELEDS", ManifestEntry{14000, 1, DataTypes::UINT8_T}},
            {"REQUESTREADING", ManifestEntry{14001, 1, DataTypes::UINT8_T}},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"READING", ManifestEntry{14100, 288, DataTypes::UINT8_T}},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace ReflectanceSpectrometer

    /******************************************************************************
     * @brief IRSpectrometer Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
     ******************************************************************************/
    namespace IRSpectrometer
    {
        // IP Address
        const AddressEntry IP_ADDRESS{192, 168, 3, 104};

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace IRSpectrometer

    /******************************************************************************
     * @brief RoveComm General Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-25
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
     * @date 2024-03-25
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
     * @date 2024-03-25
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
                case 1:    // BMS Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(BMS::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(BMS::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(BMS::ERROR, dataId);
                    }
                    break;
                case 2:    // Power Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Power::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Power::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Power::ERROR, dataId);
                    }
                    break;
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
                case 9:    // ScienceActuation Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(ScienceActuation::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(ScienceActuation::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(ScienceActuation::ERROR, dataId);
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
                case 13:    // RamanSpectrometer Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(RamanSpectrometer::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(RamanSpectrometer::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(RamanSpectrometer::ERROR, dataId);
                    }
                    break;
                case 14:    // ReflectanceSpectrometer Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(ReflectanceSpectrometer::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(ReflectanceSpectrometer::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(ReflectanceSpectrometer::ERROR, dataId);
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
