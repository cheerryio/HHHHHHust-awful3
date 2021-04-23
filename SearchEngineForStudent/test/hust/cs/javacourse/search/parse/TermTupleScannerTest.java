package hust.cs.javacourse.search.parse;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import org.junit.jupiter.api.Test;
import hust.cs.javacourse.search.parse.AbstractTermTupleScanner;
import hust.cs.javacourse.search.parse.impl.TermTupleScanner;

import java.io.*;
import java.util.Scanner;

/**
 * TermTupleScanner测试类
 */
public class TermTupleScannerTest {
    @Test
    void testTermTupleScanner(){
        String filePath="text/1.txt";
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(new File(filePath))));
            AbstractTermTupleScanner termTupleScanner=new TermTupleScanner(reader);
            AbstractTermTuple termTuple=termTupleScanner.next();
        }catch(IOException e){
            e.printStackTrace();
        }

    }
}
