package hust.cs.javacourse.search.index;

import hust.cs.javacourse.search.index.impl.DocumentBuilder;
import hust.cs.javacourse.search.index.AbstractDocument;
import org.junit.jupiter.api.Test;

import java.io.File;

/**
 * DocumentBuilder测试类
 */
public class DocumentBuilderTest {
    @Test
    void testDocumentBuilder() {
        File file = new File("text/2.txt");
        DocumentBuilder documentBuilder = new DocumentBuilder();
        AbstractDocument document = documentBuilder.build(1, file.getAbsolutePath(), file);
        System.out.println(document);
    }
}
