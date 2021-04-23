package homework.ch11_13.p4;

/**
 * 组件的对象工厂, 由对象工厂返回对象
 */
public class ComponentFactory {
    public ComponentFactory(){

    }

    /**
     * 创建组件对象, 把创建对象的复杂步骤封装在 create 方法里
     *
     * @return 创建好的一台计算机
     */
    public static Component create(){
        int id = 0;
        String aName1 = "aaa1";
        String aName2 = "aaa2";
        String aName3 = "aaa3";
        String aName4 = "aaa4";
        double aPrice1 = 5.0D;
        double aPrice2 = 10.0D;
        double aPrice3 = 15.0D;
        double aPrice4 = 25.0D;
        int var26 = id + 1;
        Component a1 = new AtomicComponent(id, aName1, aPrice1);
        Component a2 = new AtomicComponent(var26++, aName2, aPrice2);
        Component a3 = new AtomicComponent(var26++, aName3, aPrice3);
        Component a4 = new AtomicComponent(var26++, aName4, aPrice4);
        String cName1 = "ccc1";
        String cName2 = "ccc2";
        String cName3 = "ccc3";
        Component c1 = new CompositeComponent(var26++, cName1, 10.0D);
        Component c2 = new CompositeComponent(var26++, cName2, 10.0D);
        String tName = "root";
        Component root = new CompositeComponent(var26++, tName, 100.0D);

        c1.add(a1);
        c1.add(a2);
        c1.add(c2);
        c2.add(a3);
        c2.add(a4);
        root.add(c1);
        return root;
    }

}
