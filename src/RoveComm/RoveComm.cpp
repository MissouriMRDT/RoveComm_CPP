/******************************************************************************
 * @brief Main implementation for RoveComm C++
 *
 * @file RoveComm.hpp
 * @author Viktor Butkovich (vsbbkf@umsystem.edu)
 * @date 2023-09-28
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveComm.h"
#include "Consts.h"
#include "string"
#include <map>

RoveCommPacket::RoveCommPacket(int data_id = 0, char data_type = 'b', int data[], std::string ip = "", int port = ROVECOMM_UDP_PORT)
{
    this->data_id   = data_id;
    this->data_type = data_type;
    int data_count  = std::size(data);
    this->data      = data;
    // IP should be the full IP address
    //  in case of empty IP default to unknown IP
    if (ip != "")
    {
        ip_address = (ip, port);
    }
    else
    {
        ip_address = ("0.0.0.0", 0);
    }

    return;
    /*
        self.data_id = data_id
        self.data_type = data_type
        self.data_count = len(data)
        self.data = data
        # If the ip is less than 4 chars we know it's the 4th octet, otherwise it's the full IP
        # in case of empty IP default to unknow IP
        if ip_octet_4 != "" and len(ip_octet_4) < 4:
            self.ip_address = ("192.168.1." + ip_octet_4, port)
        elif ip_octet_4 != "" and len(ip_octet_4) >= 4:
            self.ip_address = (ip_octet_4, port)
        else:
            self.ip_address = ("0.0.0.0", 0)
        return
    */
}

std::map<std::string, std::string> get_manifest(std::string path = "")
{
    /*
    Grabs the json manifest file and returns it in map form

    Parameters:
    -----------
        path - the path to a specified manifest file. If left blank we default
        to manifest found in this repo
    Returns:
    --------
        manifest - the manifest in dictionary form
    */
    std::map<std::string, std::string> manifest;

    return (manifest);
}
