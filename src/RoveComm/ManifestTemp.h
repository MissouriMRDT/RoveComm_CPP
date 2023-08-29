/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file Manifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2023-08-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef MANIFEST_H
#define MANIFEST_H

#include <stdint.h>

namespace manifest
{
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

    struct ManifestEntry
    {
            int DATA_ID;
            int DATA_COUNT;
            DataTypes DATA_TYPE;
    };

#define RC_DRIVEBOARD_FIRSTOCTET  192
#define RC_DRIVEBOARD_SECONDOCTET 168
#define RC_DRIVEBOARD_THIRDOCTET  2
#define RC_DRIVEBOARD_FOURTHOCTET 103

#define RC_BMSBOARD_FIRSTOCTET  192
#define RC_BMSBOARD_SECONDOCTET 168
#define RC_BMSBOARD_THIRDOCTET  2
#define RC_BMSBOARD_FOURTHOCTET 100

#define RC_POWERBOARD_FIRSTOCTET  192
#define RC_POWERBOARD_SECONDOCTET 168
#define RC_POWERBOARD_THIRDOCTET  2
#define RC_POWERBOARD_FOURTHOCTET 101

#define RC_BLACKBOXBOARD_FIRSTOCTET  192
#define RC_BLACKBOXBOARD_SECONDOCTET 168
#define RC_BLACKBOXBOARD_THIRDOCTET  2
#define RC_BLACKBOXBOARD_FOURTHOCTET 102

#define RC_NAVBOARD_FIRSTOCTET  192
#define RC_NAVBOARD_SECONDOCTET 168
#define RC_NAVBOARD_THIRDOCTET  2
#define RC_NAVBOARD_FOURTHOCTET 104

#define RC_GIMBALBOARD_FIRSTOCTET  192
#define RC_GIMBALBOARD_SECONDOCTET 168
#define RC_GIMBALBOARD_THIRDOCTET  2
#define RC_GIMBALBOARD_FOURTHOCTET 106

#define RC_MULTIMEDIABOARD_FIRSTOCTET  192
#define RC_MULTIMEDIABOARD_SECONDOCTET 168
#define RC_MULTIMEDIABOARD_THIRDOCTET  2
#define RC_MULTIMEDIABOARD_FOURTHOCTET 105

#define RC_ARMBOARD_FIRSTOCTET  192
#define RC_ARMBOARD_SECONDOCTET 168
#define RC_ARMBOARD_THIRDOCTET  2
#define RC_ARMBOARD_FOURTHOCTET 107

#define RC_SCIENCEACTUATIONBOARD_FIRSTOCTET  192
#define RC_SCIENCEACTUATIONBOARD_SECONDOCTET 168
#define RC_SCIENCEACTUATIONBOARD_THIRDOCTET  2
#define RC_SCIENCEACTUATIONBOARD_FOURTHOCTET 108

#define RC_SCIENCESENSORSBOARD_FIRSTOCTET  192
#define RC_SCIENCESENSORSBOARD_SECONDOCTET 168
#define RC_SCIENCESENSORSBOARD_THIRDOCTET  3
#define RC_SCIENCESENSORSBOARD_FOURTHOCTET 101

#define RC_AUTONOMYBOARD_FIRSTOCTET  192
#define RC_AUTONOMYBOARD_SECONDOCTET 168
#define RC_AUTONOMYBOARD_THIRDOCTET  3
#define RC_AUTONOMYBOARD_FOURTHOCTET 100

#define RC_CAMERA1BOARD_FIRSTOCTET  192
#define RC_CAMERA1BOARD_SECONDOCTET 168
#define RC_CAMERA1BOARD_THIRDOCTET  4
#define RC_CAMERA1BOARD_FOURTHOCTET 100

#define RC_CAMERA2BOARD_FIRSTOCTET  192
#define RC_CAMERA2BOARD_SECONDOCTET 168
#define RC_CAMERA2BOARD_THIRDOCTET  4
#define RC_CAMERA2BOARD_FOURTHOCTET 101

#define RC_HEATERBOARD_FIRSTOCTET  192
#define RC_HEATERBOARD_SECONDOCTET 168
#define RC_HEATERBOARD_THIRDOCTET  2
#define RC_HEATERBOARD_FOURTHOCTET 109

#define RC_SIGNALSTACKBOARD_FIRSTOCTET  192
#define RC_SIGNALSTACKBOARD_SECONDOCTET 168
#define RC_SIGNALSTACKBOARD_THIRDOCTET  3
#define RC_SIGNALSTACKBOARD_FOURTHOCTET 102

#define RC_COREBOARD_FIRSTOCTET  192
#define RC_COREBOARD_SECONDOCTET 168
#define RC_COREBOARD_THIRDOCTET  2
#define RC_COREBOARD_FOURTHOCTET 110

#define ROVECOMM_UPDATE_RATE          100
#define RC_ROVECOMM_ETHERNET_UDP_PORT 11000
#define RC_ROVECOMM_ETHERNET_TCP_PORT 12000

#define RC_ROVECOMM_SUBNET_MAC_FIRST_BYTE  222
#define RC_ROVECOMM_SUBNET_MAC_SECOND_BYTE 173

