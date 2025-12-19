import serial
import time
import struct
import os


# ================== 配置区 ==================
BLOCK_SIZE = 256              # 必须和 MCU 的 FONT_PROGRAMMER_BUFFER_SIZE 一致
SERIAL_TIMEOUT = 3            # 串口读超时（秒）
POST_ACK_DELAY = 0.002        # 每个块 ACK 后延时（秒，2ms）
# ===========================================


def send_font_data(port, baudrate, filename):
    try:
        if not os.path.exists(filename):
            print(f"Error: File '{filename}' not found")
            return

        ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=SERIAL_TIMEOUT,
            write_timeout=SERIAL_TIMEOUT
        )

        print(f"Connected to {port} at {baudrate} baud")

        # ---------------- 等待 MCU READY ----------------
        print("Waiting for MCU ready signal...")
        start_time = time.time()
        ready_received = False

        while time.time() - start_time < 5:
            if ser.in_waiting:
                line = ser.readline().decode("ascii", errors="ignore").strip()
                if line == "READY":
                    print("MCU is ready")
                    ready_received = True
                    break

        if not ready_received:
            print("Timeout: No READY signal received from MCU")
            ser.close()
            return

        # ---------------- 读取文件 ----------------
        with open(filename, "rb") as f:
            data = f.read()

        length = len(data)
        print(f"File size: {length} bytes")

        # ---------------- 发送长度 ----------------
        ser.write(struct.pack(">I", length))
        ser.flush()

        print("Waiting for length ACK...")
        ack = ser.read(1)
        if ack != b"A":
            print(f"Failed to get length ACK: {ack}")
            ser.close()
            return

        print("Length ACK received")

        print("Waiting for erase completion...")
        start_time = time.time()
        erase_done = False

        while time.time() - start_time < 10:   # 擦 Flash 给 10 秒
            if ser.in_waiting:
                line = ser.readline().decode("ascii", errors="ignore").strip()
                if line == "ERASE_DONE":
                    print("MCU flash erase completed")
                    erase_done = True
                    break

        if not erase_done:
            print("Timeout: Flash erase not completed")
            ser.close()
            return


        # ---------------- 发送数据 ----------------
        total_blocks = (length + BLOCK_SIZE - 1) // BLOCK_SIZE
        print(f"Starting transmission ({total_blocks} blocks)...")

        sent_bytes = 0

        for block_index in range(total_blocks):
            start = block_index * BLOCK_SIZE
            end = min(start + BLOCK_SIZE, length)
            block = data[start:end]

            ser.write(block)
            ser.flush()

            # 等待 ACK
            ack = ser.read(1)
            if ack != b"A":
                print(f"Failed to get block ACK at block {block_index}: {ack}")
                ser.close()
                return

            sent_bytes = end

            # 显示进度
            if block_index % 10 == 0 or sent_bytes == length:
                progress = sent_bytes * 100 // length
                print(f"Progress: {progress}% ({sent_bytes}/{length} bytes)")

            # 给 MCU 一点喘息时间（非常重要）
            time.sleep(POST_ACK_DELAY)

        # ---------------- 等待 DONE ----------------
        print("Waiting for completion signal...")
        start_time = time.time()
        done_received = False

        while time.time() - start_time < 5:
            if ser.in_waiting:
                line = ser.readline().decode("ascii", errors="ignore").strip()
                if "DONE" in line:
                    print("MCU: Font programming completed!")
                    done_received = True
                    break
                elif line:
                    print(f"MCU: {line}")

        if done_received:
            print("=== 烧录成功 ===")
        else:
            print("Warning: No DONE signal received (data may still be valid)")

        ser.close()

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except Exception as e:
        print(f"Error: {e}")


# ================== 主入口 ==================
if __name__ == "__main__":
    PORT = "COM7"               # 修改为你的串口
    BAUDRATE = 115200           # 强烈推荐 115200
    FILENAME = "CH_Font.bin"    # 字体文件

    print("=== 字体烧录工具 ===")
    print(f"端口: {PORT}")
    print(f"波特率: {BAUDRATE}")
    print(f"文件: {FILENAME}")
    print("")

    send_font_data(PORT, BAUDRATE, FILENAME)
