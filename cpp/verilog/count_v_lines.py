import os

def count_lines_in_v_files(directory):
    total_lines = 0

    # 遍历指定目录及其子目录
    for root, dirs, files in os.walk(directory):
        for file in files:
            # 只处理 .v 文件
            if file.endswith(".v" ) or file.endswith(".vh"):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        lines = f.readlines()
                        total_lines += len(lines)
                        print(f"File: {file_path} - Lines: {len(lines)}")
                except Exception as e:
                    print(f"Could not read file {file_path}: {e}")
    
    return total_lines

if __name__ == "__main__":
    # 指定需要统计的文件夹路径
    directory = input("Enter the directory path: ")

    # 调用函数统计行数
    total_lines = count_lines_in_v_files(directory)
    print(f"\nTotal lines in all .v files: {total_lines}")
