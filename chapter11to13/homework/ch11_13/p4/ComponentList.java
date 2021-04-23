package homework.ch11_13.p4;

import java.util.ArrayList;

public class ComponentList extends ArrayList<Component> implements Iterator {
    /**
     * 记录自定义迭代器当前迭代的位置
     */
    private int position=0;

    public ComponentList(){

    }

    /**
     * 是否还有元素
     *
     * @return 如果元素还没有迭代完，返回 true;否则返回 false
     */
    @Override
    public boolean hasNext() {
        if(this.position<this.size()){
            return true;
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
        if(this.hasNext()){
            Component c=this.get(this.position);
            this.position++;
            return c;
        }
        return null;
    }
}