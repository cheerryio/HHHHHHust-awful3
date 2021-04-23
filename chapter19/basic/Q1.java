package basic;

import java.util.ArrayList;
import java.util.List;

class A<T> {
    T x;
    //static T y;
    //static T s1() {return null;}

    List<? extends Comparable<Double>> x8 = new ArrayList<Double>();
    static <T1> void s3(T1 i, T1 j){}
    T m1() {
        return null;
    }

}

public class Q1 {
}
