package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.*;

import java.io.*;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

/**
 * AbstractIndex的具体实现类
 */
public class Index extends AbstractIndex {

    /**
     * 缺省构造函数
     */
    public Index() {
        super();
    }

    /**
     * 返回索引的字符串表示
     *
     * @return 索引的字符串表示
     */
    @Override
    public String toString() {
        StringBuffer s=new StringBuffer("");
        for (AbstractTerm term : this.termToPostingListMapping.keySet()) {
            AbstractPostingList postingList = this.termToPostingListMapping.get(term);
            s.append("单词: "+term + ": "+"出现文档信息: "+postingList+"\n");
        }
        return s.toString();
    }

    /**
     * 添加文档到索引，更新索引内部的HashMap
     * 1. 构造一个posting的hashMap
     * 2. 将对应term添加到termToPostingListMapping
     *
     * @param document ：文档的AbstractDocument子类型表示
     */
    @Override
    public void addDocument(AbstractDocument document) {
        AbstractPostingList postingList=null;
        if (document == null) {
            return;
        }
        int docId = document.getDocId();
        this.docIdToDocPathMapping.put(docId,document.getDocPath());
        Map<AbstractTerm, AbstractPosting> m = new HashMap<AbstractTerm, AbstractPosting>();
        for (int i = 0; i < document.getTupleSize(); i++) {
            AbstractTermTuple termTuple = document.getTuples().get(i);
            AbstractTerm term = termTuple.term;
            AbstractPosting posting=null;
            if (this.termToPostingListMapping.containsKey(term)) {
                // 此单词出现过
                postingList=this.termToPostingListMapping.get(term);
            } else {
                // 此单词未出现过
                postingList=new PostingList();
                this.termToPostingListMapping.put(term,postingList);
            }

            int index=postingList.indexOf(docId);
            if(index!=-1){
                // 此文档id在PostingList出现过
                posting=postingList.get(index);
                posting.setFreq(posting.getFreq()+1);
            }else{
                posting=new Posting();
                posting.setDocId(docId);
                posting.setFreq(1);
                postingList.add(posting);
            }
            posting.getPositions().add(termTuple.curPos);
        }
    }

    /**
     * <pre>
     * 从索引文件里加载已经构建好的索引.内部调用FileSerializable接口方法readObject即可
     * @param file ：索引文件
     * </pre>
     */
    @Override
    public void load(File file) {
        try{
            ObjectInputStream in=new ObjectInputStream(new FileInputStream(file));
            this.readObject(in);
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    /**
     * <pre>
     * 将在内存里构建好的索引写入到文件. 内部调用FileSerializable接口方法writeObject即可
     * @param file ：写入的目标索引文件
     * </pre>
     */
    @Override
    public void save(File file) {
        try{
            ObjectOutputStream out=new ObjectOutputStream(new FileOutputStream(file));
            this.writeObject(out);
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    /**
     * 返回指定单词的PostingList
     *
     * @param term : 指定的单词
     * @return ：指定单词的PostingList;如果索引字典没有该单词，则返回null
     */
    @Override
    public AbstractPostingList search(AbstractTerm term) {
        if (!this.termToPostingListMapping.containsKey(term)) {
            return null;
        }
        return this.termToPostingListMapping.get(term);
    }

    /**
     * 返回索引的字典.字典为索引里所有单词的并集
     *
     * @return ：索引中Term列表
     */
    @Override
    public Set<AbstractTerm> getDictionary() {
        return this.termToPostingListMapping.keySet();
    }

    /**
     * <pre>
     * 对索引进行优化，包括：
     *      对索引里每个单词的PostingList按docId从小到大排序
     *      同时对每个Posting里的positions从小到大排序
     * 在内存中把索引构建完后执行该方法
     * </pre>
     */
    @Override
    public void optimize() {
        for (AbstractTerm term : this.termToPostingListMapping.keySet()) {
            AbstractPostingList postingList = this.termToPostingListMapping.get(term);
            postingList.sort();
            for (int i = 0; i < postingList.size(); i++) {
                AbstractPosting posting = postingList.get(i);
                posting.sort();
            }
        }
    }

    /**
     * 根据docId获得对应文档的完全路径名
     *
     * @param docId ：文档id
     * @return : 对应文档的完全路径名
     */
    @Override
    public String getDocName(int docId) {
        if (!this.docIdToDocPathMapping.containsKey(docId)) {
            return "not exist";
        }
        return this.docIdToDocPathMapping.get(docId);
    }

    /**
     * 写到二进制文件
     *
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try{
            out.writeObject(docIdToDocPathMapping);
            out.writeObject(termToPostingListMapping);
        }catch (IOException e){
            e.printStackTrace();
        }

        return;
    }

    /**
     * 从二进制文件读
     *
     * @param in ：输入流对象
     */
    @Override
    public void readObject(ObjectInputStream in) {
        try{
            this.docIdToDocPathMapping=(Map<Integer, String>) (in.readObject());
            this.termToPostingListMapping=(Map<AbstractTerm, AbstractPostingList>) (in.readObject());
        }catch(IOException e){
            e.printStackTrace();
        }catch(ClassNotFoundException e){
            e.printStackTrace();
        }

    }
}