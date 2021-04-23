package homework.ch11_13.p4;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Test {
    public static void main(String args[]){
        Component computer = ComponentFactory.create();
        Iterator it=computer.iterator();
        System.out.println(computer);
    }
}
