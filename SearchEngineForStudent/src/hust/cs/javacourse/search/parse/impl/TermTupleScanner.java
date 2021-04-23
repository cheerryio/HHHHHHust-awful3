package hust.cs.javacourse.search.parse.impl;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Locale;
import java.util.Scanner;

import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.index.impl.TermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleScanner;

import hust.cs.javacourse.search.util.Config;

import java.util.regex.*;

/**
 * AbstractTermTupleScanner子类实现
 */
public class TermTupleScanner extends AbstractTermTupleScanner {

    Pattern spliter = Pattern.compile(Config.STRING_SPLITTER_REGEX);
    int curcurpos = 0;

    /**
     * 缺省构造函数
     */
    public TermTupleScanner() {
        super();
    }

    /**
     * 构造函数
     *
     * @param input：指定输入流对象，应该关联到一个文本文件
     */
    public TermTupleScanner(BufferedReader input) {
        super(input);
    }

    /**
     * 获得下一个三元组
     *
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        try {
            int c = this.input.read();
            Matcher spliterm = this.spliter.matcher(String.valueOf((char) c));
            String s = "";

            // 忽略前方所有空白字符
            while (spliterm.find()) {
                c = this.input.read();
                spliterm = this.spliter.matcher(String.valueOf((char) c));
            }

            while (c != -1 && !spliterm.find()) {
                s += (char) c;
                c = this.input.read();
                spliterm = this.spliter.matcher(String.valueOf((char) c));
            }

            if(s.length() > 0) {
                // 是否忽略大小写
                if (Config.IGNORE_CASE) {
                    s = s.toLowerCase(Locale.ROOT);
                }
                AbstractTerm term = new Term(s);
                AbstractTermTuple termTuple = new TermTuple(term, curcurpos++);
                return termTuple;
            }

            return null;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
