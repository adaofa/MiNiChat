#!/bin/bash

# 日志文件保存路径
LOG_FILE="monitor_$(date +%Y%m%d_%H%M%S).log"

# MySQL 用户名和密码（请根据实际修改）
MYSQL_USER="root"
MYSQL_PASS="123456"

# Redis 配置（默认端口6379）
REDIS_CLI="redis-cli"

# 每次采样间隔（单位：秒）
INTERVAL=1

# 采样次数（例如：60 表示采样 60 次，共持续 60 秒）
COUNT=60

echo "开始监控，日志输出到 $LOG_FILE"
echo "========== 监控开始时间：$(date) ==========" >> "$LOG_FILE"

for ((i = 1; i <= $COUNT; i++)); do
    echo "------ 第 $i 次采样：$(date) ------" >> "$LOG_FILE"

    # CPU 使用率
    echo "[CPU]" >> "$LOG_FILE"
    top -bn1 | grep "Cpu(s)" >> "$LOG_FILE"

    # MySQL TPS
    echo "[MySQL TPS]" >> "$LOG_FILE"
    mysqladmin -u$MYSQL_USER -p$MYSQL_PASS extended-status | grep -E "Com_insert|Com_update|Com_delete" >> "$LOG_FILE"

    # Redis QPS
    echo "[Redis QPS]" >> "$LOG_FILE"
    $REDIS_CLI info stats | grep instantaneous_ops_per_sec >> "$LOG_FILE"

    # TCP 连接状态
    echo "[TCP Connections]" >> "$LOG_FILE"
    netstat -nat | awk '{print $6}' | sort | uniq -c >> "$LOG_FILE"

    echo "" >> "$LOG_FILE"
    sleep $INTERVAL
done

echo "========== 监控结束时间：$(date) ==========" >> "$LOG_FILE"
echo "监控完成，日志保存在：$LOG_FILE"

