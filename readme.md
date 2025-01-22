## Time:2025-1-2
## software based on FreeRTOS
Project have four tasks as listed below:
- TASK1：在任务开始后，判断Track链表长度,>=5后向云网上传飞机任务轨迹
- TASK2：在任务开始后，1HZ向Track链表中插入数据
- TASK3：接收4G模块下发的数据，根据前两位解析成CJson数据或者RTCM数据
- TASK4：解析飞控下发的mavlink消息，解析对应数据

## Hardware
- baseed on [STM32F407VGT6]
- Now is using USART1 for debug,USART2 for mavlink,USART3 for 4gcatmodule
- USART1 RX: pin A9  BaudRate 115200
- USART1 TX: pin A10
- USART2 TX: pin A2  BaudRate 230400
- USART2 TX: pin A3
- USART3 TX: pin B10 BaudRate 115200
- USART3 TX: pin B11 

## Note
- 使用时，重新配置4G模块到正常首发数据经测试需要30S时间。
- 需要配置飞控的mavlink参数
- 需要配置飞控mavlink的波特率

## bugs
Not found yet

## Time:2025-1-2
- 将获取飞控的SN码放在main函数中
- 将4g模块的初始化函数放到main函数中
- 将获取ID和时间戳放在StartTrackSendTask任务之前
