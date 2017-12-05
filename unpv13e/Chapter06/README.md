## 第六章 I/O复用：select和poll函数

#### I/O模型

**1. 阻塞式I/O**


![block io image](doc/figure-6-1.png)

**2. 非阻塞式I/O**

![nonblock io image](doc/figure-6-2.png)

**3. I/O复用(select和poll)**

![synchronous io multiplexing image](doc/figure-6-3.png)

**4. 信号驱动式I/O(SIGIO)**

![signal io image](doc/figure-6-4.png)

**5. 异步I/O(POSIX的aio_系列函数)**

![aio image](doc/figure-6-5.png)

#### 5中I/O模型比较

![IO image](doc/figure-6-6.png)

图片来源：[UNP](https://www.amazon.cn/UNIX%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B-%E5%A5%97%E6%8E%A5%E5%AD%97%E8%81%94%E7%BD%91API-%E5%8F%B2%E8%92%82%E6%96%87%E6%96%AF/dp/B011S72JB6/ref=sr_1_3?ie=UTF8&qid=1512463174&sr=8-3&keywords=unix+network+programming)
