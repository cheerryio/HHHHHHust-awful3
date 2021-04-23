package hust.cs.javacourse.search.index;

import hust.cs.javacourse.search.index.impl.Term;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Term测试类
 */
public class TermTest {
    /**
     * 测试主类
     */
    @Test
    void testTerm(){
        String termString="termString";
        String termString2="termString2";
        Term term1=new Term(termString);
        Term term2=new Term(termString);
        System.out.println(term1+"   "+term2);
        assertTrue(term1.equals(term2));
        term1.setContent(termString2);
        assertFalse(term1.equals(term2));
        assertTrue(termString2.equals(term1.getContent()));
        System.out.println(term1.compareTo(term2));
    }
}
