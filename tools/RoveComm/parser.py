import json
import sys
from datetime import date
from subprocess import call

# Configuration
rovecomm_version = 3
filename = "Manifest.h"
json_path = "../../data/RoveComm/manifest.json"
header_path = "../../src/RoveComm/" + filename
author = "Missouri S&T - Mars Rover Design Team"
organization = "Mars Rover Design Team"

# Maps types from json to struct types
type_to_struct = {
    "INT8_T"    : "DataTypes::INT8_T",
    "UINT8_T"   : "DataTypes::UINT8_T",
    "INT16_T"   : "DataTypes::INT16_T",
    "UINT16_T"  : "DataTypes::UINT16_T",
    "INT32_T"   : "DataTypes::INT32_T",
    "UINT32_T"  : "DataTypes::UINT32_T",
    "FLOAT_T"   : "DataTypes::FLOAT_T",
    "DOUBLE_T"  : "DataTypes::DOUBLE_T",
    "CHAR"      : "DataTypes::CHAR",
}

this = sys.modules[__name__]

this.manifest = None
this.manifest_file = None
this.header_file = None

def insert_address(board):
    """
    This inserts IP Address for board
    """
    if (len(this.manifest[board]["Ip"]) > 0):
        ip = this.manifest[board]["Ip"]
        ip_octs = ip.split(".")

        this.header_file.write(f"{generate_indent(2)}// IP Address\n")
        this.header_file.write(f"{generate_indent(2)}const AddressEntry IP_ADDRESS({ip_octs[0]}, {ip_octs[1]}, {ip_octs[2]}, {ip_octs[3]});\n")

        this.header_file.write("\n")

def insert_packets(board, type):
    """
    This inserts all Ids for a given type (Command, Telemetry, Error)
    Currently adds the comments, dataId, dataCount and dataType
    """
    if (len(this.manifest[board][type]) > 0):
        messages = this.manifest[board][type]

        if (type == "Commands"):
            this.header_file.write(f"{generate_indent(2)}// Commands\n")
        elif (type == "Telemetry"):
            this.header_file.write(f"{generate_indent(2)}// Telemetry\n")
        elif (type == "Error"):
            this.header_file.write(f"{generate_indent(2)}// Error\n")

        for message in messages:
            ### print(message)
            dataId = this.manifest[board][type][message]["dataId"]
            dataCount = this.manifest[board][type][message]["dataCount"]
            comments = this.manifest[board][type][message]["comments"]

            # Data type doesn't exactly match the struct type
            dataType = this.manifest[board][type][message]["dataType"]
            dataType = type_to_struct[dataType]


            this.header_file.write(f"{generate_indent(2)}const ManifestEntry {message.upper()}({dataId}, {dataCount}, {dataType});\n")

        this.header_file.write("\n")

def insert_enums(board):
    if ("Enums" in this.manifest[board]):
        enums = this.manifest[board]["Enums"]
        this.header_file.write(f"{generate_indent(2)}// Enums\n")
        for enum in enums:
            this.header_file.write(f"{generate_indent(2) + 'enum ' + enum.upper()}\n")
            this.header_file.write(f"{generate_indent(2) + '{'}\n")
            output = ""
            enum_len = len(enums[enum])
            enum_counter = 0

            for value in enums[enum]:
                if enum_counter < enum_len - 1:
                    output += f"{generate_indent(3) + value.upper() + ','}\n"
                else:
                    output += f"{generate_indent(3) + value.upper()}\n"

                enum_counter += 1

            this.header_file.write(f"{output + generate_indent(2) + '};'} \n\n")

def insert_includes():
    return "\n#ifndef MANIFEST_H\n#define MANIFEST_H\n\n#include <stdint.h>\n\nnamespace manifest\n{\n"

def insert_footer():
    return "}    // namespace manifest\n\n#endif    // MANIFEST_H\n"

def insert_datatypes_enum():
    return "    enum DataTypes\n    {\n        INT8_T,\n        UINT8_T,\n        INT16_T,\n        UINT16_T,\n        INT32_T,\n        UINT32_T,\n        FLOAT_T,\n        DOUBLE_T,\n        CHAR\n    };\n\n"

