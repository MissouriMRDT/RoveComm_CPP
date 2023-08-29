import json
import sys
from datetime import date
from subprocess import call

expected_rovecomm_version = 3

define_prefix = "#define"
indent_prefix = "    "
double_indent = "        "
manifest_prefix = "const ManifestEntry"
ip_octet1_prefix = "const int FIRSTOCTET  = "
ip_octet2_prefix = "const int SECONDOCTET = "
ip_octet3_prefix = "const int THIRDOCTET  = "
ip_octet4_prefix = "const int FOURTHOCTET = "
ip_comment = "// IP Address"
commands_comment = "// Commands"
telemetry_comment = "// Telemetry"
error_comment = "// Error"
namespace_comment = "// namespace "
today = date.today()
header = [
    "/******************************************************************************\n",
    " * @brief RoveComm Manifest\n",
    " *\n",
    " *        NOTICE! This file is auto generated and will be overwritten if edited\n",
    " *                and committed. To make changes edit the manifest.json file or\n",
    " *                edit parser.py if it is a formatting issue.\n",
    " *\n",
    " * @file Manifest.h\n",
    " * @author Missouri S&T - Mars Rover Design Team\n",
    " * @date <date>\n",
    " *\n",
    " * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved\n",
    " ******************************************************************************/\n",
    "\n",
    "#ifndef MANIFEST_H\n",
    "#define MANIFEST_H\n",
    "\n",
    "#include <stdint.h>\n",
    "\n",
    "namespace manifest\n",
    "{\n",
    "    enum DataTypes\n",
    "    {\n",
    "        INT8_T,\n",
    "        UINT8_T,\n",
    "        INT16_T,\n",
    "        UINT16_T,\n",
    "        INT32_T,\n",
    "        UINT32_T,\n",
    "        FLOAT_T,\n",
    "        DOUBLE_T,\n",
    "        CHAR\n",
    "   };\n",
    "\n",
    "   struct ManifestEntry\n",
    "   {\n",
    "        int DATA_ID;\n",
    "        int DATA_COUNT;\n",
    "        DataTypes DATA_TYPE;\n",
    "   };\n"
]

doxgen_block = [
    "/******************************************************************************\n",
    " * @brief <msg>\n",
    " *\n",
    " *\n",
    " * @author Missouri S&T - Mars Rover Design Team\n",
    " * @date <date>\n",
    " ******************************************************************************/\n"
]

footer = [
    "}\n",
    "\n",
    "#endif    // MANIFEST_H\n",
    "\n"
]

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

        # The < character indicates something is left aligned, in this case we are assuming that the name
        # plus #define is less than or equal to 50 characters and the PORT/IP less than 10
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_FIRSTOCTET':<50}{ip_octs[0]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_SECONDOCTET':<50}{ip_octs[1]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_THIRDOCTET':<50}{ip_octs[2]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_FOURTHOCTET':<50}{ip_octs[3]:<10}\n"
        )
        this.header_file.write("\n")


def insert_packets(board, type):
    """
    This inserts all Ids for a given type (Command, Telemetry, Error)
    Currently adds the comments, dataId, dataCount and dataType
    """
    if (len(this.manifest[board][type]) > 0):
        messages = this.manifest[board][type]

        if (type == "Commands"):
            this.header_file.write(f"{double_indent}{commands_comment}\n")
        elif (type == "Telemetry"):
            this.header_file.write(f"{double_indent}{telemetry_comment}\n")
        elif (type == "Error"):
            this.header_file.write(f"{double_indent}{error_comment}\n")

        for message in messages:
            print(message)
            dataId = this.manifest[board][type][message]["dataId"]
            dataCount = this.manifest[board][type][message]["dataCount"]
            comments = this.manifest[board][type][message]["comments"]

            # Data type doesn't exactly match the struct type
            dataType = this.manifest[board][type][message]["dataType"]
            dataType = type_to_struct[dataType]


            this.header_file.write(f"{double_indent}{manifest_prefix} {message.upper()}({dataId}, {dataCount}, {dataType});\n")

            # this.header_file.write(f"//{comments}\n")
            # this.header_file.write(
            #     f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_'+message.upper()+'_DATA_ID':<70}{dataId:<10}\n"
            # )
            # this.header_file.write(
            #     f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_'+message.upper()+'_DATA_COUNT':<70}{dataCount:<10}\n"
            # )
            # this.header_file.write(
            #     f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_'+message.upper()+'_DATA_TYPE':<70}{dataType:<10}\n"
            # )
        this.header_file.write("\n")

def insert_enums(board):
    if ("Enums" in this.manifest[board]):
        enums = this.manifest[board]["Enums"]
        this.header_file.write(f"////////////////////Enums\n")
        for enum in enums:
            this.header_file.write(f"{'enum ' + board.upper() + 'BOARD' + '_' + enum.upper() + ' {'}")
            output = ""
            for value in enums[enum]:
                print(value)
                output += f"{value.upper() + ','}"
            output = output[:-1] #Removing "," from the last element of the enum 
            this.header_file.write(f"{output + '};'} \n")

