# Data Structure Homework

---

## Brief

读取csrc.c文件中内容并生成ast,再由ast生成源码

---

## Environment

操作系统:elementory OS

IDE: eclipse

---

## Compilation

- 安装zlog

- ```make```

---

## WARNING

**报错不完善**

**输出懒得弄到文件里面**

**可能有些符号会直接死...**

**懒得再去windows下编译**



简单梳理一下源码:

1. 内存分配由内存池pool实现,数据结构名pool_t，具体实现参照Nginx

2. 链表使用list_head,  我的数据结构名link_head,实现参照linux内核

3. 数组封装了array_t, 原理:初始化数组时传入待存储元素大小,比如存int传入sizeof(int)

   于是初始分配20*sizeof(int)大小空间(PS:20自己宏定义)。存数据memcpy数据过去，指针后移

4. string_t 是array_t typedef的,再用宏把关于string_t的函数封装一遍，并加入独特的关于string的函数

5. string2_t本质二维数组，元素是string_t类型，自己是array_t(本质就是array套array啦)

6. 输入输出使用zlog,并在gdef.h中宏定义的6个日志输出级别为 ZLOG_INFO,ZLOG_ERROR等等

   注意日志输出用宏定义 #define LOG_ON开启或关闭

7. ptree_t的树的结构，关注child 和 love成员。 小例子：妈妈有child和love成员，孩子们也有child和love成员，妈妈的child和孩子们的love用list_head连在一起，通过妈妈找到了孩子，也能通过孩子的love找到妈妈(不推荐)。孩子由通过自己的child和孙子的love连载一起，这就是一棵树啦

   

