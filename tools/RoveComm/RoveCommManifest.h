/******************************************************************************
 * @brief RoveComm Manifest
 *
 *        NOTICE! This file is auto generated and will be overwritten if edited
 *                and committed. To make changes edit the manifest.json file or
 *                edit parser.py if it is a formatting issue.
 *
 * @file RoveCommManifest.h
 * @author Missouri S&T - Mars Rover Design Team
 * @date 2023-07-29
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef RoveCommManifest_h
#define RoveCommManifest_h

#include <stdint.h>
#include "RoveCommPacket.h"

#define RC_DRIVEBOARD_FIRSTOCTET                  192       
#define RC_DRIVEBOARD_SECONDOCTET                 168       
#define RC_DRIVEBOARD_THIRDOCTET                  2         
#define RC_DRIVEBOARD_FOURTHOCTET                 103       

#define RC_BMSBOARD_FIRSTOCTET                    192       
#define RC_BMSBOARD_SECONDOCTET                   168       
#define RC_BMSBOARD_THIRDOCTET                    2         
#define RC_BMSBOARD_FOURTHOCTET                   100       

#define RC_POWERBOARD_FIRSTOCTET                  192       
#define RC_POWERBOARD_SECONDOCTET                 168       
#define RC_POWERBOARD_THIRDOCTET                  2         
#define RC_POWERBOARD_FOURTHOCTET                 101       

#define RC_BLACKBOXBOARD_FIRSTOCTET               192       
#define RC_BLACKBOXBOARD_SECONDOCTET              168       
#define RC_BLACKBOXBOARD_THIRDOCTET               2         
#define RC_BLACKBOXBOARD_FOURTHOCTET              102       

#define RC_NAVBOARD_FIRSTOCTET                    192       
#define RC_NAVBOARD_SECONDOCTET                   168       
#define RC_NAVBOARD_THIRDOCTET                    2         
#define RC_NAVBOARD_FOURTHOCTET                   104       

#define RC_GIMBALBOARD_FIRSTOCTET                 192       
#define RC_GIMBALBOARD_SECONDOCTET                168       
#define RC_GIMBALBOARD_THIRDOCTET                 2         
#define RC_GIMBALBOARD_FOURTHOCTET                106       

#define RC_MULTIMEDIABOARD_FIRSTOCTET             192       
#define RC_MULTIMEDIABOARD_SECONDOCTET            168       
#define RC_MULTIMEDIABOARD_THIRDOCTET             2         
#define RC_MULTIMEDIABOARD_FOURTHOCTET            105       

#define RC_ARMBOARD_FIRSTOCTET                    192       
#define RC_ARMBOARD_SECONDOCTET                   168       
#define RC_ARMBOARD_THIRDOCTET                    2         
#define RC_ARMBOARD_FOURTHOCTET                   107       

#define RC_SCIENCEACTUATIONBOARD_FIRSTOCTET       192       
#define RC_SCIENCEACTUATIONBOARD_SECONDOCTET      168       
#define RC_SCIENCEACTUATIONBOARD_THIRDOCTET       2         
#define RC_SCIENCEACTUATIONBOARD_FOURTHOCTET      108       

#define RC_SCIENCESENSORSBOARD_FIRSTOCTET         192       
#define RC_SCIENCESENSORSBOARD_SECONDOCTET        168       
#define RC_SCIENCESENSORSBOARD_THIRDOCTET         3         
#define RC_SCIENCESENSORSBOARD_FOURTHOCTET        101       

#define RC_AUTONOMYBOARD_FIRSTOCTET               192       
#define RC_AUTONOMYBOARD_SECONDOCTET              168       
#define RC_AUTONOMYBOARD_THIRDOCTET               3         
#define RC_AUTONOMYBOARD_FOURTHOCTET              100       

#define RC_CAMERA1BOARD_FIRSTOCTET                192       
#define RC_CAMERA1BOARD_SECONDOCTET               168       
#define RC_CAMERA1BOARD_THIRDOCTET                4         
#define RC_CAMERA1BOARD_FOURTHOCTET               100       

#define RC_CAMERA2BOARD_FIRSTOCTET                192       
#define RC_CAMERA2BOARD_SECONDOCTET               168       
#define RC_CAMERA2BOARD_THIRDOCTET                4         
#define RC_CAMERA2BOARD_FOURTHOCTET               101       

#define RC_HEATERBOARD_FIRSTOCTET                 192       
#define RC_HEATERBOARD_SECONDOCTET                168       
#define RC_HEATERBOARD_THIRDOCTET                 2         
#define RC_HEATERBOARD_FOURTHOCTET                109       

#define RC_SIGNALSTACKBOARD_FIRSTOCTET            192       
#define RC_SIGNALSTACKBOARD_SECONDOCTET           168       
#define RC_SIGNALSTACKBOARD_THIRDOCTET            3         
#define RC_SIGNALSTACKBOARD_FOURTHOCTET           102       

#define RC_COREBOARD_FIRSTOCTET                   192       
#define RC_COREBOARD_SECONDOCTET                  168       
#define RC_COREBOARD_THIRDOCTET                   2         
#define RC_COREBOARD_FOURTHOCTET                  110       



#define ROVECOMM_UPDATE_RATE                      100       
#define RC_ROVECOMM_ETHERNET_UDP_PORT             11000     
#define RC_ROVECOMM_ETHERNET_TCP_PORT             12000     


#define RC_ROVECOMM_SUBNET_MAC_FIRST_BYTE         222       
#define RC_ROVECOMM_SUBNET_MAC_SECOND_BYTE        173       


///////////////////////////////////////////////////
////////////        System Packets      ///////////         
///////////////////////////////////////////////////

#define RC_ROVECOMM_PING_DATA_ID                  1         
#define RC_ROVECOMM_PING_REPLY_DATA_ID            2         
#define RC_ROVECOMM_SUBSCRIBE_DATA_ID             3         
#define RC_ROVECOMM_UNSUBSCRIBE_DATA_ID           4         
#define RC_ROVECOMM_INVALID_VERSION_DATA_ID       5         
#define RC_ROVECOMM_NO_DATA_DATA_ID               6         


///////////////////////////////////////////////////
////////////        DRIVEBOARD          ///////////         
///////////////////////////////////////////////////



///////////////////////////////////////////////////
////////////        BMSBOARD            ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[delay] (s) -> a delay of 0 will shutdown, not restart and cannot be reversed
#define RC_BMSBOARD_BMSSTOP_DATA_ID                                   2000      
#define RC_BMSBOARD_BMSSTOP_DATA_COUNT                                1         
#define RC_BMSBOARD_BMSSTOP_DATA_TYPE                                 uint8_t   

////////////////////Telemetry
//Total Current
#define RC_BMSBOARD_PACKI_MEAS_DATA_ID                                2100      
#define RC_BMSBOARD_PACKI_MEAS_DATA_COUNT                             1         
#define RC_BMSBOARD_PACKI_MEAS_DATA_TYPE                              float     

//Pack Voltage
#define RC_BMSBOARD_PACKV_MEAS_DATA_ID                                2101      
#define RC_BMSBOARD_PACKV_MEAS_DATA_COUNT                             1         
#define RC_BMSBOARD_PACKV_MEAS_DATA_TYPE                              float     

//C1-G, C2-1, C3-2, C4-3, C5-4, C6-5, C7-6, C8-7
#define RC_BMSBOARD_CELLV_MEAS_DATA_ID                                2102      
#define RC_BMSBOARD_CELLV_MEAS_DATA_COUNT                             8         
#define RC_BMSBOARD_CELLV_MEAS_DATA_TYPE                              float     

//Temperature
#define RC_BMSBOARD_TEMP_MEAS_DATA_ID                                 2103      
#define RC_BMSBOARD_TEMP_MEAS_DATA_COUNT                              1         
#define RC_BMSBOARD_TEMP_MEAS_DATA_TYPE                               float     

////////////////////Error
//
#define RC_BMSBOARD_PACKOVERCURRENT_DATA_ID                           2200      
#define RC_BMSBOARD_PACKOVERCURRENT_DATA_COUNT                        1         
#define RC_BMSBOARD_PACKOVERCURRENT_DATA_TYPE                         uint8_t   

//(bitmasked)
#define RC_BMSBOARD_CELLUNDERVOLTAGE_DATA_ID                          2201      
#define RC_BMSBOARD_CELLUNDERVOLTAGE_DATA_COUNT                       1         
#define RC_BMSBOARD_CELLUNDERVOLTAGE_DATA_TYPE                        uint8_t   

//
#define RC_BMSBOARD_PACKUNDERVOLTAGE_DATA_ID                          2202      
#define RC_BMSBOARD_PACKUNDERVOLTAGE_DATA_COUNT                       1         
#define RC_BMSBOARD_PACKUNDERVOLTAGE_DATA_TYPE                        uint8_t   

//
#define RC_BMSBOARD_PACKSUPERHOT_DATA_ID                              2203      
#define RC_BMSBOARD_PACKSUPERHOT_DATA_COUNT                           1         
#define RC_BMSBOARD_PACKSUPERHOT_DATA_TYPE                            uint8_t   



///////////////////////////////////////////////////
////////////        POWERBOARD          ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//M1, M2, M3, M4, M5, M6, Spare
#define RC_POWERBOARD_MOTORBUSENABLE_DATA_ID                          3000      
#define RC_POWERBOARD_MOTORBUSENABLE_DATA_COUNT                       1         
#define RC_POWERBOARD_MOTORBUSENABLE_DATA_TYPE                        uint8_t   

//Aux, Spare High
#define RC_POWERBOARD_HIGHBUSENABLE_DATA_ID                           3001      
#define RC_POWERBOARD_HIGHBUSENABLE_DATA_COUNT                        1         
#define RC_POWERBOARD_HIGHBUSENABLE_DATA_TYPE                         uint8_t   

//Gimbal, Drive, Multi, Nav, Spare Low
#define RC_POWERBOARD_LOWBUSENABLE_DATA_ID                            3002      
#define RC_POWERBOARD_LOWBUSENABLE_DATA_COUNT                         1         
#define RC_POWERBOARD_LOWBUSENABLE_DATA_TYPE                          uint8_t   

//Cam, 12vSpare, Net1, Net2
#define RC_POWERBOARD_TWELVEVBUSENABLE_DATA_ID                        3003      
#define RC_POWERBOARD_TWELVEVBUSENABLE_DATA_COUNT                     1         
#define RC_POWERBOARD_TWELVEVBUSENABLE_DATA_TYPE                      uint8_t   

////////////////////Telemetry
//M1, M2, M3, M4, M5, M6, Spare
#define RC_POWERBOARD_MOTORBUSENABLED_DATA_ID                         3100      
#define RC_POWERBOARD_MOTORBUSENABLED_DATA_COUNT                      1         
#define RC_POWERBOARD_MOTORBUSENABLED_DATA_TYPE                       uint8_t   

//Aux, Spare High
#define RC_POWERBOARD_HIGHBUSENABLED_DATA_ID                          3101      
#define RC_POWERBOARD_HIGHBUSENABLED_DATA_COUNT                       1         
#define RC_POWERBOARD_HIGHBUSENABLED_DATA_TYPE                        uint8_t   

//Gimbal, Drive, Multi, Nav, Spare Low
#define RC_POWERBOARD_LOWBUSENABLED_DATA_ID                           3102      
#define RC_POWERBOARD_LOWBUSENABLED_DATA_COUNT                        1         
#define RC_POWERBOARD_LOWBUSENABLED_DATA_TYPE                         uint8_t   

//Cam, 12vSpare, Net1, Net2
#define RC_POWERBOARD_TWELVEVBUSENABLED_DATA_ID                       3103      
#define RC_POWERBOARD_TWELVEVBUSENABLED_DATA_COUNT                    1         
#define RC_POWERBOARD_TWELVEVBUSENABLED_DATA_TYPE                     uint8_t   

//M1, M2, M3, M4, M5, M6, Spare
#define RC_POWERBOARD_MOTORBUSCURRENT_DATA_ID                         3104      
#define RC_POWERBOARD_MOTORBUSCURRENT_DATA_COUNT                      7         
#define RC_POWERBOARD_MOTORBUSCURRENT_DATA_TYPE                       float     

//Aux, Spare High
#define RC_POWERBOARD_HIGHBUSCURRENT_DATA_ID                          3105      
#define RC_POWERBOARD_HIGHBUSCURRENT_DATA_COUNT                       2         
#define RC_POWERBOARD_HIGHBUSCURRENT_DATA_TYPE                        float     

//Gimbal, Drive, Multi, Nav, Spare Low
#define RC_POWERBOARD_LOWBUSCURRENT_DATA_ID                           3106      
#define RC_POWERBOARD_LOWBUSCURRENT_DATA_COUNT                        5         
#define RC_POWERBOARD_LOWBUSCURRENT_DATA_TYPE                         float     

//Cam, 12vSpare, Net1, Net2
#define RC_POWERBOARD_TWELVEVBUSCURRENT_DATA_ID                       3107      
#define RC_POWERBOARD_TWELVEVBUSCURRENT_DATA_COUNT                    4         
#define RC_POWERBOARD_TWELVEVBUSCURRENT_DATA_TYPE                     float     

////////////////////Error
//[(0-undermaxcurrent, 1-overcurrent)] [M1, M2, M3, M4, M5, M6, Spare (Bitmask)]
#define RC_POWERBOARD_MOTORBUSOVERCURRENT_DATA_ID                     3200      
#define RC_POWERBOARD_MOTORBUSOVERCURRENT_DATA_COUNT                  1         
#define RC_POWERBOARD_MOTORBUSOVERCURRENT_DATA_TYPE                   uint8_t   

//[(0-undermaxcurrent, 1-overcurrent)] [Aux, Spare High (Bitmask)]
#define RC_POWERBOARD_HIGHBUSOVERCURRENT_DATA_ID                      3201      
#define RC_POWERBOARD_HIGHBUSOVERCURRENT_DATA_COUNT                   1         
#define RC_POWERBOARD_HIGHBUSOVERCURRENT_DATA_TYPE                    uint8_t   

//[(0-undermaxcurrent, 1-overcurrent)] [Gimbal, Drive, Multi, Nav, Cam, BBB, Spare Low (Bitmask)]
#define RC_POWERBOARD_LOWBUSOVERCURRENT_DATA_ID                       3202      
#define RC_POWERBOARD_LOWBUSOVERCURRENT_DATA_COUNT                    1         
#define RC_POWERBOARD_LOWBUSOVERCURRENT_DATA_TYPE                     uint8_t   



///////////////////////////////////////////////////
////////////        BLACKBOXBOARD       ///////////         
///////////////////////////////////////////////////

////////////////////Telemetry
//[(0-Disable, 1-Enable)]
#define RC_BLACKBOXBOARD_BLACKBOXLISTENING_DATA_ID                    4100      
#define RC_BLACKBOXBOARD_BLACKBOXLISTENING_DATA_COUNT                 1         
#define RC_BLACKBOXBOARD_BLACKBOXLISTENING_DATA_TYPE                  uint8_t   



///////////////////////////////////////////////////
////////////        NAVBOARD            ///////////         
///////////////////////////////////////////////////

////////////////////Telemetry
//[Lat, Long] [(-90, 90), (-180, 180)] (deg)
#define RC_NAVBOARD_GPSLATLON_DATA_ID                                 5100      
#define RC_NAVBOARD_GPSLATLON_DATA_COUNT                              2         
#define RC_NAVBOARD_GPSLATLON_DATA_TYPE                               double    

//[Pitch, Yaw, Roll] [(-90, 90), (0, 360), (-90, 90)] (deg)
#define RC_NAVBOARD_IMUDATA_DATA_ID                                   5101      
#define RC_NAVBOARD_IMUDATA_DATA_COUNT                                3         
#define RC_NAVBOARD_IMUDATA_DATA_TYPE                                 float     

//[Heading] [ 0, 360 ]
#define RC_NAVBOARD_COMPASSDATA_DATA_ID                               5102      
#define RC_NAVBOARD_COMPASSDATA_DATA_COUNT                            1         
#define RC_NAVBOARD_COMPASSDATA_DATA_TYPE                             float     

//[Number of satellites]
#define RC_NAVBOARD_SATELLITECOUNTDATA_DATA_ID                        5103      
#define RC_NAVBOARD_SATELLITECOUNTDATA_DATA_COUNT                     1         
#define RC_NAVBOARD_SATELLITECOUNTDATA_DATA_TYPE                      uint8_t   

//[xAxis, yAxis, zAxis] Accel in m/s^2
#define RC_NAVBOARD_ACCELEROMETERDATA_DATA_ID                         5104      
#define RC_NAVBOARD_ACCELEROMETERDATA_DATA_COUNT                      3         
#define RC_NAVBOARD_ACCELEROMETERDATA_DATA_TYPE                       float     

//[horizontal_accur, vertical_accur, heading_accur] Accuracy in meters/degrees
#define RC_NAVBOARD_ACCURACYDATA_DATA_ID                              5105      
#define RC_NAVBOARD_ACCURACYDATA_DATA_COUNT                           3         
#define RC_NAVBOARD_ACCURACYDATA_DATA_TYPE                            float     

////////////////////Error
//
#define RC_NAVBOARD_GPSLOCKERROR_DATA_ID                              5200      
#define RC_NAVBOARD_GPSLOCKERROR_DATA_COUNT                           1         
#define RC_NAVBOARD_GPSLOCKERROR_DATA_TYPE                            uint8_t   



///////////////////////////////////////////////////
////////////        GIMBALBOARD         ///////////         
///////////////////////////////////////////////////



///////////////////////////////////////////////////
////////////        MULTIMEDIABOARD     ///////////         
///////////////////////////////////////////////////

////////////////////Enums


///////////////////////////////////////////////////
////////////        ARMBOARD            ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[J1, J2, J3, J4, J5, J6] (rpm)
#define RC_ARMBOARD_ARMVELOCITYCONTROL_DATA_ID                        8000      
#define RC_ARMBOARD_ARMVELOCITYCONTROL_DATA_COUNT                     6         
#define RC_ARMBOARD_ARMVELOCITYCONTROL_DATA_TYPE                      int16_t   

//[J1, J2, J3, J4, J5, J6] (Degrees)
#define RC_ARMBOARD_ARMMOVETOPOSITION_DATA_ID                         8001      
#define RC_ARMBOARD_ARMMOVETOPOSITION_DATA_COUNT                      6         
#define RC_ARMBOARD_ARMMOVETOPOSITION_DATA_TYPE                       float     

//[J1, J2, J3, J4, J5, J6] (Degrees)
#define RC_ARMBOARD_ARMINCREMENTPOSITION_DATA_ID                      8002      
#define RC_ARMBOARD_ARMINCREMENTPOSITION_DATA_COUNT                   6         
#define RC_ARMBOARD_ARMINCREMENTPOSITION_DATA_TYPE                    float     

//[X, Y, Z, Y, P, R] (in)
#define RC_ARMBOARD_ARMMOVEIK_DATA_ID                                 8003      
#define RC_ARMBOARD_ARMMOVEIK_DATA_COUNT                              6         
#define RC_ARMBOARD_ARMMOVEIK_DATA_TYPE                               float     

//[X, Y, Z, Y, P, R] (in)
#define RC_ARMBOARD_ARMINCREMENTIKROVER_DATA_ID                       8004      
#define RC_ARMBOARD_ARMINCREMENTIKROVER_DATA_COUNT                    6         
#define RC_ARMBOARD_ARMINCREMENTIKROVER_DATA_TYPE                     float     

//[X, Y, Z, Y, P, R] (in)
#define RC_ARMBOARD_ARMINCREMENTIKWRIST_DATA_ID                       8005      
#define RC_ARMBOARD_ARMINCREMENTIKWRIST_DATA_COUNT                    6         
#define RC_ARMBOARD_ARMINCREMENTIKWRIST_DATA_TYPE                     float     

//[1-enable, 0-disable]
#define RC_ARMBOARD_LASERS_DATA_ID                                    8006      
#define RC_ARMBOARD_LASERS_DATA_COUNT                                 1         
#define RC_ARMBOARD_LASERS_DATA_TYPE                                  uint8_t   

//0: retract, 1: extend
#define RC_ARMBOARD_ENDEFFECTOR_DATA_ID                               8007      
#define RC_ARMBOARD_ENDEFFECTOR_DATA_COUNT                            1         
#define RC_ARMBOARD_ENDEFFECTOR_DATA_TYPE                             uint8_t   

//[Power] (-1000, 1000) (m%)
#define RC_ARMBOARD_GRIPPERMOVE_DATA_ID                               8008      
#define RC_ARMBOARD_GRIPPERMOVE_DATA_COUNT                            2         
#define RC_ARMBOARD_GRIPPERMOVE_DATA_TYPE                             int16_t   

//[0-Turn off Watchdog Override, 1-Turn on Watchdog Override]
#define RC_ARMBOARD_WATCHDOGOVERRIDE_DATA_ID                          8009      
#define RC_ARMBOARD_WATCHDOGOVERRIDE_DATA_COUNT                       1         
#define RC_ARMBOARD_WATCHDOGOVERRIDE_DATA_TYPE                        uint8_t   

//[J6 Forward, J6 Reverse, J5 Forward, J5 Reverse, J4 Forward, J4 Reverse, J3 Forward, J3 Reverse, J2 Forward, J2 Reverse, J1 Forward, J1 Reverse] (0-Turn off Limit Switch Override, 1-Turn on Limit Switch Override) (bitmasked)
#define RC_ARMBOARD_LIMITSWITCHOVERRIDE_DATA_ID                       8010      
#define RC_ARMBOARD_LIMITSWITCHOVERRIDE_DATA_COUNT                    1         
#define RC_ARMBOARD_LIMITSWITCHOVERRIDE_DATA_TYPE                     uint16_t  

//Prompt arm for J1-6 positions
#define RC_ARMBOARD_REQUESTJOINTPOSITIONS_DATA_ID                     8011      
#define RC_ARMBOARD_REQUESTJOINTPOSITIONS_DATA_COUNT                  1         
#define RC_ARMBOARD_REQUESTJOINTPOSITIONS_DATA_TYPE                   uint8_t   

//Start auto pushing arm J1-6 positions
#define RC_ARMBOARD_TOGGLEPOSITIONTELEM_DATA_ID                       8012      
#define RC_ARMBOARD_TOGGLEPOSITIONTELEM_DATA_COUNT                    1         
#define RC_ARMBOARD_TOGGLEPOSITIONTELEM_DATA_TYPE                     uint8_t   

//Prompt arm for XYZPYR Data
#define RC_ARMBOARD_REQUESTAXESPOSITIONS_DATA_ID                      8013      
#define RC_ARMBOARD_REQUESTAXESPOSITIONS_DATA_COUNT                   1         
#define RC_ARMBOARD_REQUESTAXESPOSITIONS_DATA_TYPE                    uint8_t   

////////////////////Telemetry
//[J1, J2, J3, J4, J5, J6] (0, Deg)
#define RC_ARMBOARD_JOINTANGLES_DATA_ID                               8101      
#define RC_ARMBOARD_JOINTANGLES_DATA_COUNT                            6         
#define RC_ARMBOARD_JOINTANGLES_DATA_TYPE                             float     

//[J1, J2, J3, J4, J5, J6] (0, rpm)
#define RC_ARMBOARD_MOTORVELOCITIES_DATA_ID                           8102      
#define RC_ARMBOARD_MOTORVELOCITIES_DATA_COUNT                        6         
#define RC_ARMBOARD_MOTORVELOCITIES_DATA_TYPE                         float     

//[X, Y, Z, Y, P, R]
#define RC_ARMBOARD_IKCOORDINATES_DATA_ID                             8103      
#define RC_ARMBOARD_IKCOORDINATES_DATA_COUNT                          6         
#define RC_ARMBOARD_IKCOORDINATES_DATA_TYPE                           float     

////////////////////Error
//[WatchDogStatus] (0-WD Not Triggered, 1-WD Triggered) 
#define RC_ARMBOARD_WATCHDOGSTATUS_DATA_ID                            8200      
#define RC_ARMBOARD_WATCHDOGSTATUS_DATA_COUNT                         1         
#define RC_ARMBOARD_WATCHDOGSTATUS_DATA_TYPE                          uint8_t   



///////////////////////////////////////////////////
////////////        SCIENCEACTUATIONBOARD///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[Power] (-1000, 1000) (m%)
#define RC_SCIENCEACTUATIONBOARD_SENSORAXIS_DATA_ID                   9000      
#define RC_SCIENCEACTUATIONBOARD_SENSORAXIS_DATA_COUNT                1         
#define RC_SCIENCEACTUATIONBOARD_SENSORAXIS_DATA_TYPE                 int16_t   

//Sign of value determines direction
#define RC_SCIENCEACTUATIONBOARD_WATERSELECTOR_DATA_ID                9001      
#define RC_SCIENCEACTUATIONBOARD_WATERSELECTOR_DATA_COUNT             1         
#define RC_SCIENCEACTUATIONBOARD_WATERSELECTOR_DATA_TYPE              int8_t    

//Water pump (0-Off, 1-Pump)
#define RC_SCIENCEACTUATIONBOARD_WATERPUMP_DATA_ID                    9002      
#define RC_SCIENCEACTUATIONBOARD_WATERPUMP_DATA_COUNT                 1         
#define RC_SCIENCEACTUATIONBOARD_WATERPUMP_DATA_TYPE                  uint8_t   

//[Zoop-axis Top, Zoop-axis Bottom, Xoop-axis Left, Xoop-axis Right, Sensor Axis Top, Sensor Axis Bottom] (0-Turn off Limit Switch Override, 1-Turn on Limit Switch Override) (bitmasked)
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHOVERRIDE_DATA_ID          9003      
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHOVERRIDE_DATA_COUNT       1         
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHOVERRIDE_DATA_TYPE        uint8_t   

//[Power] (-1000, 1000) (m%)
#define RC_SCIENCEACTUATIONBOARD_XOOPAXIS_DATA_ID                     9004      
#define RC_SCIENCEACTUATIONBOARD_XOOPAXIS_DATA_COUNT                  1         
#define RC_SCIENCEACTUATIONBOARD_XOOPAXIS_DATA_TYPE                   int16_t   

//[Power] (-1000, 1000) (m%)
#define RC_SCIENCEACTUATIONBOARD_ZOOPAXIS_DATA_ID                     9005      
#define RC_SCIENCEACTUATIONBOARD_ZOOPAXIS_DATA_COUNT                  1         
#define RC_SCIENCEACTUATIONBOARD_ZOOPAXIS_DATA_TYPE                   int16_t   

//[Position] (0 Open, 1 Closed, 2 Drop)
#define RC_SCIENCEACTUATIONBOARD_SCOOPGRABBER_DATA_ID                 9006      
#define RC_SCIENCEACTUATIONBOARD_SCOOPGRABBER_DATA_COUNT              1         
#define RC_SCIENCEACTUATIONBOARD_SCOOPGRABBER_DATA_TYPE               uint8_t   

//[Position] (0, 1 - 6, 7)
#define RC_SCIENCEACTUATIONBOARD_GOTOPOSITION_DATA_ID                 9007      
#define RC_SCIENCEACTUATIONBOARD_GOTOPOSITION_DATA_COUNT              1         
#define RC_SCIENCEACTUATIONBOARD_GOTOPOSITION_DATA_TYPE               uint8_t   

//Degrees 0-180
#define RC_SCIENCEACTUATIONBOARD_INCREMENTALSCOOP_DATA_ID             9008      
#define RC_SCIENCEACTUATIONBOARD_INCREMENTALSCOOP_DATA_COUNT          1         
#define RC_SCIENCEACTUATIONBOARD_INCREMENTALSCOOP_DATA_TYPE           int8_t    

//Quickly barely open and close the scoop
#define RC_SCIENCEACTUATIONBOARD_BUMPSCOOP_DATA_ID                    9009      
#define RC_SCIENCEACTUATIONBOARD_BUMPSCOOP_DATA_COUNT                 1         
#define RC_SCIENCEACTUATIONBOARD_BUMPSCOOP_DATA_TYPE                  uint8_t   

//Motor speed value (-1000,1000)
#define RC_SCIENCEACTUATIONBOARD_MICROSCOPEFOCUS_DATA_ID              9010      
#define RC_SCIENCEACTUATIONBOARD_MICROSCOPEFOCUS_DATA_COUNT           1         
#define RC_SCIENCEACTUATIONBOARD_MICROSCOPEFOCUS_DATA_TYPE            int16_t   

//[Position] (0, 1-12)
#define RC_SCIENCEACTUATIONBOARD_WATERPOSITION_DATA_ID                9011      
#define RC_SCIENCEACTUATIONBOARD_WATERPOSITION_DATA_COUNT             1         
#define RC_SCIENCEACTUATIONBOARD_WATERPOSITION_DATA_TYPE              uint8_t   

////////////////////Telemetry
//[scoop z-pos, scoop x-pos, sensor z-axis] (absolute position 0-360)
#define RC_SCIENCEACTUATIONBOARD_ENCODERPOSITIONS_DATA_ID             9100      
#define RC_SCIENCEACTUATIONBOARD_ENCODERPOSITIONS_DATA_COUNT          3         
#define RC_SCIENCEACTUATIONBOARD_ENCODERPOSITIONS_DATA_TYPE           float     

////////////////////Error
//[Zoop-axis Top, Zoop-axis Bottom, Xoop-axis Left, Xoop-axis Right, Sensor Axis Top, Sensor Axis Bottom] (0-OFF, 1-ON) (bitmasked)
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHTRIGGERED_DATA_ID         9200      
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHTRIGGERED_DATA_COUNT      1         
#define RC_SCIENCEACTUATIONBOARD_LIMITSWITCHTRIGGERED_DATA_TYPE       uint8_t   



///////////////////////////////////////////////////
////////////        SCIENCESENSORSBOARD ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[Wavelengths: 1, 2, 3, 4] (0-Disable, 1-Enable)
#define RC_SCIENCESENSORSBOARD_FLUOROMETERLEDS_DATA_ID                10000     
#define RC_SCIENCESENSORSBOARD_FLUOROMETERLEDS_DATA_COUNT             1         
#define RC_SCIENCESENSORSBOARD_FLUOROMETERLEDS_DATA_TYPE              uint8_t   

//Request a new Fluorometer reading
#define RC_SCIENCESENSORSBOARD_REQFLUOROMETER_DATA_ID                 10001     
#define RC_SCIENCESENSORSBOARD_REQFLUOROMETER_DATA_COUNT              1         
#define RC_SCIENCESENSORSBOARD_REQFLUOROMETER_DATA_TYPE               uint8_t   

////////////////////Telemetry
//Diode readings from sensor
#define RC_SCIENCESENSORSBOARD_FLUOROMETERDATA_DATA_ID                10100     
#define RC_SCIENCESENSORSBOARD_FLUOROMETERDATA_DATA_COUNT             215       
#define RC_SCIENCESENSORSBOARD_FLUOROMETERDATA_DATA_TYPE              uint16_t  



///////////////////////////////////////////////////
////////////        AUTONOMYBOARD       ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//
#define RC_AUTONOMYBOARD_STARTAUTONOMY_DATA_ID                        11000     
#define RC_AUTONOMYBOARD_STARTAUTONOMY_DATA_COUNT                     1         
#define RC_AUTONOMYBOARD_STARTAUTONOMY_DATA_TYPE                      uint8_t   

//
#define RC_AUTONOMYBOARD_DISABLEAUTONOMY_DATA_ID                      11001     
#define RC_AUTONOMYBOARD_DISABLEAUTONOMY_DATA_COUNT                   1         
#define RC_AUTONOMYBOARD_DISABLEAUTONOMY_DATA_TYPE                    uint8_t   

//[Lat, Lon]
#define RC_AUTONOMYBOARD_ADDPOSITIONLEG_DATA_ID                       11002     
#define RC_AUTONOMYBOARD_ADDPOSITIONLEG_DATA_COUNT                    2         
#define RC_AUTONOMYBOARD_ADDPOSITIONLEG_DATA_TYPE                     double    

//[Lat, Lon]
#define RC_AUTONOMYBOARD_ADDMARKERLEG_DATA_ID                         11003     
#define RC_AUTONOMYBOARD_ADDMARKERLEG_DATA_COUNT                      2         
#define RC_AUTONOMYBOARD_ADDMARKERLEG_DATA_TYPE                       double    

//[Lat, Lon]
#define RC_AUTONOMYBOARD_ADDGATELEG_DATA_ID                           11004     
#define RC_AUTONOMYBOARD_ADDGATELEG_DATA_COUNT                        2         
#define RC_AUTONOMYBOARD_ADDGATELEG_DATA_TYPE                         double    

//
#define RC_AUTONOMYBOARD_CLEARWAYPOINTS_DATA_ID                       11005     
#define RC_AUTONOMYBOARD_CLEARWAYPOINTS_DATA_COUNT                    1         
#define RC_AUTONOMYBOARD_CLEARWAYPOINTS_DATA_TYPE                     uint8_t   

//
#define RC_AUTONOMYBOARD_SETMAXSPEED_DATA_ID                          11006     
#define RC_AUTONOMYBOARD_SETMAXSPEED_DATA_COUNT                       1         
#define RC_AUTONOMYBOARD_SETMAXSPEED_DATA_TYPE                        uint16_t  

////////////////////Telemetry
//Enum (AUTONOMYSTATE)
#define RC_AUTONOMYBOARD_CURRENTSTATE_DATA_ID                         11100     
#define RC_AUTONOMYBOARD_CURRENTSTATE_DATA_COUNT                      1         
#define RC_AUTONOMYBOARD_CURRENTSTATE_DATA_TYPE                       uint8_t   

//
#define RC_AUTONOMYBOARD_REACHEDMARKER_DATA_ID                        11101     
#define RC_AUTONOMYBOARD_REACHEDMARKER_DATA_COUNT                     1         
#define RC_AUTONOMYBOARD_REACHEDMARKER_DATA_TYPE                      uint8_t   

//String version of most current error log
#define RC_AUTONOMYBOARD_CURRENTLOG_DATA_ID                           11102     
#define RC_AUTONOMYBOARD_CURRENTLOG_DATA_COUNT                        255       
#define RC_AUTONOMYBOARD_CURRENTLOG_DATA_TYPE                         char      

////////////////////Enums
enum AUTONOMYBOARD_AUTONOMYSTATE {IDLE,NAVIGATING,SEARCHPATTERN,APPROACHINGMARKER,APPROACHINGGATE,AVOIDANCE,REVERSING,STUCK}; 


///////////////////////////////////////////////////
////////////        CAMERA1BOARD        ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//Change which camera a feed is looking at. [0] is the feed, [1] is the camera to view.
#define RC_CAMERA1BOARD_CHANGECAMERAS_DATA_ID                         12000     
#define RC_CAMERA1BOARD_CHANGECAMERAS_DATA_COUNT                      2         
#define RC_CAMERA1BOARD_CHANGECAMERAS_DATA_TYPE                       uint8_t   

////////////////////Telemetry
//Bitmask values for which cameras are able to stream. LSB is Camera 0, MSB is Camera 7.
#define RC_CAMERA1BOARD_AVAILABLECAMERAS_DATA_ID                      12100     
#define RC_CAMERA1BOARD_AVAILABLECAMERAS_DATA_COUNT                   1         
#define RC_CAMERA1BOARD_AVAILABLECAMERAS_DATA_TYPE                    uint8_t   

//Which cameras the system is currently streaming on each port
#define RC_CAMERA1BOARD_STREAMINGCAMERAS_DATA_ID                      12101     
#define RC_CAMERA1BOARD_STREAMINGCAMERAS_DATA_COUNT                   4         
#define RC_CAMERA1BOARD_STREAMINGCAMERAS_DATA_TYPE                    uint8_t   

////////////////////Error
//Camera has errored and stopped streaming. [0] is ID of camera as an integer (not bitmask).
#define RC_CAMERA1BOARD_CAMERAUNAVAILABLE_DATA_ID                     12200     
#define RC_CAMERA1BOARD_CAMERAUNAVAILABLE_DATA_COUNT                  1         
#define RC_CAMERA1BOARD_CAMERAUNAVAILABLE_DATA_TYPE                   uint8_t   



///////////////////////////////////////////////////
////////////        CAMERA2BOARD        ///////////         
///////////////////////////////////////////////////



///////////////////////////////////////////////////
////////////        HEATERBOARD         ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//Bitmask (1-ON, 0-OFF)
#define RC_HEATERBOARD_HEATERTOGGLE_DATA_ID                           15000     
#define RC_HEATERBOARD_HEATERTOGGLE_DATA_COUNT                        1         
#define RC_HEATERBOARD_HEATERTOGGLE_DATA_TYPE                         uint16_t  

//Sets target temperature for the heaters
#define RC_HEATERBOARD_HEATERSETTEMP_DATA_ID                          15001     
#define RC_HEATERBOARD_HEATERSETTEMP_DATA_COUNT                       12        
#define RC_HEATERBOARD_HEATERSETTEMP_DATA_TYPE                        float     

////////////////////Telemetry
//[1, 2, 3...] (degrees C)
#define RC_HEATERBOARD_THERMOVALUES_DATA_ID                           15100     
#define RC_HEATERBOARD_THERMOVALUES_DATA_COUNT                        12        
#define RC_HEATERBOARD_THERMOVALUES_DATA_TYPE                         float     

//Heater #[...3, 2, 1] (0-Disabled, 1-Enabled)
#define RC_HEATERBOARD_HEATERENABLED_DATA_ID                          15101     
#define RC_HEATERBOARD_HEATERENABLED_DATA_COUNT                       1         
#define RC_HEATERBOARD_HEATERENABLED_DATA_TYPE                        uint16_t  

////////////////////Error
//Bitmask (1-Overheated, 0-not overheated)
#define RC_HEATERBOARD_OVERHEAT_DATA_ID                               15200     
#define RC_HEATERBOARD_OVERHEAT_DATA_COUNT                            1         
#define RC_HEATERBOARD_OVERHEAT_DATA_TYPE                             uint16_t  



///////////////////////////////////////////////////
////////////        SIGNALSTACKBOARD    ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[Power] (-1000, 1000) (m%)
#define RC_SIGNALSTACKBOARD_SIGNALSROTATE_DATA_ID                     16000     
#define RC_SIGNALSTACKBOARD_SIGNALSROTATE_DATA_COUNT                  1         
#define RC_SIGNALSTACKBOARD_SIGNALSROTATE_DATA_TYPE                   int16_t   

////////////////////Telemetry
//[Lat, Long] [(-90, 90), (-180, 180)] (deg)
#define RC_SIGNALSTACKBOARD_SIGNALSPOSITION_DATA_ID                   16100     
#define RC_SIGNALSTACKBOARD_SIGNALSPOSITION_DATA_COUNT                2         
#define RC_SIGNALSTACKBOARD_SIGNALSPOSITION_DATA_TYPE                 double    

//[Heading] [ 0, 360 ]
#define RC_SIGNALSTACKBOARD_SIGNALSDIRECTION_DATA_ID                  16101     
#define RC_SIGNALSTACKBOARD_SIGNALSDIRECTION_DATA_COUNT               1         
#define RC_SIGNALSTACKBOARD_SIGNALSDIRECTION_DATA_TYPE                float     



///////////////////////////////////////////////////
////////////        COREBOARD           ///////////         
///////////////////////////////////////////////////

////////////////////Commands
//[LeftSpeed, RightSpeed] (-1, 1)-> (-100%, 100%)
#define RC_COREBOARD_DRIVELEFTRIGHT_DATA_ID                           17000     
#define RC_COREBOARD_DRIVELEFTRIGHT_DATA_COUNT                        2         
#define RC_COREBOARD_DRIVELEFTRIGHT_DATA_TYPE                         float     

//[LF, LM, LR, RF, RM, RR] (-1, 1)-> (-100%, 100%)
#define RC_COREBOARD_DRIVEINDIVIDUAL_DATA_ID                          17001     
#define RC_COREBOARD_DRIVEINDIVIDUAL_DATA_COUNT                       6         
#define RC_COREBOARD_DRIVEINDIVIDUAL_DATA_TYPE                        float     

//[0-Turn off Watchdog Override, 1-Turn on Watchdog Override]
#define RC_COREBOARD_WATCHDOGOVERRIDE_DATA_ID                         17002     
#define RC_COREBOARD_WATCHDOGOVERRIDE_DATA_COUNT                      1         
#define RC_COREBOARD_WATCHDOGOVERRIDE_DATA_TYPE                       uint8_t   

//[Tilt](degrees -180-180)
#define RC_COREBOARD_LEFTDRIVEGIMBALINCREMENT_DATA_ID                 17003     
#define RC_COREBOARD_LEFTDRIVEGIMBALINCREMENT_DATA_COUNT              1         
#define RC_COREBOARD_LEFTDRIVEGIMBALINCREMENT_DATA_TYPE               int16_t   

//[Tilt](degrees -180-180)
#define RC_COREBOARD_RIGHTDRIVEGIMBALINCREMENT_DATA_ID                17004     
#define RC_COREBOARD_RIGHTDRIVEGIMBALINCREMENT_DATA_COUNT             1         
#define RC_COREBOARD_RIGHTDRIVEGIMBALINCREMENT_DATA_TYPE              int16_t   

//[Pan, Tilt](degrees -180-180)
#define RC_COREBOARD_LEFTMAINGIMBALINCREMENT_DATA_ID                  17005     
#define RC_COREBOARD_LEFTMAINGIMBALINCREMENT_DATA_COUNT               2         
#define RC_COREBOARD_LEFTMAINGIMBALINCREMENT_DATA_TYPE                int16_t   

//[Pan, Tilt](degrees -180-180)
#define RC_COREBOARD_RIGHTMAINGIMBALINCREMENT_DATA_ID                 17006     
#define RC_COREBOARD_RIGHTMAINGIMBALINCREMENT_DATA_COUNT              2         
#define RC_COREBOARD_RIGHTMAINGIMBALINCREMENT_DATA_TYPE               int16_t   

//[Tilt](degrees -180-180)
#define RC_COREBOARD_BACKDRIVEGIMBALINCREMENT_DATA_ID                 17007     
#define RC_COREBOARD_BACKDRIVEGIMBALINCREMENT_DATA_COUNT              1         
#define RC_COREBOARD_BACKDRIVEGIMBALINCREMENT_DATA_TYPE               int16_t   

//
#define RC_COREBOARD_INITIATETESTROUTINE_DATA_ID                      17008     
#define RC_COREBOARD_INITIATETESTROUTINE_DATA_COUNT                   1         
#define RC_COREBOARD_INITIATETESTROUTINE_DATA_TYPE                    uint8_t   

//[R, G, B] (0, 255)
#define RC_COREBOARD_LEDRGB_DATA_ID                                   17009     
#define RC_COREBOARD_LEDRGB_DATA_COUNT                                3         
#define RC_COREBOARD_LEDRGB_DATA_TYPE                                 uint8_t   

//[Pattern] (Enum)
#define RC_COREBOARD_LEDPATTERNS_DATA_ID                              17010     
#define RC_COREBOARD_LEDPATTERNS_DATA_COUNT                           1         
#define RC_COREBOARD_LEDPATTERNS_DATA_TYPE                            uint8_t   

//[Teleop, Autonomy, Reached Goal] (enum)
#define RC_COREBOARD_STATEDISPLAY_DATA_ID                             17011     
#define RC_COREBOARD_STATEDISPLAY_DATA_COUNT                          1         
#define RC_COREBOARD_STATEDISPLAY_DATA_TYPE                           uint8_t   

//Set Brightness (0-255)
#define RC_COREBOARD_BRIGHTNESS_DATA_ID                               17012     
#define RC_COREBOARD_BRIGHTNESS_DATA_COUNT                            1         
#define RC_COREBOARD_BRIGHTNESS_DATA_TYPE                             uint8_t   

//0: Teleop, 1: Autonomy
#define RC_COREBOARD_SETWATCHDOGMODE_DATA_ID                          17013     
#define RC_COREBOARD_SETWATCHDOGMODE_DATA_COUNT                       1         
#define RC_COREBOARD_SETWATCHDOGMODE_DATA_TYPE                        uint8_t   

////////////////////Telemetry
//[LF, LM, LR, RF, RM, RR] (-1000, 1000)-> (-100%, 100%)
#define RC_COREBOARD_DRIVESPEEDS_DATA_ID                              17100     
#define RC_COREBOARD_DRIVESPEEDS_DATA_COUNT                           6         
#define RC_COREBOARD_DRIVESPEEDS_DATA_TYPE                            int16_t   

////////////////////Enums
enum COREBOARD_DISPLAYSTATE {TELEOP,AUTONOMY,REACHED_GOAL}; 
enum COREBOARD_PATTERNS {MRDT,BELGIUM,MERICA,DIRT,DOTA,MCD,WINDOWS}; 


#endif // RoveCommManifest_h