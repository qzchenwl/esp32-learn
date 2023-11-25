def wav_to_c_array(wav_path, c_array_path):
    # 读取wav文件中的音频数据
    with open(wav_path, 'rb') as wav_file:
        wav_data = wav_file.read()

    # 跳过WAV文件头（通常为44字节）
    wav_data = wav_data[44:]

    # 开始写入C数组
    with open(c_array_path, 'w') as c_file:
        c_file.write('const unsigned char sample[] = {\n    ')
        for i, byte in enumerate(wav_data):
            c_file.write('0x{:02x}, '.format(byte))
            if (i + 1) % 12 == 0:
                c_file.write('\n    ')
        c_file.write('\n};\n')

if __name__ == '__main__':
    import sys
    wav_to_c_array(sys.argv[1], sys.argv[2])