    ///////////////////////////////////////////////////
    ////////////        System Packets      ///////////
    ///////////////////////////////////////////////////

#define RC_ROVECOMM_PING_DATA_ID            1
#define RC_ROVECOMM_PING_REPLY_DATA_ID      2
#define RC_ROVECOMM_SUBSCRIBE_DATA_ID       3
#define RC_ROVECOMM_UNSUBSCRIBE_DATA_ID     4
#define RC_ROVECOMM_INVALID_VERSION_DATA_ID 5
#define RC_ROVECOMM_NO_DATA_DATA_ID         6

    /******************************************************************************
     * @brief Drive Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Drive
    {}    // namespace Drive

    /******************************************************************************
     * @brief BMS Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace BMS
    {
        // Commands
        const ManifestEntry BMSSTOP(2000, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry PACKI_MEAS(2100, 1, DataTypes::FLOAT_T);
        const ManifestEntry PACKV_MEAS(2101, 1, DataTypes::FLOAT_T);
        const ManifestEntry CELLV_MEAS(2102, 8, DataTypes::FLOAT_T);
        const ManifestEntry TEMP_MEAS(2103, 1, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry PACKOVERCURRENT(2200, 1, DataTypes::UINT8_T);
        const ManifestEntry CELLUNDERVOLTAGE(2201, 1, DataTypes::UINT8_T);
        const ManifestEntry PACKUNDERVOLTAGE(2202, 1, DataTypes::UINT8_T);
        const ManifestEntry PACKSUPERHOT(2203, 1, DataTypes::UINT8_T);

    }    // namespace BMS

    /******************************************************************************
     * @brief Power Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Power
    {
        // Commands
        const ManifestEntry MOTORBUSENABLE(3000, 1, DataTypes::UINT8_T);
        const ManifestEntry HIGHBUSENABLE(3001, 1, DataTypes::UINT8_T);
        const ManifestEntry LOWBUSENABLE(3002, 1, DataTypes::UINT8_T);
        const ManifestEntry TWELVEVBUSENABLE(3003, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry MOTORBUSENABLED(3100, 1, DataTypes::UINT8_T);
        const ManifestEntry HIGHBUSENABLED(3101, 1, DataTypes::UINT8_T);
        const ManifestEntry LOWBUSENABLED(3102, 1, DataTypes::UINT8_T);
        const ManifestEntry TWELVEVBUSENABLED(3103, 1, DataTypes::UINT8_T);
        const ManifestEntry MOTORBUSCURRENT(3104, 7, DataTypes::FLOAT_T);
        const ManifestEntry HIGHBUSCURRENT(3105, 2, DataTypes::FLOAT_T);
        const ManifestEntry LOWBUSCURRENT(3106, 5, DataTypes::FLOAT_T);
        const ManifestEntry TWELVEVBUSCURRENT(3107, 4, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry MOTORBUSOVERCURRENT(3200, 1, DataTypes::UINT8_T);
        const ManifestEntry HIGHBUSOVERCURRENT(3201, 1, DataTypes::UINT8_T);
        const ManifestEntry LOWBUSOVERCURRENT(3202, 1, DataTypes::UINT8_T);

    }    // namespace Power

    /******************************************************************************
     * @brief Blackbox Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Blackbox
    {
        // Telemetry
        const ManifestEntry BLACKBOXLISTENING(4100, 1, DataTypes::UINT8_T);

    }    // namespace Blackbox

    /******************************************************************************
     * @brief Nav Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Nav
    {
        // Telemetry
        const ManifestEntry GPSLATLON(5100, 2, DataTypes::DOUBLE_T);
        const ManifestEntry IMUDATA(5101, 3, DataTypes::FLOAT_T);
        const ManifestEntry COMPASSDATA(5102, 1, DataTypes::FLOAT_T);
        const ManifestEntry SATELLITECOUNTDATA(5103, 1, DataTypes::UINT8_T);
        const ManifestEntry ACCELEROMETERDATA(5104, 3, DataTypes::FLOAT_T);
        const ManifestEntry ACCURACYDATA(5105, 3, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry GPSLOCKERROR(5200, 1, DataTypes::UINT8_T);

    }    // namespace Nav

    /******************************************************************************
     * @brief Gimbal Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Gimbal
    {}    // namespace Gimbal

    /******************************************************************************
     * @brief Multimedia Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Multimedia
    {
        ////////////////////Enums
    }    // namespace Multimedia

    /******************************************************************************
     * @brief Arm Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Arm
    {
        // Commands
        const ManifestEntry ARMVELOCITYCONTROL(8000, 6, DataTypes::INT16_T);
        const ManifestEntry ARMMOVETOPOSITION(8001, 6, DataTypes::FLOAT_T);
        const ManifestEntry ARMINCREMENTPOSITION(8002, 6, DataTypes::FLOAT_T);
        const ManifestEntry ARMMOVEIK(8003, 6, DataTypes::FLOAT_T);
        const ManifestEntry ARMINCREMENTIKROVER(8004, 6, DataTypes::FLOAT_T);
        const ManifestEntry ARMINCREMENTIKWRIST(8005, 6, DataTypes::FLOAT_T);
        const ManifestEntry LASERS(8006, 1, DataTypes::UINT8_T);
        const ManifestEntry ENDEFFECTOR(8007, 1, DataTypes::UINT8_T);
        const ManifestEntry GRIPPERMOVE(8008, 2, DataTypes::INT16_T);
        const ManifestEntry WATCHDOGOVERRIDE(8009, 1, DataTypes::UINT8_T);
        const ManifestEntry LIMITSWITCHOVERRIDE(8010, 1, DataTypes::UINT16_T);
        const ManifestEntry REQUESTJOINTPOSITIONS(8011, 1, DataTypes::UINT8_T);
        const ManifestEntry TOGGLEPOSITIONTELEM(8012, 1, DataTypes::UINT8_T);
        const ManifestEntry REQUESTAXESPOSITIONS(8013, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry JOINTANGLES(8101, 6, DataTypes::FLOAT_T);
        const ManifestEntry MOTORVELOCITIES(8102, 6, DataTypes::FLOAT_T);
        const ManifestEntry IKCOORDINATES(8103, 6, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry WATCHDOGSTATUS(8200, 1, DataTypes::UINT8_T);

    }    // namespace Arm

    /******************************************************************************
     * @brief ScienceActuation Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace ScienceActuation
    {
        // Commands
        const ManifestEntry SENSORAXIS(9000, 1, DataTypes::INT16_T);
        const ManifestEntry WATERSELECTOR(9001, 1, DataTypes::INT8_T);
        const ManifestEntry WATERPUMP(9002, 1, DataTypes::UINT8_T);
        const ManifestEntry LIMITSWITCHOVERRIDE(9003, 1, DataTypes::UINT8_T);
        const ManifestEntry XOOPAXIS(9004, 1, DataTypes::INT16_T);
        const ManifestEntry ZOOPAXIS(9005, 1, DataTypes::INT16_T);
        const ManifestEntry SCOOPGRABBER(9006, 1, DataTypes::UINT8_T);
        const ManifestEntry GOTOPOSITION(9007, 1, DataTypes::UINT8_T);
        const ManifestEntry INCREMENTALSCOOP(9008, 1, DataTypes::INT8_T);
        const ManifestEntry BUMPSCOOP(9009, 1, DataTypes::UINT8_T);
        const ManifestEntry MICROSCOPEFOCUS(9010, 1, DataTypes::INT16_T);
        const ManifestEntry WATERPOSITION(9011, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry ENCODERPOSITIONS(9100, 3, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry LIMITSWITCHTRIGGERED(9200, 1, DataTypes::UINT8_T);

    }    // namespace ScienceActuation

    /******************************************************************************
     * @brief ScienceSensors Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace ScienceSensors
    {
        // Commands
        const ManifestEntry FLUOROMETERLEDS(10000, 1, DataTypes::UINT8_T);
        const ManifestEntry REQFLUOROMETER(10001, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry FLUOROMETERDATA(10100, 215, DataTypes::UINT16_T);

    }    // namespace ScienceSensors

    /******************************************************************************
     * @brief Autonomy Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Autonomy
    {
        // Commands
        const ManifestEntry STARTAUTONOMY(11000, 1, DataTypes::UINT8_T);
        const ManifestEntry DISABLEAUTONOMY(11001, 1, DataTypes::UINT8_T);
        const ManifestEntry ADDPOSITIONLEG(11002, 2, DataTypes::DOUBLE_T);
        const ManifestEntry ADDMARKERLEG(11003, 2, DataTypes::DOUBLE_T);
        const ManifestEntry ADDGATELEG(11004, 2, DataTypes::DOUBLE_T);
        const ManifestEntry CLEARWAYPOINTS(11005, 1, DataTypes::UINT8_T);
        const ManifestEntry SETMAXSPEED(11006, 1, DataTypes::UINT16_T);

        // Telemetry
        const ManifestEntry CURRENTSTATE(11100, 1, DataTypes::UINT8_T);
        const ManifestEntry REACHEDMARKER(11101, 1, DataTypes::UINT8_T);
        const ManifestEntry CURRENTLOG(11102, 255, DataTypes::CHAR);

        ////////////////////Enums
        enum AUTONOMYBOARD_AUTONOMYSTATE
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
     * @brief Camera1 Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Camera1
    {
        // Commands
        const ManifestEntry CHANGECAMERAS(12000, 2, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry AVAILABLECAMERAS(12100, 1, DataTypes::UINT8_T);
        const ManifestEntry STREAMINGCAMERAS(12101, 4, DataTypes::UINT8_T);

        // Error
        const ManifestEntry CAMERAUNAVAILABLE(12200, 1, DataTypes::UINT8_T);

    }    // namespace Camera1

    /******************************************************************************
     * @brief Camera2 Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Camera2
    {}    // namespace Camera2

    /******************************************************************************
     * @brief Heater Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Heater
    {
        // Commands
        const ManifestEntry HEATERTOGGLE(15000, 1, DataTypes::UINT16_T);
        const ManifestEntry HEATERSETTEMP(15001, 12, DataTypes::FLOAT_T);

        // Telemetry
        const ManifestEntry THERMOVALUES(15100, 12, DataTypes::FLOAT_T);
        const ManifestEntry HEATERENABLED(15101, 1, DataTypes::UINT16_T);

        // Error
        const ManifestEntry OVERHEAT(15200, 1, DataTypes::UINT16_T);

    }    // namespace Heater

    /******************************************************************************
     * @brief SignalStack Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace SignalStack
    {
        // Commands
        const ManifestEntry SIGNALSROTATE(16000, 1, DataTypes::INT16_T);

        // Telemetry
        const ManifestEntry SIGNALSPOSITION(16100, 2, DataTypes::DOUBLE_T);
        const ManifestEntry SIGNALSDIRECTION(16101, 1, DataTypes::FLOAT_T);

    }    // namespace SignalStack

    /******************************************************************************
     * @brief Core Board IP Address, Commands, Telemetry, and Error Packet Information
     *
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-08-29
     ******************************************************************************/
    namespace Core
    {
        // Commands
        const ManifestEntry DRIVELEFTRIGHT(17000, 2, DataTypes::FLOAT_T);
        const ManifestEntry DRIVEINDIVIDUAL(17001, 6, DataTypes::FLOAT_T);
        const ManifestEntry WATCHDOGOVERRIDE(17002, 1, DataTypes::UINT8_T);
        const ManifestEntry LEFTDRIVEGIMBALINCREMENT(17003, 1, DataTypes::INT16_T);
        const ManifestEntry RIGHTDRIVEGIMBALINCREMENT(17004, 1, DataTypes::INT16_T);
        const ManifestEntry LEFTMAINGIMBALINCREMENT(17005, 2, DataTypes::INT16_T);
        const ManifestEntry RIGHTMAINGIMBALINCREMENT(17006, 2, DataTypes::INT16_T);
        const ManifestEntry BACKDRIVEGIMBALINCREMENT(17007, 1, DataTypes::INT16_T);
        const ManifestEntry INITIATETESTROUTINE(17008, 1, DataTypes::UINT8_T);
        const ManifestEntry LEDRGB(17009, 3, DataTypes::UINT8_T);
        const ManifestEntry LEDPATTERNS(17010, 1, DataTypes::UINT8_T);
        const ManifestEntry STATEDISPLAY(17011, 1, DataTypes::UINT8_T);
        const ManifestEntry BRIGHTNESS(17012, 1, DataTypes::UINT8_T);
        const ManifestEntry SETWATCHDOGMODE(17013, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry DRIVESPEEDS(17100, 6, DataTypes::INT16_T);

        ////////////////////Enums
        enum COREBOARD_DISPLAYSTATE
        {
            TELEOP,
            AUTONOMY,
            REACHED_GOAL
        };

        enum COREBOARD_PATTERNS
        {
            MRDT,
            BELGIUM,
            MERICA,
            DIRT,
            DOTA,
            MCD,
            WINDOWS
        };
    }     // namespace Core

#endif    // MANIFEST_H
