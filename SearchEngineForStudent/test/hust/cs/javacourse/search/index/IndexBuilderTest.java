package hust.cs.javacourse.search.index;

import hust.cs.javacourse.search.index.impl.Index;
import hust.cs.javacourse.search.index.impl.IndexBuilder;
import hust.cs.javacourse.search.index.impl.DocumentBuilder;
import hust.cs.javacourse.search.util.Config;
import org.junit.jupiter.api.Test;
import java.io.*;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * IndexBuilder测试类
 */
public class IndexBuilderTest {
    /**
     * 测试函数
     */
    @Test
    public void testIndexBuilderWrite(){
        DocumentBuilder documentBuilder=new DocumentBuilder();
        IndexBuilder indexBuilder=new IndexBuilder(documentBuilder);
        AbstractIndex index= indexBuilder.buildIndex(Config.DOC_DIR);
        index.optimize();

        // 写到文件
        Path pathname= Paths.get(Config.INDEX_DIR,Config.INDEX_FILENAME);
        try{
            ObjectOutputStream out=new ObjectOutputStream(new FileOutputStream(pathname.toString()));
            index.writeObject(out);
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    @Test
    public void testIndexBuilderRead(){
        AbstractIndex index=new Index();
        Path pathname= Paths.get(Config.INDEX_DIR,Config.INDEX_FILENAME);
        //读入文件
        try{
            ObjectInputStream in=new ObjectInputStream((new FileInputStream(pathname.toString())));
            index.readObject(in);
            System.out.println(index);
        }catch (IOException e){
            e.printStackTrace();
        }
        System.out.println(index);
    }

}
