public class Q13 {
    public static void main(String[] args){
        new B13().display();
    }
}
class A13{
    public void draw() {
        System.out.print("Draw A.");
    }
    public void display() {
        draw();
        System.out.print("Display A.");
    }
}
class B13 extends A13{
    public void draw() {
        System.out.print("Draw B.");
    }
    public void display() {
        super.display();
        System.out.print("Display B.");
    }
}
