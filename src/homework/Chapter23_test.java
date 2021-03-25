package homework;

public class Chapter23_test {
    public static void main(String[] args) {
        int x = 0, y = 0;
        System.out.println( ((x > 1) && (++x == 0)) + " " + x);
        System.out.println( ((y < 1) | (y++ == 0)) + " " + y);
    }
}