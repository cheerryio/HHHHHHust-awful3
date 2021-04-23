package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;

import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * AbstractIndexSearcher子类实现
 */
public class IndexSearcher extends AbstractIndexSearcher {
    /**
     * 从指定索引文件打开索引，加载到index对象里. 一定要先打开索引，才能执行search方法
     *
     * @param indexFile ：指定索引文件
     */
    @Override
    public void open(String indexFile) {
        try {
            ObjectInputStream in = new ObjectInputStream((new FileInputStream(indexFile)));
            this.index.readObject(in);
        } catch (Exception e) {
            System.err.println(e);
        }
        return;
    }

    /**
     * 根据单个检索词进行搜索
     *
     * @param queryTerm ：检索词
     * @param sorter    ：排序器
     * @return ：命中结果数组
     */
    @Override
    public AbstractHit[] search(AbstractTerm queryTerm, Sort sorter) {
        AbstractPostingList postingList = this.index.search(queryTerm);
        if (postingList == null) {
            return null;
        }
        List<AbstractHit> abstractHits = new ArrayList<AbstractHit>();
        for (int i = 0; i < postingList.size(); i++) {
            AbstractPosting posting = postingList.get(i);
            int docId = posting.getDocId();
            String docPath = this.index.getDocName(docId);
            Map<AbstractTerm, AbstractPosting> m = new HashMap<AbstractTerm, AbstractPosting>();
            m.put(queryTerm, posting);
            AbstractHit hit = new Hit(docId, docPath, m);
            sorter.score(hit);
            abstractHits.add(hit);
        }

        sorter.sort(abstractHits);
        AbstractHit[] r = new AbstractHit[abstractHits.size()];
        abstractHits.toArray(r);
        return r;
    }

    /**
     * 根据二个检索词进行搜索
     * 实现思路：
     * 1. 先拿到两个term的PostingList，对两个PostingList里面的所有Posting创建AbstractHit
     * 2. 因为对一个docId不能重复，所以用map。map的key是docId，value是map<AbstractTerm,AbstractPosting>
     * 3. 最后集中创建List<AbstractHit>
     * 4. 调用score+sort
     * 5. 返回
     *
     * @param queryTerm1 ：第1个检索词
     * @param queryTerm2 ：第2个检索词
     * @param sorter     ：    排序器
     * @param combine    ：   多个检索词的逻辑组合方式
     * @return ：命中结果数组
     */
    @Override
    public AbstractHit[] search(AbstractTerm queryTerm1, AbstractTerm queryTerm2, Sort sorter, AbstractIndexSearcher.LogicalCombination combine) {
        Map<Integer, Map<AbstractTerm, AbstractPosting>> sMap = new HashMap<Integer, Map<AbstractTerm, AbstractPosting>>();
        if (this.index.search(queryTerm1) == null && combine == LogicalCombination.AND) {
            return null;
        }
        if (this.index.search(queryTerm2) == null && combine == LogicalCombination.AND) {
            return null;
        }

        java.util.function.BiConsumer<AbstractTerm, Map<Integer, Map<AbstractTerm, AbstractPosting>>> buildsMap = (queryTerm, m) -> {
            AbstractPostingList postingList = this.index.search(queryTerm);
            for (int i = 0; i < postingList.size(); i++) {
                AbstractPosting posting = postingList.get(i);
                int docId = posting.getDocId();
                if (m.containsKey(docId)) {
                    Map<AbstractTerm, AbstractPosting> termPostingMap = m.get(docId);
                    termPostingMap.put(queryTerm, posting);
                } else {
                    Map<AbstractTerm, AbstractPosting> termPostingMap = new HashMap<AbstractTerm, AbstractPosting>();
                    termPostingMap.put(queryTerm, posting);
                    m.put(docId, termPostingMap);
                }
            }
        };
        if (this.index.search(queryTerm1) != null) {
            buildsMap.accept(queryTerm1, sMap);
        }
        if (this.index.search(queryTerm2) != null) {
            buildsMap.accept(queryTerm2, sMap);
        }

        List<AbstractHit> hits = new ArrayList<AbstractHit>();
        for (int docId : sMap.keySet()) {
            Map<AbstractTerm, AbstractPosting> termPostingMap = sMap.get(docId);
            switch (combine) {
                case AND:
                    if (termPostingMap.containsKey(queryTerm1) && termPostingMap.containsKey(queryTerm2)) {

                    } else {
                        continue;
                    }
                    break;
                case OR:
                    break;
                case ANDNEIGHBOR:
                    // 判断多个检索词都在文档中出现
                    if (termPostingMap.containsKey(queryTerm1) && termPostingMap.containsKey(queryTerm2)) {

                    } else {
                        continue;
                    }

                    AbstractPosting posting1 = termPostingMap.get(queryTerm1);
                    AbstractPosting posting2 = termPostingMap.get(queryTerm2);
                    List<Integer> positions1 = posting1.getPositions();
                    List<Integer> positions2 = posting2.getPositions();
                    java.util.function.BiPredicate<List<Integer>, List<Integer>> hasOneDistance = (p1, p2) -> {
                        int l1 = p1.size();
                        int l2 = p2.size();
                        while (l1 != 0 && l2 != 0) {
                            if (p1.get(l1-1) > p2.get(l2-1)) {
                                if (p1.get(l1-1) - p2.get(l2-1) == 1) {
                                    return true;
                                }
                                l1--;
                            }else{
                                if(p2.get(l2-1) - p1.get(l1-1) == 1){
                                    return true;
                                }
                                l2--;
                            }
                        }
                        return false;
                    };
                    if(hasOneDistance.test(positions1,positions2)){

                    }else{
                        continue;
                    }
                    break;
                default:
                    continue;
            }

            // 该docId检索有效，需要返回给用户，开始创建Hit对象
            String docPath = this.index.getDocName(docId);
            AbstractHit hit = new Hit(docId, docPath, termPostingMap);
            sorter.score(hit);
            hits.add(hit);
        }
        sorter.sort(hits);
        AbstractHit[] hitArray = new AbstractHit[hits.size()];
        hits.toArray(hitArray);

        return hitArray;
    }
}
