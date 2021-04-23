package homework.ch11_13.p4;

import java.util.ArrayList;
import java.util.List;

public class CompositeIterator implements Iterator {
    /**
     * 保存遍历到的每个节点的迭代器的列表
     */
    protected List<Iterator> iterators=new ArrayList<Iterator>();

    /**
     * 构造函数
     *
     * @param iterator  要迭代的组件树的根节点的迭代器
     */
    public CompositeIterator(Iterator iterator){
        this.iterators.add(iterator);
    }

    /**
     * 是否还有元素
     *
     * @return 如果元素还没有迭代完，返回 true;否则返回 false
     */
    @Override
    public boolean hasNext() {
        if(this.iterators.size()<=0){
            return false;
        }
        while(this.iterators.size()>0){
            Iterator it=this.iterators.get(0);
            if(it.hasNext()){
                return true;
            }else{
                this.iterators.remove(it);
            }
        }
        return false;
    }

    /**
     * 获取下一个组件
     *
     * @return 下一个组件
     */
    @Override
    public Component next() {
        Component c=this.iterators.get(0).next();
        if(c instanceof CompositeComponent){
            this.iterators.add(c.iterator());
        }
        return c;
    }
}
