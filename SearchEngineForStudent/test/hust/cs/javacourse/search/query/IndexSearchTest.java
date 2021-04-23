package hust.cs.javacourse.search.query;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.query.impl.SimpleSorter;
import hust.cs.javacourse.search.util.Config;
import org.junit.jupiter.api.Test;
import hust.cs.javacourse.search.query.impl.IndexSearcher;
import hust.cs.javacourse.search.index.IndexBuilderTest;
import java.nio.file.Path;

import java.nio.file.Paths;

/**
 * IndexSearch测试类
 */
public class IndexSearchTest {
    /**
     * 测试单个关键字的全文检索
     */
    @Test
    void testIndexSearchOne(){
        Sort sorter;
        AbstractHit[] hits;
        new IndexBuilderTest().testIndexBuilderWrite();
        IndexSearcher indexSearcher = new IndexSearcher();
        Path pathname = Paths.get(Config.INDEX_DIR, Config.INDEX_FILENAME);
        indexSearcher.open(pathname.toString());
        AbstractTerm term = new Term("according");

        sorter = new SimpleSorter();
        hits = indexSearcher.search(term, sorter);
        if(hits!=null){
            for (AbstractHit hit : hits) {
                System.out.println(hit);
            }
        }
    }

    /**
     * 测试两个搜索关键字的全文检索和短语检索
     */
    @Test
    void testIndexSearchTwo() {
        Sort sorter;
        AbstractHit[] hits;
        new IndexBuilderTest().testIndexBuilderWrite();
        IndexSearcher indexSearcher = new IndexSearcher();
        Path pathname = Paths.get(Config.INDEX_DIR, Config.INDEX_FILENAME);
        indexSearcher.open(pathname.toString());
        AbstractTerm term1 = new Term("health");
        AbstractTerm term2 = new Term("conditions");

        sorter = new SimpleSorter();
        hits = indexSearcher.search(term1, term2, sorter, AbstractIndexSearcher.LogicalCombination.ANDNEIGHBOR);
        if(hits!=null){
            for (AbstractHit hit : hits) {
                System.out.println(hit);
            }
        }
    }
}
