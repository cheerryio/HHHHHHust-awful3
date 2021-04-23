package hust.cs.javacourse.search.index;

import hust.cs.javacourse.search.index.impl.Posting;
import hust.cs.javacourse.search.index.AbstractPosting;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Posting测试类
 */
public class PostingTest {
    @Test
    void testPosting(){
        int docId=1;
        int freq=1;
        List<Integer> positions=new ArrayList<Integer>(Arrays.asList(1,5,2,4,5));
        Posting posting1=new Posting(docId,freq,positions);
        System.out.println(posting1);
        System.out.println("getDocId: "+posting1.getDocId());
        System.out.println("getFreq: "+posting1.getFreq());
        System.out.println("get positions: "+posting1.getPositions());
        posting1.sort();
        System.out.println("after sort positions: "+posting1.getPositions());
    }
}
