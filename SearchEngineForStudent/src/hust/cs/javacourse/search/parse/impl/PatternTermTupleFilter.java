package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;

import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * AbstractTermTupleFilter装饰者实现
 */
public class PatternTermTupleFilter extends AbstractTermTupleFilter{
    /**
     * 分隔符正则
     */
    Pattern pattern=Pattern.compile(Config.TERM_FILTER_PATTERN);

    /**
     * 构造函数
     * @param input：单词输入流
     */
    public PatternTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
    }

    /**
     * 获得下一个三元组
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next(){
        java.util.function.Function<AbstractTermTuple,Boolean> filter=(termTuple)->{
            return !pattern.matcher(termTuple.term.getContent()).matches();
        };

        AbstractTermTuple termTuple = this.input.next();
        while(termTuple != null && filter.apply(termTuple)){
            termTuple=this.input.next();
        }
        return termTuple;
    }
}
