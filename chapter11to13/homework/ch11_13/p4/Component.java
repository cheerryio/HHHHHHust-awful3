package homework.ch11_13.p4;

abstract public class Component {
    /**
     * 组件的唯一 id
     */
    protected int id;
    /**
     * 组件的名字
     */
    protected String name;
    /**
     * 组件的价格
     */
    protected double price;

    /**
     * 缺省构造函数
     */
    public Component(){

    }

    /**
     * 构造函数
     *
     * @param id 组件 id
     * @param name 组件名称
     * @param price 组件价格
     */
    public Component(int id,String name,double price){
        this.id=id;
        this.name=name;
        this.price=price;
    }

    public int getId(){
        return this.id;
    }

    public void setId(int id){
        this.id=id;
    }

    public String getName(){
        return this.name;
    }

    public void setName(String name){
        this.name=name;
    }

    public double getPrice(){
        return this.price;
    }

    public void setPrice(double price){
        this.price=price;
    }

    /**
     * 添加子组件，对于没有子组件的 AtomicComponent 如内存条，调用这个方法应该抛出
     * UnsupportedOperationException. 相同的子组件不能重复加入
     *
     * @param component
     * @throws UnsupportedOperationException
     */
    public abstract void add(Component component) throws UnsupportedOperationException;

    /**
     * 删除子组件，对于没有子组件的 AtomicComponent 如内存条，调用这个方法应该抛出
     * UnsupportedOperationException.
     *
     * @param component
     * @throws UnsupportedOperationException
     */
    public abstract void remove(Component component) throws UnsupportedOperationException;

    /**
     * 计算组件的价格。对于复合组件应该计算其子组件的价格之和
     *
     * @return
     */
    public abstract double calcPrice();

    /**
     * 返回组件的迭代器
     * @return 组件的迭代器
     */
    public abstract Iterator iterator();

    public boolean equals(Object obj){
        if(obj instanceof Component){
            Component aobj=(Component)obj;
            return this.id==aobj.getId();
        }
        return false;
    }

    public String toString(){
        return "Id:"+this.id+" 名字:"+this.name+" 价格:"+this.price+"\n";
    }
}
