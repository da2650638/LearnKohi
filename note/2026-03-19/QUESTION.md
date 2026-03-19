1.clock_gettime(CLOCK_MONOTONIC, &now); //now的数值通常是系统启动以来所经历的时间，并不是某个时间点
2.头文件互相包含，并且需要对方完整类型定义的情况下会导致编译不通过。