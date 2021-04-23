package basic;

import java.util.ArrayList;

class GeometricObject {};
class PolyGon extends GeometricObject {};
class Rectangle extends PolyGon {};

public class Q2 {
    public static void main(String[] args){
        GeometricObject o = new Rectangle ();
        Class clz1 = o. getClass ();

        System.out.println(o.getClass().getSimpleName());

        ArrayList<String> lists1 = new ArrayList();
        ArrayList lists2 = new ArrayList<String>();
    }
}
