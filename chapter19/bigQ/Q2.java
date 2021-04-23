package bigQ;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

class TwoTuple<T1 extends Comparable<T1>,T2 extends Comparable<T2>> implements Comparable<TwoTuple<T1, T2>> {

    private T1 first;
    private T2 second;

    /**
     * 缺省构造函数
     */
    public TwoTuple(){

    }

    public TwoTuple(T1 first,T2 second){
        this.first=first;
        this.second=second;
    }

    public T1 getFirst() {
        return first;
    }

    public void setFirst(T1 first) {
        this.first = first;
    }

    public T2 getSecond() {
        return second;
    }

    public void setSecond(T2 second) {
        this.second = second;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TwoTuple<?, ?> twoTuple = (TwoTuple<?, ?>) o;
        return Objects.equals(first, twoTuple.first) && Objects.equals(second, twoTuple.second);
    }

    @Override
    public int hashCode() {
        return Objects.hash(first, second);
    }

    @Override
    public String toString() {
        return "("+this.first.toString()+","+this.second.toString()+")";
    }

    @Override
    public int compareTo(TwoTuple<T1, T2> o) {
        if(!this.first.equals(o.getFirst())){
            return this.first.compareTo(o.getFirst());
        }
        return this.second.compareTo(o.getSecond());
    }
}

public class Q2 {
    public static void main(String[] args) {
        TwoTuple<Integer, String> twoTuple1 = new TwoTuple<>(1, "ccc");
        TwoTuple<Integer, String> twoTuple2 = new TwoTuple<>(1, "bbb");
        TwoTuple<Integer, String> twoTuple3 = new TwoTuple<>(1, "aaa");
        TwoTuple<Integer, String> twoTuple4 = new TwoTuple<>(2, "ccc");
        TwoTuple<Integer, String> twoTuple5 = new TwoTuple<>(2, "bbb");
        TwoTuple<Integer, String> twoTuple6 = new TwoTuple<>(2, "aaa");
        List<TwoTuple<Integer, String>> list = new ArrayList<>();
        list.add(twoTuple1);
        list.add(twoTuple2);
        list.add(twoTuple3);
        list.add(twoTuple4);
        list.add(twoTuple5);
        list.add(twoTuple6);
        //测试equals，contains方法是基于equals方法结果来判断
        TwoTuple<Integer, String> twoTuple10 = new TwoTuple<>(1, "ccc"); //内容=twoTuple1
        System.out.println(twoTuple1.equals(twoTuple10)+" ,supposed to be true"); //应该为true
        if (!list.contains(twoTuple10)) {
            list.add(twoTuple10);  //这时不应该重复加入
            System.out.println("shouldn't see this message");
        }
        Collections.sort(list);
        for (TwoTuple<Integer, String> t: list) {
            System.out.println(t);
        }
        TwoTuple<TwoTuple<Integer,String >,TwoTuple<Integer,String >> tt1 =
                new TwoTuple<>(new TwoTuple<>(1,"aaa"),new TwoTuple<>(1,"bbb"));
        TwoTuple<TwoTuple<Integer,String >,TwoTuple<Integer,String >> tt2 =
                new TwoTuple<>(new TwoTuple<>(1,"aaa"),new TwoTuple<>(2,"bbb"));
        System.out.println(tt1.compareTo(tt2)); //输出-1
        System.out.println(tt1);

    }
}
