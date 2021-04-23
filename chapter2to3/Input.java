import java.util.Scanner;

public class Input {
    public static void main(String[] args) {
        System.out.print("Please input your choice[1,2]:");
        long i = new Scanner(System.in).nextLong();
        Object o="hello";
        switch ((int)i) {
            case 1:
                System.out.println("Your choice is1 1");
                break;
            case 2:
                System.out.println("Your choice is 2");
                break;
            default:
                System.out.println("Wrong choice");

        }

    }
}
