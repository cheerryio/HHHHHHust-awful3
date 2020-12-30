# word count

## 多线程
python的多线程，使用class继承threading.Thread，需要在class里重写run函数作为线程的主要执行过程。调用class的start方法显式开始线程，调用join方法阻塞等待线程返回

## Map
### map
将每一个单词映射成 (word,1)，并将映射完成后的数组排序，便于combine

### combine
对连续的 (word,1),(word,1),(word,1)，combine成 (word,3)

### shuffle
用哈希函数将特定首字母开头的单词hash到一组(一种方式)

## Reduce
### reduce
从分布式客户机拉去对应hash映射文件，将同一word的count值相加