def insert_address_struct():
    return "    struct AddressEntry\n    {\n        public:\n            int FIRST_OCTET;\n            int SECOND_OCTET;\n            int THIRD_OCTET;\n            int FOURTH_OCTET;\n    };\n\n"

def insert_manifest_struct():
    return "    struct ManifestEntry\n    {\n        public:\n            int DATA_ID;\n            int DATA_COUNT;\n            DataTypes DATA_TYPE;\n    };\n\n"

def insert_general():
    this.update_rate = this.manifest_file["updateRate"]
    this.header_file.write(f"{generate_indent(2)}const int UPDATE_RATE            = {this.update_rate};\n")

    this.udp_port = this.manifest_file["ethernetUDPPort"]
    this.tcp_port = this.manifest_file["ethernetTCPPort"]
    this.header_file.write(f"{generate_indent(2)}const int ETHERNET_UDP_PORT      = {this.udp_port};\n")
    this.header_file.write(f"{generate_indent(2)}const int ETHERNET_TCP_PORT      = {this.tcp_port};\n")

    # Also grab the first 3 octets of the subnet IP
    # this.subnet_ip = this.manifest_file["subnetIP"]
    # this.header_file.write(f"{generate_indent(2)}const int SUBNET_IP_FIRST_OCTET  = {this.subnet_ip[0]};\n")
    # this.header_file.write(f"{generate_indent(2)}const int SUBNET_IP_SECOND_OCTET = {this.subnet_ip[1]};\n")
    # this.header_file.write(f"{generate_indent(2)}const int SUBNET_IP_THIRD_OCTET  = {this.subnet_ip[2]};\n")

    this.subnet_mac = this.manifest_file["MACaddressPrefix"]
    this.header_file.write(f"{generate_indent(2)}const int SUBNET_MAC_FIRST_BYTE  = {this.subnet_mac[0]};\n")
    this.header_file.write(f"{generate_indent(2)}const int SUBNET_MAC_SECOND_BYTE = {this.subnet_mac[1]};\n")

def insert_system():
    max_len = 0
    this.system_packets = this.manifest_file["SystemPackets"]

    for packet in this.system_packets:
        temp = "const int " + packet.upper() + "_DATA_ID"
        
        if len(temp) > max_len:
            max_len = len(temp)

    for packet in this.system_packets:
        temp = "const int " + packet.upper() + "_DATA_ID"

        temp += generate_spaces(max_len - len(temp))

        this.header_file.write(f"{generate_indent(2)}{temp} = {this.system_packets[packet]};\n")

def sanity_check(manifest):
    if manifest["ManifestSpecVersion"] != rovecomm_version:
        ### print("Expected Manifest Spec v" + str(rovecomm_version) + ", Aborting")
        exit()

def generate_file_header():
    output = []

    # Append Start Line
    output += "/******************************************************************************\n"

    # Append Brief
    output += [
        " * @brief RoveComm Manifest\n",
        " *\n",
        " *        NOTICE! This file is auto generated and will be overwritten if edited\n",
        " *                and committed. To make changes edit the manifest.json file or\n",
        " *                edit parser.py if it is a formatting issue.\n"
    ]

    # Append Blank Line
    output += " *\n"

    # Append Filename
    output += " * @file " + filename + "\n"
    
    # Append Author
    output += " * @author " + author + "\n"

    # Append Date
    output += " * @date " + date.today().strftime("%Y-%m-%d") + "\n"

    # Append Blank Line
    output += " *\n"

    # Append Copyright
    output += " * @copyright Copyright " + organization + " " + date.today().strftime("%Y") + " - All Rights Reserved\n"

    # Append End Line
    output += " ******************************************************************************/\n"

    # Return File Header
    return output

