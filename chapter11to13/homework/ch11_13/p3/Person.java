package homework.ch11_13.p3;

public class Person implements Cloneable {
    private int age;
    private String name;

    /**
     * 缺省构造函数
     */
    public Person(){

    }

    /**
     * 构造函数
     * @param name
     * @param age
     */
    public Person(String name,int age){
        this.name=name;
        this.age=age;
    }

    public String getName(){
        return this.name;
    }

    public void setName(String name){
        this.name=name;
    }

    public int getAge(){
        return this.age;
    }

    public void setAge(int age){
        this.age=age;
    }

    @Override
    public String toString() {
        return "年龄:"+this.age+" 名字:"+this.name+"\n";
    }

    @Override
    public boolean equals(Object obj) {
        if(this==obj){
            return true;
        }
        if(obj instanceof Person){
            Person aobj=(Person)obj;
            if(this.name!=null && !this.name.equals(aobj.getName())){
                return false;
            }
            return this.age==aobj.getAge();
        }
        return false;
    }

    @Override
    public Object clone() throws CloneNotSupportedException {
        Person newObj=(Person)(super.clone());
        if(this.name!=null){
            newObj.setName(new String(this.name));
        }
        return newObj;
    }
}