if __name__ == "__main__":
    # Load the json file
    this.manifest_file = open("../../data/RoveComm/manifest.json", "r").read()
    this.manifest_file = json.loads(this.manifest_file)

    # Check manifest spec version
    if this.manifest_file["ManifestSpecVersion"] != expected_rovecomm_version:
        print("Expected Manifest Spec v" + str(expected_rovecomm_version) + ", Aborting")
        exit()

    # Manifest contains additional info not necessary for header file
    this.manifest = this.manifest_file["RovecommManifest"]
    this.header_file = open("../../src/RoveComm/ManifestTemp.h", "w")

    for line in header:
        if "@date" in line:
            line = " * @date " + today.strftime("%Y-%m-%d" + "\n")
        this.header_file.write(line)

    # Write all the Ips and Ports together
    for board in this.manifest:
        ip = this.manifest[board]["Ip"]
        ip_octs = ip.split(".")

        # The < character indicates something is left aligned, in this case we are assuming that the name
        # plus #define is less than or equal to 50 characters and the PORT/IP less than 10
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_FIRSTOCTET':<50}{ip_octs[0]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_SECONDOCTET':<50}{ip_octs[1]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_THIRDOCTET':<50}{ip_octs[2]:<10}\n"
        )
        this.header_file.write(
            f"{define_prefix + ' RC_'+board.upper()+'BOARD'+'_FOURTHOCTET':<50}{ip_octs[3]:<10}\n"
        )
        this.header_file.write("\n")

    # A couple of newlines between IP assignments and rovecomm messages
    this.header_file.write("\n\n")

    # Insert the update rate and UDP port
    this.update_rate = this.manifest_file["updateRate"]
    this.header_file.write(f"{define_prefix + ' ROVECOMM_UPDATE_RATE':<50}{this.update_rate:<10}\n")

    this.udp_port = this.manifest_file["ethernetUDPPort"]
    this.tcp_port = this.manifest_file["ethernetTCPPort"]
    this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_ETHERNET_UDP_PORT':<50}{this.udp_port:<10}\n")
    this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_ETHERNET_TCP_PORT':<50}{this.tcp_port:<10}\n")

    # Also grab the first 3 octets of the subnet IP
    # this.subnet_ip = this.manifest_file["subnetIP"]
    # this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_SUBNET_IP_FIRST_OCTET':<50}{this.subnet_ip[0]:<10}\n")
    # this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_SUBNET_IP_SECOND_OCTET':<50}{this.subnet_ip[1]:<10}\n")
    # this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_SUBNET_IP_THIRD_OCTET':<50}{this.subnet_ip[2]:<10}\n")

    this.header_file.write("\n\n")

    # Grabs the first 5 bytes of the MAC address
    this.subnet_mac = this.manifest_file["MACaddressPrefix"]
    this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_SUBNET_MAC_FIRST_BYTE':<50}{this.subnet_mac[0]:<10}\n")
    this.header_file.write(f"{define_prefix + ' RC_ROVECOMM_SUBNET_MAC_SECOND_BYTE':<50}{this.subnet_mac[1]:<10}\n")

    this.header_file.write("\n\n")

    # First insert the reserved System Id's
    this.system_packets = this.manifest_file["SystemPackets"]
    this.header_file.write(f"///////////////////////////////////////////////////\n")
    this.header_file.write(f"{'////////////':<20}{'System Packets':<20}{'///////////':<20}\n")
    this.header_file.write(f"///////////////////////////////////////////////////\n\n")

    for packet in this.system_packets:
        this.header_file.write(
            f"{define_prefix + ' RC_ROVECOMM_'+packet.upper()+'_DATA_ID':<50}{this.system_packets[packet]:<10}\n"
        )
    this.header_file.write("\n\n")

    # Now go through and write all the commands and telemetry
    for board in this.manifest:
        # Add Doxgen Comment Block
        for line in doxgen_block:
            if "<msg>" in line:
                line = " * @brief " + board + " Board IP Address, Commands, Telemetry, and Error Packet Information\n"
            if "<date>" in line:
                line = " * @date " + today.strftime("%Y-%m-%d" + "\n")
            this.header_file.write(indent_prefix + line)

        # Open Namespace
        this.header_file.write(f"{indent_prefix}namespace {board}\n")
        this.header_file.write(indent_prefix + "{\n")

        # Insert IP Octets
        

        # Insert the commands, telemetry and error messages for this particular board
        insert_packets(board, "Commands")
        insert_packets(board, "Telemetry")
        insert_packets(board, "Error")
        insert_enums(board)

        # Close Namespace
        this.header_file.write(indent_prefix + "}" + indent_prefix + namespace_comment + board + "\n")

        # Write a couple of newlines to seperate boards
        this.header_file.write("\n")

    this.header_file.write("#endif    // MANIFEST_H\n")
    this.header_file.close()

    # Run Clang Format
    lc = ["clang-format-18","../../src/RoveComm/ManifestTemp.h"]
    retcode=call(lc)
    sys.exit(retcode)
