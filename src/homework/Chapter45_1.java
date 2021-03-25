package homework;

public class Chapter45_1 {
    public static void main(String[] args){
        String s1="Welcome to Java";
        String s2=s1;
        String s3=new String("Welcome to Java");

        System.out.println("s1.indexOf(\"to\"): "+s1.indexOf("to"));
        System.out.println("s1.lastIndexOf(\"o\",15): "+s1.lastIndexOf("o",15));
        System.out.println("s1.substring(3,11): "+s1.substring(3,11));
        System.out.println("s1.startsWith(\"wel\"): "+s1.startsWith("wel"));
        System.out.println("s1.replace(\"o\",\"T\"): "+s1.replace("o","T"));
    }
}
