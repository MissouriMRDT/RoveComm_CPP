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

    namespace Drive
    {
        // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 103;
    }    // namespace Drive

    namespace BMS
    {
        // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 100;

        // Commands
        const ManifestEntry BMSSTOP(2000, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry PACKI_MEAS(2100, 1, DataTypes::FLOAT_T);
        const ManifestEntry PACKV_MEAS(2101, 1, DataTypes::FLOAT_T);
        const ManifestEntry CELLV_MEAS(2102, 8, DataTypes::FLOAT_T);
        const ManifestEntry TEMP_MEAS(2103, 8, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry PACKOVERCURRENT(2200, 1, DataTypes::UINT8_T);
        const ManifestEntry CELLUNDERVOLTAGE(2201, 1, DataTypes::UINT8_T);
        const ManifestEntry PACKUNDERVOLTAGE(2202, 8, DataTypes::UINT8_T);
        const ManifestEntry PACKSUPERHOT(2203, 8, DataTypes::UINT8_T);
    }    // namespace BMS

    namespace Power
    {
        // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 101;

        // Commands
        const ManifestEntry BMSSTOP(2000, 1, DataTypes::UINT8_T);

        // Telemetry
        const ManifestEntry PACKI_MEAS(2100, 1, DataTypes::FLOAT_T);
        const ManifestEntry PACKV_MEAS(2101, 1, DataTypes::FLOAT_T);
        const ManifestEntry CELLV_MEAS(2102, 8, DataTypes::FLOAT_T);
        const ManifestEntry TEMP_MEAS(2103, 8, DataTypes::FLOAT_T);

        // Error
        const ManifestEntry PACKOVERCURRENT(2200, 1, DataTypes::UINT8_T);
        const ManifestEntry CELLUNDERVOLTAGE(2201, 1, DataTypes::UINT8_T);
        const ManifestEntry PACKUNDERVOLTAGE(2202, 8, DataTypes::UINT8_T);
        const ManifestEntry PACKSUPERHOT(2203, 8, DataTypes::UINT8_T);
    }    // namespace Power

    namespace Blackbox
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 102;
    }    // namespace Blackbox

    namespace Nav
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 104;
    }    // namespace Nav

    namespace Gimbal
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 106;
    }    // namespace Gimbal

    namespace Multimedia
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 105;
    }    // namespace Multimedia

    namespace Arm
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 107;
    }    // namespace Arm

    namespace ScienceActuation
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 108;
    }    // namespace ScienceActuation

    namespace ScienceSensors
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 3;
        const int FOURTHOCTET = 101;
    }    // namespace ScienceSensors

    namespace Autonomy
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 3;
        const int FOURTHOCTET = 100;
    }    // namespace Autonomy

    namespace Camera1
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 4;
        const int FOURTHOCTET = 100;
    }    // namespace Camera1

    namespace Camera2
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 4;
        const int FOURTHOCTET = 101;
    }    // namespace Camera2

    namespace Heater
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 109;
    }    // namespace Heater

    namespace SignalStack
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 3;
        const int FOURTHOCTET = 102;
    }    // namespace SignalStack

    namespace Core
    {    // IP Address
        const int FIRSTOCTET  = 192;
        const int SECONDOCTET = 168;
        const int THIRDOCTET  = 2;
        const int FOURTHOCTET = 110;
    }    // namespace Core

    namespace System
    {
        const int UPDATE_RATE             = 100;
        const int ETHERNET_UDP_PORT       = 11000;
        const int ETHERNET_TCP_PORT       = 12000;

        const int SUBNET_MAC_FIRST_BYTE   = 222;
        const int SUBNET_MAC_SECOND_BYTE  = 173;

        const int PING_DATA_ID            = 1;
        const int PING_REPLY_DATA_ID      = 2;
        const int SUBSCRIBE_DATA_ID       = 3;
        const int UNSUBSCRIBE_DATA_ID     = 4;
        const int INVALID_VERSION_DATA_ID = 5;
        const int NO_DATA_DATA_ID         = 6;
    }    // namespace System
}    // namespace manifest

#endif    // MANIFEST_H
