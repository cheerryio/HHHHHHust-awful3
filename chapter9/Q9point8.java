/**
 * 9.8 (风扇类 Fan)设计一个名为 Fan 的类来表示一个风扇。这个类包括：
 * • 三个名为 SLOW、MEDIUM 和 FAST 而值为 1、2 和 3 的常量，表示风扇的速度。
 * •
 * — 个名为 speed 的 int 类型私有数据域，表示风扇的速度（畎认值为 SLOW)。
 * •
 * — 个名为 on 的 boolean 类型私有数据域. 表示风扇是否打开（默认值为 false〉。
 * • 一个名为 radius 的 double 类型私有数据域，表示风扇的半径（默认值为 S )。
 * • 一个名为 color 的 string 类型数据域，表示风扇的颜色（默认值为 blue)。
 * • 这四个数据域的访问器和修改器。
 * • 一个创建默认风扇的无参构造方法。
 * • 一个名为 toStringO 的方法返回描述风扇的宇符串。如果风扇是打开的，那么该方法在一个
 * 组合的宇符串中返回风扇的速度、顔色和半径。如果风扇没有打开，该方法就会返回一个由
 * “ fan is off” 和风扇颜色及半径组合成的字符串。
 * 画出该类的 UML 图并实现这个类。编写一个测试程序，创建两个 Fan 对象。将第一个对象
 * 设置为最大速度、半径为 10、颜色为 yellow、状态为打开。将第二个对象设置为中等速度、半
 * 径为 S 、颜色为 blue、状态为关闭。通过调用它们的 toString 方法显示这些对象。
 */


public class Q9point8 {
    /**
     * 风扇转速
     * SLOW=1,MEDIUM=2,FAST=3
     */
    public static enum SPEED {
        SLOW,
        MEDIUM,
        FAST
    }

    /**
     * 风扇速度
     */
    private int speed=SPEED.SLOW.ordinal()+1;

    /**
     * 风扇启动
     */
    private boolean on=false;

    /**
     * 风扇半径
     */
    private double radius=10;

    /**
     * 风扇颜色
     */
    private String color="blue";

    /**
     * 缺省构造函数
     */
    public Q9point8(){

    }

    public int getSpeed(){
        return this.speed;
    }

    public void setSpeed(SPEED speed){
        this.speed=speed.ordinal()+1;
    }

    public boolean getOn(){
        return this.on;
    }

    public void setOn(boolean on){
        this.on=on;
    }

    public double getRadius(){
        return this.radius;
    }

    public void setRadius(double radius){
        this.radius=radius;
    }

    public String getColor(){
        return this.color;
    }

    public void setColor(String color){
        this.color=color;
    }

    @Override
    public String toString() {
        if(this.on){
            return "速度: "+this.speed+
                    ", 颜色: "+this.color+
                    ", 半径: "+this.radius;
        }else{
            return "fan is off"+
                    ", 颜色: "+this.color+
                    ", 半径: "+this.radius;
        }
    }

    public static void main(String[] args){
        Q9point8 fan1=new Q9point8();
        Q9point8 fan2=new Q9point8();

        fan1.setSpeed(SPEED.FAST);
        fan1.setRadius(10.0);
        fan1.setColor("yellow");
        fan1.setOn(true);

        fan2.setSpeed(SPEED.MEDIUM);
        fan2.setRadius(5.0);
        fan2.setColor("blue");
        fan2.setOn(false);

        System.out.println(fan1);
        System.out.println(fan2);
    }
}
