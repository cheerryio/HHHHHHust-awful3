package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.index.impl.TermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;

import hust.cs.javacourse.search.parse.AbstractTermTupleStream;

import java.util.*;

import hust.cs.javacourse.search.util.StopWords;

/**
 * AbstractTermTupleFilter装饰者实现
 */
public class StopWordTermTupleFilter extends AbstractTermTupleFilter{
    /**
     * 存放所有停用词的List
     */
    public List<String> stopwordList=null;

    /**
     * 构造函数
     * @param input：Filter的输入，类型为AbstractTermTupleStream
     */
    public StopWordTermTupleFilter(AbstractTermTupleStream input){
        super(input);
        this.stopwordList=new ArrayList<String>(Arrays.asList(StopWords.STOP_WORDS));
    }

    /**
     * 获得下一个三元组
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next(){
        java.util.function.Function<AbstractTermTuple,Boolean> filter=(termTuple)->{
            if(this.stopwordList.contains(termTuple.term.getContent().toLowerCase(Locale.ROOT))){
                return true;
            }
            return false;
        };

        AbstractTermTuple termTuple = this.input.next();
        while(termTuple != null && filter.apply(termTuple)){
            termTuple=this.input.next();
        }
        return termTuple;
    }
}
