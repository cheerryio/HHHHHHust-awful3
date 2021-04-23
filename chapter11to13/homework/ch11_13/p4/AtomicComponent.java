package homework.ch11_13.p4;

public class AtomicComponent extends Component {
    /**
     * 缺省构造函数
     */
    public AtomicComponent() {
        super();
    }

    /**
     * 构造函数
     *
     * @param id    组件 id
     * @param name  组件名称
     * @param price 组件价格
     */
    public AtomicComponent(int id, String name, double price) {
        super(id, name, price);
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
        throw new UnsupportedOperationException();
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
        throw new UnsupportedOperationException();
    }

    /**
     * 计算组件的价格。对于复合组件应该计算其子组件的价格之和
     *
     * @return
     */
    @Override
    public double calcPrice() {
        return this.price;
    }

    /**
     * 返回组件的迭代器
     *
     * @return 组件的迭代器
     */
    @Override
    public Iterator iterator() {
        return new NullIterator();
    }
}
