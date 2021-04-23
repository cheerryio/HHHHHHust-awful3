class Base {
    public Base(){
        System.out.println("C");
    }

    public Base(String s) {
        System.out.print("B");
    }
}

class Q8 extends Base {
    public Q8(String s) {
        System.out.print("D");
    }
    public static void main(String [] args) {
        new Q8("C");
    }
}

