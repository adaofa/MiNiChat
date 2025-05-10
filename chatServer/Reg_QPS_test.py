import socket
import threading
import json
import time
import uuid

def generate_uuid():
    return str(uuid.uuid4())

def send_register_request(user_name, password):
    try:
        s = socket.socket()
        s.connect(("127.0.0.1", 8000))

        register_msg = json.dumps({
            "msgid": 4,
            "name": user_name,
            "password": password
        }) + "\n"

        s.sendall(register_msg.encode())
        response = s.recv(1024).decode()
        print(f"[REGISTERED] 用户名: {user_name}, 响应: {response}")
        s.close()
    except Exception as e:
        print("通信失败：", e)

def worker():
    user_name = generate_uuid()
    send_register_request(user_name, "abc")

# 启动并发线程
threads = []
start = time.time()

for _ in range(10000):  # 你机器上 10000 太多可能会卡死或拒绝连接
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)

# 等待所有线程完成
for t in threads:
    t.join()

end = time.time()
total_requests = len(threads)
qps = total_requests / (end - start)
print(f"总请求数: {total_requests}, 耗时: {end - start:.2f}s, QPS: {qps:.2f}")
