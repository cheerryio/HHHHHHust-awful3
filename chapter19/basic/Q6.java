package basic;

import java.util.ArrayList;
import java.util.List;

class A6 {}
class B6 extends A6 {}
class C6 extends B6 {}
class D6 extends C6 {}
class Test6 {
    public static <T> void m (List<? super T> list1, List<? extends T> list2) {}
}


public class Q6 {
    public static void main(String[] args){
        List<B6> l1_1 = new ArrayList<> ();
        List<B6> l1_2 = new ArrayList<>();
        Test6.m (l1_1, l1_2);

        List<B6> l2_1 = new ArrayList<> ();
        List<D6> l2_2 = new ArrayList<>();
        Test6.m (l2_1, l2_2);

        List<B6> l3_1 = new ArrayList<> ();
        List<A6> l3_2 = new ArrayList<>();
        //Test6.m (l3_1, l3_2);
    }
}
