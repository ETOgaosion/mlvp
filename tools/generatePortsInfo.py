import os
import re

def generate_ports_info(module, flle):
    # Combine module and file to create path
    path = 'include/MCVPack/BareDut/' + module + '/V' + flle + '.h'

    # Read file and find lines matching the regex pattern
    ports_in_name = []
    ports_out_name = []
    ports_in_len = []
    ports_out_len = []
    with open(path, 'r') as f:
        pattern_in = r'VL_IN(\d+)\(&(\w+),(\d+),0\);'
        pattern_out = r'VL_OUT(\d+)\(&(\w+),(\d+),0\);'
        for line in f:
            matches_in = re.findall(pattern_in, line)
            matches_out = re.findall(pattern_out, line)
            if matches_in:
                for match in matches_in:
                    ports_in_name.append(match[1])
                    ports_in_len.append(int(match[2])+1)
            if matches_out:
                for match in matches_out:
                    ports_out_name.append(match[1])
                    ports_out_len.append(int(match[2])+1)
        
    print(ports_in_name)
    print(ports_in_len)
    print(ports_out_name)
    print(ports_out_len)

    # Create the designPortsGen.h file if it doesn't exist
    design_ports_path = 'include/Database/designPortsGen.h'
    if not os.path.exists(design_ports_path):
        with open(design_ports_path, 'w') as f:
            f.write('')

    # Write the port names and lengths to the designPortsGen.h file
    with open(design_ports_path, 'w') as f:
        f.write('#define PORT_IN_INFO {')
        for i in range(len(ports_in_name)):
            f.write('{"' + ports_in_name[i] + '",' + str(ports_in_len[i]) + '}')
            if i != len(ports_in_name) - 1:
                f.write(',')
        f.write('}\n')
        f.write('#define PORT_OUT_INFO {')
        for i in range(len(ports_out_name)):
            f.write('{"' + ports_out_name[i] + '",' + str(ports_out_len[i]) + '}')
            if i != len(ports_out_name) - 1:
                f.write(',')
        f.write('}')

# Example usage
generate_ports_info('Memory', 'memory')
