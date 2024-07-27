# Define the struct format to unpack the entire byte array
main_struct_format = '<2H1I16sf8s36s4s16s'  # Little-endian: 2 unsigned 16-bit ints, 1 unsigned 32-bit int, 8-byte string, float, 40-byte string, 4-byte string, 16-byte string

louver_format = '<6s10s4H3f'
wind_format = '<2H'
gnss_format = '<3f4s'