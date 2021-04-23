package homework.ch11_13.p4;

import java.lang.reflect.InvocationTargetException;

public class CompositeComponent extends Component {
    /**
     * 保存子组件,放在 ComponentList 里
     */
    protected ComponentList childs = new ComponentList();

    /**
     * 缺省构造函数
     */
    public CompositeComponent() {
        super();
    }

    /**
     * 构造函数
     *
     * @param id    组件 id
     * @param name  组件名称
     * @param price 组件价格
     */
    public CompositeComponent(int id, String name, double price) {
        super(id, name, price);
    }

    @Override
    public double getPrice() {
        return this.calcPrice();
    }

    /**
     * 添加子组件，对于没有子组件的 AtomicComponent 如内存条，调用这个方法应该抛出
     * UnsupportedOperationException. 相同的子组件不能重复加入
     *
     * @param component
     * @throws UnsupportedOperationException
     */
    @Override
    public void add(Component component) throws UnsupportedOperationException {
        this.childs.add(component);
    }

    /**
     * 删除子组件，对于没有子组件的 AtomicComponent 如内存条，调用这个方法应该抛出
     * UnsupportedOperationException.
     *
     * @param component
     * @throws UnsupportedOperationException
     */
    @Override
    public void remove(Component component) throws UnsupportedOperationException {
        this.childs.remove(component);
    }

    /**
     * 计算组件的价格。对于复合组件应该计算其子组件的价格之和
     *
     * @return
     */
    @Override
    public double calcPrice() {
        int sum = 0;
        for (Component child : childs) {
            sum += child.getPrice();
        }
        return sum;
    }

    /**
     * 返回组件的迭代器
     *
     * @return 组件的迭代器
     */
    @Override
    public Iterator iterator() {
        return new CompositeIterator(this.childs);
    }

    @Override
    public String toString() {
        Component prevC=null;
        Component c=this;
        Iterator prevIt=null;
        Iterator it=this.iterator();
        StringBuffer s=new StringBuffer("");
        s.append(super.toString());
        for(Component child:childs){
            s.append(child.toString());
        }

        return s.toString();
    }
}
