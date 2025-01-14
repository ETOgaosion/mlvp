import os
import re
import shutil

def open_file(path):
    if os.path.exists(path):
        dir = os.path.dirname(path)
        basename = os.path.basename(path)
        dst = dir + '/' + basename + '.bak'
        shutil.copyfile(path, dst)
    with open(path, 'w') as f:
        f.write('')

def generate_ports_info(module, file):
    # Combine module and file to create path
    path = 'include/MCVPack/BareDut/' + module + '/V' + file + '.h'

    # Read file and find lines matching the regex pattern
    ports_in_name = []
    ports_out_name = []
    ports_in_scale = []
    ports_out_scale = []
    
    skip_ports = ['clk', 'clock'];
    with open(path, 'r') as f:
        pattern_in = r'VL_IN(\d+)\(&(\w+),(\d+),0\);'
        pattern_out = r'VL_OUT(\d+)\(&(\w+),(\d+),0\);'
        for line in f:
            matches_in = re.findall(pattern_in, line)
            matches_out = re.findall(pattern_out, line)
            if matches_in:
                for match in matches_in:
                    if match[1] in skip_ports:
                        continue
                    if int(match[2]) > 63:
                        raise ValueError('Not support port width larger than 64, please split this port')
                    ports_in_name.append(match[1])
                    ports_in_scale.append(2 ** (int(match[2])+1) - 1)
            if matches_out:
                for match in matches_out:
                    if int(match[2]) > 63:
                        raise ValueError('Not support port width larger than 64, please split this port')
                    ports_out_name.append(match[1])
                    ports_out_scale.append(2 ** (int(match[2])+1) - 1)
        
    print(ports_in_name)
    print(ports_in_scale)
    print(ports_out_name)
    print(ports_out_scale)

    # include/Database/designPortsGen.h
    design_ports_path = 'include/Database/designPortsGen.h'
    open_file(design_ports_path)

    # Write the port names and lengths to the designPortsGen.h file
    with open(design_ports_path, 'w') as f:
        f.write('#define PORT_IN_INFO {')
        for i in range(len(ports_in_name)):
            f.write('{"' + ports_in_name[i] + '",' + str(ports_in_scale[i]) + '}')
            if i != len(ports_in_name) - 1:
                f.write(',')
        f.write('}\n')
        f.write('#define PORT_OUT_INFO {')
        for i in range(len(ports_out_name)):
            f.write('{"' + ports_out_name[i] + '",' + str(ports_out_scale[i]) + '}')
            if i != len(ports_out_name) - 1:
                f.write(',')
        f.write('}')


# Example usage
generate_ports_info('Memory', 'memory')
