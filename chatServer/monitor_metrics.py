import psutil
import subprocess
import time
import matplotlib.pyplot as plt
from datetime import datetime

# 获取CPU使用率
def get_cpu_usage():
    return psutil.cpu_percent(interval=1)

# 获取MySQL TPS（总事务数）
def get_mysql_tps():
    result = subprocess.run(
        ["mysql", "-u", "root", "-p123456", "-e", "SHOW GLOBAL STATUS LIKE 'Com_insert'"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    output = result.stdout.decode()
    insert_count = int(output.split("\n")[1].split()[1])
    return insert_count

# 获取Redis QPS
def get_redis_qps():
    result = subprocess.run(
        ["redis-cli", "info", "stats"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    output = result.stdout.decode()
    for line in output.split("\n"):
        if "instantaneous_ops_per_sec" in line:
            return float(line.split(":")[1])
    return 0

# 记录数据
cpu_data = []
mysql_data = []
redis_data = []
timestamps = []

# 定期获取数据并绘制图形
for _ in range(10):  # 获取100次数据样本
    timestamp = datetime.now().strftime('%H:%M:%S')
    cpu_usage = get_cpu_usage()
    mysql_tps = get_mysql_tps()
    redis_qps = get_redis_qps()

    cpu_data.append(cpu_usage)
    mysql_data.append(mysql_tps)
    redis_data.append(redis_qps)
    timestamps.append(timestamp)

    time.sleep(1)  # 每秒采样一次

# 绘制图表
plt.figure(figsize=(12, 6))

# CPU使用率图表
plt.subplot(3, 1, 1)
plt.plot(timestamps, cpu_data, label='CPU Usage (%)', color='tab:red')
plt.xlabel('Time')
plt.ylabel('CPU Usage (%)')
plt.title('CPU Usage over Time')
plt.xticks(rotation=45)
plt.tight_layout()

# MySQL TPS图表
plt.subplot(3, 1, 2)
plt.plot(timestamps, mysql_data, label='MySQL TPS', color='tab:blue')
plt.xlabel('Time')
plt.ylabel('MySQL TPS')
plt.title('MySQL TPS over Time')
plt.xticks(rotation=45)
plt.tight_layout()

# Redis QPS图表
plt.subplot(3, 1, 3)
plt.plot(timestamps, redis_data, label='Redis QPS', color='tab:green')
plt.xlabel('Time')
plt.ylabel('Redis QPS')
plt.title('Redis QPS over Time')
plt.xticks(rotation=45)
plt.tight_layout()

# 显示图表
plt.show()

