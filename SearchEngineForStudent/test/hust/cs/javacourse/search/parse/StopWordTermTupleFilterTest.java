package hust.cs.javacourse.search.parse;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.impl.TermTupleScanner;
import org.junit.jupiter.api.Test;

import hust.cs.javacourse.search.parse.impl.StopWordTermTupleFilter;

import java.io.*;

/**
 * StopWordTermTupleFiler测试类
 */
public class StopWordTermTupleFilterTest {
    @Test
    void testTermTupleFilter(){
        String filePath="text/1.txt";
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(new File(filePath))));
            AbstractTermTupleScanner termTupleScanner=new TermTupleScanner(reader);
            AbstractTermTupleFilter termTupleFilter=new StopWordTermTupleFilter(termTupleScanner);
            AbstractTermTuple termTuple =termTupleFilter.next();
        }catch(IOException e){
            e.printStackTrace();
        }
    }
}
