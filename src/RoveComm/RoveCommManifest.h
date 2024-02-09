/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file in
 *                the RoveComm_Base repository or edit the parser.py if the
 *                error is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2023-10-21
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_MANIFEST_H
#define ROVECOMM_MANIFEST_H

#include <map>
#include <stdint.h>
#include <string>

namespace manifest
{
    /******************************************************************************
     * @brief Enumeration of Data Types to be used in RoveComm
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    enum DataTypes : uint8_t
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
     * @date 2023-10-21
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
     * @date 2023-10-21
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
     * @date 2023-10-21
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
     * @date 2023-10-21
     ******************************************************************************/
    namespace BMS
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"BMSSTOP", ManifestEntry(2000, 1, DataTypes::UINT8_T)}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"PACKI_MEAS", ManifestEntry(2100, 1, DataTypes::FLOAT_T)},
                                                                {"PACKV_MEAS", ManifestEntry(2101, 1, DataTypes::FLOAT_T)},
                                                                {"CELLV_MEAS", ManifestEntry(2102, 8, DataTypes::FLOAT_T)},
                                                                {"TEMP_MEAS", ManifestEntry(2103, 1, DataTypes::FLOAT_T)}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"PACKOVERCURRENT", ManifestEntry(2200, 1, DataTypes::UINT8_T)},
                                                            {"CELLUNDERVOLTAGE", ManifestEntry(2201, 1, DataTypes::UINT8_T)},
                                                            {"PACKUNDERVOLTAGE", ManifestEntry(2202, 1, DataTypes::UINT8_T)},
                                                            {"PACKSUPERHOT", ManifestEntry(2203, 1, DataTypes::UINT8_T)}};
    }    // namespace BMS

    /******************************************************************************
     * @brief Power Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace Power
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 101);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"MOTORBUSENABLE", ManifestEntry(3000, 1, DataTypes::UINT8_T)},
                                                               {"HIGHBUSENABLE", ManifestEntry(3001, 1, DataTypes::UINT8_T)},
                                                               {"LOWBUSENABLE", ManifestEntry(3002, 1, DataTypes::UINT8_T)},
                                                               {"TWELVEVBUSENABLE", ManifestEntry(3003, 1, DataTypes::UINT8_T)}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"MOTORBUSENABLED", ManifestEntry(3100, 1, DataTypes::UINT8_T)},
                                                                {"HIGHBUSENABLED", ManifestEntry(3101, 1, DataTypes::UINT8_T)},
                                                                {"LOWBUSENABLED", ManifestEntry(3102, 1, DataTypes::UINT8_T)},
                                                                {"TWELVEVBUSENABLED", ManifestEntry(3103, 1, DataTypes::UINT8_T)},
                                                                {"MOTORBUSCURRENT", ManifestEntry(3104, 7, DataTypes::FLOAT_T)},
                                                                {"HIGHBUSCURRENT", ManifestEntry(3105, 2, DataTypes::FLOAT_T)},
                                                                {"LOWBUSCURRENT", ManifestEntry(3106, 5, DataTypes::FLOAT_T)},
                                                                {"TWELVEVBUSCURRENT", ManifestEntry(3107, 4, DataTypes::FLOAT_T)}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"MOTORBUSOVERCURRENT", ManifestEntry(3200, 1, DataTypes::UINT8_T)},
                                                            {"HIGHBUSOVERCURRENT", ManifestEntry(3201, 1, DataTypes::UINT8_T)},
                                                            {"LOWBUSOVERCURRENT", ManifestEntry(3202, 1, DataTypes::UINT8_T)}};

    }    // namespace Power

    /******************************************************************************
     * @brief Blackbox Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace Blackbox
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 102);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"BLACKBOXLISTENING", ManifestEntry(4100, 1, DataTypes::UINT8_T)}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};

    }    // namespace Blackbox

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace Nav
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 104);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"GPSLATLON", ManifestEntry(5100, 2, DataTypes::DOUBLE_T)},
                                                                {"IMUDATA", ManifestEntry(5101, 3, DataTypes::FLOAT_T)},
                                                                {"COMPASSDATA", ManifestEntry(5102, 1, DataTypes::FLOAT_T)},
                                                                {"SATELLITECOUNTDATA", ManifestEntry(5103, 1, DataTypes::UINT8_T)},
                                                                {"ACCELEROMETERDATA", ManifestEntry(5104, 3, DataTypes::FLOAT_T)},
                                                                {"ACCURACYDATA", ManifestEntry(5105, 3, DataTypes::FLOAT_T)}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"GPSLOCKERROR", ManifestEntry(5200, 1, DataTypes::UINT8_T)}};

    }    // namespace Nav

    /******************************************************************************
     * @brief Gimbal Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
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
     * @date 2023-10-21
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
     * @date 2023-10-21
     ******************************************************************************/
    namespace Arm
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 107);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"ARMVELOCITYCONTROL", {8000, 6, DataTypes::INT16_T}},
                                                               {"ARMMOVETOPOSITION", {8001, 6, DataTypes::FLOAT_T}},
                                                               {"ARMINCREMENTPOSITION", {8002, 6, DataTypes::FLOAT_T}},
                                                               {"ARMMOVEIK", {8003, 6, DataTypes::FLOAT_T}},
                                                               {"ARMINCREMENTIKROVER", {8004, 6, DataTypes::FLOAT_T}},
                                                               {"ARMINCREMENTIKWRIST", {8005, 6, DataTypes::FLOAT_T}},
                                                               {"LASERS", {8006, 1, DataTypes::UINT8_T}},
                                                               {"ENDEFFECTOR", {8007, 1, DataTypes::UINT8_T}},
                                                               {"GRIPPERMOVE", {8008, 2, DataTypes::INT16_T}},
                                                               {"WATCHDOGOVERRIDE", {8009, 1, DataTypes::UINT8_T}},
                                                               {"LIMITSWITCHOVERRIDE", {8010, 1, DataTypes::UINT16_T}},
                                                               {"REQUESTJOINTPOSITIONS", {8011, 1, DataTypes::UINT8_T}},
                                                               {"TOGGLEPOSITIONTELEM", {8012, 1, DataTypes::UINT8_T}},
                                                               {"REQUESTAXESPOSITIONS", {8013, 1, DataTypes::UINT8_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"JOINTANGLES", {8101, 6, DataTypes::FLOAT_T}},
                                                                {"MOTORVELOCITIES", {8102, 6, DataTypes::FLOAT_T}},
                                                                {"IKCOORDINATES", {8103, 6, DataTypes::FLOAT_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"WATCHDOGSTATUS", {8200, 1, DataTypes::UINT8_T}}};

    }    // namespace Arm

    /******************************************************************************
     * @brief ScienceActuation Board IP Address, Commands, Telemetry, and Error
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace ScienceActuation
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 108);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"SENSORAXIS", {9000, 1, DataTypes::INT16_T}},
                                                               {"WATERSELECTOR", {9001, 1, DataTypes::INT8_T}},
                                                               {"WATERPUMP", {9002, 1, DataTypes::UINT8_T}},
                                                               {"LIMITSWITCHOVERRIDE", {9003, 1, DataTypes::UINT8_T}},
                                                               {"XOOPAXIS", {9004, 1, DataTypes::INT16_T}},
                                                               {"ZOOPAXIS", {9005, 1, DataTypes::INT16_T}},
                                                               {"SCOOPGRABBER", {9006, 1, DataTypes::UINT8_T}},
                                                               {"GOTOPOSITION", {9007, 1, DataTypes::UINT8_T}},
                                                               {"INCREMENTALSCOOP", {9008, 1, DataTypes::INT8_T}},
                                                               {"BUMPSCOOP", {9009, 1, DataTypes::UINT8_T}},
                                                               {"MICROSCOPEFOCUS", {9010, 1, DataTypes::INT16_T}},
                                                               {"WATERPOSITION", {9011, 1, DataTypes::UINT8_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"ENCODERPOSITIONS", {9100, 3, DataTypes::FLOAT_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"LIMITSWITCHTRIGGERED", {9200, 1, DataTypes::UINT8_T}}};

    }    // namespace ScienceActuation

    /******************************************************************************
     * @brief ScienceSensors Board IP Address, Commands, Telemetry, and Error
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace ScienceSensors
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 101);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"FLUOROMETERLEDS", {10000, 1, DataTypes::UINT8_T}}, {"REQFLUOROMETER", {10001, 1, DataTypes::UINT8_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"FLUOROMETERDATA", {10100, 215, DataTypes::UINT16_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};

    }    // namespace ScienceSensors

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace Autonomy
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"STARTAUTONOMY", {11000, 1, DataTypes::UINT8_T}},
                                                               {"DISABLEAUTONOMY", {11001, 1, DataTypes::UINT8_T}},
                                                               {"ADDPOSITIONLEG", {11002, 2, DataTypes::DOUBLE_T}},
                                                               {"ADDMARKERLEG", {11003, 2, DataTypes::DOUBLE_T}},
                                                               {"ADDGATELEG", {11004, 2, DataTypes::DOUBLE_T}},
                                                               {"CLEARWAYPOINTS", {11005, 1, DataTypes::UINT8_T}},
                                                               {"SETMAXSPEED", {11006, 1, DataTypes::UINT16_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"CURRENTSTATE", {11100, 1, DataTypes::UINT8_T}},
                                                                {"REACHEDMARKER", {11101, 1, DataTypes::UINT8_T}},
                                                                {"CURRENTLOG", {11102, 255, DataTypes::CHAR}}};

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
     * @date 2023-10-21
     ******************************************************************************/
    namespace Camera1
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 4, 100);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"CHANGECAMERAS", {12000, 2, DataTypes::UINT8_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"AVAILABLECAMERAS", {12100, 1, DataTypes::UINT8_T}},
                                                                {"STREAMINGCAMERAS", {12101, 4, DataTypes::UINT8_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"CAMERAUNAVAILABLE", {12200, 1, DataTypes::UINT8_T}}};

    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
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
     * @date 2023-10-21
     ******************************************************************************/
    namespace Heater
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 109);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"HEATERTOGGLE", {15000, 1, DataTypes::UINT16_T}}, {"HEATERSETTEMP", {15001, 12, DataTypes::FLOAT_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"THERMOVALUES", {15100, 12, DataTypes::FLOAT_T}}, {"HEATERENABLED", {15101, 1, DataTypes::UINT16_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {{"OVERHEAT", {15200, 1, DataTypes::UINT16_T}}};

    }    // namespace Heater

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace SignalStack
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 3, 102);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"SIGNALSROTATE", {16000, 1, DataTypes::INT16_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"SIGNALSPOSITION", {16100, 2, DataTypes::DOUBLE_T}},
                                                                {"SIGNALSDIRECTION", {16101, 1, DataTypes::FLOAT_T}}};

        // Error
        const std::map<std::string, ManifestEntry> ERROR = {};

    }    // namespace SignalStack

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace Core
    {
        // IP Address
        const AddressEntry IP_ADDRESS(192, 168, 2, 110);

        // Commands
        const std::map<std::string, ManifestEntry> COMMANDS = {{"DRIVELEFTRIGHT", {17000, 2, DataTypes::FLOAT_T}},
                                                               {"DRIVEINDIVIDUAL", {17001, 6, DataTypes::FLOAT_T}},
                                                               {"WATCHDOGOVERRIDE", {17002, 1, DataTypes::UINT8_T}},
                                                               {"LEFTDRIVEGIMBALINCREMENT", {17003, 1, DataTypes::INT16_T}},
                                                               {"RIGHTDRIVEGIMBALINCREMENT", {17004, 1, DataTypes::INT16_T}},
                                                               {"LEFTMAINGIMBALINCREMENT", {17005, 2, DataTypes::INT16_T}},
                                                               {"RIGHTMAINGIMBALINCREMENT", {17006, 2, DataTypes::INT16_T}},
                                                               {"BACKDRIVEGIMBALINCREMENT", {17007, 1, DataTypes::INT16_T}},
                                                               {"INITIATETESTROUTINE", {17008, 1, DataTypes::UINT8_T}},
                                                               {"LEDRGB", {17009, 3, DataTypes::UINT8_T}},
                                                               {"LEDPATTERNS", {17010, 1, DataTypes::UINT8_T}},
                                                               {"STATEDISPLAY", {17011, 1, DataTypes::UINT8_T}},
                                                               {"BRIGHTNESS", {17012, 1, DataTypes::UINT8_T}},
                                                               {"SETWATCHDOGMODE", {17013, 1, DataTypes::UINT8_T}}};

        // Telemetry
        const std::map<std::string, ManifestEntry> TELEMETRY = {{"DRIVESPEEDS", {17100, 6, DataTypes::INT16_T}}};

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
     * @date 2023-10-21
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
     * @date 2023-10-21
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

    namespace Helpers
    {
        inline DataTypes getDataTypeFromMap(const std::map<std::string, ManifestEntry>& dataMap, uint16_t dataId)
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

        inline DataTypes getDataTypeFromId(uint16_t dataId)
        {
            int boardId      = dataId / 1000;          // Determine board ID based on thousands place
            int dataTypeCode = (dataId / 100) % 10;    // Determine data type code based on hundreds place

            // Determine the board namespace based on boardId
            switch (boardId)
            {
                case 1:    // Drive Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Drive::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Drive::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Drive::ERROR, dataId);
                    }
                    break;
                case 2:    // BMS Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(BMS::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(BMS::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(BMS::ERROR, dataId);
                    }
                    break;
                case 3:    // Power Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Power::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Power::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Power::ERROR, dataId);
                    }
                    break;
                case 4:    // Blackbox Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Blackbox::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Blackbox::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Blackbox::ERROR, dataId);
                    }
                    break;
                case 5:    // Nav Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Nav::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Nav::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Nav::ERROR, dataId);
                    }
                    break;
                case 6:    // Gimbal Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Gimbal::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Gimbal::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Gimbal::ERROR, dataId);
                    }
                    break;
                case 7:    // Multimedia Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Multimedia::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Multimedia::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Multimedia::ERROR, dataId);
                    }
                    break;
                case 8:    // Arm Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Arm::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Arm::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Arm::ERROR, dataId);
                    }
                    break;
                case 9:    // ScienceActuation Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(ScienceActuation::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(ScienceActuation::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(ScienceActuation::ERROR, dataId);
                    }
                    break;
                case 10:    // ScienceSensors Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(ScienceSensors::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(ScienceSensors::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(ScienceSensors::ERROR, dataId);
                    }
                    break;
                case 11:    // Autonomy Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Autonomy::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Autonomy::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Autonomy::ERROR, dataId);
                    }
                    break;
                case 12:    // Camera1 Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Camera1::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Camera1::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Camera1::ERROR, dataId);
                    }
                    break;
                case 13:    // Camera2 Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Camera2::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Camera2::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Camera2::ERROR, dataId);
                    }
                    break;
                case 14:    // Reserved for future use
                    break;
                case 15:    // Heater Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Heater::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Heater::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Heater::ERROR, dataId);
                    }
                    break;
                case 16:    // SignalStack Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(SignalStack::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(SignalStack::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(SignalStack::ERROR, dataId);
                    }
                    break;
                case 17:    // Core Board
                    if (dataTypeCode == 0)
                    {
                        return getDataTypeFromMap(Core::COMMANDS, dataId);
                    }
                    else if (dataTypeCode == 1)
                    {
                        return getDataTypeFromMap(Core::TELEMETRY, dataId);
                    }
                    else if (dataTypeCode == 2)
                    {
                        return getDataTypeFromMap(Core::ERROR, dataId);
                    }
                    break;
                default:
                    // Invalid board ID
                    break;
            }

            // If dataId is not found in any namespace, return a default type
            return DataTypes::CHAR;
        }
    }    // namespace Helpers

}    // namespace manifest

#endif    // ROVECOMM_MANIFEST_H
