import java.io.IOException;
import java.util.StringTokenizer;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.hadoop.io.Writable;


public class ScoreCount {
    /********** Begin **********/

    public static class CountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        private Text a = new Text();
        private IntWritable b = new IntWritable();
        @Override
        //在这重载map函数，直接将输入中的value复制到输出数据的key上 注意在map方法中要抛出异常：throws IOException,InterruptedException
        protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            /********** Begin **********/
            // 读取每行数据
            //按数据格式逗号分隔开
            StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
            while (itr.hasMoreTokens()) {
                StringTokenizer itr2 = new StringTokenizer(itr.nextToken(), ",");
                String className = itr2.nextToken();
                itr2.nextToken();
                int score = Integer.parseInt(itr2.nextToken());
                a.set(className);
                b.set(score);
                context.write(a, b);
            }

            //获取课程名称，字符串

            //获取每门课的分数，int类型

            //调用Context的write方法，key是课程名称是Text类型，value是分数，是IntWriteable类型

            /********** End **********/
        }
    }

    public static class CountReducer extends Reducer<Text, IntWritable, Text, Text> {
        private Text result = new Text();
        @Override
        //在这重载reduce函数，直接将输入中的key复制到输出数据的key上  注意在reduce方法上要抛出异常：throws IOException,InterruptedException
        protected void reduce(Text key, Iterable<IntWritable> values,
                              org.apache.hadoop.mapreduce.Reducer<Text, IntWritable, Text, Text>.Context context)
                throws IOException, InterruptedException {
            /********** Begin **********/
            
            //循环读出values中的值（分数），并在循环中统计总分数以及最大分数和最小分数
            int maxScore = 0, minScore = 100, sum = 0, count = 0;
            double avg = 0;
            for (IntWritable val : values) {
                maxScore = Math.max(maxScore, val.get());
                minScore = Math.min(minScore, val.get());
                sum += val.get();
                count += 1;
            }
            //循环外计算分数的平均值
            avg = (double) sum / count;
            String r = "max=" + maxScore + "\t" + "min=" + minScore + "\t" + "avg=" + avg;
            //按照上面的格式写出

            //调用conetxt的write方法，写出key和result；
            result.set(r);
            context.write(key, result);
            /********** End **********/
        }
    }

    // 主函数
    public static void main(String[] args) throws Exception {
        // 获取配置参数
        Configuration conf = new Configuration();
        // 定义作业对象
        Job job = new Job(conf, "FlowCount");
        // 注册分布式类
        job.setJarByClass(ScoreCount.class);
        // 注册Mapper类
        job.setMapperClass(CountMapper.class);
        // 注册Reducer类
        job.setReducerClass(CountReducer.class);
        // 注册输出格式类
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        //设置输入路径
        String inputfile = "/user/input";
        //设置输出路径
        String outputFile = "/user/output";
        // 设置输入输出路径
        FileInputFormat.setInputPaths(job, new Path(inputfile));
        FileOutputFormat.setOutputPath(job, new Path(outputFile));
        // 运行程序
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }


    /********** End **********/
}