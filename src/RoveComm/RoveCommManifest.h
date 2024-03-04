/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2024-03-04
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
     * @date 2024-03-04
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
     * @date 2024-03-04
     ******************************************************************************/
    struct AddressEntry
    {
        public:
            int FIRST_OCTET;
            int SECOND_OCTET;
            int THIRD_OCTET;
            int FOURTH_OCTET;
    };

    /******************************************************************************
     * @brief Manifest Entry Object for RoveComm.
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    struct ManifestEntry
    {
        public:
            int DATA_ID;
            int DATA_COUNT;
            DataTypes DATA_TYPE;
    };

    /******************************************************************************
     * @brief Drive Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Drive
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 103);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Drive

    /******************************************************************************
     * @brief BMS Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace BMS
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"BMSSTOP", ManifestEntry(2000, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"PACKI_MEAS", ManifestEntry(2100, 1, DataTypes::FLOAT_T)},
            {"PACKV_MEAS", ManifestEntry(2101, 1, DataTypes::FLOAT_T)},
            {"CELLV_MEAS", ManifestEntry(2102, 8, DataTypes::FLOAT_T)},
            {"TEMP_MEAS", ManifestEntry(2103, 1, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"PACKOVERCURRENT", ManifestEntry(2200, 1, DataTypes::UINT8_T)},
            {"CELLUNDERVOLTAGE", ManifestEntry(2201, 1, DataTypes::UINT8_T)},
            {"PACKUNDERVOLTAGE", ManifestEntry(2202, 1, DataTypes::UINT8_T)},
            {"PACKSUPERHOT", ManifestEntry(2203, 1, DataTypes::UINT8_T)},
        };
    }    // namespace BMS

    /******************************************************************************
     * @brief Power Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Power
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 101);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"MOTORBUSENABLE", ManifestEntry(3000, 1, DataTypes::UINT8_T)},
            {"HIGHBUSENABLE", ManifestEntry(3001, 1, DataTypes::UINT8_T)},
            {"LOWBUSENABLE", ManifestEntry(3002, 1, DataTypes::UINT8_T)},
            {"TWELVEVBUSENABLE", ManifestEntry(3003, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"MOTORBUSENABLED", ManifestEntry(3100, 1, DataTypes::UINT8_T)},
            {"HIGHBUSENABLED", ManifestEntry(3101, 1, DataTypes::UINT8_T)},
            {"LOWBUSENABLED", ManifestEntry(3102, 1, DataTypes::UINT8_T)},
            {"TWELVEVBUSENABLED", ManifestEntry(3103, 1, DataTypes::UINT8_T)},
            {"MOTORBUSCURRENT", ManifestEntry(3104, 7, DataTypes::FLOAT_T)},
            {"HIGHBUSCURRENT", ManifestEntry(3105, 2, DataTypes::FLOAT_T)},
            {"LOWBUSCURRENT", ManifestEntry(3106, 5, DataTypes::FLOAT_T)},
            {"TWELVEVBUSCURRENT", ManifestEntry(3107, 4, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"MOTORBUSOVERCURRENT", ManifestEntry(3200, 1, DataTypes::UINT8_T)},
            {"HIGHBUSOVERCURRENT", ManifestEntry(3201, 1, DataTypes::UINT8_T)},
            {"LOWBUSOVERCURRENT", ManifestEntry(3202, 1, DataTypes::UINT8_T)},
        };
    }    // namespace Power

    /******************************************************************************
     * @brief Blackbox Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Blackbox
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 102);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"BLACKBOXLISTENING", ManifestEntry(4100, 1, DataTypes::UINT8_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Blackbox

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 104);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"GPSLATLON", ManifestEntry(5100, 2, DataTypes::DOUBLE_T)},
            {"IMUDATA", ManifestEntry(5101, 3, DataTypes::FLOAT_T)},
            {"COMPASSDATA", ManifestEntry(5102, 1, DataTypes::FLOAT_T)},
            {"SATELLITECOUNTDATA", ManifestEntry(5103, 1, DataTypes::UINT8_T)},
            {"ACCELEROMETERDATA", ManifestEntry(5104, 3, DataTypes::FLOAT_T)},
            {"ACCURACYDATA", ManifestEntry(5105, 3, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"GPSLOCKERROR", ManifestEntry(5200, 1, DataTypes::UINT8_T)},
        };
    }    // namespace Nav

    /******************************************************************************
     * @brief Gimbal Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Gimbal
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 106);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Gimbal

    /******************************************************************************
     * @brief Multimedia Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Multimedia
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 105);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
    }    // namespace Multimedia

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 107);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"ARMVELOCITYCONTROL", ManifestEntry(8000, 6, DataTypes::INT16_T)},
            {"ARMMOVETOPOSITION", ManifestEntry(8001, 6, DataTypes::FLOAT_T)},
            {"ARMINCREMENTPOSITION", ManifestEntry(8002, 6, DataTypes::FLOAT_T)},
            {"ARMMOVEIK", ManifestEntry(8003, 6, DataTypes::FLOAT_T)},
            {"ARMINCREMENTIKROVER", ManifestEntry(8004, 6, DataTypes::FLOAT_T)},
            {"ARMINCREMENTIKWRIST", ManifestEntry(8005, 6, DataTypes::FLOAT_T)},
            {"LASERS", ManifestEntry(8006, 1, DataTypes::UINT8_T)},
            {"ENDEFFECTOR", ManifestEntry(8007, 1, DataTypes::UINT8_T)},
            {"GRIPPERMOVE", ManifestEntry(8008, 2, DataTypes::INT16_T)},
            {"WATCHDOGOVERRIDE", ManifestEntry(8009, 1, DataTypes::UINT8_T)},
            {"LIMITSWITCHOVERRIDE", ManifestEntry(8010, 1, DataTypes::UINT16_T)},
            {"REQUESTJOINTPOSITIONS", ManifestEntry(8011, 1, DataTypes::UINT8_T)},
            {"TOGGLEPOSITIONTELEM", ManifestEntry(8012, 1, DataTypes::UINT8_T)},
            {"REQUESTAXESPOSITIONS", ManifestEntry(8013, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"JOINTANGLES", ManifestEntry(8101, 6, DataTypes::FLOAT_T)},
            {"MOTORVELOCITIES", ManifestEntry(8102, 6, DataTypes::FLOAT_T)},
            {"IKCOORDINATES", ManifestEntry(8103, 6, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"WATCHDOGSTATUS", ManifestEntry(8200, 1, DataTypes::UINT8_T)},
        };
    }    // namespace Arm

    /******************************************************************************
     * @brief ScienceActuation Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace ScienceActuation
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 108);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"SENSORAXIS", ManifestEntry(9000, 1, DataTypes::INT16_T)},
            {"WATERSELECTOR", ManifestEntry(9001, 1, DataTypes::INT8_T)},
            {"WATERPUMP", ManifestEntry(9002, 1, DataTypes::UINT8_T)},
            {"LIMITSWITCHOVERRIDE", ManifestEntry(9003, 1, DataTypes::UINT8_T)},
            {"XOOPAXIS", ManifestEntry(9004, 1, DataTypes::INT16_T)},
            {"ZOOPAXIS", ManifestEntry(9005, 1, DataTypes::INT16_T)},
            {"SCOOPGRABBER", ManifestEntry(9006, 1, DataTypes::UINT8_T)},
            {"GOTOPOSITION", ManifestEntry(9007, 1, DataTypes::UINT8_T)},
            {"INCREMENTALSCOOP", ManifestEntry(9008, 1, DataTypes::INT8_T)},
            {"BUMPSCOOP", ManifestEntry(9009, 1, DataTypes::UINT8_T)},
            {"MICROSCOPEFOCUS", ManifestEntry(9010, 1, DataTypes::INT16_T)},
            {"WATERPOSITION", ManifestEntry(9011, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"ENCODERPOSITIONS", ManifestEntry(9100, 3, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"LIMITSWITCHTRIGGERED", ManifestEntry(9200, 1, DataTypes::UINT8_T)},
        };
    }    // namespace ScienceActuation

    /******************************************************************************
     * @brief ScienceSensors Board IP Address, Commands, Telemetry, and Error 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace ScienceSensors
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 101);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"FLUOROMETERLEDS", ManifestEntry(10000, 1, DataTypes::UINT8_T)},
            {"REQFLUOROMETER", ManifestEntry(10001, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"FLUOROMETERDATA", ManifestEntry(10100, 215, DataTypes::UINT16_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace ScienceSensors

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"STARTAUTONOMY", ManifestEntry(11000, 1, DataTypes::UINT8_T)},
            {"DISABLEAUTONOMY", ManifestEntry(11001, 1, DataTypes::UINT8_T)},
            {"ADDPOSITIONLEG", ManifestEntry(11002, 2, DataTypes::DOUBLE_T)},
            {"ADDMARKERLEG", ManifestEntry(11003, 2, DataTypes::DOUBLE_T)},
            {"ADDGATELEG", ManifestEntry(11004, 2, DataTypes::DOUBLE_T)},
            {"CLEARWAYPOINTS", ManifestEntry(11005, 1, DataTypes::UINT8_T)},
            {"SETMAXSPEED", ManifestEntry(11006, 1, DataTypes::UINT16_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"CURRENTSTATE", ManifestEntry(11100, 1, DataTypes::UINT8_T)},
            {"REACHEDMARKER", ManifestEntry(11101, 1, DataTypes::UINT8_T)},
            {"CURRENTLOG", ManifestEntry(11102, 255, DataTypes::CHAR)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
        enum AUTONOMYSTATE
        {
            IDLE,
            NAVIGATING,
            SEARCHPATTERN,
            APPROACHINGMARKER,
            APPROACHINGGATE,
            AVOIDANCE,
            REVERSING,
            STUCK
        }; 

    }    // namespace Autonomy

    /******************************************************************************
     * @brief Camera1 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 4, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"CHANGECAMERAS", ManifestEntry(12000, 2, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"AVAILABLECAMERAS", ManifestEntry(12100, 1, DataTypes::UINT8_T)},
            {"STREAMINGCAMERAS", ManifestEntry(12101, 4, DataTypes::UINT8_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"CAMERAUNAVAILABLE", ManifestEntry(12200, 1, DataTypes::UINT8_T)},
        };
    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Camera2
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 4, 101);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};
        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {};
        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace Camera2

    /******************************************************************************
     * @brief Heater Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Heater
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 109);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"HEATERTOGGLE", ManifestEntry(15000, 1, DataTypes::UINT16_T)},
            {"HEATERSETTEMP", ManifestEntry(15001, 12, DataTypes::FLOAT_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"THERMOVALUES", ManifestEntry(15100, 12, DataTypes::FLOAT_T)},
            {"HEATERENABLED", ManifestEntry(15101, 1, DataTypes::UINT16_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {
            {"OVERHEAT", ManifestEntry(15200, 1, DataTypes::UINT16_T)},
        };
    }    // namespace Heater

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace SignalStack
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 102);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"SIGNALSROTATE", ManifestEntry(16000, 1, DataTypes::INT16_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"SIGNALSPOSITION", ManifestEntry(16100, 2, DataTypes::DOUBLE_T)},
            {"SIGNALSDIRECTION", ManifestEntry(16101, 1, DataTypes::FLOAT_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
    }    // namespace SignalStack

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet 
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
     ******************************************************************************/
    namespace Core
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 110);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {
            {"DRIVELEFTRIGHT", ManifestEntry(17000, 2, DataTypes::FLOAT_T)},
            {"DRIVEINDIVIDUAL", ManifestEntry(17001, 6, DataTypes::FLOAT_T)},
            {"WATCHDOGOVERRIDE", ManifestEntry(17002, 1, DataTypes::UINT8_T)},
            {"LEFTDRIVEGIMBALINCREMENT", ManifestEntry(17003, 1, DataTypes::INT16_T)},
            {"RIGHTDRIVEGIMBALINCREMENT", ManifestEntry(17004, 1, DataTypes::INT16_T)},
            {"LEFTMAINGIMBALINCREMENT", ManifestEntry(17005, 2, DataTypes::INT16_T)},
            {"RIGHTMAINGIMBALINCREMENT", ManifestEntry(17006, 2, DataTypes::INT16_T)},
            {"BACKDRIVEGIMBALINCREMENT", ManifestEntry(17007, 1, DataTypes::INT16_T)},
            {"INITIATETESTROUTINE", ManifestEntry(17008, 1, DataTypes::UINT8_T)},
            {"LEDRGB", ManifestEntry(17009, 3, DataTypes::UINT8_T)},
            {"LEDPATTERNS", ManifestEntry(17010, 1, DataTypes::UINT8_T)},
            {"STATEDISPLAY", ManifestEntry(17011, 1, DataTypes::UINT8_T)},
            {"BRIGHTNESS", ManifestEntry(17012, 1, DataTypes::UINT8_T)},
            {"SETWATCHDOGMODE", ManifestEntry(17013, 1, DataTypes::UINT8_T)},
        };

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {
            {"DRIVESPEEDS", ManifestEntry(17100, 6, DataTypes::INT16_T)},
        };

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};
        
        // Enums
        enum DISPLAYSTATE
        {
            TELEOP,
            AUTONOMY,
            REACHED_GOAL
        }; 

        enum PATTERNS
        {
            MRDT,
            BELGIUM,
            MERICA,
            DIRT,
            DOTA,
            MCD,
            WINDOWS
        }; 

    }    // namespace Core

    /******************************************************************************
     * @brief RoveComm General Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2024-03-04
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
     * @date 2024-03-04
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
     * @date 2024-03-04
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
                case 2:    // BMS Board
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
                case 3:    // Power Board
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
                case 10:    // ScienceSensors Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(ScienceSensors::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(ScienceSensors::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(ScienceSensors::ERROR, dataId);
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
                case 15:    // Heater Board
                    if (dataTypeCode == 0)
                    {
                        return GetDataTypeFromMap(Heater::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return GetDataTypeFromMap(Heater::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return GetDataTypeFromMap(Heater::ERROR, dataId);
                    }
                    break;
                case 16:    // SignalStack Board
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
                case 17:    // Core Board
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