def generate_doxygen_block(brief):
    output = "/******************************************************************************\n"

    # Append Brief
    if len(brief) > 68:
        first = True
        temp_line = ""
        temp = brief.split(" ")

        for word in temp:
            if len(temp_line) + len(word) < 68:
                temp_line += word + " "
            else:
                if first:
                    output += " * @brief " + temp_line + "\n"
                    first = False
                    temp_line = ""
                else:
                    output += " *        " + temp_line + "\n"
                    temp_line = ""
    else:
        output += " * @brief " + brief + "\n"

    # Append Blank Line
    output += " *\n"

    # Append Author
    output += " * @author " + author + "\n"

    # Append Date
    output += " * @date " + date.today().strftime("%Y-%m-%d") + "\n"

    # Append End Line
    output += " ******************************************************************************/"

    # Return File Header
    return output

def generate_indent(num = 1):
    output = ""
    
    for i in range(num):
        output += generate_spaces(4)

    return output

def generate_spaces(num = 1):
    output = ""
    
    for i in range(num):
        output += " "

    return output

if __name__ == "__main__":
    ###############################
    ######### Set Up File #########
    ###############################
    
    # Load the json file
    this.manifest_file = open(json_path, "r").read()
    this.manifest_file = json.loads(this.manifest_file)

    # Check manifest spec version
    sanity_check(this.manifest_file)

    # Manifest contains additional info not necessary for header file
    this.manifest = this.manifest_file["RovecommManifest"]
    this.header_file = open(header_path, "w")

    ##############################
    ######## Write to File #######
    ##############################

    ## Add Header Doxygen Block
    for line in generate_file_header():
        this.header_file.write(line)

    ## Add Includes
    for line in insert_includes():
        this.header_file.write(line)

    ## Add DataTypes Enum
    lines_index = 0
    lines = generate_doxygen_block("Enumeration of Data Types to be used in RoveComm").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1
    for line in insert_datatypes_enum():
        this.header_file.write(line)

    ## Add AddressEntry Struct
    lines_index = 0
    lines = generate_doxygen_block("IP Address Object for RoveComm.").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1
    for line in insert_address_struct():
        this.header_file.write(line)

    ## Add ManifestEntry Struct
    lines_index = 0
    lines = generate_doxygen_block("Manifest Entry Object for RoveComm.").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1
    for line in insert_manifest_struct():
        this.header_file.write(line)

    ## Add Board Namespaces
    for board in this.manifest:

        # Add Doxgen Comment Block
        lines_index = 0
        lines = generate_doxygen_block(board + " Board IP Address, Commands, Telemetry, and Error Packet Information").split("\n")
        for line in lines:
            if lines_index < len(lines) - 1:
                this.header_file.write(generate_indent(1) + line + "\n")
            else:
                this.header_file.write(generate_indent(1) + line + "\n")

            lines_index += 1

        # Open Namespace
        this.header_file.write(f"{generate_indent(1)}namespace {board}\n")
        this.header_file.write(generate_indent(1) + "{\n")

        # Insert IP Octets
        insert_address(board)

        # Insert the commands, telemetry and error messages for this particular board
        insert_packets(board, "Commands")
        insert_packets(board, "Telemetry")
        insert_packets(board, "Error")
        insert_enums(board)

        # Close Namespace
        this.header_file.write(generate_indent(1) + "}" + generate_indent(1) + "// namespace " + board + "\n")

        # Write a newline to seperate boards
        this.header_file.write("\n")

    ## Add General Namspace
    lines_index = 0
    lines = generate_doxygen_block("RoveComm General Information").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1

    # Open Namespace
    this.header_file.write(f"{generate_indent(1)}namespace General\n")
    this.header_file.write(generate_indent(1) + "{\n")

    insert_general()

    # Close Namespace
    this.header_file.write(generate_indent(1) + "}" + generate_indent(1) + "// namespace General\n")

    # Write a newline to seperate boards
    this.header_file.write("\n")

    ## Add System Namespace
    lines_index = 0
    lines = generate_doxygen_block("RoveComm System Information").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1

    # Open Namespace
    this.header_file.write(f"{generate_indent(1)}namespace System\n")
    this.header_file.write(generate_indent(1) + "{\n")

    insert_system()

    # Close Namespace
    this.header_file.write(generate_indent(1) + "}" + generate_indent(1) + "// namespace System\n")

    # Write a newline to seperate boards
    this.header_file.write("\n")

    ## Add Footer
    for line in insert_footer():
        this.header_file.write(line)

    ## Close File
    this.header_file.close()
