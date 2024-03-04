import json
import sys
from datetime import date
import subprocess

# Configuration
rovecomm_version = 3
filename = "RoveCommManifest.h"
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
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> COMMANDS = {{\n")

        elif (type == "Telemetry"):
            this.header_file.write(f"{generate_indent(2)}// Telemetry\n")
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> TELEMETRY = {{\n")
        elif (type == "Error"):
            this.header_file.write(f"{generate_indent(2)}// Error\n")
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> ERROR = {{\n")

        for message in messages:
            ### print(message)
            dataId = this.manifest[board][type][message]["dataId"]
            dataCount = this.manifest[board][type][message]["dataCount"]
            comments = this.manifest[board][type][message]["comments"]

            # Data type doesn't exactly match the struct type
            dataType = this.manifest[board][type][message]["dataType"]
            dataType = type_to_struct[dataType]


            this.header_file.write(f"{generate_indent(3)}{{\"{message.upper()}\", ManifestEntry({dataId}, {dataCount}, {dataType})}},\n")

        this.header_file.write(f"{generate_indent(2)}}};\n")

        if (type != "Error"):
            this.header_file.write("\n")
    else:
        if (type == "Commands"):
            this.header_file.write(f"{generate_indent(2)}// Commands\n")
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> COMMANDS = {{}};\n")

        elif (type == "Telemetry"):
            this.header_file.write(f"{generate_indent(2)}// Telemetry\n")
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> TELEMETRY = {{}};\n")
        elif (type == "Error"):
            this.header_file.write(f"{generate_indent(2)}// Error\n")
            this.header_file.write(f"{generate_indent(2)}const std::map<std::string, ManifestEntry> ERROR = {{}};\n")

def insert_enums(board):
    """
    This inserts all Enums for a given type
    """
    if ("Enums" in this.manifest[board]):
        enums = this.manifest[board]["Enums"]
        this.header_file.write(f"{generate_indent(2)}\n")
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
    """
    This inserts the guard block, all includes, and opens the manifest namespace
    """
    out = "\n"
    out += "#ifndef MANIFEST_H\n"
    out += "#define MANIFEST_H\n"
    out += "\n"
    out += "#include <map>\n"
    out += "#include <stdint.h>\n"
    out += "#include <string>\n"
    out += "\n"
    out += "namespace manifest\n"
    out += "{\n"

    return out

def insert_footer():
    """
    This inserts the closing bracket of the manifest namespace, and closes the guard block
    """
    out = "}    // namespace manifest\n"
    out += "\n"
    out += "#endif    // MANIFEST_H\n"

    return out

def insert_datatypes_enum():
    """
    This inserts the DataTypes enumeration
    """
    out = generate_indent(1) + "enum DataTypes\n"
    out += generate_indent(1) + "{\n"
    out += generate_indent(2) + "INT8_T,\n"
    out += generate_indent(2) + "UINT8_T,\n"
    out += generate_indent(2) + "INT16_T,\n"
    out += generate_indent(2) + "UINT16_T,\n"
    out += generate_indent(2) + "INT32_T,\n"
    out += generate_indent(2) + "UINT32_T,\n"
    out += generate_indent(2) + "FLOAT_T,\n"
    out += generate_indent(2) + "DOUBLE_T,\n"
    out += generate_indent(2) + "CHAR\n"
    out += generate_indent(1) + "};\n"
    out += "\n"

    return out

def insert_address_struct():
    """
    This inserts the AddressEntry struct
    """
    out = generate_indent(1) + "struct AddressEntry\n"
    out += generate_indent(1) + "{\n"
    out += generate_indent(2) + "public:\n"
    out += generate_indent(3) + "int FIRST_OCTET;\n"
    out += generate_indent(3) + "int SECOND_OCTET;\n"
    out += generate_indent(3) + "int THIRD_OCTET;\n"
    out += generate_indent(3) + "int FOURTH_OCTET;\n"
    out += generate_indent(3) + "std::string IP_STR = std::to_string(FIRST_OCTET) + \".\" + std::to_string(SECOND_OCTET) + \".\" + std::to_string(THIRD_OCTET) + \".\" + std::to_string(FOURTH_OCTET);\n"
    out += generate_indent(1) + "};\n"
    out += "\n"

    return out

def insert_manifest_struct():
    """
    This inserts the ManifestEntry struct
    """
    out = generate_indent(1) + "struct ManifestEntry\n"
    out += generate_indent(1) + "{\n"
    out += generate_indent(2) + "public:\n"
    out += generate_indent(3) + "int DATA_ID;\n"
    out += generate_indent(3) + "int DATA_COUNT;\n"
    out += generate_indent(3) + "DataTypes DATA_TYPE;\n"
    out += generate_indent(1) + "};\n"
    out += "\n"

    return out

def insert_general():
    """
    This inserts the General Information that needs to be included in RoveComm
    """
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
    """
    This inserts the System Information that needs to be included in RoveComm
    """
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

def find_board_and_data_id(json_file):
    results = set()
    with open(json_file) as f:
        data = json.load(f)

    for board_name, component in data['RovecommManifest'].items():
        if 'Commands' in component:
            for command in component['Commands'].values():
                data_id = command['dataId'] // 1000
                results.add((board_name, data_id))
        elif 'Telemetry' in component:
            for telemetry in component['Telemetry'].values():
                data_id = telemetry['dataId'] // 1000
                results.add((board_name, data_id))
        elif 'Error' in component:
            for error in component['Error'].values():
                data_id = error['dataId'] // 1000
                results.add((board_name, data_id))

    return sorted(results, key=lambda x: x[1])

def insert_helpers():
    """
    This inserts the Helper Information that needs to be included in RoveComm
    """

    # GetDataTypeFromMap function
    this.header_file.write(f"{generate_indent(2)}inline DataTypes GetDataTypeFromMap(const std::map<std::string, ManifestEntry>& dataMap, uint16_t dataId)\n")
    this.header_file.write(f"{generate_indent(2)}{{\n")
    this.header_file.write(f"{generate_indent(3)}for (const auto& entry : dataMap)\n")
    this.header_file.write(f"{generate_indent(3)}{{\n")
    this.header_file.write(f"{generate_indent(4)}if (entry.second.DATA_ID == dataId)\n")
    this.header_file.write(f"{generate_indent(4)}{{\n")
    this.header_file.write(f"{generate_indent(5)}return entry.second.DATA_TYPE;\n")
    this.header_file.write(f"{generate_indent(4)}}}\n")
    this.header_file.write(f"{generate_indent(3)}}}\n")
    this.header_file.write(f"{generate_indent(3)}return DataTypes::CHAR;{generate_indent(1)}// Default return value if dataId not found\n")
    this.header_file.write(f"{generate_indent(2)}}}\n")

    # GetDataTypeFromMap function
    this.header_file.write(f"{generate_indent(2)}\n")
    this.header_file.write(f"{generate_indent(2)}inline DataTypes GetDataTypeFromId(uint16_t dataId)\n")
    this.header_file.write(f"{generate_indent(2)}{{\n")
    this.header_file.write(f"{generate_indent(3)}int boardId      = dataId / 1000;          // Determine board ID based on thousands place\n")
    this.header_file.write(f"{generate_indent(3)}int dataTypeCode = (dataId / 100) % 10;    // Determine data type code based on hundreds place\n")
    this.header_file.write(f"{generate_indent(3)}\n")
    this.header_file.write(f"{generate_indent(3)}// Determine the board namespace based on boardId\n")
    this.header_file.write(f"{generate_indent(3)}switch (boardId)\n")
    this.header_file.write(f"{generate_indent(3)}{{\n")
    board_and_data_ids = find_board_and_data_id("../../data/RoveComm/manifest.json")
    for board_name, data_id in board_and_data_ids:
        this.header_file.write(f"{generate_indent(4)}case {data_id}:{generate_indent(1)}// {board_name} Board\n")
        this.header_file.write(f"{generate_indent(5)}if (dataTypeCode == 0)\n")
        this.header_file.write(f"{generate_indent(5)}{{\n")
        this.header_file.write(f"{generate_indent(6)}return GetDataTypeFromMap({board_name}::COMMANDS, dataId);\n")
        this.header_file.write(f"{generate_indent(5)}}}\n")
        this.header_file.write(f"{generate_indent(5)}else if (dataTypeCode == 1)\n")
        this.header_file.write(f"{generate_indent(5)}{{\n")
        this.header_file.write(f"{generate_indent(6)}return GetDataTypeFromMap({board_name}::TELEMETRY, dataId);\n")
        this.header_file.write(f"{generate_indent(5)}}}\n")
        this.header_file.write(f"{generate_indent(5)}else if (dataTypeCode == 2)\n")
        this.header_file.write(f"{generate_indent(5)}{{\n")
        this.header_file.write(f"{generate_indent(6)}return GetDataTypeFromMap({board_name}::ERROR, dataId);\n")
        this.header_file.write(f"{generate_indent(5)}}}\n")
        this.header_file.write(f"{generate_indent(5)}break;\n")
    this.header_file.write(f"{generate_indent(4)}default:\n")
    this.header_file.write(f"{generate_indent(5)}// Invalid Board ID\n")
    this.header_file.write(f"{generate_indent(5)}break;\n")
    this.header_file.write(f"{generate_indent(3)}}}\n")
    this.header_file.write(f"{generate_indent(3)}\n")
    this.header_file.write(f"{generate_indent(3)}// If dataId is not found in any namespace, return a default type\n")
    this.header_file.write(f"{generate_indent(3)}return DataTypes::CHAR;\n")
    this.header_file.write(f"{generate_indent(2)}}}\n")

def sanity_check(manifest):
    """
    This checks that we are trying to parse the correct version of RoveComm
    """
    if manifest["ManifestSpecVersion"] != rovecomm_version:
        print("Expected Manifest Spec v" + str(rovecomm_version) + ", Aborting")
        exit()

def generate_file_header():
    """
    This adds a doxygen file header to the top of the manifest header
    """
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
    """
    This adds a doxygen comment block with custom brief
    """
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
    """
    Indents by adding spaces
    """
    output = ""
    
    for i in range(num):
        output += generate_spaces(4)

    return output

def generate_spaces(num = 1):
    """
    Adds the appropriate number of spaces
    """
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

    ## Add Helpers Namespace
    this.header_file.write(f"{generate_indent(2)}\n")
    lines_index = 0
    lines = generate_doxygen_block("RoveComm Helper Functions").split("\n")
    for line in lines:
        if lines_index < len(lines) - 1:
            this.header_file.write(generate_indent(1) + line + "\n")
        else:
            this.header_file.write(generate_indent(1) + line + "\n")

        lines_index += 1

    # Open Namespace
    this.header_file.write(f"{generate_indent(1)}namespace Helpers\n")
    this.header_file.write(generate_indent(1) + "{\n")

    insert_helpers()

    # Close Namespace
    this.header_file.write(generate_indent(1) + "}" + generate_indent(1) + "// namespace Helpers\n")

    # Write a newline to seperate boards
    this.header_file.write("\n")

    # Add Footer
    for line in insert_footer():
        this.header_file.write(line)
