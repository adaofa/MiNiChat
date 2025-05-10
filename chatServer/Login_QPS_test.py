import socket
import threading
import json
import time
import random
from concurrent.futures import ThreadPoolExecutor, as_completed
from tqdm import tqdm

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8000
PASSWORD = "abc"
CONCURRENCY = 2000 # 最大并发线程数
MAX_RETRIES = 2     # 每个请求最多重试次数
TIMEOUT = 3         # 单次连接超时时间（秒）

success_count = 0
lock = threading.Lock()
error_log = []

def send_login_request(user_id, password):
    global success_count
    for attempt in range(MAX_RETRIES + 1):
        try:
            s = socket.socket()
            s.settimeout(TIMEOUT)
            s.connect((SERVER_IP, SERVER_PORT))

            login_msg = json.dumps({
                "msgid": 1,
                "id": user_id,
                "password": password
            }) + "\n"

            s.sendall(login_msg.encode())
            response = s.recv(1024).decode()
            s.close()

            with lock:
                success_count += 1
            return
        except Exception as e:
            if attempt == MAX_RETRIES:
                with lock:
                    error_log.append(f"[ERROR] ID: {user_id}, 错误: {e}")

if __name__ == "__main__":
    start_time = time.time()

    user_ids = list(range(126982, 136982))  # 共 10000 个用户
    with ThreadPoolExecutor(max_workers=CONCURRENCY) as executor:
        futures = [executor.submit(send_login_request, uid, PASSWORD) for uid in user_ids]
        for _ in tqdm(as_completed(futures), total=len(futures), desc="登录中"):
            pass

    end_time = time.time()
    duration = end_time - start_time
    qps = success_count / duration

    print(f"\n✅ 登录成功数: {success_count}")
    print(f"⏱️ 总耗时: {duration:.2f} 秒")
    print(f"🚀 登录QPS: {qps:.2f}")

    if error_log:
        with open("login_errors.log", "w") as f:
            f.write("\n".join(error_log))
        print(f"❗ 失败记录已保存到 login_errors.log")

